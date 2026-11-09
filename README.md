# 3sgc, sh0ds static site generator (in C)

My first little C project. A static site generator in C11 with 0 dependencies, just the standard C library and POSIX. It builds my website, [sh0ds.dev](https://sh0ds.dev).

## What it does

Walks everything in the content folder, reads the front matter at the top of each markdown file and sorts the posts by date. Then it renders the pages through the templates and creates the blog and project index pages.

## Requirements

- a C compiler with C11 support
- the POSIX filesystem API (`dirent.h`, `unistd.h`, `sys/stat.h`)

No third-party libraries, no framework.

## Building

```bash
make          # builds to build/3sgc
make clean    # removes build/
```

It builds with `-Wall -Wextra` plus AddressSanitizer and UndefinedBehaviorSanitizer, so if something's off with memory I find out right away.

## Usage

```bash
./build/3sgc <site-path> <mode>
```

Modes:
- `build`: posts, indexes and static assets
- `content`: posts and indexes only
- `static`: only copy the static assets

For example:

```bash
./build/3sgc sites/site1 build
```

## Site structure

```
sites/site1/
├── content/
│   ├── blog/      ← blog posts go here (gets /blog/index.html)
│   └── projects/  ← projects go here (gets /projects/index.html)
├── static/        ← CSS, JS, images (copied as they are to public/)
├── templates/
│   ├── basic.html     ← template for a single post
│   ├── blog.html      ← blog list (use {{#posts}}...{{/posts}})
│   └── projects.html  ← project list (same syntax)
└── public/          ← the output, created by build
```

## Front matter

Every `.md` file in `content/` can have this at the top:

```yaml
---
title: Post Title
author: sh0ds
date: 2026-10-06
summary: Short description
---
```

In the templates you can use `{{ title }}`, `{{ author }}`, `{{ date }}`, `{{ summary }}`, `{{ content }}` and `{{ url }}`.

## Lists of posts

```html
{{#posts}}
  <li><a href="{{ url }}">{{ title }}</a></li>
{{/posts}}
```

This repeats once for every post in that section (blog or projects).

## TODO

I kept the markdown part simple first so I actually finish the project. This is what's still missing:

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
