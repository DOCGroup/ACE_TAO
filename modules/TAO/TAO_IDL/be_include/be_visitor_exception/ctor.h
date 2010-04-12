/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ctor.h
 *
 *  $Id$
 *
 *  Visitor for Exceptions.
 *  For code generation for the special constructor for exceptions
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_EXCEPTION_CTOR_H_
#define _BE_VISITOR_EXCEPTION_CTOR_H_

/**
 * @class be_visitor_exception_ctor
 =TITLE
 * be_visitor_exception_ctor
 * =DESCRIPTION
 * generation of the special constructor
 */
class be_visitor_exception_ctor : public be_visitor_scope
{
public:
  /// ctor
  be_visitor_exception_ctor (be_visitor_context *ctx);

  /// dtor
  ~be_visitor_exception_ctor (void);

  /// post processing after each element
  virtual int post_process (be_decl *);

  /// visit exception. We provide code for this method in the derived class
  virtual int visit_exception (be_exception *node);

  /// visit the argument node
  virtual int visit_field (be_field *node);

  // =visit all the nodes that can be the types for the field

  /// visit array type
  virtual int visit_array (be_array *node);

  /// visit the enum node
  virtual int visit_enum (be_enum *node);

  /// visit interface
  virtual int visit_interface (be_interface *node);

  /// visit interface forward
  virtual int visit_interface_fwd (be_interface_fwd *node);

  /// visit valuebox
  virtual int visit_valuebox (be_valuebox *node);

  /// visit valuetype
  virtual int visit_valuetype (be_valuetype *node);

  /// visit valuetype forward
  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);

  /// visit predefined type
  virtual int visit_predefined_type (be_predefined_type *node);

  /// visit sequence type
  virtual int visit_sequence (be_sequence *node);

  /// visit string type
  virtual int visit_string (be_string *node);

  /// visit structure type
  virtual int visit_structure (be_structure *node);

  /// visit union type
  virtual int visit_union (be_union *node);

  /// visit the typedef type
  virtual int visit_typedef (be_typedef *node);

private:
  int emit_common (be_type *node);

};

#endif /* _BE_VISITOR_EXCEPTION_CTOR_H_ */
