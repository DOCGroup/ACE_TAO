
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_interface.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface class
//
// = AUTHOR
//    Carlos O'Ryan and Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_INTERFACE_H)
#define TAO_BE_VISITOR_INTERFACE_H

#include "be_visitor_scope.h"


// = DESCRIPTION
//   Interface code generation is complex, the client stubs require
//   two different class and the server also require other two.
//   Only one of them includes whatever nested typedefs, exceptions
//   and or structures are defined in the interface.
//   To break this complexity a different visitor is used for each
//   case.
//
// = NOTE
//   Visitors were added recently into the compiler, so some
//   code is still generated using the old "state oriented" approach;
//   notably only the _collocated class is actually generated using
//   the visitor, in the other cases (the interface class and the
//   skeleton class) we still call back the old code generation
//   methods.
//

class be_visitor_interface : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_interface
  //
  // = DESCRIPTION
  //   This is a concrete visitor for interface that abstracts all common tasks
  //

public:
  be_visitor_interface (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit the interface node

  // =visit methods on all elements syntactically valid in a Interface scope

  virtual int visit_attribute (be_attribute *node);
  // visit attribute

  virtual int visit_constant (be_constant *node);
  // visit a constant

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_exception (be_exception *node);
  // visit an exception

  virtual int visit_operation (be_operation *node);
  // visit operation

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_union (be_union *node);
  // visit a union

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef node

};

// we need derived interface visitors for the client and server header files. For
// the others, they use the default interface visitor

class be_visitor_interface_ch : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for interface
  //
  //
public:
  be_visitor_interface_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_ch (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor

};

class be_visitor_interface_ci : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client inline for interface
  //
  //
public:
  be_visitor_interface_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_ci (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor

};

class be_visitor_interface_cs : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for interface
  //
  //
public:
  be_visitor_interface_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_cs (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor
};

class be_visitor_interface_sh : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_sh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for interface
  //
  //
public:
  be_visitor_interface_sh (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_sh (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor
};

class be_visitor_interface_si : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_si
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server inline for interface
  //
  //
public:
  be_visitor_interface_si (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_si (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor
};

class be_visitor_interface_ss : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server skeletons for interface
  //
  //
public:
  be_visitor_interface_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_ss (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor
};

class be_visitor_interface_collocated_sh : public be_visitor_interface
{
  // = TITLE
  //   Generate the "collocated" class declaration.

public:
  be_visitor_interface_collocated_sh (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_interface_collocated_sh (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit an interface

};

class be_visitor_interface_collocated_ss : public be_visitor_interface
{
  // = TITLE
  //   Generate the "collocated" class declaration.

public:
  be_visitor_interface_collocated_ss (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_interface_collocated_ss (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit interface

};

class be_visitor_interface_any_op_ch : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for interface that generates the Any operator
  //   declarations
  //

public:
  be_visitor_interface_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_any_op_ch (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit interface
};

class be_visitor_interface_any_op_cs : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_any_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for interface that generates the Any operator
  //   implementations
  //

public:
  be_visitor_interface_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_any_op_cs (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit interface
};

class be_visitor_interface_tie_sh : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_tie_sh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for the TIE class
  //
  //
public:
  be_visitor_interface_tie_sh (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_tie_sh (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor
};

class be_visitor_interface_tie_si : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_tie_si
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server inline for operations
  //   of the TIE class
  //
  //
public:
  be_visitor_interface_tie_si (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_tie_si (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor
};

#endif // _BE_VISITOR_INTERFACE_H
