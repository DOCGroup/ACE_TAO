#define ACE_BUILD_DLL
// $Id$

#include "ace/Process.h"
#include "ace/ARGV.h"
#include "ace/SString.h"

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
  ACE_OS::memset ((void *) &this->startup_info_, 
		  0, sizeof this->startup_info_);
  ACE_OS::memset ((void *) &this->process_info_, 
		  0, sizeof this->process_info_);
  this->startup_info_.cb = sizeof this->startup_info_;
#endif /* ACE_WIN32 */
  this->cwd_[0] = '\0';
}

ACE_Process::~ACE_Process (void)
{
#if defined (ACE_WIN32)
  // Just in case <start> wasn't called.
  if (this->set_handles_called_)
    {
      ::CloseHandle (this->startup_info_.hStdInput);
      ::CloseHandle (this->startup_info_.hStdOutput);
      ::CloseHandle (this->startup_info_.hStdOutput);
      this->set_handles_called_ = 0;
    }

  // Free resources allocated in kernel.
  ACE_OS::close (this->process_info_.hThread);
  ACE_OS::close (this->process_info_.hProcess);
#endif /* ACE_WIN32 */
}

int
ACE_Process::set_handles (ACE_HANDLE std_in,
			  ACE_HANDLE std_out,
			  ACE_HANDLE std_err)
{
#if defined (ACE_WIN32)
  this->set_handles_called_ = 1;

  // Tell the new process to use our std handles.
  this->startup_info_.dwFlags = STARTF_USESTDHANDLES;

  if (std_in == ACE_INVALID_HANDLE)
    std_in = ACE_STDIN;
  if (std_out == ACE_INVALID_HANDLE)
    std_out = ACE_STDOUT;
  if (std_err == ACE_INVALID_HANDLE)
    std_err = ACE_STDERR;

  if (!::DuplicateHandle (::GetCurrentProcess(),
			  std_in,
			  ::GetCurrentProcess(),
			  &this->startup_info_.hStdInput,
			  NULL,
			  TRUE,
			  DUPLICATE_SAME_ACCESS))
    return -1;

  if (!::DuplicateHandle (::GetCurrentProcess(),
			  std_out,
			  ::GetCurrentProcess(),
			  &this->startup_info_.hStdOutput,
			  NULL,
			  TRUE,
			  DUPLICATE_SAME_ACCESS))
    return -1;

  if (!::DuplicateHandle (::GetCurrentProcess(),
			  std_err,
			  ::GetCurrentProcess(),
			  &this->startup_info_.hStdError,
			  NULL,
			  TRUE,
			  DUPLICATE_SAME_ACCESS))
    return -1;
#else /* ACE_WIN32 */
  this->stdin_ = std_in;
  this->stdout_ = std_out;
  this->stderr_ = std_err;
#endif /* ACE_WIN32 */

  return 0; // Success.
}

int
ACE_Process::set_cwd (const TCHAR *cwd)
{
  ACE_OS::strncpy (this->cwd_, cwd, MAXPATHLEN);
  // This is for paranoia...
  this->cwd_[MAXPATHLEN] = '\0';
  return 0;
}

pid_t
ACE_Process::start (char *argv[], char *envp[])
{
#if defined (ACE_WIN32)
  ACE_ARGV argv_buf (argv);

  LPTSTR buf = (LPTSTR) ACE_WIDE_STRING (argv_buf.buf ());

  if (buf == 0)
    return -1;

  // If there is no current working directory, we *MUST* pass 0, not "".
  char *cwd = strlen (cwd_) == 0 ? 0 : cwd_;

  BOOL fork_result = 
    ::CreateProcess (NULL,
		     buf,
		     NULL, // No process attributes.
		     NULL, // No thread attributes.
		     TRUE, // Allow handle inheritance.
		     NULL, // CREATE_NEW_CONSOLE, // Create a new console window.
		     envp, // Environment.
		     cwd, // Current directory to start in.
		     &this->startup_info_,
		     &this->process_info_);

  if (set_handles_called_)
    {
      ::CloseHandle (this->startup_info_.hStdInput);
      ::CloseHandle (this->startup_info_.hStdOutput);
      ::CloseHandle (this->startup_info_.hStdError);
      this->set_handles_called_ = 0;
    }

  if (fork_result) // If success.
    return 0;
  else
    // CreateProcess failed.
    return -1;
#else /* ACE_WIN32 */
  // Fork the new process.
  this->child_id_ = ACE_OS::fork (argv == 0 ? "child" : argv[1]);

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

      // If we must, set the working directory for the child process.
      if (this->cwd_[0] != '\0')
	::chdir (cwd_);

      if (argv != 0)
	{
	  // Child process executes the command.
	  int result;
      
	  if (envp == 0)
	    result = ACE_OS::execvp (argv[0], argv);
	  else
	    result = ACE_OS::execve (argv[0], argv, envp);

	  if (result == -1)
	    // If the execv fails, this child needs to exit.
	    ACE_OS::exit (errno);
	}
      return 0;
      /* NOTREACHED */
    default:
      // Server process.  The fork succeeded.
      return this->child_id_;
    }
#endif /* ACE_WIN32 */
}

ACE_Process::ACE_Process (char *argv[],
			  ACE_HANDLE std_in,
			  ACE_HANDLE std_out,
			  ACE_HANDLE std_err,
			  char *envp[])
#if defined (ACE_WIN32)
  : set_handles_called_ (0)
#else /* ACE_WIN32 */
  : stdin_ (ACE_INVALID_HANDLE),
    stdout_ (ACE_INVALID_HANDLE),
    stderr_ (ACE_INVALID_HANDLE)
#endif /* ACE_WIN32 */
{
#if defined (ACE_WIN32)
  ACE_OS::memset ((void *) &this->startup_info_, 
		  0, 
		  sizeof this->startup_info_);
  ACE_OS::memset ((void *) &this->process_info_, 
		  0, 
		  sizeof this->process_info_);
  this->startup_info_.cb = sizeof this->startup_info_;
#endif /* ACE_WIN32 */
  this->cwd_[0] = '\0';

  if (this->set_handles (std_in, std_out, std_err) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "set_handles"));
  else if (this->start (argv, envp) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "start"));
}
