
//=============================================================================
/**
 *  @file    interface_ex_idl.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface node.
 *  This provides for code generation in the CIAO
 *  executor IDL
 *
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef _BE_INTERFACE_INTERFACE_EX_IDL_H_
#define _BE_INTERFACE_INTERFACE_EX_IDL_H_

/**
 * @class be_visitor_interface_ex_idl
 *
 * @brief be_visitor_interface_ex_idl
 *
 * This is a concrete visitor to generate the
 * CIAO executor IDL for interface
 */
class be_visitor_interface_ex_idl : public be_visitor_interface
{
public:
  be_visitor_interface_ex_idl (be_visitor_context *ctx);

  ~be_visitor_interface_ex_idl (void);

  virtual int visit_interface (be_interface *node);
};

#endif /* _BE_INTERFACE_INTERFACE_EX_IDL_H_ */
