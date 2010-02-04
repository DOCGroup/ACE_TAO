/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    field_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the base "BE_Field" node
 *  This generates code for structure members in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_FIELD_CH_H_
#define _BE_VISITOR_FIELD_CH_H_

/**
 * @class be_visitor_field_ch
 *
 * @brief be_visitor_field_ch
 *
 * This is a concrete visitor for the be_field node for the client header.
 */
class be_visitor_field_ch : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_field_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_field_ch (void);

  /// visit the field node
  virtual int visit_field (be_field *node);

  // =visit operations on all possible data types that a field can be

  /// visit array type
  virtual int visit_array (be_array *node);

  /// visit enum type
  virtual int visit_enum (be_enum *node);

  /// visit interface type
  virtual int visit_interface (be_interface *node);

  /// visit interface forward type
  virtual int visit_interface_fwd (be_interface_fwd *node);

  /// visit valuebox type
  virtual int visit_valuebox (be_valuebox *node);

  /// visit valuetype type
  virtual int visit_valuetype (be_valuetype *node);

  /// visit valuetype forward type
  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);

  /// visit predefined type
  virtual int visit_predefined_type (be_predefined_type *node);

  /// visit sequence type
  virtual int visit_sequence (be_sequence *node);

  /// visit string type
  virtual int visit_string (be_string *node);

  /// visit structure type
  virtual int visit_structure (be_structure *node);

  /// visit typedefed type
  virtual int visit_typedef (be_typedef *node);

  /// visit union type
  virtual int visit_union (be_union *node);

  /// visit a component node
  virtual int visit_component (be_component *node);

  /// visit a forward declared component node
  virtual int visit_component_fwd (be_component_fwd *node);

  /// visit an eventtype node
  virtual int visit_eventtype (be_eventtype *node);

  /// visit a forward declared eventtype node
  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);

 private:
  int emit_common (be_type *node);
};

#endif /*  _BE_VISITOR_FIELD_CH_H_ */
