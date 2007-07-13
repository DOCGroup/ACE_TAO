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

#ifndef _UTL_SCOPE_UTL_SCOPE_HH
#define _UTL_SCOPE_UTL_SCOPE_HH

// This is the base class for name scoping. Each entity which can
// be directly contained in a naming scope has an add() function
// defined for it. The functions defined on the base scoping class
// all return FALSE. Each subclass of UTL_Scope should
// define its own add() functions for those types which are legal
// in that context. Then, for those types for which no local functions
// have been defined the function on the base class will be called to
// return FALSE.
//
// The "AST_" classes may redefine add() functions to implement any
// name binding mechanisms needed for their own functionality. It is
// the responsibility of those functions then to call the add()
// function defined in the parent "AST_" class.

#include "ast_decl.h"
#include "ast_expression.h"
#include "ast_typedef.h"
#include "utl_scoped_name.h"

// This is for AIX w/IBM C++.
class Identifier;

class AST_PredefinedType;
class AST_Module;
class AST_Interface;
class AST_InterfaceFwd;
class AST_ValueBox;
class AST_ValueType;
class AST_ValueTypeFwd;
class AST_EventType;
class AST_EventTypeFwd;
class AST_Component;
class AST_ComponentFwd;
class AST_Home;
class AST_Constant;
class AST_Exception;
class AST_Attribute;
class AST_Operation;
class AST_Argument;
class AST_Union;
class AST_UnionFwd;
class AST_UnionBranch;
class AST_Structure;
class AST_StructureFwd;
class AST_Field;
class AST_Enum;
class AST_EnumVal;
class AST_Sequence;
class AST_String;
class AST_Array;
class AST_Native;
class AST_Factory;
class UTL_StrList;
class UTL_NameList;

// Forward declaration of active iterator for UTL_Scope.
class UTL_ScopeActiveIterator;

class TAO_IDL_FE_Export UTL_Scope : public virtual COMMON_Base
{
public:
  // Enum to denote the kind of iteration desired.
  enum ScopeIterationKind {
      IK_both           // Iterate through both decls and local types.
    , IK_decls          // Iterate only through decls.
    , IK_localtypes     // Iterate only through local types.
  };

  // Operations.

  // Constructor(s).
  UTL_Scope (void);
  UTL_Scope (AST_Decl::NodeType nt);

  // Destructor.
  virtual ~UTL_Scope (void);

  // Scope Management Protocol.
  virtual AST_PredefinedType *add_predefined_type (AST_PredefinedType *t);

  virtual AST_Module *add_module (AST_Module *m);

  virtual AST_Interface *add_interface (AST_Interface *i);

  virtual AST_InterfaceFwd *add_interface_fwd (AST_InterfaceFwd *i);

  virtual AST_ValueType *add_valuetype (AST_ValueType *i);

  virtual AST_ValueTypeFwd *add_valuetype_fwd (AST_ValueTypeFwd *i);

  virtual AST_EventType *add_eventtype (AST_EventType *i);

  virtual AST_EventTypeFwd *add_eventtype_fwd (AST_EventTypeFwd *i);

  virtual AST_Component *add_component (AST_Component *i);

  virtual AST_ComponentFwd *add_component_fwd (AST_ComponentFwd *i);

  virtual AST_Home *add_home (AST_Home *i);

  virtual AST_Constant *add_constant (AST_Constant *c);

  virtual AST_Exception *add_exception (AST_Exception *e);

  virtual AST_Attribute *add_attribute (AST_Attribute *a);

  virtual AST_Operation *add_operation (AST_Operation *o);

  virtual AST_Argument *add_argument (AST_Argument *a);

  virtual AST_Union *add_union (AST_Union *u);

  virtual AST_UnionFwd *add_union_fwd (AST_UnionFwd *u);

  virtual AST_UnionBranch *add_union_branch (AST_UnionBranch *b);

  virtual AST_Structure *add_structure (AST_Structure *s);

  virtual AST_StructureFwd *add_structure_fwd (AST_StructureFwd *s);

  virtual AST_Field *add_field (AST_Field *f);

  virtual AST_Enum *add_enum (AST_Enum *e);

  virtual AST_EnumVal *add_enum_val (AST_EnumVal *v);

