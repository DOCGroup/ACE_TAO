// -*- C++ -*-

//=============================================================================
/**
 *  @file    be_interface.h
 *
 *  $Id$
 *
 *  Extension of class AST_Interface that provides additional means for C++
 *  mapping of an interface.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 *  @author Michael Kircher
 */
//=============================================================================

#ifndef TAO_BE_INTERFACE_H
#define TAO_BE_INTERFACE_H

#include "be_scope.h"
#include "be_type.h"
#include "be_codegen.h"
#include "ast_interface.h"

class TAO_OutStream;
class TAO_IDL_Inheritance_Hierarchy_Worker;
class be_visitor;
class be_interface_strategy;

class UTL_ExceptList;

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

  be_interface (UTL_ScopedName *n,
                AST_Type **ih,
                long nih,
                AST_Interface **ih_flat,
                long nih_flat,
                bool local,
                bool abstract);

  /// Destructor.
  ~be_interface (void);

  /// Set the strategy to generate the names.
  be_interface_strategy *set_strategy (be_interface_strategy *new_strategy);

  // Methods, which access the strategy.

  /// Return the local name.
  const char *local_name (void) const;

  /// Return the stringified full name.
  virtual const char *full_name (void);

  /// Return the flattened full scoped name.
  virtual const char *flat_name (void);

  /// Retrieve the repository ID.
  virtual const char *repoID (void) const;

  /// Retrieve the fully scoped skel class name.
  const char *full_skel_name (void) const;

  /// Retrieve the fully qualified collocated class name.
  const char *full_coll_name (int) const;

  /// Retrieve the fully qualified collocated class name.
  const char *local_coll_name (int) const;

  /// retrieve the name of the base proxy implementation.
  virtual const char *base_proxy_impl_name (void);

  /// retrieve the fully qualified name of the base proxy
  /// implementation.
  virtual const char *full_base_proxy_impl_name (void);

  /// retrieve the name of the remote  proxy implementation.
  virtual const char *remote_proxy_impl_name (void);

  /// retrieve the fully qualified name of the remote
  /// proxy implementation.
  virtual const char *full_remote_proxy_impl_name (void);

  /// retrieve the name of the Directx  proxy implementation.
  virtual const char *direct_proxy_impl_name (void);

  /// retrieve the fully qualified name of the Directx  proxy
  /// implementation.
  virtual const char *full_direct_proxy_impl_name (void);

  /// retrieve the name of the base proxy broker.
  virtual const char *base_proxy_broker_name (void);

  /// retrieve the fully qualified name of the base proxy broker.
  virtual const char *full_base_proxy_broker_name (void);

  /// retrieve the name of the remote  proxy broker implementation.
  virtual const char *remote_proxy_broker_name (void);

  /// retrieve the fully qualified name of the remote  proxy broker
  /// implementation.
  virtual const char *full_remote_proxy_broker_name (void);

  /// retrieve the name of the strategized proxy broker implementation.
  virtual const char *strategized_proxy_broker_name (void);

  /// retrieve the fully qualified name of the strategized proxy broker
  /// implementation.
  virtual const char *full_strategized_proxy_broker_name (void);

  /// Return the client scope that encloses the interface.
  virtual const char *client_enclosing_scope (void);

  /// Return the "flattened" scope that encloses
  /// the interface.
  virtual const char *flat_client_enclosing_scope (void);

  /// Return the server scope that encloses the interface.
  virtual const char *server_enclosing_scope (void);

  /// Retrieve skeleton name.
  const char *relative_skel_name (const char *skel_name);

  /// Build up the skeleton name.
  void compute_full_skel_name (const char *prefix,
                               char *&skel_name);

  static const char *relative_name (const char *localname,
                                    const char *othername);

  /// Call the default constructors of all the base classes.
  virtual void gen_def_ctors (TAO_OutStream* os);

  /// Generated the global hooks used for non-defined forward
  /// declared interfaces, and the contructor from stub object.
  virtual void gen_stub_ctor (TAO_OutStream* os);

  /// Generate the declarations used by the template _var, _out
  /// classes for interfaces, and by sequence template classes.
  void gen_var_out_seq_decls (void);

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
  /// CCMObject is traversed only for components regardless
  /// of the flag, it is there to disable this traversal for
  /// component servant and executor code generation.
  int traverse_inheritance_graph (
    TAO_IDL_Inheritance_Hierarchy_Worker &worker,
    TAO_OutStream *os,
    bool abstract_paths_only = false,
    bool add_ccm_object = true);

  /// Wrap the @c gen parameter and call the generic version of
  /// traverse_inheritance_graph().
  /// CCMObject is traversed only for components regardless
  /// of the flag, it is there to disable this traversal for
  /// component servant and executor code generation.
  int traverse_inheritance_graph (
    tao_code_emitter gen,
    TAO_OutStream *os,
    bool abstract_paths_only = false,
    bool add_ccm_object = true);

  /**
   * Am I in some form of multiple inheritance
   * -1 => error
   * 0 => no
   * 1 => yes
   */
  int in_mult_inheritance (void);

  /// Set a new value.
  void in_mult_inheritance (int mi);

  /// Pass along BE-specific member values when redefining a fwd decl.
  virtual void redefine (AST_Interface *from);

  /// Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing.

  DEF_NARROW_FROM_DECL (be_interface);
  DEF_NARROW_FROM_SCOPE (be_interface);

  /// Helper method passed to the template method that generates code for the
  /// is_a method.
  static int is_a_helper (be_interface *,
                          be_interface *,
                          TAO_OutStream *os);

  /// Helper method passed to the template method to generate code for the
  /// operation table.
  static int ami_handler_gen_optable_helper (be_interface *,
                                             be_interface *,
                                             TAO_OutStream *os);

  /// Helper method passed to the template method to generate code for the
  /// skeletons in the header and inline files.
  static int gen_skel_helper (be_interface *,
                              be_interface *,
                              TAO_OutStream *);

  /// Helper method passed to the template method to generate code for the
  /// collocated functions in the header file.
  static int gen_colloc_op_decl_helper (be_interface *derived,
                                        be_interface *ancestor,
                                        TAO_OutStream *os);

  /// Helper method passed to the template method to generate code for the
  /// collocated functions in the source file.
  static int gen_colloc_op_defn_helper (be_interface *derived,
                                        be_interface *ancestor,
                                        TAO_OutStream *os);

  /// Helper method passed to the template method to invoke ctors of all the
  /// base classes.
  static int copy_ctor_helper (be_interface *,
                               be_interface *,
                               TAO_OutStream *os);

  /// Helper method to determine if the interface node is involved in some kind
  /// of multiple inheritance or not. Required on the skeleton side.
  static int in_mult_inheritance_helper (be_interface *,
                                         be_interface *,
                                         TAO_OutStream *os);

  static int gen_def_ctors_helper (be_interface *node,
                                   be_interface *base,
                                   TAO_OutStream *os);

  // Helper method to generate a call to the default
  // constructors of all the base classes.

  /// Helper method to initialize the obj_ member of each generated abstract
  /// base class.
  static int gen_abstract_init_helper (be_interface *node,
                                       be_interface *base,
                                       TAO_OutStream *os);

  /// Helper method passed to traverse_inheritance_graph(),
  /// collects supported operations and attributes.
  static int op_attr_decl_helper (be_interface *node,
                                  be_interface *base,
                                  TAO_OutStream *os);

  /// Generate the operation table including entries for inherited interfaces.
  int gen_operation_table (const char *flat_name,
                           const char *skeleton_class_name);

  /// generate the operation table entries.
  int gen_optable_entries (be_interface *derived_interface,
                           const char *full_skeleton_name,
                           TAO_OutStream *os);

  /// If we are local, regenerate non-local base class operations as
  /// pure virtual.
  int convert_parent_ops (be_visitor *visitor);

  /// Common code called from gen_colloc_op_defn_helper().
  static void gen_collocated_skel_body (be_interface *derived,
                                        be_interface *ancestor,
                                        AST_Decl *d,
                                        const char *prefix,
                                        bool direct,
                                        UTL_ExceptList *list,
                                        TAO_OutStream *os);

  /// Overridden from class be_type.
  virtual void gen_ostream_operator (TAO_OutStream *os,
                                     bool use_underscore);

  /// Overridden from class be_type.
  virtual void gen_member_ostream_operator (TAO_OutStream *os,
                                            const char *instance_name,
                                            bool use_underscore,
                                            bool accessor = false);

  /// Compute whether or not we have both abstract and concrete parents,
  /// and make a list of the abstract parents, if any.
  void analyze_parentage (void);

  /// Find the next state, used to hide differences between variants of
  /// interfaces.
  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                    int is_extra_state = 0);

  /// Returns 1 if additional code needs to be generated, the behavior
  /// is driven by the strategy connected with this interface.
  int has_extra_code_generation (TAO_CodeGen::CG_STATE current_state);

  /// Sets the original interface from which this one was created,
  /// applies only to implied IDL.
  void original_interface (be_interface *original_interface);

  /// Returns the original interface from which this one was created,
  /// applies only to implied IDL
  be_interface *original_interface ();

  /// Returns an interface, which can be used instead.
  /// Needs to get set by the strategy.
  be_interface *replacement ();

  /// Do we have both abstract and concrete parents?
  int has_mixed_parentage (void);

  /// Is Components::SessionComponent an immediate parent?
  int session_component_child (void);

  /// Is EventConsumerBase our parent?
  bool is_event_consumer (void);

  /**
   * Common code for facet generation, whether we are
   * navigating from the component port or forcing
   * facet generation for all interfaces.
   */
  void gen_facet_idl (TAO_OutStream &os);
  int gen_facet_svnt_hdr (be_visitor *visitor,
                          TAO_OutStream &os);
  int gen_facet_svnt_src (be_visitor *visitor,
                          TAO_OutStream &os);

  /// Helper function called from visitors and used internally.
  void gen_nesting_open (TAO_OutStream &os);
  void gen_nesting_close (TAO_OutStream &os);

