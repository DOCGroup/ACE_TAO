/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    sequence_cs.h
 *
 *  Concrete visitor for the Sequence class
 *  This one provides code generation for the Sequence node in the client
 *  stubs.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_SEQUENCE_SEQUENCE_CS_H_
#define _BE_VISITOR_SEQUENCE_SEQUENCE_CS_H_

/**
 * @class be_visitor_sequence_cs
 *
 * @brief be_visitor_sequence_cs
 *
 * This is a concrete visitor to generate the client stubs for
 * sequences
 */
class TAO_OutStream;
class be_visitor_sequence_cs : public be_visitor_decl
{
public:
  /// constructor
  explicit be_visitor_sequence_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_sequence_cs () override  = default;

  be_visitor_sequence_cs (const be_visitor_sequence_cs &) = delete;
  be_visitor_sequence_cs (be_visitor_sequence_cs &&) = delete;
  be_visitor_sequence_cs &operator= (const be_visitor_sequence_cs &) = delete;
  be_visitor_sequence_cs &operator= (be_visitor_sequence_cs &&) = delete;

  /// visit sequence node
  int visit_sequence (be_sequence *node) override;
};

#endif /* _BE_VISITOR_SEQUENCE_SEQUENCE_CS_H_ */
