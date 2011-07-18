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
//    valuebox_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Valuebox node.
//    This provides code generation for valueboxes in the client stubs.
//
// = AUTHOR
//    Gary Maxey
//
// ============================================================================

#ifndef _BE_VISITOR_VALUEBOX_CS_H_
#define _BE_VISITOR_VALUEBOX_CS_H_

class be_visitor_valuebox_cs : public be_visitor_valuebox
{
  //
  // = TITLE
  //   be_visitor_valuebox_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for valuebox
  //
  //
public:
  be_visitor_valuebox_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuebox_cs (void);
  // destructor

  virtual int visit_valuebox (be_valuebox *node);
  // visit valuebox.

  virtual int visit_array (be_array *node);
  // visit array.

  virtual int visit_enum (be_enum *node);
  // visit enum.

  virtual int visit_interface (be_interface *node);
  // visit interface.

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type.

  virtual int visit_sequence (be_sequence *node);
  // visit sequence.

  virtual int visit_string (be_string *node);
  // visit string.

  virtual int visit_structure (be_structure *node);
  // visit structure.

  virtual int visit_typedef (be_typedef *node);
  // visit typedef.

  virtual int visit_union (be_union *node);
  // visit union.

private:

  /// Generate virtual destructor.
  void emit_destructor (void);

  int emit_for_predef_enum (char const * marshal_arg);

};

#endif /* _BE_VISITOR_VALUEBOX_CS_H_ */
