/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_decl.h
//
// = DESCRIPTION
//    Extension of the AST_Decl class. 
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (TAO_BE_DECL_H)
#define TAO_BE_DECL_H

/*
 * BE_Decl
 */
class be_decl : public virtual AST_Decl
{
  // = TITLE
  //   be_decl
  // = DESCRIPTION
  //   The back end extension of the AST_Decl class. Provides an abstract
  //   interface 
  //
public:
  enum SIZE_TYPE
  {
    FIXED,
    VARIABLE
  };
  // indicates if we are fixed size or variable. Most useful for structs,
  // unions, and arrays.

  // =Operations

  be_decl (void);
  // Default constructor

  be_decl (AST_Decl::NodeType type, UTL_ScopedName *n, UTL_StrList *pragmas);
  // constructor that sets the node type

  ~be_decl (void);
  // destructor

  virtual int gen_var_defn (void);
  // generate the _var class definition

  virtual int gen_var_impl (void);
  // generate the implementation for the _var class

  virtual int gen_out_defn (void);
  // generate the _out class definition

  virtual int gen_out_impl (void);
  // generate the _out implementation

  virtual int gen_client_header (void) = 0;
  // Generates the client-side header information for the decl 

  virtual int gen_client_stubs (void) = 0;
  // Generates the client-side stubs for the decl

  virtual int gen_server_header (void) = 0;
  // Generates the server-side header information for the decl 

  virtual int gen_server_skeletons (void) = 0;
  // Generates the server-side skeletons for the decl

  virtual int gen_client_inline (void) = 0;
  // Generates the client-side inline for the decl

  virtual int gen_server_inline (void) = 0;
  // Generates the server-side inlines for the decl

  virtual idl_bool lookup_seq_name (Identifier *);
  // lookup a name inside a list of generated seq names

  virtual idl_bool add_seq_name (Identifier *);
  // add a generated seq name to a list. 

  virtual void size_type (SIZE_TYPE);
  // set the size type

  virtual SIZE_TYPE size_type (void) const;
  // return our size type

  const char *repoID (void);
  // retrieve the repository ID

  const char *fullname (void);
  // return the stringified full name

  const char *flatname (void);
  // return the flattened full scoped name

  virtual idl_bool is_nested (void);
  // determines if we are inside of a nested scope or not

  virtual int gen_encapsulation (void);
  // encapsulation of parameters

  virtual long tc_encap_len (void);
  // return length of encapsulation

  // Narrowing
  DEF_NARROW_METHODS1 (be_decl, AST_Decl);
  DEF_NARROW_FROM_DECL (be_decl);

protected:
  // =helper

  virtual void compute_repoID (void);
  // computes the repoID

  virtual void compute_fullname (void);
  // computes the fully scoped name

  virtual void compute_flatname (void);
  // compute the flattened fully scoped name 

  virtual int tc_name2long (const char *name, long *&, long &);
  // name represented as a padded array of longs
 
  virtual long repoID_encap_len (void);
  // return encapsulation length required to hold repository ID

  virtual long name_encap_len (void);
  // return encapsulation length required to hold IDL name

  // variables that indicate if the code generation for that node is already
  // been done. This way we avoid regenerating same code.
  idl_bool cli_hdr_gen_;
  idl_bool cli_stub_gen_;
  idl_bool cli_inline_gen_;
  idl_bool srv_hdr_gen_;
  idl_bool srv_skel_gen_;
  idl_bool srv_inline_gen_;

  UTL_IdList *seq_names_;
  // list of generated sequence names in the current scope

  char *fullname_;
  // our full scoped name

  char *flatname_;
  // flattened fully scoped name

  char *repoID_;
  // repository ID

  SIZE_TYPE size_type_;
  // whether we are fixed or variable size (by default fixed)

  long encap_len_;
  // encapsulation length - required for typecodes

};

#endif // if !defined
