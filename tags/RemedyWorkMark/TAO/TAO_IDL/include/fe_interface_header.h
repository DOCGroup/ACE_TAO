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

#ifndef _FE_INTERFACE_HEADER_FE_INTERFACE_HH
#define _FE_INTERFACE_HEADER_FE_INTERFACE_HH

#include "utl_scoped_name.h"

#include "ast_typedef.h"

class UTL_NameList;
class AST_Interface;

// FE_InterfaceHeader
// Internal class for FE to describe interface headers

class TAO_IDL_FE_Export FE_InterfaceHeader
{
public:
  FE_InterfaceHeader (UTL_ScopedName *n,
                      UTL_NameList *inherits,
                      bool is_local,
                      bool is_abstract,
                      bool compile_now);

  virtual ~FE_InterfaceHeader (void);

  // Data Accessors.
  UTL_ScopedName *name (void) const;
  AST_Type **inherits (void) const;
  long n_inherits (void) const;
  AST_Interface **inherits_flat (void) const;
  long n_inherits_flat (void) const;

  bool is_local (void) const;
  // See if we are a local interface.

  bool is_abstract (void) const;
  // See if we are an abstract interface.

  virtual void destroy (void);
  // Destroy anything allocated for this class.

  // Operations.

  // Compile the flattened unique list of interfaces which this
  // interface inherits from.
protected:
  void compile_inheritance (UTL_NameList *ifaces,
                            bool for_valuetype);

  void compile_one_inheritance (AST_Type *i);

  // Called from compile_inheritance().
  int check_inherit (AST_Interface *i,
                     bool for_valuetype);

  void add_inheritance (AST_Type *i);
  void add_inheritance_flat (AST_Interface *i);
  bool already_seen (AST_Type *ip);
  bool already_seen_flat (AST_Interface *ip);

  void install_in_header (void);

  void destroy_flat_arrays (void);

  // Data.
protected:
  UTL_ScopedName *interface_name_;
  bool has_template_parent_;

  // Inherited interfaces.
  AST_Type **inherits_;
  long n_inherits_;

  // Used for name clash checking.
  AST_Interface  **inherits_flat_;
  long n_inherits_flat_;

  bool is_local_;
  bool is_abstract_;

  AST_Type **iseen_;
  AST_Interface **iseen_flat_;
  long iallocated_;
  long iused_;
  long iallocated_flat_;
  long iused_flat_;
};

#endif           // _FE_INTERFACE_HEADER_FE_INTERFACE_HH
