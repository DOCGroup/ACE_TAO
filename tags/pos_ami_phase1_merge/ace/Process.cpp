// $Id$

#define ACE_BUILD_DLL
#include "ace/Process.h"
#include "ace/ARGV.h"
#include "ace/SString.h"

#if !defined (__ACE_INLINE__)
#include "ace/Process.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Process, "$Id$")

ACE_Process::ACE_Process (void)
#if !defined (ACE_WIN32)
  : child_id_ (0)
#endif /* !defined (ACE_WIN32) */
{
#if defined (ACE_WIN32)
  ACE_OS::memset ((void *) &this->process_info_,
                  0, sizeof this->process_info_);
#endif /* ACE_WIN32 */
}

ACE_Process::~ACE_Process (void)
{
#if defined (ACE_WIN32)
  // Free resources allocated in kernel.
  ACE_OS::close (this->process_info_.hThread);
  ACE_OS::close (this->process_info_.hProcess);
#endif /* ACE_WIN32 */
}

pid_t
ACE_Process::spawn (ACE_Process_Options &options)
{
#if defined (ACE_WIN32)
  BOOL fork_result =
    ::CreateProcess (0,
                     options.command_line_buf (), // command-line options
                     options.get_process_attributes (),
                     options.get_thread_attributes (),
                     options.handle_inheritence (),
                     options.creation_flags (),
                     options.env_buf (), // environment variables
                     options.working_directory (),
                     options.startup_info (),
                     &this->process_info_);

  if (fork_result) // If success.
    return this->getpid ();
  else
    // CreateProcess failed.
    return -1;
#elif defined (CHORUS)
  // This only works if we exec.  Chorus does not really support
  // forking
  if (ACE_BIT_ENABLED (options.creation_flags (),
                       ACE_Process_Options::NO_EXEC))
    ACE_NOTSUP_RETURN (-1);

  // These are all currently unsupported.
  if (options.get_stdin () != ACE_INVALID_HANDLE)
    ACE_NOTSUP_RETURN (-1);
  if (options.get_stdout () != ACE_INVALID_HANDLE)
    ACE_NOTSUP_RETURN (-1);
  if (options.get_stderr () != ACE_INVALID_HANDLE)
    ACE_NOTSUP_RETURN (-1);
  if (options.working_directory () != 0)
    ACE_NOTSUP_RETURN (-1);

  if (options.env_argv ()[0] == 0)
    // command-line args
    this->child_id_ = ACE_OS::execvp (options.command_line_argv ()[0],
                                      options.command_line_argv ());
  else
    {
      // Add the new environment variables to the environment context
      // of the context before doing an <execvp>.
      for (char *const *user_env = options.env_argv ();
           *user_env != 0;
           user_env++)
        if (ACE_OS::putenv (*user_env) != 0)
          return -1;

      // Now the forked process has both inherited variables and the
      // user's supplied variables.
      this->child_id_ = ACE_OS::execvp (options.command_line_argv ()[0],
                                        options.command_line_argv ());
    }

  return this->child_id_;
#else /* ACE_WIN32 */
  // Fork the new process.
  this->child_id_ = ACE::fork (options.command_line_argv ()[0],
                               options.avoid_zombies ());

  // If we're not supposed to exec, return the process id.
  if (ACE_BIT_ENABLED (options.creation_flags (), ACE_Process_Options::NO_EXEC))
    return this->child_id_;

  switch (this->child_id_)
    {
    case -1:
      // Error.
      return -1;
    case 0:
      // Child process.
      {
        if (options.get_stdin () != ACE_INVALID_HANDLE
            && ACE_OS::dup2 (options.get_stdin (), ACE_STDIN) == -1)
          ACE_OS::exit (errno);
        else if (options.get_stdout () != ACE_INVALID_HANDLE
                 && ACE_OS::dup2 (options.get_stdout (), ACE_STDOUT) == -1)
          ACE_OS::exit (errno);
        else if (options.get_stderr () != ACE_INVALID_HANDLE
                 && ACE_OS::dup2 (options.get_stderr (), ACE_STDERR) == -1)
          ACE_OS::exit (errno);

        // close down unneeded descriptors
        ACE_OS::close (options.get_stdin ());
        ACE_OS::close (options.get_stdout ());
        ACE_OS::close (options.get_stderr ());

        // If we must, set the working directory for the child process.
        if (options.working_directory () != 0)
          ACE_OS::chdir (options.working_directory ());

        // Child process executes the command.
        int result = 0;

        if (options.env_argv ()[0] == 0)
          // command-line args
          result = ACE_OS::execvp (options.command_line_argv ()[0],
                                   options.command_line_argv ());
        else
          {
#if defined( ghs )
            // GreenHills 1.8.8 (for VxWorks 5.3.x) can't compile
            // this code.  Processes aren't supported on VxWorks
            // anyways.
            ACE_NOTSUP_RETURN (-1);
#else
            // Add the new environment variables to the environment context
            // of the context before doing an <execvp>.
            for (char *const *user_env = options.env_argv ();
                 *user_env != 0;
                 user_env++)
              if (ACE_OS::putenv (*user_env) != 0)
                return -1;

            // Now the forked process has both inherited variables and
            // the user's supplied variables.
            result = ACE_OS::execvp (options.command_line_argv ()[0],
                                     options.command_line_argv ());
#endif /* ghs */
          }

        if (result == -1)
          {
            // If the execv fails, this child needs to exit.

            // Exit with the errno so that the calling process can
            // catch this and figure out what went wrong.
            ACE_OS::exit (errno);
          }

        return 0;
      }
    default:
      // Server process.  The fork succeeded.
      return this->child_id_;
    }
#endif /* ACE_WIN32 */
}

