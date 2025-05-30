#ifndef ACE_ARGV_CPP
#define ACE_ARGV_CPP

#include "ace/Log_Category.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_Memory.h"

#if !defined (__ACE_INLINE__)
#include "ace/ARGV.inl"
#endif /* __ACE_INLINE__ */

// Open versioned namespace, if enabled by the user.
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE_Tt (ACE_ARGV_Queue_Entry_T)
ACE_ALLOC_HOOK_DEFINE_Tt (ACE_ARGV_T)

template <typename CHAR_TYPE>
void
ACE_ARGV_Queue_Entry_T<CHAR_TYPE>::dump () const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_ARGV_Queue_Entry_T::dump");

  ACELIB_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACELIB_DEBUG ((LM_DEBUG,  ACE_TEXT ("arg_ = %s"), this->arg_));
  ACELIB_DEBUG ((LM_DEBUG,  ACE_TEXT ("quote_arg_ = %d"), (int)this->quote_arg_));
  ACELIB_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

template <typename CHAR_TYPE>
void
ACE_ARGV_T<CHAR_TYPE>::dump () const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_ARGV_T::dump");

  ACELIB_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACELIB_DEBUG ((LM_DEBUG,  ACE_TEXT ("argc_ = %d"), this->argc_));

  ACE_ARGV *this_obj = const_cast<ACE_ARGV *> (this);

  for (int i = 0; i < this->argc_; i++)
    ACELIB_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\nargv_[%i] = %s"),
                i,
                this_obj->argv ()[i]));

  ACELIB_DEBUG ((LM_DEBUG,  ACE_TEXT ("\nbuf = %s\n"), this->buf_));
  ACELIB_DEBUG ((LM_DEBUG,  ACE_TEXT ("\n")));
  ACELIB_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

// Creates this->argv_ out of this->buf_.  New memory is allocated for
// each element of the array.  This is used by the array-to-string
// style constructor and for creating this->argv_ when in iterative
// mode.

template <typename CHAR_TYPE>
int
ACE_ARGV_T<CHAR_TYPE>::string_to_argv ()
{
  ACE_TRACE ("ACE_ARGV_T::string_to_argv");

  return ACE_OS::string_to_argv (this->buf_,
                                 this->argc_,
                                 this->argv_,
                                 this->substitute_env_args_);
}

template <typename CHAR_TYPE>
ACE_ARGV_T<CHAR_TYPE>::ACE_ARGV_T (const CHAR_TYPE buf[],
                                   bool substitute_env_args)
  : substitute_env_args_ (substitute_env_args),
    iterative_ (false),
    argc_ (0),
    argv_ (0),
    buf_ (0),
    length_ (0),
    queue_ ()
{
  ACE_TRACE ("ACE_ARGV_T::ACE_ARGV_T CHAR_TYPE[] to CHAR_TYPE *[]");

  if (buf == 0 || buf[0] == 0)
    return;

  // Make an internal copy of the string.
#if defined (ACE_HAS_ALLOC_HOOKS)
  ACE_ALLOCATOR(this->buf_,
                          static_cast<char*> (ACE_Allocator::instance()->malloc(sizeof (CHAR_TYPE) * (ACE_OS::strlen (buf) + 1))));
#else
  ACE_NEW (this->buf_,
           CHAR_TYPE[ACE_OS::strlen (buf) + 1]);
#endif /* ACE_HAS_ALLOC_HOOKS */

  ACE_OS::strcpy (this->buf_, buf);

  // Create this->argv_.
  if (this->string_to_argv () == -1)
    ACELIB_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("string_to_argv")));
}

template <typename CHAR_TYPE>
ACE_ARGV_T<CHAR_TYPE>::ACE_ARGV_T (CHAR_TYPE *argv[],
                                   bool substitute_env_args,
                                   bool quote_arg)
  : substitute_env_args_ (substitute_env_args),
    iterative_ (false),
    argc_ (0),
    argv_ (0),
    buf_ (0),
    length_ (0),
    queue_ ()
{
  ACE_TRACE ("ACE_ARGV_T::ACE_ARGV_T CHAR_TYPE*[] to CHAR_TYPE[]");

  if (argv == 0 || argv[0] == 0)
    return;

  this->argc_ = ACE_OS::argv_to_string (argv,
                                        this->buf_,
                                        substitute_env_args,
                                        quote_arg);
}

