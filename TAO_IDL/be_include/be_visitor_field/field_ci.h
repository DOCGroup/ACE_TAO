/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    field_ci.h
 *
 *  $Id$
 *
 *  Concrete visitor for the base "BE_Field" node
 *  This generates code for structure members in the client inline.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_FIELD_CI_H_
#define _BE_VISITOR_FIELD_CI_H_

/**
 * @class be_visitor_field_ci
 *
 * @brief be_visitor_field_ci
 *
 * This is a concrete visitor for the be_field node for the client inline
 * file
 */
class be_visitor_field_ci : public be_visitor_decl
{
public:
  be_visitor_field_ci (be_visitor_context *ctx);
  ~be_visitor_field_ci (void);

  virtual int visit_field (be_field *node);

  virtual int visit_array (be_array *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_structure_fwd (be_structure_fwd *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_union (be_union *node);
  virtual int visit_union_fwd (be_union_fwd *node);
};

#endif /*  _BE_VISITOR_FIELD_CI_H_ */
