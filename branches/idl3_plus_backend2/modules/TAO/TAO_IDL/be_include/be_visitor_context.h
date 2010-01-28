//
// $Id$
//

/* -*- C++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
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

#ifndef TAO_BE_VISITOR_CONTEXT_H
#define TAO_BE_VISITOR_CONTEXT_H

#include "ast_visitor_context.h"

#include "fe_utils.h"

#include "be_codegen.h"

class TAO_OutStream;
class be_decl;
class be_interface;
class be_typedef;
class be_attribute;
class be_scope;

// When interface is defined as macro redefine it as aceinterface
#if defined (interface)
# define aceinterface interface
# undef interface
#endif /* interface */

class be_visitor_context : public ast_visitor_context
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

  void reset (void);
  // reset the context

  void stream (TAO_OutStream *os);
  // set the output stream

  TAO_OutStream *stream (void);
  // get the stream

  void scope (be_scope *scope);
  // set the scope

  be_scope *scope (void);
  // get the scope in which code will be generated

  void node (be_decl *node);
  // current node being visited

  be_decl *node (void);
  // node being visited

  void state (TAO_CodeGen::CG_STATE);
  // code generation state

  TAO_CodeGen::CG_STATE state (void);
  // return the code generation state

  void sub_state (TAO_CodeGen::CG_SUB_STATE);
  // code generation state

  TAO_CodeGen::CG_SUB_STATE sub_state (void);
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

  void exception (bool);
  // is it the special ctor for exceptions?

  bool exception (void);
  // is it the special ctor for exceptions?

  void comma (bool);
  // scope supoorts generation of comma after each element

  bool comma (void);
  // are we supposed to generate a comma?

  be_interface *interface (void) const;
  void interface (be_interface *);
  // In some cases we need to generate the objects of one interface as
  // members of a derived visitor (the TIE classes are a good
  // example). This field is use to hold the interface where we are
  // *generating* code, as opposed to the interfaces where the methods
  // or structures are defined.
  
  const char *export_macro (void) const;
  // return the export macro currently in force

  const char *non_null_export_macro (void) const;
  // if anyop export macro is empty, check stub.

private:
  TAO_CodeGen::CG_STATE state_;
  // code generation state

  TAO_CodeGen::CG_SUB_STATE sub_state_;
  // code generation state

  TAO_OutStream *os_;
  // current output stream

  be_scope *scope_;
  // current scope in which code is generated

  be_decl *node_;
  // current node

  be_typedef *alias_;
  // aliased node used as a scoped name to define IDL types.

  be_typedef *tdef_;
  // typedef node

  be_attribute *attr_;
  // attribute node stored here while generating its code

  bool exception_;
  // is it the special ctor for exceptions?

  bool comma_;
  // whether scope should generate a comma after every element

  be_interface *interface_;
  // See the method declaration above.
};

#if defined (interface)
# define interface aceinterface
#endif /* interface */

#endif /* BE_VISITOR_CONTEXT_H */
