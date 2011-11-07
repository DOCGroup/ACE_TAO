/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    discriminant_cs.h
 *
 *  $Id$
 *
 *   Visitor for the Union class.
 *   This one generates code for the discriminant of the Union in the client
 *   stubs.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_UNION_DISCRIMINANT_CS_H_
#define _BE_VISITOR_UNION_DISCRIMINANT_CS_H_

/**
 * @class be_visitor_union_discriminant_cs
 *
 * @brief be_visitor_union_discriminant_cs
 *
 * This is a concrete visitor to generate the client stubs for union
 * discriminant
 */
class be_visitor_union_discriminant_cs : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_union_discriminant_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_union_discriminant_cs (void);

  /// visit an enum. Required to generate the typecode for an enum definition
  /// appearing side the union
  virtual int visit_enum (be_enum *node);

};

#endif /* _BE_VISITOR_UNION_DISCRIMINANT_CS_H_ */
