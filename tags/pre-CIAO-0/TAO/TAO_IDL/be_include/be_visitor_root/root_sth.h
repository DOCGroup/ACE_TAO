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
//    root_sth.h
//
// = DESCRIPTION
//    Concrete visitor for the Root class
//    This one provides code generation for elements of the Root node in the
//    server template header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_STH_H_
#define _BE_VISITOR_ROOT_ROOT_STH_H_

class be_visitor_root_sth : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_sth
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server template
  //   header for root.
  //
  //
public:
  be_visitor_root_sth (be_visitor_context *ctx);
  // Constructor.

  ~be_visitor_root_sth (void);
  // Destructor.

  virtual int init (void);
  // Set the right context and make a visitor.

  // Overrides of base class methods so we can skip unnecessary nodes.

  virtual int visit_scope (be_scope *node);

  virtual int visit_module (be_module *node);

  virtual int visit_interface (be_interface *node);
};

#endif /* _BE_VISITOR_ROOT_ROOT_STH_H_ */
