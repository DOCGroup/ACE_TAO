// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_field.h
//
// = DESCRIPTION
//    Extension of class AST_Field that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_FIELD_H
#define BE_FIELD_H

/*
 * BE_Field
 */
class be_field : public virtual AST_Field,
                 public virtual be_decl
{
public:
  // =Operations

  be_field (void);
  // default constructor

  be_field (AST_Type *ft, UTL_ScopedName *n, UTL_StrList *p,
                                    Visibility vis = vis_NA);
  // constructor

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS2 (be_field, AST_Field, be_decl);
  DEF_NARROW_FROM_DECL (be_field);

protected:
  virtual int compute_size_type (void);
  // compute the size type if it is unknown
};

#endif
