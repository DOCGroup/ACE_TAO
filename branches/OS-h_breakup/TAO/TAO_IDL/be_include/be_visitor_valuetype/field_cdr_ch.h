/* -*- c++ -*- */
//
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    field_cdr_ch.h
//
// = DESCRIPTION
//    Visitor generating code for Field in the client header file.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_VALUETYPE_FIELD_CDR_CH_H_
#define _BE_VISITOR_VALUETYPE_FIELD_CDR_CH_H_

#include "be_visitor_scope.h"

class be_visitor_valuetype_field_cdr_ch : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_valuetype_field_cdr_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for the be_field node for the client header.
  //
public:
  be_visitor_valuetype_field_cdr_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_field_cdr_ch (void);
  // destructor

  virtual int visit_field (be_field *node);
  // visit the field node

  // =visit operations on all possible data types that a field can be

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit enum type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_typedef (be_typedef *node);
  // visit typedefed type

  virtual int visit_union (be_union *node);
  // visit union type

  const char *pre_, *post_;
};

#endif /*  _BE_VISITOR_VALUETYPE_FIELD_CDR_CH_H_ */
