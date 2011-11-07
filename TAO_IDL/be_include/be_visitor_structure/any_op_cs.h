/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    any_op_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Structure class
 *  This one provides code generation for the Any operators for the structure
 *  in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_STRUCTURE_ANY_OP_CS_H_
#define _BE_VISITOR_STRUCTURE_ANY_OP_CS_H_

/**
 * @class be_visitor_structure_any_op_cs
 *
 * @brief be_visitor_structure_any_op_cs
 *
 * This is a concrete visitor for structure that generates the Any operator
 * implementations
 */
class be_visitor_structure_any_op_cs : public be_visitor_structure
{

public:
  /// constructor
  be_visitor_structure_any_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_structure_any_op_cs (void);

  /// visit structure
  virtual int visit_structure (be_structure *node);

  /// visit field
  virtual int visit_field (be_field *node);

  /// visit field
  virtual int visit_union (be_union *node);

  /// visit field
  virtual int visit_enum (be_enum *node);
};

#endif /* _BE_VISITOR_STRUCTURE_ANY_OP_CS_H_ */
