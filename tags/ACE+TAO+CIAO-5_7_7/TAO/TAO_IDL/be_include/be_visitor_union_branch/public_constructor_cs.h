/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    public_constructor_cs.h
 *
 *  $Id$
 *
 *   Visitor for the Union class.
 *   This one generates code for the constructor of the union class
 *
 *
 *  @author Johnny Willemsen
 */
//=============================================================================


#ifndef _BE_VISITOR_UNION_BRANCH_PUBLIC_CONSTRUCTOR_CS_H_
#define _BE_VISITOR_UNION_BRANCH_PUBLIC_CONSTRUCTOR_CS_H_

/**
 * @class be_visitor_union_branch_public_constructor_cs
 *
 * @brief be_visitor_union_branch_public_constructor_cs
 *
 * This is used to generate the body of the constructor
 */
class be_visitor_union_branch_public_constructor_cs : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_union_branch_public_constructor_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_union_branch_public_constructor_cs (void);

  /// visit the union_branch node
  virtual int visit_union_branch (be_union_branch *node);

  // =visit operations on all possible data types that a union_branch can be

  /// visit array type
  virtual int visit_array (be_array *node);

  /// visit predefined type
  virtual int visit_predefined_type (be_predefined_type *node);

  /// visit sequence type
  virtual int visit_sequence (be_sequence *node);

  /// visit structure type
  virtual int visit_structure (be_structure *node);

  /// visit typedefed type
  virtual int visit_typedef (be_typedef *node);

  /// visit union type
  virtual int visit_union (be_union *node);
};

#endif /* _BE_VISITOR_UNION_BRANCH_PUBLIC_CONSTRUCTOR_CS_H_ */
