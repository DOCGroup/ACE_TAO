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

  virtual int gen_typecode (void);
  // generate the typecode

  virtual long tc_size (void);
  // return typecode size

  // Narrowing
  DEF_NARROW_METHODS2 (be_interface_fwd, AST_InterfaceFwd, be_type);
  DEF_NARROW_FROM_DECL (be_interface_fwd);
};

#endif // if !defined
