//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    request_info_ss.h
//
// = DESCRIPTION
//    Visitors for generation of code for Arguments as private member lists in the
//    request_info class for interceptors
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef _BE_VISITOR_ARGUMENT_REQUEST_INFO_SS_H_
#define _BE_VISITOR_ARGUMENT_REQUEST_INFO_SS_H_

class be_visitor_args_request_info_ss : public be_visitor_args
{
  //
  // = TITLE
  //   Generates the code for arguments output.
  //
  // = DESCRIPTION
  //   At several stages in the code generation a node of type
  //   "be_operation" is visited, and the code for its arguments must
  //   be generated. This class generates the constructor member list
  //   for the Request_Info class.
  //
public:
  be_visitor_args_request_info_ss (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_request_info_ss (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node
};

#endif /* _BE_VISITOR_ARGUMENT_REQUEST_INFO_SS_H_ */
