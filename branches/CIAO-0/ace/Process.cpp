// $Id$

#include "ace/OS.h"
#include "ace/Process.h"
#include "ace/ARGV.h"
#include "ace/Signal.h"
#include "ace/SString.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "ace/Process.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (ace, Process, "$Id$")


// This function acts as a signal handler for SIGCHLD. We don't really want
// to do anything with the signal - it's just needed to interrupt a sleep.
// See wait() for more info.
#if !defined (ACE_WIN32)
static void
sigchld_nop (int, siginfo_t *, ucontext_t *)
{
  return;
}
#endif /* ACE_WIN32 */



ACE_Process::ACE_Process (void)
  :
#if !defined (ACE_WIN32)
  child_id_ (ACE_INVALID_PID),
#endif /* !defined (ACE_WIN32) */
  exit_code_ (0)
{
#if defined (ACE_WIN32)
  ACE_OS::memset ((void *) &this->process_info_,
                  0,
                  sizeof this->process_info_);
#endif /* ACE_WIN32 */
}

ACE_Process::~ACE_Process (void)
{
#if defined (ACE_WIN32)
  // Free resources allocated in kernel.
  ACE_OS::close (this->process_info_.hThread);
  ACE_OS::close (this->process_info_.hProcess);
#endif /* ACE_WIN32 */
  // If any handles were duplicated for the child process and
  // still not closed, get them now.
  this->close_dup_handles ();
}

int
ACE_Process::prepare (ACE_Process_Options &)
{
  return 0;
}

