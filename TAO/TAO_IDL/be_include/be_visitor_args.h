//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_args.h
//
// = DESCRIPTION
//    Visitors for generation of code for Arguments
//
// = AUTHOR
//    Aniruddha Gokhale and Carlos O' Ryan
//
// ============================================================================

#ifndef _BE_VISITOR_ARGS_H
#define _BE_VISITOR_ARGS_H

#include "be_visitor_decl.h"

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

class be_visitor_args_arglist : public be_visitor_args
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
  be_visitor_args_arglist (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_arglist (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  // =visit all the nodes that can be the types for the argument

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit the enum node

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward

  virtual int visit_native (be_native *node);
  // visit native node

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_union (be_union *node);
  // visit union type

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef type

};

// ************************************************************
// class be_visitor_args_pre_docall_cs
// ************************************************************
class be_visitor_args_pre_docall_cs : public be_visitor_args
{
  //
  // = TITLE
  //   be_visitor_args_docall_cs
  //
  // = DESCRIPTION
  //   Code to be generated when making the do_static_call
  //
public:
  be_visitor_args_pre_docall_cs (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_pre_docall_cs (void);
  // destructor

  virtual int void_return_type (void);
  // return true if the operation return type is void

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  // =visit all the nodes that can be the types for the argument

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_union (be_union *node);
  // visit union type

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef type

};

// ************************************************************
// class be_visitor_args_docall_cs
// ************************************************************
class be_visitor_args_docall_cs : public be_visitor_args
{
  //
  // = TITLE
  //   be_visitor_args_docall_cs
  //
  // = DESCRIPTION
  //   Code to be generated when making the do_static_call
  //
public:
  be_visitor_args_docall_cs (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_docall_cs (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  // =visit all the nodes that can be the types for the argument

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit the enum node

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_union (be_union *node);
  // visit union type

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef type

};

// ************************************************************
// class be_visitor_args_post_docall_cs
// ************************************************************
class be_visitor_args_post_docall_cs : public be_visitor_args
{
  //
  // = TITLE
  //   be_visitor_args_post_docall_cs
  //
  // = DESCRIPTION
  //   Code to be generated after the do_static_call is done
  //
public:
  be_visitor_args_post_docall_cs (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_post_docall_cs (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  // =visit all the nodes that can be the types for the argument

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef type
};

// ************************************************************
// class be_visitor_args_vardecl_ss
// ************************************************************
class be_visitor_args_vardecl_ss : public be_visitor_args
{
  //
  // = TITLE
  //   be_visitor_args_vardecl_ss
  //
  // = DESCRIPTION
  //   Visitor for variable declaration for argument
  //
public:
  be_visitor_args_vardecl_ss (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_vardecl_ss (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  // =visit all the nodes that can be the types for the argument

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit the enum node

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_union (be_union *node);
  // visit union type

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef type

};

// ************************************************************
// class be_visitor_args_pre_upcall_ss
// ************************************************************
class be_visitor_args_pre_upcall_ss : public be_visitor_args
{
  //
  // = TITLE
  //   be_visitor_args_upcall_ss
  //
  // = DESCRIPTION
  //   Visitor for passing argument to the upcall
  //
public:
  be_visitor_args_pre_upcall_ss (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_pre_upcall_ss (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  // =visit all the nodes that can be the types for the argument

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit the enum node

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_union (be_union *node);
  // visit union type

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef type

};

// ************************************************************
// class be_visitor_args_upcall_ss
// ************************************************************
class be_visitor_args_upcall_ss : public be_visitor_args
{
  //
  // = TITLE
  //   be_visitor_args_upcall_ss
  //
  // = DESCRIPTION
  //   Visitor for passing argument to the upcall
  //
public:
  be_visitor_args_upcall_ss (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_upcall_ss (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  // =visit all the nodes that can be the types for the argument

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit the enum node

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_union (be_union *node);
  // visit union type

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef type

};

// ************************************************************
// class be_visitor_args_marshal_ss
// ************************************************************
class be_visitor_args_marshal_ss : public be_visitor_args
{
  //
  // = TITLE
  //   be_visitor_args_marshal_ss
  //
  // = DESCRIPTION
  //   Visitor for passing argument to the marshaling/demarshaling routine
  //
public:
  be_visitor_args_marshal_ss (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_marshal_ss (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  // =visit all the nodes that can be the types for the argument

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit the enum node

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_union (be_union *node);
  // visit union type

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef type

};

// ************************************************************
// class be_visitor_args_post_upcall_ss
// ************************************************************
class be_visitor_args_post_upcall_ss : public be_visitor_args
{
  //
  // = TITLE
  //   be_visitor_args_post_upcall_ss
  //
  // = DESCRIPTION
  //   Visitor for post processing after upcall
  //
public:
  be_visitor_args_post_upcall_ss (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_post_upcall_ss (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  // =visit all the nodes that can be the types for the argument

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit the enum node

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_union (be_union *node);
  // visit union type

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef type

};

// ************************************************************
// class be_visitor_args_post_marshal_ss
// ************************************************************
class be_visitor_args_post_marshal_ss : public be_visitor_args
{
  //
  // = TITLE
  //   be_visitor_args_post_marshal_ss
  //
  // = DESCRIPTION
  //   Visitor for post processing after marshal
  //
public:
  be_visitor_args_post_marshal_ss (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_post_marshal_ss (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  // =visit all the nodes that can be the types for the argument

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit the enum node

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_union (be_union *node);
  // visit union type

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef type

};

#endif // _BE_VISITOR_ARGS_H
