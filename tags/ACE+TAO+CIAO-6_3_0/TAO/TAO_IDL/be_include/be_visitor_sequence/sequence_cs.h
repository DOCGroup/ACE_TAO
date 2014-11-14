/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    sequence_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Sequence class
 *  This one provides code generation for the Sequence node in the client
 *  stubs.
 *
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
  be_visitor_sequence_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_sequence_cs (void);

  /// visit sequence node
  virtual int visit_sequence (be_sequence *node);
};

#endif /* _BE_VISITOR_SEQUENCE_SEQUENCE_CS_H_ */
