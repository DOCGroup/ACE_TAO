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
  virtual int visit_predefined_type (be_predefined_type *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_interface_fwd (be_interface_fwd *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_union (be_union *node);
  virtual int visit_array (be_array *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_string (be_string *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_native (be_native *node);

  void argument_direction (int direction);
  void current_type_name (UTL_ScopedName* name);

private:
  TAO_OutStream& stream (void) const;

  // Several types have the same or very similar mappings, for
  // instance: structures, unions and sequences, this routines
  // abstract those mappings.
  int dump_interface (void) const;
  int dump_structure (void) const;

private:
  TAO_OutStream *stream_;

  int argument_direction_;
  // The current argument direction.

  UTL_ScopedName* current_type_name_;
  // The scoped name for the current argument type name.
  // We cannot use just node->name() because we could be using an
  // aliased name (through typedefs) in which case the real name must
  // be used.
};

#endif // _BE_VISITOR_ARGS_H
