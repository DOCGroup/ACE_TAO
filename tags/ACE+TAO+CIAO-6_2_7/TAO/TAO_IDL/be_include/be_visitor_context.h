
/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_context.h
 *
 *  $Id$
 *
 *  This class serves to provide all the necessary context information to the
 *  visitor as it generates code. This may include the scope inside which the
 *  code is generated, the output file that is being used, and others. This
 *  context information will be passed to the abstract/concrete visitor
 *  factory which in turn will use it to construct the right visitor.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

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
  /// default constructor
  be_visitor_context (void);

  /// default constructor
  be_visitor_context (const be_visitor_context &ctx);

  /// destructor
  ~be_visitor_context (void);

  /// assignment operator
  be_visitor_context &operator= (const be_visitor_context &ctx);

  // =helpers

  /// reset the context
  void reset (void);

  /// set the output stream
  void stream (TAO_OutStream *os);

  /// get the stream
  TAO_OutStream *stream (void);

  /// set the scope
  void scope (be_scope *scope);

  /// get the scope in which code will be generated
  be_scope *scope (void);

  /// current node being visited
  void node (be_decl *node);

  /// node being visited
  be_decl *node (void);

  /// code generation state
  void state (TAO_CodeGen::CG_STATE);

  /// return the code generation state
  TAO_CodeGen::CG_STATE state (void);

  /// code generation state
  void sub_state (TAO_CodeGen::CG_SUB_STATE);

  /// return the code generation state
  TAO_CodeGen::CG_SUB_STATE sub_state (void);

  /// set the alias node (if any)
  void alias (be_typedef *node);

  /// return the aliased node (if any)
  be_typedef *alias (void);

  /// set the typedef node (if any)
  void tdef (be_typedef *node);

  /// return the typedef node (if any)
  be_typedef *tdef (void);

  /// set the attribute node if any
  void attribute (be_attribute *attr);

  /// get the attribute node
  be_attribute *attribute (void);

  /// is it the special ctor for exceptions?
  void exception (bool);

  /// is it the special ctor for exceptions?
  bool exception (void);

  /// scope supoorts generation of comma after each element
  void comma (bool);

  /// are we supposed to generate a comma?
  bool comma (void);

  /**
   * In some cases we need to generate the objects of one interface as
   * members of a derived visitor (the TIE classes are a good
   * example). This field is use to hold the interface where we are
   * *generating* code, as opposed to the interfaces where the methods
   * or structures are defined.
   */
  be_interface *interface (void) const;
  void interface (be_interface *);

  /// Accessor for the member.
  ACE_CString &port_prefix (void);

  /// return the export macro currently in force
  const char *export_macro (void) const;

  /// if anyop export macro is empty, check stub.
  const char *non_null_export_macro (void) const;

private:
  /// code generation state
  TAO_CodeGen::CG_STATE state_;

  /// code generation state
  TAO_CodeGen::CG_SUB_STATE sub_state_;

  /// current output stream
  TAO_OutStream *os_;

  /// current scope in which code is generated
  be_scope *scope_;

  /// current node
  be_decl *node_;

  /// aliased node used as a scoped name to define IDL types.
  be_typedef *alias_;

  /// typedef node
  be_typedef *tdef_;

  /// attribute node stored here while generating its code
  be_attribute *attr_;

  /// is it the special ctor for exceptions?
  bool exception_;

  /// whether scope should generate a comma after every element
  bool comma_;

  /// See the method declaration above.
  be_interface *interface_;

  /// Used in enough visitors to put it here.
  ACE_CString port_prefix_;
};

#if defined (interface)
# define interface aceinterface
#endif /* interface */

#endif /* BE_VISITOR_CONTEXT_H */
