// $Id$
//
// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    test_handle_close.cpp
//
// = DESCRIPTION
//
//    This application tests whether handle_close gets called and if
//    the correct masks are passed along.  The handler should get
//    handle_close called for all three masks (READ, WRITE, and
//    EXCEPT).
//
// = AUTHOR
//    Irfan Pyarali
// 
// ============================================================================

#include "ace/Get_Opt.h"
#include "ace/Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/Auto_Ptr.h"
#include "ace/Pipe.h"

ACE_RCSID(WFMO_Reactor, test_handle_close, "$Id$")

// Use the WFMO_Reactor
static int opt_wfmo_reactor = 0;

// Use the Select_Reactor
static int opt_select_reactor = 0;

// Make pipe readable in main()
static int write_to_pipe_in_main = 0;

// Write some data to the pipe.  This will cause handle_input to get
// called.
void
write_to_pipe (ACE_Pipe &pipe)
{
  char *data = "hello";
  int len = ACE_OS::strlen (data);
  
  int result = ACE::send (pipe.write_handle (),
                          data, 
                          len);
  ACE_ASSERT (result == len);
}

// Simple handler class
class Handler : public ACE_Event_Handler
{
public:
  Handler (ACE_Pipe &pipe)
    : pipe_ (pipe)
  {
  }

  ACE_HANDLE get_handle (void) const
  {
    return this->pipe_.read_handle ();
  }

  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask close_mask)
  {
    ACE_DEBUG ((LM_DEBUG, 
                "Handle closed called with mask = %d\n",
                close_mask));
    return 0;
  }

  int handle_input (ACE_HANDLE handle)
  {
    // Remove for reading
    return -1;
  }

  int handle_output (ACE_HANDLE handle)
  {
    // Write to the pipe; this causes handle_input to get called.
    if (!write_to_pipe_in_main)
      write_to_pipe (this->pipe_);

    // Remove for writing
    return -1;
  }

protected:
  ACE_Pipe &pipe_;
};

class Different_Handler : public Handler
{
public:

  Different_Handler (ACE_Pipe &pipe)
    : Handler (pipe)
  {    
  }

  int handle_input (ACE_HANDLE handle)
  {
    // Remove for reading
    int result = ACE_Reactor::instance ()->remove_handler (this,
                                                           ACE_Event_Handler::READ_MASK);
    ACE_ASSERT (result == 0);

    return 0;
  }

  int handle_output (ACE_HANDLE handle)
  {
    // Remove for writing
    ACE_Reactor_Mask mask = ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::DONT_CALL;
    int result = ACE_Reactor::instance ()->remove_handler (this,
                                                           mask);
    ACE_ASSERT (result == 0);

    // Write to the pipe; this causes handle_input to get called.
    if (!write_to_pipe_in_main)
      write_to_pipe (this->pipe_);

    return 0;
  }
};


//
// Selection of which reactor should get created
//
void
create_reactor (void)
{
  ACE_Reactor_Impl *impl = 0;

  if (opt_wfmo_reactor)
    {
#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
      ACE_NEW (impl,
               ACE_WFMO_Reactor);
#endif /* ACE_WIN32 */
    }
  else if (opt_select_reactor)
    ACE_NEW (impl,
             ACE_Select_Reactor);

  ACE_Reactor *reactor = 0;
  ACE_NEW (reactor,
           ACE_Reactor (impl));
  ACE_Reactor::instance (reactor);
}

int
main (int argc, char *argv[])
{
  int result = 0;

  // Parse args
  ACE_Get_Opt getopt (argc, argv, ASYS_TEXT ("swm"), 1);
  for (int c; (c = getopt ()) != -1; )
    switch (c)
      {
      case 's':
        opt_select_reactor = 1;
        break;
      case 'w':
        opt_wfmo_reactor = 1;
        break;
      case 'm':
        write_to_pipe_in_main = 1;
        break;
      }

  // Create pipes
  ACE_Pipe pipe1, pipe2;

  result = pipe1.open ();
  ACE_ASSERT (result == 0);

  result = pipe2.open ();
  ACE_ASSERT (result == 0);

  // Create handlers
  Handler handler (pipe1);
  Different_Handler different_handler (pipe2);

  // Create reactor
  create_reactor ();

  // Manage memory automagically.
  auto_ptr<ACE_Reactor> reactor (ACE_Reactor::instance ());
  auto_ptr<ACE_Reactor_Impl> impl;

  // If we are using other that the default implementation, we must
  // clean up.
  if (opt_select_reactor || opt_wfmo_reactor)
    impl = auto_ptr<ACE_Reactor_Impl> (ACE_Reactor::instance ()->implementation ());
  
  // Register handlers
  ACE_Reactor_Mask mask = 
    ACE_Event_Handler::READ_MASK | 
    ACE_Event_Handler::WRITE_MASK |
    ACE_Event_Handler::EXCEPT_MASK;
    
  result = ACE_Reactor::instance ()->register_handler (&handler,
                                                       mask);
  ACE_ASSERT (result == 0);

  result = ACE_Reactor::instance ()->register_handler (&different_handler,
                                                       mask);
  ACE_ASSERT (result == 0);
  
  // Write to the pipe; this causes handle_input to get called.  
  if (write_to_pipe_in_main)
    {
      write_to_pipe (pipe1);
      write_to_pipe (pipe2);
    }      

  // Note that handle_output will get called automatically since the
  // pipe is writable!

  // Run for three seconds
  ACE_Time_Value time (3);
  ACE_Reactor::instance ()->run_event_loop (time);

  return 0;
}
