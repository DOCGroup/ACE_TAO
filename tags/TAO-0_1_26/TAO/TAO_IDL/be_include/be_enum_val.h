/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_enum_val.h
//
// = DESCRIPTION
//    Extension of class AST_EnumVal that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (BE_ENUM_VAL_H)
#define BE_ENUM_VAL_H

/*
 * BE_EnumVal
 */
class be_enum_val : public virtual AST_EnumVal,
                    public virtual be_decl
{
public:
  // =Operations

  be_enum_val (void);
  // default constructor

  be_enum_val (unsigned long v, UTL_ScopedName *n, UTL_StrList *p);
  // constructor

  virtual int gen_encapsulation (void);
  // generate the typecode

  virtual long tc_encap_len (void);
  // return length of encapsulation

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS2 (be_enum_val, AST_EnumVal, be_decl);
  DEF_NARROW_FROM_DECL (be_enum_val);
};

#endif
