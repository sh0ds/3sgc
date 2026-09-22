#include <stdio.h>
#include <stlib.h>

int main (int argc, char* argv[]) {
    if (argc == 1) {
        printf("Building %s\n", argv[0]);
        exit(0);
    } else {
        printf("usage: statiq path_to_site_folder\n");
        exit(1);
    }
}