int
ACE_Process::wait (int *status)
{
#if defined (ACE_WIN32)
  // Notice that status doesn't get updated.
  int retv;

  // Don't try to get the process exit status if wait failed so we can
  // keep the original error code intact.
  if ((retv = ::WaitForSingleObject (process_info_.hProcess, INFINITE))
      != WAIT_FAILED && status != 0)
    // The error status of GetExitCodeProcess is nontheless not tested.
    // (Don't know how to return the value.)
    ::GetExitCodeProcess (process_info_.hProcess, (LPDWORD) status);
  return retv;
#else /* ACE_WIN32 */
  return ACE_OS::waitpid (this->child_id_, status, 0);
#endif /* ACE_WIN32 */
}

int
ACE_Process::wait (const ACE_Time_Value &tv)
{
#if defined (ACE_WIN32)
  return ::WaitForSingleObject (process_info_.hProcess, tv.msec ());
#else /* ACE_WIN32 */
  ACE_UNUSED_ARG (tv);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_WIN32 */
}

// ************************************************************

ACE_Process_Options::ACE_Process_Options (int ie,
                                          int cobl,
                                          int ebl,
                                          int mea)
  :
#if !defined (ACE_HAS_WINCE)
    inherit_environment_ (ie),
#endif
    creation_flags_ (0),
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
    avoid_zombies_ (0),
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
    command_line_buf_ (0)
{
  ACE_NEW (command_line_buf_, TCHAR[cobl]);
  command_line_buf_[0] = '\0';

#if !defined (ACE_HAS_WINCE)
  working_directory_[0] = '\0';
  ACE_NEW (environment_buf_, TCHAR[ebl]);
  ACE_NEW (environment_argv_, LPTSTR[mea]);
  environment_buf_[0] = '\0';
  environment_argv_[0] = 0;

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
// Notice that CE version is an empty function and is placed
// in Process.i for efficiency.
void
ACE_Process_Options::inherit_environment (void)
{
  // Ensure only once execution.
  if (environment_inherited_)
    return;
  environment_inherited_ = 1;

  // Get the existing environment.
  LPTSTR existing_environment = ::GetEnvironmentStrings ();

  int index = 0;

  while (existing_environment[index] != '\0')
    {
      int len = ACE_OS::strlen (existing_environment + index);

      // Add the string to our env buffer.
      if (this->setenv_i (existing_environment+index, len) == -1)
        {
          ACE_ERROR ((LM_ERROR, "%p.\n",
                      "ACE_Process_Options::ACE_Process_Options"));
          break;
        }

      // Skip to the next word.
      index += len + 1;
    }

  ::FreeEnvironmentStrings (existing_environment);
}

#else /* defined ACE_WIN32 */

char * const *
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
        {
          command_line_argv_[x] = parser.next ();
        }
      while (command_line_argv_[x] != 0 &&
             // substract one for the ending zero.
             ++x < MAX_COMMAND_LINE_OPTIONS-1);

      command_line_argv_[x] = 0;
    }

  return command_line_argv_;
}

