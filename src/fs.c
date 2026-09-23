#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "fs.h"

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

int join_path(char *out, size_t out_size, const char *dir, const char *name) {

    int res = snprintf(out, out_size, "%s/%s", dir, name);
    if (res < 0 || (size_t)res >= out_size) {
        return -1;
    }

    return 0;
}

// open folder and print it's contents
int copy_dir(const char *src, const char *dst){

    DIR *sd;
	struct dirent *entry;
	struct stat st;

	mkdir(dst, 0777);

	// open src
	sd = opendir(src);
	if (sd == NULL) {
	    perror("Cannot open folder");
					return -1;
	}

	// copy every entry
	while ((entry = readdir(sd)) != NULL) {

	    // skip current and parent directory
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {continue;}

		// create path of src and dst via helper function
		char src_path[PATH_MAX];
		char dst_path[PATH_MAX];
		if (join_path(src_path, sizeof(src_path), src, entry->d_name) == -1 ||
		    join_path(dst_path, sizeof(dst_path), dst, entry->d_name) == -1) {
		    fprintf(stderr, "Joining path failed\n");
      closedir(sd);
		    return -1;
		}

		// get type of src
		if (stat(src_path, &st) != 0) {
		    perror(src_path);
      closedir(sd);
		    return -1;
		}
		if (S_ISDIR(st.st_mode)) {      // call copy_dir recursively if directory
		    if(copy_dir(src_path, dst_path) != 0) {
				closedir(sd);
				return -1;
			}
		} else {                        // else copy the file
		    if(copy_file(src_path, dst_path) != 0) {
				closedir(sd);
				return -1;
			}
		}
	}

	closedir(sd);

	return 0;
}
