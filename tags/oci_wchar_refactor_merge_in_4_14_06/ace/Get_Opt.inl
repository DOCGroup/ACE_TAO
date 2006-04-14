// -*- C++ -*-
//
// $Id$

#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "ace/SString.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_stdlib.h"

ACE_ALLOC_HOOK_DEFINE(ACE_Get_Arg_Opt)

template < typename CHAR_TYPE >
ACE_Get_Arg_Opt<CHAR_TYPE>::ACE_Get_Arg_Opt (int argc,
                          Char **argv,
                          const Char *optstring,
                          int skip,
                          int report_errors,
                          int ordering,
                          int long_only)
  : argc_ (argc),
    argv_ (argv),
    optind (skip),
    opterr (report_errors),
    optarg (0),
    optstring_ (0),
    long_only_ (long_only),
    has_colon_ (0),
    last_option_ (0),
    nextchar_ (0),
    optopt_ (0),
    ordering_ (ordering),
    nonopt_start_ (optind),
    nonopt_end_ (optind),
    long_option_ (0)
{
  ACE_TRACE ("ACE_Get_Arg_Opt::ACE_Get_Arg_Opt");

  if ( optstring != 0 )
    ACE_NEW (this->optstring_, String (optstring));
  else
    ACE_NEW (this->optstring_, String);
  ACE_NEW (this->last_option_, String);

  // First check to see if POSIXLY_CORRECT was set.
  // Win32 is the only platform capable of wide-char env var.
#if defined (ACE_WIN32)
  const ACE_TCHAR *env_check = ACE_LIB_TEXT ("POSIXLY_CORRECT");
#else
  const char *env_check = "POSIXLY_CORRECT";
#endif
  if (ACE_OS::getenv (env_check) != 0)
    this->ordering_ = REQUIRE_ORDER;

  // Now, check to see if any or the following were passed at
  // the begining of optstring: '+' same as POSIXLY_CORRECT;
  // '-' turns off POSIXLY_CORRECT; or ':' which signifies we
  // should return ':' if a parameter is missing for an option.
  // We use a loop here, since a combination of "{+|-}:" in any
  // order should be legal.
  int done  = 0;
  int offset = 0;
  while (!done)
    {
      switch (optstring[offset++])
        {
        case ACE_LIB_TEXT('+'):
          this->ordering_ = REQUIRE_ORDER;
          break;
        case ACE_LIB_TEXT('-'):
          this->ordering_ = RETURN_IN_ORDER;
          break;
        case ACE_LIB_TEXT(':'):
          this->has_colon_ = 1;
          break;
        default:
          // Quit as soon as we see something else...
          done = 1;
          break;
        }
    }
}

template < typename CHAR_TYPE >
ACE_Get_Arg_Opt<CHAR_TYPE>::~ACE_Get_Arg_Opt (void)
{
  ACE_TRACE ("ACE_Get_Arg_Opt::~ACE_Get_Arg_Opt");

  size_t i = 0;
  size_t size = this->long_opts_.size ();
  ACE_Get_Opt_Long_Option *option = 0;
  for (i = 0; i < size; ++i)
    {
     int retval = this->long_opts_.get (option, i);
      if (retval != 0)
        {
          // Should never happen.
          retval = 0;
          continue;
        }
      if (option)
        {
          delete option;
          option = 0;
        }
    }
  delete this->optstring_;
  delete this->last_option_;
}

