// $Id$
//
// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    test_directory_changes.cpp
//
// = DESCRIPTION
//
//    This application tests the working of ReactorEx when users are
//    interested in monitoring changes in the filesystem.
//
// = AUTHOR
//    Irfan Pyarali
// 
// ============================================================================

#include "ace/ReactorEx.h"

static int stop_test = 0;
static LPCTSTR directory = __TEXT (".");
static LPCTSTR temp_file = __TEXT ("foo");

class Event_Handler : public ACE_Event_Handler
{
public:
  Event_Handler (ACE_ReactorEx &reactorEx);
  ~Event_Handler (void);
  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);  
  int handle_close (ACE_HANDLE handle,
		    ACE_Reactor_Mask close_mask);

private:
  ACE_HANDLE handle_;
};

Event_Handler::Event_Handler (ACE_ReactorEx &reactorEx)
  : handle_ (ACE_INVALID_HANDLE)
{
  this->reactorEx (&reactorEx);

  int change_notification_flags = FILE_NOTIFY_CHANGE_FILE_NAME; 

  this->handle_ = ::FindFirstChangeNotification (directory,  // pointer to name of directory to watch  
						 FALSE,	// flag for monitoring directory or directory tree  
						 change_notification_flags // filter conditions to watch for 
						 );
  if (this->handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR ((LM_ERROR, "FindFirstChangeNotification could not be setup\n"));
  
  if (this->reactorEx ()->register_handler (this,
					    this->handle_) != 0)
    ACE_ERROR ((LM_ERROR, "Registration with ReactorEx could not be done\n"));		    
}

Event_Handler::~Event_Handler (void)
{
}

int 
Event_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Something changed in this directory\n"));
  ::FindNextChangeNotification (this->handle_);
  if (stop_test)
    this->reactorEx ()->close ();
  return 0;
}

int 
Event_Handler::handle_close (ACE_HANDLE handle,
			     ACE_Reactor_Mask close_mask)
{
  ACE_DEBUG ((LM_DEBUG, "Event_Handler removed from ReactorEx\n"));
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
main (void)
{
  ACE_ReactorEx reactorEx;
  Event_Handler handler (reactorEx);

  ACE_ASSERT (ACE_OS::thr_create ((ACE_THR_FUNC) worker, 0, 0, 0) == 0);

  int result = 0;
  while (result != -1)
    result = reactorEx.handle_events ();

  return 0;
}
