//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    request_info_result.h
//
// = DESCRIPTION
//    Visitors for generation of code for building Dynamic::Result
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef _BE_VISITOR_ARGUMENT_REQUEST_INFO_RESULT_H_
#define _BE_VISITOR_ARGUMENT_REQUEST_INFO_RESULT_H_

class be_visitor_args_request_info_result : public be_visitor_args
{
  //
  // = TITLE
  //   Generates the code for request info result output.
  //
  // = DESCRIPTION
  //   At several stages in the code generation a node of type
  //   "be_operation" is visited, and the code for its arguments must
  //   be generated. Here the generationis to build the ParameterList
  //   on demand.

public:
  be_visitor_args_request_info_result (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_request_info_result (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined types like boolean, char, octet, wchar.
};

#endif /* _BE_VISITOR_ARGUMENT_REQUEST_INFO_RESULT_H_ */
