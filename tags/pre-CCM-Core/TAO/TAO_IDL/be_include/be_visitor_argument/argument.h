//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    argument.h
//
// = DESCRIPTION
//    Visitors for generation of code for Arguments
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ARGUMENT_ARGUMENT_H_
#define _BE_VISITOR_ARGUMENT_ARGUMENT_H_

class be_visitor_args : public be_visitor_decl
{
  //
  // = TITLE
  //   Generates the code for arguments output.
  //
  // = DESCRIPTION
  //   At several stages in the code generation a node of type
  //   "be_operation" is visited, and the code for its arguments must
  //   be generated.
  //   Sometimes the argument declarations must be generated (such as
  //   in the class declaration), other the arguments names (such as
  //   in method invocations).
  //   This class implements some common functionality required across all
  //   argument visitors
  //
public:
  be_visitor_args (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args (void);
  // destructor

  //= helper
  virtual const char *type_name (be_type *type, const char *suffix=0);
  // helper that produces either a nested name for header files or full name
  // for others

  virtual AST_Argument::Direction direction (void);
  // return the direction of the argument node

  virtual int visit_argument (be_argument *node);
  // must be overridden by derived classes
};

#endif /* _BE_VISITOR_ARGUMENT_ARGUMENT_H_ */
