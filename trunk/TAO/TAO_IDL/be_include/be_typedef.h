// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_typedef.h
//
// = DESCRIPTION
//    Extension of class AST_typedef that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (BE_TYPEDEF_H)
#define BE_TYPEDEF_H

/*
 * BE_Typedef
 */
class be_typedef : public virtual AST_Typedef,
                   public virtual be_type
{
public:
  // =Operations

  be_typedef (void);
  // default constructor

  be_typedef (AST_Type *bt, UTL_ScopedName *n, UTL_StrList *p);
  // constructor

  virtual int gen_client_header (void);
  // Generates the client-side header information for the typedef 

  virtual int gen_client_stubs (void);
  // Generates the client-side stubs for the typedef

  virtual int gen_server_header (void);
  // Generates the server-side header information for the typedef 

  virtual int gen_server_skeletons (void);
  // Generates the server-side skeletons for the typedef

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the typedef

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the typedef

  virtual int gen_typecode (void);
  // generate the typecode

  // Narrowing
  DEF_NARROW_METHODS2 (be_typedef, AST_Typedef, be_type);
  DEF_NARROW_FROM_DECL (be_typedef);

};

#endif
