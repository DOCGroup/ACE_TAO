// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef _AST_VALUETYPE_AST_EVENTTYPE_HH
#define _AST_VALUETYPE_AST_EVENTTYPE_HH

#include "ast_valuetype.h"

class TAO_IDL_FE_Export AST_EventType : public virtual AST_ValueType
{
public:
  AST_EventType (void);

  AST_EventType (UTL_ScopedName *n,
                 AST_Interface **inherits,
                 long n_inherits,
                 AST_ValueType *inherits_concrete,
                 AST_Interface **inherits_flat,
                 long n_inherits_flat,
                 AST_Interface **supports,
                 long n_supports,
                 AST_Interface *supports_concrete,
                 idl_bool abstract,
                 idl_bool truncatable,
                 idl_bool custom);

  virtual ~AST_EventType (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.
  DEF_NARROW_METHODS1(AST_EventType, AST_ValueType);
  DEF_NARROW_FROM_DECL(AST_EventType);
  DEF_NARROW_FROM_SCOPE(AST_EventType);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);
};

#endif           // _AST_VALUETYPE_AST_EVENTTYPE_HH
