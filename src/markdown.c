#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markdown.h"

static void close_p(FILE* out, int *in_p) {
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

int md_to_html(FILE *md_file, FILE *html_file) {

    //variables
    char *ln = NULL;
    size_t cap = 0;
    ssize_t len;
    int in_p = 0;

    // getline loop
    while ((len = getline(&ln, &cap, md_file)) != -1) {
        int lvl = 0;
        
        if (len > 0 && ln[len - 1] == '\n') {
            ln[--len] = '\0';
        }
        if (len > 0 && ln[len - 1] == '\r') {
            ln[--len] = '\0';
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

    int result = (ferror(md_file) || ferror(html_file)) ? -1 : 0;
    free(ln);

    return result;
}
