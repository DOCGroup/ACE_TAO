// $Id$

#ifndef ACE_ADAPTER_MEMORY_POOL_H
#define ACE_ADAPTER_MEMORY_POOL_H
#include "ace/pre.h"

#include "../../Memory_Pool.h"

#if !defined (ACE_LACKS_SBRK)

class ACE_Sbrk_Memory_Pool_W : ACE_Sbrk_Memory_Pool
{
public:
  ACE_Sbrk_Memory_Pool_W (const wchar_t *backing_store_name = 0,
                          const OPTIONS *options = 0)
    : ACE_Sbrk_Memory_Pool (ACE_TEXT_WCHAR_TO_CHAR (backing_store_name), options)
  {}

  virtual ~ACE_Sbrk_Memory_Pool_W (void)
  {}
}
#endif /* !ACE_LACKS_SBRK */

#if !defined (ACE_LACKS_SYSV_SHMEM)

class ACE_Shared_Memory_Pool_W : public ACE_Shared_Memory_Pool
{
public:
  ACE_Shared_Memory_Pool_W (const wchar_t *backing_store_name = 0,
                            const OPTIONS *options = 0)
    : ACE_Shared_Memory_Pool (ACE_TEXT_WCHAR_TO_CHAR (backing_store_name), options)
  {}

  virtual ~ACE_Shared_Memory_Pool_W (void)
  {}

};
#endif /* !ACE_LACKS_SYSV_SHMEM */

class ACE_Local_Memory_Pool_W : public ACE_Local_Memory_Pool
{
public:
  ACE_Local_Memory_Pool_W (const wchar_t *backing_store_name = 0,
                           const OPTIONS *options = 0)
    : ACE_Local_Memory_Pool (ACE_TEXT_WCHAR_TO_CHAR (backing_store_name), options)
  {}

  virtual ~ACE_Local_Memory_Pool_W (void)
  {}
};

class ACE_MMAP_Memory_Pool_W : public ACE_MMAP_Memory_Pool
{
public:
  ACE_MMAP_Memory_Pool_W (const wchar_t *backing_store_name = 0,
                          const OPTIONS *options = 0)
    : ACE_MMAP_Memory_Pool (ACE_TEXT_WCHAR_TO_CHAR (backing_store_name), options)
  {}

  virtual ~ACE_MMAP_Memory_Pool_W (void)
  {}
};

class ACE_Lite_MMAP_Memory_Pool_W : public ACE_Lite_MMAP_Memory_Pool
{
public:
  ACE_Lite_MMAP_Memory_Pool_W (const wchar_t *backing_store_name = 0,
                               const OPTIONS *options = 0)
    : ACE_Lite_MMAP_Memory_Pool (ACE_TEXT_WCHAR_TO_CHAR (backing_store_name), options)
  {}

  virtual ~ACE_Lite_MMAP_Memory_Pool_W (void)
  {}
};

#if defined (ACE_WIN32)

class ACE_Pagefile_Memory_Pool_W : public ACE_Pagefile_Memory_Pool
{
public:
  ACE_Pagefile_Memory_Pool_W (const wchar_t *backing_store_name = 0,
                              const OPTIONS *options = 0)
    : ACE_Pagefile_Memory_Pool (ACE_TEXT_WCHAR_TO_CHAR (backing_store_name), options)
  {}
};

#endif /* ACE_WIN32 */

#include "ace/post.h"
#endif /* ACE_ADAPTER_MEMORY_POOL_H */