pid_t
ACE_Process::spawn (ACE_Process_Options &options)
{
  if (prepare (options) < 0)
    return ACE_INVALID_PID;

  // Stash the passed/duped handle sets away in this object for later
  // closing if needed or requested. At the same time, figure out which
  // ones to include in command line options if that's needed below.
  ACE_Handle_Set *set_p = 0;
  if (options.dup_handles (this->dup_handles_))
    set_p = &this->dup_handles_;
  else if (options.passed_handles (this->handles_passed_))
    set_p = &this->handles_passed_;

  // If we are going to end up running a new program (i.e. Win32, or
  // NO_EXEC option is set) then get any handles passed in the options,
  // and tack them onto the command line with +H <handle> options,
  // unless the command line runs out of space.
  // Note that we're using the knowledge that all the options, argvs, etc.
  // passed to the options are all sitting in the command_line_buf. Any
  // call to get the argv then splits them out. So, regardless of the
  // platform, tack them all onto the command line buf and take it
  // from there.
  if (set_p && !ACE_BIT_ENABLED (options.creation_flags (),
                                 ACE_Process_Options::NO_EXEC))
    {
      int maxlen = 0;
      ACE_TCHAR *cmd_line_buf = options.command_line_buf (&maxlen);
      size_t max_len = ACE_static_cast (size_t, maxlen);
      size_t curr_len = ACE_OS::strlen (cmd_line_buf);
      ACE_Handle_Set_Iterator h_iter (*set_p);
      // Because the length of the to-be-formatted +H option is not
      // known, and we don't have a snprintf, guess at the space
      // needed (20 chars), and use that as a limit.
      for (ACE_HANDLE h = h_iter ();
           h != ACE_INVALID_HANDLE && curr_len + 20 < max_len;
           h = h_iter ())
        {
          curr_len += ACE_OS::sprintf (&cmd_line_buf[curr_len],
                                       ACE_LIB_TEXT (" +H %d"),
                                       h);
        }
    }

#if defined (ACE_HAS_WINCE)
  // Note that WinCE does not have process name included in the command line as argv[0]
  // like other OS environment.  Therefore, it is user's whole responsibility to call
  // 'ACE_Process_Options::process_name(const ACE_TCHAR *name)' to set the proper
  // process name (the execution file name with path if needed).

  BOOL fork_result =
    ACE_TEXT_CreateProcess (options.process_name(),
                            options.command_line_buf(),
                            options.get_process_attributes(),  // must be NULL in CE
                            options.get_thread_attributes(),   // must be NULL in CE
                            options.handle_inheritence(),      // must be false in CE
                            options.creation_flags(),          // must be NULL in CE
                            options.env_buf(),                 // environment variables, must be NULL in CE
                            options.working_directory(),       // must be NULL in CE
                            options.startup_info(),            // must be NULL in CE
                            &this->process_info_);

  if (fork_result)
    {
      parent (this->getpid ());
      return this->getpid ();
    }
  return ACE_INVALID_PID;

#elif defined (ACE_WIN32)
  BOOL fork_result =
    ACE_TEXT_CreateProcess (0,
                            options.command_line_buf (),
                            options.get_process_attributes (),
                            options.get_thread_attributes (),
                            options.handle_inheritence (),
                            options.creation_flags (),
                            options.env_buf (), // environment variables
                            options.working_directory (),
                            options.startup_info (),
                            &this->process_info_);

  if (fork_result)
    {
      parent (this->getpid ());
      return this->getpid ();
    }
  return ACE_INVALID_PID;

#elif defined (CHORUS)
  // This only works if we exec.  Chorus does not really support
  // forking.
  if (ACE_BIT_ENABLED (options.creation_flags (),
                       ACE_Process_Options::NO_EXEC))
    ACE_NOTSUP_RETURN (ACE_INVALID_PID);

  // These are all currently unsupported.
  if (options.get_stdin () != ACE_INVALID_HANDLE)
    ACE_NOTSUP_RETURN (ACE_INVALID_PID);
  if (options.get_stdout () != ACE_INVALID_HANDLE)
    ACE_NOTSUP_RETURN (ACE_INVALID_PID);
  if (options.get_stderr () != ACE_INVALID_HANDLE)
    ACE_NOTSUP_RETURN (ACE_INVALID_PID);
  if (options.working_directory () != 0)
    ACE_NOTSUP_RETURN (ACE_INVALID_PID);

  if (options.env_argv ()[0] == 0)
    // command-line args
    this->child_id_ = ACE_OS::execvp (options.process_name (),
                                      options.command_line_argv ());
  else
    {
      // Add the new environment variables to the environment context
      // of the context before doing an <execvp>.
      for (char *const *user_env = options.env_argv ();
           *user_env != 0;
           user_env++)
        if (ACE_OS::putenv (*user_env) != 0)
          return ACE_INVALID_PID;

      // Now the forked process has both inherited variables and the
      // user's supplied variables.
      this->child_id_ = ACE_OS::execvp (options.process_name (),
                                        options.command_line_argv ());
    }

  return this->child_id_;
#else /* ACE_WIN32 */
  // Fork the new process.
  this->child_id_ = ACE::fork (options.process_name (),
                               options.avoid_zombies ());

  if (this->child_id_ == 0)
    {
# if !defined (ACE_LACKS_SETPGID)
      // If we're the child and the options specified a non-default
      // process group, try to set our pgid to it.  This allows the
      // <ACE_Process_Manager> to wait for processes by their
      // process-group.
      if (options.getgroup () != ACE_INVALID_PID
          && ACE_OS::setpgid (0,
                              options.getgroup ()) < 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("%p.\n"),
                    ACE_LIB_TEXT ("ACE_Process::spawn: setpgid failed.")));
# endif /* ACE_LACKS_SETPGID */

# if !defined (ACE_LACKS_SETREGID)
      if (options.getrgid () != (uid_t) -1
          || options.getegid () != (uid_t) -1)
        if (ACE_OS::setregid (options.getrgid (),
                              options.getegid ()) == -1)
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("%p.\n"),
                      ACE_LIB_TEXT ("ACE_Process::spawn: setregid failed.")));
# endif /* ACE_LACKS_SETREGID */

# if !defined (ACE_LACKS_SETREUID)
      // Set user and group id's.
      if (options.getruid () != (uid_t) -1
          || options.geteuid () != (uid_t) -1)
        if (ACE_OS::setreuid (options.getruid (),
                              options.geteuid ()) == -1)
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("%p.\n"),
                      ACE_LIB_TEXT ("ACE_Process::spawn: setreuid failed.")));
