#include <stdlib.h>
#include "fs.h"
#include "markdown.h"

/*
 * TODO:
 * loop over content folder, converting every entry to html,
 * implement shell args to 1: build content folder; 2: copy static to public,
 * add more markdown syntax:
 *      - links
 *      - images
 *      - horizontal rules
 *      - bold and italic
 *      - lists (ordered, unordered)
 *      - syntax highlighting
 *      - blockquotes
 *      - footnotes
 *      - highlight
 *      - subscript, superscript
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
