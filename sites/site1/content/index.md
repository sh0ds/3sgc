# header1
## header2
### header3
#### header 4
##### header 5
###### header 6

paragraph 1
"sees" & "soos" :) 

paragraph 2
Hello, <World!>

``` 
#include <stdlib.h>
#include "fs.h"
#include "markdown.h"


int main (int argc, char *argv[]) {
    /*if (argc > 1) {
        printf("Building %s\n", argv[1]);
    } else {
        printf("usage: 3sgc path_to_site_folder\n");
        exit(1);
        }*/
    char *md_path = "sites/site1/content/index.md";
    char *html_path = "sites/site1/static/index.html";
    const char *temp_path = "sites/site1/templates/basic.html";

    render_page(temp_path, md_path, html_path);

    // copy_dir("tests/site1/static", "tests/site1/public");

    /*
    char str[50];
    join_path(str, 50, "test", "test.css");
	printf("%s\n", str);
	*/

    return 0;
}

```
