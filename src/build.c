#include "fs.h"
#include "frontmatter.h"
#include "template.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>

int render_page(const char *temp_path, const char *md_path,
                const char *html_path) {
  int result = -1;
  char *temp = NULL;
  FILE *md = NULL;
  FILE *html = NULL;
  FrontMatter fm;

  // read template into a string
  if ((temp = read_file(temp_path)) == NULL)
    goto cleanup;

  // open files
  md = fopen(md_path, "rb");
  if (md == NULL) {
    perror("Error opening .md file");
    goto cleanup;
  }

  if ((parse_fm(md, &fm)) != 0) {
    fprintf(stderr, "Error parsing FM: %s.\n", md_path);
    goto cleanup;
  }

  html = fopen(html_path, "wb");
  if (html == NULL) {
    perror("Error opening .html file");
    goto cleanup;
  }

  if(fill_temp(html, temp, md, &fm) == -1) {
    fprintf(stderr, "Error filling in template.");
    goto cleanup;
  }

  result = 0;

cleanup:
  if (md != NULL)
    fclose(md);
  if (html != NULL) {
    if (fclose(html) != 0) {
      perror(html_path);
      result = -1;
    }
  }
  free(temp);

  return result;
}

int build_content(const char *src, const char *dst, const char *temp) {
  DIR *sd;
  struct dirent *entry;
  struct stat st;

  // create dst, it's fine if it already exists
  if (mkdir(dst, 0777) != 0 && errno != EEXIST) {
    perror(dst);
    return -1;
  }

  sd = opendir(src);
  if (sd == NULL) {
    perror(src);
    return -1;
  }

  while ((entry = readdir(sd)) != NULL) {

    // skip current and parent directory
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    // create path of src and dst via helper function
    char src_path[PATH_MAX];
    char dst_path[PATH_MAX];
    if (join_path(src_path, sizeof(src_path), src, entry->d_name) == -1 ||
        join_path(dst_path, sizeof(dst_path), dst, entry->d_name) == -1) {
      fprintf(stderr, "Joining path failed\n");
      closedir(sd);
      return -1;
    }

    // get type of src
    if (lstat(src_path, &st) != 0) {
      perror(src_path);
      closedir(sd);
      return -1;
    }

    const char *dot = strrchr(entry->d_name, '.'); // pointer to extension

    // recursive call if folder
    if (S_ISDIR(st.st_mode)) {
      if (build_content(src_path, dst_path, temp) != 0) {
        closedir(sd);
        return -1;
      }
    } else if (S_ISREG(st.st_mode)) {
      if (dot != NULL && strcmp(dot, ".md") == 0) {
        char html_name[PATH_MAX];
        char html_path[PATH_MAX];

        if (replace_ext(html_name, sizeof(html_name), entry->d_name, ".html") ==
            -1) {
          closedir(sd);
          return -1;
        }
        if (join_path(html_path, sizeof(html_path), dst, html_name) == -1) {
          closedir(sd);
          return -1;
        }
        if (render_page(temp, src_path, html_path) == -1) {
          closedir(sd);
          return -1;
        }
      } else {
        fprintf(stderr, "%s is not a markdown file.\n", entry->d_name);
      }
    } else {
      // skip symlinks, FIFOs, devices, sockets
      fprintf(stderr, "Skipping %s (not a regular file)\n", src_path);
    }
  }

  closedir(sd);
  return 0;
}
