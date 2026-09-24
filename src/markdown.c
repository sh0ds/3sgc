#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markdown.h"

// close paragraph helper
static void close_paragraph(FILE* out, int *in_paragraph) {
    if (*in_paragraph) {
        fprintf(out, "</p>\n");
        *in_paragraph = 0;
    }
}

// TODO: add HTML escaping function

int md_to_html(const char *md, const char *html) {
    // open files
    FILE *md_file = fopen(md, "r");
    if (md_file == NULL) {
        perror("Error opening markdown file");
        return -1;
    }
    FILE *html_file = fopen(html, "w");
    if (html_file == NULL) {
        perror("Error opening HTML file");
        fclose(md_file);
        return -1;
    }
    
    //variables
    char *line = NULL;
    size_t cap = 0;
    ssize_t len;
    int in_paragraph = 0;

    // getline loop
    while ((len = getline(&line, &cap, md_file)) != -1) {
        int level = 0;
        
        if (len > 0 && line[len - 1] == '\n') {
            line[--len] = '\0';
        }

        while (line[level] == '#') {
            level++;
        }

        // header check
        if (level > 0 && level < 7 && line[level] == ' ') {
            close_paragraph(html_file, &in_paragraph);
            fprintf(html_file, "<h%d>%s</h%d>\n", level, line + level + 1, level);
        
        // empty line check    
        } else if (len == 0) {
            close_paragraph(html_file, &in_paragraph);

        // if not in paragraph, open <p>, print
        } else {
            if (!in_paragraph) {
                fprintf(html_file, "<p>");
                in_paragraph = 1;
            }
            fprintf(html_file, "%s\n", line);
        }
    }
    close_paragraph(html_file, &in_paragraph);

    int result = ferror(md_file) ? -1 : 0;
    free(line);
    fclose(md_file);
    fclose(html_file);
    return result;
}