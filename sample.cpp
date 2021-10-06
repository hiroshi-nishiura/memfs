#define MEMFS_IMPLEMENTATION
#include "sample_table.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void get_names(const char *path, std::vector<std::string> &names)
{
  char name[256];
  m_dir dir;
  int rc = m_opendir(&dir, path);
  if (rc == 0)
  {
    for (;;)
    {
      rc = m_readdir(&dir);
      if (rc)
        break;

      strcpy(name, path);
      strcat(name, "/");
      strcat(name, m_name(&dir));

      if (m_isdir(&dir))
      {
        get_names(name, names);
      }
      else
      {
        names.push_back(name);
      }
    }
  }
}

int main()
{
#ifdef sample_data
  m_set(sample_table, sample_data);
#else
  m_set(sample_table, _binary_sample_data_bin_start);
#endif
  printf("memfs %s\n", m_enabled() ? "enabled" : "disabled");

  // dir
  std::vector<std::string> names;
  get_names(".", names);
  for (const auto &name : names)
    std::cout << name << std::endl;

  // file
  m_file file;
  size_t nr, size;
  uint8_t *buf;
  std::fstream fs;

  int rc = m_open(&file, "./sample/images/img01.jpg");
  if (rc == 0)
  {
    size = m_size(&file);
    printf("file size:%ld\n", size);

    buf = (uint8_t *)malloc(size);

    rc = m_read(&file, buf, size, &nr);
    printf("file read 1: %d, %ld\n", rc, nr);
    if (size == nr)
    {
      fs.open("img01.jpg", std::ios::out | std::ios::binary);
      fs.write((const char *)buf, size);
      fs.close();
    }

    rc = m_read(&file, buf, size, &nr);
    printf("file read 2: %d, %ld\n", rc, nr);

    rc = m_seek(&file, 0);
    printf("file seek: %d\n", rc);

    rc = m_seek(&file, size * 2);
    printf("file seek 2: %d\n", rc);

    rc = m_read(&file, buf, size / 2, &nr);
    printf("file read 3: %d, %ld\n", rc, nr);

    rc = m_read(&file, buf, size, &nr);
    printf("file read 4: %d, %ld\n", rc, nr);

    rc = m_read(&file, buf, 1, &nr);
    printf("file read 5: %d, %ld\n", rc, nr);

    rc = m_close(&file);
    printf("file close: %d\n", rc);

    free(buf);
  }
  else
    printf("not found\n");

  rc = m_open(&file, "./images/images01/img04.jpg");
  if (rc == 0)
  {
    size = m_size(&file);
    buf = (uint8_t *)malloc(size);
    rc = m_read(&file, buf, size, 0);

    fs.open("img04.jpg", std::ios::out | std::ios::binary);
    fs.write((const char *)buf, size);
    fs.close();

    rc = m_close(&file);
    free(buf);
  }
  else
    printf("not found\n");

  rc = m_open(&file, "./img03.jpg");
  if (rc == 0)
  {
    size = m_size(&file);
    buf = (uint8_t *)malloc(size);
    m_read(&file, buf, size, 0);

    fs.open("img03.jpg", std::ios::out | std::ios::binary);
    fs.write((const char *)buf, size);
    fs.close();

    rc = m_close(&file);
    free(buf);
  }
  else
    printf("not found\n");
}
