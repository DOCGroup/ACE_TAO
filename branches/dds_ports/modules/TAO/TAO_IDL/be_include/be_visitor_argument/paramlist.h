//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    paramlist.h
//
// = DESCRIPTION
//    Visitors for generation of code for building Dynamic::ParamList
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef _BE_VISITOR_ARGUMENT_PARAMLIST_H_
#define _BE_VISITOR_ARGUMENT_PARAMLIST_H_

class be_visitor_args_paramlist : public be_visitor_args
{
  //
  // = TITLE
  //   Generates the code for arguments output.
  //
  // = DESCRIPTION
  //   At several stages in the code generation a node of type
  //   "be_operation" is visited, and the code for its arguments must
  //   be generated. Here the generationis to build the ParameterList
  //   on demand.

public:
  be_visitor_args_paramlist (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_paramlist (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined types like boolean, char, octet, wchar.

  virtual int visit_string (be_string *node);
  // visit the string since insertion of string and w_string is special.
};

#endif /* _BE_VISITOR_ARGUMENT_PARAMLIST_H_ */
