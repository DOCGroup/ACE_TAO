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

#ifndef _UTL_IDENTIFIER_UTL_IDENTIFIER_HH
#define _UTL_IDENTIFIER_UTL_IDENTIFIER_HH

#include "TAO_IDL_FE_Export.h"
#include "idl_bool.h"
#include "ace/OS.h"

// Needed to recognize ACE_OSTREAM_TYPE, and for BCB it's
// not included in ace/OS.h, so we include it here.
#if defined (__BORLANDC__)
# include "ace/streams.h"
#endif

class TAO_IDL_FE_Export Identifier
{
public:
  Identifier (void);
  // Default Constructor.

  Identifier (const char *s);
  // Constructor.

  virtual ~Identifier (void);
  // Destructor.

  // Operations

  char *get_string (void);
  // Get the underlying string.

  void replace_string (const char * s);
  // Replace the underlying string and free the old one.

  long compare (Identifier *other);
  // Compare with other Identifier.

  long case_compare (Identifier *other);
  // Also check for case-only difference.

  long case_compare_quiet (Identifier *other);
  // Like the above but suppressing error or warning I/O

  Identifier *copy (void);
  // Create a deep copy.

  idl_bool escaped (void) const;
  // Accessor for the member.

  virtual void dump (ACE_OSTREAM_TYPE &o);
  // Dump to an ostream.

  virtual void destroy (void);
  // Cleanup function.

private:
    // Storage for data.
    char *pv_string;

    // Did the IDL string have a leading underscore?
    idl_bool escaped_;
};

#endif          // _UTL_IDENTIFIER_UTL_IDENTIFIER_HH
