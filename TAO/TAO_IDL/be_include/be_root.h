// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_root.h
//
// = DESCRIPTION
//    Extension of class AST_Root that provides additional means for C++
//    mapping of an interface.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (TAO_BE_ROOT_H)
#define TAO_BE_ROOT_H

class be_root : public virtual AST_Root
  // = TITLE
  //   The back end extension of the AST_Root class
  //
  // = DESCRIPTION
  //
{
public:
  // Operations
  be_root();
  // Default constructor

  be_root(UTL_ScopedName *n, UTL_StrList *p);
  // Constructor that sets its scoped name <n>, and any pragmas <p>

  long gen_idl2cplusplus_mapping();
  // generates the IDL to C++ mapping

  long gen_client_header();
  // Generates the client-side header information for the interface 

  long gen_client_stubs();
  // Generates the client-side stubs for the interface

  long gen_server_header();
  // Generates the server-side header information for the interface 

  long gen_server_skeletons();
  // Generates the server-side skeletons for the interface

  // Narrowing
  DEF_NARROW_METHODS1(be_root, AST_root);
  DEF_NARROW_FROM_DECL(be_root);
  DEF_NARROW_FROM_SCOPE(be_root);
};

#endif
