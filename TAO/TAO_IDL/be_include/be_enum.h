/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_enum.h
//
// = DESCRIPTION
//    Extension of class AST_Enum that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (BE_ENUM_H)
#define BE_ENUM_H

/*
 * BE_Enum
 */
class be_enum : public virtual AST_Enum,
                public virtual be_scope,
                public virtual be_type
{
public:
  // Operations

  be_enum (void);
  // default constructor

  be_enum (UTL_ScopedName *n, UTL_StrList *p);
  // constructor

  virtual int gen_client_header (void);
  // Generates the client-side header information for the enum 

  virtual int gen_client_stubs (void);
  // Generates the client-side stubs for the enum

  virtual int gen_server_header (void);
  // Generates the server-side header information for the enum 

  virtual int gen_server_skeletons (void);
  // Generates the server-side skeletons for the enum

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the enum

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the enum

  virtual int gen_typecode (void);
  // generate the typecode

  virtual int gen_encapsulation (void);
  // encapsulation for parameters

  virtual long tc_size (void);
  // return typecode size

  virtual long tc_encap_len (void);
  // return length of encapsulation

  virtual int member_count (void);
  // return the count of members

  // Narrowing
  DEF_NARROW_METHODS3 (be_enum, AST_Enum, be_scope, be_type);
  DEF_NARROW_FROM_DECL (be_enum);
  DEF_NARROW_FROM_SCOPE (be_enum);

private:
  //=helper

  int compute_member_count (void);
  // count the number of members

  int member_count_;
  // number of members
};

#endif
