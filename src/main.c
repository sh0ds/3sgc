#include <stdio.h>
#include <stdlib.h>

int copy_file(const char *src, const char *dst) {
    FILE *f_src = fopen(src, "rb");
	if (f_src == NULL) {
		perror("Error opening source file");
		return -1;
	}
    FILE *f_dst = fopen(dst, "wb");
	if (f_dst == NULL) {
		perror("Error opening destination file");
		fclose(f_src);
		return -1;
	}
    char buf[4096];
    size_t rf;
	while ((rf = fread(buf, 1, sizeof buf, f_src)) > 0) {	
		if (fwrite(buf, sizeof(char), rf, f_dst) < rf) {
			perror("Write failed");
			fclose(f_src);
			fclose(f_dst);
			return -1;
		}
    }
	
	if (ferror(f_src) != 0) {
		perror("Error reading source file");
		fclose(f_src);
		fclose(f_dst);
		return -1;
	}
	fclose(f_src);
	fclose(f_dst);
	
	return 0;
}

int main (int argc, char *argv[]) {
    /*if (argc > 1) {
        printf("Building %s\n", argv[1]);
    } else {
        printf("usage: statiq path_to_site_folder\n");
        exit(1);
        }*/

    copy_file("./tests/site1/static/style.css", "./tmp/style.css");

    return 0;
}