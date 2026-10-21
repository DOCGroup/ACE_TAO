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

  ~AST_PortType () override;

  // Cleanup function.
  void destroy () override;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &o) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  static AST_Decl::NodeType const NT;

protected:
  AST_Provides *fe_add_provides (AST_Provides *p) override;

  AST_Uses *fe_add_uses (AST_Uses *p) override;

  AST_Attribute *fe_add_attribute (AST_Attribute *a) override;
};

#endif           // AST_PORTTYPE_H
