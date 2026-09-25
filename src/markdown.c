#include "markdown.h"
#include "fs.h"
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static void strip_nl(char *s) {
  size_t n = strlen(s);
  if (n > 0 && s[n - 1] == '\n')
    s[--n] = '\0';
  if (n > 0 && s[n - 1] == '\r')
    s[--n] = '\0';
}

static void close_p(FILE *out, int *in_p) {
  if (*in_p) {
    fprintf(out, "</p>\n");
    *in_p = 0;
  }
}

static void write_text(FILE *out, const char *s) {
  size_t len = strlen(s);

  for (size_t i = 0; i < len; i++) {
    switch (s[i]) {
    case '<':
      fputs("&lt;", out);
      break;
    case '>':
      fputs("&gt;", out);
      break;
    case '&':
      fputs("&amp;", out);
      break;
    case '"':
      fputs("&quot;", out);
      break;
    default:
      fputc(s[i], out);
      break;
    }
  }
}


static int name_is(const char *name, int len, const char *word) {
  return (strlen(word) == (size_t)len && strncmp(name, word, len));
}

static const char *get_value(const FrontMatter *fm, const char *key, int len) {
  if (name_is(key, len, "title")) {
    return fm->title;
  } else if (name_is(key, len, "author")) {
    return fm->author;
  } else if (name_is(key, len, "date")) {
    return fm->date;
  } else if (name_is(key, len, "summary")) {
    return fm->summary;
  }
  return NULL;
}

static int fill_temp(FILE *out, const char *temp, FILE *md, const FrontMatter *fm) {
  int len;
  const char *open = NULL, *close = NULL, *key_end = NULL, *cursor = temp, *value;

  while(1) {
    if ((open = strstr(cursor, "{{")) == NULL) {
      fputs(cursor, out);
      break;
    }

    fwrite(cursor, 1, open - cursor, out);
    open = open + 2;
    if ((close = strstr(open, "}}")) == NULL) {
      fprintf(stderr, "No closing '}}' found.\n");
      return -1;
    }

    while (*open == ' ') {open++;}
    key_end = close;
    while (*(key_end-1) == ' ') {key_end--;}

    len = key_end - open;

    if (name_is(++open, len, "content") == 1) {
      if(md_convert(md, out) == -1) {return -1;}
    } else {
      if((value = get_value(fm, open, len)) == NULL) {
        fprintf(stderr, "Unknown name: %.*s\n", len, open);
        return -1;
      }
      write_text(out, value);
    }

    cursor = close + 2;
  }

  return 0;
}

static void set_kv_pair(FrontMatter *fm, const char *key, const char *value) {
  if (strcmp(key, "title") == 0) {
    snprintf(fm->title, sizeof(fm->title), "%s", value);
  } else if (strcmp(key, "author") == 0) {
    snprintf(fm->author, sizeof(fm->author), "%s", value);
  } else if (strcmp(key, "date") == 0) {
    snprintf(fm->date, sizeof(fm->date), "%s", value);
  } else if (strcmp(key, "summary") == 0) {
    snprintf(fm->summary, sizeof(fm->summary), "%s", value);
  } else {
    fprintf(stderr, "Unknown FM key: %s\n", key);
  }
}

int parse_fm(FILE *md, FrontMatter *fm) {
  char *ln = NULL;
  size_t cap = 0;
  int rs = -1;

  memset(fm, 0, sizeof(*fm));

  if (getline(&ln, &cap, md) == -1) {
    rs = ferror(md) ? -1 : 0;
    goto cleanup;
  }
  strip_nl(ln);
  if (strcmp(ln, "---") != 0) {
    fseek(md, 0, SEEK_SET);
    rs = 0;
    goto cleanup;
  }

  while (getline(&ln, &cap, md) != -1) {
    strip_nl(ln);
    if (strcmp(ln, "---") == 0) {
      rs = 0;
      goto cleanup;
    }

    char *colon = strchr(ln, ':');
    if (colon == NULL)
      continue;

    *colon = '\0';
    char *value = colon + 1;
    while (*value == ' ')
      value++;

    set_kv_pair(fm, ln, value);
  }

  fprintf(stderr, "FM is missing closing '---'\n");

cleanup:
  free(ln);
  return rs;
}

int md_convert(FILE *md_file, FILE *html_file) {
  // variables
  char *ln = NULL;
  size_t cap = 0;
  ssize_t len;
  int in_p = 0, in_code = 0;

  // getline loop
  while ((len = getline(&ln, &cap, md_file)) != -1) {
    int lvl = 0;

    if (len > 0 && ln[len - 1] == '\n') {
      ln[--len] = '\0';
    }
    if (len > 0 && ln[len - 1] == '\r') {
      ln[--len] = '\0';
    }

    /*
     *     -- CODE BLOCKS --
     */

    // check for fence
    if (strncmp(ln, "```", 3) == 0) {
      if (!in_code) {
        close_p(html_file, &in_p);
        fprintf(html_file, "<pre><code>");
        in_code = 1;
      } else {
        fprintf(html_file, "</code></pre>\n");
        in_code = 0;
      }
      continue;
    }

    // check if in code
    if (in_code) {
      write_text(html_file, ln);
      fputc('\n', html_file);
      continue;
    }

    /*
     *      -- HEADERS --
     */

    // header check, convert number of '#' to <h*>
    while (ln[lvl] == '#') {
      lvl++;
    }
    if (lvl > 0 && lvl < 7 && ln[lvl] == ' ') {
      close_p(html_file, &in_p);
      fprintf(html_file, "<h%d>", lvl);
      write_text(html_file, ln + lvl + 1);
      fprintf(html_file, "</h%d>\n", lvl);

      /*
       *       -- EMPTY LINES & PARAPGRAPHS --
       */

      // empty line check
    } else if (len == 0) {
      close_p(html_file, &in_p);

      // checking whether in <p> or not
    } else {
      if (!in_p) {
        fprintf(html_file, "<p>");
        in_p = 1;
      }
      write_text(html_file, ln);
      fputc('\n', html_file);
    }
  }
  close_p(html_file, &in_p);
  if (in_code) {
    fprintf(html_file, "</code></pre>\n");
  }

  int result = (ferror(md_file) || ferror(html_file)) ? -1 : 0;
  free(ln);

  return result;
}

int md_to_html(const char *md_path, const char *html_path) {
  FILE *md = fopen(md_path, "rb");
  if (md == NULL) {
    perror("Error opening .md file");
    return -1;
  }
  FILE *html = fopen(html_path, "wb");
  if (html == NULL) {
    perror("Error opening .html file");
    fclose(md);
    return -1;
  }

  int result = md_convert(md, html);

  fclose(html);
  fclose(md);

  return result;
}

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

  printf("%s\n", fm.title);

  html = fopen(html_path, "wb");
  if (html == NULL) {
    perror("Error opening .html file");
    goto cleanup;
  }

  fill_temp(html, temp, md, &fm);

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
