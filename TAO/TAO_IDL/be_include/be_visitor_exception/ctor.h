/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ctor.h
 *
 *  Visitor for Exceptions.
 *  For code generation for the special constructor for exceptions
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_EXCEPTION_CTOR_H_
#define _BE_VISITOR_EXCEPTION_CTOR_H_

/**
 * @class be_visitor_exception_ctor
 *
 * generation of the special constructor
 */
class be_visitor_exception_ctor : public be_visitor_scope
{
public:
  be_visitor_exception_ctor (be_visitor_context *ctx);
  ~be_visitor_exception_ctor ();

  virtual int post_process (be_decl *);

  virtual int visit_exception (be_exception *node);
  virtual int visit_field (be_field *node);

  virtual int visit_array (be_array *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_interface_fwd (be_interface_fwd *node);
  virtual int visit_valuebox (be_valuebox *node);
  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  virtual int visit_predefined_type (be_predefined_type *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_string (be_string *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_structure_fwd (be_structure_fwd *node);
  virtual int visit_union (be_union *node);
  virtual int visit_union_fwd (be_union_fwd *node);
  virtual int visit_typedef (be_typedef *node);

private:
  int emit_common (be_type *node);
};

#endif /* _BE_VISITOR_EXCEPTION_CTOR_H_ */
