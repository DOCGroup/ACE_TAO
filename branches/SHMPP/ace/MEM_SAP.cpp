// $Id$

#define ACE_BUILD_DLL
#include "ace/MEM_SAP.h"
#include "ace/Auto_Ptr.h"

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
ACE_MEM_SAP::create_shm_malloc (const char *name,
                                MALLOC_OPTIONS *options)
{
  ACE_TRACE ("ACE_MEM_SAP::create_shm_malloc");

  if (this->shm_malloc_ != 0)
    return -1;                  // already initialized.

  int delete_options = 0;

  if (options == 0)
    {
      delete_options = 1;
      ACE_NEW_RETURN (options,
                      MALLOC_OPTIONS (),
                      -1);
    }
  auto_ptr<MALLOC_OPTIONS> safe_options (options);

  ACE_NEW_RETURN (this->shm_malloc_,
                  MALLOC (name,
                          name,
                          options),
                  -1);

  if (delete_options == 0)
    safe_options.release ();
  return 0;
}

int
ACE_MEM_SAP::close_shm_malloc (const int remove)
{
  if (this->shm_malloc_ != 0)
    {
      this->shm_malloc_->remove ();
      return 0;
    }
  return -1;
}
