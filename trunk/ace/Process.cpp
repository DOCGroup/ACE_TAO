#define ACE_BUILD_DLL
// $Id$

#include "ace/Process.h"

#if !defined (__ACE_INLINE__)
#include "ace/Process.i"
#endif /* __ACE_INLINE__ */

int
ACE_Process::wait (void)
{
#if defined (ACE_WIN32)
  return ::WaitForSingleObject (process_info_.hProcess, INFINITE);
#else /* ACE_WIN32 */
  return ACE_OS::waitpid (this->child_id_, 0, 0);
#endif /* ACE_WIN32 */
}

ACE_Process::ACE_Process (void)
#if defined (ACE_WIN32)
  : set_handles_called_ (0)
#else /* ACE_WIN32 */
  : stdin_ (ACE_INVALID_HANDLE),
    stdout_ (ACE_INVALID_HANDLE),
    stderr_ (ACE_INVALID_HANDLE)
#endif /* ACE_WIN32 */
{
#if defined (ACE_WIN32)
  ACE_OS::memset ((void *) &startup_info_, 0, sizeof startup_info_);
  ACE_OS::memset ((void *) &process_info_, 0, sizeof process_info_);
  startup_info_.cb = sizeof startup_info_;
#endif /* ACE_WIN32 */
}

ACE_Process::~ACE_Process (void)
{
#if defined (ACE_WIN32)
  // Just in case start wasn't called.
  if (set_handles_called_)
    {
      ::CloseHandle (startup_info_.hStdInput);
      ::CloseHandle (startup_info_.hStdOutput);
      ::CloseHandle (startup_info_.hStdOutput);
      set_handles_called_ = 0;
    }

  // Free resources allocated in kernel.
  ACE_OS::close (process_info_.hThread);
  ACE_OS::close (process_info_.hProcess);

#endif /* ACE_WIN32 */
}

int
ACE_Process::set_handles (ACE_HANDLE std_in,
			  ACE_HANDLE std_out,
			  ACE_HANDLE std_err)
{
#if defined (ACE_WIN32)
  set_handles_called_ = 1;

  // Tell the new process to use our std handles.
  startup_info_.dwFlags = STARTF_USESTDHANDLES;

  if (std_in == ACE_INVALID_HANDLE)
    std_in = ACE_STDIN;
  if (std_out == ACE_INVALID_HANDLE)
    std_out = ACE_STDOUT;
  if (std_err == ACE_INVALID_HANDLE)
    std_err = ACE_STDERR;

  if (!::DuplicateHandle (::GetCurrentProcess(),
			  std_in,
			  ::GetCurrentProcess(),
			  &startup_info_.hStdInput,
			  NULL,
			  TRUE,
			  DUPLICATE_SAME_ACCESS))
    return -1;

  if (!::DuplicateHandle (::GetCurrentProcess(),
			  std_out,
			  ::GetCurrentProcess(),
			  &startup_info_.hStdOutput,
			  NULL,
			  TRUE,
			  DUPLICATE_SAME_ACCESS))
    return -1;

  if (!::DuplicateHandle (::GetCurrentProcess(),
			  std_err,
			  ::GetCurrentProcess(),
			  &startup_info_.hStdError,
			  NULL,
			  TRUE,
			  DUPLICATE_SAME_ACCESS))
    return -1;
#else /* ACE_WIN32 */
  stdin_ = std_in;
  stdout_ = std_out;
  stderr_ = std_err;
#endif /* ACE_WIN32 */

  return 0; // Success.
}

int
ACE_Process::start (char *argv[])
{
#if defined (ACE_WIN32)
  ACE_ARGV argv_buf (argv);

  char *buf = argv_buf.buf ();

  if (buf == 0)
    return -1;

  BOOL fork_result = 
    ::CreateProcess (NULL,
		     buf,
		     NULL, // No process attributes.
		     NULL,  // No thread attributes.
		     TRUE, // Allow handle inheritance.
		     NULL, // CREATE_NEW_CONSOLE, // Create a new console window.
		     NULL, // No environment.
		     NULL, // No current directory.
		     &startup_info_,
		     &process_info_);

  if (set_handles_called_)
    {
      ::CloseHandle (startup_info_.hStdInput);
      ::CloseHandle (startup_info_.hStdOutput);
      ::CloseHandle (startup_info_.hStdOutput);
      set_handles_called_ = 0;
    }

  if (fork_result) // If success.
    return 0;
  else
    // CreateProcess failed.
    return -1;
#else /* ACE_WIN32 */
  // Fork the new process.
  this->child_id_ = ACE_OS::fork ();

  switch (this->child_id_)
    {
    case -1:
      // Error.
      return -1;
    case 0:
      if (stdin_ != ACE_INVALID_HANDLE
	  && ACE_OS::dup2 (stdin_, ACE_STDIN) == -1)
	return -1;
      else if (stdout_ != ACE_INVALID_HANDLE
	       && ACE_OS::dup2 (stdout_, ACE_STDOUT) == -1)
	return -1;
      else if (stderr_ != ACE_INVALID_HANDLE
	       && ACE_OS::dup2 (stderr_, ACE_STDERR) == -1)
	return -1;

      // Child process executes the command.
      if (ACE_OS::execv (argv[0], argv) == -1)
	// If the execv fails, this child needs to exit.
	ACE_OS::exit (errno);
    default:
      // Server process.  The fork succeeded.
      return 0;
    }
#endif /* ACE_WIN32 */
}

ACE_Process::ACE_Process (char *argv[],
			  ACE_HANDLE std_in,
			  ACE_HANDLE std_out,
			  ACE_HANDLE std_err)
#if defined (ACE_WIN32)
  : set_handles_called_ (0)
#else /* ACE_WIN32 */
  : stdin_ (ACE_INVALID_HANDLE),
    stdout_ (ACE_INVALID_HANDLE),
    stderr_ (ACE_INVALID_HANDLE)
#endif /* ACE_WIN32 */
{
#if defined (ACE_WIN32)
  ACE_OS::memset ((void *) &startup_info_, 0, sizeof startup_info_);
  ACE_OS::memset ((void *) &process_info_, 0, sizeof process_info_);
  startup_info_.cb = sizeof startup_info_;
#endif /* ACE_WIN32 */

  if (this->set_handles (std_in, std_out, std_err) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "set_handles"));
  else if (this->start (argv) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "start"));
}
