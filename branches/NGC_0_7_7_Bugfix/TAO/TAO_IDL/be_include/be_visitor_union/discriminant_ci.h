/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    discriminant_ci.h
 *
 *  $Id$
 *
 *   Visitor for the Union class.
 *   This one generates code for the discriminant of the Union in the client
 *   inline file.
 *
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
  ~be_visitor_union_discriminant_ci (void);

  /// visit an enum
  virtual int visit_enum (be_enum *node);

  /// visit a predefined type
  virtual int visit_predefined_type (be_predefined_type *node);

  /// visit a typedef node
  virtual int visit_typedef (be_typedef *node);
};

#endif /* _BE_VISITOR_UNION_DISCRIMINANT_CI_H_ */
