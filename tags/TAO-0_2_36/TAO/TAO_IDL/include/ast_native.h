// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef _AST_NATIVE_H_
#define _AST_NATIVE_H_

// Representation of "native" IDL type added by the POA spec
//
/*
** DEPENDENCIES: ast_decl.h
**
** USE: Included from ast.h
*/

class AST_Native : public virtual AST_Type
{
public:
  // =Operations

  // Constructor(s)
  AST_Native (void);
  // default constructor

  AST_Native(UTL_ScopedName *n, UTL_StrList *p);
  // constructor that initializes its scoped name

  virtual ~AST_Native (void) {}
  // destructor

  // Narrowing
  DEF_NARROW_METHODS1(AST_Native, AST_Type);
  DEF_NARROW_FROM_DECL(AST_Native);

  // AST Dumping
  virtual void          dump(ostream &o);
};

#endif /* AST_NATIVE_H */