  virtual AST_Typedef *add_typedef (AST_Typedef *t);

  virtual UTL_StrList *add_context (UTL_StrList *c);

  virtual UTL_NameList *add_exceptions (UTL_NameList *e);

  virtual AST_Sequence *add_sequence (AST_Sequence *s);

  virtual AST_String *add_string (AST_String *s);

  virtual AST_Array *add_array (AST_Array *a);

  virtual AST_Native *add_native (AST_Native *n);

  virtual AST_Factory *add_factory (AST_Factory *f);

  // Call back end add_XXX functions for this scope.
  virtual AST_Decl *call_add (void);

  // Data Accessors.
  AST_Decl::NodeType scope_node_type (void)
  {
    return pd_scope_node_type;
  }

  // Narrowing.

  DEF_NARROW_FROM_SCOPE(UTL_Scope);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Other Operations.

  // Name Lookup Mechanism
  AST_Decl *lookup_by_name (UTL_ScopedName *,
                            bool treat_as_ref,
                            bool in_parent = true,
                            bool full_def_only = false);

  // Look up the Identifier * specified only in the local scope.
  AST_Decl *lookup_by_name_local (Identifier *,
                                  long index,
                                  bool full_def_only = false);

  // Look up a predefined type by its ExprType.
  AST_Decl *lookup_primitive_type (AST_Expression::ExprType);

  // Look up one of the pseudo-object types.
  AST_Decl *lookup_pseudo (Identifier *);

  virtual AST_Decl *look_in_previous (Identifier *e,
                                      bool ignore_fwd = false);

  // How many entries are used?
  virtual unsigned long nmembers (void);

  // Add to decls. Node represents a local declaration
  // The new decl e is inserted after ex if ex is not 0.
  // This one is public because the case of a reopened
  // module necessitates a call to this from
  // be_generator::create_module().
  void add_to_scope (AST_Decl *e,
                     AST_Decl *ex = 0);

  // Add to name_referenced identifier list. It's public because
  // a union needs to add its enum discriminator label names.
  void add_to_name_referenced (Identifier *id);

  // Accessors for the has_prefix_ member.
  bool has_prefix (void);
  void has_prefix (bool val);

  // Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

protected:
  // UTL_Scope protected addition protocol. This protocol is
  // used from sub-classes of UTL_Scope to insert nodes directly
  // into the specified node table (either decls or local_types)
  // Additionally, UTL_Scope also maintains a record of what nodes
  // defined elsewhere were referenced in this scope.

  // Replace old_decl with new_decl.
  void replace_scope (AST_Decl *old_decl,
                      AST_Decl *new_decl);

  // Replace old_decl with new_decl.
  void replace_referenced (AST_Decl *old_decl,
                           AST_Decl *new_decl);

  // Add to referenced nodes set.
  // If ex is not 0 'e' will be inserted at the position
  // after ex, which is already in the list.
  void add_to_referenced (AST_Decl *e,
                          bool recursive,
                          Identifier *id,
                          AST_Decl *ex = 0);

  // Add to local types. Node represents a local manifest type.
  void add_to_local_types (AST_Decl *e);

  // Has this node been referenced here already?
  virtual bool referenced (AST_Decl *e,
                               Identifier *id = 0);

  // Look up a scoped name in the inherited list.
  virtual AST_Decl *look_in_inherited (UTL_ScopedName *,
                                       bool treat_as_ref);

  // Look up a scoped name in the supported interface list.
  virtual AST_Decl *look_in_supported (UTL_ScopedName *,
                                       bool treat_as_ref);

  // Lookup based on the local name.
  AST_Decl *lookup_for_add (AST_Decl *d,
                            bool treat_as_ref);

  // Is there a (case-insensitive) clash between a local name
  // and an IDL keyword?
  int idl_keyword_clash (Identifier *e);

  // Checks for modules, or defns of forward declared struct or unions.
  bool redef_clash (AST_Decl::NodeType new_nt,
                    AST_Decl::NodeType scope_elem_nt);

  void check_for_predef_seq (AST_Decl *d);
  // Set the appropriate *_seen_ flag if we are seeing a spec-defined
  // sequence of a basic type.

protected:
  // Data.

