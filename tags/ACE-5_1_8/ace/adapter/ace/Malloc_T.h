// $Id$

#ifndef ACE_ADAPTER_MALLOC_T_H
#define ACE_ADAPTER_MALLOC_T_H
#include "ace/pre.h"

#include "../../Malloc_T.h"

template <class MALLOC>
class ACE_Allocator_Adapter_W : public ACE_Allocator_Adapter<MALLOC>
{
public:
  ACE_Allocator_Adapter_W (const wchar_t *pool_name = 0)
    : ACE_Allocator_Adapter<MALLOC> (ACE_TEXT_WCHAR_TO_CHAR (pool_name))
  {}

  ACE_Allocator_Adapter_W (const wchar_t *pool_name,
                           const wchar_t *lock_name,
                           MEMORY_POOL_OPTIONS options = 0)
    : ACE_Allocator_Adapter<MALLOC> (ACE_TEXT_WCHAR_TO_CHAR (pool_name),
                                     ACE_TEXT_WCHAR_TO_CHAR (lock_name),
                                     options)
  {}

  virtual ~ACE_Allocator_Adapter_W (void)
  {}
};


template <ACE_MEM_POOL_1, class ACE_LOCK, class ACE_CB>
class ACE_Malloc_T_W : public ACE_Malloc_T<ACE_MEM_POOL_2, ACE_LOCK, ACE_CB>
{
public:
  ACE_Malloc_T_W (const wchar_t *pool_name = 0)
    : ACE_Malloc_T<ACE_MEM_POOL_2, ACE_LOCK, ACE_CB> (ACE_TEXT_WCHAR_TO_CHAR (pool_name))
  {}

  ACE_Malloc_T_W (const wchar_t *pool_name,
                  const wchar_t *lock_name,
                  const ACE_MEM_POOL_OPTIONS *options = 0)
    : ACE_Malloc_T<ACE_MEM_POOL_2, ACE_LOCK, ACE_CB> (ACE_TEXT_WCHAR_TO_CHAR (pool_name),
                                                      ACE_TEXT_WCHAR_TO_CHAR (lock_name),
                                                      options);
  {}


#if !defined (ACE_HAS_TEMPLATE_TYPEDEFS)
  ACE_Malloc_T_W (const wchar_t *pool_name,
                  const wchar_t *lock_name,
                  const void *options = 0)
    : ACE_Malloc_T<ACE_MEM_POOL_2, ACE_LOCK, ACE_CB> (ACE_TEXT_WCHAR_TO_CHAR (pool_name),
                                                      ACE_TEXT_WCHAR_TO_CHAR (lock_name),
                                                      options);
  {}
#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */

  ~ACE_Malloc_T_W (void)
  {}
};


template <ACE_MEM_POOL_1, class ACE_LOCK>
class ACE_Malloc_W : public ACE_Malloc <ACE_MEM_POOL_2, ACE_LOCK>
{
public:
  ACE_Malloc_W (const wchar_t *pool_name = 0)
    : ACE_Malloc <ACE_MEM_POOL_2, ACE_LOCK> (ACE_TEXT_WCHAR_TO_CHAR (pool_name))
  {}

  ACE_Malloc_W (const wchar_t *pool_name,
                const wchar_t *lock_name,
                const ACE_MEM_POOL_OPTIONS *options = 0)
    : ACE_Malloc <ACE_MEM_POOL_2, ACE_LOCK> (ACE_TEXT_WCHAR_TO_CHAR (pool_name),
                                             ACE_TEXT_WCHAR_TO_CHAR (lock_name),
                                             options)
  {}

#if !defined (ACE_HAS_TEMPLATE_TYPEDEFS)
  ACE_Malloc_W (const wchar_t *pool_name,
                const wchar_t *lock_name,
                const void *options = 0)
    : ACE_Malloc <ACE_MEM_POOL_2, ACE_LOCK> (ACE_TEXT_WCHAR_TO_CHAR (pool_name),
                                             ACE_TEXT_WCHAR_TO_CHAR (lock_name),
                                             options)
  {}
#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */
};

#include "ace/post.h"
#endif /* ACE_MALLOC_H */
