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
//    module.h
//
// = DESCRIPTION
//    Concrete visitor for the Module class
//    This one provides the generic visitor for the Module node.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_MODULE_MODULE_H_
#define _BE_VISITOR_MODULE_MODULE_H_

class be_visitor_module : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_module
  //
  // = DESCRIPTION
  //   This is a concrete visitor for module that abstracts all common tasks
  //

public:
  be_visitor_module (be_visitor_context *ctx);
  // constructor

  ~be_visitor_module (void);
  // destructor

  // =visit methods on all elements syntactically valid in a Module scope

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

  virtual int visit_module (be_module *node);
  // visit a module

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_union (be_union *node);
  // visit a union

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef node

};

#endif /* _BE_VISITOR_MODULE_MODULE_H_ */
