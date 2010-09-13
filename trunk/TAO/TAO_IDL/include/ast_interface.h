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
  AST_Interface (UTL_ScopedName *n,
                 AST_Type **ih,
                 long nih,
                 AST_Interface **ih_flat,
                 long nih_flat,
                 bool local,
                 bool abstract);

  virtual ~AST_Interface (void);

  // This serves for interfaces, value types, components, and eventtypes.
  static void fwd_redefinition_helper (AST_Interface *&i,
                                       UTL_Scope *s);

  // Overridden for valuetypes, components, and eventtypes.
  virtual void redefine (AST_Interface *from);

  AST_Type **inherits (void) const;

  long n_inherits (void) const;

  AST_Interface **inherits_flat (void) const;

  long n_inherits_flat (void) const;

  ACE_Unbounded_Queue<AST_Type *> &get_insert_queue (void);
  ACE_Unbounded_Queue<AST_Type *> &get_del_queue (void);

  AST_Operation *be_add_operation (AST_Operation *);

  void be_replace_operation (AST_Decl *old_op,
                             AST_Decl *new_op);

  // Is this interface defined? This predicate returns FALSE when an
  // interface was forward declared but not defined yet, and TRUE in
  // all other cases.
  bool is_defined (void);

  // Check if we have redefined any of our parents' operations or attributes,
  // and check if there is such a clash among the parents
  bool redef_clash (void);

  // Accessors for the member.
  bool home_equiv (void) const;
  void home_equiv (bool val);

  // Accessors for the member.
  AST_InterfaceFwd *fwd_decl (void) const;
  void fwd_decl (AST_InterfaceFwd *node);

  // Look through inherited interfaces.
  virtual AST_Decl *look_in_inherited (UTL_ScopedName *e,
                                       bool full_def_only);

  AST_Decl *look_in_inherited_local (Identifier *e,
                                     bool full_def_only = false);

  /// Do we have both abstract and concrete parents?
  int has_mixed_parentage (void);

  /// Compute whether or not we have both abstract and concrete parents,
  /// and make a list of the abstract parents, if any.
  void analyze_parentage (void);

  // Recursively called on valuetype to check for legal use as
  // a primary key. Overridden for valuetype, struct, sequence,
  // union, array, typedef, and interface.
  virtual bool legal_for_primary_key (void) const;

  virtual AST_Decl *special_lookup (UTL_ScopedName *e,
                                    bool full_def_only);

  /// Accessors for the members.
  AST_Interface *ami_handler (void) const;
  void ami_handler (AST_Interface *handler);
  AST_Interface *ami4ccm_uses (void) const;
  void ami4ccm_uses (AST_Interface *implied);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL(AST_Interface);
  DEF_NARROW_FROM_SCOPE(AST_Interface);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  static AST_Decl::NodeType const NT;
  typedef AST_InterfaceFwd FWD_TYPE;

protected:
  // Data.

  // Immediate ancestors.
  AST_Type **pd_inherits;
  long pd_n_inherits;

  // All ancestors.
  AST_Interface **pd_inherits_flat;
  long pd_n_inherits_flat;

  // Queue data structure needed for breadth-first traversal of
  // inheritance tree.
  ACE_Unbounded_Queue<AST_Type *> insert_queue;

  // For a special case of a deeply nested inheritance graph and one specific
  // way of inheritance in which a node that was already visited,
  // but is not present in
  // the queue, gets inserted at the tail. This situation arises when a node
  // multiply inherits from two or more interfaces in which the first parent is
  // higher up in the tree than the second parent. In addition, if the second
  // parent turns out to be a child of the first .

  // Queue of dequeued nodes to be searched for the above case.
  ACE_Unbounded_Queue<AST_Type *> del_queue;

  // Are we the equivalent interface of a home?
  bool home_equiv_;

  // The forward declaration we may have been created from.
  AST_InterfaceFwd *fwd_decl_;

  // Must keep these base interface placeholders in a separate
  // container, so they can be destroyed. Iterating over
  // pd_inherits won't work because the real interfaces will
  // probably already be destroyed and the pointers will be
  // garbage.
  ACE_Unbounded_Queue<AST_Type *> param_holders_;

  /// Do we have both abstract and concrete parents?
  int has_mixed_parentage_;

  /// Store here for quick retrieval without an AST lookup.
  AST_Interface *ami_handler_;
  AST_Interface *ami4ccm_uses_;

public:
  // Scope Management Protocol.

  virtual AST_Constant *fe_add_constant (AST_Constant *c);

  virtual AST_Exception *fe_add_exception (AST_Exception *e);

  virtual AST_Attribute *fe_add_attribute (AST_Attribute *a);

  virtual AST_Operation *fe_add_operation (AST_Operation *o);

  virtual AST_Union *fe_add_union (AST_Union *u);

  virtual AST_Structure *fe_add_structure (AST_Structure *s);

  virtual AST_UnionFwd *fe_add_union_fwd (AST_UnionFwd *u);

  virtual AST_StructureFwd *fe_add_structure_fwd (AST_StructureFwd *s);

  virtual AST_Enum *fe_add_enum (AST_Enum *e);

  virtual AST_EnumVal *fe_add_enum_val (AST_EnumVal *v);

  virtual AST_Typedef *fe_add_typedef (AST_Typedef *t);

  virtual AST_Native *fe_add_native (AST_Native *n);

protected:
  // Lookup based on the local name, override of UTL_Scope definition.
  // This version checks for redefinitions of attributes or operations.
  AST_Decl *lookup_for_add (AST_Decl *d);

  void redef_clash_populate_r (AST_Type *t);
  // Populate the insert queue with our parents, and, if we are a
  // valuetype, with our supported interface and our parents'
  // supported interfaces.

  int insert_non_dup (AST_Type *t,
                      bool abstract_paths_only = false);
  // Do non-duplicating insert of bi, by searching both the
  // insert queue and the delete queue.
};

#endif           // _AST_INTERFACE_AST_INTERFACE_HH