# endif /* ACE_LACKS_SETREUID */

      this->child (ACE_OS::getppid ());
    }
  else if (this->child_id_ != -1)
    this->parent (this->child_id_);

  // If we're not supposed to exec, return the process id.
  if (ACE_BIT_ENABLED (options.creation_flags (),
                       ACE_Process_Options::NO_EXEC))
    return this->child_id_;

  switch (this->child_id_)
    {
    case -1:
      // Error.
      return ACE_INVALID_PID;
    case 0:
      // Child process...exec the
      {
        if (options.get_stdin () != ACE_INVALID_HANDLE
            && ACE_OS::dup2 (options.get_stdin (),
                             ACE_STDIN) == -1)
          ACE_OS::exit (errno);
        else if (options.get_stdout () != ACE_INVALID_HANDLE
                 && ACE_OS::dup2 (options.get_stdout (),
                                  ACE_STDOUT) == -1)
          ACE_OS::exit (errno);
        else if (options.get_stderr () != ACE_INVALID_HANDLE
                 && ACE_OS::dup2 (options.get_stderr (),
                                  ACE_STDERR) == -1)
          ACE_OS::exit (errno);

        // close down unneeded descriptors
        ACE_OS::close (options.get_stdin ());
        ACE_OS::close (options.get_stdout ());
        ACE_OS::close (options.get_stderr ());

        // If we must, set the working directory for the child
        // process.
        if (options.working_directory () != 0)
          ACE_OS::chdir (options.working_directory ());
        // Should check for error here!

        // Child process executes the command.
        int result = 0;

        if (options.inherit_environment ())
          {
            // Add the new environment variables to the environment
            // context of the context before doing an <execvp>.
            for (char *const *user_env = options.env_argv ();
                 *user_env != 0;
                 user_env++)
              if (ACE_OS::putenv (*user_env) != 0)
                return ACE_INVALID_PID;

            // Now the forked process has both inherited variables and
            // the user's supplied variables.
            result = ACE_OS::execvp (options.process_name (),
                                     options.command_line_argv ());
          }
        else
          {
#if defined (ghs)
            // GreenHills 1.8.8 (for VxWorks 5.3.x) can't compile this
            // code.  Processes aren't supported on VxWorks anyways.
            ACE_NOTSUP_RETURN (ACE_INVALID_PID);
#else
            result = ACE_OS::execve (options.process_name (),
                                     options.command_line_argv (),
                                     options.env_argv ());
# endif /* ghs */
          }
        if (result == -1)
          {
            // If the execv fails, this child needs to exit.

            // Exit with the errno so that the calling process can
            // catch this and figure out what went wrong.
            ACE_OS::_exit (errno);
          }
        // ... otherwise, this is never reached.
        return 0;
      }
    default:
      // Server process.  The fork succeeded.
      return this->child_id_;
    }
#endif /* ACE_WIN32 */
}

void
ACE_Process::parent (pid_t)
{
  // nothing to do
}

void
ACE_Process::child (pid_t)
{
  // nothing to do
}

void
ACE_Process::unmanage (void)
{
  // nothing to do
}

int
ACE_Process::running (void) const
{
#if defined (ACE_WIN32)
    DWORD code;

    BOOL result = ::GetExitCodeProcess (this->gethandle (),
                                        &code);
    return result && code == STILL_ACTIVE;
#else
    return ACE_OS::kill (this->getpid (),
                         0) == 0
      || errno != ESRCH;
#endif /* ACE_WIN32 */
}

