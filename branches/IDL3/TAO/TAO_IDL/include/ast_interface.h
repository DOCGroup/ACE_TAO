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

#ifndef _AST_INTERFACE_AST_INTERFACE_HH
#define _AST_INTERFACE_AST_INTERFACE_HH

// NOTE: add (AST_EnumValue *) is defined here because enums can
// be defined manifest locally; the constants defined in these
// enums are inserted in the enclosing scope.

#include "ast_type.h"
#include "utl_scope.h"

class TAO_IDL_FE_Export AST_Interface : public virtual AST_Type,
                                        public virtual UTL_Scope
{
public:
  AST_Interface (void);

  AST_Interface (UTL_ScopedName *n,
                 AST_Interface **ih,
                 long nih,
                 AST_Interface **ih_flat,
                 long nih_flat,
                 idl_bool local,
                 idl_bool abstract);

  virtual ~AST_Interface (void);

  // This serves for both interfaces and value types.
  static void fwd_redefinition_helper (AST_Interface *&i,
                                       UTL_Scope *s);

  // This serves only for interfaces, but it is called
  // from the corresponding AST_ValueType function().
  virtual void redefine (AST_Interface *from);

  AST_Interface **inherits (void) const;

  long n_inherits (void) const;

  AST_Interface **inherits_flat (void) const;

  long n_inherits_flat (void) const;

  void be_add_operation (AST_Operation *);

  void be_replace_operation (AST_Decl *old_op,
                             AST_Decl *new_op);

  // Is this interface defined? This predicate returns FALSE when an
  // interface was forward declared but not defined yet, and TRUE in
  // all other cases.
  idl_bool is_defined (void)
  {
    return (pd_n_inherits < 0) ? I_FALSE : I_TRUE;
  }

  // Check if any member's name clashes with a parent's
  // member's name, or if any parents' members' names
  // clash with each other.
  void inherited_name_clash (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.
  DEF_NARROW_METHODS2(AST_Interface, AST_Type, UTL_Scope);
  DEF_NARROW_FROM_DECL(AST_Interface);
  DEF_NARROW_FROM_SCOPE(AST_Interface);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

protected:
  // Data.

  // Immediate ancestors.
  AST_Interface **pd_inherits;
  long pd_n_inherits;

  // All ancestors.
  AST_Interface **pd_inherits_flat;
  long pd_n_inherits_flat;

  // Scope Management Protocol.
  friend int tao_yyparse (void);

  virtual AST_Constant *fe_add_constant (AST_Constant *c);

  virtual AST_Exception *fe_add_exception (AST_Exception *e);

  virtual AST_Attribute *fe_add_attribute(AST_Attribute *a);

  virtual AST_Field *fe_add_field (AST_Field *o);

  virtual AST_Operation *fe_add_operation(AST_Operation *o);

  virtual AST_Union *fe_add_union (AST_Union *u);

  virtual AST_Structure *fe_add_structure (AST_Structure *s);

  virtual AST_UnionFwd *fe_add_union_fwd (AST_UnionFwd *u);

  virtual AST_StructureFwd *fe_add_structure_fwd (AST_StructureFwd *s);

  virtual AST_Enum *fe_add_enum (AST_Enum *e);

  virtual AST_EnumVal *fe_add_enum_val (AST_EnumVal *v);

  virtual AST_Typedef *fe_add_typedef (AST_Typedef *t);

  virtual AST_Native *fe_add_native (AST_Native *n);
};

#endif           // _AST_INTERFACE_AST_INTERFACE_HH
