//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_ami_exception_holder_operation_cs.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations in client stubs
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de<
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_OPERATION_AMI_EXCEPTION_HOLDER_OPERATION_CS_H_
#define _BE_VISITOR_OPERATION_OPERATION_AMI_EXCEPTION_HOLDER_OPERATION_CS_H_


class be_visitor_operation_ami_exception_holder_operation_cs : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operation_ami_exception_holder_operation_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for operation
  //
  //
public:
  be_visitor_operation_ami_exception_holder_operation_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_ami_exception_holder_operation_cs (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation.
};



#endif /* _BE_VISITOR_OPERATION_OPERATION_AMI_EXCEPTION_HOLDER_OPERATION_CS_H_ */
