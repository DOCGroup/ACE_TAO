// $Id$

#define ACE_BUILD_DLL
#include "ace/MEM_SAP.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/MEM_SAP.i"
#endif

ACE_RCSID(ace, IPC_SAP, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_IPC_SAP)

void
ACE_MEM_SAP::dump (void) const
{
  ACE_TRACE ("ACE_MEM_SAP::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  if (this->shm_malloc_ != 0)
    this->shm_malloc_->dump ();
  else
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("ACE_MEM_SAP uninitialized.\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_MEM_SAP::ACE_MEM_SAP (void)
  : shm_malloc_ (0)
{
  // ACE_TRACE ("ACE_MEM_SAP::ACE_MEM_SAP");
}

int
ACE_MEM_SAP::create_shm_malloc (LPCTSTR name,
                                MALLOC_OPTIONS *options)
{
  ACE_TRACE ("ACE_MEM_SAP::create_shm_malloc");

  if (this->shm_malloc_ != 0)
    return -1;                  // already initialized.

  ACE_NEW_RETURN (this->shm_malloc_,
                  MALLOC (name,
                          0,
                          options),
                  -1);

  return 0;
}

int
ACE_MEM_SAP::close_shm_malloc (const int remove)
{
  if (this->shm_malloc_ != 0 && remove != 0)
    {
      this->shm_malloc_->remove ();
      return 0;
    }
  return -1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Write_Guard<ACE_Process_Mutex>;
template class ACE_Read_Guard<ACE_Process_Mutex>;
#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
template class ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_PI_Control_Block>;
#else
template class ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex>;
template class ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_Control_Block>;
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Write_Guard<ACE_Process_Mutex>
#pragma instantiate ACE_Read_Guard<ACE_Process_Mutex>
#if defined (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
#pragma instantiate ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_PI_Control_Block>
#else
#pragma instantiate ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex>
#pragma instantiate ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_Control_Block>
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
