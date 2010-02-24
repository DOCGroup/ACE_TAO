//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    be_visitor_any_extracted_type_decl.h
//
// = DESCRIPTION
//    Encapsulation of rules for declaring variables
//    for Any extraction
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef BE_VISITOR_ANY_EXTRACTED_TYPE_DECL_H
#define BE_VISITOR_ANY_EXTRACTED_TYPE_DECL_H

#include "be_visitor_decl.h"

class TAO_OutStream;

class be_visitor_any_extracted_type_decl : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_any_extracted_type_decl
  //
  // = DESCRIPTION
  //   Contains overloaded methods that generate declarations of
  //   CORBA typenames for Any extraction
  //
  //
public:
  be_visitor_any_extracted_type_decl (be_visitor_context *ctx);
  virtual ~be_visitor_any_extracted_type_decl (void);
  
  virtual int visit_array (be_array *);
  virtual int visit_component (be_component *);
  virtual int visit_enum (be_enum *);
  virtual int visit_eventtype (be_eventtype *);
  virtual int visit_home (be_home *);
  virtual int visit_interface (be_interface *);
  virtual int visit_predefined_type (be_predefined_type *);
  virtual int visit_sequence (be_sequence *);
  virtual int visit_string (be_string *);
  virtual int visit_structure (be_structure *);
  virtual int visit_typedef (be_typedef *);
  virtual int visit_union (be_union *);
  virtual int visit_valuebox (be_valuebox *);
  virtual int visit_valuetype (be_valuetype *);
  
protected:
  TAO_OutStream & os_;
  const char * var_name_;
  const char * tmp_name_;
};

#endif /* BE_VISITOR_ANY_EXTRACTED_TYPE_DECL_H */

