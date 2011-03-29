/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_eventtype.h
 *
 *  $Id$
 *
 *  Extension of class AST_EventType and be_valuetype that provides
 *  additional means for C++ mapping of an valuetype.
 *
 *
 */
//=============================================================================

#ifndef TAO_BE_EVENTTYPE_H
#define TAO_BE_EVENTTYPE_H

#include "be_valuetype.h"
#include "ast_eventtype.h"

/**
 * Backend-class for eventtypes
 *
 * Extends be_valuetype.
 */
class be_eventtype : public virtual be_valuetype,
                     public virtual AST_EventType
{
public:
  be_eventtype (UTL_ScopedName *n,
                AST_Type **inherits,
                long n_inherits,
                AST_Type *inherits_concrete,
                AST_Interface **inherits_flat,
                long n_inherits_flat,
                AST_Type **supports,
                long n_supports,
                AST_Type *supports_concrete,
                bool abstract,
                bool truncatable,
                bool custom);

  ~be_eventtype (void);

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);

  // Narrowing.
  DEF_NARROW_FROM_DECL (be_eventtype);
  DEF_NARROW_FROM_SCOPE (be_eventtype);
};

#endif  // if !defined
