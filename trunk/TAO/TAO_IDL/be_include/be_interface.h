/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_interface.h
//
// = DESCRIPTION
//    Extension of class AST_Interface that provides additional means for C++
//    mapping of an interface.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale,
//    Michael Kircher
//
// ============================================================================

#ifndef TAO_BE_INTERFACE_H
#define TAO_BE_INTERFACE_H

#include "be_scope.h"
#include "be_type.h"
#include "be_codegen.h"
#include "ast_interface.h"

class TAO_OutStream;

// Forward declaration of the strategy
class be_interface_strategy;

/*
 * BE_Interface
 */
class be_interface : public virtual AST_Interface,
                     public virtual be_scope,
                     public virtual be_type
{
  // = TITLE
  //   The back end extension of the AST_Interface class
  //
  // = DESCRIPTION
  //
public:
  enum {
    THRU_POA = 0,
    DIRECT = 1
  };

  // used to pass functions to the template method
  typedef int (*tao_code_emitter) (be_interface *, be_interface *, TAO_OutStream *);

  // Operations
  be_interface (void);
  // Default constructor

  be_interface (UTL_ScopedName *n,
                AST_Interface **ih,
                long nih,
                UTL_StrList *p);
  // Constructor that sets its scoped name <n>, a list of inherited interfaces
  // <ih>, the number of inherited interfaces <nih>, and any prgmas <p>

  ~be_interface (void);
  // dtor

  be_interface_strategy *set_strategy (be_interface_strategy *new_strategy);
  // Set the strategy to generate the names

  // Methods, which access the strategy

  const char *local_name (void) const;
  // return the local name

  const char *full_name (void) const;
  // return the stringified full name

  const char *flat_name (void) const;
  // return the flattened full scoped name.

  const char *repoID (void) const;
  // retrieve the repository ID.

  const char *full_skel_name (void) const;
  // retrieve the fully scoped skel class name.

  const char *full_coll_name (int) const;
  // retrieve the fully qualified collocated class name

  const char *local_coll_name (int) const;
  // retrieve the fully qualified collocated class name.

  const char *relative_skel_name (const char *skel_name);
  // relative skeleton name

  void compute_full_skel_name (const char *prefix,char *&skel_name);
  // Build up the skeleton name

  static const char *relative_name (const char *localname, const char *othername);

  virtual void gen_def_ctors (TAO_OutStream* os);
  //call the default constructors of all the base classes

  virtual void gen_copy_ctors (TAO_OutStream* os);
  //call the copy constructors of all the base classes

  virtual int gen_var_defn (char *interface_name = 0);
  // generate the var definition. If <interface_name> is not 0, generate
  // the var defn for that name. Otherwise, do it for the interface you
  // are visiting (this).

  virtual int gen_var_impl (char *interface_local_name = 0,
                            char *interface_full_name  = 0);
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
  //
  // Each interface (to fix names "T") also defines two help classes,
  // the "collocated" class inherits from T, but delegates on the
  // skeleton for T (usually POA_T or POA_ModuleName::T), in other
  // words it is a Bridge from T to its implementation.
  // The class is nested inside the skeleton class.
  //
  // The "stub" is a class defined on the client scope, it actually
  // defines the stubs (all operations in T are pure virtual).
  // @@ TODO currently the stub class is not implemented.
  //

  virtual int traverse_inheritance_graph (tao_code_emitter gen,
                                          TAO_OutStream *os);
  // template method using breadth first traversal of inheritance graph

  int in_mult_inheritance (void);
  // am I in some form of multiple inheritance
  // -1 => error
  // 0 => no
  // 1 => yes

  void in_mult_inheritance (int mi);
  // set a new value

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS3 (be_interface, AST_Interface, be_scope, be_type);
  DEF_NARROW_FROM_DECL (be_interface);
  DEF_NARROW_FROM_SCOPE (be_interface);

  static int is_a_helper (be_interface *, be_interface *, TAO_OutStream *os);
  // helper method passed to the template method that generates code for the
  // is_a method

  static int downcast_helper (be_interface *,
                              be_interface *,
                              TAO_OutStream *os);
  // helper method passed to the template method that generates code for the
  // downcast.

  static int gen_optable_helper (be_interface *,
                                 be_interface *,
                                 TAO_OutStream *os);
  // helper method passed to the template method to generate code for the
  // operation table

  static int ami_handler_gen_optable_helper (be_interface *,
                                             be_interface *,
                                             TAO_OutStream *os);
  // helper method passed to the template method to generate code for the
  // operation table

  static int gen_skel_helper (be_interface *,
                              be_interface *,
                              TAO_OutStream *os);
  // helper method passed to the template method to generate code for the
  // skeletons in the inline file

  static int copy_ctor_helper (be_interface *,
                               be_interface *,
                               TAO_OutStream *os);
  // helper method passed to the template method to invoke ctors of all the
  // base classes.

  static int in_mult_inheritance_helper (be_interface *,
                                         be_interface *,
                                         TAO_OutStream *os);
  // helper method to determine if the interface node is involved in some kind
  // of multiple inheritance or not. Required on the skeleton side

  static int gen_def_ctors_helper (be_interface* node,
                                   be_interface* base,
                                   TAO_OutStream *os);

  //helper method to generate a call to the default constructors of all the base classes

  static int gen_copy_ctors_helper (be_interface* node,
                                   be_interface* base,
                                   TAO_OutStream *os);

  //helper method to generate a call to the copy constructors of all the base classes


  int gen_operation_table (void);
  // generate the operation table including entries for inherited interfaces

  int gen_optable_entries (be_interface *);
  // generate the operation table entries.

  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state);
  // find the next state, used to hide differences between variants of 
  // interfaces.

private:
  void gen_gperf_input_header (TAO_OutStream *ss);
  // Output the header (type declaration and %%) to the gperf's input
  // file.

  int gen_gperf_things (void);
  // Run GPERF and get the correct lookup and other operations
  // depending on which strategy we are using. Returns 0 on sucess, -1
  // on error.

  void gen_perfect_hash_class_definition (void);
  // Outputs the class definition for the perfect hashing. This class
  // will inherit from the TAO_Perfect_Hash_OpTable.

  void gen_binary_search_class_definition (void);
  // Outputs the class definition for the binary search . This class
  // will inherit from the TAO_Binary_Search_OpTable.

  void gen_linear_search_class_definition (void);
  // Outputs the class defintion for the linear search. This class
  // will inherit from the TAO_Linear_Search.

  int gen_gperf_lookup_methods (void);
  // This calls the GPERF program and gets the correct operation
  // lookup methods for the current OpLookup strategy.

  void gen_perfect_hash_instance (void);
  // Create an instance of this perfect hash table.

  void gen_binary_search_instance (void);
  // Create an instance of the binary search optable.

  void gen_linear_search_instance (void);
  // Create an instance of the linear search optable.

  int skel_count_;
  // Number of static skeletons in the operation table.

  int in_mult_inheritance_;
  // am I directly or indirectly involved in a multiple inheritance. If the
  // value is -1 => not computed yet.

  be_interface_strategy *strategy_;
  // Member for holding the strategy for generating names
};

#endif  // if !defined

