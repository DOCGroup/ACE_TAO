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
  // Operations
  be_interface (void);
  // Default constructor

  be_interface (UTL_ScopedName *n, AST_Interface **ih, long nih,
                UTL_StrList *p);
  // Constructor that sets its scoped name <n>, a list of inherited interfaces
  // <ih>, the number of inherited interfaces <nih>, and any prgmas <p>

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

  const char *full_skel_name (void);
  // retrieve the fully scoped skel class name

  virtual int gen_typecode (void);
  // generate the typecode

  virtual long tc_encap_len (void);
  // return the total byte length of ourselves represented as an encapsulation

  // Narrowing
  DEF_NARROW_METHODS3 (be_interface, AST_Interface, be_scope, be_type);
  DEF_NARROW_FROM_DECL (be_interface);
  DEF_NARROW_FROM_SCOPE (be_interface);

private:
  void compute_fullskelname (void);
  // compute the fully scoped skel class name

  // helper methods for the C++ mapping process
  int gen_operation_table (void);

  char *full_skel_name_;  // fully scoped skeleton name
};

#endif  // if !defined
