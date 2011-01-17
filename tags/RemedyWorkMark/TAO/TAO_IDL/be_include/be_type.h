
//=============================================================================
/**
 *  @file    be_type.h
 *
 *  $Id$
 *
 *  Extension of class AST_Type that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#ifndef BE_TYPE_H
#define BE_TYPE_H

#include "be_decl.h"
#include "ast_type.h"

#include "ace/SString.h"

class TAO_OutStream;

class be_type : public virtual AST_Type,
                public virtual be_decl
{
public:
  be_type (AST_Decl::NodeType nt,
           UTL_ScopedName *n);

  virtual ~be_type (void);

  /**
   * Return the typecode name. When both, the prefix and the suffix
   * are non null, it computes and returns a tc name. Else, it also
   * stores the result in a member variable.
   */
  UTL_ScopedName *tc_name (void);

  /// Type name of a node used when generating declarations for smart
  /// proxies.
  virtual const char *nested_sp_type_name (be_decl *use_scope,
                                           const char *suffix = 0,
                                           const char *prefix = 0);

  /// Compute the value of the member.
  void gen_fwd_helper_name (void);

  /// No-op, overridden in derived classes.
  virtual void gen_ostream_operator (TAO_OutStream *os,
                                     bool use_underscore);

  /// Generic implementation here, overridden for some types.
  virtual void gen_member_ostream_operator (TAO_OutStream *os,
                                            const char *instance_name,
                                            bool use_underscore,
                                            bool accessor = false);

  /// Accessor to the member.
  const char *fwd_helper_name (void) const;
  void fwd_helper_name (const char *name);

  /// Generate _var and _out typedefs for structs and unions.
  void gen_common_varout (TAO_OutStream *os);

  /// Generate common typedef and any destructor decl.
  void gen_stub_decls (TAO_OutStream *os);

  /// Accessors for the member.
  bool seen_in_sequence (void) const;
  virtual void seen_in_sequence (bool val);

  /// Accessors for the member.
  bool seen_in_operation (void) const;
  virtual void seen_in_operation (bool val);

  /**
   * Typedefs are tricky to handle, in many points their mapping
   * depend on base type they are aliasing.  Since typedefs can be
   * recursive simply using "base_type->node_type()" will not work, so
   * the most "unaliased" type is needed.
   */
  virtual AST_Decl::NodeType base_node_type (void) const;

  /// Clean up allocated members.
  virtual void destroy (void);

  // Visiting.
  virtual int accept (be_visitor* visitor);

  // Narrowing.

  DEF_NARROW_FROM_DECL (be_type);

protected:
  /// Computes the fully scoped typecode name.
  virtual void compute_tc_name (void);

  /// Typecode name.
  UTL_ScopedName *tc_name_;

  /// Used by interfaces, valuetypes and arrays to name helper structs.
  ACE_CString fwd_helper_name_;

  /// Have we generated our _var and _out class typedefs yet?
  bool common_varout_gen_;

  /// Has this declaration been used as a sequence element?
  bool seen_in_sequence_;

  /// Has this declaration been used as a return type or parameter?
  bool seen_in_operation_;
};

#endif // end of if !defined
