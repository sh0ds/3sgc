#include <stdlib.h>
#include "fs.h"
#include "markdown.h"

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
    if (argc == 2) {
        if (strcmp(argv[1], "build") == 0) {

        } else if (strcmp(argv[1], "content") == 0) {

        } else if (strcmp(argv[1], "static") == 0) {

        } else {
            fprintf(stderr, "Unrecognised argument, try again. (type '3sgc' for usage)\n");
            return 1;
        }
    } else if (argc > 2) {
        fprintf(stderr, "Too many arguments, try again. (type '3sgc' for usage)\n");
        return 1;
    } else {
        printf("usage: 3sgc <command>\n");
        printf("commands:\n");
        printf("build: build content folder and copy static to public\n");
        printf("content: build content folder only\n");
        printf("static: copy static to public only\n");
        return 1;
    }

    return 0;
}
