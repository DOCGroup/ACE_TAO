/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_param_holder.h
 *
 *  $Id$
 *
 *  Place holder referenced by AST_Field (or subclass)
 *  representing a corresponding template parameter in IDL.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_BE_PARAM_HOLDER_H
#define TAO_BE_PARAM_HOLDER_H

#include "be_type.h"

#include "ast_param_holder.h"

class be_param_holder : public virtual AST_Param_Holder,
                        public virtual be_type
{
public:
  be_param_holder (UTL_ScopedName *parameter_name,
                   FE_Utils::T_Param_Info *info);

  virtual ~be_param_holder (void);

  // Narrowing.
  DEF_NARROW_FROM_DECL (be_param_holder);

  // Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int accept (be_visitor *visitor);
};

#endif // TAO_BE_PARAM_HOLDER_H
