//
// $Id$
//

#ifndef _BE_VISITOR_ARGS_H
#define _BE_VISITOR_ARGS_H

#include "be_visitor.h"

class TAO_OutStream;

class be_visitor_args_decl : public be_visitor
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
  //   This class implements the declaration output, in other words,
  //   it outputs both the arguments and their types.
  //
public:
  be_visitor_args_decl (TAO_OutStream *stream);
  virtual ~be_visitor_args_decl (void);

  virtual int visit_operation (be_operation *node);
  virtual int visit_argument (be_argument *node);

private:
  TAO_OutStream& stream (void) const;

private:
  TAO_OutStream *stream_;
};

#endif // _BE_VISITOR_ARGS_H
