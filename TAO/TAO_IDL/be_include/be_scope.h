/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_scope.h
//
// = DESCRIPTION
//    Extension of the UTL_Scope CFE class
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (TAO_BE_SCOPE_H)
#define TAO_BE_SCOPE_H

/*
 * BE_Scope
 */
class be_scope : public virtual UTL_Scope
{
  // = TITLE
  //   be_scope
  // = DESCRIPTION
  //   The back end extension of the UTL_Scope class
  //
public:
  // Operations

  be_scope (void);
  // Default constructor

  be_scope (AST_Decl::NodeType nt);
  // constructor that sets the node type

  virtual ~be_scope (void);
  // destructor

  virtual int gen_client_header (void);
  // Generates the client-side header information for the scope 

  virtual int gen_client_stubs (void);
  // Generates the client-side stubs for the scope

  virtual int gen_server_header (void);
  // Generates the server-side header information for the scope 

  virtual int gen_server_skeletons (void);
  // Generates the server-side skeletons for the scope

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the scope

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the scope

  virtual int gen_typecode (void);
  // generate the typecode

  virtual long tc_encap_len (void);
  // return the total byte length of ourselves represented as an encapsulation

  // Narrowing
  DEF_NARROW_METHODS1 (be_scope, UTL_Scope);
  DEF_NARROW_FROM_SCOPE (be_scope);
};

#endif // if !defined
