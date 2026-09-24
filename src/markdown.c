#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markdown.h"


int md_to_html(const char *md, const char *html) {
    
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
    
    char *line = NULL;
    size_t cap = 0;
    ssize_t len;
    int in_paragraph = 0;

    while ((len = getline(&line, &cap, md_file)) != -1) {
        int level = 0;
        
        if (len > 0 && line[len - 1] == '\n') {
            line[--len] = '\0';
        }
        while (line[level] == '#') {
            level++;
        }
        if (level > 0 && level < 7 && line[level] == ' ') {
            if (in_paragraph) {
                fprintf(html_file, "</p>\n");
                in_paragraph = 0;
            }
            fprintf(html_file, "<h%d>%s</h%d>\n", level, line + level + 1, level);
    
    // 1. if a paragraph is open: print "</p>\n", clear the flag
    // 2. print the heading
    } else if (len == 0) {
        if (in_paragraph) {
            fprintf(html_file, "</p>\n");
            in_paragraph = 0;
        }

    // if a paragraph is open: print "</p>\n", clear the flag
    } else {
        if (!in_paragraph) {
            fprintf(html_file, "<p>");
            in_paragraph = 1;
        }
    // 1. if no paragraph is open: print "<p>", set the flag
    // 2. print the line (always, not in an else)
        fprintf(html_file, "%s\n", line);
        }
    }
    if (in_paragraph) {
        fprintf(html_file, "</p>\n");
    }
    int result = ferror(md_file) ? -1 : 0;
    free(line);
    fclose(md_file);
    fclose(html_file);
    return result;
}