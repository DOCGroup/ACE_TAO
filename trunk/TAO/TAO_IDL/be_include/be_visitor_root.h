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
//    be_visitor_root.h
//
// = DESCRIPTION
//    Concrete visitor for the Root class
//
// = AUTHOR
//    Aniruddha Gokhale and Carlos O'Ryan
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_ROOT_H)
#define TAO_BE_VISITOR_ROOT_H

#include "be_visitor.h"
#include "be_visitor_scope.h"

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

  virtual int visit_module (be_module *node);
  // visit a module

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_union (be_union *node);
  // visit a union

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef node

  // =helper. This is used by derived clases to set the right context

  virtual int init (void);
  // will be overridden by derived classes that set the appropriate context
};

class be_visitor_root_ch : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for root
  //
  //
public:
  be_visitor_root_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root_ch (void);
  // destructor

  virtual int init (void);
  // set the right context and make a visitor
};

class be_visitor_root_ci : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client inline for root
  //
  //
public:
  be_visitor_root_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root_ci (void);
  // destructor

  virtual int init (void);
  // set the right context and make a visitor
};

class be_visitor_root_cs : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for root
  //
  //
public:
  be_visitor_root_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root_cs (void);
  // destructor

  virtual int init (void);
  // set the right context and make a visitor
};

class be_visitor_root_sh : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_sh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for root
  //
  //
public:
  be_visitor_root_sh (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root_sh (void);
  // destructor

  virtual int init (void);
  // set the right context and make a visitor
};

class be_visitor_root_si : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_si
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server inline for root
  //
  //
public:
  be_visitor_root_si (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root_si (void);
  // destructor

  virtual int init (void);
  // set the right context and make a visitor
};

class be_visitor_root_ss : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server skeletons for root
  //
  //
public:
  be_visitor_root_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root_ss (void);
  // destructor

  virtual int init (void);
  // set the right context and make a visitor
};


class be_visitor_root_any_op : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_any_op
  //
  // = DESCRIPTION
  //   This is a concrete visitor for root that generates the Any operator
  //   declarations and implementations
  //

public:
  be_visitor_root_any_op (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root_any_op (void);
  // destructor

  virtual int visit_root (be_root *node);
  // visit root
};

#endif // TAO_BE_VISITOR_ROOT_H
