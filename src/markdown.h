#ifndef MARKDOWN_H
#define MARKDOWN_H
#include <stdio.h>

int md_to_html(FILE *md_file, FILE *html_file);
static void close_p(FILE* out, int *in_p);

#endif
