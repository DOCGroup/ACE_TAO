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
#include "utl_err.h"
#include "ace/streams.h"

ACE_RCSID (util, 
           utl_string, 
           "$Id$")

UTL_String::UTL_String (void)
  : p_str (0),
    c_str (0),
    len (0)
{
}

UTL_String::UTL_String (const char *str)
{
  if (str == 0)
    {
      this->len = 0;
      this->p_str = 0;
      this->c_str = 0;
    }
  else
    {
      this->len = ACE_OS::strlen (str);
      this->p_str = ACE_OS::strdup (str);
      this->c_str = (char *) ACE_OS::malloc (this->len + 1);
      this->canonicalize ();
    }
}

UTL_String::UTL_String (UTL_String *s)
{
  if (s == 0)
    {
     this->p_str = 0;
     this->c_str = 0;
     this->len = 0;
    }
  else
    {
      char *b = s->get_string ();

      if (b == 0)
        {
          this->p_str = 0;
          this->c_str = 0;
          this->len = 0;
        }
      else
        {
          this->len = ACE_OS::strlen (b);
          this->p_str = ACE_OS::strdup (b);
          this->c_str = (char *) ACE_OS::malloc (this->len + 1);
          this->canonicalize ();
        }
    }
}

UTL_String::~UTL_String (void)
{
}

// Compute a canonical form for this string. This is (implemented as)
// a corresponding string with all upper case characters where the
// original has lower case characters, identical characters otherwise.
void
UTL_String::canonicalize (void)
{
  for (size_t i = 0; i < this->len; ++i)
    {
      if (isalpha (this->p_str[i]))
        {
          this->c_str[i] = (char) toupper (this->p_str[i]);
        }
      else
        {
          this->c_str[i] = this->p_str[i];
        }
    }

  c_str[this->len] = '\0';
}

// Compare two UTL_String *.
idl_bool
UTL_String::compare (UTL_String *s)
{
  char *s_c_str;
  long result;

  if (this->c_str == 0
      || s == 0
      || (s_c_str = s->get_canonical_rep ()) == 0)
    {
      result = I_FALSE;
    }
  else
    {
      result =
        (ACE_OS::strcmp (this->c_str, s_c_str) == 0) ? I_TRUE : I_FALSE;
    }

  // Check that the names are typed consistently.
  if (result == I_TRUE
      && ACE_OS::strcmp (this->p_str, s->get_string ()) != 0)
    {
      // Prevents redundant error reporting if we're in this branch.
      result = I_FALSE;

      if (idl_global->case_diff_error ())
        {
          idl_global->err ()->name_case_error (this->p_str,
                                               s->get_string ());
        }
      else
        {
          idl_global->err ()->name_case_warning (this->p_str,
                                                 s->get_string ());
        }
    }

  return result;
}

long
UTL_String::compare_quiet (UTL_String *s)
{
  char  *s_c_str;
  long  result;

  if (this->c_str == 0
      || s == 0
      || (s_c_str = s->get_canonical_rep ()) == 0)
    {
      result = I_FALSE;
    }
  else if (ACE_OS::strcmp (this->c_str, s_c_str) != 0)
    {
      result = I_FALSE;
    }
  else if (ACE_OS::strcmp (this->p_str, s->get_string ()) != 0)
    {
      result = I_TRUE;
    }
  else
    {
      result = I_FALSE;
    }

  return result;
}

void
UTL_String::destroy (void)
{
  if (this->p_str != 0)
    {
      ACE_OS::free (this->p_str);
      this->p_str = 0;
    }

  if (this->c_str != 0)
    {
      ACE_OS::free (this->c_str);
      this->c_str = 0;
    }
}

// Get the char * from a String.
char *
UTL_String::get_string (void)
{
  return this->p_str;
}

// Get the canonical representation from a String.
char *
UTL_String::get_canonical_rep (void)
{
  if (this->c_str == 0)
    {
      this->c_str = (char *) ACE_OS::malloc (this->len + 1);
      this->canonicalize ();
    }

  return this->c_str;
}

// AST Dumping.
void
UTL_String::dump (ACE_OSTREAM_TYPE &o)
{
  o << this->p_str;
}
