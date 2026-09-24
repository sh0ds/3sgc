#include "fs.h"
#include "markdown.h"


int main (int argc, char *argv[]) {
    /*if (argc > 1) {
        printf("Building %s\n", argv[1]);
    } else {
        printf("usage: 3sgc path_to_site_folder\n");
        exit(1);
        }*/

    FILE *md = fopen("tests/site1/content/index.md", "r");
    FILE *html = fopen("tests/site1/static/index.html", "w");
    md_to_html(md, html);

    copy_dir("tests/site1/static", "tests/site1/public");

    /*
    char str[50];
    join_path(str, 50, "test", "test.css");
	printf("%s\n", str);
	*/

    return 0;
}
