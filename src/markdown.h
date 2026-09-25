#ifndef MARKDOWN_H
#define MARKDOWN_H
#include <stdio.h>

typedef struct {
    char title[256];
    char author[128];
    char date[16];
    char summary[512];
} FrontMatter;

int md_to_html(const char *md_path, const char *html_path);
void ins_content(const char *html_path, const char *temp_path);
int render_page(const char* temp_path, const char *md_path, const char *html_path);
int md_convert(FILE *md_file, FILE *html_file);
int build_content(const char *src, const char *dst, const char *temp);
int parse_fm(FILE *md, FrontMatter *fm);

#endif
