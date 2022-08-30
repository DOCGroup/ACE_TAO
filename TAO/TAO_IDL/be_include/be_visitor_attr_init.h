/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_attr_init.h
 *
 *  Encapsulation of rules for generating attribute initializatin
 *
 *  @author Johnny Willemsen
 */
//=============================================================================


#ifndef BE_VISITOR_ATTR_INIT_H
#define BE_VISITOR_ATTR_INIT_H

#include "be_visitor_decl.h"

class TAO_OutStream;

/**
 * @class be_visitor_attr_init
 *
 * @brief be_visitor_attr_init
 *
 * Contains overloaded methods that generate null return
 * values, compliant with the CORBA IDL C++ mapping, and
 * portable to all platforms supported by ACE.
 */
class be_visitor_attr_init : public be_visitor_decl
{
public:
  be_visitor_attr_init (be_visitor_context *ctx);
  ~be_visitor_attr_init () override;

  int visit_array (be_array *) override;
  int visit_component (be_component *) override;
  int visit_enum (be_enum *) override;
  int visit_eventtype (be_eventtype *) override;
  int visit_home (be_home *) override;
  int visit_interface (be_interface *) override;
  int visit_predefined_type (be_predefined_type *) override;
  int visit_sequence (be_sequence *) override;
  int visit_string (be_string *) override;
  int visit_structure (be_structure *) override;
  int visit_typedef (be_typedef *) override;
  int visit_union (be_union *) override;
  int visit_valuebox (be_valuebox *) override;
  int visit_valuetype (be_valuetype *) override;

protected:
  TAO_OutStream & os_;
};

#endif /* BE_VISITOR_ATTR_INIT_H */
