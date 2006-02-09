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

class UTL_NameList;
class AST_Interface;
class AST_ValueType;
class AST_Component;
class AST_Home;

// FE_interfade_header
// Internal class for FE to describe interface headers
//
// FE_obv_header
// Internal class for FE to describe valuetype headers.
//
// FE_component_header
// Internal class for FE to describe component headers.

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
  AST_Interface **inherits (void) const;
  long n_inherits (void) const;
  AST_Interface **inherits_flat (void) const;
  long n_inherits_flat (void) const;

  bool is_local (void) const;
  // See if we are a local interface.

  bool is_abstract (void) const;
  // See if we are an abstract interface.

  void destroy (void);
  // Destroy anything allocated for this class.

  // Data.
protected:
  UTL_ScopedName *pd_interface_name;

  // Inherited interfaces.
  AST_Interface **pd_inherits;
  long pd_n_inherits;

  // Used for name clash checking.
  AST_Interface  **pd_inherits_flat;
  long pd_n_inherits_flat;

  bool pd_is_local;
  bool pd_is_abstract;

  // Operations.

  // Compile the flattened unique list of interfaces which this
  // interface inherits from.
protected:
  void compile_inheritance (UTL_NameList *ifaces,
                            bool for_valuetype);

  void compile_one_inheritance (AST_Interface *i);

  // Called from compile_inheritance().
  int check_inherit (AST_Interface *i,
                     bool for_valuetype);
};

class TAO_IDL_FE_Export FE_OBVHeader : public FE_InterfaceHeader
{
public:

  FE_OBVHeader (UTL_ScopedName *n,
                UTL_NameList *inherits,
                UTL_NameList *supports,
                bool truncatable,
                bool is_eventtype = false);
  virtual ~FE_OBVHeader (void);

  // Data Accessors.
  AST_Interface **supports (void) const;
  long n_supports (void) const;
  AST_ValueType *inherits_concrete (void) const;
  AST_Interface *supports_concrete (void) const;
  bool truncatable (void) const;

protected:
  // Supported interfaces.
  AST_Interface **pd_supports;
  long pd_n_supports;

  AST_ValueType *pd_inherits_concrete;
  AST_Interface *pd_supports_concrete;

  // Currently ignored.
  bool pd_truncatable;

protected:
  void compile_inheritance (UTL_NameList *vtypes,
                            bool is_eventtype);
  void compile_supports (UTL_NameList *supports);
  bool check_concrete_supported_inheritance (AST_Interface *d);
};

class TAO_IDL_FE_Export FE_EventHeader : public FE_OBVHeader
{
public:

  FE_EventHeader (UTL_ScopedName *n,
                  UTL_NameList *inherits,
                  UTL_NameList *supports,
                  bool truncatable);
  virtual ~FE_EventHeader (void);
};

// Unlike value types, a component's supported interfaces are simply
// added to the inheritance list in generated code, so we use the
// existing base class mechanism for managing the inheritance list
// to manage the derived class's supported interface list.
class TAO_IDL_FE_Export FE_ComponentHeader : public FE_InterfaceHeader
{
public:

  FE_ComponentHeader (UTL_ScopedName *n,
                      UTL_ScopedName *base_component,
                      UTL_NameList *supports,
                      bool compile_now);
  virtual ~FE_ComponentHeader (void);

  // Data Accessors.
  AST_Component *base_component (void) const;
  AST_Interface **supports (void) const;
  long n_supports (void) const;
  AST_Interface **supports_flat (void) const;
  long n_supports_flat (void) const;

protected:
  void compile_inheritance (UTL_ScopedName *base_component);
  void compile_supports (UTL_NameList *supports);

protected:
  AST_Component *pd_base_component;
};

// We use the 'base_component' member of the base class to
// store the 'managed_component' member of the derived class.
// By inheriting from FE_ComponentHeader, we also get the
// reuse of the mechanism described in the comment above
// for handling the supported interface list.
class TAO_IDL_FE_Export FE_HomeHeader : public FE_ComponentHeader
{
public:

  FE_HomeHeader (UTL_ScopedName *n,
                 UTL_ScopedName *base_home,
                 UTL_NameList *supports,
                 UTL_ScopedName *managed_component,
                 UTL_ScopedName *primary_key);
  virtual ~FE_HomeHeader (void);

  // Data Accessors.
  AST_Home *base_home (void) const;
  AST_Component *managed_component (void) const;
  AST_ValueType *primary_key (void) const;

protected:
  AST_Home *pd_base_home;
  AST_Component *pd_managed_component;
  AST_ValueType *pd_primary_key;

protected:
  void compile_inheritance (UTL_ScopedName *base_home);
  void compile_managed_component (UTL_ScopedName *managed_compoent);
  void compile_primary_key (UTL_ScopedName *primary_key);
};

#endif           // _FE_INTERFACE_HEADER_FE_INTERFACE_HH
