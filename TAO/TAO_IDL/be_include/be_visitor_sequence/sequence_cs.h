/* -*- c++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    sequence_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Sequence class
//    This one provides code generation for the Sequence node in the client
//    stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_SEQUENCE_SEQUENCE_CS_H_
#define _BE_VISITOR_SEQUENCE_SEQUENCE_CS_H_

class TAO_OutStream;
class be_visitor_sequence_cs : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_sequence_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for
  //   sequences
  //
public:
  be_visitor_sequence_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_sequence_cs (void);
  // destructor

  virtual int visit_sequence (be_sequence *node);
  // visit sequence node

  int gen_varout_tmplinst (be_sequence *node,
                           be_type *bt);
  // Generate explicit template instantiations for our _var
  // and _out classes.

  void gen_managed_type_tmplinst (be_sequence *node,
                                  be_type *bt);
  // Generate explicit instantiation for our element type, if
  // it is a managed type and if Any operator generation is suppressed.

private:

  // Generate explicit instantiation for the base class type.
  int gen_base_class_tmplinst (be_sequence *node);
};

#endif /* _BE_VISITOR_SEQUENCE_SEQUENCE_CS_H_ */
