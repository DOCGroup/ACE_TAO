// -*- C++ -*-

//=============================================================================
/**
 *  @file    be_interface.h
 *
 *  Extension of class AST_Interface that provides additional means for C++
 *  mapping of an interface.
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

#include <string>
#include <string_view>

class TAO_OutStream;
class TAO_IDL_Inheritance_Hierarchy_Worker;
class be_visitor;
//class be_interface_strategy;

class UTL_ExceptList;

/**
 * The back end extension of the AST_Interface class
 */
class be_interface : public virtual AST_Interface,
                     public virtual be_scope,
                     public virtual be_type
{
public:
  // Used to pass functions to the template method.
  using tao_code_emitter = int (*) (be_interface *,
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
  ~be_interface ();

  // Methods, which access the strategy.

  /// Return the local name.
  std::string const& local_name () const;

  /// Retrieve the fully scoped skel class name.
  std::string_view full_skel_name () const;

  /// Retrieve the fully qualified collocated class name.
  std::string_view full_coll_name (int) const;

  /// Retrieve the fully qualified collocated class name.
  std::string_view local_coll_name (int) const;

  /// retrieve the name of the direct proxy implementation.
  virtual std::string_view direct_proxy_impl_name () const;

  /// retrieve the fully qualified name of the direct proxy
  /// implementation.
  virtual std::string_view full_direct_proxy_impl_name () const;

  /// Return the client scope that encloses the interface.
  virtual std::string_view client_enclosing_scope () const;

  /// Return the "flattened" scope that encloses
  /// the interface.
  virtual std::string_view flat_client_enclosing_scope () const;

  /// Return the server scope that encloses the interface.
  virtual std::string_view server_enclosing_scope () const;

  /// Retrieve skeleton name.
  std::string relative_skel_name (std::string_view skel_name) const;

  /// Build up the skeleton name.
  std::string compute_full_skel_name (std::string_view prefix) const;

  /// Compute the collocation names.
  void compute_coll_names (int type) const;

  static std::string relative_name (std::string_view localname,
                                    std::string_view othername);

  /// Call the default constructors of all the base classes.
  virtual void gen_def_ctors (TAO_OutStream* os);

  /// Generated the global hooks used for non-defined forward
  /// declared interfaces, and the constructor from stub object.
  virtual void gen_stub_ctor (TAO_OutStream* os);

  /// Generate the declarations used by the template _var, _out
  /// classes for interfaces, and by sequence template classes.
  void gen_var_out_seq_decls ();

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
  int in_mult_inheritance ();

  /// Set a new value.
  void in_mult_inheritance (int mi);

  /// Applies to interfaces, components, and homes.
  bool has_rw_attributes () const;

  /// Pass along BE-specific member values when redefining a fwd decl.
  void redefine (AST_Interface *from) override;

  /// Cleanup function.
  void destroy () override;

  // Visiting.
  int accept (be_visitor *visitor) override;

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
  int gen_operation_table (std::string_view flat_name,
                           std::string_view skeleton_class_name);

  /// generate the operation table entries.
  int gen_optable_entries (be_interface *derived_interface,
                           std::string_view full_skeleton_name,
                           TAO_OutStream *os);

  /// If we are local, regenerate non-local base class operations as
  /// pure virtual.
  int convert_parent_ops (be_visitor *visitor);

  /// Overridden from class be_type.
  void gen_ostream_operator (TAO_OutStream *os,
                             bool use_underscore) override;

  /// Overridden from class be_type.
  void gen_member_ostream_operator (TAO_OutStream *os,
                                   std::string_view instance_name,
                                   bool use_underscore,
                                   bool accessor = false) override;

  /// Sets the original interface from which this one was created,
  /// applies only to implied IDL.
  void original_interface (be_interface *original_interface);

  /// Returns the original interface from which this one was created,
  /// applies only to implied IDL
  be_interface *original_interface ();

  /// Is EventConsumerBase our parent?
  bool is_event_consumer () const;

  /**
   * Common code for facet generation, whether we are
   * navigating from the component port or forcing
   * facet generation for all interfaces.
   */
  void gen_facet_idl (TAO_OutStream &os);

  /// Used with ami4ccm.
  int gen_ami4ccm_idl (TAO_OutStream *os);

  bool is_ami_rh () const;
  void is_ami_rh (bool val);

  bool is_ami4ccm_rh () const;
  void is_ami4ccm_rh (bool val);

  /// Accessors for the member.
  bool dds_connector_traits_done () const;
  void dds_connector_traits_done (bool val);

  /// Generate the "public virtual" entries in the stub header.
  virtual void gen_stub_inheritance (TAO_OutStream *os);

  /// Generate the "public virtual" entries in the skel header.
  virtual void gen_skel_inheritance (TAO_OutStream *os);

  /// Generate the string compares for ancestors in _is_a().
  virtual int gen_is_a_ancestors (TAO_OutStream *os);

protected:
  /**
   * CDreate a new string made by the concatenation
   * of "str" and "suffix" and using the
   * "separator" to concatenate the two.
   */
  std::string create_with_prefix_suffix (std::string_view prefix,
                                         std::string_view str,
                                         std::string_view suffix,
                                         std::string_view separator = "") const;

protected:
  enum Suffix_Code
  {
    PROXY_IMPL = 0
  };

  enum Tag_Code
  {
    DIRECT = 0,
    GC_PREFIX = 1 // Prefix used for the generated class
                  // This prefix is used to avoid name conflicts
                  // with the user classes.
  };

  inline static constexpr std::string_view suffix_table_[] = {"_Proxy_Impl"};
  inline static constexpr std::string_view tag_table_[] = {"_Direct", "_TAO_"};

  // Proxy Implementation names.
  mutable std::string direct_proxy_impl_name_;

  mutable std::string full_direct_proxy_impl_name_;

  mutable std::string client_scope_;
  mutable bool client_scope_computed_ {};
  mutable std::string flat_client_scope_;
  mutable bool flat_client_scope_computed_ {};

  mutable std::string server_scope_;
  mutable bool server_scope_computed_ {};

private:
  /// Output the header (type declaration and %%) to the gperf's input
  /// file.
  void gen_gperf_input_header (TAO_OutStream *ss);

  /**
   * Run GPERF and get the correct lookup and other operations
   * depending on which strategy we are using. Returns 0 on sucess, -1
   * on error.
   */
  int gen_gperf_things (std::string const &flat_name);

  /// Outputs the class definition for the perfect hashing. This class
  /// will inherit from the TAO_Perfect_Hash_OpTable.
  void gen_perfect_hash_class_definition (std::string const &flat_name);

  /// Outputs the class definition for the binary search . This class
  /// will inherit from the TAO_Binary_Search_OpTable.
  void gen_binary_search_class_definition (std::string const &flat_name);

  /// Outputs the class definition for the linear search. This class
  /// will inherit from the TAO_Linear_Search.
  void gen_linear_search_class_definition (std::string const &flat_name);

  /// This calls the GPERF program and gets the correct operation
  /// lookup methods for the current OpLookup strategy.
  int gen_gperf_lookup_methods (std::string const &flat_name);

  /// Create an instance of this perfect hash table.
  void gen_perfect_hash_instance (std::string const &flat_name);

  /// Create an instance of the binary search optable.
  void gen_binary_search_instance (std::string const &flat_name);

  /// Create an instance of the linear search optable.
  void gen_linear_search_instance (std::string const &flat_name);

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
  bool var_out_seq_decls_gen_ {};

protected:
  /// Number of static skeletons in the operation table.
  int skel_count_ {};

  /// Am I directly or indirectly involved in a multiple inheritance. If the
  /// value is -1 => not computed yet.
  int in_mult_inheritance_ {-1};

  /// The original interface from which this one was created,
  /// applies only to implied IDL
  be_interface *original_interface_ {};

  /// Are we an AMH reply handler?
  bool is_amh_rh_ {};

  /// Are we an AMI reply handler?
  bool is_ami_rh_ {};

  /// Are we an AMI4CCM reply handler?
  bool is_ami4ccm_rh_ {};

  mutable std::string full_skel_name_;

  mutable std::string full_coll_name_;

  mutable std::string local_coll_name_;

  /// Current cached collocated name.
  mutable int cached_type_ {-1};

  /// Used to suppress generation of some CCM servant methods/
  bool has_rw_attributes_ {};

  /// Flag to ensure that DDS connector impl traits members
  /// are generated only once per port interface.
  bool dds_connector_traits_done_ {};
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

class be_code_emitter_wrapper
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  explicit be_code_emitter_wrapper (be_interface::tao_code_emitter emitter);

  virtual int emit (be_interface *derived_interface,
                    TAO_OutStream *output_stream,
                    be_interface *base_interface) override;

private:
  be_interface::tao_code_emitter emitter_;
};

#endif  // if !defined
