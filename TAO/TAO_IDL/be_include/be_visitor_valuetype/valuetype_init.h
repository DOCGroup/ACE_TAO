//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_init.h
//
// = DESCRIPTION
//    Coomon visitor for the Valuetype node.
//    This one provides code generation for valuetype factory construct.
// 
//
// = AUTHOR
//    Boris Kolpackov <bosk@ipmce.ru>
//    
//
// ============================================================================

#ifndef _BE_VALUETYPE_VALUETYPE_INIT_H_
#define _BE_VALUETYPE_VALUETYPE_INIT_H_


class be_visitor_valuetype_init : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_valuetype_init
  //
  // = DESCRIPTION
  //   This is a common visitor to generate valuetype factory constructs.
  //
  //

public:
  be_visitor_valuetype_init (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_init (void);
  // destructor

public:

  virtual int visit_attribute (be_attribute *node);
  // visit attribute

  virtual int visit_constant (be_constant *node);
  // visit a constant

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_operation (be_operation *node);
  // visit operation

  virtual int visit_exception (be_exception *node);
  // visit exception (not used)

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_union (be_union *node);
  // visit a union

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef node

  virtual int visit_field (be_field *node);
  // visit a field

  virtual int visit_factory (be_factory *node);
  // visit a factory
  
  void begin_public (void);
  void begin_private (void);
};

#endif /* _BE_VALUETYPE_VALUETYPE_INIT_H_ */
