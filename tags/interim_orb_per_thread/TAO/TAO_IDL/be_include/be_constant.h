/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_constant.h
//
// = DESCRIPTION
//    Extension of class AST_Constant that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (BE_CONSTANT_H)
#define BE_CONSTANT_H

/*
 * BE_Constant
 */
class be_constant : public virtual AST_Constant , public virtual be_decl
{
public:
  // Operations

  be_constant (void);
  // constructor
  
  be_constant (AST_Expression::ExprType et,
               AST_Expression *v,
               UTL_ScopedName *n,
               UTL_StrList *p);
  // constructor

  virtual int gen_client_header (void);
  // Generates the client-side header information for the constant 

  virtual int gen_client_stubs (void);
  // Generates the client-side stubs for the constant

  virtual int gen_server_header (void);
  // Generates the server-side header information for the constant 

  virtual int gen_server_skeletons (void);
  // Generates the server-side skeletons for the constant

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the constant

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the constant

  // Narrowing
  DEF_NARROW_METHODS2 (be_constant, AST_Constant, be_decl);
  DEF_NARROW_FROM_DECL (be_constant);

private:
  char *exprtype_to_string (void);
  // returns the appropriate type
};

#endif // if !defined
