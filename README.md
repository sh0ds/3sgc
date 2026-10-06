# 3sgc — sh0ds static site generator (in C)

A static site generator written in C11. No dependencies beyond libc and POSIX.

## What it does

Collects front matter from markdown files, sorts by date, renders pages through templates, generates blog and project indexes. All in roughly 500 lines.

## Requirements

- C compiler with C11 support
- POSIX filesystem API (`dirent.h`, `unistd.h`, `sys/stat.h`)

No third-party libraries. No framework.

## Building

```bash
make          # builds to build/3sgc
make clean    # removes build/
```

Compiler flags include `-Wall -Wextra` plus AddressSanitizer and UndefinedBehaviorSanitizer. If it compiles, it probably won't segfault.

## Usage

```bash
./build/3sgc <site-path> <mode>
```

Modes:
- `build` — posts, indexes, and static assets
- `content` — posts and indexes only
- `static` — copy static assets only

Example:

```bash
./build/3sgc /home/sh0ds/Projects/3sgc/sites/site1 build
```

## Site structure

```
sites/site1/
├── content/
│   ├── blog/      ← blog posts go here (gets /blog/index.html)
│   └── projects/  ← projects go here (gets /projects/index.html)
├── static/        ← CSS, JS, images (copied verbatim to public/)
├── templates/
│   ├── basic.html     ← single post template
│   ├── blog.html      ← blog listing (use {{#posts}}...{{/posts}})
│   └── projects.html  ← project listing (same section syntax)
└── public/          ← generated output (created by build)
```

## Front matter

Every `.md` file in `content/` supports YAML-like front matter:

```yaml
---
title: Post Title
author: sh0ds
date: 2026-10-06
summary: Short description
---
```

Available placeholders in templates: `{{ title }}`, `{{ author }}`, `{{ date }}`, `{{ summary }}`, `{{ content }}`, `{{ url }}`.

## Section syntax (for indexes)

```html
{{#posts}}
  <li><a href="{{ url }}">{{ title }}</a></li>
{{/posts}}
```

Repeats once per post, filtered by section (blog vs. projects).

## TODO

Markdown syntax still to implement:

- [ ] Lists (unordered `-` / `+`)
- [ ] Lists (ordered `1.`)
- [ ] Nested lists
- [ ] Block quotes
- [ ] Horizontal rules `---`
- [ ] Inline code `` `code` ``
- [ ] Bold `**text**` and italic `*text*`
- [ ] Links `[text](url)`
- [ ] Images `![alt](src)`
- [ ] Fence info strings → `<code class="language-c">`
- [ ] Tables
- [ ] Strikethrough `~~text~~`
- [ ] Highlight `==text==`
- [ ] Subscript / superscript
- [ ] Footnotes
- [ ] Autolinks (`<https://...>`)
- [ ] Escape sequences `\*`

## License

MIT. Do whatever you want with it, but don't blame me when you break it.
