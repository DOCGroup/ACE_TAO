// This may look like C, but it's really -*- C++ -*-
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

#ifndef _AST_MODULE_AST_MODULE_HH
#define _AST_MODULE_AST_MODULE_HH

#include "ast_decl.h"
#include "utl_scope.h"
#include "ace/Unbounded_Set.h"

// Representation of module.
//
// NOTE: add(AST_EnumValue *) is defined here because enums can
// be defined manifest locally; the constants defined in these
// enums are inserted in the enclosing scope.

class TAO_IDL_FE_Export AST_Module : public virtual AST_Decl,
                                     public virtual UTL_Scope
{
public:
  // Operations.

  // Constructor(s) and destructor.
  AST_Module (void);

  AST_Module (UTL_ScopedName *n);

  virtual ~AST_Module (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL(AST_Module);
  DEF_NARROW_FROM_SCOPE(AST_Module);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Involved in OBV_ namespace generation.
  void set_has_nested_valuetype (void);
  bool has_nested_valuetype (void);

  // Allows adding an interface to a later point
  // The interface i is inserted after interface ix, if
  // ix is not null.
  int be_add_interface (AST_Interface *i,
                        AST_Interface *ix = 0);

  // Has this node been referenced here already?
  // Override of method in UTL_Scope.
  virtual bool referenced (AST_Decl *e,
                               Identifier *id = 0);

  // Add decls from previous opening of this module to the
  // 'previous' set of this module, along with the argument's
  // own 'previous' set.
  void add_to_previous (AST_Module *m);

  // Called by lookup_by_name_local, to check for declaration
  // in a previous opening of this module.
  virtual AST_Decl *look_in_previous (Identifier *e, bool ignore_fwd = false);

  // Accessor to the member.
  ACE_Unbounded_Set<AST_Decl *> &previous (void);

  // Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

private:
  friend void fe_populate_global_scope (AST_Module *m);
  friend int tao_yyparse (void);

  // Scope Management Protocol

  virtual AST_PredefinedType *fe_add_predefined_type (AST_PredefinedType *t);

  virtual AST_Module *fe_add_module (AST_Module *m);

  virtual AST_Interface *fe_add_interface (AST_Interface *i);

  virtual AST_InterfaceFwd *fe_add_interface_fwd (AST_InterfaceFwd *i);

  virtual AST_ValueType *fe_add_valuetype (AST_ValueType *i);

  virtual AST_ValueTypeFwd *fe_add_valuetype_fwd (AST_ValueTypeFwd *i);

  virtual AST_EventType *fe_add_eventtype (AST_EventType *i);

  virtual AST_EventTypeFwd *fe_add_eventtype_fwd (AST_EventTypeFwd *i);

  virtual AST_Component *fe_add_component (AST_Component *i);

  virtual AST_ComponentFwd *fe_add_component_fwd (AST_ComponentFwd *i);

  virtual AST_Home *fe_add_home (AST_Home *i);

  virtual AST_Constant *fe_add_constant (AST_Constant *c);

  virtual AST_Exception *fe_add_exception (AST_Exception *e);

  virtual AST_Union *fe_add_union (AST_Union *u);

  virtual AST_UnionFwd *fe_add_union_fwd (AST_UnionFwd *u);

  virtual AST_Structure *fe_add_structure (AST_Structure *s);

  virtual AST_StructureFwd *fe_add_structure_fwd (AST_StructureFwd *s);

  virtual AST_Enum *fe_add_enum (AST_Enum *e);

  virtual AST_EnumVal *fe_add_enum_val (AST_EnumVal *v);

  virtual AST_Typedef *fe_add_typedef (AST_Typedef *t);

  virtual AST_Native *fe_add_native (AST_Native *n);

  virtual AST_ValueBox *fe_add_valuebox (AST_ValueBox *vb);

  bool pd_has_nested_valuetype;

  ACE_Unbounded_Set<AST_Decl *> previous_;
  // Container for declaration from previous openings of this module.
};

#endif           // _AST_MODULE_AST_MODULE_HH
