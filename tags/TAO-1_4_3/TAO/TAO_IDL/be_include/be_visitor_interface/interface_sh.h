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
//    interface_sh.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This provides for code generation in the server header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_INTERFACE_INTERFACE_SH_H_
#define _BE_INTERFACE_INTERFACE_SH_H_

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
    
  static int gen_abstract_ops_helper (be_interface *node,
                                      be_interface *base,
                                      TAO_OutStream *os);
  // Helper to generate the declarations for the operations
  // of any abstract parents we may have.

protected:
  virtual void this_method (be_interface *node);
  virtual int generate_amh_classes (be_interface *node);
};

#endif /* _BE_INTERFACE_INTERFACE_SH_H_ */
