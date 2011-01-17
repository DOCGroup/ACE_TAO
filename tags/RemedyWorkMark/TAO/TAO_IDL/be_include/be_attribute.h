
//=============================================================================
/**
 *  @file    be_attribute.h
 *
 *  $Id$
 *
 *  Extension of class AST_Attribute that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#ifndef BE_ATTRIBUTE_H
#define BE_ATTRIBUTE_H

#include "ast_attribute.h"
#include "be_field.h"

class AST_Type;
class be_visitor;

class be_attribute : public virtual AST_Attribute,
                     public virtual be_field
{
public:
  be_attribute (bool ro,
                AST_Type *ft,
                UTL_ScopedName *n,
                bool local,
                bool abstract);

  // Non-virtual override of frontend method.
  be_type *field_type (void) const;

  // Visiting.
  virtual int accept (be_visitor *visitor);

  /// Cleanup.
  virtual void destroy (void);

  /// Sets the original attribute from which this one was created,
  /// applies only to implied IDL.
  void original_attribute (be_attribute *original_attribute);

  /// Returns the original attribute from which this one was created,
  /// applies only to implied IDL
  be_attribute *original_attribute ();

  // Narrowing

  DEF_NARROW_FROM_DECL (be_attribute);
};

#endif
