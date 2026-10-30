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

#include <string>
#include <utility>

//////////////////////////////////////////////////////////////////////
//// Static functions may be used by non-UTL_String instances.
//////////////////////////////////////////////////////////////////////

bool
UTL_String::strcmp_caseless (
  std::string_view lhs,
  std::string_view rhs,
  bool &mixed_case)
{
  mixed_case = false;
  if (lhs.size () != rhs.size ())
    {
      return false;
    }

  for (size_t i = 0; i < lhs.size (); ++i)
    {
      if (lhs[i] != rhs[i])
        {
          if (ACE_OS::ace_toupper (lhs[i]) != ACE_OS::ace_toupper (rhs[i]))
            {
              return false;
            }

          mixed_case = true;
        }
    }

  return true;
}

// Compare two strings.
bool
UTL_String::compare (std::string const& lhs, std::string const& rhs)
{
  bool result= false;
  bool mixed=  false;

  if (strcmp_caseless (lhs, rhs, mixed))
    {
      result= !mixed;
      if (mixed) // Strings match (differing case)
        {
          if (idl_global->case_diff_error ())
            {
              idl_global->err ()->name_case_error (lhs, rhs);

              // If we try to continue from here, we risk a crash.
              throw Bailout ();
            }
          else
            {
              idl_global->err ()->name_case_warning (lhs, rhs);
            }
        }
    }

  return result;
}

// Like the above but without error or warning message output.
bool
UTL_String::compare_quiet (std::string const& lhs, std::string const& rhs)
{
  bool result= false;
  bool mixed=  false;

  if (strcmp_caseless (lhs, rhs, mixed))
    {
      result= mixed;
    }

  return result;
}

// Get canonical representation. This is the all upper case corresponding string.
std::string
UTL_String::get_canonical_rep (std::string_view str)
{
  std::string result (str);

  for (char &c : result)
    {
      c = static_cast<char> (ACE_OS::ace_toupper (static_cast<unsigned char> (c)));
    }

  return result;
}

//////////////////////////////////////////////////////////////////////

UTL_String::UTL_String (char const* str)
  : value_ (str != nullptr ? str : "")
{
}

UTL_String::UTL_String (std::string value)
  : value_ (std::move (value))
{
}

UTL_String::~UTL_String () = default;

void
UTL_String::destroy ()
{
  this->canonical_rep_.clear ();
  this->value_.clear ();
}

// Compare two UTL_String *.
bool
UTL_String::compare (UTL_String const& s) const
{
  return compare (this->value_, s.value_);
}

bool
UTL_String::compare (UTL_String const *s) const
{
  return s != nullptr && this->compare (*s);
}

bool
UTL_String::compare_quiet (UTL_String const& s) const
{
  return compare_quiet (this->value_, s.value_);
}

bool
UTL_String::compare_quiet (UTL_String const *s) const
{
  return s != nullptr && this->compare_quiet (*s);
}

// Get the canonical representation from a String.
std::string const&
UTL_String::get_canonical_rep ()
{
  if (this->canonical_rep_.empty () && !this->value_.empty ())
    {
      this->canonical_rep_ = get_canonical_rep (this->value_);
    }

  return this->canonical_rep_;
}

// AST Dumping.
void
UTL_String::dump (ACE_OSTREAM_TYPE &o)
{
  o << this->value_;
}
