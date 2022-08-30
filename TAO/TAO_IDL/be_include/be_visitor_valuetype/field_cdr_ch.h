/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    field_cdr_ch.h
 *
 *  Visitor generating code for Field in the client header file.
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUETYPE_FIELD_CDR_CH_H_
#define _BE_VISITOR_VALUETYPE_FIELD_CDR_CH_H_

#include "be_visitor_scope.h"

/**
 * @class be_visitor_valuetype_field_cdr_ch
 *
 * @brief be_visitor_valuetype_field_cdr_ch
 *
 * This is a concrete visitor for the be_field node for the client header.
 */
class be_visitor_valuetype_field_cdr_ch : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_valuetype_field_cdr_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_field_cdr_ch ();

  /// visit the field node
  int visit_field (be_field *node) override;

  // =visit operations on all possible data types that a field can be

  /// visit array type
  int visit_array (be_array *node) override;

  /// visit enum type
  int visit_enum (be_enum *node) override;

  /// visit sequence type
  int visit_sequence (be_sequence *node) override;

  /// visit structure type
  int visit_structure (be_structure *node) override;

  /// visit typedefed type
  int visit_typedef (be_typedef *node) override;

  /// visit union type
  int visit_union (be_union *node) override;

  const char *pre_, *post_;
};

#endif /*  _BE_VISITOR_VALUETYPE_FIELD_CDR_CH_H_ */
