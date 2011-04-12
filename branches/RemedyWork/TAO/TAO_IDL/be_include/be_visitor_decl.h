/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_decl.h
 *
 *  $Id$
 *
 *  Concrete visitor for the base "BE_Decl" node
 *
 *
 *  @author Aniruddha Gokhale and Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_BE_VISITOR_DECL_H
#define TAO_BE_VISITOR_DECL_H

#include "be_visitor.h"
#include "be_codegen.h"
#include "be_helper.h"

/**
 * @class be_visitor_decl
 *
 * @brief be_visitor_decl
 *
 * This is a concrete visitor for the be_decl node. Its only purpose is to
 * maintain state information for the derived visitor
 */
class be_visitor_decl : public be_visitor
{
public:
  be_visitor_decl (be_visitor_context *ctx);

  ~be_visitor_decl (void);

  be_visitor_context *ctx (void);

protected:
  /// Any context information required by the visitor.
  be_visitor_context *ctx_;

  /// Factors out common code used when one anonymous type is
  /// contained by another.
  int gen_anonymous_base_type (be_type *bt,
                               TAO_CodeGen::CG_STATE cg_state);

  /// Our file stream.
  TAO_OutStream &os_;
};

#endif // TAO_BE_VISITOR_DECL_H
