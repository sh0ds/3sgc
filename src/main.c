#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "fs.h"
#include "build.h"
#include "index.h"
#include "post_store.h"

static void print_usage(const char *prog)
{
    fprintf(stderr,
            "Usage: %s <site-path> <mode>\n"
            "\n"
            "<mode>:\n"
            "  build    - posts, indexes, and static assets\n"
            "  content  - posts and indexes only\n"
            "  static   - copy static assets only\n",
            prog);
}

int main(int argc, char *argv[])
{
    char c_path[PATH_MAX], s_path[PATH_MAX];
    char t_dir[PATH_MAX], p_path[PATH_MAX], basic[PATH_MAX];
    int do_content, do_static;

    if (argc != 3) {
        print_usage(argv[0]);
        return 1;
    }

    if (strcmp(argv[2], "build") == 0) {
        do_content = do_static = 1;
    } else if (strcmp(argv[2], "content") == 0) {
        do_content = 1;
        do_static = 0;
    } else if (strcmp(argv[2], "static") == 0) {
        do_content = 0;
        do_static = 1;
    } else {
        fprintf(stderr, "Unknown mode: %s\n", argv[2]);
        print_usage(argv[0]);
        return 1;
    }

    if (snprintf(c_path, sizeof(c_path), "%s/content", argv[1]) >= (int)sizeof(c_path) ||
        snprintf(s_path, sizeof(s_path), "%s/static", argv[1]) >= (int)sizeof(s_path) ||
        snprintf(t_dir,  sizeof(t_dir),  "%s/templates", argv[1]) >= (int)sizeof(t_dir) ||
        snprintf(p_path, sizeof(p_path), "%s/public", argv[1]) >= (int)sizeof(p_path) ||
        snprintf(basic,  sizeof(basic),  "%s/basic.html", t_dir) >= (int)sizeof(basic)) {
        fprintf(stderr, "Path too long: %s\n", argv[1]);
        return 1;
    }

    if (do_content) {
        PostStore posts;

        post_store_init(&posts);
        if (build_content(c_path, p_path, basic, &posts) != 0 ||
            render_indexes(&posts, p_path, t_dir) != 0) {
            fprintf(stderr, "Content build failed\n");
            post_store_free(&posts);
            return 1;
        }
        printf("Built %zu posts\n", posts.count);
        post_store_free(&posts);
    }

    if (do_static) {
        if (copy_dir(s_path, p_path) != 0) {
            fprintf(stderr, "Static copy failed\n");
            return 1;
        }
    }

    return 0;
}