pid_t
ACE_Process::wait (const ACE_Time_Value &tv,
                   ACE_exitcode *status)
{
#if defined (ACE_WIN32)
  // Don't try to get the process exit status if wait failed so we can
  // keep the original error code intact.
  switch (::WaitForSingleObject (process_info_.hProcess,
                                 tv.msec ()))
    {
    case WAIT_OBJECT_0:
      if (status != 0)
        // The error status of <GetExitCodeProcess> is nonetheless not
        // tested because we don't know how to return the value.
        ::GetExitCodeProcess (process_info_.hProcess,
                              status);
      return this->getpid ();
    case WAIT_TIMEOUT:
      errno = ETIME;
      return 0;
    default:
      ACE_OS::set_errno_to_last_error ();
      return -1;
    }
#else /* ACE_WIN32 */
  if (tv == ACE_Time_Value::zero)
    ACE_OSCALL_RETURN (ACE_OS::waitpid (this->child_id_,
                                        status,
                                        WNOHANG),
                       int, ACE_INVALID_PID);

  if (tv == ACE_Time_Value::max_time)
    return this->wait (status);

  // Need to wait but limited to specified time.
  // Force generation of SIGCHLD, even though we don't want to
  // catch it - just need it to interrupt the sleep below.
  // If this object has a reactor set, assume it was given at
  // open(), and there's already a SIGCHLD action set, so no
  // action is needed here.
  ACE_Sig_Action old_action;
  ACE_Sig_Action do_sigchld ((ACE_SignalHandler)sigchld_nop);
  do_sigchld.register_action (SIGCHLD, &old_action);

  pid_t pid;
  ACE_Time_Value tmo (tv);       // Need one we can change
  for (ACE_Countdown_Time time_left (&tmo); ; time_left.update ())
    {
      pid = ACE_OS::waitpid (this->getpid (), status, WNOHANG);
      if (pid > 0 || pid == ACE_INVALID_PID)
        break;          // Got a child or an error - all done

      // pid 0, nothing is ready yet, so wait.
      // Do a sleep (only this thread sleeps) til something
      // happens. This relies on SIGCHLD interrupting the sleep.
      // If SIGCHLD isn't delivered, we'll need to do something
      // with sigaction to force it.
      if (-1 == ACE_OS::sleep (tmo) && errno == EINTR)
        continue;
      // Timed out
      pid = 0;
      break;
    }

  // Restore the previous SIGCHLD action if it was changed.
  old_action.register_action (SIGCHLD);

  return pid;
#endif /* ACE_WIN32 */
}

void
ACE_Process::close_dup_handles (void)
{
  if (this->dup_handles_.num_set () > 0)
    {
      ACE_Handle_Set_Iterator h_iter (this->dup_handles_);
      for (ACE_HANDLE h = h_iter ();
           h != ACE_INVALID_HANDLE;
           h = h_iter ())
        ACE_OS::closesocket (h);
      this->dup_handles_.reset ();
    }
  return;
}

void
ACE_Process::close_passed_handles (void)
{
  if (this->handles_passed_.num_set () > 0)
    {
      ACE_Handle_Set_Iterator h_iter (this->handles_passed_);
      for (ACE_HANDLE h = h_iter ();
           h != ACE_INVALID_HANDLE;
           h = h_iter ())
        ACE_OS::closesocket (h);
      this->handles_passed_.reset ();
    }
  return;
}


ACE_Process_Options::ACE_Process_Options (int ie,
                                          int cobl,
                                          int ebl,
                                          int mea)
  :
#if !defined (ACE_HAS_WINCE)
    inherit_environment_ (ie),
#endif /* ACE_HAS_WINCE */
    creation_flags_ (0),
    avoid_zombies_ (0),
#if !defined (ACE_HAS_WINCE)
#if defined (ACE_WIN32)
    environment_inherited_ (0),
    handle_inheritence_ (TRUE),
    process_attributes_ (NULL),
    thread_attributes_ (NULL),
#else /* ACE_WIN32 */
    stdin_ (ACE_INVALID_HANDLE),
    stdout_ (ACE_INVALID_HANDLE),
    stderr_ (ACE_INVALID_HANDLE),
    ruid_ ((uid_t) -1),
    euid_ ((uid_t) -1),
    rgid_ ((uid_t) -1),
    egid_ ((uid_t) -1),
#endif /* ACE_WIN32 */
    set_handles_called_ (0),
    environment_buf_index_ (0),
    environment_argv_index_ (0),
    environment_buf_ (0),
    environment_buf_len_ (ebl),
    max_environment_args_ (mea),
    max_environ_argv_index_ (mea - 1),
#endif /* !ACE_HAS_WINCE */
    command_line_argv_calculated_ (0),
    command_line_buf_ (0),
    command_line_buf_len_ (cobl),
    process_group_ (ACE_INVALID_PID)
{
  ACE_NEW (command_line_buf_,
           ACE_TCHAR[cobl]);
  command_line_buf_[0] = '\0';

#if !defined (ACE_HAS_WINCE)
  working_directory_[0] = '\0';
  ACE_NEW (environment_buf_,
           ACE_TCHAR[ebl]);
  ACE_NEW (environment_argv_,
           ACE_TCHAR *[mea]);
  environment_buf_[0] = '\0';
  environment_argv_[0] = 0;
  process_name_[0] = '\0';
#if defined (ACE_WIN32)
  ACE_OS::memset ((void *) &this->startup_info_,
                  0,
                  sizeof this->startup_info_);
  this->startup_info_.cb = sizeof this->startup_info_;
#endif /* ACE_WIN32 */
#endif /* !ACE_HAS_WINCE */
}

