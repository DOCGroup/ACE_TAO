// $Id$

#include "ace/Event_Handler_T.h"

ACE_RCSID(Misc, test_event_handler_t, "$Id$")

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)

class ACE_Test_Sig_Handler
{
public:
  ACE_Test_Sig_Handler (void) {}
  virtual ACE_HANDLE get_handle (void) const { return 0; }
  virtual void set_handle (ACE_HANDLE) {}
  virtual int handle_async_io (ACE_HANDLE) { return 0; }
  virtual int shutdown (ACE_HANDLE, ACE_Reactor_Mask) { return 0; }
  virtual int signal_handler (ACE_HANDLE /* signum */,
                              siginfo_t * = 0,
                              ucontext_t * = 0)
  {
    return 0;
  }
};

int
main (int, char *[])
{
  typedef ACE_Event_Handler_T<ACE_Test_Sig_Handler> EH_SH;

  // Tie the ACE_Event_Handler_T together with the methods from ACE_Test_Sig_Handler.
  EH_SH tied_sh (new ACE_Test_Sig_Handler, 1,
                 &ACE_Test_Sig_Handler::get_handle,
                 &ACE_Test_Sig_Handler::handle_async_io,
                 &ACE_Test_Sig_Handler::shutdown,
                 &ACE_Test_Sig_Handler::signal_handler);
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Event_Handler_T<ACE_Test_Sig_Handler>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Event_Handler_T<ACE_Test_Sig_Handler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
int
main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR, "your platform does not support template typedefs\n"), 1);
}
#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */
