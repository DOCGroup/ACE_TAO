// $Id$

#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/Synch.h"
#include "ace/Task.h"
#include "ClientContext.h"


void*
ClientContext::get_attribute (const char *name)
{
  void * value = 0;
  attributeMap_.find (name, value);
  return value;
}

void
ClientContext::set_attribute (const char *name, void *value)
{
  attributeMap_.bind (name, value);
}

// Listing 2 code/ch14
class HA_CommandHandler : public ACE_Task<ACE_MT_SYNCH>
{
public:
  virtual int svc (void)
  {
    ACE_thread_t tid = this->thr_mgr ()->thr_self ();
    // Set our identifier in TSS.
    this->tss_ctx_->set_attribute ("thread_id", &tid);

    while (handle_requests () > 0)
      ;

    return 0;
  }

  int handle_requests (void)
  {
    ACE_thread_t *tid =
      (ACE_thread_t*)this->tss_ctx_->get_attribute ("thread_id");
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) TSS TID: %d \n"),
                *tid));

    // do work.
    return -1;
  }

private:
  ACE_TSS<ClientContext> tss_ctx_;
};
// Listing 2

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  HA_CommandHandler handler;
  handler.activate (THR_NEW_LWP | THR_JOINABLE, 5);
  handler.wait ();
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_TSS<ClientContext>;
template class ACE_Hash_Map_Entry<const char*,void*>;
template class ACE_Hash_Map_Manager<const char *,void*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<const char*,void*,ACE_Hash<const char*>,ACE_Equal_To<const char*>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<const char*,void*,ACE_Hash<const char*>, ACE_Equal_To<const char*>, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_TSS<ClientContext>
#pragma instantiate ACE_Hash_Map_Entry<const char*,void*>
#pragma instantiate ACE_Hash_Map_Manager<const char *,void*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<const char*,void*,ACE_Hash<const char*>,ACE_Equal_To<const char*>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<const char*,void*,ACE_Hash<const char*>, ACE_Equal_To<const char*>, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts (ACE_TEXT ("This example requires threads."));
  return 0;
}

#endif /* ACE_HAS_THREADS */