#if !defined (ACE_HAS_WINCE)
#if defined (ACE_WIN32)
void
ACE_Process_Options::inherit_environment (void)
{
  // Ensure only once execution.
  if (environment_inherited_)
    return;
  environment_inherited_ = 1;

  // Get the existing environment.
  ACE_TCHAR *existing_environment = ACE_OS::getenvstrings ();

  int slot = 0;

  while (existing_environment[slot] != '\0')
    {
      int len = ACE_OS::strlen (existing_environment + slot);

      // Add the string to our env buffer.
      if (this->setenv_i (existing_environment + slot, len) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("%p.\n"),
                      ACE_LIB_TEXT ("ACE_Process_Options::ACE_Process_Options")));
          break;
        }

      // Skip to the next word.
      slot += len + 1;
    }

  ACE_TEXT_FreeEnvironmentStrings (existing_environment);
}

#else /* defined ACE_WIN32 */

ACE_TCHAR * const *
ACE_Process_Options::env_argv (void)
{
  return environment_argv_;
}

#endif /* ACE_WIN32 */

int
ACE_Process_Options::setenv (ACE_TCHAR *envp[])
{
  int i = 0;
  while (envp[i])
    {
      if (this->setenv_i (envp[i],
                          ACE_OS::strlen (envp[i])) == -1)
        return -1;
      i++;
    }

#if defined (ACE_WIN32)
  if (inherit_environment_)
    this->inherit_environment ();
#endif /* ACE_WIN32 */

  return 0;
}

int
ACE_Process_Options::setenv (const ACE_TCHAR *format, ...)
{
  ACE_TCHAR stack_buf[DEFAULT_COMMAND_LINE_BUF_LEN];

  // Start varargs.
  va_list argp;
  va_start (argp, format);

  // Add the rest of the varargs.
  ACE_OS::vsprintf (stack_buf,
                    format,
                    argp);
  // End varargs.
  va_end (argp);

  // Append the string to are environment buffer.
  if (this->setenv_i (stack_buf,
                      ACE_OS::strlen (stack_buf)) == -1)
    return -1;

#if defined (ACE_WIN32)
  if (inherit_environment_)
    this->inherit_environment ();
#endif /* ACE_WIN32 */

  return 0;
}

int
ACE_Process_Options::setenv (const ACE_TCHAR *variable_name,
                             const ACE_TCHAR *format, ...)
{
  ACE_TCHAR newformat[DEFAULT_COMMAND_LINE_BUF_LEN];

  // Add in the variable name.
  ACE_OS::sprintf (newformat,
                   ACE_LIB_TEXT ("%s=%s"),
                   variable_name,
                   format);

  ACE_TCHAR stack_buf[DEFAULT_COMMAND_LINE_BUF_LEN];

  // Start varargs.
  va_list argp;
  va_start (argp, format);

  // Add the rest of the varargs.
  ACE_OS::vsprintf (stack_buf, newformat, argp);

  // End varargs.
  va_end (argp);

  // Append the string to our environment buffer.
  if (this->setenv_i (stack_buf,
                      ACE_OS::strlen (stack_buf)) == -1)
    return -1;

#if defined (ACE_WIN32)
  if (inherit_environment_)
    this->inherit_environment ();
#endif /* ACE_WIN32 */

  return 0;
}

int
ACE_Process_Options::setenv_i (ACE_TCHAR *assignment,
                               int len)
{
  // Add one for the null char.
  len++;

  // If environment larger than allocated buffer return. Also check to
  // make sure we have enough room.
  if (environment_argv_index_ == max_environ_argv_index_
      || (len + environment_buf_index_) >= environment_buf_len_)
    return -1;

  // Copy the new environment string.
  ACE_OS::memcpy (environment_buf_ + environment_buf_index_,
                  assignment,
                  len * sizeof (ACE_TCHAR));

  // Update the argv array.
  environment_argv_[environment_argv_index_++] =
    environment_buf_ + environment_buf_index_;
  environment_argv_[environment_argv_index_] = 0;

  // Update our index.
  environment_buf_index_ += len;

  // Make sure the buffer is null-terminated.
  environment_buf_[environment_buf_index_] = '\0';
  return 0;
}

