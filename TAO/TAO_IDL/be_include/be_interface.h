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
class be_interface : public virtual AST_Interface
  // = TITLE
  //   The back end extension of the AST_Interface class
  //
  // = DESCRIPTION
  //
{
public:
  // Operations
  be_interface();
  // Default constructor

  be_interface(UTL_ScopedName *n, AST_Interface **ih, long nih,
	       UTL_StrList *p);
  // Constructor that sets its scoped name <n>, a list of inherited interfaces
  // <ih>, the number of inherited interfaces <nih>, and any prgmas <p>

  void gen_client_header();
  // Generates the client-side header information for the interface 

  void gen_client_impl();
  // Generates the client-side implementation for the interface

  void gen_server_header();
  // Generates the server-side header information for the interface 

  void gen_server_impl();
  // Generates the server-side implementation for the interface

  // Narrowing
  DEF_NARROW_METHODS1(be_interface, AST_Interface);
  DEF_NARROW_FROM_DECL(be_interface);
  DEF_NARROW_FROM_SCOPE(be_interface);
};

#endif
