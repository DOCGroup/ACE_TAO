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

  ACE_DEBUG ((LM_DEBUG, "\nbuf = %s\n"));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_ARGV::ACE_ARGV (char buf[], 
		    int substitute_env_args)
  : argc_ (0),
    argv_ (0), 
    buf_ (0)
{
  ACE_TRACE ("ACE_ARGV::ACE_ARGV");

  if (buf == 0)
    return;

  char *cp = buf;

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

  for (size_t i = 0; i < this->argc_; i++)
    {
      // Skip whitespace..
      while (isspace (*buf))
        buf++;
    
      // Copy next argument and move to next whitespace..
      if (*buf == '\'' || *buf == '"')
        {
          char quote = *buf++;

          for (cp = arg; 
	       *buf != '\0' && *buf != quote; 
	       buf++, cp++)
            if (unsigned (cp - arg) < sizeof arg)
              *cp = *buf;
	  
          *cp = '\0';
          if (*buf == quote)
            buf++;
        }
      else
        {
          for (cp = arg; 
	       *buf && !isspace (*buf); 
	       buf++, cp++)
            if (unsigned (cp - arg) < sizeof arg)
              *cp = *buf;
          *cp = '\0';
        }

      // Check for environment variable substitution here.
      if (substitute_env_args)
	this->argv_[i] = ACE::strenvdup (arg);
      else
	this->argv_[i] = ACE_OS::strdup (arg);
    }

  this->argv_[this->argc_] = 0;
}

ACE_ARGV::ACE_ARGV (char *argv[],
		    int substitute_env_args)
  : argc_ (0),
    argv_ (0), 
    buf_ (0)
{
  ACE_TRACE ("ACE_ARGV::ACE_ARGV");

  if (argv == 0 || argv[0] == 0)
    return;

  int buf_len = 0;

  // Determine the length of the buffer.

  for (int i = 0; argv[i] != 0; i++)
    {
      char *temp;

      // Account for environment variables.
      if (substitute_env_args
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

  ACE_NEW (this->buf_, char[buf_len]);

  char *end = this->buf_;

  for (int j = 0; argv[j] != 0; j++)
    {
      char *temp;

      // Account for environment variables.
      if (substitute_env_args
	  && (argv[j][0] == '$'
	  && (temp = ACE_OS::getenv (&argv[j][1])) != 0))
	end = ACE::strecpy (end, temp);
      else
	end = ACE::strecpy (end, argv[j]);

      // Add white space and advance the pointer.
      *end++ = ' ';
    }

  // Null terminate the string.
  *end = '\0';
}

// Free up the space allocated by the constructor..

ACE_ARGV::~ACE_ARGV (void)
{
  ACE_TRACE ("ACE_ARGV::~ACE_ARGV");
  if (this->argv_ == 0)
    return;

  for (int i = 0; this->argv_[i] != 0; i++)
    ACE_OS::free ((void *) this->argv_[i]);

  delete [] this->argv_;
  delete [] this->buf_;
}