int
ACE_Process_Options::set_handles (ACE_HANDLE std_in,
                                  ACE_HANDLE std_out,
                                  ACE_HANDLE std_err)
{
  this->set_handles_called_ = 1;
#if defined (ACE_WIN32)

  // Tell the new process to use our std handles.
  this->startup_info_.dwFlags = STARTF_USESTDHANDLES;

  if (std_in == ACE_INVALID_HANDLE)
    std_in = ACE_STDIN;
  if (std_out == ACE_INVALID_HANDLE)
    std_out = ACE_STDOUT;
  if (std_err == ACE_INVALID_HANDLE)
    std_err = ACE_STDERR;

  if (!::DuplicateHandle (::GetCurrentProcess (),
                          std_in,
                          ::GetCurrentProcess (),
                          &this->startup_info_.hStdInput,
                          0,
                          TRUE,
                          DUPLICATE_SAME_ACCESS))
    return -1;

  if (!::DuplicateHandle (::GetCurrentProcess (),
                          std_out,
                          ::GetCurrentProcess (),
                          &this->startup_info_.hStdOutput,
                          0,
                          TRUE,
                          DUPLICATE_SAME_ACCESS))
    return -1;

  if (!::DuplicateHandle (::GetCurrentProcess (),
                          std_err,
                          ::GetCurrentProcess (),
                          &this->startup_info_.hStdError,
                          0,
                          TRUE,
                          DUPLICATE_SAME_ACCESS))
    return -1;
#else /* ACE_WIN32 */
  this->stdin_ = ACE_OS::dup (std_in);
  this->stdout_ = ACE_OS::dup (std_out);
  this->stderr_ = ACE_OS::dup (std_err);
#endif /* ACE_WIN32 */

  return 0; // Success.
}


void
ACE_Process_Options::release_handles ()
{
  if (set_handles_called_)
    {
#if defined (ACE_WIN32)
      ACE_OS::close (startup_info_.hStdInput);
      ACE_OS::close (startup_info_.hStdOutput);
      ACE_OS::close (startup_info_.hStdError);
#else /* ACE_WIN32 */
      ACE_OS::close (stdin_);
      ACE_OS::close (stdout_);
      ACE_OS::close (stderr_);
#endif /* ACE_WIN32 */
      set_handles_called_ = 0;
    }
}
#endif /* !ACE_HAS_WINCE */


ACE_Process_Options::~ACE_Process_Options (void)
{
#if !defined (ACE_HAS_WINCE)
  release_handles();
  delete [] environment_buf_;
  delete [] environment_argv_;
#endif /* !ACE_HAS_WINCE */
  delete [] command_line_buf_;
}

int
ACE_Process_Options::command_line (const ACE_TCHAR *const argv[])
{
  // @@ Factor out the code between this
  int i = 0;

  if (argv[i])
    {
      ACE_OS::strcat (command_line_buf_, argv[i]);
      while (argv[++i])
        {
          ACE_OS::strcat (command_line_buf_,
                          ACE_LIB_TEXT (" "));
          ACE_OS::strcat (command_line_buf_,
                          argv[i]);
        }
    }

  return 0; // Success.
}

int
ACE_Process_Options::command_line (const ACE_TCHAR *format, ...)
{
  // Store all ... args in argp.
  va_list argp;
  va_start (argp, format);

  // sprintf the format and args into command_line_buf__.
  ACE_OS::vsprintf (command_line_buf_,
                    format,
                    argp);

  // Useless macro.
  va_end (argp);

  return 0;
}

#if defined (ACE_HAS_WCHAR) && !defined (ACE_HAS_WINCE)
/**
 * @note Not available on Windows CE because it doesn't have a char version of
 * vsprintf.
 */
