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

  virtual int gen_var_defn (char *interface_local_name = 0);
  // Generate the _var class definition.
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
  // node, else use the arguments.

  UTL_ScopedName *tc_name (void);
  // Return the typecode name. When both, the prefix and the suffix
  // are non null, it computes and returns a tc name. Else, it also
  // stores the result in a member variable.

  virtual const char *nested_type_name (be_decl *d,
                                        const char *suffix = 0,
                                        const char *prefix = 0);
  // Type name of a node used when generating declarations.

  virtual const char *nested_sp_type_name (be_decl *use_scope, 
                                           const char *suffix = 0, 
                                           const char *prefix = 0);
  // Type name of a node used when generating declarations for smart
  // proxies.

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
  DEF_NARROW_METHODS2 (be_type, AST_Type, be_decl);
  DEF_NARROW_FROM_DECL (be_type);

protected:
  virtual void compute_tc_name (void);
  // Computes the fully scoped typecode name.

  virtual const char *nested_name (const char* local_name,
                                   const char* full_name,
                                   be_decl *use_scope, 
                                   const char *suffix, 
                                   const char *prefix);
  // Type name of a node used when generating declarations.

  UTL_ScopedName *tc_name_;
  // Typecode name.

  char *nested_type_name_;
  // For the corresponding method.
};

#endif // end of if !defined
