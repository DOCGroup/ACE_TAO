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
//    root.h
//
// = DESCRIPTION
//    Concrete visitor for the Root class
//    This one provides the generic visitor for the Root node.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_H_
#define _BE_VISITOR_ROOT_ROOT_H_

class be_visitor_root : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_root
  //
  // = DESCRIPTION
  //   This is a concrete visitor for root that abstracts all common tasks
  //

public:
  be_visitor_root (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root (void);
  // destructor

  virtual int visit_root (be_root *node);
  // visit root

  // =visit methods on all elements syntactically valid in a Root scope

  virtual int visit_constant (be_constant *node);
  // visit a constant

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_exception (be_exception *node);
  // visit an exception

  virtual int visit_interface (be_interface *node);
  // visit an interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface

  virtual int visit_valuetype (be_valuetype *node);
  // visit valuetype

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  // visit valuetype_fwd

  virtual int visit_module (be_module *node);
  // visit a module

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_structure_fwd (be_structure_fwd *node);
  // visit a forward declared structure

  virtual int visit_union (be_union *node);
  // visit a union

  virtual int visit_union_fwd (be_union_fwd *node);
  // visit a forward declared union

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef node

  // =helper. This is used by derived clases to set the right context

  virtual int init (void);
  // will be overridden by derived classes that set the appropriate context
};

#endif /* _BE_VISITOR_ROOT_ROOT_H_ */