template <typename CHAR_TYPE>
ACE_ARGV_T<CHAR_TYPE>::ACE_ARGV_T (int argc,
                                   CHAR_TYPE *argv[],
                                   bool substitute_env_args,
                                   bool quote_arg)
  : substitute_env_args_ (substitute_env_args),
    iterative_ (false),
    argc_ (0),
    argv_ (0),
    buf_ (0),
    length_ (0),
    queue_ ()
{
  ACE_TRACE ("ACE_ARGV_T::ACE_ARGV_T int,CHAR_TYPE*[] to CHAR_TYPE[]");

  this->argc_ = ACE_OS::argv_to_string (argc,
                                        argv,
                                        this->buf_,
                                        substitute_env_args,
                                        quote_arg);
}


template <typename CHAR_TYPE>
ACE_ARGV_T<CHAR_TYPE>::ACE_ARGV_T (CHAR_TYPE *first_argv[],
                                   CHAR_TYPE *second_argv[],
                                   bool substitute_env_args,
                                   bool quote_args)
  : substitute_env_args_ (substitute_env_args),
    iterative_ (false),
    argc_ (0),
    argv_ (0),
    buf_ (0),
    length_ (0),
    queue_ ()
{
  ACE_TRACE ("ACE_ARGV_T::ACE_ARGV_T CHAR_TYPE*[] + CHAR_TYPE *[] to CHAR_TYPE[]");

  int first_argc = 0;
  int second_argc = 0;

  CHAR_TYPE *first_buf = 0;
  CHAR_TYPE *second_buf = 0;
  size_t buf_len = 1;

  // convert the first argv to a string
  if (first_argv != 0 && first_argv[0] != 0)
    {
      first_argc = ACE_OS::argv_to_string (first_argv,
                                           first_buf,
                                           substitute_env_args,
                                           quote_args);
      buf_len += ACE_OS::strlen (first_buf);
    }

  // convert the second argv to a string
  if (second_argv != 0 && second_argv[0] != 0)
    {
      second_argc = ACE_OS::argv_to_string (second_argv,
                                            second_buf,
                                            substitute_env_args,
                                            quote_args);
      buf_len += ACE_OS::strlen (second_buf);
    }

  // Add the number of arguments in both the argvs.
  this->argc_ = first_argc + second_argc;

  // Allocate memory to the lenght of the combined argv string.
  ACE_NEW (this->buf_,
           CHAR_TYPE[buf_len + 1]);

  // copy the first argv string to the buffer
  ACE_OS::strcpy (this->buf_, first_buf);

  // concatenate the second argv string to the buffer
  ACE_OS::strcat (this->buf_, second_buf);

  //   Delete the first and second buffers
#if defined (ACE_HAS_ALLOC_HOOKS)
  ACE_Allocator::instance()->free (first_buf);
  ACE_Allocator::instance()->free (second_buf);
#else
  delete [] first_buf;
  delete [] second_buf;
#endif /* ACE_HAS_ALLOC_HOOKS */
}

template <typename CHAR_TYPE>
ACE_ARGV_T<CHAR_TYPE>::ACE_ARGV_T (bool substitute_env_args)
  : substitute_env_args_ (substitute_env_args),
    iterative_ (true),
    argc_ (0),
    argv_ (0),
    buf_ (0),
    length_ (0),
    queue_ ()
{
  ACE_TRACE ("ACE_ARGV_T::ACE_ARGV_T Iterative");

  // Nothing to do yet -- the user puts in arguments via add ()
}

template <typename CHAR_TYPE>
int
ACE_ARGV_T<CHAR_TYPE>::add (const CHAR_TYPE *next_arg, bool quote_arg)
{
  // Only allow this to work in the "iterative" verion -- the
  // ACE_ARGVs created with the one argument constructor.
  if (!this->iterative_)
    {
      errno = EINVAL;
      return -1;
    }

  this->length_ += ACE_OS::strlen (next_arg);
  if (quote_arg && ACE_OS::strchr (next_arg, ' ') != 0)
    {
      this->length_ += 2;
      if (ACE_OS::strchr (next_arg, '"') != 0)
        for (const CHAR_TYPE * p = next_arg; *p != '\0'; ++p)
          if (*p == '"') ++this->length_;
    }
  else
    {
      quote_arg = false;
    }

  // Put the new argument at the end of the queue.
  if (this->queue_.enqueue_tail (ACE_ARGV_Queue_Entry_T<CHAR_TYPE> (next_arg, quote_arg)) == -1)
    ACELIB_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Can't add more to ARGV queue")),
                      -1);

  ++this->argc_;

  // Wipe argv_ and buf_ away so that they will be recreated if the
  // user calls argv () or buf ().
  if (this->argv_ != 0)
    {
      for (int i = 0; this->argv_[i] != 0; i++)
        ACE_OS::free ((void *) this->argv_[i]);

#if defined (ACE_HAS_ALLOC_HOOKS)
      ACE_Allocator::instance()->free (this->argv_);
#else
      delete [] this->argv_;
#endif /* ACE_HAS_ALLOC_HOOKS */
      this->argv_ = 0;
    }

