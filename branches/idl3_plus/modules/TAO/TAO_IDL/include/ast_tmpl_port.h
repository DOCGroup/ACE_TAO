#ifndef AST_TMPL_PORT_H
#define AST_TMPL_PORT_H

#include "ast_field.h"
#include "ast_porttype.h"

class TAO_IDL_FE_Export AST_Tmpl_Port
  : public virtual AST_Field
{
public:
  AST_Tmpl_Port (
    UTL_ScopedName *n,
    AST_PortType *porttype_ref);

  virtual ~AST_Tmpl_Port (void);

  AST_PortType *port_type (void) const;

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL (AST_Tmpl_Port);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);
};

#endif /* AST_TMPL_PORT_H */

