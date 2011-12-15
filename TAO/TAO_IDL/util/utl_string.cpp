// $Id$

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
  const char *lhs,
  const char *rhs,
  bool &mixed_case)
{
  int difference;

  // Advance until (difference between the strings is found) or (End of String)
  while (!(difference= static_cast<int> (*lhs) - static_cast<int> (*rhs)) && *lhs)
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
      while (!(difference= static_cast<int> (ACE_OS::ace_toupper (*lhs)) -
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
UTL_String::compare (const char *lhs, const char *rhs)
{
  bool result= false;
  bool mixed=  false;

  if (lhs && rhs && strcmp_caseless (lhs, rhs, mixed))
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
UTL_String::compare_quiet (const char *lhs, const char *rhs)
{
  bool result= false;
  bool mixed=  false;

  if (lhs && rhs && strcmp_caseless (lhs, rhs, mixed))
    {
      result= mixed;
    }

  return result;
}

// Get canonical representation. This is (implemented as) the all upper
// case corresponding string.
void
UTL_String::get_canonical_rep (const char *src, char *dest)
{
  while (!!(*dest++= static_cast<char> (ACE_OS::ace_toupper (*src++))))
    {}
}

// Get canonical representation. This is (implemented as) the all upper
// case corresponding string.
void
UTL_String::get_canonical_rep (ACE_CString &cstr)
{
  get_canonical_rep (&cstr [0], &cstr [0]);
}

//////////////////////////////////////////////////////////////////////

UTL_String::UTL_String (void)
  : copy_taken (false),
    p_str      (0),
    c_str      (0)
{
}

UTL_String::UTL_String (const char *str, bool take_copy)
  : copy_taken (str ? take_copy : false),
    p_str      (this->copy_taken ? ACE::strnew (str)
                                 : const_cast<char *>(str)),
    c_str      (0)
{
}

UTL_String::UTL_String (UTL_String *s, bool force_copy)
  : copy_taken (force_copy ? true : s->copy_taken),
    p_str      (this->copy_taken ? ACE::strnew (s->p_str)
                                 : const_cast<char *> (s->p_str)),
    c_str      (0)
{
}

UTL_String::~UTL_String (void)
{
  delete [] this->c_str;
  if (copy_taken)
    {
      ACE::strdelete (this->p_str);
    }
}

void
UTL_String::destroy (void)
{
  delete [] this->c_str;
  this->c_str = 0;
  if (this->copy_taken)
    {
       ACE::strdelete (this->p_str);
       this->copy_taken = 0;
    }
  this->p_str = 0;
}

// Compare two UTL_String *.
bool
UTL_String::compare (UTL_String *s)
{
  return (this->p_str && s && s->get_string () &&
          compare (this->p_str, s->get_string ()));
}

bool
UTL_String::compare_quiet (UTL_String *s)
{
  return (this->p_str && s && s->get_string () &&
          compare_quiet (this->p_str, s->get_string ()));
}

// Get the canonical representation from a String.
char *
UTL_String::get_canonical_rep (void)
{
  if (!this->c_str && this->p_str)
    {
      get_canonical_rep (
        this->p_str,
        this->c_str = new char [ACE_OS::strlen (this->p_str)+1]);
    }

  return this->c_str;
}

// AST Dumping.
void
UTL_String::dump (ACE_OSTREAM_TYPE &o)
{
  o << this->p_str;
}
