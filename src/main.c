#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int copy_file(const char *src, const char *dst);
int print_folder(const char* folder);


int main (int argc, char *argv[]) {
    /*if (argc > 1) {
        printf("Building %s\n", argv[1]);
    } else {
        printf("usage: 3sgc path_to_site_folder\n");
        exit(1);
        }*/

    // copy_file("./tests/site1/static/style.css", "./tmp/style.css");
	print_folder("./tests");

    return 0;
}


// copies src to dst, in buffer sizes of 4KB
int copy_file(const char *src, const char *dst) {
    FILE *f_src, *f_dst;
	
	// open source file
	f_src = fopen(src, "rb");
	if (f_src == NULL) {
		perror("Error opening source file");
		return -1;
	}

	// open destination file
    f_dst = fopen(dst, "wb");
	if (f_dst == NULL) {
		perror("Error opening destination file");
		fclose(f_src);
		return -1;
	}

	// while fread returns > 0, write to destination
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

// open folder and print it's contents
int print_folder(const char* folder){ 
	DIR *dir;
	struct dirent *entry;

	// open folder
	dir = opendir(folder);
	if (dir == NULL) {
		perror("Cannot open folder");
		return -1;
	}

	// print every entry
	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
			printf("%s\n", entry->d_name);
		}
	}

	closedir(dir);

	return 0;
}