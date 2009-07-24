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
//    be_mirror_port.h
//
// = DESCRIPTION
//    Extension of class AST_Mirror_Port for C++ code generation.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef BE_MIRROR_PORT_H
#define BE_MIRROR_PORT_H

#include "ast_mirror_port.h"

#include "be_extended_port.h"

class be_visitor;

class be_mirror_port : public virtual AST_Mirror_Port,
                       public virtual be_extended_port
{
  //
  // = TITLE
  //   be_mirror_port
  //
  // = DESCRIPTION
  //   Extension of class AST_Mirror_Port for C++ code generation.
  //
public:
  be_mirror_port (
    UTL_ScopedName *n,
    AST_PortType *porttype_ref,
    UTL_NameList *template_args);
    
  virtual ~be_mirror_port (void);
  
  virtual void destroy (void);
  
  virtual int accept (be_visitor *visitor);

  DEF_NARROW_FROM_DECL (be_mirror_port);
};

#endif // BE_MIRROR_PORT_H
