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

#include "utl_identifier.h"
#include "global_extern.h"
#include "utl_err.h"
#include "utl_string.h"

ACE_RCSID (util, 
           utl_identifier, 
           "$Id$")

Identifier::Identifier (void)
  : pv_string (0),
    escaped_ (0)
{
}

Identifier::Identifier (const char *s)
{
  idl_bool shift = 0;
  this->escaped_ = 0;

  if (*s == '_')
    {
      // Only one leading underscore is allowed.
      if (s[1] == '_')
        {
          idl_global->err ()->error0 (UTL_Error::EIDL_UNDERSCORE);
        }

      shift = 1;
      this->escaped_ = 1;

      ACE_CString str (s,
                       0,
                       0);

      if (str.find ("_cxx_") == 0
          || str.find ("_tc_") == 0
          || str.find ("_tao_") == 0)
        {
          shift = 0;
        }
    }

  if (shift)
    {
      this->pv_string = ACE::strnew (s + 1);
    }
  else
    {
      this->pv_string = ACE::strnew (s);
    }
}

Identifier::~Identifier (void)
{
  if (this->pv_string != 0)
    {
      ACE::strdelete (this->pv_string);
      this->pv_string = 0;
    }
}

// Operations.

char *
Identifier::get_string (void)
{
  return this->pv_string;
}

void
Identifier::replace_string (const char * s)
{
  if (this->pv_string != 0)
    {
      delete [] this->pv_string;
    }

  this->pv_string = ACE::strnew (s);
}

// Compare two Identifier *
long
Identifier::compare (Identifier *o)
{
  if (o == 0)
    {
      return I_FALSE;
    };

  if (this->pv_string == 0 || o->get_string () == 0)
    {
      return I_FALSE;
    }

  if ((this->escaped_ ^ o->escaped_) == 1)
    {
      return I_FALSE;
    }

  return (ACE_OS::strcmp (this->pv_string, o->get_string ()) == 0);
}

// Report the appropriate error if the two identifiers differ only in case.
long
Identifier::case_compare (Identifier *o)
{
  UTL_String member (this->pv_string);
  UTL_String other (o->get_string ());

  long result = member.compare (&other);

  member.destroy ();
  other.destroy ();

  return result;
}

// Report no error if the two identifiers differ only in case.
long
Identifier::case_compare_quiet (Identifier *o)
{
  UTL_String member (this->pv_string);
  UTL_String other (o->pv_string);

  long result = member.compare_quiet (&other);

  member.destroy ();
  other.destroy ();

  return result;
}

Identifier *
Identifier::copy (void)
{
  Identifier *retval = 0;
  ACE_NEW_RETURN (retval,
                  Identifier (this->pv_string),
                  0);

  retval->escaped_ = this->escaped_;

  return retval;
}

idl_bool
Identifier::escaped (void) const
{
  return this->escaped_;
}

// Dumping.
void
Identifier::dump (ACE_OSTREAM_TYPE &o)
{
  if (this->pv_string == 0)
    {
      return;
    }

  o << this->pv_string;
}

void
Identifier::destroy (void)
{
}
