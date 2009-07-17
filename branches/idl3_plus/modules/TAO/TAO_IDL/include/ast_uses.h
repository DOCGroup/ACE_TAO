// -*- C++ -*-
// $Id$

#ifndef AST_USES_H
#define AST_USES_H

#include "ast_field.h"

class AST_Interface;
class AST_Visitor;

class TAO_IDL_FE_Export AST_Uses : public virtual AST_Field
{
public:
  AST_Uses (UTL_ScopedName *n,
                AST_Interface *uses_type);
            
  virtual ~AST_Uses (void);
  
  // Data Accessor.
  AST_Interface *uses_type (void) const;

  // Narrowing.
  DEF_NARROW_FROM_DECL(AST_Uses);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);
  
  // Cleanup.
  virtual void destroy (void);
};

#endif // AST_USES_H

