#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "fs.h"
#include "markdown.h"
#include <string.h>

/*
 * TODO:
 *  1. loop over content folder, converting every .md entry to html :: done
 *  2. implement shell args:
 *      - build: build content folder and copy static to public
 *      - content: build content folder only
 *      - static: copy static to public only
 *  3. front matter:
 *      - parse key: value block between --- lines
 *      - {{ title }} placeholder in template
 *      - date for posts
 *  4. easy block-level syntax:
 *      - horizontal rules
 *      - syntax highlighting (language class for prism)
 *      - blockquotes
 *  5. lists (ordered, unordered)
 *  6. inline syntax:
 *      - bold and italic
 *      - links
 *      - images
 *      - highlight
 *      - subscript, superscript
 *  7. footnotes
 */


int main (int argc, char *argv[]) {

    if (argc == 3) {
        const char *path = argv[1];
        const char *arg = argv[2];
        char c_path[PATH_MAX], s_path[PATH_MAX], t_path[PATH_MAX], p_path[PATH_MAX];
        join_path(c_path, sizeof(c_path), path, "content");
        join_path(s_path, sizeof(s_path), path, "static");
        join_path(t_path, sizeof(t_path), path, "templates/template.html");
        join_path(p_path, sizeof(p_path), path, "public");

        if (strcmp(arg, "build") == 0) {
            if (build_content(c_path, s_path, t_path) == -1) {
                fprintf(stderr, "Build failed.\n");
                return 1;
            }
            if (copy_dir(s_path, p_path) == -1) {
                fprintf(stderr, "Copying failed.\n");
                return 1;
            }
            printf("Build and copy complete.\n");
            return 0;
        } else if (strcmp(arg, "content") == 0) {
            if (build_content(c_path, s_path, t_path) == -1) {
                fprintf(stderr, "Build failed.\n");
                return 1;
            }
            printf("Build complete.\n");
            return 0;
        } else if (strcmp(arg, "static") == 0) {
            if (copy_dir(s_path, p_path) == -1) {
                fprintf(stderr, "Copying failed.\n");
                return 1;
            }
            printf("Copy complete.");
            return 0;
        } else {
            fprintf(stderr, "Unrecognised argument, try again. (type '3sgc' for usage)\n");
            return 1;
        }
    } else if (argc > 3) {
        fprintf(stderr, "Too many arguments, try again. (type '3sgc' for usage)\n");
        return 1;
    } else {
        printf("usage:\n            3sgc <site_path> <command>");
        printf("\ncommands:\n");
        printf("'build'     build content folder and copy static to public\n");
        printf("'content'   build content folder only\n");
        printf("'static'    copy static to public only\n");
        return 1;
    }

    return 0;
}
