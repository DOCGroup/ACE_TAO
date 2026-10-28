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

Identifier::Identifier (std::string_view s)
{
  preprocess_and_replace_string (s);
}

Identifier::Identifier (Identifier const &other)
  : escaped_ (other.escaped ())
{
  this->string_ = other.string_;
}

// Operations.

char *
Identifier::get_string ()
{
  return this->string_ ? this->string_->data () : nullptr;
}

char const *
Identifier::get_string () const
{
  return this->string_ ? this->string_->c_str () : nullptr;
}

std::optional<std::string_view>
Identifier::get_string_view () const
{
  if (!this->string_)
    {
      return std::nullopt;
    }

  return *this->string_;
}

std::string_view
Identifier::get_string_view_or_empty () const
{
  return this->string_ ? std::string_view (*this->string_) : std::string_view {};
}

void
Identifier::replace_string (std::string_view s)
{
  this->string_.emplace (s);
}

void
Identifier::preprocess_and_replace_string (std::string_view s)
{
  bool shift = false;

  if (!s.empty () && s.front () == '_')
    {
      // Only one leading underscore is allowed.
      if (s.size () > 1 && s[1] == '_')
        {
          idl_global->err ()->error0 (UTL_Error::EIDL_UNDERSCORE);
        }

      shift = true;
      this->escaped_ = true;
      std::string str (s);
      std::string_view const c_prefix = "_cxx_";

      if (str.find ("_tc_") == 0
          || str.find ("_tao_") == 0)
        {
          shift = false;
        }
      else if (str.find (c_prefix) == 0)
        {
          str = str.substr (c_prefix.length ());
          std::string_view const eh_suffix = "_excep";
          std::string::size_type const pos =
            str.length () - eh_suffix.length ();

          // If we have an AMI exception holder suffix, strip it off.
          if (str.find (eh_suffix) == pos)
            {
              str = str.substr (0, pos);
            }

          TAO_IDL_CPP_Keyword_Table cpp_key_tbl;
          unsigned int len =
            static_cast<unsigned int> (str.length ());
          TAO_IDL_CPP_Keyword_Entry const *entry =
            cpp_key_tbl.lookup (str.c_str (), len);

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
  if (o == nullptr ||
      !o->string_ ||
      !this->string_ ||
      this->escaped_ ^ o->escaped_)
    {
      return false;
    }

  return *this->string_ == *o->string_;
}

// Report the appropriate error if the two identifiers differ only in case.
bool
Identifier::case_compare (Identifier const *o) const
{
  std::optional<std::string_view> const lhs = this->get_string_view ();
  std::optional<std::string_view> const rhs = o != nullptr ? o->get_string_view () : std::nullopt;
  return lhs && rhs && UTL_String::compare (*lhs, *rhs);
}

// Report no error if the two identifiers differ only in case.
bool
Identifier::case_compare_quiet (Identifier const *o) const
{
  std::optional<std::string_view> const lhs = this->get_string_view ();
  std::optional<std::string_view> const rhs = o != nullptr ? o->get_string_view () : std::nullopt;
  return lhs && rhs && UTL_String::compare_quiet (*lhs, *rhs);
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
  if (!this->string_)
    {
      return;
    }

  /*
   * Annotation ids are prefixed with '@' to effectively create an alternative
   * namespace for them. This hides that hack from dumping.
   */
  std::string_view name (*this->string_);
  if (!name.empty () && name.front () == '@')
    {
      name.remove_prefix (1);
    }
  o << name;
}

void
Identifier::destroy ()
{
}

bool
Identifier::operator== (Identifier const &other) const
{
  return this->string_ == other.string_;
}

Identifier &
Identifier::operator= (Identifier const &other)
{
  this->string_ = other.string_;
  return *this;
}
