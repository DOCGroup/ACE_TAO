/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_enum_val.h
//
// = DESCRIPTION
//    Extension of class AST_EnumVal that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (BE_ENUM_VAL_H)
#define BE_ENUM_VAL_H

/*
 * BE_EnumVal
 */
class be_enum_val : public virtual AST_EnumVal,
                    public virtual be_decl
{
public:
  // =Operations

  be_enum_val (void);
  // default constructor

  be_enum_val (unsigned long v, UTL_ScopedName *n, UTL_StrList *p);
  // constructor

  virtual int gen_client_header (void);
  // Generates the client-side header information for the EnumVal 

  virtual int gen_client_stubs (void);
  // Generates the client-side stubs for the EnumVal

  virtual int gen_server_header (void);
  // Generates the server-side header information for the EnumVal 

  virtual int gen_server_skeletons (void);
  // Generates the server-side skeletons for the EnumVal

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the EnumVal

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the EnumVal

  virtual int gen_typecode (void);
  // generate the typecode

  virtual long tc_encap_len (void);
  // return the total byte length of ourselves represented as an encapsulation

  // Narrowing
  DEF_NARROW_METHODS2 (be_enum_val, AST_EnumVal, be_decl);
  DEF_NARROW_FROM_DECL (be_enum_val);
};

#endif
