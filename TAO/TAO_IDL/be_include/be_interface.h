/* -*- c++ -*- */
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
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_BE_INTERFACE_H)
#define TAO_BE_INTERFACE_H

class TAO_OutStream;

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

  // used to pass functions to the template method
  typedef int (*tao_code_emitter) (be_interface *, be_interface *, TAO_OutStream *);

  // Operations
  be_interface (void);
  // Default constructor

  be_interface (UTL_ScopedName *n, AST_Interface **ih, long nih,
                UTL_StrList *p);
  // Constructor that sets its scoped name <n>, a list of inherited interfaces
  // <ih>, the number of inherited interfaces <nih>, and any prgmas <p>

  ~be_interface (void);
  // dtor

  virtual int gen_client_header (void);
  // Generates the client-side header information for the interface

  virtual int gen_client_stubs (void);
  // Generates the client-side stubs for the interface

  virtual int gen_server_header (void);
  // Generates the server-side header information for the interface

  virtual int gen_server_skeletons (void);
  // Generates the server-side skeletons for the interface

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the interface

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the interface

  virtual int gen_var_defn (void);
  // generate the _var class definition

  virtual int gen_var_impl (void);
  // generate the implementation for the _var class

  virtual int gen_out_defn (void);
  // generate the _out class definition

  virtual int gen_out_impl (void);
  // generate the _out implementation

  const char *full_skel_name (void);
  // retrieve the fully scoped skel class name

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
  const char *full_coll_name (void) const;
  // retrieve the fully qualified collocated class name

  virtual int gen_typecode (void);
  // generate the typecode

  virtual int gen_encapsulation (void);
  // encapsulation for parameters

  virtual long tc_size (void);
  // return typecode size

  virtual long tc_encap_len (void);
  // return length of encapsulation

  virtual int traverse_inheritance_graph (tao_code_emitter gen,
                                          TAO_OutStream *os);
  // template method using breadth first traversal of inheritance graph

  virtual int write_as_return (TAO_OutStream *stream,
			       be_type *type);
  // write the mapping as a return value on the stream.

  // Visiting
  virtual int accept (be_visitor* visitor);

  // Narrowing
  DEF_NARROW_METHODS3 (be_interface, AST_Interface, be_scope, be_type);
  DEF_NARROW_FROM_DECL (be_interface);
  DEF_NARROW_FROM_SCOPE (be_interface);

  static int is_a_helper (be_interface *, be_interface *, TAO_OutStream *os);
  // helper method passed to the template method

  static int gen_optable_helper (be_interface *,
                                 be_interface *,
                                 TAO_OutStream *os);
  // helper method passed to the template method

  static int gen_skel_helper (be_interface *,
                              be_interface *,
                              TAO_OutStream *os);
  // helper method passed to the template method

private:
  void compute_fullskelname (void);
  // compute the fully scoped skel class name

  int gen_operation_table (void);
  // generate the operation table including entries for inherited interfaces

  int gen_optable_entries (be_interface *);
  // generate the operation table entries

  void compute_coll_name (void);
  // compute the fully qualified collocated class name.

private:
  char *full_skel_name_;  // fully scoped skeleton name

  int skel_count_; // number of static skeletons in the operation table

  char *full_coll_name_;
};

#endif  // if !defined
