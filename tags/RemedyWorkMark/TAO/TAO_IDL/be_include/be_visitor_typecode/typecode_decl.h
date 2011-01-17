/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    typecode_decl.h
 *
 *  $Id$
 *
 *  Concrete visitor for the generation of TypeCodes.
 *  This one generates the declaration.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_TYPECODE_TYPECODE_DECL_H_
#define _BE_VISITOR_TYPECODE_TYPECODE_DECL_H_

/**
 * @class be_visitor_typecode_decl
 *
 * @brief be_visitor_typecode_decl
 *
 * This is a concrete visitor for generating TypeCode declarations
 */
class be_visitor_typecode_decl : public be_visitor_decl
{

public:
  be_visitor_typecode_decl (be_visitor_context *ctx);
  ~be_visitor_typecode_decl (void);

  /// visit a generic be_type node.
  virtual int visit_type (be_type *node);

  virtual int visit_array (be_array *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_exception (be_exception *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);
  virtual int visit_home (be_home *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_union (be_union *node);
  virtual int visit_valuebox (be_valuebox *node);
  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);
};

#endif /* _BE_VISITOR_TYPECODE_TYPECODE_DECL_H_ */
