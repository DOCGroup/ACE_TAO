
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_extended_port.h
 *
 *  Extension of class AST_Extended_Port for C++ code generation.
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef BE_EXTENDED_PORT_H
#define BE_EXTENDED_PORT_H

#include "ast_extended_port.h"

#include "be_field.h"
#include "be_porttype.h"

class be_visitor;

/**
 * Extension of class AST_Extended_Port for C++ code generation.
 */
class be_extended_port : public virtual AST_Extended_Port,
                         public virtual be_field
{
public:
  be_extended_port (
    UTL_ScopedName *n,
    AST_PortType *porttype_ref);

  virtual ~be_extended_port ();

  be_porttype *port_type () const;

  virtual void destroy ();

  virtual int accept (be_visitor *visitor);
};

#endif // BE_EXTENDED_PORT_H
