/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_enum.h
//
// = DESCRIPTION
//    Extension of class AST_Enum that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_ENUM_H
#define BE_ENUM_H

/*
 * BE_Enum
 */
class be_enum : public virtual AST_Enum,
                public virtual be_scope,
                public virtual be_type
{
public:
  // Operations

  be_enum (void);
  // default constructor

  be_enum (UTL_ScopedName *n, UTL_StrList *p);
  // constructor

  virtual int member_count (void);
  // return the count of members

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS3 (be_enum, AST_Enum, be_scope, be_type);
  DEF_NARROW_FROM_DECL (be_enum);
  DEF_NARROW_FROM_SCOPE (be_enum);

private:
  //=helper

  int compute_member_count (void);
  // count the number of members

  int member_count_;
  // number of members
};

#endif
