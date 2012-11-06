//=============================================================================
/**
 *  @file    Directory_Changes.cpp
 *
 *  $Id$
 *
 *
 *  This application tests the working of WFMO_Reactor when users
 *  are interested in monitoring changes in the filesystem.
 *
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#include "ace/OS_main.h"

#if defined (ACE_WIN32)

#include "ace/Reactor.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/Log_Msg.h"



static int stop_test = 0;
static const ACE_TCHAR *directory = ACE_TEXT (".");
static const ACE_TCHAR *temp_file = ACE_TEXT ("foo");

class Event_Handler : public ACE_Event_Handler
{
public:
  Event_Handler (ACE_Reactor &reactor);
  ~Event_Handler (void);
  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask close_mask);

private:
  ACE_HANDLE handle_;
};

Event_Handler::Event_Handler (ACE_Reactor &reactor)
  : handle_ (ACE_INVALID_HANDLE)
{
  this->reactor (&reactor);

  int change_notification_flags = FILE_NOTIFY_CHANGE_FILE_NAME;

  this->handle_ = ACE_TEXT_FindFirstChangeNotification (directory,  // pointer to name of directory to watch
                                                        FALSE, // flag for monitoring directory or directory tree
                                                        change_notification_flags // filter conditions to watch for
                                                        );
  if (this->handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR ((LM_ERROR, "FindFirstChangeNotification could not be setup\n"));

  if (this->reactor ()->register_handler (this,
                                          this->handle_) != 0)
    ACE_ERROR ((LM_ERROR, "Registration with Reactor could not be done\n"));
}

Event_Handler::~Event_Handler (void)
{
}

int
Event_Handler::handle_signal (int, siginfo_t *, ucontext_t *)
{
  ::FindNextChangeNotification (this->handle_);
  if (stop_test)
    this->reactor ()->close ();
  return 0;
}

int
Event_Handler::handle_close (ACE_HANDLE,
                             ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "Event_Handler removed from Reactor\n"));
  ::FindCloseChangeNotification (this->handle_);
  return 0;
}

void
worker (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread creation\n"));
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread creating temporary file\n"));
  ACE_HANDLE file = ACE_OS::open (temp_file, _O_CREAT | _O_EXCL);
  if (file == ACE_INVALID_HANDLE)
    ACE_ERROR ((LM_ERROR, "Error in creating %s: %p\n", temp_file, "ACE_OS::open"));
  else
    {
      ACE_OS::close (file);
      ACE_DEBUG ((LM_DEBUG, "(%t) Thread sleeping\n"));
      ACE_OS::sleep (3);
      ACE_DEBUG ((LM_DEBUG, "(%t) Thread removing temporary file\n"));
      stop_test = 1;
      ACE_OS::unlink (temp_file);
    }
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_Reactor reactor;
  Event_Handler handler (reactor);

  int result = ACE_OS::thr_create ((ACE_THR_FUNC) worker, 0, 0, 0);
  ACE_TEST_ASSERT (result == 0);

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
#endif /* ACE_WIN32 */
