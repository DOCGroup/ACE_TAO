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
//    Extension of class AST_Interface_Fwd that provides additional means for C++
//    mapping of an interface.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (BE_INTERFACE_FWD_H)
#define BE_INTERFACE_FWD_H

/*
 * BE_InterfaceFwd
 */
class be_interface_fwd : public virtual AST_InterfaceFwd,
                         public virtual be_type
{
  // =TITLE
  //  be_interface_fwd
  // =DESCRIPTION
  //  Extensions to the AST_Interface_Fwd class
public:
  // Operations

  be_interface_fwd (void);
  // default constructor

  be_interface_fwd (UTL_ScopedName *n, UTL_StrList *p);
  // constructor

  virtual int gen_var_defn (void);
  // generate the _var class definition

  virtual int gen_var_impl (void);
  // generate the implementation for the _var class

  virtual int gen_out_defn (void);
  // generate the _out class definition

  virtual int gen_out_impl (void);
  // generate the _out implementation

  virtual int gen_typecode (void);
  // generate the typecode

  virtual long tc_size (void);
  // return typecode size

  // Visiting
  virtual int accept (be_visitor* visitor);

  // Narrowing
  DEF_NARROW_METHODS2 (be_interface_fwd, AST_InterfaceFwd, be_type);
  DEF_NARROW_FROM_DECL (be_interface_fwd);

};

#endif // if !defined
