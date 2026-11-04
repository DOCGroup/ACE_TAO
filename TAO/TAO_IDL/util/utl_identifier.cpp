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

Identifier::Identifier (std::string const& s)
{
  preprocess_and_replace_string (s);
}

Identifier::Identifier (Identifier const &) = default;

Identifier::~Identifier () = default;

// Operations.

std::string const&
Identifier::get_string () const
{
  return this->pv_string_;
}

void
Identifier::replace_string (std::string const& s)
{
  this->pv_string_ = s;
}

void
Identifier::preprocess_and_replace_string (std::string const& s)
{
  bool shift = false;

  if (!s.empty () && s[0] == '_')
    {
      // Only one leading underscore is allowed.
      if (s.size () > 1 && s[1] == '_')
        {
          idl_global->err ()->error0 (UTL_Error::EIDL_UNDERSCORE);
        }

      shift = true;
      this->escaped_ = true;
      static constexpr char cxx_prefix[] = "_cxx_";
      static constexpr char eh_suffix[] = "_excep";

      if (s.compare (0, 4, "_tc_") == 0
          || s.compare (0, 5, "_tao_") == 0)
        {
          shift = false;
        }
      else if (s.compare (0, sizeof (cxx_prefix) - 1, cxx_prefix) == 0)
        {
          std::string::size_type const keyword_offset = sizeof (cxx_prefix) - 1;
          std::string::size_type keyword_length = s.size () - keyword_offset;
          std::string::size_type const suffix_length = sizeof (eh_suffix) - 1;

          // If we have an AMI exception holder suffix, strip it off.
          if (keyword_length >= suffix_length
              && s.compare (s.size () - suffix_length, suffix_length,
                            eh_suffix) == 0)
            {
              keyword_length -= suffix_length;
            }

          TAO_IDL_CPP_Keyword_Table cpp_key_tbl;
          unsigned int len =
            static_cast<unsigned int> (keyword_length);
          TAO_IDL_CPP_Keyword_Entry const *entry =
            cpp_key_tbl.lookup (s.c_str () + keyword_offset, len);

          if (entry != nullptr)
            {
              shift = false;
            }
        }
    }

  replace_string (shift ? s.substr (1) : s);
}

// Compare two Identifier *
bool
Identifier::compare (Identifier const *o) const
{
  if (o == nullptr || this->escaped_ ^ o->escaped_)
    {
      return false;
    }

  return this->pv_string_ == o->pv_string_;
}

// Report the appropriate error if the two identifiers differ only in case.
bool
Identifier::case_compare (Identifier const *o) const
{
  if (o == nullptr)
    {
      return false;
    }

  return UTL_String::compare (this->get_string (), o->get_string ());
}

// Report no error if the two identifiers differ only in case.
bool
Identifier::case_compare_quiet (Identifier const *o) const
{
  if (o == nullptr)
    {
      return false;
    }

  return UTL_String::compare_quiet (this->get_string (), o->get_string ());
}

Identifier *
Identifier::copy () const
{
  Identifier *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  Identifier (*this),
                  nullptr);

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
  /*
   * Annotation ids are prefixed with '@' to effectively create an alternative
   * namespace for them. This hides that hack from dumping.
   */
  if (!this->pv_string_.empty () && this->pv_string_[0] == '@')
    {
      o << this->pv_string_.substr (1);
    }
  else
    {
      o << this->pv_string_;
    }
}

bool
Identifier::operator== (Identifier const &other) const
{
  return this->pv_string_ == other.pv_string_;
}

Identifier &
Identifier::operator= (Identifier const &other)
{
  this->pv_string_ = other.pv_string_;
  return *this;
}
