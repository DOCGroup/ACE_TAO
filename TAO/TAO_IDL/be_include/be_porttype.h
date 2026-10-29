
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_porttype.h
 *
 *  Extension of class AST_PortType for C++ code generation.
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef BE_PORTTYPE_H
#define BE_PORTTYPE_H

#include "ast_porttype.h"

#include "be_scope.h"
#include "be_type.h"

class be_visitor;

/**
 * Extension of class AST_PortType for C++ code generation.
 */
class be_porttype : public virtual AST_PortType,
                    public virtual be_scope,
                    public virtual be_type
{
public:
  explicit be_porttype (UTL_ScopedName *n);

  ~be_porttype () override;

  void destroy () override;

  int accept (be_visitor *visitor) override;
};

#endif // BE_PORTTYPE_H
