// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_union_branch.h
//
// = DESCRIPTION
//    Extension of class AST_UnionBranch that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (BE_UNION_BRANCH_H)
#define BE_UNION_BRANCH_H

/*
 * BE_UnionBranch
 */
class be_union_branch : public virtual AST_UnionBranch,
                        public virtual be_decl
{
public:
  // Operations
  be_union_branch (void);
  // default constructor

  be_union_branch (AST_UnionLabel *lab, AST_Type *ft, UTL_ScopedName *n,
                   UTL_StrList *p);
  // constructor

  virtual int gen_client_header (void);
  // Generates the client-side header information for the union branch

  virtual int gen_client_stubs (void);
  // Generates the client-side stubs for the union branch

  virtual int gen_server_header (void);
  // Generates the server-side header information for the union branch

  virtual int gen_server_skeletons (void);
  // Generates the server-side skeletons for the union branch

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the union branch

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the union branch

  virtual int gen_encapsulation (void);
  // generate the encapsulation

  virtual long tc_encap_len (void);
  // return the total byte length of ourselves represented as an encapsulation

  // Narrowing
  DEF_NARROW_METHODS2 (be_union_branch, AST_UnionBranch, be_decl);
  DEF_NARROW_FROM_DECL (be_union_branch);
};

#endif
