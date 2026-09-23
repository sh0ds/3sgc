#ifndef FS_H
#include <stddef.h>
#define FS_H

int copy_file(const char *src, const char *dst);

int copy_dir(const char *src, const char *dst);

int join_path(char *out, size_t out_size, const char *dir, const char *name);

#endif
