/* -*- c++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_operation.h
//
// = DESCRIPTION
//    Concrete visitor for the Operation class
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_OPERATION_H)
#define TAO_BE_VISITOR_OPERATION_H

#include "be_visitor.h"
#include "be_visitor_scope.h"

// ************************************************************
// Operation visitor for client header
// ************************************************************

class be_visitor_operation_ch : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for operation
  //
  //
public:
  be_visitor_operation_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_ch (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation. We provide code for this method in the derived class

};

// ************************************************************
// Operation visitor for client stubs
// ************************************************************

class be_visitor_operation_cs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for operation
  //
  //
public:
  be_visitor_operation_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_cs (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation.

  virtual int visit_argument (be_argument *node);
  // visit argument to generate ParamData entries

  virtual int post_process (void);
  // stuff to output after every member of the scope is handled
};

// ************************************************************
// Operation visitor for server header
// ************************************************************

class be_visitor_operation_sh : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_sh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for operation
  //
  //
public:
  be_visitor_operation_sh (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_sh (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation. We provide code for this method in the derived class

};

// ************************************************************
// Operation visitor for server skeletons
// ************************************************************

class be_visitor_operation_ss : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server skeletons for operation
  //
  //
public:
  be_visitor_operation_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_ss (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation. We provide code for this method in the derived class

  virtual int visit_argument (be_argument *node);
  // visit argument to generate ParamData entries

  virtual int post_process (void);
  // stuff to output after every member of the scope is handled
};

// ************************************************************
// Operation visitor for server header in the collocated class
// ************************************************************

class be_visitor_operation_collocated_sh : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_collocated_sh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for operation
  //   in the collocated class
  //
  //
public:
  be_visitor_operation_collocated_sh (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_collocated_sh (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation node.

};

// *****************************************************************
// Operation visitor for server skeletons for the collocated class
// *****************************************************************

class be_visitor_operation_collocated_ss : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server skeletons for
  //   operation in the collocated class
  //
  //
public:
  be_visitor_operation_collocated_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_collocated_ss (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation node.

};

// ***********************************************************************
// Operation visitor for operation declaration inside the TIE class
// ***********************************************************************

class be_visitor_operation_tie_sh : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_tie_sh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the operaton inside a TIE class
  //
  //
public:
  be_visitor_operation_tie_sh (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_tie_sh (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // set the right context and make a visitor
};

// ***********************************************************************
// Operation visitor for operation declaration inside the TIE class
// ***********************************************************************

class be_visitor_operation_tie_si : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_tie_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the operaton inside a TIE class
  //
  //
public:
  be_visitor_operation_tie_si (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_tie_si (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // set the right context and make a visitor
};

// ***********************************************************************
// Operation visitor for return types in the signature of the operation
// ***********************************************************************

class be_visitor_operation_rettype : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_ch
  //
  // = DESCRIPTION
  //   This is a visitor to generate operation return types
  //
  //
public:
  be_visitor_operation_rettype (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype (void);
  // destructor

  int visit_array (be_array *node);
  // visit an array node

  int visit_enum (be_enum *node);
  // visit an enum node

  int visit_interface (be_interface *node);
  // visit an interface node

  int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node

  int visit_native (be_native *node);
  // visit native type

  int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type node

  int visit_sequence (be_sequence *node);
  // visit a sequence node

  int visit_string (be_string *node);
  // visit a sequence node

  int visit_structure (be_structure *node);
  // visit a structure node

  int visit_typedef (be_typedef *node);
  // visit a typedef node

  int visit_union (be_union *node);
  // visit a union node

};

// ******************************************************************************
// Operation visitor for argument list - generates parameters in the signature
// ******************************************************************************

class be_visitor_operation_arglist : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operaion_arglist
  //
  // = DESCRIPTION
  //   This is a visitor to generate operation argument list
  //
  //
public:
  be_visitor_operation_arglist (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_arglist (void);
  // destructor

  int visit_operation (be_operation *node);
  // visit the operation

  int visit_argument (be_argument *node);
  // visit each argument

};

// ************************************************************
// Operation visitor for return type variable declaration
// ************************************************************

class be_visitor_operation_rettype_vardecl_cs :public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_vardecl_cs
  //
  // = DESCRIPTION
  //   This is a visitor to generate a variable declaration ofr an operation
  //   return type
  //
public:
  be_visitor_operation_rettype_vardecl_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype_vardecl_cs (void);
  // destructor

  int visit_array (be_array *node);
  // visit an array node

  int visit_enum (be_enum *node);
  // visit an enum node

  int visit_interface (be_interface *node);
  // visit an interface node

  int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node

  int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type node

  int visit_sequence (be_sequence *node);
  // visit a sequence node

  int visit_string (be_string *node);
  // visit a sequence node

  int visit_structure (be_structure *node);
  // visit a structure node

  int visit_typedef (be_typedef *node);
  // visit a typedef node

  int visit_union (be_union *node);
  // visit a union node

};

// ************************************************************
// Operation visitor for preprocessing before do_static_call is invoked
// ************************************************************

class be_visitor_operation_rettype_pre_docall_cs :public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_pre_docall_cs
  //
  // = DESCRIPTION
  //   This is a visitor to generate any pre processing before the do_static_call is
  //   made
  //
public:
  be_visitor_operation_rettype_pre_docall_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype_pre_docall_cs (void);
  // destructor

  int visit_array (be_array *node);
  // visit an array node

  int visit_interface (be_interface *node);
  // visit an interface node

  int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node

  int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type node

  int visit_sequence (be_sequence *node);
  // visit a sequence node

  int visit_structure (be_structure *node);
  // visit a structure node

  int visit_typedef (be_typedef *node);
  // visit a typedef node

  int visit_union (be_union *node);
  // visit a union node

};

// ************************************************************
// Operation visitor for passing return variable to do_static_call
// ************************************************************

class be_visitor_operation_rettype_docall_cs :public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_docall_cs
  //
  // = DESCRIPTION
  //   This is a visitor to generate a variable declaration ofr an operation
  //   return type
  //
public:
  be_visitor_operation_rettype_docall_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype_docall_cs (void);
  // destructor

  int visit_array (be_array *node);
  // visit an array node

  int visit_enum (be_enum *node);
  // visit an enum node

  int visit_interface (be_interface *node);
  // visit an interface node

  int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node

  int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type node

  int visit_sequence (be_sequence *node);
  // visit a sequence node

  int visit_string (be_string *node);
  // visit a sequence node

  int visit_structure (be_structure *node);
  // visit a structure node

  int visit_typedef (be_typedef *node);
  // visit a typedef node

  int visit_union (be_union *node);
  // visit a union node

};

// ************************************************************
// Operation visitor for postprocessing after do_static_call is invoked
// ************************************************************

class be_visitor_operation_rettype_post_docall_cs :public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_psot_docall_cs
  //
  // = DESCRIPTION
  //   This is a visitor to generate any post processing after the do_static_call is
  //   made
  //
public:
  be_visitor_operation_rettype_post_docall_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype_post_docall_cs (void);
  // destructor

  int visit_interface (be_interface *node);
  // visit an interface node

  int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node
};

// ************************************************************
// Operation visitor for returning the return val
// ************************************************************

class be_visitor_operation_rettype_return_cs :public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_return_cs
  //
  // = DESCRIPTION
  //   This is a visitor to generate a variable declaration ofr an operation
  //   return type
  //
public:
  be_visitor_operation_rettype_return_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype_return_cs (void);
  // destructor

  int visit_array (be_array *node);
  // visit an array node

  int visit_enum (be_enum *node);
  // visit an enum node

  int visit_interface (be_interface *node);
  // visit an interface node

  int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node

  int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type node

  int visit_sequence (be_sequence *node);
  // visit a sequence node

  int visit_string (be_string *node);
  // visit a sequence node

  int visit_structure (be_structure *node);
  // visit a structure node

  int visit_typedef (be_typedef *node);
  // visit a typedef node

  int visit_union (be_union *node);
  // visit a union node

};

// ******************************************************************************
// Visitor for
// ******************************************************************************

class be_visitor_operation_exceptlist_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operaion_exceptlist_cs
  //
  // = DESCRIPTION
  //   This is a visitor to generate operation exception list to be provided to
  //   the do_static_call
  //
  //
public:
  be_visitor_operation_exceptlist_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_exceptlist_cs (void);
  // destructor

  int visit_operation (be_operation *node);
  // visit the operation

};

// ************************************************************
// generic operation visitor for docall, upcall, pre/post
// ************************************************************

class be_visitor_operation_argument : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_argument
  //
  // = DESCRIPTION
  //   This is a visitor to generate a variable declaration ofr an operation
  //   return type
  //
public:
  be_visitor_operation_argument (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_argument (void);
  // destructor

  int post_process (void);
  // stuff to o/p after each element of the scope is handled

  int visit_operation (be_operation *node);
  // visit the operation

  int visit_argument (be_argument *node);
  // visit argument

};

// ************************************************************
// Operation visitor for return type variable declaration in the server
// skeleton
// ************************************************************

class be_visitor_operation_rettype_vardecl_ss :public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_vardecl_ss
  //
  // = DESCRIPTION
  //   This is a visitor to generate a variable declaration ofr an operation
  //   return type in the server skeleton
  //
public:
  be_visitor_operation_rettype_vardecl_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype_vardecl_ss (void);
  // destructor

  int visit_array (be_array *node);
  // visit an array node

  int visit_enum (be_enum *node);
  // visit an enum node

  int visit_interface (be_interface *node);
  // visit an interface node

  int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node

  int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type node

  int visit_sequence (be_sequence *node);
  // visit a sequence node

  int visit_string (be_string *node);
  // visit a sequence node

  int visit_structure (be_structure *node);
  // visit a structure node

  int visit_typedef (be_typedef *node);
  // visit a typedef node

  int visit_union (be_union *node);
  // visit a union node

};

// ***********************************************************************
// Operation visitor for passing return type to the marshal/demarshal routine
// inside the server skeleton
// ***********************************************************************

class be_visitor_operation_rettype_marshal_ss : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_marshal_ss
  //
  // = DESCRIPTION
  //   This is a visitor to pass return type to the marshal/demarshal routines
  //
  //
public:
  be_visitor_operation_rettype_marshal_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype_marshal_ss (void);
  // destructor

  int visit_array (be_array *node);
  // visit an array node

  int visit_enum (be_enum *node);
  // visit an enum node

  int visit_interface (be_interface *node);
  // visit an interface node

  int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node

  int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type node

  int visit_sequence (be_sequence *node);
  // visit a sequence node

  int visit_string (be_string *node);
  // visit a sequence node

  int visit_structure (be_structure *node);
  // visit a structure node

  int visit_typedef (be_typedef *node);
  // visit a typedef node

  int visit_union (be_union *node);
  // visit a union node

};

// ***********************************************************************
// Operation visitor for assignment to return types in an upcall
// ***********************************************************************

class be_visitor_operation_rettype_assign_ss : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_ch
  //
  // = DESCRIPTION
  //   This is a visitor to generate code that assigns to the return type
  //   variable in the upcall
  //
  //
public:
  be_visitor_operation_rettype_assign_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype_assign_ss (void);
  // destructor

  int visit_array (be_array *node);
  // visit an array node

  int visit_enum (be_enum *node);
  // visit an enum node

  int visit_interface (be_interface *node);
  // visit an interface node

  int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node

  int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type node

  int visit_sequence (be_sequence *node);
  // visit a sequence node

  int visit_string (be_string *node);
  // visit a sequence node

  int visit_structure (be_structure *node);
  // visit a structure node

  int visit_typedef (be_typedef *node);
  // visit a typedef node

  int visit_union (be_union *node);
  // visit a union node

};

// ***********************************************************************
// Operation visitor for post upcall processing of return types
// ***********************************************************************

class be_visitor_operation_rettype_post_upcall_ss : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_post_upcall_ss
  //
  // = DESCRIPTION
  //   This is a visitor for post processing after an upcall
  //
  //
public:
  be_visitor_operation_rettype_post_upcall_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype_post_upcall_ss (void);
  // destructor

  int visit_array (be_array *node);
  // visit an array node

  int visit_enum (be_enum *node);
  // visit an enum node

  int visit_interface (be_interface *node);
  // visit an interface node

  int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node

  int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type node

  int visit_sequence (be_sequence *node);
  // visit a sequence node

  int visit_string (be_string *node);
  // visit a sequence node

  int visit_structure (be_structure *node);
  // visit a structure node

  int visit_typedef (be_typedef *node);
  // visit a typedef node

  int visit_union (be_union *node);
  // visit a union node

};

#endif // TAO_BE_VISITOR_OPERATION_H
