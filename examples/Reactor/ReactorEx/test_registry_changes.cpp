// $Id$
//
// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    test_registry_changes.cpp
//
// = DESCRIPTION
//
//    This application tests the working of ReactorEx when users are
//    interested in monitoring changes in the registry.
//
// = AUTHOR
//    Irfan Pyarali
// 
// ============================================================================

#include "ace/ReactorEx.h"
#include "ace/Registry.h"

static int stop_test = 0;
static HKEY context_to_monitor = HKEY_CURRENT_USER;
static LPCTSTR temp_context_name = __TEXT ("ACE temporary context");

class Event_Handler : public ACE_Event_Handler
{
public:
  Event_Handler (ACE_ReactorEx &reactorEx);
  ~Event_Handler (void);
  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);  
  int handle_close (ACE_HANDLE handle,
		    ACE_Reactor_Mask close_mask);
  ACE_Registry::Naming_Context &context (void);
  
private:
  ACE_Auto_Event event_;
  ACE_Registry::Naming_Context context_;
};

Event_Handler::Event_Handler (ACE_ReactorEx &reactorEx)
  : context_ (context_to_monitor)
{
  this->reactorEx (&reactorEx);
  
  if (::RegNotifyChangeKeyValue (this->context_.key (), // handle of key to watch 
				 FALSE, // flag for subkey notification 
				 REG_NOTIFY_CHANGE_NAME, // changes to be reported 
				 this->event_.handle (), // handle of signaled event 
				 TRUE // flag for asynchronous reporting  
				 ) != ERROR_SUCCESS)
    ACE_ERROR ((LM_ERROR, "RegNotifyChangeKeyValue could not be setup\n"));
  
  if (this->reactorEx ()->register_handler (this,
					    this->event_.handle ()) != 0)
    ACE_ERROR ((LM_ERROR, "Registration with ReactorEx could not be done\n"));		    
}

Event_Handler::~Event_Handler (void)
{
}

int 
Event_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Something changed in the Registry\n"));

  if (stop_test)
    this->reactorEx ()->close ();
  else if (::RegNotifyChangeKeyValue (this->context_.key (), // handle of key to watch 
				      FALSE, // flag for subkey notification 
				      REG_NOTIFY_CHANGE_NAME, // changes to be reported 
				      this->event_.handle (), // handle of signaled event 
				      TRUE // flag for asynchronous reporting  
				      ) != ERROR_SUCCESS)
    ACE_ERROR ((LM_ERROR, "RegNotifyChangeKeyValue could not be setup\n"));
  
  return 0;
}

int 
Event_Handler::handle_close (ACE_HANDLE handle,
			     ACE_Reactor_Mask close_mask)
{
  ACE_DEBUG ((LM_DEBUG, "Event_Handler removed from ReactorEx\n"));
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
main (void)
{
  ACE_ReactorEx reactorEx;
  Event_Handler handler (reactorEx);

  ACE_ASSERT (ACE_OS::thr_create ((ACE_THR_FUNC) worker, &handler, 0, 0) == 0);

  int result = 0;
  while (result != -1)
    result = reactorEx.handle_events ();

  return 0;
}
