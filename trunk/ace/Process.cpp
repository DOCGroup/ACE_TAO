// $Id$

#define ACE_BUILD_DLL
#include "ace/Process.h"
#include "ace/ARGV.h"
#include "ace/SString.h"

#if !defined (__ACE_INLINE__)
#include "ace/Process.i"
#endif /* __ACE_INLINE__ */

ACE_Tokenizer::ACE_Tokenizer (LPTSTR buffer)
  : buffer_ (buffer),
    index_ (0),
    preserves_index_ (0),
    delimiter_index_ (0)
{
}

int
ACE_Tokenizer::delimiter (TCHAR d)
{
  if (delimiter_index_ == MAX_DELIMITERS)
    return -1;

  delimiters_[delimiter_index_].delimiter_ = d;
  delimiters_[delimiter_index_].replace_ = 0;
  delimiter_index_++;
  return 0;
}

int
ACE_Tokenizer::delimiter_replace (TCHAR d, TCHAR replacement)
{
  if (delimiter_index_ == MAX_DELIMITERS)
    return -1;

  delimiters_[delimiter_index_].delimiter_ = d;
  delimiters_[delimiter_index_].replacement_ = replacement;
  delimiters_[delimiter_index_].replace_ = 1;
  delimiter_index_++;
  return 0;
}

int 
ACE_Tokenizer::preserve_designators (TCHAR start, TCHAR stop, int strip)
{
  if (preserves_index_ == MAX_PRESERVES)
    return -1;

  preserves_[preserves_index_].start_ = start;
  preserves_[preserves_index_].stop_ = stop;
  preserves_[preserves_index_].strip_ = strip;
  preserves_index_++;
  return 0;
}

int 
ACE_Tokenizer::is_delimiter (TCHAR d, int &replace, TCHAR &r)
{
  replace = 0;

  for (int x=0; x < delimiter_index_; x++)
    if (delimiters_[x].delimiter_ == d)
      {
	if (delimiters_[x].replace_)
	  {
	    r = delimiters_[x].replacement_;
	    replace = 1;
	  }
	return 1;
      }

  return 0;
}

int 
ACE_Tokenizer::is_preserve_designator (TCHAR start, TCHAR &stop, int &strip)
{
  for (int x=0; x < preserves_index_; x++)
    if (preserves_[x].start_ == start)
      {
	stop = preserves_[x].stop_;
	strip = preserves_[x].strip_;
	return 1;
      }

  return 0;
}

LPTSTR
ACE_Tokenizer::next (void)
{
  // Check if the previous pass was the last one in the buffer.
  if (index_ == -1)
    {
      index_ = 0;
      return 0;
    }

  TCHAR replacement;
  int replace;
  LPTSTR next_token;

  // Skip all leading delimiters.
  while (1)
    {
      // Check for end of string.
      if (buffer_[index_] == '\0')
	{
	  // If we hit EOS at the start, return 0.
	  index_ = 0;
	  return 0;
	}

      if (this->is_delimiter (buffer_[index_], replace, replacement))
	index_++;
      else
	break;
    } 

  // When we reach this point, buffer_[index_] is a non-delimiter and
  // not EOS - the start of our next_token.
  next_token = buffer_ + index_;

  // A preserved region is it's own token.
  TCHAR stop;
  int strip;
  if (this->is_preserve_designator (buffer_[index_], stop, strip))
    {
      while (++index_)
	{
	  if (buffer_[index_] == '\0')
	    {
	      index_ = -1;
	      goto EXIT_LABEL;
	    }
	      
	  if (buffer_[index_] == stop)
	    break;
	}

      if (strip)
	{
	  // Skip start preserve designator.
	  next_token += 1;
	  // Zap the stop preserve designator.
	  buffer_[index_] = '\0';
	  // Increment to the next token.
	  index_++;
	}
      else
	next_token = buffer_ + index_;
      
      goto EXIT_LABEL;
    }

  // Step through finding the next delimiter or EOS.
  while (1)
    {
      // Advance pointer.
      index_++;

      // Check for end of string.
      if (buffer_[index_] == '\0')
	{
	  index_ = -1;
	  goto EXIT_LABEL;
	}

      // Check for delimiter.
      if (this->is_delimiter (buffer_[index_], replace, replacement))
	{
	  // Replace the delimiter.
	  if (replace != 0)
	    buffer_[index_] = replacement;

	  // Move the pointer up and return.
	  index_++;
	  goto EXIT_LABEL;
	}

      // A preserve designator signifies the end of this token.
      if (this->is_preserve_designator (buffer_[index_], stop, strip))
	goto EXIT_LABEL;
    }

EXIT_LABEL:
  return next_token;
}

// ************************************************************

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
		     &process_info_);

  if (fork_result) // If success.
    return 0;
  else
    // CreateProcess failed.
    return -1;
