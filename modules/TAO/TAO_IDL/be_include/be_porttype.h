//
// $Id$
//

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    be_porttype.h
//
// = DESCRIPTION
//    Extension of class AST_PortType for C++ code generation.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef BE_PORTTYPE_H
#define BE_PORTTYPE_H

#include "ast_porttype.h"

class be_visitor;

class be_porttype : public virtual AST_PortType
{
  //
  // = TITLE
  //   be_porttype
  //
  // = DESCRIPTION
  //   Extension of class AST_PortType for C++ code generation.
  //
public:
  be_porttype (
    UTL_ScopedName *n,
    const FE_Utils::T_PARAMLIST_INFO *template_params);
    
  virtual ~be_porttype (void);
  
  virtual void destroy (void);
  
  virtual int accept (be_visitor *visitor);
};

#endif // BE_PORTTYPE_H
