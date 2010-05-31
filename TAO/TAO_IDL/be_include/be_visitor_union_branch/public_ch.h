/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    public_ch.h
 *
 *  $Id$
 *
 *   Visitor for the Union class.
 *   This one generates code for public part of the union class for the union
 *   members in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_UNION_BRANCH_PUBLIC_CH_H_
#define _BE_VISITOR_UNION_BRANCH_PUBLIC_CH_H_

/**
 * @class be_visitor_union_branch_public_ch
 *
 * @brief be_visitor_union_branch_public_ch
 *
 * This is a concrete visitor for the be_union_branch node for the client
 * header. This generates the code for the public section of the "union"
 * class
 */
class be_visitor_union_branch_public_ch : public be_visitor_decl
{
public:
  be_visitor_union_branch_public_ch (be_visitor_context *ctx);
  ~be_visitor_union_branch_public_ch (void);

  virtual int visit_union_branch (be_union_branch *node);

  virtual int visit_array (be_array *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_interface_fwd (be_interface_fwd *node);
  virtual int visit_valuebox (be_valuebox *node);
  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  virtual int visit_predefined_type (be_predefined_type *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_string (be_string *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_structure_fwd (be_structure_fwd *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_union (be_union *node);
  virtual int visit_union_fwd (be_union_fwd *node);
};

#endif /* _BE_VISITOR_UNION_BRANCH_PUBLIC_CH_H_ */
