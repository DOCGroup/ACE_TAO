// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    upcall_command_ss.cpp
//
// = DESCRIPTION
//    Visitor that generates operation-specific TAO::Upcall_Command
//    objects in skeletons.
//
// = AUTHOR
//    Ossama Othman
//
// ============================================================================


#ifndef _BE_INTERFACE_UPCALL_COMMAND_SS_H_
#define _BE_INTERFACE_UPCALL_COMMAND_SS_H_

class be_visitor_interface_upcall_command_ss
  : public be_visitor_interface
{
public:

  /// Constructor
  be_visitor_interface_upcall_command_ss (be_visitor_context *ctx);

  /// destructor
  virtual ~be_visitor_interface_upcall_command_ss (void);

  /// visit an interface
  virtual int visit_interface (be_interface *node);

  /// visit a component
  virtual int visit_component (be_component *node);

  /// Helper to generate the declarations for the operations
  /// of any abstract parents we may have.
  static int gen_abstract_ops_helper (be_interface * node,
                                      be_interface * base,
                                      TAO_OutStream * os);

};

#endif /* _BE_INTERFACE_UPCALL_COMMAND_SS_H_ */
