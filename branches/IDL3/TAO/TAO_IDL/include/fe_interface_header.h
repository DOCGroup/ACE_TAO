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

#include "TAO_IDL_FE_Export.h"
#include "utl_scoped_name.h"

class UTL_NameList;
class AST_Interface;
class AST_ValueType;

// Internal class for FE to describe interface headers
//
// FE_obv_header
// Internal class for FE to describe valuetype headers
//

class TAO_IDL_FE_Export FE_InterfaceHeader
{
public:
  FE_InterfaceHeader (UTL_ScopedName *n,
                      UTL_NameList *inherits,
                      idl_bool is_local,
                      idl_bool is_abstract,
                      idl_bool compile_now);

  virtual ~FE_InterfaceHeader (void);

  // Data Accessors
  UTL_ScopedName *name (void) const;
  AST_Interface **inherits (void) const;
  long n_inherits (void) const;
  AST_Interface **inherits_flat (void) const;
  long n_inherits_flat (void) const;

  idl_bool is_local (void) const;
  // See if we are a local interface.

  idl_bool is_abstract (void) const;
  // See if we are an abstract interface.

  // Data
protected:
  UTL_ScopedName *pd_interface_name;

  // Inherited interfaces.
  AST_Interface **pd_inherits;
  long pd_n_inherits;

  // Used for name clash checking.
  AST_Interface  **pd_inherits_flat;
  long pd_n_inherits_flat;

  idl_bool pd_is_local;
  idl_bool pd_is_abstract;

  // Operations

  // Compile the flattened unique list of interfaces which this
  // interface inherits from.
protected:
  void compile_inheritance (UTL_NameList *ifaces,
                            idl_bool for_valuetype);

  void compile_one_inheritance (AST_Interface *i);

  // Called from compile_inheritance().
  idl_bool check_inherit (AST_Interface *i,
                          idl_bool for_valuetype);
};


class FE_obv_header : public FE_InterfaceHeader
{
public:

  FE_obv_header (UTL_ScopedName *n, 
                 UTL_NameList *inherits, 
                 UTL_NameList *supports,
                 idl_bool truncatable);
  virtual ~FE_obv_header (void);

  // Data Accessors
  AST_Interface **supports (void) const;
  long n_supports (void) const;
  AST_ValueType *inherits_concrete (void) const;
  AST_Interface *supports_concrete (void) const;
  idl_bool truncatable (void) const;

private:
  // Supported interfaces.
  AST_Interface **pd_supports;
  long pd_n_supports;

  AST_ValueType *pd_inherits_concrete;
  AST_Interface *pd_supports_concrete;

  // Currently ignored.
  idl_bool pd_truncatable;

private:
  void compile_supports (UTL_NameList *supports);
  idl_bool check_concrete_supported_inheritance (AST_Interface *d);
};

#endif           // _FE_INTERFACE_HEADER_FE_INTERFACE_HH
