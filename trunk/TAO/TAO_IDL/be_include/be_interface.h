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
class TAO_IDL_Inheritance_Hierarchy_Worker;

// Forward declaration of the strategy
class be_interface_strategy;

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
  enum
  {
    THRU_POA = 0,
    DIRECT = 1
  };

  // Used to pass functions to the template method.
  typedef int (*tao_code_emitter) (be_interface *,
                                   be_interface *,
                                   TAO_OutStream *);

  be_interface (void);
  // Default constructor.

  be_interface (UTL_ScopedName *n,
                AST_Interface **ih,
                long nih,
                AST_Interface **ih_flat,
                long nih_flat,
                idl_bool local,
                idl_bool abstract);
  // Constructor that sets its scoped name <n>, a list of inherited interfaces
  // <ih>, the number of inherited interfaces <nih>, and any prgmas <p>.

  ~be_interface (void);
  // Destructor.

  be_interface_strategy *set_strategy (be_interface_strategy *new_strategy);
  // Set the strategy to generate the names.

  // Methods, which access the strategy.

  const char *local_name (void) const;
  // Return the local name.

  virtual const char *full_name (void);
  // Return the stringified full name.

  const char *flat_name (void) const;
  // Return the flattened full scoped name.

  virtual const char *repoID (void) const;
  // Retrieve the repository ID.

  const char *full_skel_name (void) const;
  // Retrieve the fully scoped skel class name.

  const char *full_coll_name (int) const;
  // Retrieve the fully qualified collocated class name.

  const char *local_coll_name (int) const;
  // Retrieve the fully qualified collocated class name.

    virtual const char *base_proxy_impl_name (void);
  // retrieve the name of the base proxy implementation.

  virtual const char *full_base_proxy_impl_name (void);
  // retrieve the fully qualified name of the base proxy
  // implementation.

  virtual const char *remote_proxy_impl_name (void);
  // retrieve the name of the remote  proxy implementation.

  virtual const char *full_remote_proxy_impl_name (void);
  // retrieve the fully qualified name of the remote
  // proxy implementation.

  virtual const char *thru_poa_proxy_impl_name (void);
  // retrieve the name of the ThruPOA  proxy implementation.

  virtual const char *full_thru_poa_proxy_impl_name (void);
  // retrieve the fully qualified name of the ThruPOA  proxy
  // implementation.

  virtual const char *direct_proxy_impl_name (void);
  // retrieve the name of the Directx  proxy implementation.

  virtual const char *full_direct_proxy_impl_name (void);
  // retrieve the fully qualified name of the Directx  proxy
  // implementation.

  virtual const char *base_proxy_broker_name (void);
  // retrieve the name of the base proxy broker.

  virtual const char *full_base_proxy_broker_name (void);
  // retrieve the fully qualified name of the base proxy broker.

  virtual const char *remote_proxy_broker_name (void);
  // retrieve the name of the remote  proxy broker implementation.

  virtual const char *full_remote_proxy_broker_name (void);
  // retrieve the fully qualified name of the remote  proxy broker
  // implementation.

  virtual const char *strategized_proxy_broker_name (void);
  // retrieve the name of the strategized proxy broker implementation.

  virtual const char *full_strategized_proxy_broker_name (void);
  // retrieve the fully qualified name of the strategized proxy broker
  // implementation.

  virtual const char *client_enclosing_scope (void);
  // Return the client scope that encloses the interface.

  virtual const char *flat_client_enclosing_scope (void);
  // Return the "flattened" scope that encloses
  // the interface.

  virtual const char *server_enclosing_scope (void);
  // Return the server scope that encloses the interface.

  const char *relative_skel_name (const char *skel_name);
  // Retrieve skeleton name.

  void compute_full_skel_name (const char *prefix,
                               char *&skel_name);
  // Build up the skeleton name.

  static const char *relative_name (const char *localname,
                                    const char *othername);

  virtual void gen_def_ctors (TAO_OutStream* os);
  // Call the default constructors of all the base classes.

  virtual void gen_copy_ctors (TAO_OutStream* os);
  // Call the copy constructors of all the base classes.

  virtual void gen_stub_ctor (TAO_OutStream* os);
  // Generated the global hooks used for non-defined forward
  // declared interfaces, and the contructor from stub object.

  virtual int gen_var_defn (char *interface_name = 0);
  // Generate the var definition. If <interface_name> is not 0, generate
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

  /// Iterate over the inheritance hierarchy and call the
  /// worker->emit() method for each interface on it.
  virtual int traverse_inheritance_graph (TAO_IDL_Inheritance_Hierarchy_Worker &worker,
                                          TAO_OutStream *os);

  /// Wrap the @c gen parameter and call the generic version of
  /// traverse_inheritance_graph()
  virtual int traverse_inheritance_graph (tao_code_emitter gen,
                                          TAO_OutStream *os);

  int in_mult_inheritance (void);
  // Am I in some form of multiple inheritance
  // -1 => error
  // 0 => no
  // 1 => yes

  void in_mult_inheritance (int mi);
  // Set a new value.

  virtual void destroy (void);
  // Cleanup function.

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing.
  DEF_NARROW_METHODS3 (be_interface, AST_Interface, be_scope, be_type);
  DEF_NARROW_FROM_DECL (be_interface);
  DEF_NARROW_FROM_SCOPE (be_interface);

  static int is_a_helper (be_interface *,
                          be_interface *,
                          TAO_OutStream *os);
  // Helper method passed to the template method that generates code for the
  // is_a method.

  static int queryinterface_helper (be_interface *,
                                    be_interface *,
                                    TAO_OutStream *os);
  // Helper method passed to the template method that generates code for the
  // _tao_QueryInterface method.

  static int downcast_helper (be_interface *,
                              be_interface *,
                              TAO_OutStream *os);
  // Helper method passed to the template method that generates code for the
  // downcast.

  static int gen_optable_helper (be_interface *,
                                 be_interface *,
                                 TAO_OutStream *os);
  // Helper method passed to the template method to generate code for the
  // operation table.

  static int ami_handler_gen_optable_helper (be_interface *,
                                             be_interface *,
                                             TAO_OutStream *os);
  // Helper method passed to the template method to generate code for the
  // operation table.

  static int gen_skel_helper (be_interface *,
                              be_interface *,
                              TAO_OutStream *os);
  // Helper method passed to the template method to generate code for the
  // skeletons in the inline file.

  static int copy_ctor_helper (be_interface *,
                               be_interface *,
                               TAO_OutStream *os);
  // Helper method passed to the template method to invoke ctors of all the
  // base classes.

  static int in_mult_inheritance_helper (be_interface *,
                                         be_interface *,
                                         TAO_OutStream *os);
  // Helper method to determine if the interface node is involved in some kind
  // of multiple inheritance or not. Required on the skeleton side.

  static int gen_def_ctors_helper (be_interface* node,
                                   be_interface* base,
                                   TAO_OutStream *os);

  // Helper method to generate a call to the default
  // constructors of all the base classes.

  static int gen_copy_ctors_helper (be_interface* node,
                                    be_interface* base,
                                    TAO_OutStream *os);

  // Helper method to generate a call to the copy
  // constructors of all the base classes.


  int gen_operation_table (void);
  // Generate the operation table including entries for inherited interfaces.

  int gen_operation_table (const char *flat_name,
                           const char *skeleton_class_name);
  // Like the previous version, but receive the class "flat name" and
  // skeleton names as arguments.  Useful in the generation of closely
  // related classes, such as the AMH skeleton.

  int gen_optable_entries (be_interface *);
  // generate the operation table entries.

  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                    int is_extra_state = 0);
  // Find the next state, used to hide differences between variants of
  // interfaces.

  int has_extra_code_generation (TAO_CodeGen::CG_STATE current_state);
  // Returns 1 if additional code needs to be generated, the behavior
  // is driven by the strategy connected with this interface.

  void original_interface (be_interface *original_interface);
  // Sets the original interface from which this one was created,
  // applies only to implied IDL.

  be_interface *original_interface ();
  // Returns the original interface from which this one was created,
  // applies only to implied IDL

  be_interface *replacement ();
  // Returns an interface, which can be used instead.
  // Needs to get set by the strategy.

