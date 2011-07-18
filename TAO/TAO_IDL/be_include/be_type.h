// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_type.h
//
// = DESCRIPTION
//    Extension of class AST_Type that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

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
  be_type (void);
  // Default constructor.

  be_type (AST_Decl::NodeType nt,
           UTL_ScopedName *n);
  // Constructor.

  virtual ~be_type (void);
  // Destructor.

  UTL_ScopedName *tc_name (void);
  // Return the typecode name. When both, the prefix and the suffix
  // are non null, it computes and returns a tc name. Else, it also
  // stores the result in a member variable.

  virtual const char *nested_sp_type_name (be_decl *use_scope,
                                           const char *suffix = 0,
                                           const char *prefix = 0);
  // Type name of a node used when generating declarations for smart
  // proxies.

  void gen_fwd_helper_name (void);
  // Compute the value of the member.
  
  virtual void gen_ostream_operator (TAO_OutStream *os);
  // No-op, overridden in derived classes.

  virtual void gen_member_ostream_operator (TAO_OutStream *os,
                                            const char *instance_name,
                                            bool accessor = false);
  // Generic implementation here, overridden for some types.

  const char *fwd_helper_name (void) const;
  void fwd_helper_name (const char *name);
  // Accessor to the member.

  void gen_common_varout (TAO_OutStream *os);
  // Generate _var and _out typedefs for structs and unions.

  bool seen_in_sequence (void) const;
  virtual void seen_in_sequence (bool val);
  // Accessors for the member.

  bool seen_in_operation (void) const;
  virtual void seen_in_operation (bool val);
  // Accessors for the member.

  virtual AST_Decl::NodeType base_node_type (void) const;
  // Typedefs are tricky to handle, in many points their mapping
  // depend on base type they are aliasing.  Since typedefs can be
  // recursive simply using "base_type->node_type()" will not work, so
  // the most "unaliased" type is needed.

  virtual void destroy (void);
  // Clean up allocated members.

  // Visiting.
  virtual int accept (be_visitor* visitor);

  // Narrowing.

  DEF_NARROW_FROM_DECL (be_type);

protected:
  virtual void compute_tc_name (void);
  // Computes the fully scoped typecode name.

  UTL_ScopedName *tc_name_;
  // Typecode name.

  ACE_CString fwd_helper_name_;
  // Used by interfaces, valuetypes and arrays to name helper structs.

  bool common_varout_gen_;
  // Have we generated our _var and _out class typedefs yet?

  bool seen_in_sequence_;
  // Has this declaration been used as a sequence element?

  bool seen_in_operation_;
  // Has this declaration been used as a return type or parameter?
};

#endif // end of if !defined
