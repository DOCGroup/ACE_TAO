// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_attribute.h
//
// = DESCRIPTION
//    Extension of class AST_Attribute that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_ATTRIBUTE_H
#define BE_ATTRIBUTE_H

/*
 * BE_Attribute
 */
class be_attribute : public virtual AST_Attribute,
                     public virtual be_decl
{
public:
  // =Operations

  be_attribute (void);
  // default constructor

  be_attribute (idl_bool ro, AST_Type *ft, UTL_ScopedName *n, UTL_StrList *p);
  // constructor

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS2 (be_attribute, AST_Attribute, be_decl);
  DEF_NARROW_FROM_DECL (be_attribute);
};

#endif
