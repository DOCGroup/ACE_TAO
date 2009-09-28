// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    be_template_interface.h
//
// = DESCRIPTION
//    Extension of class AST_Template_Interface that provides additional means for C++
//    mapping of an interface.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef TAO_BE_TEMPLATE_INTERFACE_H
#define TAO_BE_TEMPLATE_INTERFACE_H

#include "be_scope.h"
#include "be_type.h"
#include "ast_template_interface.h"

class be_visitor;
class be_interface_strategy;

class be_template_interface : public virtual AST_Template_Interface,
                              public virtual be_scope,
                              public virtual be_type
{
  // = TITLE
  //   The back end extension of the AST_Template_Interface class
  //
  // = DESCRIPTION
  //
public:
  be_template_interface (
    UTL_ScopedName *n,
    AST_Interface **ih,
    long nih,
    AST_Interface **ih_flat,
    long nih_flat,
    FE_Utils::T_PARAMLIST_INFO *template_params);

  virtual ~be_template_interface (void);

  virtual void destroy (void);
  
  virtual int accept (be_visitor *visitor);

  DEF_NARROW_FROM_DECL (be_template_interface);
};

#endif /* TAO_BE_TEMPLATE_INTERFACE_H */