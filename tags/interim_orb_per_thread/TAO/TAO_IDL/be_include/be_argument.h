// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_argument.h
//
// = DESCRIPTION
//    Extension of class AST_Argument that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (BE_ARGUMENT_H)
#define BE_ARGUMENT_H

/*
 * BE_Argument
 */
class be_argument : public virtual AST_Argument,
                    public virtual be_decl
{
public:
  // =Operations

  be_argument (void);
  // default constructor

  be_argument (AST_Argument::Direction d, AST_Type *ft, UTL_ScopedName *n,
               UTL_StrList *p);
  // constructor

  virtual int gen_client_header (void);
  // generate client header for the argument

  virtual int gen_client_stubs (void);
  // generate client side stubs for the argument

  virtual int gen_server_header (void);
  // generate server header for the argument

  virtual int gen_server_skeletons (void);
  // generate server skeletons for the argument

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the argument

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the argument

  // Narrowing
  DEF_NARROW_METHODS2 (be_argument, AST_Argument, be_decl);
  DEF_NARROW_FROM_DECL (be_argument);
};

#endif
