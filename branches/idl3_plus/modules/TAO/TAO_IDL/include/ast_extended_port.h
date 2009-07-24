// -*- C++ -*-
// $Id$

#ifndef AST_EXTENDED_PORT_H
#define AST_EXTENDED_PORT_H

#include "ast_field.h"

class AST_PortType;
class UTL_NameList;

class TAO_IDL_FE_Export AST_Extended_Port
  : public virtual AST_Field
{
public:
  AST_Extended_Port (
    UTL_ScopedName *n,
    AST_PortType *porttype_ref,
    UTL_NameList *template_args);

  virtual ~AST_Extended_Port (void);
  
  AST_PortType *port_type (void) const;
  UTL_NameList *template_args (void) const;

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL (AST_Extended_Port);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

protected:
  UTL_NameList *template_args_;
};

#endif /* AST_EXTENDED_PORT_H */

