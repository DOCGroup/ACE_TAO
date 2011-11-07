/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    field_cdr_ch.h
 *
 *  $Id$
 *
 *  Visitor generating code for Field in the client header file.
 *
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
  ~be_visitor_valuetype_field_cdr_ch (void);

  /// visit the field node
  virtual int visit_field (be_field *node);

  // =visit operations on all possible data types that a field can be

  /// visit array type
  virtual int visit_array (be_array *node);

  /// visit enum type
  virtual int visit_enum (be_enum *node);

  /// visit sequence type
  virtual int visit_sequence (be_sequence *node);

  /// visit structure type
  virtual int visit_structure (be_structure *node);

  /// visit typedefed type
  virtual int visit_typedef (be_typedef *node);

  /// visit union type
  virtual int visit_union (be_union *node);

  const char *pre_, *post_;
};

#endif /*  _BE_VISITOR_VALUETYPE_FIELD_CDR_CH_H_ */
