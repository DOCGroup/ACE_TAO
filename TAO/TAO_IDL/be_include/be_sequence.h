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

  virtual int gen_typecode (void);
  // generate the typecode

  virtual int gen_encapsulation (void);
  // encapsulation for parameters

  virtual long tc_size (void);
  // return typecode size

  virtual long tc_encap_len (void);
  // return length of encapsulation

  // Narrowing
  DEF_NARROW_METHODS3 (be_sequence, AST_Sequence, be_scope, be_type);
  DEF_NARROW_FROM_DECL (be_sequence);

private:
  void compute_scoped_name (void);
  // for anonymous sequences, we compute our scoped name

  idl_bool unbounded_;
  // whether we are bounded or unbounded

  be_sequence *seq_node_;
  // if we enclose a sequence node
};

#endif
