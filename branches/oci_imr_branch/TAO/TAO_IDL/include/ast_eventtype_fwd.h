// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef _AST_VALUETYPE_FWD_AST_EVENTTYPE_FWD_HH
#define _AST_VALUETYPE_FWD_AST_EVENTTYPE_FWD_HH

#include "ast_valuetype_fwd.h"

// Representation of a forward eventtype declaration.

class TAO_IDL_FE_Export AST_EventTypeFwd : public virtual AST_ValueTypeFwd
{
public:
  AST_EventTypeFwd (void);

  AST_EventTypeFwd (AST_Interface *dummy,
                    UTL_ScopedName *n);

  virtual ~AST_EventTypeFwd (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.
  DEF_NARROW_METHODS1(AST_EventTypeFwd, AST_ValueTypeFwd);
  DEF_NARROW_FROM_DECL(AST_EventTypeFwd);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);
};

#endif           // _AST_VALUETYPE_FWD_AST_EVENTTYPE_FWD_HH
