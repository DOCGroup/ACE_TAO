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

#include "be_interface.h"

class TAO_OutStream;

class be_valuetype : public virtual be_interface
{
  // = TITLE
  //    Backend-class for valuetypes
  //
  // = DESCRIPTION
  //    Extends be_interface and overrides member functions for valuetype
  //    mapping.
  //
public:
  be_valuetype (void);
  // Default constructor.

  be_valuetype (UTL_ScopedName *n, 
                AST_Interface **ih, 
                long nih,
                UTL_StrList *p, 
                idl_bool set_abstract = 0);
  // Constructor that sets its scoped name <n>, a list of inherited valuetypes
  // and supported interfaces <ih>, the number of inherited interfaces <nih>,
  // and any pragmas <p>.

  ~be_valuetype (void);
  // Destructor.

  virtual void redefine (AST_Interface *from, 
                         UTL_StrList *p);

  idl_bool opt_accessor (void);
  // Should generate optimized form?

  virtual int gen_var_defn (char *local_name = 0);
  // Generate the _var class definition.

  virtual int gen_var_impl (char *local_name = 0,
                            char *full_name = 0);
  // Generate the implementation for the _var class.

  virtual int gen_out_defn (char *local_name = 0);
  // Generate the _out class definition.

  virtual int gen_out_impl (char *local_name = 0,
                            char *full_name = 0);
  // Generate the _out implementation.

  const char *full_obv_skel_name (void);
  // Retrieve the fully scoped skel class name.

  // TODO.
#if 0
  virtual int gen_typecode (void);
  // Generate the typecode.

  virtual int gen_encapsulation (void);
  // Encapsulation for parameters.

  virtual long tc_size (void);
  // Return typecode size.

  virtual long tc_encap_len (void);
  // Return length of encapsulation.
#endif

  const char *field_pd_prefix (void);

  const char *field_pd_postfix (void);

  // For building the name for private data fields.
  be_valuetype *statefull_inherit (void);


  // Visiting.
  virtual int accept (be_visitor *visitor);

 // Narrowing.
  DEF_NARROW_METHODS1 (be_valuetype, be_interface);
  DEF_NARROW_FROM_DECL (be_valuetype);
  DEF_NARROW_FROM_SCOPE (be_valuetype);

  void compute_fullobvskelname (void);
  // Compute the fully scoped skel class name.


private:
  char *full_obv_skel_name_;
};

#endif  /* IDL_HAS_VALUETYPE */

#endif  // if !defined