int
ACE_Process_Options::command_line (const ACE_ANTI_TCHAR *format, ...)
{
  ACE_ANTI_TCHAR *anti_clb;
  ACE_NEW_RETURN (anti_clb,
                  ACE_ANTI_TCHAR[this->command_line_buf_len_],
                  -1);

  // Store all ... args in argp.
  va_list argp;
  va_start (argp, format);

  // sprintf the format and args into command_line_buf_.
  ACE_OS::vsprintf (anti_clb,
                    format,
                    argp);

  // Useless macro.
  va_end (argp);

  ACE_OS::strcpy (this->command_line_buf_,
                  ACE_TEXT_ANTI_TO_TCHAR (anti_clb));

  delete [] anti_clb;

  return 0;
}
#endif /* ACE_HAS_WCHAR && !ACE_HAS_WINCE */

ACE_TCHAR *
ACE_Process_Options::env_buf (void)
{
#if !defined (ACE_HAS_WINCE)
  if (environment_buf_[0] == '\0')
    return 0;
  else
    return environment_buf_;
#else
  return 0;
#endif /* !ACE_HAS_WINCE */
}

ACE_TCHAR * const *
ACE_Process_Options::command_line_argv (void)
{
  if (command_line_argv_calculated_ == 0)
    {
      command_line_argv_calculated_ = 1;

      // This tokenizer will replace all spaces with end-of-string
      // characters and will preserve text between "" and '' pairs.
      ACE_Tokenizer parser (command_line_buf_);
      parser.delimiter_replace (' ', '\0');
      parser.preserve_designators ('\"', '\"'); // "
      parser.preserve_designators ('\'', '\'');

      int x = 0;
      do
        command_line_argv_[x] = parser.next ();
      while (command_line_argv_[x] != 0
             // substract one for the ending zero.
             && ++x < MAX_COMMAND_LINE_OPTIONS - 1);

      command_line_argv_[x] = 0;
    }

  return command_line_argv_;
}


// Cause the specified handle to be passed to a child process
// when it's spawned.
int
ACE_Process_Options::pass_handle (ACE_HANDLE h)
{
# if defined (ACE_WIN32)
#  if defined (ACE_HAS_WINCE)
  ACE_NOTSUP_RETURN (-1);
#  else

  // This is oriented towards socket handles... may need some adjustment
  // for non-sockets.
  // This is all based on an MSDN article:
  // http://support.microsoft.com/support/kb/articles/Q150/5/23.asp
  // If on Win95/98, the handle needs to be duplicated for the to-be-spawned
  // process. On WinNT, they get inherited by the child process automatically.
  // If the handle is duplicated, remember the duplicate so it can be
  // closed later. Can't be closed now, or the child won't get it.
  OSVERSIONINFO osvi;
  ZeroMemory (&osvi, sizeof (osvi));
  osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
  // If this is Win95/98 or we can't tell, duplicate the handle.
  if (!GetVersionEx (&osvi) || osvi.dwPlatformId != VER_PLATFORM_WIN32_NT)
    {
      HANDLE dup_handle;
      if (!DuplicateHandle (GetCurrentProcess (),
                            ACE_static_cast (HANDLE, h),
                            GetCurrentProcess (),
                            &dup_handle,
                            0,
                            TRUE,   // Inheritable
                            DUPLICATE_SAME_ACCESS))
        return -1;
      dup_handles_.set_bit (ACE_static_cast (ACE_HANDLE, dup_handle));
    }
#  endif /* ACE_HAS_WINCE */
#endif /* ACE_WIN32 */

  this->handles_passed_.set_bit (h);

  return 0;
}

// Get a copy of the handles the ACE_Process_Options duplicated
// for the spawned process.
int
ACE_Process_Options::dup_handles (ACE_Handle_Set &set) const
{
  if (this->dup_handles_.num_set () == 0)
    return 0;
  set.reset ();
  set = this->dup_handles_;
  return 1;
}

// Get a copy of the handles passed to the spawned process. This
// will be the set of handles previously passed to @arg pass_handle().
int
ACE_Process_Options::passed_handles (ACE_Handle_Set &set) const
{
  if (this->handles_passed_.num_set () == 0)
    return 0;
  set.reset ();
  set = this->handles_passed_;
  return 1;
}

ACE_Managed_Process::ACE_Managed_Process (void)
{
}

ACE_Managed_Process::~ACE_Managed_Process (void)
{
}
