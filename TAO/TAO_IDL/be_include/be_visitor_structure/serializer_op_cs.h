/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    serializer_op_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Structure class
 *  This one provides code generation for the TAO::DCPS::Serializer operators
 *  for the structure in the client stub.
 *
 *
 *  @author Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_STRUCTURE_serializer_op_CS_H_
#define _BE_VISITOR_STRUCTURE_serializer_op_CS_H_

/**
 * @class be_visitor_structure_serializer_op_cs
 *
 * @brief be_visitor_structure_serializer_op_cs
 *
 * This is a concrete visitor for structure that generates the Serializer operator
 * implementations
 */
class be_visitor_structure_serializer_op_cs : public be_visitor_structure
{

public:
  /// constructor
  be_visitor_structure_serializer_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_structure_serializer_op_cs (void);

  /// visit structure
  virtual int visit_structure (be_structure *node);

  /// any post processing that needs to be done after a field is handled
  virtual int post_process (be_decl *);

  /// any preprocessing that needs to be done before a field is handled
  virtual int pre_process (be_decl *bd);
};

#endif /* _BE_VISITOR_STRUCTURE_serializer_op_CS_H_ */
