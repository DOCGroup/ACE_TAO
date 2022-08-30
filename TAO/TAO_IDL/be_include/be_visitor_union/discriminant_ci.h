/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    discriminant_ci.h
 *
 *   Visitor for the Union class.
 *   This one generates code for the discriminant of the Union in the client
 *   inline file.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_UNION_DISCRIMINANT_CI_H_
#define _BE_VISITOR_UNION_DISCRIMINANT_CI_H_

/**
 * @class be_visitor_union_discriminant_ci
 *
 * @brief be_visitor_union_discriminant_ci
 *
 * This is a concrete visitor to generate the client inline for union
 * discriminant
 */
class be_visitor_union_discriminant_ci : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_union_discriminant_ci (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_union_discriminant_ci () override;

  /// visit an enum
  int visit_enum (be_enum *node) override;

  /// visit a predefined type
  int visit_predefined_type (be_predefined_type *node) override;

  /// visit a typedef node
  int visit_typedef (be_typedef *node) override;
};

#endif /* _BE_VISITOR_UNION_DISCRIMINANT_CI_H_ */
