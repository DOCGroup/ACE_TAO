// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_argument.h
//
// = DESCRIPTION
//    Extension of class AST_Argument that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_ARGUMENT_H
#define BE_ARGUMENT_H

/*
 * BE_Argument
 */
class be_argument : public virtual AST_Argument,
                    public virtual be_decl
{
public:
  // =Operations

  be_argument (void);
  // default constructor

  be_argument (AST_Argument::Direction d, AST_Type *ft, UTL_ScopedName *n,
               UTL_StrList *p);
  // constructor

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS2 (be_argument, AST_Argument, be_decl);
  DEF_NARROW_FROM_DECL (be_argument);
};

#endif
