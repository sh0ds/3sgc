#include <stdlib.h>
#include "fs.h"
#include "markdown.h"

/*
 * TODO:
 *  1. loop over content folder, converting every .md entry to html
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

    char *md_path = "sites/site1/content/test.md";
    char *html_path = "sites/site1/static/index.html";
    const char *temp_path = "sites/site1/templates/basic.html";

    render_page(temp_path, md_path, html_path);

    // copy_dir("sites/site1/static", "sites/site1/public");

    return 0;
}