private:
  /// Output the header (type declaration and %%) to the gperf's input
  /// file.
  void gen_gperf_input_header (TAO_OutStream *ss);

  /**
   * Run GPERF and get the correct lookup and other operations
   * depending on which strategy we are using. Returns 0 on sucess, -1
   * on error.
   */
  int gen_gperf_things (const char *flat_name);

  /// Outputs the class definition for the perfect hashing. This class
  /// will inherit from the TAO_Perfect_Hash_OpTable.
  void gen_perfect_hash_class_definition (const char *flat_name);

  /// Outputs the class definition for the binary search . This class
  /// will inherit from the TAO_Binary_Search_OpTable.
  void gen_binary_search_class_definition (const char *flat_name);

  /// Outputs the class defintion for the linear search. This class
  /// will inherit from the TAO_Linear_Search.
  void gen_linear_search_class_definition (const char *flat_name);

  /// This calls the GPERF program and gets the correct operation
  /// lookup methods for the current OpLookup strategy.
  int gen_gperf_lookup_methods (const char *flat_name);

  /// Create an instance of this perfect hash table.
  void gen_perfect_hash_instance (const char *flat_name);

  /// Create an instance of the binary search optable.
  void gen_binary_search_instance (const char *flat_name);

  /// Create an instance of the linear search optable.
  void gen_linear_search_instance (const char *flat_name);

  /**
   * Called from traverse_inheritance_graph(), since base
   * components and base homes are inserted before the actual
   * traversal, it must be done tail-recursively to get the
   * chain of parents in the correct order when generating
   * copy constructors etc.
   */
  void enqueue_base_component_r (AST_Component *node);
  void enqueue_base_home_r (AST_Home *node);

protected:
  /// Have these been done already?
  int var_out_seq_decls_gen_;

protected:
  /// Number of static skeletons in the operation table.
  int skel_count_;

  /// Am I directly or indirectly involved in a multiple inheritance. If the
  /// value is -1 => not computed yet.
  int in_mult_inheritance_;

  /// Member for holding the strategy for generating names.
  be_interface_strategy *strategy_;

  /// The original interface from which this one was created,
  /// applies only to implied IDL
  be_interface *original_interface_;

  /// Do we have both abstract and concrete parents?
  int has_mixed_parentage_;

  /// Are we a direct child of Components::SessionComponent?
  int session_component_child_;
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
  virtual ~TAO_IDL_Inheritance_Hierarchy_Worker (void);

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

class be_code_emitter_wrapper
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  be_code_emitter_wrapper (be_interface::tao_code_emitter emitter);

  virtual int emit (be_interface *derived_interface,
                    TAO_OutStream *output_stream,
                    be_interface *base_interface);

private:
  be_interface::tao_code_emitter emitter_;
};

#endif  // if !defined
