/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_valuetype.h
//
// = DESCRIPTION
//    Extension of class AST_Interface and be_interface that provides
//    additional means for C++ mapping of an valuetype.
//
// ============================================================================

#ifndef TAO_BE_VALUETYPE_H
#define TAO_BE_VALUETYPE_H

#ifdef IDL_HAS_VALUETYPE

class TAO_OutStream;

/*
 * BE_Valuetype
 */
class be_valuetype : public virtual be_interface
{
  // = TITLE
  //   Backend-class for valuetypes
  //
  // = DESCRIPTION
  //   Extends be_interface and overrides member functions for valuetype
  //   mapping.
  //
public:

  // used to pass functions to the template method
  //: typedef int (*tao_code_emitter) (be_interface *, be_interface *, TAO_OutStream *);

  // Operations
  be_valuetype (void);
  // Default constructor

  be_valuetype (UTL_ScopedName *n, AST_Interface **ih, long nih,
                UTL_StrList *p, idl_bool set_abstract = 0);
  // Constructor that sets its scoped name <n>, a list of inherited valuetypes
  // and supported interfaces <ih>, the number of inherited interfaces <nih>,
  // and any pragmas <p>

  ~be_valuetype (void);
  // dtor

  virtual void redefine (be_valuetype *from, UTL_StrList *p);

  idl_bool opt_accessor ();
  // should generate optimized form ?

  virtual int gen_var_defn (void);
  // generate the _var class definition

  // virtual void gen_def_ctors (TAO_OutStream* os);
  //call the default constructors of all the base classes



  // virtual void gen_copy_ctors (TAO_OutStream* os);
  //call the copy constructors of all the base classes

  virtual int gen_var_impl (void);
  // generate the implementation for the _var class

  virtual int gen_out_defn (void);
  // generate the _out class definition

  virtual int gen_out_impl (void);
  // generate the _out implementation

  const char *full_obv_skel_name (void);
  // retrieve the fully scoped skel class name


  virtual int gen_typecode (void);
  // generate the typecode

  virtual int gen_encapsulation (void);
  // encapsulation for parameters

  virtual long tc_size (void);
  // return typecode size

  virtual long tc_encap_len (void);
  // return length of encapsulation

  // virtual int traverse_inheritance_graph (tao_code_emitter gen,
  //                                         TAO_OutStream *os);
  // template method using breadth first traversal of inheritance graph

  //: const char *relative_skel_name (const char *other_class_name);
  // relative skeleton name

  const char *field_pd_prefix ();
  const char *field_pd_postfix ();
  // for building the name for private data fields
  be_valuetype *statefull_inherit ();


  // Visiting
  virtual int accept (be_visitor *visitor);

  virtual idl_bool is_valuetype ();
  virtual idl_bool is_abstract_valuetype ();
  virtual void set_abstract_valuetype ();

 // Narrowing
  DEF_NARROW_METHODS1 (be_valuetype, be_interface);
  DEF_NARROW_FROM_DECL (be_valuetype);
  DEF_NARROW_FROM_SCOPE (be_valuetype);

  void compute_fullobvskelname (void);
  // compute the fully scoped skel class name


private:
  char * full_obv_skel_name_;
  idl_bool abstract_;
};

#endif  /* IDL_HAS_VALUETYPE */

#endif  // if !defined
