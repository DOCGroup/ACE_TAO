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
//    direct_collocated_sh.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This provides code generation for direct collocated classes in server header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_INTERFACE_DIRECT_COLLOCATED_SH_H_
#define _BE_INTERFACE_DIRECT_COLLOCATED_SH_H_

class be_visitor_interface_direct_collocated_sh : public be_visitor_interface
{
  // = TITLE
  //   Generate the "direct collocated" class declaration.

public:
  be_visitor_interface_direct_collocated_sh (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_interface_direct_collocated_sh (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit an interface

  static int gen_abstract_ops_helper (be_interface *node,
                                      be_interface *base,
                                      TAO_OutStream *os);
  // Helper to generate the declarations for the operations
  // of any abstract parents we may have.
};

#endif /* _BE_INTERFACE_DIRECT_COLLOCATED_SH_H_ */
