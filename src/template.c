#include "template.h"
#include "frontmatter.h"
#include "markdown.h"
#include "string.h"

static int name_is(const char *name, int len, const char *word) {
  return (strlen(word) == (size_t)len && strncmp(name, word, len) == 0);
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

int fill_temp(FILE *out, const char *temp, FILE *md, const FrontMatter *fm) {
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
    if (len <= 0) {
      fprintf(stderr, "Invalid placeholder.");
      return -1;
    }

    if (name_is(open, len, "content") == 1) {
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
