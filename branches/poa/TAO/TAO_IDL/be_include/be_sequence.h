// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_sequence.h
//
// = DESCRIPTION
//    Extension of class AST_Sequence that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (BE_SEQUENCE_H)
#define BE_SEQUENCE_H

/*
 * BE_Sequence
 */

// A sequence in OMG IDL does not define a scoping construct just as a struct
// or union or an interface do. However, in the C++ mapping, a sequence becomes
// a class. If the base type of a sequence is another anonymous sequence, then
// the base type is defined in the scope of this sequence. Hence we define
// be_sequence to possess the additional characteristics of a scope
class be_sequence : public virtual AST_Sequence,
                    public virtual be_scope,
                    public virtual be_type
{
public:
  enum MANAGED_TYPE
  {
    MNG_UNKNOWN ,
    MNG_NONE,
    MNG_STRING,
    MNG_OBJREF
  };

  // =Operations

  be_sequence (void);
  // default constructor

  be_sequence (AST_Expression *v, AST_Type *bt);
  // constructor

  virtual int create_name (void);
  // create a name for ourselves. If we are typedefed, then we get the name of
  // the typedef node, else we generate a name for ourselves

  virtual int gen_client_header (void);
  // Generates the client-side header information for the sequence

  virtual int gen_client_stubs (void);
  // Generates the client-side stubs for the sequence

  virtual int gen_server_header (void);
  // Generates the server-side header information for the sequence

  virtual int gen_server_skeletons (void);
  // Generates the server-side skeletons for the sequence

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the sequence

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the sequence

  virtual int gen_var_defn (void);
  // generate the _var class definition

  virtual int gen_var_impl (void);
  // generate the implementation for the _var class

  virtual int gen_out_defn (void);
  // generate the _out class definition

  virtual int gen_out_impl (void);
  // generate the _out implementation

  virtual int gen_managed_type_ch (void);
  // generate code for managed types in header

  virtual int gen_managed_type_ci (void);
  // generate code for managed types in inlined file

  virtual int gen_managed_type_cs (void);
  // generate code for managed types in impl file

  virtual int gen_typecode (void);
  // generate the typecode

  virtual int gen_encapsulation (void);
  // encapsulation for parameters

  virtual long tc_size (void);
  // return typecode size

  virtual long tc_encap_len (void);
  // return length of encapsulation

  virtual MANAGED_TYPE managed_type (void);
  // return the managed type

  // =Scope management functions
  be_sequence *fe_add_sequence (be_sequence *);

  virtual be_decl *decl (void);
  // overridden method on the be_scope class

  // Narrowing
  DEF_NARROW_METHODS3 (be_sequence, AST_Sequence, be_scope, be_type);
  DEF_NARROW_FROM_DECL (be_sequence);

protected:
  virtual const char *gen_name (void);
  // helper to create_name

private:
  idl_bool unbounded_;
  // whether we are bounded or unbounded

  MANAGED_TYPE mt_;
  // our managed type
};

#endif
