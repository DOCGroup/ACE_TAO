/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_valuetype.h
 *
 *  $Id$
 *
 *  Extension of class AST_Interface and be_interface that provides
 *  additional means for C++ mapping of an valuetype.
 *
 *
 */
//=============================================================================

#ifndef TAO_BE_VALUETYPE_H
#define TAO_BE_VALUETYPE_H

#include "be_interface.h"
#include "ast_valuetype.h"
#include "ast_field.h"

class TAO_OutStream;

class be_valuetype : public virtual be_interface,
                     public virtual AST_ValueType
{
  // = TITLE
  //    Backend-class for valuetypes
  //
  // = DESCRIPTION
  //    Extends be_interface and overrides member functions for valuetype
  //    mapping.
  //
public:
  be_valuetype (UTL_ScopedName *n,
                AST_Type **inherits,
                long n_inherits,
                AST_Type *inherits_concrete,
                AST_Interface **inherits_flat,
                long n_inherits_flat,
                AST_Type **supports,
                long n_supports,
                AST_Type *supports_concrete,
                bool abstract,
                bool truncatable,
                bool custom);

  ~be_valuetype (void);

  virtual void redefine (AST_Interface *from);

  /// Should generate optimized form?
  bool opt_accessor (void);

  /// Generate the helper functions definition.
  virtual int gen_helper_header (char *local_name = 0,
                                 char *full_name = 0);

  /// Generate the helper functions inline implementation.
  virtual int gen_helper_inline (char *local_name = 0,
                                 char *full_name = 0);

  /// Generate the helper functions implementation.
  virtual int gen_helper_stubs (char *local_name = 0,
                                char *full_name = 0);

  /// Overridden from class be_type.
  virtual void gen_ostream_operator (TAO_OutStream *os,
                                     bool use_underscore);

  /// Generate the declarations used by the template _var, _out
  /// classes for valuetypes, and by sequence template classes.
  void gen_var_out_seq_decls (void);

  /// Retrieve the fully scoped skel class name.
  const char *full_obv_skel_name (void);

  const char *field_pd_prefix (void);

  const char *field_pd_postfix (void);

  // For building the name for private data fields.
  be_valuetype *statefull_inherit (void);

  /// Load the insert queue with all the interfaces we support, and
  /// call the generic version of traverse_inheritance_graph().
  int traverse_supports_list_graphs (
      tao_code_emitter gen,
      TAO_OutStream *os,
      bool use_abstract_paths = true,
      bool use_concrete_paths = true
    );

  /// Load the insert queue with the concrete interface we support, and
  /// call the generic version of traverse_inheritance_graph().
  int traverse_concrete_inheritance_graph (
      tao_code_emitter gen,
      TAO_OutStream *os
    );

  /// Do we support at least one abstract interface?
  bool supports_abstract (void) const;

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);

 // Narrowing.

  DEF_NARROW_FROM_DECL (be_valuetype);
  DEF_NARROW_FROM_SCOPE (be_valuetype);

  /// Compute the fully scoped skel class name.
  void compute_fullobvskelname (void);

  /// Compute the count of private/public/all data members.
  ACE_CDR::ULong data_members_count (
      AST_Field::Visibility vis = AST_Field::vis_NA
    );

  /// Helper method passed to the template method to generate code for
  /// adding abstract supported interfaces to the inheritance list.
  static int abstract_supports_helper (be_interface *node,
                                       be_interface *base,
                                       TAO_OutStream *os);

  /// Helper method to generate *_skel operations for the concrete
  /// interface that we support (if any) and those of its base classes.
  static int gen_skel_helper (be_interface *node,
                              be_interface *base,
                              TAO_OutStream *os);

  /// Accessor to the member.
  const char *fwd_helper_name (void) const;

  // There are three possible situations.
  // (1) If there is no initializers but at least one operation.
  //     In this case we don't need to bother about factory.
  //
  // (2) There are no (operations or initializers) (i.e. only state
  //     members) then we need a concrete type-specific factory
  //     class whose create_for_unmarshal creates OBV_ class.
  //
  // (3) There is at least one operation and at least one initializer.
  //     In this case we need to generate abstract factory class.
  //
  // Here I reflect these situations.
  enum FactoryStyle
  {
    FS_UNKNOWN,
    FS_NO_FACTORY,
    FS_CONCRETE_FACTORY,
    FS_ABSTRACT_FACTORY
  };

  /// Determine what kind of factory needed.
  FactoryStyle determine_factory_style (void);

  /// Recurse down the inheritance tree to determine
  /// if valuetype has at least one operation/attribute.
  bool have_operation (void);

  /// Check if VT supports an interface with at least 1 operation.
  static bool have_supported_op (be_interface *node);

  /// Use the above enum and methods to determine this after the
  /// node's scope is visited but before code generation.
  virtual bool will_have_factory (void);

  /// Decides whether to generate the initializing contstructor or not.
  bool has_member (void);

private:
  /// Do we support at least one abstract interface?
  bool supports_abstract_;

  /// Have these been done already?
  bool var_out_seq_decls_gen_;

  /// To differentiate from the interface skeleton name.
  char *full_obv_skel_name_;
};

#endif  // if !defined
