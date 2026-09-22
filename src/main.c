#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv[]) {
    if (argc > 1) {
        printf("Building %s\n", argv[1]);
    } else {
        printf("usage: statiq path_to_site_folder\n");
        exit(1);
    }

    return 0;
}
