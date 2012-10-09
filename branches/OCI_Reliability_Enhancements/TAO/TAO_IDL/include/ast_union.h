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

#ifndef _AST_UNION_AST_UNION_HH
#define _AST_UNION_AST_UNION_HH

#include "ast_structure.h"

// NOTE: add (AST_ConcreteType *) is defined here because a union
// can contain locally defined types in addition to fields.
//
// NOTE: add (AST_EnumValue *) is defined here because enums can
// be defined manifest locally; the constants defined in these
// enums are inserted in the enclosing scope. It is unlikely that
// a BE writer will need to overload this function in AST_Union.

class TAO_IDL_FE_Export AST_Union : public virtual AST_Structure
{
public:
  AST_Union (AST_ConcreteType *disc_type,
             UTL_ScopedName *n,
             bool local,
             bool abstract);

  virtual ~AST_Union (void);

  // This also calls the base class version.
  virtual void redefine (AST_Structure *from);

  virtual bool in_recursion (ACE_Unbounded_Queue<AST_Type *> &list);
  // Are we or the parameter node involved in some kind of recursion?

  // Data Accessors.

  AST_ConcreteType *disc_type (void);

  AST_Expression::ExprType udisc_type (void);

  // Narrowing.
  DEF_NARROW_FROM_DECL(AST_Union);
  DEF_NARROW_FROM_SCOPE(AST_Union);

  struct DefaultValue
  {
    union PermittedTypes
    {
      ACE_CDR::Char char_val;
      ACE_CDR::WChar wchar_val;
      ACE_CDR::Boolean bool_val;
      ACE_CDR::Short short_val;
      ACE_CDR::UShort ushort_val;
      ACE_CDR::Long long_val;
      ACE_CDR::ULong ulong_val;
      ACE_CDR::ULong enum_val;
      ACE_CDR::LongLong longlong_val;
      ACE_CDR::ULongLong ulonglong_val;
    } u;
    long computed_;
    // computed == -1 => error condition
    //          == 0 => does not exist because all cases have been covered
    //          == 1 => already computed
    //          == -2 => initial value
  };

  int default_value (DefaultValue &);
  // Get the default value.

  virtual int default_index (void);
  // Return the default index used.

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  static AST_Decl::NodeType const NT;

protected:
  virtual int compute_size_type (void);
  // Compute the size type if it is unknown.

  virtual AST_UnionBranch *fe_add_union_branch (AST_UnionBranch *b);
  // Moved out of private section so it can be called from subclass.

private:
  // Data.

  AST_ConcreteType *pd_disc_type;
  // Discriminator type.

  AST_Expression::ExprType pd_udisc_type;
  // Its expression type.
  // Convention: udisc_type == EV_enum denotes an enum value.

  // Operations.

  // Look up a branch by node pointer.
  AST_UnionBranch *lookup_branch (AST_UnionBranch *branch);

  // Look up the branch with the "default" label.
  AST_UnionBranch *lookup_default (void);

  // Look up a branch given a branch with a label. This is used to
  // check for duplicate labels.
  AST_UnionBranch *lookup_label (AST_UnionBranch *b);

  // Look up a union branch given an enumerator. This is used to
  // check for duplicate enum labels.
  AST_UnionBranch *lookup_enum (AST_UnionBranch *b);

  friend int tao_yyparse (void);
  friend class ast_visitor_tmpl_module_inst;

  // Scope Management Protocol.

  virtual AST_Union *fe_add_union (AST_Union *u);

  virtual AST_Structure *fe_add_structure (AST_Structure *s);

  virtual AST_Enum *fe_add_enum (AST_Enum *e);

  virtual AST_EnumVal *fe_add_enum_val (AST_EnumVal *v);

  virtual int compute_default_value (void);
  // Compute the default value (if any).

  int compute_default_index (void);
  // Count the default index.

  DefaultValue default_value_;
  // Default value (if any).

  int default_index_;
  // Default label index (zero based indexing).
};

#endif           // _AST_UNION_AST_UNION_HH
