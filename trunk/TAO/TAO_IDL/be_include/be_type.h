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

class TAO_OutStream;

/*
 * BE_Type
 */
class be_type : public virtual AST_Type,
                public virtual be_decl
{
public:
  // =Operations

  be_type (void);
  // default constructor

  be_type (AST_Decl::NodeType nt, UTL_ScopedName *n, UTL_StrList *p);
  // constructor

  virtual ~be_type (void);

  virtual int gen_var_defn (char *interface_local_name = 0);
  // generate the _var class definition.
  // generate the var definition. If <interface_name> is not 0, generate
  // the var defn for that name. Otherwise, do it for the interface you
  // are visiting (this).

  virtual int gen_var_impl (char *interface_local_name = 0,
                            char *interface_full_name = 0);
  // Generate the implementation for the _var class.
  // If any one of the argument is 0, then use the name in <this>,
  // otherwise use the name given. Just making the class more useful.

  virtual int gen_out_defn (char *interface_name = 0);
  // Generate the out class definition. If <interface_name> is not 0,
  // generate the out defn for that name. Otherwise, do it for the
  // interface you are visiting (this).

  virtual int gen_out_impl (char *interface_local_name = 0,
                            char *interface_full_name = 0);
  // Generate the out class implementation.
  // If any one of the argument is 0, then use the name giin this
  // node, else use the arguments. );

  UTL_ScopedName *tc_name (const char *prefix = 0,
                           const char *suffix = 0);
  // Return the typecode name. When both, the prefix and the suffix
  // are non null, it computes and returns a tc name. Else, it also
  // stores the result in a member variable.

  virtual const char *nested_type_name (be_decl *d,
                                        const char *suffix = 0,
                                        const char *prefix = 0);
  // type name of a node used when generating declarations

  virtual const char *nested_sp_type_name (be_decl *use_scope, 
                                           const char *suffix = 0, 
                                           const char *prefix = 0);
  // type name of a node used when generating declarations for smart
  // proxies.

  virtual AST_Decl::NodeType base_node_type (void) const;
  // Typedefs are tricky to handle, in many points their mapping
  // depend on base type they are aliasing.  Since typedefs can be
  // recursive simply using "base_type->node_type()" will not work, so
  // the most "unaliased" type is needed.

  virtual idl_bool in_recursion (be_type *node = 0);
  // determine if we are involved in some kind of limited recursion. Most types
  // cannot be involved except structs and unions.
  // If the parameter is 0, we are trying to determine this for ourselves.

  // Visiting
  virtual int accept (be_visitor* visitor);

  // Narrowing
  DEF_NARROW_METHODS2 (be_type, AST_Type, be_decl);
  DEF_NARROW_FROM_DECL (be_type);

protected:
  virtual void compute_tc_name (void);
  // computes the fully scoped typecode name

  virtual UTL_ScopedName * compute_tc_name (const char *prefix,
                                            const char *suffix);
  // Compute and return the value.

  virtual const char *nested_name (const char* local_name,
                                   const char* full_name,
                                   be_decl *use_scope, 
                                   const char *suffix, 
                                   const char *prefix);
  // type name of a node used when generating declarations

  UTL_ScopedName *tc_name_;
  // typecode name

  char *type_name_;
  // type name (this uses the ACE NESTED macro)

  char *nested_type_name_;
  // for the corresponding method.
};

#endif // end of if !defined
