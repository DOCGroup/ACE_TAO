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
//    interface.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This one provides the generic visitor for interfaces from which others
//    will inherit.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_INTERFACE_INTERFACE_H_
#define _BE_INTERFACE_INTERFACE_H_

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

 protected:
    int is_amh_rh_node (be_interface *node);
};

#endif /*  _BE_INTERFACE_INTERFACE_H_ */
