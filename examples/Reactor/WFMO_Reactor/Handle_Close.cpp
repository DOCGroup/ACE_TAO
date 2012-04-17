//=============================================================================
/**
 *  @file    Handle_Close.cpp
 *
 *  $Id$
 *
 *
 *  This application tests whether handle_close gets called and if
 *  the correct masks are passed along.  The handler should get
 *  handle_close called for all three masks (READ, WRITE, and
 *  EXCEPT).
 *
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "ace/Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/Auto_Ptr.h"
#include "ace/Pipe.h"
#include "ace/OS_main.h"



// Use the WFMO_Reactor
static int opt_wfmo_reactor = 0;

// Use the Select_Reactor
static int opt_select_reactor = 0;

// Make pipe readable in main()
static int write_to_pipe_in_main = 0;

// Cancel reads
static int cancel_reads = 0;

// Write some data to the pipe.  This will cause handle_input to get
// called.
void
write_to_pipe (ACE_Pipe &pipe)
{
  const char *data = "hello";
  size_t len = ACE_OS::strlen (data);

  ssize_t result = ACE::send (pipe.write_handle (),
                              data,
                              len);
  ACE_TEST_ASSERT ((size_t)result == len);
}

// Simple handler class
class Handler : public ACE_Event_Handler
{
public:
  Handler (ACE_Pipe &pipe)
    : pipe_ (pipe)
  {
  }

  ~Handler (void)
  {
    this->reactor (0);
  }

  ACE_HANDLE get_handle (void) const
  {
    return this->pipe_.read_handle ();
  }

  int handle_close (ACE_HANDLE,
                    ACE_Reactor_Mask close_mask)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Handler::handle_close called with mask = %d\n",
                close_mask));
    return 0;
  }

  int handle_input (ACE_HANDLE)
  {
    ACE_DEBUG ((LM_DEBUG, "Handler::handle_input\n"));

    // Remove for reading
    return -1;
  }

  int handle_output (ACE_HANDLE handle)
  {
    ACE_UNUSED_ARG (handle);
    ACE_DEBUG ((LM_DEBUG, "Handler::handle_output\n"));

    // Optionally cancel reads
    if (cancel_reads)
      {
        int result = this->reactor ()->cancel_wakeup (this,
                                                      ACE_Event_Handler::READ_MASK);
        ACE_TEST_ASSERT (result != -1);
      }

    // Write to the pipe; this causes handle_input to get called.
    if (!write_to_pipe_in_main)
      write_to_pipe (this->pipe_);

    // Remove for writing
    return -1;
  }

protected:
  ACE_Pipe &pipe_;
};

class Different_Handler : public ACE_Event_Handler
{
public:

  Different_Handler (ACE_Pipe &pipe)
    : pipe_ (pipe)
  {
  }

  ~Different_Handler (void)
  {
    this->reactor (0);
  }

  ACE_HANDLE get_handle (void) const
  {
    return this->pipe_.read_handle ();
  }

  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask close_mask)
  {
    ACE_UNUSED_ARG (handle);
    ACE_DEBUG ((LM_DEBUG,
                "Different_Handler::handle_close called with mask = %d\n",
                close_mask));
    return 0;
  }

  int handle_input (ACE_HANDLE handle)
  {
    ACE_UNUSED_ARG (handle);
    ACE_DEBUG ((LM_DEBUG, "Different_Handler::handle_input\n"));

    // Remove for reading
    int result = this->reactor ()->remove_handler (this,
                                                   ACE_Event_Handler::READ_MASK);
    ACE_TEST_ASSERT (result == 0);

    return 0;
  }

  int handle_output (ACE_HANDLE handle)
  {
    ACE_UNUSED_ARG (handle);
    ACE_DEBUG ((LM_DEBUG, "Different_Handler::handle_output\n"));

    // Add for reading
    int result = this->reactor ()->mask_ops (this,
                                             ACE_Event_Handler::READ_MASK,
                                             ACE_Reactor::ADD_MASK);
    ACE_TEST_ASSERT (result != -1);

    ACE_Reactor_Mask old_masks =
      ACE_Event_Handler::WRITE_MASK |
      ACE_Event_Handler::EXCEPT_MASK;

    ACE_TEST_ASSERT (old_masks ==
                static_cast<ACE_Reactor_Mask> (result));

    // Get new masks
    result = this->reactor ()->mask_ops (this,
                                         ACE_Event_Handler::NULL_MASK,
                                         ACE_Reactor::GET_MASK);
    ACE_TEST_ASSERT (result != -1);

    ACE_Reactor_Mask current_masks =
      ACE_Event_Handler::READ_MASK |
      ACE_Event_Handler::WRITE_MASK |
      ACE_Event_Handler::EXCEPT_MASK;

    ACE_TEST_ASSERT (current_masks ==
                static_cast<ACE_Reactor_Mask> (result));

    // Remove for writing
    ACE_Reactor_Mask mask = ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::DONT_CALL;
    result = this->reactor ()->remove_handler (this,
                                               mask);
    ACE_TEST_ASSERT (result == 0);

    // Write to the pipe; this causes handle_input to get called.
    if (!write_to_pipe_in_main)
      write_to_pipe (this->pipe_);

    return 0;
  }

protected:
  ACE_Pipe &pipe_;
};


//
// Selection of which reactor should get created
//
ACE_Reactor *
create_reactor (void)
{
  ACE_Reactor_Impl *impl = 0;

  if (opt_wfmo_reactor)
    {
#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
      ACE_NEW_RETURN (impl,
                      ACE_WFMO_Reactor,
                      0);
#endif /* ACE_WIN32 */
    }
  else if (opt_select_reactor)
    {
      ACE_NEW_RETURN (impl,
                      ACE_Select_Reactor,
                      0);
    }
  else
    {
      ACE_Reactor *singleton_reactor =
        ACE_Reactor::instance ();
      ACE_Reactor::instance (0);
      return singleton_reactor;
    }

  ACE_Reactor *reactor = 0;
  ACE_NEW_RETURN (reactor,
                  ACE_Reactor (impl,
                               1),
                  0);

  return reactor;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result = 0;

  //FUZZ: disable check_for_lack_ACE_OS
  // Parse args
  ACE_Get_Opt getopt (argc, argv, ACE_TEXT ("swmc"), 1);

  for (int c; (c = getopt ()) != -1; )
  //FUZZ: enable check_for_lack_ACE_OS
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
      case 'c':
        cancel_reads = 1;
        break;
      }

  // Create pipes
  ACE_Pipe pipe1, pipe2;

  result = pipe1.open ();
  ACE_TEST_ASSERT (result == 0);

  result = pipe2.open ();
  ACE_TEST_ASSERT (result == 0);

  // Create handlers
  Handler handler (pipe1);
  Different_Handler different_handler (pipe2);

  // Manage memory automagically.
  auto_ptr<ACE_Reactor> reactor (create_reactor ());

  // Register handlers
  ACE_Reactor_Mask handler_mask =
    ACE_Event_Handler::READ_MASK |
    ACE_Event_Handler::WRITE_MASK |
    ACE_Event_Handler::EXCEPT_MASK;

  ACE_Reactor_Mask different_handler_mask =
    ACE_Event_Handler::WRITE_MASK |
    ACE_Event_Handler::EXCEPT_MASK;

  result = reactor->register_handler (&handler,
                                      handler_mask);
  ACE_TEST_ASSERT (result == 0);

  result = reactor->register_handler (&different_handler,
                                      different_handler_mask);
  ACE_TEST_ASSERT (result == 0);

  // Write to the pipe; this causes handle_input to get called.
  if (write_to_pipe_in_main)
    {
      write_to_pipe (pipe1);
      write_to_pipe (pipe2);
    }

  // Note that handle_output will get called automatically since the
  // pipe is writable!

  // Run for three seconds
  ACE_Time_Value wait_time (3);

  reactor->run_reactor_event_loop (wait_time);

  ACE_DEBUG ((LM_DEBUG, "\nClosing down the application\n\n"));

  return 0;
}
