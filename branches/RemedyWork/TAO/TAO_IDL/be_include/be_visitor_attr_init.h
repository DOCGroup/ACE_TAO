/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_attr_init.h
 *
 *  $Id$
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
  virtual ~be_visitor_attr_init (void);

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
};

#endif /* BE_VISITOR_ATTR_INIT_H */
