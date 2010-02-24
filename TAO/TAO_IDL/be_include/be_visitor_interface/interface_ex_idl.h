// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    interface_ex_idl.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This provides for code generation in the CIAO
//    executor IDL
//
// = AUTHOR
//   Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef _BE_INTERFACE_INTERFACE_EX_IDL_H_
#define _BE_INTERFACE_INTERFACE_EX_IDL_H_

class be_visitor_interface_ex_idl : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_ex_idl
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the
  //   CIAO executor IDL for interface
  //
  //
public:
  be_visitor_interface_ex_idl (be_visitor_context *ctx);

  ~be_visitor_interface_ex_idl (void);

  virtual int visit_interface (be_interface *node);
};

#endif /* _BE_INTERFACE_INTERFACE_EX_IDL_H_ */