char * const *
ACE_Process_Options::env_argv (void)
{
  return environment_argv_;
}

#endif /* ACE_WIN32 */

int
ACE_Process_Options::setenv (LPTSTR envp[])
{
  int i = 0;
  while (envp[i])
    {
      if (this->setenv_i (envp[i], ACE_OS::strlen (envp[i])) == -1)
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
ACE_Process_Options::setenv (LPCTSTR format, ...)
{
  TCHAR stack_buf[DEFAULT_COMMAND_LINE_BUF_LEN];

  // Start varargs.
  va_list argp;
  va_start (argp, format);

  // Add the rest of the varargs.
  ACE_OS::vsprintf (stack_buf, format, argp);

  // End varargs.
  va_end (argp);

  // Append the string to are environment buffer.
  if (this->setenv_i (stack_buf, ACE_OS::strlen (stack_buf)) == -1)
    return -1;

#if defined (ACE_WIN32)
  if (inherit_environment_)
    this->inherit_environment ();
#endif /* ACE_WIN32 */

  return 0;
}

int
ACE_Process_Options::setenv (LPCTSTR variable_name,
                             LPCTSTR format, ...)
{
  TCHAR newformat[DEFAULT_COMMAND_LINE_BUF_LEN];

  // Add in the variable name.
  ACE_OS::sprintf (newformat, ACE_TEXT ("%s=%s"), variable_name, format);

  TCHAR stack_buf[DEFAULT_COMMAND_LINE_BUF_LEN];

  // Start varargs.
  va_list argp;
  va_start (argp, format);

  // Add the rest of the varargs.
  ACE_OS::vsprintf (stack_buf, newformat, argp);

  // End varargs.
  va_end (argp);

  // Append the string to our environment buffer.
  if (this->setenv_i (stack_buf, ACE_OS::strlen (stack_buf)) == -1)
    return -1;

#if defined (ACE_WIN32)
  if (inherit_environment_)
    this->inherit_environment ();
#endif /* ACE_WIN32 */

  return 0;
}

int
ACE_Process_Options::setenv_i (LPTSTR assignment, int len)
{
  // Add one for the null char.
  len++;

  // If environment larger than allocated buffer return. Also check to
  // make sure we have enough room.
  if ( environment_argv_index_ == max_environ_argv_index_  ||
       (len + environment_buf_index_) >= environment_buf_len_ )
  {
    return -1;
  }

  // Copy the new environment string.
  ACE_OS::memcpy (environment_buf_ + environment_buf_index_,
                  assignment, len * sizeof (TCHAR));

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
  this->stdin_ = ACE_OS::dup (std_in);
  this->stdout_ = ACE_OS::dup (std_out);
  this->stderr_ = ACE_OS::dup (std_err);
#endif /* ACE_WIN32 */

  return 0; // Success.
}
#endif /* !ACE_HAS_WINCE */

ACE_Process_Options::~ACE_Process_Options (void)
{
#if !defined (ACE_HAS_WINCE)
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
  delete [] environment_buf_;
  delete [] environment_argv_;
#endif /* !ACE_HAS_WINCE */
  delete [] command_line_buf_;
}

int
ACE_Process_Options::command_line (LPTSTR argv[])
{
  int i;

  if (argv[i = 0])
    {
      ACE_OS::strcat (command_line_buf_, argv[i]);
      while (argv[++i])
        {
          ACE_OS::strcat (command_line_buf_, ACE_TEXT (" "));
          ACE_OS::strcat (command_line_buf_, argv[i]);
        }
    }

  return 0; // Success.
}

int
ACE_Process_Options::command_line (LPCTSTR format, ...)
{
  // Store all ... args in argp.
  va_list argp;
  va_start (argp, format);

  // sprintf the format and args into command_line_buf__.
  ACE_OS::vsprintf (command_line_buf_, format, argp);

  // Useless macro.
  va_end (argp);

  return 0;
}

LPTSTR
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
