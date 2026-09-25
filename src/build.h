#ifndef BUILD_H
#define BUILD_H

int build_content(const char *src, const char *dst, const char *temp);
int render_page(const char *temp_path, const char *md_path,
                const char *html_path);

#endif
