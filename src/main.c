#include <stdio.h>
#include <stdlib.h>

void copy_file(const char *src, const char *dst) {
    FILE *f_src = fopen(src, "rb");
    FILE *f_dst = fopen(dst, "wb");
    char str[100];
    while (fgets(str, 100, f_src)) {
        printf("%s", str);
    }
}

int main (/*int argc, char *argv[]*/) {
    /*if (argc > 1) {
        printf("Building %s\n", argv[1]);
    } else {
        printf("usage: statiq path_to_site_folder\n");
        exit(1);
        }*/

    copy_file("./tests/site1/static/style.css", "./tmp/style.css");

    return 0;
}
