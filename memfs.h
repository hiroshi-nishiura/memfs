/*
 * @brief  Simple OnMemory File System
 */
#ifndef _MEMFS_H_
#define _MEMFS_H_

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

  // file
  typedef struct m_file_impl
  {
    int index;
    size_t pos;
  } m_file;

  int m_open(m_file *file, const char *file_path);
  int m_close(m_file *file);
  int m_read(m_file *file, void *buf, size_t size, size_t *nread);
  size_t m_size(m_file *file);
  int m_seek(m_file *file, long pos);

  // dir
  typedef struct m_dir_impl
  {
    int index;
    int num;
    int cur;
  } m_dir;

  bool m_isdir(m_dir *dir);
  const char *m_name(m_dir *dir);
  int m_opendir(m_dir *dir, const char *path);
  int m_readdir(m_dir *dir);

  // table
  typedef struct m_table_impl
  {
    size_t next, pos, size;
    const char *name;
  } m_table;

  // init must be called before any other api
  int m_set(const m_table *table, const uint8_t *data);

  // return enabled or disabled
  bool m_enabled();

#ifdef __cplusplus
} // extern "C"
#endif
#endif //_MEMFS_H_