  // What type is this node? This is a cache of the information
  // in AST_Decl
  const AST_Decl::NodeType pd_scope_node_type;

  // Storage for declarations in this scope.
  AST_Decl **pd_decls;                // Store declarations
  long pd_decls_allocated;            // How many allocated?
  long pd_decls_used;                 // How many used?

  // Storage for local manifest types in this scope.
  AST_Decl **pd_local_types;          // Store types
  long pd_locals_allocated;           // How many allocated?
  long pd_locals_used;                // How many used?

  // Storage for referenced nodes in this scope.
  AST_Decl **pd_referenced;           // Store references
  long pd_referenced_allocated;       // How many allocated?
  long  pd_referenced_used;           // How many used?

  // Storage for identifiers used in this scope. CORBA 2.3 introduced
  // stricter rules for clashes during name resolution, and the information
  // in pd_referenced is not enough to catch them all.
  Identifier **pd_name_referenced;    // Store name references
  long pd_name_referenced_allocated;  // How many allocated?
  long pd_name_referenced_used;       // How many used?

  // Have we seen a #pragma prefix declaration in this scope?
  bool has_prefix_;

  // Friend class UTL_ScopeActiveIterator defines active iterator for
  // UTL_Scope. Definition follows below.
  friend class  UTL_ScopeActiveIterator;

private:
  friend int tao_yyparse (void);
  friend class AST_Enum;
  friend class IDL_GlobalData;

  // Scope Management Protocol.
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

  virtual AST_Constant *fe_add_constant (AST_Constant *i);

  virtual AST_Exception *fe_add_exception (AST_Exception *e);

  virtual AST_Attribute *fe_add_attribute (AST_Attribute *a);

  virtual AST_Operation *fe_add_operation (AST_Operation *o);

  virtual AST_Argument *fe_add_argument (AST_Argument *a);

  virtual AST_Union *fe_add_union (AST_Union *u);

  virtual AST_UnionFwd *fe_add_union_fwd (AST_UnionFwd *u);

  virtual AST_UnionBranch *fe_add_union_branch (AST_UnionBranch *b);

  virtual AST_Structure *fe_add_structure (AST_Structure *s);

  virtual AST_StructureFwd *fe_add_structure_fwd (AST_StructureFwd *s);

  virtual AST_Field *fe_add_field (AST_Field *f);

  virtual AST_Enum *fe_add_enum (AST_Enum *e);

  virtual AST_EnumVal *fe_add_enum_val (AST_EnumVal *v);

  virtual AST_Typedef *fe_add_typedef (AST_Typedef *t);

  virtual UTL_StrList *fe_add_context (UTL_StrList *c);

  virtual UTL_NameList *fe_add_exceptions (UTL_NameList *e);

  virtual AST_Sequence *fe_add_sequence (AST_Sequence *s);

  virtual AST_String *fe_add_string (AST_String *s);

  virtual AST_Array *fe_add_array (AST_Array *a);

  virtual AST_Native *fe_add_native (AST_Native *n);

  virtual AST_Factory *fe_add_factory (AST_Factory *f);

  virtual AST_ValueBox *fe_add_valuebox (AST_ValueBox *vb);
};

// Active iterator for a UTL_Scope node
class TAO_IDL_FE_Export UTL_ScopeActiveIterator
{
public:
  // Operations.

  // Constructor.
  UTL_ScopeActiveIterator (UTL_Scope *s,
                           UTL_Scope::ScopeIterationKind ik);

  // Advance to next item.
  void next (void);

  // Get current item.
  AST_Decl *item (void);

  // Have we iterated over entire scope?
  bool is_done (void);

  // What kind of iterator is this?
  UTL_Scope::ScopeIterationKind iteration_kind (void);

  // What stage are we in with this iterator?
  UTL_Scope::ScopeIterationKind iteration_stage (void);

private:
  // Scope to iterate over.
  UTL_Scope *iter_source;

  // What kind of iteration?
  UTL_Scope::ScopeIterationKind ik;

  // What stage?
  UTL_Scope::ScopeIterationKind stage;

  // What location in stage?
  long il;
};

#endif           // _UTL_SCOPE_UTL_SCOPE_HH
