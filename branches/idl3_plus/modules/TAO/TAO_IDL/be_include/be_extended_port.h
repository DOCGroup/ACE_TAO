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
//    be_extended_port.h
//
// = DESCRIPTION
//    Extension of class AST_Extended_Port for C++ code generation.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef BE_EXTENDED_PORT_H
#define BE_EXTENDED_PORT_H

#include "ast_extended_port.h"

#include "be_field.h"

class be_visitor;

class be_extended_port : public virtual AST_Extended_Port,
                         public virtual be_field
{
  //
  // = TITLE
  //   be_extended_port
  //
  // = DESCRIPTION
  //   Extension of class AST_Extended_Port for C++ code generation.
  //
public:
  be_extended_port (
    UTL_ScopedName *n,
    AST_PortType *porttype_ref,
    UTL_NameList *template_args);
    
  virtual ~be_extended_port (void);
  
  virtual void destroy (void);
  
  virtual int accept (be_visitor *visitor);

  DEF_NARROW_FROM_DECL (be_extended_port);
};

#endif // BE_EXTENDED_PORT_H
