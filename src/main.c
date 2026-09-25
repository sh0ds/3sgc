#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "fs.h"
#include "markdown.h"
#include <string.h>

/*
 * TODO:
 *  1. loop over content folder, converting every .md entry to html :: done ::
 *  2. implement shell args:    :: done ::
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
    /*
    if (argc == 3) {
        const char *path = argv[1];
        const char *arg = argv[2];
        char c_path[PATH_MAX], s_path[PATH_MAX], t_path[PATH_MAX], p_path[PATH_MAX];

        // build paths
        join_path(c_path, sizeof(c_path), path, "content");
        join_path(s_path, sizeof(s_path), path, "static");
        join_path(t_path, sizeof(t_path), path, "templates/basic.html");
        join_path(p_path, sizeof(p_path), path, "public");

        // copy static to public
        copy_dir(s_path,p_path);



    } */

    FILE *md = fopen("./sites/site1/content/test2.md", "rb");
    FrontMatter fm = {0};
    parse_fm(md, &fm);

    return 0;
}
