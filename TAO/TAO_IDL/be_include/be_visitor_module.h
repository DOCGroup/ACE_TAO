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
//    be_visitor_module.h
//
// = DESCRIPTION
//    Concrete visitor for the Module class
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_MODULE_H)
#define TAO_BE_VISITOR_MODULE_H

#include "be_visitor.h"
#include "be_visitor_scope.h"

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

// we need derived module visitors for the client and server header files. For
// the others, they use the default module visitor

class be_visitor_module_ch : public be_visitor_module
{
  //
  // = TITLE
  //   be_visitor_module_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for module
  //
  //
public:
  be_visitor_module_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_module_ch (void);
  // destructor

  virtual int visit_module (be_module *node);
  // visit module. We provide code for this method in the derived class

};

class be_visitor_module_sh : public be_visitor_module
{
  //
  // = TITLE
  //   be_visitor_module_sh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for module
  //
  //
public:
  be_visitor_module_sh (be_visitor_context *ctx);
  // constructor

  ~be_visitor_module_sh (void);
  // destructor

  virtual int visit_module (be_module *node);
  // visit module. We provide code for this method in the derived class

};

class be_visitor_module_any_op : public be_visitor_module
{
  //
  // = TITLE
  //   be_visitor_module_any_op
  //
  // = DESCRIPTION
  //   This is a concrete visitor for module that generates the Any operator
  //   declarations and implementations
  //

public:
  be_visitor_module_any_op (be_visitor_context *ctx);
  // constructor

  ~be_visitor_module_any_op (void);
  // destructor

  virtual int visit_module (be_module *node);
  // visit module
};

#endif // TAO_BE_VISITOR_MODULE_H
