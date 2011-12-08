//=============================================================================
/**
 *  @file    Registry_Changes.cpp
 *
 *  $Id$
 *
 *
 *  This application tests the working of Reactor when users are
 *  interested in monitoring changes in the registry.
 *
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#include "ace/OS_main.h"

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_REGISTRY) && !defined (ACE_LACKS_REGNOTIFYCHANGEKEYVALUE)

#include "ace/Reactor.h"
#include "ace/Registry.h"
#include "ace/Auto_Event.h"
#include "ace/OS_NS_unistd.h"



static int stop_test = 0;
static HKEY context_to_monitor = HKEY_CURRENT_USER;
static const ACE_TCHAR *temp_context_name = ACE_TEXT ("ACE temporary context");

class Event_Handler : public ACE_Event_Handler
{
public:
  Event_Handler (ACE_Reactor &reactor);
  ~Event_Handler (void);
  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask close_mask);
  ACE_Registry::Naming_Context &context (void);

private:
  ACE_Auto_Event event_;
  ACE_Registry::Naming_Context context_;
};

Event_Handler::Event_Handler (ACE_Reactor &reactor)
  : context_ (context_to_monitor)
{
  this->reactor (&reactor);

  if (::RegNotifyChangeKeyValue (this->context_.key (), // handle of key to watch
                                 FALSE, // flag for subkey notification
                                 REG_NOTIFY_CHANGE_NAME, // changes to be reported
                                 this->event_.handle (), // handle of signaled event
                                 TRUE // flag for asynchronous reporting
                                 ) != ERROR_SUCCESS)
    ACE_ERROR ((LM_ERROR, "RegNotifyChangeKeyValue could not be setup\n"));

  if (this->reactor ()->register_handler (this,
                                          this->event_.handle ()) != 0)
    ACE_ERROR ((LM_ERROR, "Registration with Reactor could not be done\n"));
}

Event_Handler::~Event_Handler (void)
{
}

int
Event_Handler::handle_signal (int, siginfo_t *, ucontext_t *)
{
  if (stop_test)
    this->reactor ()->close ();
  else if (::RegNotifyChangeKeyValue (this->context_.key (), // handle of key to watch
                                      FALSE, // flag for subkey notification
                                      REG_NOTIFY_CHANGE_NAME, // changes to be reported
                                      this->event_.handle (), // handle of signaled event
                                      TRUE // flag for asynchronous reporting
                                      ) != ERROR_SUCCESS)
    ACE_ERROR ((LM_ERROR,
                "RegNotifyChangeKeyValue could not be setup\n"));
  return 0;
}

int
Event_Handler::handle_close (ACE_HANDLE,
                             ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "Event_Handler removed from Reactor\n"));
  return 0;
}

ACE_Registry::Naming_Context &
Event_Handler::context (void)
{
  return this->context_;
}

void
worker (Event_Handler *event_handler)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread creation\n"));
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread creating temporary registry entry\n"));

  ACE_Registry::Naming_Context temp_context;
  int result = event_handler->context ().bind_new_context (temp_context_name,
                                                           temp_context);

  if (result == -1)
    ACE_ERROR ((LM_ERROR, "Error in creating %s: %p\n", temp_context_name, "bind_new_context"));
  else
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) Thread sleeping\n"));
      ACE_OS::sleep (3);

      ACE_DEBUG ((LM_DEBUG, "(%t) Thread removing registry entry\n"));
      stop_test = 1;
      event_handler->context ().unbind_context (temp_context_name);
    }
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_Reactor reactor;
  Event_Handler handler (reactor);

  int result = ACE_OS::thr_create ((ACE_THR_FUNC) worker, &handler, 0, 0);
  ACE_ASSERT (result == 0);

  for (result = 0; result != -1; result = reactor.handle_events ())
    continue;

  return 0;
}
#else /* !ACE_WIN32 */
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  return 0;
}
#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_REGISTRY */
