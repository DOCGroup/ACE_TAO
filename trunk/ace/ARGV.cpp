// ARGV.cpp
// $Id$

// Transforms a string BUF into an ARGV-style vector of strings. 

#define ACE_BUILD_DLL
#include "ace/ARGV.h"

#if !defined (__ACE_INLINE__)
#include "ace/ARGV.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_ARGV)

void
ACE_ARGV::dump (void) const
{
  ACE_TRACE ("ACE_ARGV::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "argc_ = %d", this->argc_));

  for (size_t i = 0; i < this->argc_; i++)
    ACE_DEBUG ((LM_DEBUG, "\nargv_[%i] = %s", i, this->argv_[i]));

  ACE_DEBUG ((LM_DEBUG, "\nbuf = %s\n", this->buf_));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Creates this->argv_ out of this->buf_.  New memory is allocated
// for each element of the array.  This is used by the array-to-string
// style constructor and for creating this->argv_ when in iterative mode.
void
ACE_ARGV::string_to_array()
{
  ACE_TRACE("ACE_ARGV::string_to_array");

  // Reset the number of arguments
  this->argc_ = 0;
  
  if (this->buf_ == 0)
    return;

 char *cp = this->buf_;

 // First pass: count arguments.

 // '#' is the start-comment token..
 while (*cp != '\0' && *cp != '#')
   {
     // Skip whitespace..
     while (isspace (*cp))
       cp++;
     
     // Increment count and move to next whitespace..
     if (*cp != '\0')
       this->argc_++;
     
     // Grok quotes....
     if (*cp == '\'' || *cp == '"')
       {
	 char quote = *cp;
	 
	 // Scan past the string..
	 for (cp++; *cp != '\0' && *cp != quote; cp++)
	   continue;

	 // '\0' implies unmatched quote..
	 if (*cp == '\0')
	   {
	     ACE_ERROR ((LM_ERROR, 
			 "unmatched %c detected\n", quote));
	     this->argc_--;
	     break;
	   } 
	 else 
	   cp++;
       }
     else // Skip over non-whitespace....
       while (*cp != '\0' && !isspace (*cp))
	 cp++;
   }
 
 // Second pass: copy arguments..
 char arg[BUFSIZ];
 
 // Make a new argv vector of argc + 1 elements.
 ACE_NEW (this->argv_, char *[this->argc_ + 1]);
 
 char *ptr = this->buf_;

 for (size_t i = 0; i < this->argc_; i++)
   {
     // Skip whitespace..
     while (isspace (*ptr))
       ptr++;
     
     // Copy next argument and move to next whitespace..
     if (*ptr == '\'' || *ptr == '"')
       {
	 char quote = *ptr++;
	 
	 for (cp = arg; 
	      *ptr != '\0' && *ptr != quote; 
	      ptr++, cp++)
	   if (unsigned (cp - arg) < sizeof arg)
	     *cp = *ptr;
	 
	 *cp = '\0';
	 if (*ptr == quote)
	   ptr++;
       }
     else
       {
	 for (cp = arg; 
	      *ptr && !isspace (*ptr); 
	      ptr++, cp++)
	   if (unsigned (cp - arg) < sizeof arg)
	     *cp = *ptr;
	 *cp = '\0';
       }
     
     // Check for environment variable substitution here.
     if (this->substitute_env_args_)
       this->argv_[i] = ACE::strenvdup (arg);
     else
       this->argv_[i] = ACE_OS::strdup (arg);
   }
 
 this->argv_[this->argc_] = 0;
}



ACE_ARGV::ACE_ARGV (char buf[], 
		    int substitute_env_args)
  : substitute_env_args_ (substitute_env_args),
    state_ (TO_PTR_ARRAY),
    argc_ (0),
    argv_ (0),
    buf_ (0),
    length_ (0)
{
  ACE_TRACE ("ACE_ARGV::ACE_ARGV char[] to char *[]");

  if ((buf == 0) || (buf[0]==0))
    return;

  // Make an internal copy of the string
  ACE_NEW(this->buf_, char[ACE_OS::strlen(buf) + 1]);
  ACE_OS::strcpy(this->buf_, buf);

  // Create this->argv_ 
  this->string_to_array();
}

ACE_ARGV::ACE_ARGV (char *argv[],
		    int substitute_env_args)
  : substitute_env_args_ (substitute_env_args),
    state_ (TO_STRING),
    argc_ (0),
    argv_ (0),
    buf_ (0),
    length_ (0)
{
  ACE_TRACE ("ACE_ARGV::ACE_ARGV char*[] to char[]");

  if (argv == 0 || argv[0] == 0)
    return;

  int buf_len = 0;
  
  // Determine the length of the buffer.

  for (int i = 0; argv[i] != 0; i++)
    {
      char *temp;

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

  ACE_NEW (this->buf_, char[buf_len + 1]);

  char *end = this->buf_;
  int j;

  for (j = 0; argv[j] != 0; j++)
    {
      char *temp;

      // Account for environment variables.
      if (this->substitute_env_args_
	  && (argv[j][0] == '$'
	  && (temp = ACE_OS::getenv (&argv[j][1])) != 0))
	end = ACE::strecpy (end, temp);
      else
	end = ACE::strecpy (end, argv[j]);

      // Add white space and advance the pointer.
      *end++ = ' ';
    }

  // Remember how many arguments there are
  this->argc_ = j;

  // Null terminate the string.
  *end = '\0';
}

ACE_ARGV::ACE_ARGV(int substitute_env_args)
  : substitute_env_args_ (substitute_env_args),
    state_ (ITERATIVE),
    argc_ (0),
    argv_ (0),
    buf_ (0),
    length_ (0)
{
  ACE_TRACE ("ACE_ARGV::ACE_ARGV Iterative");

  // Nothing to do yet -- the user puts in arguments via add()
};

int
ACE_ARGV::add(const char *next_arg)
{
  // Only allow this to work in the "iterative" verion -- the
  // ACE_ARGVs created with the one argument constructor.
  if (this->state_ != ITERATIVE)
    return -1;

  // Put the new argument at the end of the queue
  if (this->queue_.enqueue_tail ((char *)next_arg) < 0) {
    ACE_ERROR ((LM_ERROR, "Can't add more to ARGV queue"));
    return -1;
  }

  this->length_ += ACE_OS::strlen(next_arg);

  this->argc_++;

  // Wipe argv_ and buf_ away so that they will be recreated if
  // the user calls argv() or buf().
  if (this->argv_ != 0) {
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
ACE_ARGV::create_buf_from_queue(void)
{
  ACE_TRACE ("ACE_ARGV::create_buf_from_queue");

  // If the are no arguments, don't do anything
  if (this->argc_ <= 0)
    return -1;

  delete [] this->buf_;

  ACE_NEW_RETURN(this->buf_, char[this->length_ + this->argc_], -1);

  // Get an iterator over the queue
  ACE_Unbounded_Queue_Iterator<char *> iter(this->queue_);

  char **arg;
  char *ptr = this->buf_;
  size_t len;
  int more = 0;

  while (!iter.done()) {

    // Get next argument from the queue
    iter.next(arg);

    more = iter.advance();

    len = ACE_OS::strlen(*arg);

    // Copy the argument into buf_
    ACE_OS::memcpy((void *)ptr, (const void *)(*arg), len);

    // Move the pointer down
    ptr += len;

    // Put in an argument separating space
    if (more != 0) 
      *ptr++ = ' ';
  }

  // Put in the null terminator
  *ptr = 0;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Unbounded_Queue<char *>;
template class ACE_Unbounded_Queue_Iterator<char *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Unbounded_Queue<char *>
#pragma instantiate ACE_Unbounded_Queue_Iterator<char *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
