// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_type.h
//
// = DESCRIPTION
//    Extension of class AST_Type that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (BE_TYPE_H)
#define BE_TYPE_H

/*
 * BE_Type
 */
class be_type : public virtual AST_Type,
                public virtual be_decl
{
public:
  // =Operations

  be_type (void);
  // default constructor

  be_type (AST_Decl::NodeType nt, UTL_ScopedName *n, UTL_StrList *p);
  // constructor

  virtual int gen_client_header (void);
  // Generates the client-side header information for the type 

  virtual int gen_client_stubs (void);
  // Generates the client-side stubs for the type

  virtual int gen_server_header (void);
  // Generates the server-side header information for the type 

  virtual int gen_server_skeletons (void);
  // Generates the server-side skeletons for the type

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the type

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the type

  virtual int gen_typecode (void);
  // generate the typecode data structure

  virtual long tc_encap_len (void);
  // return the total byte length of ourselves represented as an encapsulation

  // Narrowing
  DEF_NARROW_METHODS2 (be_type, AST_Type, be_decl);
  DEF_NARROW_FROM_DECL (be_type);

private: 
  // helpers
  int gen_predefined_type (void);
  int gen_string (void);
  int gen_array (void);
  int gen_sequence (void);
  int gen_enum (void);
  int gen_struct (void);
  int gen_union (void);
  int gen_typedef (void);
  int gen_interface (void);
};

#endif
