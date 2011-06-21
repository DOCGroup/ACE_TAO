// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef AST_PORTTYPE_H
#define AST_PORTTYPE_H

#include "ast_type.h"
#include "utl_scope.h"

class AST_Provides;
class AST_Uses;
class AST_Publishes;
class AST_Emits;
class AST_Consumes;

class TAO_IDL_FE_Export AST_PortType
  : public virtual AST_Type,
    public virtual UTL_Scope
{
public:
  AST_PortType (UTL_ScopedName *n);

  virtual ~AST_PortType (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL (AST_PortType);
  DEF_NARROW_FROM_SCOPE (AST_PortType);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  static AST_Decl::NodeType const NT;

protected:
  virtual AST_Provides *fe_add_provides (AST_Provides *p);

  virtual AST_Uses *fe_add_uses (AST_Uses *p);

  virtual AST_Attribute *fe_add_attribute (AST_Attribute *a);
};

#endif           // AST_PORTTYPE_H
