/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.
2550 Garcia Avenue
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

*/

#include "utl_string.h"
#include "global_extern.h"
#include "fe_extern.h"
#include "utl_err.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/OS_NS_ctype.h"

//////////////////////////////////////////////////////////////////////
//// Static functions may be used by non-UTL_String i.e. const char *
//////////////////////////////////////////////////////////////////////

bool
UTL_String::strcmp_caseless (
  char const* lhs,
  char const* rhs,
  bool &mixed_case)
{
  int difference;

  // Advance until (difference between the strings is found) or (End of String)
  while (0 == (difference = static_cast<int> (*lhs) - static_cast<int> (*rhs)) && *lhs)
    {
      ++lhs;
      ++rhs;
    }

  // If not (End of Strings, therefore difference found above) check if a
  // caseless match would work instead.
  mixed_case=
    (*lhs &&
     *rhs &&
     !(static_cast<int> (ACE_OS::ace_toupper (*lhs)) -
       static_cast<int> (ACE_OS::ace_toupper (*rhs))
    ) );
  if (mixed_case)
    {
      ++lhs;
      ++rhs;

      // Continue caseless compare until (difference between strings) or (End of String)
      while (0 == (difference = static_cast<int> (ACE_OS::ace_toupper (*lhs)) -
                                static_cast<int> (ACE_OS::ace_toupper (*rhs))) && *lhs)
        {
          ++lhs;
          ++rhs;
        }
    }

  // true is match (no difference found), false is difference found.
  return !difference;
}

// Compare two const char *.
bool
UTL_String::compare (char const* lhs, char const* rhs)
{
  bool result= false;
  bool mixed=  false;

  if (lhs != nullptr && rhs != nullptr && strcmp_caseless (lhs, rhs, mixed))
    {
      result= !mixed;
      if (mixed) // Strings match (differing case)
        {
          if (idl_global->case_diff_error ())
            {
              idl_global->err ()->name_case_error (
                  const_cast<char *> (lhs),
                  const_cast<char *> (rhs));

              // If we try to continue from here, we risk a crash.
              throw Bailout ();
            }
          else
            {
              idl_global->err ()->name_case_warning (
                const_cast<char *> (lhs),
                const_cast<char *> (rhs));
            }
        }
    }

  return result;
}

// Like the above but without error or warning message output.
bool
UTL_String::compare_quiet (char const* lhs, char const* rhs)
{
  bool result= false;
  bool mixed=  false;

  if (lhs != nullptr && rhs != nullptr && strcmp_caseless (lhs, rhs, mixed))
    {
      result= mixed;
    }

  return result;
}

// Get canonical representation. This is (implemented as) the all upper
// case corresponding string.
void
UTL_String::get_canonical_rep (char const* src, char *dest)
{
  while (0 != (*dest++ = static_cast<char> (ACE_OS::ace_toupper (*src++))))
    {}
}

// Get canonical representation. This is (implemented as) the all upper
// case corresponding string.
void
UTL_String::get_canonical_rep (ACE_CString &cstr)
{
  get_canonical_rep (&cstr [0], &cstr [0]);
}

void
UTL_String::get_canonical_rep (std::string &str)
{
  for (char &ch : str)
    {
      ch = static_cast<char> (ACE_OS::ace_toupper (ch));
    }
}

//////////////////////////////////////////////////////////////////////

UTL_String::UTL_String ()
  : string_ (),
    has_string_ (false)
{
}

UTL_String::UTL_String (char const* str, bool take_copy)
  : string_ (str != nullptr ? str : ""),
    has_string_ (str != nullptr)
{
  if (this->has_string_ && take_copy)
    {
      this->owned_string_.emplace (str);
      this->string_ = *this->owned_string_;
    }
}

UTL_String::UTL_String (UTL_String *s, bool force_copy)
  : string_ (s->string_),
    has_string_ (s->has_string_)
{
  if (this->has_string_ && (force_copy || s->owned_string_))
    {
      this->owned_string_.emplace (s->string_);
      this->string_ = *this->owned_string_;
    }
}

UTL_String::~UTL_String () = default;

char *
UTL_String::get_string () const
{
  return this->has_string_ ? const_cast<char *> (this->string_.data ()) : nullptr;
}

std::string_view
UTL_String::get_string_view () const
{
  return this->has_string_ ? this->string_ : std::string_view ();
}

bool
UTL_String::has_string () const
{
  return this->has_string_;
}

void
UTL_String::destroy ()
{
  this->canonical_string_.reset ();
  this->owned_string_.reset ();
  this->string_ = std::string_view ();
  this->has_string_ = false;
}

// Compare two UTL_String *.
bool
UTL_String::compare (UTL_String *s)
{
  return (this->has_string_ && s != nullptr && s->has_string () &&
          compare (this->string_.data (), s->string_.data ()));
}

bool
UTL_String::compare_quiet (UTL_String *s)
{
  return (this->has_string_ && s != nullptr && s->has_string () &&
          compare_quiet (this->string_.data (), s->string_.data ()));
}

// Get the canonical representation from a String.
char *
UTL_String::get_canonical_rep ()
{
  if (!this->canonical_string_ && this->has_string_)
    {
      this->canonical_string_.emplace (this->string_);
      get_canonical_rep (*this->canonical_string_);
    }

  return this->canonical_string_
    ? const_cast<char *> (this->canonical_string_->data ())
    : nullptr;
}

// AST Dumping.
void
UTL_String::dump (ACE_OSTREAM_TYPE &o)
{
  o << (this->has_string_ ? this->string_.data () : "");
}