template < typename CHAR_TYPE >
typename ACE_Get_Arg_Opt<CHAR_TYPE>::Char
ACE_Get_Arg_Opt<CHAR_TYPE>::nextchar_i (void)
{
  ACE_TRACE ("ACE_Get_Arg_Opt::nextchar_i");

  if (this->ordering_ == PERMUTE_ARGS)
    if (this->permute () == eof())
      return eof();

  // Update scanning pointer.
  if (this->optind >= this->argc_)
    {
      // We're done...
      this->nextchar_ = 0;
      return eof();
    }
  else if (*(this->nextchar_ = this->argv_[this->optind]) != ACE_LIB_TEXT('-')
            || this->nextchar_[1] == ACE_LIB_TEXT('\0'))
    {
      // We didn't get an option.

      if (this->ordering_ == REQUIRE_ORDER
          || this->ordering_ == PERMUTE_ARGS)
        // If we permuted or require the options to be in order, we're done.
        return eof();

      // It must be RETURN_IN_ORDER...
      this->optarg = this->argv_[this->optind++];
      this->nextchar_ = 0;
      return 1;
    }
  else if (this->nextchar_[1] != 0
           && *++this->nextchar_ == ACE_LIB_TEXT('-')
           && this->nextchar_[1] == ACE_LIB_TEXT('\0'))
    {
      // Found "--" so we're done...
      ++this->optind;
      this->nextchar_ = 0;
      return eof();
    }

  // If it's a long option, and we allow long options advance nextchar_.
  if (*this->nextchar_ == ACE_LIB_TEXT('-') && this->long_opts_.size () != 0)
    this->nextchar_++;

  return ACE_LIB_TEXT('\0');
}

template < typename CHAR_TYPE >
int
ACE_Get_Arg_Opt<CHAR_TYPE>::long_option_i (void)
{
  ACE_TRACE ("ACE_Get_Arg_Opt::long_option_i");

  ACE_Get_Opt_Long_Option *p;
  Char *s = this->nextchar_;
  int hits = 0;
  int exact = 0;
  ACE_Get_Opt_Long_Option *pfound = 0;
  int indfound = 0;

  // Advance to the end of the long option name so we can use
  // it to get the length for a string compare.
  while (*s && *s != ACE_LIB_TEXT('='))
    s++;

  size_t len = s - this->nextchar_;
  // set last_option_ to nextchar_, up to the '='.
  this->last_option (String (this->nextchar_, len));

  size_t size = this->long_opts_.size ();
  u_int option_index = 0;
  for (option_index = 0; option_index < size ; option_index++)
    {
      p = this->long_opts_[option_index];
      ACE_ASSERT (p);

      if (!ACE_OS::strncmp (p->name_, this->nextchar_, len))
        {
          // Got at least a partial match.
          pfound = p;
          indfound = option_index;
          hits += 1;
          if (len == ACE_OS::strlen(p->name_))
            {
              // And in fact, it's an exact match, so let's use it.
              exact = 1;
              break;
            }
        }
    }

  if ((hits > 1) && !exact)
    {
      // Great, we found a match, but unfortunately we found more than
      // one and it wasn't exact.
      if (this->opterr)
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("%s: option `%s' is ambiguous\n"),
                    this->argv_[0], this->argv_[this->optind]));
      this->nextchar_ = 0;
      this->optind++;
      return ACE_LIB_TEXT('?');
    }

  if (pfound != 0)
    {
      // Okay, we found a good one (either a single hit or an exact match).
      option_index = indfound;
      this->optind++;
      if (*s)
        {
          // s must point to '=' which means there's an argument (well
          // close enough).
          if (pfound->has_arg_ != NO_ARG)
            // Good, we want an argument and here it is.
            this->optarg = ++s;
          else
            {
              // Whoops, we've got what looks like an argument, but we
              // don't want one.
              if (this->opterr)
                  ACE_ERROR
                    ((LM_ERROR,
                      ACE_LIB_TEXT ("%s: long option `--%s' doesn't allow ")
                      ACE_LIB_TEXT ("an argument\n"),
                      this->argv_[0], pfound->name_));
              // The spec doesn't cover this, so we keep going and the program
              // doesn't know we ignored an argument if opt_err is off!!!
            }
        }
      else if (pfound->has_arg_ == ARG_REQUIRED)
        {
          // s didn't help us, but we need an argument. Note that
          // optional arguments for long options must use the "=" syntax,
          // so we won't get here in that case.
          if (this->optind < this->argc_)
            // We still have some elements left, so use the next one.
            this->optarg = this->argv_[this->optind++];
          else
            {
              // All out of elements, so we have to punt...
              if (this->opterr)
                ACE_ERROR ((LM_ERROR,
                            ACE_LIB_TEXT ("%s: long option '--%s' requires ")
                            ACE_LIB_TEXT ("an argument\n"),
                            this->argv_[0], pfound->name_));
              this->nextchar_ = 0;
              this->optopt_ = pfound->val_;   // Remember matching short equiv
              return this->has_colon_ ? ACE_LIB_TEXT(':') : ACE_LIB_TEXT('?');
            }
        }
      this->nextchar_ = 0;
      this->long_option_ = pfound;
      // Since val_ has to be either a valid short option or 0, this works
      // great.  If the user really wants to know if a long option was passed.
      this->optopt_ = pfound->val_;
      return pfound->val_;
    }
  if (!this->long_only_ || this->argv_[this->optind][1] == ACE_LIB_TEXT('-')
      || this->optstring_->find (*this->nextchar_) == String::npos)
    {
      // Okay, we couldn't find a long option.  If it isn't long_only (which
      // means try the long first, and if not found try the short) or a long
      // signature was passed, e.g. "--", or it's not a short (not sure when
      // this could happen) it's an error.
      if (this->opterr)
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("%s: illegal long option '--%s'\n"),
                    this->argv_[0], this->nextchar_));
      this->nextchar_ = 0;
      this->optind++;
      return ACE_LIB_TEXT('?');
    }
  return this->short_option_i ();
}

template < typename CHAR_TYPE >
int
ACE_Get_Arg_Opt<CHAR_TYPE>::short_option_i (void)
{
  ACE_TRACE ("ACE_Get_Arg_Opt::short_option_i");

  /* Look at and handle the next option-character.  */
  Char opt = *this->nextchar_++;
  // Set last_option_ to opt
  this->last_option (opt);

  Char *oli = 0;
  oli =
    const_cast<Char*> (ACE_OS::strchr (this->optstring_->c_str (), opt));

  /* Increment `optind' when we start to process its last character.  */
  if (*this->nextchar_ == ACE_LIB_TEXT('\0'))
    ++this->optind;

  if (oli == 0 || opt == ACE_LIB_TEXT(':'))
    {
      if (this->opterr)
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("%s: illegal short option -- %c\n"),
                    this->argv_[0], opt));
      return ACE_LIB_TEXT('?');
    }
  if (opt == ACE_LIB_TEXT('W') && oli[1] == ACE_LIB_TEXT(';'))
    {
      if (this->nextchar_[0] == 0)
        this->nextchar_ = this->argv_[this->optind];
      return long_option_i ();
    }
  this->optopt_ = oli[0];      // Remember the option that matched
  if (oli[1] == ACE_LIB_TEXT(':'))
    {
      if (oli[2] == ACE_LIB_TEXT(':'))
        {
          // Takes an optional argument, and since short option args must
          // must follow directly in the same argument, a NULL nextchar_
          // means we didn't get one.
          if (*this->nextchar_ != ACE_LIB_TEXT('\0'))
            {
              this->optarg = this->nextchar_;
              this->optind++;
            }
          else
            this->optarg = 0;
          this->nextchar_ = 0;
        }
      else
        {
          // Takes a required argument.
          if (*this->nextchar_ != ACE_LIB_TEXT('\0'))
            {
              // Found argument in same argv-element.
              this->optarg = this->nextchar_;
              this->optind++;
            }
          else if (this->optind == this->argc_)
            {
              // Ran out of arguments before finding required argument.
              if (this->opterr)
                ACE_ERROR ((LM_ERROR,
                            ACE_LIB_TEXT ("%s: short option requires ")
                            ACE_LIB_TEXT ("an argument -- %c\n"),
                            this->argv_[0], opt));
              opt = this->has_colon_ ? ACE_LIB_TEXT(':') : ACE_LIB_TEXT('?');
            }
          else
            // Use the next argv-element as the argument.
            this->optarg = this->argv_[this->optind++];
          this->nextchar_ = 0;
        }
    }
  return opt;
}

template < typename CHAR_TYPE >
int
ACE_Get_Arg_Opt<CHAR_TYPE>::operator () (void)
{
  ACE_TRACE ("ACE_Get_Opt_Long::operator");

  // First of all, make sure we reinitialize any pointers..
  this->optarg = 0;
  this->long_option_ = 0;

  if (this->argv_ == 0)
    {
      // It can happen, e.g., on VxWorks.
      this->optind = 0;
      return -1;
    }

  // We check this because we can string short options together if the
  // preceding one doesn't take an argument.
  if (this->nextchar_ == 0 || *this->nextchar_ == ACE_LIB_TEXT('\0'))
    {
      int retval = this->nextchar_i ();
      if (retval != 0)
        return retval;
    }

  if (((this->argv_[this->optind][0] == ACE_LIB_TEXT('-'))
       && (this->argv_[this->optind][1] == ACE_LIB_TEXT('-'))) || this->long_only_)
    return this->long_option_i ();

  return this->short_option_i ();
}

template < typename CHAR_TYPE >
int
ACE_Get_Arg_Opt<CHAR_TYPE>::long_option (const Char *name,
                          OPTION_ARG_MODE has_arg)
{
  ACE_TRACE ("ACE_Get_Arg_Opt::long_option (const Char *name, OPTION_ARG_MODE has_arg)");
  return this->long_option (name, 0, has_arg);
}

template < typename CHAR_TYPE >
int
ACE_Get_Arg_Opt<CHAR_TYPE>::long_option (const Char *name,
                          int short_option,
                          OPTION_ARG_MODE has_arg)
{
  ACE_TRACE ("ACE_Get_Arg_Opt::long_option (const Char *name, int short_option, OPTION_ARG_MODE has_arg)");

  // We only allow valid alpha-numeric characters as short options.
  // If short_options is not a valid alpha-numeric, we can still return it
  // when the long option is found, but won't allow the caller to pass it on
  // the command line (how could they?).  The special case is 0, but since
  // we always return it, we let the caller worry about that.
#if defined (_MSC_VER) && (_MSC_VER >= 1300)
  // For MSVC 7.x, we need to prevent "illegal" character getting into
  // isalnum, otherwise, it will crash the program.
  if (short_option > 0 &&
      short_option < 256 &&
      ACE_OS::ace_isalnum (short_option) != 0)
#else
  if (ACE_OS::ace_isalnum (short_option) != 0)
#endif /* _MSC_VER && _MSC_VER >= 1300 */
    {
      // If the short_option already exists, make sure it matches, otherwise
      // add it.
      Char *s = 0;
      if ((s = const_cast<Char*> (
                 ACE_OS::strchr (this->optstring_->c_str (),
                                 short_option))) != 0)
        {
          // Short option exists, so verify the argument options
          if (s[1] == ACE_LIB_TEXT(':'))
            {
              if (s[2] == ACE_LIB_TEXT(':'))
                {
                  if (has_arg != ARG_OPTIONAL)
                    {
                      if (this->opterr)
                        ACE_ERROR
                          ((LM_ERROR,
                            ACE_LIB_TEXT ("Existing short option '%c' takes ")
                            ACE_LIB_TEXT ("optional argument; adding %s ")
                            ACE_LIB_TEXT ("requires ARG_OPTIONAL\n"),
                            short_option, name));
                      return -1;
                    }
                }
              else
                if (has_arg != ARG_REQUIRED)
                  {
                    if (this->opterr)
                      ACE_ERROR
                        ((LM_ERROR,
                          ACE_LIB_TEXT ("Existing short option '%c' requires ")
                          ACE_LIB_TEXT ("an argument; adding %s ")
                          ACE_LIB_TEXT ("requires ARG_REQUIRED\n"),
                          short_option, name));
                    return -1;
                  }
            }
          else if (has_arg != NO_ARG)
            {
              if (this->opterr)
                ACE_ERROR
                  ((LM_ERROR,
                    ACE_LIB_TEXT ("Existing short option '%c' does not ")
                    ACE_LIB_TEXT ("accept an argument; adding %s ")
                    ACE_LIB_TEXT ("requires NO_ARG\n"),
                    short_option, name));
              return -1;
            }
        }
      else
        {
          // Didn't find short option, so add it...
          *this->optstring_ += (Char) short_option;
          if (has_arg == ARG_REQUIRED)
          {
            Char delim[3] = { ':', '\0' };
            *this->optstring_ += delim;
          }
          else if (has_arg == ARG_OPTIONAL)
          {
            Char delim[3] = { ':', ':', '\0' };
            *this->optstring_ += delim;
          }
        }
    }

  ACE_Get_Opt_Long_Option *option =
    new ACE_Get_Opt_Long_Option (name, has_arg, short_option);

  if (!option)
    return -1;

  // Add to array
  size_t size = this->long_opts_.size ();
  if (this->long_opts_.size (size + 1) != 0
      || this->long_opts_.set (option, size) != 0)
    {
      delete option;
      ACE_ERROR_RETURN
        ((LM_ERROR, ACE_LIB_TEXT ("Could not add long option to array.\n")),
         -1);
    }
  return 0;
}

template < typename CHAR_TYPE >
const typename ACE_Get_Arg_Opt<CHAR_TYPE>::Char*
ACE_Get_Arg_Opt<CHAR_TYPE>::long_option (void) const
{
  ACE_TRACE ("ACE_Get_Arg_Opt::long_option (void)");
  if (this->long_option_)
    return this->long_option_->name_;
  return 0;
}

template < typename CHAR_TYPE >
const typename ACE_Get_Arg_Opt<CHAR_TYPE>::Char*
ACE_Get_Arg_Opt<CHAR_TYPE>::last_option (void) const
{
  return this->last_option_->c_str ();
}

template < typename CHAR_TYPE >
void
ACE_Get_Arg_Opt<CHAR_TYPE>::last_option (const typename ACE_Get_Arg_Opt<CHAR_TYPE>::String &last_option)
{
  *this->last_option_ = last_option;
}

template < typename CHAR_TYPE >
void
ACE_Get_Arg_Opt<CHAR_TYPE>::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Get_Arg_Opt::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")
              ACE_LIB_TEXT ("opstring_ = %s\n")
              ACE_LIB_TEXT ("long_only_ = %d\n")
              ACE_LIB_TEXT ("has_colon_ = %d\n")
              ACE_LIB_TEXT ("last_option_ = %s\n")
              ACE_LIB_TEXT ("nextchar_ = %s\n")
              ACE_LIB_TEXT ("optopt_ = %c\n")
              ACE_LIB_TEXT ("ordering_ = %d\n"),
              this->optstring_->c_str (),
              this->long_only_,
              this->has_colon_,
              this->last_option_->c_str (),
              this->nextchar_,
              this->optopt_,
              this->ordering_));

  // now loop through the
  size_t size = this->long_opts_.size ();
  for (u_int i = 0; i < size ; ++i)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")
                  ACE_LIB_TEXT ("long_option name_ = %s\n")
                  ACE_LIB_TEXT ("has_arg_ = %d\n")
                  ACE_LIB_TEXT ("val_ = %d\n"),
                  this->long_opts_[i]->name_,
                  this->long_opts_[i]->has_arg_,
                  this->long_opts_[i]->val_));
    }
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

template < typename CHAR_TYPE >
void
ACE_Get_Arg_Opt<CHAR_TYPE>::permute_args (void)
{
  ACE_TRACE ("ACE_Get_Arg_Opt::permute_args");

  int cstart, cyclelen, i, j, ncycle, nnonopts, nopts, pos;
  Char *swap;
  int opt_end = this->optind;

  nnonopts = this->nonopt_end_ - this->nonopt_start_;
  nopts = opt_end - this->nonopt_end_;
  ncycle = ACE::gcd (nnonopts, nopts);
  cyclelen = (opt_end - this->nonopt_start_) / ncycle;

  this->optind = this->optind - nnonopts;

  for (i = 0; i < ncycle; i++)
    {
      cstart = this->nonopt_end_ + i;
      pos = cstart;
      for (j = 0; j < cyclelen; j++)
        {
          if (pos >= this->nonopt_end_)
            pos -= nnonopts;
          else
            pos += nopts;
          swap = this->argv_[pos];

          ((Char **)this->argv_)[pos] = argv_[cstart];

          ((Char **)this->argv_)[cstart] = swap;
        }
    }
}

template < typename CHAR_TYPE >
int
ACE_Get_Arg_Opt<CHAR_TYPE>::permute (void)
{
  ACE_TRACE ("ACE_Get_Arg_Opt::permute");

  if (this->nonopt_start_ != this->nonopt_end_
      && this->nonopt_start_ != this->optind)
    this->permute_args ();

  this->nonopt_start_ = this->optind;

  // Skip over args untill we find the next option.
  while (this->optind < this->argc_
         && (this->argv_[this->optind][0] != ACE_LIB_TEXT('-')
             || this->argv_[this->optind][1] == ACE_LIB_TEXT('\0')))
    this->optind++;

  // Got an option, so mark this as the end of the non options.
  this->nonopt_end_ = this->optind;

  const Char doubleDash[] = { '-', '-', 0 };
  if (this->optind != this->argc_
      && ACE_OS::strcmp (this->argv_[this->optind],
                         doubleDash) == 0)
    {
      // We found the marker for the end of the options.
      this->optind++;

      if (this->nonopt_start_ != this->nonopt_end_
          && this->nonopt_end_ != this->optind)
        this->permute_args ();
    }

  if (this->optind == this->argc_)
    {
      if (this->nonopt_start_ != this->nonopt_end_)
        this->optind = this->nonopt_start_;
      return eof();
    }
  return 0;
}

template < typename CHAR_TYPE >
const typename ACE_Get_Arg_Opt<CHAR_TYPE>::Char *
ACE_Get_Arg_Opt<CHAR_TYPE>::optstring (void) const
{
  return this->optstring_->c_str ();
}

template < typename CHAR_TYPE >
ACE_Get_Arg_Opt<CHAR_TYPE>::ACE_Get_Opt_Long_Option::ACE_Get_Opt_Long_Option (
  const Char *name,
  int has_arg,
  int val)
  :  name_ (ACE::strnew (name)),
     has_arg_ (has_arg),
     val_ (val)
{}

template < typename CHAR_TYPE >
ACE_Get_Arg_Opt<CHAR_TYPE>::ACE_Get_Opt_Long_Option::~ACE_Get_Opt_Long_Option (void)
{
  // MSVC++ 6 can't deal with deleting a const Char *.
#if defined (_MSC_VER) && _MSC_VER < 1300
  delete [] const_cast <Char *> (this->name_);
#else
  delete [] this->name_;
#endif  /* _MSC_VER < 1300 */
}
template < typename CHAR_TYPE >
int
ACE_Get_Arg_Opt<CHAR_TYPE>::eof()
{
  // Since EOF may or may not be -1,
  // we have to use EOF to maintain
  // backwards compatibility.
  return EOF;
}

// Accessors

template < typename CHAR_TYPE >
bool
ACE_Get_Arg_Opt<CHAR_TYPE>::ACE_Get_Opt_Long_Option::operator < (const ACE_Get_Opt_Long_Option &rhs)
{
  return this->name_ < rhs.name_;
}

template < typename CHAR_TYPE >
int
ACE_Get_Arg_Opt<CHAR_TYPE>::argc (void) const
{
  return this->argc_;
}

template < typename CHAR_TYPE >
typename ACE_Get_Arg_Opt<CHAR_TYPE>::Char **
ACE_Get_Arg_Opt<CHAR_TYPE>::argv (void) const
{
  return this->argv_;
}

template < typename CHAR_TYPE >
typename ACE_Get_Arg_Opt<CHAR_TYPE>::Char*
ACE_Get_Arg_Opt<CHAR_TYPE>::opt_arg (void) const
{
  return this->optarg;
}

template < typename CHAR_TYPE >
typename ACE_Get_Arg_Opt<CHAR_TYPE>::Char
ACE_Get_Arg_Opt<CHAR_TYPE>::opt_opt (void)
{
  return this->optopt_;
}

template < typename CHAR_TYPE >
int &
ACE_Get_Arg_Opt<CHAR_TYPE>::opt_ind (void)
{
  return this->optind;
}
