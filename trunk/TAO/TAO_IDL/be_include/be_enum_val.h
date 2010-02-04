/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_enum_val.h
 *
 *  $Id$
 *
 *  Extension of class AST_EnumVal that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================


#ifndef BE_ENUM_VAL_H
#define BE_ENUM_VAL_H

#include "be_decl.h"
#include "ast_enum_val.h"

class be_visitor;

class be_enum_val : public virtual AST_EnumVal,
                    public virtual be_decl
{
public:
  /// Default constructor.
  be_enum_val (void);

  /// Constructor.
  be_enum_val (ACE_CDR::ULong v,
               UTL_ScopedName *n);

  // Visiting
  virtual int accept (be_visitor *visitor);

  //Cleanup.
  virtual void destroy (void);

  // Narrowing

  DEF_NARROW_FROM_DECL (be_enum_val);
};

#endif
