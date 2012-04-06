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

#ifndef _STRING_STRING_HH
#define _STRING_STRING_HH

#include "TAO_IDL_FE_Export.h"

#include "ace/iosfwd.h"
  /* Basic_Types.h are needed in QNX for size_t type. */
#include "ace/Basic_Types.h"
#include "ace/SStringfwd.h"

class TAO_IDL_FE_Export UTL_String
{
  // =TITLE
  //  UTL_String
  // =DESCRIPTION
  //  string class to store identifiers
public:
  UTL_String (void);

  UTL_String (const char *str, bool take_copy= false);

  UTL_String (UTL_String *s, bool force_copy= false);

  virtual ~UTL_String (void);

  virtual void destroy (void);
  // Cleanup function.

  virtual void dump (ACE_OSTREAM_TYPE &o);
  // Dump to the ostream.

  char *get_string (void) {return this->p_str;}
  // Get contents of utl_string.

  char *get_canonical_rep (void);
  // Get canonical representation. This is (implemented as) the all upper
  // case corresponding string.

  bool compare (UTL_String *s);
  // Compare two UTL_String *

  bool compare_quiet (UTL_String *s);
  // Like the above but without error or warning message output.

  static bool strcmp_caseless (
    const char *lhs,
    const char *rhs,
    bool &mixed_case);
  // Caseless string compare returns true if match, mixed_case is
  // true if match only true due to ignoring case differences.

  static bool compare (const char *lhs, const char *rhs);
  // Compare two const char * (like the UTL_String compare)

  static bool compare_quiet (const char *lhs, const char *rhs);
  // Like the above but without error or warning message output.

  static void get_canonical_rep (const char *src, char *dest);
  // Get canonical representation. This is (implemented as) the all upper
  // case corresponding string.

  static void get_canonical_rep (ACE_CString &cstr);
  // Get canonical representation. This is (implemented as) the all upper
  // case corresponding string.

private:
  // Data
  bool copy_taken;
  // If delete needs to be called on destroy/delete

  char *p_str;
  // Storage for characters.

  char *c_str;
  // Storage for canonicalized string.
};

#endif           // _STRING_STRING_HH
