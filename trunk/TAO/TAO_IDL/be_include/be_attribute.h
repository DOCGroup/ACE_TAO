// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_attribute.h
//
// = DESCRIPTION
//    Extension of class AST_Attribute that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (BE_ATTRIBUTE_H)
#define BE_ATTRIBUTE_H

/*
 * BE_Attribute
 */
class be_attribute : public virtual AST_Attribute,
                     public virtual be_decl
{
public:
  // =Operations

  be_attribute (void);
  // default constructor

  be_attribute (idl_bool ro, AST_Type *ft, UTL_ScopedName *n, UTL_StrList *p);
  // constructor

  virtual int gen_client_header (void);
  // generate client header for the attribute

  virtual int gen_client_stubs (void);
  // generate client side stubs for the attribute

  virtual int gen_server_header (void);
  // generate server header for the attribute

  virtual int gen_server_skeletons (void);
  // generate server skeletons for the attribute

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the attribute

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the attribute

  // Narrowing
  DEF_NARROW_METHODS2 (be_attribute, AST_Attribute, be_decl);
  DEF_NARROW_FROM_DECL (be_attribute);
};

#endif
