//
// $Id$
//

/* -*- C++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_context.h
//
// = DESCRIPTION
//    This class serves to provide all the necessary context information to the
//    visitor as it generates code. This may include the scope inside which the
//    code is generated, the output file that is being used, and others. This
//    context information will be passed to the abstract/concrete visitor
//    factory which in turn will use it to construct the right visitor.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_CONTEXT_H)
#define TAO_BE_VISITOR_CONTEXT_H

class be_visitor_context
{
public:
  be_visitor_context (void);
  // default constructor

  be_visitor_context (const be_visitor_context &ctx);
  // default constructor

  ~be_visitor_context (void);
  // destructor

  be_visitor_context &operator= (const be_visitor_context &ctx);
  // assignment operator

  // =helpers

  void stream (TAO_OutStream *os);
  // set the output stream

  TAO_OutStream *stream (void);
  // get the stream

  void scope (be_decl *scope);
  // set the scope

  be_decl *scope (void);
  // get the scope in which code will be generated

  void node (be_decl *node);
  // current node being visited

  be_decl *node (void);
  // node being visited

  void state (TAO_CodeGen::CG_STATE);
  // code generation state

  TAO_CodeGen::CG_STATE state (void);
  // return the code generation state

  void alias (be_typedef *node);
  // set the alias node (if any)

  be_typedef *alias (void);
  // return the aliased node (if any)

  void tdef (be_typedef *node);
  // set the typedef node (if any)

  be_typedef *tdef (void);
  // return the typedef node (if any)

  void attribute (be_attribute *attr);
  // set the attribute node if any

  be_attribute *attribute (void);
  // get the attribute node

  void comma (idl_bool);
  // scope supoorts generation of comma after each element

  idl_bool comma (void);
  // are we supposed to generate a comma?

  // = helpers

  // visitors may need the actual type of the node stored in this context. We
  // could have kept separate nodes but that would have increased the size of
  // the context class by a large extent. So we will rely on the
  // "narrow_from_decl" approach

  // two types of helpers are provided here. One of them returns the node as
  // the desired actual type. The other returns the scope as the desired type.

  be_argument *be_node_as_argument (void);
  // return the node as argument if possible

  be_array *be_node_as_array (void);
  // return the node as array if possible

  be_attribute *be_node_as_attribute (void);
  // return the node as attribute if possible

  be_constant *be_node_as_constant (void);
  // return the node as constant if possible

  be_enum *be_node_as_enum (void);
  // return the node as enum if possible

  be_enum_val *be_node_as_enum_val (void);
  // return the node as enum_val if possible

  be_exception *be_node_as_exception (void);
  // return the node as exception if possible

  be_field *be_node_as_field (void);
  // return the node as field if possible

  be_interface *be_node_as_interface (void);
  // return the node as interface if possible

  be_interface_fwd *be_node_as_interface_fwd (void);
  // return the node as interface_fwd if possible

  be_module *be_node_as_module (void);
  // return the node as module if possible

  be_operation *be_node_as_operation (void);
  // return the node as operation if possible

  be_predefined_type *be_node_as_predefined_type (void);
  // return the node as predefined_type if possible

  be_root *be_node_as_root (void);
  // return the node as root if possible

  be_sequence *be_node_as_sequence (void);
  // return the node as sequence if possible

  be_string *be_node_as_string (void);
  // return the node as string if possible

  be_structure *be_node_as_structure (void);
  // return the node as structure if possible

  be_typedef *be_node_as_typedef (void);
  // return the node as typedef if possible

  be_union *be_node_as_union (void);
  // return the node as union if possible

  be_union_branch *be_node_as_union_branch (void);
  // return the node as union_branch if possible

  // for scope

  be_enum *be_scope_as_enum (void);
  // return the scope as enum if possible

  be_exception *be_scope_as_exception (void);
  // return the scope as exception if possible

  be_interface *be_scope_as_interface (void);
  // return the scope as interface if possible

  be_module *be_scope_as_module (void);
  // return the scope as module if possible

  be_operation *be_scope_as_operation (void);
  // return the scope as operation if possible

  be_root *be_scope_as_root (void);
  // return the scope as root if possible

  be_structure *be_scope_as_structure (void);
  // return the scope as structure if possible

  be_union *be_scope_as_union (void);
  // return the scope as union if possible

private:
  TAO_CodeGen::CG_STATE state_;
  // code generation state

  TAO_OutStream *os_;
  // current output stream

  be_decl *scope_;
  // current scope in which code is generated

  be_decl *node_;
  // current node

  be_typedef *alias_;
  // aliased node used as a scoped name to define IDL types.

  be_typedef *tdef_;
  // typedef node

  be_attribute *attr_;
  // attribute node stored here while generating its code

  idl_bool comma_;
  // whether scope should generate a comma after every element
};


#endif /* BE_VISITOR_CONTEXT_H */
