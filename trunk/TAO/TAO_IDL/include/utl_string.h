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

// utl_string.hh - contains a quick and dirty TEMPORARY string implementation

/*
** DEPENDENCIES: NONE
**
** USE: Included from util.hh
*/

#ifndef String
#define String	UTL_String
#endif	// String

class	UTL_String
{
  // =TITLE
  //  UTL_String
  // =DESCRIPTION
  //  string class to store identifiers
public:
  // =Operations

  UTL_String (void);
  // default constructor

  UTL_String (char *str);
  // construct from a char *

  UTL_String (unsigned long maxlen);
  // constructor with an initial string length

  UTL_String (UTL_String *s);
  // construct using a pointer to UTL_String

  virtual ~UTL_String (void) {}
  // destructor 

  // =AST Dumping

  virtual void dump (ostream &o);
  // dump to the ostream

  // =Other Operations

  char *get_string (void);
  // Get contents of utl_string

  char *get_canonical_rep (void);
  // Get canonical representation. This is (implemented as) the all upper
  // case corresponding string

  virtual long compare (UTL_String *s);
  // Compare two String *

private:
  // Data
  char		*p_str;		
  // Storage for characters

  char		*c_str;		
  // Canonicalized string

  unsigned long	len;		
  // How long is string

  unsigned long alloced;	
  // How much allocated

  // =Private helper operations

  void canonicalize (void);	
  // Compute canonical representation
};

#endif           // _STRING_STRING_HH
