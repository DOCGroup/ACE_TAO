
//=============================================================================
/**
 *  @file    be_valuebox.h
 *
 *  $Id$
 *
 *  Extension of class AST_Valuebox that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Gary Maxey
 */
//=============================================================================

#ifndef TAO_BE_VALUEBOX_H
#define TAO_BE_VALUEBOX_H

#include "ast_valuebox.h"
#include "be_type.h"

class AST_Type;
class be_visitor;

class be_valuebox : public virtual AST_ValueBox,
                    public virtual be_type
{
public:
  be_valuebox ( AST_Type *boxed_type,
                UTL_ScopedName *n );

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);

  // Narrowing
  DEF_NARROW_FROM_DECL (be_valuebox);
};

#endif /* TAO_BE_VALUEBOX_H */
