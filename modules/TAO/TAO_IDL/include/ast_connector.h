// -*- C++ -*-
// $Id$

#ifndef AST_CONNECTOR_H
#define AST_CONNECTOR_H

#include "ast_component.h"
#include "fe_utils.h"

class AST_Tmpl_Port;
class AST_Tmpl_Mirror_Port;

class TAO_IDL_FE_Export AST_Connector
  : public virtual AST_Component
{
public:
  AST_Connector (void);

  AST_Connector (UTL_ScopedName *n,
                 AST_Connector *base_connector,
                 FE_Utils::T_PARAMLIST_INFO *template_params);

  virtual ~AST_Connector (void);
  
  AST_Connector *base_connector (void) const;

  // Cleanup function.
  virtual void destroy (void);
  
  // Narrowing.

  DEF_NARROW_FROM_DECL (AST_Connector);
  DEF_NARROW_FROM_SCOPE (AST_Connector);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);
  
protected:
  virtual AST_Tmpl_Port *fe_add_tmpl_port (AST_Tmpl_Port *p);
  virtual AST_Tmpl_Mirror_Port *fe_add_tmpl_mirror_port (
    AST_Tmpl_Mirror_Port *p);
  
private:
  FE_Utils::T_PARAMLIST_INFO *template_params_;
};

#endif // AST_CONNECTOR_H
