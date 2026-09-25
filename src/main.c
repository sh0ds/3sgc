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
    /*if (argc > 2) {
        printf("Building %s\n", argv[1]);
    } else {
        printf("usage: 3sgc path_to_site_folder -flag\n");
        printf("flags: -s: convert to static, -p: push to public\n");
        exit(1);
        }*/

    char *src = "sites/site1/content";
    char *dst = "sites/site1/static";
    const char *temp = "sites/site1/templates/basic.html";

    build_content(src, dst, temp);

    // copy_dir("sites/site1/static", "sites/site1/public");

    return 0;
}
