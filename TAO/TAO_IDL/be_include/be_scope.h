/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_scope.h
//
// = DESCRIPTION
//    Extension of the UTL_Scope CFE class
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef TAO_BE_SCOPE_H
#define TAO_BE_SCOPE_H

class be_decl;

/*
 * BE_Scope
 */
class be_scope : public virtual UTL_Scope
{
  // = TITLE
  //   be_scope
  // = DESCRIPTION
  //   The back end extension of the UTL_Scope class
  //
public:
  // Operations

  be_scope (void);
  // Default constructor

  be_scope (AST_Decl::NodeType nt);
  // constructor that sets the node type

  virtual ~be_scope (void);
  // destructor

  virtual int gen_encapsulation (void);
  // encapsulation for parameters

  virtual long tc_encap_len (void);
  // return length of encapsulation

  virtual void comma (unsigned short set);
  // set the comma producing state

  int comma (void) const;
  // get the comma producing state

  virtual be_decl *decl (void);
  // return the be_decl node corresponding to this scope node

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS1 (be_scope, UTL_Scope);
  DEF_NARROW_FROM_SCOPE (be_scope);

private:
  unsigned short comma_;
  // if set, generate a comma after every element is handled
};

#endif // if !defined
