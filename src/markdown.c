#include "markdown.h"
#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    // header check, convert number of '#' to <h*>
    while (ln[lvl] == '#') {
      lvl++;
    }
    if (lvl > 0 && lvl < 7 && ln[lvl] == ' ') {
      close_p(html_file, &in_p);
      fprintf(html_file, "<h%d>", lvl);
      write_text(html_file, ln + lvl + 1);
      fprintf(html_file, "</h%d>\n", lvl);

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
  char *temp = NULL, *pos = NULL;
  FILE *md = NULL;
  FILE *html = NULL;

  // read template into a string
  if ((temp = read_file(temp_path)) == NULL)
    goto cleanup;

  // find the position of the placeholder
  if ((pos = strstr(temp, "{{ content }}")) == NULL) {
    fprintf(stderr, "Cannot find placeholder in %s\n", temp_path);
    goto cleanup;
  }

  // open files
  md = fopen(md_path, "rb");
  if (md == NULL) {
    perror("Error opening .md file");
    goto cleanup;
  }
  html = fopen(html_path, "wb");
  if (html == NULL) {
    perror("Error opening .html file");
    goto cleanup;
  }

  // write upper half of template
  fwrite(temp, 1, pos - temp, html);

  // call md_convert for directly converting the content
  if (md_convert(md, html) == -1)
    goto cleanup;

  // write lower half of template
  fputs(pos + strlen("{{ content }}"), html);

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
