/*
 * @brief  Simple OnMemory File System
 */
#include "memfs.h"

const m_table *cur_memfs_table = 0;
const uint8_t *cur_memfs_data = 0;

#define MEMFS_MAX_PATH 256

static int _m_find(const char *path)
{
  char pnames[MEMFS_MAX_PATH];

  if (!path)
    return -1;
  memset(pnames, 0, sizeof(pnames));
  strncpy(pnames, path, MEMFS_MAX_PATH);

  int index = 0;
  for (char *p = pnames; p || *p; p++)
  {
    char *name = p;
    p = strchr(name, '/');
    if (p)
      *p = 0;
    if (*name)
    {
      while (index < cur_memfs_table[0].next)
      {
        // hit
        if (!strcmp(name, cur_memfs_table[index].name))
        {
          // reached
          if (p == 0)
            return index;
          // dir
          else if (cur_memfs_table[index].size == 0)
            index++;
          // next
          break;
        }
        // next
        else
          index = cur_memfs_table[index].next;
      }
    }
    if (p == 0)
      break;
  }
  return -1;
}

int m_open(m_file *file, const char *file_path)
{
  if (!m_enabled() || !file || !file_path)
    return -1;
  int i = _m_find(file_path);
  if (i >= 0)
  {
    file->index = i;
    file->pos = 0;
    return 0;
  }
  file->index = -1;
  file->pos = -1;
  return -1;
}

int m_close(m_file *file)
{
  if (!m_enabled() || !file || file->index < 0)
    return -1;
  // do nothing
  return 0;
}

int m_read(m_file *file, void *buf, size_t size, size_t *nread)
{
  if (nread)
    *nread = 0;
  if (!m_enabled() || !file || file->index < 0 || !buf || size <= 0)
    return -1;

  const m_table *table = &cur_memfs_table[file->index];
  if (file->pos + size > table->size)
    size = table->size - file->pos;
  if (size <= 0)
    return -1;

  memcpy(buf, cur_memfs_data + table->pos + file->pos, size);
  file->pos += size;
  if (nread)
    *nread = size;
  return 0;
}

size_t m_size(m_file *file)
{
  if (!m_enabled() || !file || file->index < 0)
    return -1;
  return cur_memfs_table[file->index].size;
}

int m_seek(m_file *file, long pos)
{
  if (!m_enabled() || !file || file->index < 0 || pos < 0 || pos > m_size(file))
    return -1;
  file->pos = pos;
  return 0;
}

bool m_isdir(m_dir *dir)
{
  if (!m_enabled() || !dir || dir->index < 0)
    return false;
  return cur_memfs_table[dir->index].size == 0;
}

const char *m_name(m_dir *dir)
{
  if (!dir || dir->index < 0)
    return 0;
  return cur_memfs_table[dir->index].name;
}

int m_opendir(m_dir *dir, const char *path)
{
  if (!m_enabled() || !dir || dir->index < 0)
    return -1;
  int index = _m_find(path);
  if (index >= 0 && cur_memfs_table[index].size == 0)
  {
    dir->index = index;
    dir->num = cur_memfs_table[index].pos;
    dir->cur = 0;
    return 0;
  }
  dir->index = -1;
  dir->num = -1;
  dir->cur = -1;
  return -1;
}

int m_readdir(m_dir *dir)
{
  if (!m_enabled() || !dir || dir->index < 0)
    return -1;
  if (dir->cur < dir->num)
  {
    if (dir->cur)
      dir->index = cur_memfs_table[dir->index].next;
    else
      dir->index++;
    dir->cur++;
    return 0;
  }
  return -1;
}

int m_set(const m_table *table, const uint8_t *data)
{
  cur_memfs_table = table;
  cur_memfs_data = data;
  return 0;
}

bool m_enabled()
{
  return cur_memfs_table && cur_memfs_data;
}
