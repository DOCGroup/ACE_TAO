// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_union.h
//
// = DESCRIPTION
//    Extension of class AST_Union that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (BE_UNION_H)
#define BE_UNION_H

/*
 * BE_Union
 */
class be_union : public virtual AST_Union,
                 public virtual be_scope,
                 public virtual be_type
{
public:
  // =Operations

  be_union (void);
  // default constructor

  be_union(AST_ConcreteType *dt, UTL_ScopedName *n, UTL_StrList *p);
  // constructor

  virtual int gen_client_header (void);
  // Generates the client-side header information for the union 

  virtual int gen_client_stubs (void);
  // Generates the client-side stubs for the union

  virtual int gen_server_header (void);
  // Generates the server-side header information for the union 

  virtual int gen_server_skeletons (void);
  // Generates the server-side skeletons for the union

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the union

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the union

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

  virtual int default_index (void);
  // return the default index used

  // Narrowing
  DEF_NARROW_METHODS3 (be_union, AST_Union, be_scope, be_type);
  DEF_NARROW_FROM_DECL(be_union);
  DEF_NARROW_FROM_SCOPE(be_union);

private:
  //=helper

  int compute_member_count (void);
  // count the number of members

  int compute_default_index (void);
  // count the default index

  int member_count_;
  // number of members

  int default_index_;
  // default label index (zero based indexing)
};

#endif