#else /* ACE_WIN32 */
  // Fork the new process.
  this->child_id_ = ACE_OS::fork (options.command_line_argv ()[0]);

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
	  return -1;
	else if (options.get_stdout () != ACE_INVALID_HANDLE
		 && ACE_OS::dup2 (options.get_stdout (), ACE_STDOUT) == -1)
	  return -1;
	else if (options.get_stderr () != ACE_INVALID_HANDLE
		 && ACE_OS::dup2 (options.get_stderr (), ACE_STDERR) == -1)
	  return -1;

	// If we must, set the working directory for the child process.
	if (options.working_directory () != 0)
	  ::chdir (options.working_directory ());

	// Child process executes the command.
	int result;
      
	if (options.env_argv ()[0] == 0)
	  result = ACE_OS::execvp (options.command_line_argv ()[0],
				   options.command_line_argv ()); // command-line args
	else
	  result = ACE_OS::execve (options.command_line_argv ()[0],
				   options.command_line_argv (), // command-line args
				   options.env_argv ()); // environment variables

	if (result == -1)
	  // If the execv fails, this child needs to exit.
	  ACE_OS::exit (errno);

	return 0;
      }

    default:
      // Server process.  The fork succeeded.
      return this->child_id_;
    }

  return 0;
#endif /* ACE_WIN32 */
}

int
ACE_Process::wait (void)
{
#if defined (ACE_WIN32)
  return ::WaitForSingleObject (process_info_.hProcess, INFINITE);
#else /* ACE_WIN32 */
  return ACE_OS::waitpid (this->child_id_, 0, 0);
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

int
ACE_Process_Old::wait (void)
{
#if defined (ACE_WIN32)
  return ::WaitForSingleObject (process_info_.hProcess, INFINITE);
#else /* ACE_WIN32 */
  return ACE_OS::waitpid (this->child_id_, 0, 0);
#endif /* ACE_WIN32 */
}

ACE_Process_Old::ACE_Process_Old (void)
#if defined (ACE_WIN32)
  : set_handles_called_ (0)
#else /* ACE_WIN32 */
  : stdin_ (ACE_INVALID_HANDLE),
    stdout_ (ACE_INVALID_HANDLE),
    stderr_ (ACE_INVALID_HANDLE),
    child_id_ (0)
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

ACE_Process_Old::~ACE_Process_Old (void)
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
ACE_Process_Old::set_handles (ACE_HANDLE std_in,
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
ACE_Process_Old::set_cwd (LPCTSTR cwd)
{
  ACE_OS::strncpy (this->cwd_, cwd, MAXPATHLEN);
  // This is for paranoia...
  this->cwd_[MAXPATHLEN] = '\0';
  return 0;
}

pid_t
ACE_Process_Old::start (char *argv[], char *envp[])
{
#if defined (ACE_WIN32)
  ACE_ARGV argv_buf (argv);

  LPTSTR buf = (LPTSTR) ACE_WIDE_STRING (argv_buf.buf ());

  if (buf == 0)
    return -1;

  // If there is no current working directory, we *MUST* pass 0, not "".
  TCHAR *cwd = ACE_OS::strlen (cwd_) == 0 ? 0 : cwd_;

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

ACE_Process_Old::ACE_Process_Old (char *argv[],
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

// ************************************************************

ACE_Process_Options::ACE_Process_Options (int ie,
					  int cobl)
  : inherit_environment_ (ie),
#if defined (ACE_WIN32)
    environment_inherited_ (0),
    set_handles_called_ (0),
    handle_inheritence_ (TRUE),
    creation_flags_ (0),
    process_attributes_ (NULL),
    thread_attributes_ (NULL),
#else /* ACE_WIN32 */
    stdin_ (ACE_INVALID_HANDLE),
    stdout_ (ACE_INVALID_HANDLE),
    stderr_ (ACE_INVALID_HANDLE),
#endif /* ACE_WIN32 */
    environment_buf_index_ (0),
    environment_argv_index_ (0),
    command_line_buf_ (0),
    command_line_argv_calculated_ (0)
{
  ACE_NEW (command_line_buf_, TCHAR[cobl]);
  command_line_buf_[0] = '\0';

  working_directory_[0] = '\0';
  environment_buf_[0] = '\0';
  environment_argv_[0] = 0;
  
#if defined (ACE_WIN32)
  ACE_OS::memset ((void *) &this->startup_info_, 
		  0, 
		  sizeof this->startup_info_);
  this->startup_info_.cb = sizeof this->startup_info_;
#endif /* ACE_WIN32 */
}

#if defined (ACE_WIN32)
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

char **
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
  ACE_OS::sprintf (newformat, __TEXT ("%s=%s"), variable_name, format);

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

  // Check if we're out of room.
  if ((len + environment_buf_index_) >= ENVIRONMENT_BUFFER)
    return -1;

  // Copy the new environment string.
  ACE_OS::memcpy (environment_buf_ + environment_buf_index_,
		  assignment, len);

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

ACE_Process_Options::~ACE_Process_Options (void)
{
#if defined (ACE_WIN32)
  if (set_handles_called_)
    {
      ::CloseHandle (startup_info_.hStdInput);
      ::CloseHandle (startup_info_.hStdOutput);
      ::CloseHandle (startup_info_.hStdError);
      set_handles_called_ = 0;
    }
#endif /* ACE_WIN32 */

  delete [] command_line_buf_;
}

int
ACE_Process_Options::set_handles (ACE_HANDLE std_in,
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
ACE_Process_Options::command_line (LPTSTR argv[])
{
  int i;

  if (argv[i = 0])
    {
      ACE_OS::strcat (command_line_buf_, argv[i]);
      while (argv[++i])
        {
          ACE_OS::strcat (command_line_buf_, __TEXT (" "));
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
  if (environment_buf_[0] == '\0')
    return 0;
  else
    return environment_buf_;
}