private:
  void gen_gperf_input_header (TAO_OutStream *ss);
  // Output the header (type declaration and %%) to the gperf's input
  // file.

  int gen_gperf_things (const char *flat_name);
  // Run GPERF and get the correct lookup and other operations
  // depending on which strategy we are using. Returns 0 on sucess, -1
  // on error.

  void gen_perfect_hash_class_definition (const char *flat_name);
  // Outputs the class definition for the perfect hashing. This class
  // will inherit from the TAO_Perfect_Hash_OpTable.

  void gen_binary_search_class_definition (const char *flat_name);
  // Outputs the class definition for the binary search . This class
  // will inherit from the TAO_Binary_Search_OpTable.

  void gen_linear_search_class_definition (const char *flat_name);
  // Outputs the class defintion for the linear search. This class
  // will inherit from the TAO_Linear_Search.

  int gen_gperf_lookup_methods (const char *flat_name);
  // This calls the GPERF program and gets the correct operation
  // lookup methods for the current OpLookup strategy.

  void gen_perfect_hash_instance (const char *flat_name);
  // Create an instance of this perfect hash table.

  void gen_binary_search_instance (const char *flat_name);
  // Create an instance of the binary search optable.

  void gen_linear_search_instance (const char *flat_name);
  // Create an instance of the linear search optable.

  int skel_count_;
  // Number of static skeletons in the operation table.

  int in_mult_inheritance_;
  // Am I directly or indirectly involved in a multiple inheritance. If the
  // value is -1 => not computed yet.

  be_interface_strategy *strategy_;
  // Member for holding the strategy for generating names.

  be_interface *original_interface_;
};

/**
 * @class TAO_IDL_Inheritance_Hierarcy_Worker
 *
 * @brief Implement the 'external form' of the iterator pattern for
 * the interface inheritance hierarchy.
 *
 * Many components in the IDL compiler need to traverse the
 * inheritance hierarchy for a particular interface, and generate code
 * for each base class.  The code to perform the traversal is
 * encapsulated in be_interface, and this class defines the interface
 * (in the C++ sense) that other IDL components must use to perform
 * the work on each element on the hierarchy.
 *
 * This class is a relatively recent addition to the IDL compiler,
 * originally just a pointer to function was used to customize the
 * traversal algorithm.  The class was added because we need to pass
 * some state to some of the code emitters, thus a function is not
 * good enough.
 */
class TAO_IDL_BE_Export TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  /// Destructor
  /**
   * This is a no-op, simply put here to keep compilers happy.
   */
  virtual ~TAO_IDL_Inheritance_Hierarchy_Worker ();

  /// Define the method invoked during the inheritance traversal
  /**
   * This method is invoked for each base interface in the hierarchy.
   *
   * @param derived_interface Pointer to the most derived interface in
   * the hierarchy, it remains constant during the complete traversal.
   *
   * @param output_stream The output stream that should be used to
   * emit code.
   *
   * @param base_interface Pointer to the base interface in the
   * hierarchy, it changes on each iteration.
   *
   * @return 0 if there was no error, -1 if there was one.
   */
  virtual int emit (be_interface *derived_interface,
                    TAO_OutStream *output_stream,
                    be_interface *base_interface) = 0;
};

#endif  // if !defined