#if defined (ACE_HAS_ALLOC_HOOKS)
  ACE_Allocator::instance()->free (this->buf_);
#else
  delete [] this->buf_;
#endif /* ACE_HAS_ALLOC_HOOKS */
  this->buf_ = 0;

  return 0;
}

template <typename CHAR_TYPE>
int
ACE_ARGV_T<CHAR_TYPE>::add (CHAR_TYPE *argv[], bool quote_args)
{
  for (int i = 0; argv[i] != 0; i++)
    if (this->add (argv[i], quote_args) == -1)
      return -1;

  return 0;
}

// Free up argv_ and buf_
template <typename CHAR_TYPE>
ACE_ARGV_T<CHAR_TYPE>::~ACE_ARGV_T ()
{
  ACE_TRACE ("ACE_ARGV_T::~ACE_ARGV_T");

  if (this->argv_ != 0)
    for (int i = 0; this->argv_[i] != 0; i++)
#if defined (ACE_HAS_ALLOC_HOOKS)
      ACE_Allocator::instance()->free ((void *) this->argv_[i]);
#else
      ACE_OS::free ((void *) this->argv_[i]);
#endif /* ACE_HAS_ALLOC_HOOKS */


#if defined (ACE_HAS_ALLOC_HOOKS)
  ACE_Allocator::instance()->free (this->argv_);
  ACE_Allocator::instance()->free (this->buf_);
#else
  delete [] this->argv_;
  delete [] this->buf_;
#endif /* ACE_HAS_ALLOC_HOOKS */
}

// Create buf_ out of the queue_.  This is only used in the
// "iterative" mode.

template <typename CHAR_TYPE>
int
ACE_ARGV_T<CHAR_TYPE>::create_buf_from_queue ()
{
  ACE_TRACE ("ACE_ARGV_T::create_buf_from_queue");

  // If the are no arguments, don't do anything
  if (this->argc_ <= 0)
    return -1;

#if defined (ACE_HAS_ALLOC_HOOKS)
  ACE_Allocator::instance()->free (this->buf_);
#else
  delete [] this->buf_;
#endif /* ACE_HAS_ALLOC_HOOKS */

#if defined (ACE_HAS_ALLOC_HOOKS)
  ACE_ALLOCATOR_RETURN (this->buf_,
                        static_cast<char*> (ACE_Allocator::instance()->malloc(sizeof (CHAR_TYPE) * (this->length_ + this->argc_))),
                        -1);
#else
  ACE_NEW_RETURN (this->buf_,
                  CHAR_TYPE[this->length_ + static_cast<unsigned int> (this->argc_)],
                  -1);
#endif /* ACE_HAS_ALLOC_HOOKS */

  // Get an iterator over the queue
  ACE_Unbounded_Queue_Iterator<ACE_ARGV_Queue_Entry_T<CHAR_TYPE> > iter (this->queue_);

  ACE_ARGV_Queue_Entry_T<CHAR_TYPE> *arg = 0;
  CHAR_TYPE *ptr = this->buf_;
  size_t len;

  while (!iter.done ())
    {
      // Get next argument from the queue.
      iter.next (arg);
      iter.advance ();

      if (arg->quote_arg_)
        {
          *ptr++ = '"';
          if (ACE_OS::strchr (arg->arg_, '"') != 0)
            {
              CHAR_TYPE prev = 0;
              for (const CHAR_TYPE * p = arg->arg_; *p != '\0'; ++p)
                {
                  if (*p == '"' && prev != '\\') *ptr++ = '\\';
                  prev = *ptr++ = *p;
                }
            }
          else
            {
              len = ACE_OS::strlen (arg->arg_);
              // Copy the argument into buf_
              ACE_OS::memcpy ((void *) ptr,
                              (const void *) (arg->arg_),
                              len * sizeof (CHAR_TYPE));
              // Move the pointer down.
              ptr += len;
            }
          *ptr++ = '"';
        }
      else
        {
          len = ACE_OS::strlen (arg->arg_);
          // Copy the argument into buf_
          ACE_OS::memcpy ((void *) ptr,
                          (const void *) (arg->arg_),
                          len * sizeof (CHAR_TYPE));
          // Move the pointer down.
          ptr += len;
        }

      // Put in an argument separating space.
      *ptr++ = ' ';
    }

  // Put in the NUL terminator
  ptr[-1] = '\0';

  return 0;
}

// Close versioned namespace, if enabled by the user.
ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_ARGV_CPP */
