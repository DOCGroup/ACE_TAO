// ARGV.cpp
// $Id$

// Transforms a string BUF into an ARGV-style vector of strings. 

#define ACE_BUILD_DLL
#include "ace/ARGV.h"

#if !defined (__ACE_INLINE__)
#include "ace/ARGV.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, ARGV, "$Id$")

ACE_ALLOC_HOOK_DEFINE (ACE_ARGV)

void
ACE_ARGV::dump (void) const
{
  ACE_TRACE ("ACE_ARGV::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("argc_ = %d"), this->argc_));

  for (size_t i = 0; i < this->argc_; i++)
    ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nargv_[%i] = %s"), i, this->argv_[i]));

  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nbuf = %s\n"), this->buf_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Creates this->argv_ out of this->buf_.  New memory is allocated for
// each element of the array.  This is used by the array-to-string
// style constructor and for creating this->argv_ when in iterative
// mode.

int
ACE_ARGV::string_to_argv (void)
{
  ACE_TRACE ("ACE_ARGV::string_to_argv");

  return ACE_OS::string_to_argv (this->buf_,
                                 this->argc_,
                                 this->argv_,
                                 this->substitute_env_args_);
}

int
ACE_ARGV::argv_to_string (ASYS_TCHAR **argv, ASYS_TCHAR *&buf)
{
  return ACE_OS::argv_to_string (argv, buf);
}

ACE_ARGV::ACE_ARGV (const ASYS_TCHAR buf[],
		    int substitute_env_args)
  : substitute_env_args_ (substitute_env_args),
    state_ (TO_PTR_ARRAY),
    argc_ (0),
    argv_ (0),
    buf_ (0),
    length_ (0)
{
  ACE_TRACE ("ACE_ARGV::ACE_ARGV ASYS_TCHAR[] to ASYS_TCHAR *[]");

  if (buf == 0 || buf[0] == 0)
    return;

  // Make an internal copy of the string.
  ACE_NEW (this->buf_,
           ASYS_TCHAR[ACE_OS::strlen (buf) + 1]);
  ACE_OS::strcpy (this->buf_, buf);

  // Create this->argv_.
  if (this->string_to_argv () == -1)
    ACE_ERROR ((LM_ERROR, 
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("string_to_argv")));
}

ACE_ARGV::ACE_ARGV (ASYS_TCHAR *argv[],
		    int substitute_env_args)
  : substitute_env_args_ (substitute_env_args),
    state_ (TO_STRING),
    argc_ (0),
    argv_ (0),
    buf_ (0),
    length_ (0)
{
  ACE_TRACE ("ACE_ARGV::ACE_ARGV ASYS_TCHAR*[] to ASYS_TCHAR[]");

  if (argv == 0 || argv[0] == 0)
    return;

  int buf_len = 0;
  
  // Determine the length of the buffer.

  for (int i = 0; argv[i] != 0; i++)
    {
      ASYS_TCHAR *temp;

      // Account for environment variables.
      if (this->substitute_env_args_
	  && (argv[i][0] == '$'
	  && (temp = ACE_OS::getenv (&argv[i][1])) != 0))
	buf_len += ACE_OS::strlen (temp);	
      else
	buf_len += ACE_OS::strlen (argv[i]);

      // Add one for the extra space between each string.
      buf_len++;
    }

  // Step through all argv params and copy each one into buf; separate
  // each param with white space.

  ACE_NEW (this->buf_, ASYS_TCHAR[buf_len + 1]);

  ASYS_TCHAR *end = this->buf_;
  int j;

  for (j = 0; argv[j] != 0; j++)
    {
      ASYS_TCHAR *temp;

      // Account for environment variables.
      if (this->substitute_env_args_
	  && (argv[j][0] == '$'
	  && (temp = ACE_OS::getenv (&argv[j][1])) != 0))
	end = ACE::strecpy (end, temp);
      else
	end = ACE::strecpy (end, argv[j]);

      // Replace the null char that strecpy copies with white space as
      // a separator.
      *(end-1) = ' ';
    }

  // Remember how many arguments there are
  this->argc_ = j;

  // Null terminate the string.
  *end = '\0';
}

ACE_ARGV::ACE_ARGV (ASYS_TCHAR *first_argv[], 
                    ASYS_TCHAR *second_argv[],
                    int substitute_env_args)
  : substitute_env_args_ (substitute_env_args),
    state_ (TO_STRING),
    argc_ (0),
    argv_ (0),
    buf_ (0),
    length_ (0)
{
  ACE_TRACE ("ACE_ARGV::ACE_ARGV ASYS_TCHAR*[] + ASYS_TCHAR *[] to ASYS_TCHAR[]");

  int first_argc;
  int second_argc;

  ASYS_TCHAR *first_buf;
  ASYS_TCHAR *second_buf;

  // convert the first argv to a string
  first_argc = this->argv_to_string (first_argv,first_buf);

  // convert the second argv to a string
  second_argc = this->argv_to_string (second_argv,second_buf);

  // Add the number of arguments in both the argvs.
  this->argc_ = first_argc + second_argc;

  int buf_len = ACE_OS::strlen (first_buf) + ACE_OS::strlen (second_buf) + 1;

  // Allocate memory to the lenght of the combined argv string.
  ACE_NEW (this->buf_, ASYS_TCHAR[buf_len + 1]);

  // copy the first argv string to the buffer
  ACE_OS::strcpy (this->buf_,first_buf);

  // concatenate the second argv string to the buffer
  ACE_OS::strcat (this->buf_,second_buf);

  //   Delete the first and second buffers

  delete [] first_buf;

  delete [] second_buf;
}


ACE_ARGV::ACE_ARGV (int substitute_env_args)
  : substitute_env_args_ (substitute_env_args),
    state_ (ITERATIVE),
    argc_ (0),
    argv_ (0),
    buf_ (0),
    length_ (0)
{
  ACE_TRACE ("ACE_ARGV::ACE_ARGV Iterative");

  // Nothing to do yet -- the user puts in arguments via add ()
}

int
ACE_ARGV::add (const ASYS_TCHAR *next_arg)
{
  // Only allow this to work in the "iterative" verion -- the
  // ACE_ARGVs created with the one argument constructor.
  if (this->state_ != ITERATIVE)
    return -1;

  // Put the new argument at the end of the queue.
  if (this->queue_.enqueue_tail ((ASYS_TCHAR *) next_arg) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("Can't add more to ARGV queue")),
                      -1);

  this->length_ += ACE_OS::strlen (next_arg);

  this->argc_++;

  // Wipe argv_ and buf_ away so that they will be recreated if the
  // user calls argv () or buf ().
  if (this->argv_ != 0) 
    {
      for (int i = 0; this->argv_[i] != 0; i++)
	ACE_OS::free ((void *) this->argv_[i]);

      delete [] this->argv_;
      this->argv_ = 0;
    }

  delete [] this->buf_;
  this->buf_ = 0;

  return 0;
}

// Free up argv_ and buf_

ACE_ARGV::~ACE_ARGV (void)
{
  ACE_TRACE ("ACE_ARGV::~ACE_ARGV");
 
  if (this->argv_ != 0)
    for (int i = 0; this->argv_[i] != 0; i++)
      ACE_OS::free ((void *) this->argv_[i]);

  delete [] this->argv_;
  delete [] this->buf_;
}

// Create buf_ out of the queue_.  This is only used in the
// "iterative" mode.

int
ACE_ARGV::create_buf_from_queue (void)
{
  ACE_TRACE ("ACE_ARGV::create_buf_from_queue");

  // If the are no arguments, don't do anything
  if (this->argc_ <= 0)
    return -1;

  delete [] this->buf_;

  ACE_NEW_RETURN (this->buf_,
                  ASYS_TCHAR[this->length_ + this->argc_],
                  -1);

  // Get an iterator over the queue
  ACE_Unbounded_Queue_Iterator<ASYS_TCHAR *> iter (this->queue_);

  ASYS_TCHAR **arg;
  ASYS_TCHAR *ptr = this->buf_;
  size_t len;
  int more = 0;

  while (!iter.done ()) 
    {
      // Get next argument from the queue.
      iter.next (arg);

      more = iter.advance ();

      len = ACE_OS::strlen (*arg);

      // Copy the argument into buf_
      ACE_OS::memcpy ((void *) ptr,
                      (const void *) (*arg),
                      len);
      // Move the pointer down.
      ptr += len;

      // Put in an argument separating space.
      if (more != 0) 
	*ptr++ = ' ';
    }

  // Put in the NUL terminator
  *ptr = '\0';

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Unbounded_Queue<ASYS_TCHAR *>;
template class ACE_Unbounded_Queue_Iterator<ASYS_TCHAR *>;
template class ACE_Node<ASYS_TCHAR *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Unbounded_Queue<ASYS_TCHAR *>
#pragma instantiate ACE_Unbounded_Queue_Iterator<ASYS_TCHAR *>
#pragma instantiate ACE_Node<ASYS_TCHAR *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
