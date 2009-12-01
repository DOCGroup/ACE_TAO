// -*- C++ -*-
// $Id$

#ifndef AST_PUBLISHES_H
#define AST_PUBLISHES_H

#include "ast_field.h"

class AST_EventType;
class AST_Visitor;

class TAO_IDL_FE_Export AST_Publishes : public virtual AST_Field
{
public:
  AST_Publishes (UTL_ScopedName *n,
                 AST_EventType *publishes_type);
            
  virtual ~AST_Publishes (void);
  
  // Data Accessor.
  AST_EventType *publishes_type (void) const;

  // Narrowing.
  DEF_NARROW_FROM_DECL(AST_Publishes);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);
  
  // Cleanup.
  virtual void destroy (void);
};

#endif // AST_PUBLISHES_H

