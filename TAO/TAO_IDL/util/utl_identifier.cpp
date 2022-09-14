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

#include "utl_identifier.h"
#include "global_extern.h"
#include "utl_err.h"
#include "utl_string.h"
#include "fe_private.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"
#include "ace/OS_NS_string.h"

Identifier::Identifier ()
  : pv_string (nullptr),
    escaped_ (false)
{
}

Identifier::Identifier (const char *s)
  : pv_string (nullptr),
    escaped_ (false)
{
  preprocess_and_replace_string (s);
}

Identifier::Identifier (const Identifier &other)
  : pv_string (nullptr),
    escaped_ (other.escaped ())
{
  *this = other;
}

Identifier::~Identifier ()
{
  if (this->pv_string != nullptr)
    {
      ACE::strdelete (this->pv_string);
    }
}

// Operations.

char *
Identifier::get_string ()
{
  return this->pv_string;
}

const char *
Identifier::get_string () const
{
  return this->pv_string;
}

void
Identifier::replace_string (const char * s)
{
  if (pv_string)
    {
      delete [] this->pv_string;
    }
  this->pv_string = s ? ACE::strnew (s) : nullptr;
}

void
Identifier::preprocess_and_replace_string (const char * s)
{
  bool shift = false;

  if (*s == '_')
    {
      // Only one leading underscore is allowed.
      if (s[1] == '_')
        {
          idl_global->err ()->error0 (UTL_Error::EIDL_UNDERSCORE);
        }

      shift = true;
      this->escaped_ = true;
      ACE_CString str (s);
      const char *c_prefix = "_cxx_";

      if (str.find ("_tc_") == 0
          || str.find ("_tao_") == 0)
        {
          shift = false;
        }
      else if (str.find (c_prefix) == 0)
        {
          str = str.substr (ACE_OS::strlen (c_prefix));
          const char *eh_suffix = "_excep";
          ACE_CString::size_type pos =
            str.length () - ACE_OS::strlen (eh_suffix);

          // If we have an AMI exception holder suffix, strip it off.
          if (str.find (eh_suffix) == pos)
            {
              str = str.substr (0, pos);
            }

          TAO_IDL_CPP_Keyword_Table cpp_key_tbl;
          unsigned int len =
            static_cast<unsigned int> (str.length ());
          const TAO_IDL_CPP_Keyword_Entry *entry =
            cpp_key_tbl.lookup (str.c_str (), len);

          if (entry != nullptr)
            {
              shift = false;
            }
        }
    }

  replace_string (shift ? s + 1 : s);
}

// Compare two Identifier *
bool
Identifier::compare (Identifier *o)
{
  if (!o ||
      !o->pv_string ||
      !this->pv_string ||
      this->escaped_ ^ o->escaped_)
    {
      return false;
    }

  return !ACE_OS::strcmp (this->pv_string, o->pv_string);
}

// Report the appropriate error if the two identifiers differ only in case.
bool
Identifier::case_compare (Identifier *o)
{
  return UTL_String::compare (this->pv_string, o->pv_string);
}

// Report no error if the two identifiers differ only in case.
bool
Identifier::case_compare_quiet (Identifier *o)
{
  return UTL_String::compare_quiet (this->pv_string, o->pv_string);
}

Identifier *
Identifier::copy ()
{
  Identifier *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  Identifier (this->pv_string),
                  nullptr);

  retval->escaped_ = this->escaped_;

  return retval;
}

bool
Identifier::escaped () const
{
  return this->escaped_;
}

// Dumping.
void
Identifier::dump (ACE_OSTREAM_TYPE &o)
{
  if (this->pv_string == nullptr)
    {
      return;
    }

  /*
   * Annotation ids are prefixed with '@' to effectively create an alternative
   * namespace for them. This hides that hack from dumping.
   */
  o << ((pv_string[0] == '@') ? pv_string + 1 : pv_string);
}

void
Identifier::destroy ()
{
}

bool
Identifier::operator== (const Identifier &other) const
{
  return !ACE_OS::strcmp (pv_string, other.get_string ());
}

Identifier &
Identifier::operator= (const Identifier &other)
{
  replace_string (other.get_string ());
  return *this;
}
