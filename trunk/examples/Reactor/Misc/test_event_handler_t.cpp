
// $Id$

#include "ace/Event_Handler_T.h"

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)

class ACE_Sig_Handler
{
public:
  ACE_Sig_Handler (void) {}
  virtual ACE_HANDLE get_handle (void) const { return 0; }
  virtual void set_handle (ACE_HANDLE) {}
  virtual int handle_async_io (ACE_HANDLE) { return 0; }
  virtual int shutdown (ACE_HANDLE, ACE_Reactor_Mask) { return 0; }
  virtual int signal_handler (ACE_HANDLE signum
#if defined (ACE_HAS_SIGINFO_T)
    , siginfo_t * = 0, ucontext_t * = 0
#endif /* ACE_HAS_SIGINFO_T */
    )
  {
    return 0;
  }
};

int 
main (int, char *[])
{
  typedef ACE_Event_Handler_T<ACE_Sig_Handler> EH_SH;

  // Tie the ACE_Event_Handler_T together with the methods from ACE_Sig_Handler.
  EH_SH tied_sh (new ACE_Sig_Handler, 1,
		 &ACE_Sig_Handler::get_handle,
		 &ACE_Sig_Handler::handle_async_io,
		 &ACE_Sig_Handler::shutdown,
		 &ACE_Sig_Handler::signal_handler);
  return 0;
}
#else
int
main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR, "your platform does not support template typedefs\n"), 1);
}
#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */
