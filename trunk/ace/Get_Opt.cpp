// Get_Opt.cpp
// $Id$

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "ace/Get_Opt.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Get_Opt, "$Id$")

/*
 * Copyright (c) 1987, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*-
 * Copyright (c) 2000 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Dieter Baron and Thomas Klausner.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed by the NetBSD
 *        Foundation, Inc. and its contributors.
 * 4. Neither the name of The NetBSD Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

ACE_ALLOC_HOOK_DEFINE(ACE_Get_Opt)

ACE_Get_Opt::ACE_Get_Opt (int argc,
                          ACE_TCHAR **argv,
                          const ACE_TCHAR *optstring,
                          int skip,
                          int report_errors,
                          int ordering,
                          int long_only)
  : argc_ (argc),
    argv_ (argv),
    optstring_ (optstring),
    opt_ind_ (skip),
    opt_err_ (report_errors),
    long_only_ (long_only),
    has_colon_ (0),
    opt_arg_ (0),
    nextchar_ (0),
    ordering_ (ordering),
    nonopt_start_ (opt_ind_),
    nonopt_end_ (opt_ind_),
    long_option_ (0)
{
  ACE_TRACE ("ACE_Get_Opt::ACE_Get_Opt");

  // First check to see if POSIXLY_CORRECT was set.
  if (ACE_OS::getenv (ACE_TEXT("POSIXLY_CORRECT"))) 
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
        case '+':
          this->ordering_ = REQUIRE_ORDER;
          break;
        case '-':
          this->ordering_ = RETURN_IN_ORDER;
          break;
        case ':':
          this->has_colon_ = 1;
          break;
        default:
          // Quit as soon as we see something else...
          done = 1;
          break;
        }
    }
}

ACE_Get_Opt::~ACE_Get_Opt (void)
{   
  ACE_TRACE ("ACE_Get_Opt::~ACE_Get_Opt");

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
}

int
ACE_Get_Opt::nextchar_i (void)
{
  ACE_TRACE ("ACE_Get_Opt::nextchar_i");

  if (this->ordering_ == PERMUTE_ARGS)
    if (this->permute () == EOF)
      return EOF;

  // Update scanning pointer.
  if (this->opt_ind_ >= this->argc_)
    {
      // We're done...
      this->nextchar_ = 0;
      return EOF;
    }
  else if (*(this->nextchar_ = this->argv_[this->opt_ind_]) != '-' 
            || this->nextchar_[1] == '\0')
    {
      // We didn't get an option.

      if (this->ordering_ == REQUIRE_ORDER 
          || this->ordering_ == PERMUTE_ARGS)
        // If we permuted or require the options to be in order, we're done.
        return EOF;

      // It must be RETURN_IN_ORDER...
      this->opt_arg_ = this->argv_[this->opt_ind_++];
      this->nextchar_ = 0;
      return 1;
    }
  else if (this->nextchar_[1] != 0
           && *++this->nextchar_ == '-'
           && this->nextchar_[1] == 0)
    {
      // Found "--" so we're done...
      ++this->opt_ind_;
      this->nextchar_ = 0;
      return EOF;
    }

  // If it's a long option, and we allow long options advance nextchar_.
  if (*this->nextchar_ == '-' && this->long_opts_.size () != 0)
    this->nextchar_++;

  return 0;
}

int
ACE_Get_Opt::long_option_i (void)
{
  ACE_TRACE ("ACE_Get_Opt::long_option_i");

  ACE_Get_Opt_Long_Option *p;
  ACE_TCHAR *s = this->nextchar_;
  int hits = 0;
  int exact = 0;
  ACE_Get_Opt_Long_Option *pfound = 0;
  int indfound = 0;

  // Advance to the end of the long option name so we can use
  // it to get the length for a string compare.
  while (*s && *s != '=')
    s++;

  size_t len = s - this->nextchar_;
  size_t size = this->long_opts_.size ();
  u_int option_index = 0;
  for (option_index = 0; option_index < size ; option_index++)
    {
      p = this->long_opts_[option_index];
      ACE_ASSERT (p);

      if (!ACE_OS::strncmp (p->name_.c_str (), this->nextchar_, len))
        {
          // Got at least a partial match.
          pfound = p;
          indfound = option_index;
          hits += 1;
          if (len == p->name_.length ())
            {
              // And in fact, it an exact match, so let's use it.
              exact = 1;
              break;
            }
        }
    }

  if ((hits > 1) && !exact)
    {
      // Great, we found a match, but unfortunately we found more than
      // one and it wasn't exact. 
      if (this->opt_err_)
	ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("%s: option `%s' is ambiguous\n"),
		    this->argv_[0], this->argv_[this->opt_ind_]));
      this->nextchar_ = 0;
      this->opt_ind_++;
      return '?';
    }
  
  if (pfound != 0)
    {
      // Okay, we found a good one (either a single hit or an exact match).
      option_index = indfound;
      this->opt_ind_++;
      if (*s)
        {
          // s must point to '=' which means there's an argument (well close enougth).
          if (pfound->has_arg_ != NO_ARG)
            // Good, we want an argument and here it is.
            this->opt_arg_ = ++s;
          else
            {
              // Whoops, we've got what looks like an argument, but we don't want one.
              if (this->opt_err_)
                  ACE_ERROR ((LM_ERROR,
                              ACE_LIB_TEXT ("%s: long option `--%s' doesn't allow an argument\n"),
                              this->argv_[0], pfound->name_.c_str ()));
              // The spec doesn't cover this, so we keep going and the program
              // doesn't know we ignored an argument if opt_err is off!!!
            }
        }
      else if (pfound->has_arg_ == ARG_REQUIRED)
        {
          // s didn't help us, but we need an argument. Note that optional arguments
          // for long options must use the "=" syntax, so we won't get here
          // in that case.
          if (this->opt_ind_ < this->argc_)
            // We still have some elements left, so use the next one.
            this->opt_arg_ = this->argv_[this->opt_ind_++];
          else
            {
              // All out of elements, so we have to punt...
              if (this->opt_err_)
                ACE_ERROR ((LM_ERROR,
                            ACE_LIB_TEXT ("%s: long option '--%s' requires an argument\n"),
                            this->argv_[0], pfound->name_.c_str ()));
              this->nextchar_ = 0;
              return this->has_colon_ ? ':' : '?';
            }
        }
      this->nextchar_ = 0;
      this->long_option_ = pfound;
      // Since val_ has to be either a valid short option or 0, this works
      // great.  If the user really wants to know if a long option was passed.
      return pfound->val_;
    }
  if (!this->long_only_ || this->argv_[this->opt_ind_][1] == '-'
      || this->optstring_.find (*this->nextchar_) == ACE_TString::npos)
    {
      // Okay, we couldn't find a long option.  If it isn't long_only (which
      // means try the long first, and if not found try the short) or a long
      // signature was passed, e.g. "--", or it's not a short (not sure when
      // this could happen) it's an error.
      if (this->opt_err_)
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("%s: illegal long option '--%s'\n"),
                    this->argv_[0], this->nextchar_));
      this->nextchar_ = 0;
      this->opt_ind_++;
      return '?';
    }
  return this->short_option_i ();
}

int
ACE_Get_Opt::short_option_i (void)
{
  ACE_TRACE ("ACE_Get_Opt::short_option_i");

  /* Look at and handle the next option-character.  */
  ACE_TCHAR opt = *this->nextchar_++;
  ACE_TCHAR *oli = 0;
  oli = ACE_const_cast (ACE_TCHAR*, ACE_OS::strchr (this->optstring_.c_str (), opt));
  
  /* Increment `optind' when we start to process its last character.  */
  if (*this->nextchar_ == '\0')
    ++this->opt_ind_;
  
  if (oli == 0 || opt == ':')
    {
      if (this->opt_err_)
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("%s: illegal short option -- %c\n"),
                    this->argv_[0], opt));
      return '?';
    }
  if (opt == 'W' && oli[1] == ';')
    {
      if (this->nextchar_[0] == 0)
        this->nextchar_ = this->argv_[this->opt_ind_];
      return long_option_i ();
    }
  if (oli[1] == ':')
    {
      if (oli[2] == ':')
        {
          // Takes an optional argument, and since short option args must
          // must follow directly in the same argument, a NULL nextchar_ 
          // means we didn't get one.
          if (*this->nextchar_ != '\0')
            {
              this->opt_arg_ = this->nextchar_;
              this->opt_ind_++;
            }
          else
            this->opt_arg_ = 0;
          this->nextchar_ = 0;
        }
      else
        {
          // Takes a required argument.
          if (*this->nextchar_ != '\0')
            {
              // Found argument in same argv-element.
              this->opt_arg_ = this->nextchar_;
              this->opt_ind_++;
            }
          else if (this->opt_ind_ == this->argc_)
            {
              // Ran out of arguments before finding required argument.
              if (this->opt_err_)
                ACE_ERROR ((LM_ERROR,
                            ACE_LIB_TEXT ("%s: short option requires an argument -- %c\n"),
                            this->argv_[0], opt));
              opt = this->has_colon_ ? ':' : '?';
            }
          else
            // Use the next argv-element as the argument.
            this->opt_arg_ = this->argv_[this->opt_ind_++];
          this->nextchar_ = 0;
        }
    }
  return opt;
}

int
ACE_Get_Opt::operator () (void)
{
  ACE_TRACE ("ACE_Get_Opt_Long::operator");

  // First of all, make sure we reinitialize any pointers..
  this->opt_arg_ = 0;
  this->long_option_ = 0;

  this->opt_arg_ = 0;
  if (this->argv_ == 0)
    {
      // It can happen, e.g., on VxWorks.
      this->opt_ind_ = 0;
      return -1;
    }

  // We check this because we can string short options together if the preceding one doesn't take 
  // an argument.
  if (this->nextchar_ == 0 || *this->nextchar_ == '\0')
    {
      int retval = this->nextchar_i ();
      if (retval != 0)
        return retval;
    }

  if (((this->argv_[this->opt_ind_][0] == '-')
       && (this->argv_[this->opt_ind_][1] == '-')) || this->long_only_)
    return this->long_option_i ();

  return this->short_option_i ();
}

int 
ACE_Get_Opt::long_option (const ACE_TCHAR *name,
                          OPTION_ARG_MODE has_arg)
{
  ACE_TRACE ("ACE_Get_Opt::long_option (const ACE_TCHAR *name, OPTION_ARG_MODE has_arg)");
  return this->long_option (name, 0, has_arg);
}

int 
ACE_Get_Opt::long_option (const ACE_TCHAR *name,
                          int short_option,
                          OPTION_ARG_MODE has_arg)
{
  ACE_TRACE ("ACE_Get_Opt::long_option (const ACE_TCHAR *name, int short_option, OPTION_ARG_MODE has_arg)");

  // We only allow valid alpha-numeric characters as short options.  If short_options
  // is not a valid alpha-numeric, we can still return it when the long option
  // is found, but won't allow the caller to pass it on the command line (how could
  // they?).  The special case is 0, but since we always return it, we let the caller
  // worry about that.  
  if (isalnum (short_option) != 0)
    {
      // If the short_option already exists, make sure it matches, otherwise
      // add it.
      ACE_TCHAR *s = 0;
      if ((s = ACE_const_cast (ACE_TCHAR*, 
                               ACE_OS::strchr (this->optstring_.c_str (), short_option))) != 0)
        {
          // Short option exists, so verify the argument options
          if (s[1] == ':')
            {
              if (s[2] == ':')
                {
                  if (has_arg != ARG_OPTIONAL)
                    ACE_ERROR_RETURN ((LM_ERROR, 
                                       "Argument type for existing short option '%c' "
                                       "does not match '%s'-- != 'ARG_OPTIONAL'\n",
                                       short_option, s), -1);
                }
              else
                if (has_arg != ARG_REQUIRED)
                  ACE_ERROR_RETURN ((LM_ERROR, 
                                     "Argument type for existing short option '%c' "
                                     "does not match '%s' -- != 'ARG_REQUIRED'\n", 
                                     short_option, s), -1);
            }
          else if (has_arg != NO_ARG)
            ACE_ERROR_RETURN ((LM_ERROR, 
                               "Argument type for existing short option '%c' "
                               "does not match '%s' -- != 'NO_ARG'\n", 
                               short_option, s), -1);
        }
      else
        {
          // Didn't find short option, so add it...
          this->optstring_ += (ACE_TCHAR) short_option;
          if (has_arg == ARG_REQUIRED)
            this->optstring_ += ACE_TEXT (":");
          else if (has_arg == ARG_OPTIONAL)
            this->optstring_ += ACE_TEXT ("::");
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
      ACE_ERROR_RETURN ((LM_ERROR, 
                         "Could not add long option to array.\n"), -1);
    }
  return 0;
}

const ACE_TCHAR*
ACE_Get_Opt::long_option (void) const
{
  ACE_TRACE ("ACE_Get_Opt::long_option (void)");
  if (this->long_option_)
    return this->long_option_->name_.c_str ();
  return 0;
}

void
ACE_Get_Opt::dump (void) const
{
  ACE_TRACE ("ACE_Get_Opt::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void
ACE_Get_Opt::permute_args (void)
{
  ACE_TRACE ("ACE_Get_Opt::permute_args");

  int cstart, cyclelen, i, j, ncycle, nnonopts, nopts, pos;
  ACE_TCHAR *swap;
  int opt_end = this->opt_ind_;

  nnonopts = this->nonopt_end_ - this->nonopt_start_;
  nopts = opt_end - this->nonopt_end_;
  ncycle = ACE::gcd (nnonopts, nopts);
  cyclelen = (opt_end - this->nonopt_start_) / ncycle;

  this->opt_ind_ = this->opt_ind_ - nnonopts;

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
          
          ((ACE_TCHAR **)this->argv_)[pos] = argv_[cstart];
          
          ((ACE_TCHAR **)this->argv_)[cstart] = swap;
        }
    }
}

int
ACE_Get_Opt::permute (void)         
{
  ACE_TRACE ("ACE_Get_Opt::permute");

  if (this->nonopt_start_ != this->nonopt_end_
      && this->nonopt_start_ != this->opt_ind_)
    this->permute_args ();

  this->nonopt_start_ = this->opt_ind_;

  // Skip over args untill we find the next option.
  while (this->opt_ind_ < this->argc_
         && (this->argv_[this->opt_ind_][0] != '-' 
             || this->argv_[this->opt_ind_][1] == '\0'))
    this->opt_ind_++;

  // Got an option, so mark this as the end of the non options.
  this->nonopt_end_ = this->opt_ind_;

  if (this->opt_ind_ != this->argc_ 
      && ACE_OS::strcmp (this->argv_[this->opt_ind_], "--") == 0)
    {
      // We found the marker for the end of the options.
      this->opt_ind_++;

      if (this->nonopt_start_ != this->nonopt_end_ 
          && this->nonopt_end_ != this->opt_ind_)
        this->permute_args ();
    }
  
  if (this->opt_ind_ == this->argc_)
    {
      if (this->nonopt_start_ != this->nonopt_end_)
        this->opt_ind_ = this->nonopt_start_;
      return EOF;
    }
  return 0;
} 

