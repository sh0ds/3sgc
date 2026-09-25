### header 3
paragraph 3

#### header 4
paragraph 4

```
int render_page(const char *temp_path, const char *md_path,
                const char *html_path) {
  int result = -1;
  char *temp = NULL, *pos = NULL;
  FILE *md = NULL;
  FILE *html = NULL;

  // read template into a string
  if ((temp = read_file(temp_path)) == NULL)
    goto cleanup;

  // find the position of the placeholder
  if ((pos = strstr(temp, "{{ content }}")) == NULL) {
    fprintf(stderr, "Cannot find placeholder in %s\n", temp_path);
    goto cleanup;
  }

  // open files
  md = fopen(md_path, "rb");
  if (md == NULL) {
    perror("Error opening .md file");
    goto cleanup;
  }
  html = fopen(html_path, "wb");
  if (html == NULL) {
    perror("Error opening .html file");
    goto cleanup;
  }

  // write upper half of template
  fwrite(temp, 1, pos - temp, html);

  // call md_convert for directly converting the content
  if (md_convert(md, html) == -1)
    goto cleanup;

  // write lower half of template
  fputs(pos + strlen("{{ content }}"), html);

  result = 0;

cleanup:
  if (md != NULL)
    fclose(md);
  if (html != NULL) {
    if (fclose(html) != 0) {
      perror(html_path);
      result = -1;
    }
  }
  free(temp);

  return result;
}
```