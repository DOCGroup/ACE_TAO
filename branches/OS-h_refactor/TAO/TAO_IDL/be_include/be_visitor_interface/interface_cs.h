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
//    interface_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This one provides code generation for interfaces in the client stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_INTERFACE_INTERFACE_CS_H_
#define _BE_INTERFACE_INTERFACE_CS_H_

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

  static int gen_abstract_ops_helper (be_interface *node,
                                      be_interface *base,
                                      TAO_OutStream *os);
  // Helper to generate the definitions for the operations
  // of any abstract parents we may have.

protected:
  void gen_concrete_unchecked_narrow (be_interface *node,
                                      be_type *bt,
                                      TAO_OutStream *os);
  void gen_abstract_unchecked_narrow (be_interface *node,
                                      be_type *bt,
                                      TAO_OutStream *os);
};

#endif /* _BE_INTERFACE_INTERFACE_CS_H_ */
