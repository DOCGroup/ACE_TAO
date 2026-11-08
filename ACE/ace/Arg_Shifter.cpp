#ifndef ACE_ARG_SHIFTER_T_CPP
#define ACE_ARG_SHIFTER_T_CPP

#include "ace/Arg_Shifter.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_Errno.h"
#include "ace/OS_Memory.h"
#if defined (ACE_HAS_ALLOC_HOOKS)
# include "ace/Malloc_Base.h"
#endif /* ACE_HAS_ALLOC_HOOKS */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename CHAR_TYPE>
ACE_Arg_Shifter_T<CHAR_TYPE>::ACE_Arg_Shifter_T (int &argc,
                                                 CHAR_TYPE const **argv,
                                                 CHAR_TYPE const **temp)
  : argc_ (argc),
    total_size_ (argc),
    temp_ (const_cast<CHAR_TYPE **> (temp)),
    argv_ (const_cast<CHAR_TYPE **> (argv)),
    back_ (argc - 1)
{
  this->init ();
}

template <typename CHAR_TYPE>
ACE_Arg_Shifter_T<CHAR_TYPE>::ACE_Arg_Shifter_T (int &argc,
                                                 CHAR_TYPE **argv,
                                                 CHAR_TYPE **temp)
  : argc_ (argc),
    total_size_ (argc),
    temp_ (temp),
    argv_ (argv),
    back_ (argc - 1)
{
  this->init ();
}

template <typename CHAR_TYPE>
void
ACE_Arg_Shifter_T<CHAR_TYPE>::init ()
{
  // If not provided with one, allocate a temporary array.
  if (this->temp_ == nullptr)
    {
#if defined (ACE_HAS_ALLOC_HOOKS)
      this->temp_ = reinterpret_cast<CHAR_TYPE **>
        (ACE_Allocator::instance ()->malloc (sizeof (CHAR_TYPE*) * this->total_size_));
#else
      ACE_NEW (this->temp_,
               CHAR_TYPE *[this->total_size_]);
#endif /* ACE_HAS_ALLOC_HOOKS */
      this->owns_temp_ = this->temp_ != nullptr;
    }

  if (this->temp_ != nullptr)
    {
      // Fill the temporary array.
      this->argc_ = 0;
      for (int i {}; i < this->total_size_; ++i)
        {
          this->temp_[i] = this->argv_[i];
          this->argv_[i] = nullptr;
        }
    }
  else
    {
      // Allocation failed, prohibit iteration.
      this->current_index_ = this->argc_;
      this->front_ = this->argc_;
    }
}

template <typename CHAR_TYPE>
ACE_Arg_Shifter_T<CHAR_TYPE>::~ACE_Arg_Shifter_T ()
{
  // Delete the temporary vector.
  if (this->owns_temp_)
    {
#if defined (ACE_HAS_ALLOC_HOOKS)
      ACE_Allocator::instance ()->free (this->temp_);
#else
      delete [] this->temp_;
#endif /* ACE_HAS_ALLOC_HOOKS */
    }
}

template <typename CHAR_TYPE>
CHAR_TYPE const *
ACE_Arg_Shifter_T<CHAR_TYPE>::get_current () const
{
  CHAR_TYPE const *retval {};

  if (this->is_anything_left ())
    retval = this->temp_[current_index_];

  return retval;
}

template <typename CHAR_TYPE>
CHAR_TYPE const *
ACE_Arg_Shifter_T<CHAR_TYPE>::get_the_parameter (CHAR_TYPE const *flag)
{
  // check to see if any arguments still exist
  if (!this->is_anything_left())
    return nullptr;

  // check to see if the flag is the argument
  int const offset = this->cur_arg_strncasecmp (flag);
  if (offset == -1)
    return nullptr;

  if (offset == 0)
    {
      this->consume_arg ();

      if (!this->is_parameter_next())
        {
          return nullptr;
        }
    }
  // the parameter is in the middle somewhere...
  return this->temp_[current_index_] + offset;
}

template <typename CHAR_TYPE>
int
ACE_Arg_Shifter_T<CHAR_TYPE>::cur_arg_strncasecmp (CHAR_TYPE const *flag)
{
  if (!this->is_anything_left ())
    return -1;

  size_t const flag_length = ACE_OS::strlen (flag);
  CHAR_TYPE const * const arg = this->temp_[this->current_index_];

  if (ACE_OS::strncasecmp (arg, flag, flag_length) != 0)
    return -1;

  size_t const arg_length = ACE_OS::strlen (arg);
  size_t remaining = flag_length;
  while (remaining < arg_length && arg[remaining] == CHAR_TYPE (' '))
    ++remaining;
  return (arg_length == flag_length) ? 0 : static_cast<int> (remaining);
}

template <typename CHAR_TYPE>
int
ACE_Arg_Shifter_T<CHAR_TYPE>::consume_arg (int number)
{
  int retval {};

  // Stick knowns at the end of the vector (consumed).
  if (this->is_anything_left() >= number)
    {
      for (int i {}, j {this->back_ - (number - 1)};
           i < number;
           ++i, ++j, ++this->current_index_)
        this->argv_[j] = this->temp_[this->current_index_];

      this->back_ -= number;
      retval = 1;
    }

  return retval;
}

template <typename CHAR_TYPE>
int
ACE_Arg_Shifter_T<CHAR_TYPE>::ignore_arg (int number)
{
  int retval {};

  // Keep unknowns at the head of the vector.
  if (this->is_anything_left () >= number)
    {
      for (int i {};
           i < number;
           ++i, ++this->current_index_, ++this->front_)
        this->argv_[this->front_] = this->temp_[this->current_index_];

      retval = 1;
      this->argc_ += number;
    }

  return retval;
}

template <typename CHAR_TYPE>
int
ACE_Arg_Shifter_T<CHAR_TYPE>::is_anything_left () const
{
  return this->total_size_ - this->current_index_;
}

template <typename CHAR_TYPE>
bool
ACE_Arg_Shifter_T<CHAR_TYPE>::is_option_next () const
{
  return this->is_anything_left () &&
    this->temp_[this->current_index_][0] == '-';
}

template <typename CHAR_TYPE>
bool
ACE_Arg_Shifter_T<CHAR_TYPE>::is_parameter_next () const
{
  return this->is_anything_left ()
    && this->temp_[this->current_index_][0] != '-';
}

template <typename CHAR_TYPE>
int
ACE_Arg_Shifter_T<CHAR_TYPE>::num_ignored_args () const
{
  return this->front_;
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif
