#ifndef MARKDOWN_H
#define MARKDOWN_H
#include <stdio.h>

int md_to_html(const char *md, const char *html);
static void close_paragraph(FILE* out, int *in_paragraph);

#endif