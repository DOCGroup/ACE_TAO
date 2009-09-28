// $Id$

#ifndef FE_INSTANTIATED_INTERFACE_HEADER_H
#define FE_INSTANTIATED_INTERFACE_HEADER_H

// FE_Instantiated_InterfaceHeader
// Internal class for FE to describe template interface instantiations.

#include "utl_scoped_name.h"

class UTL_NameList;
class AST_Instantiated_Interface;
class AST_Template_Interface;

class TAO_IDL_FE_Export FE_Instantiated_InterfaceHeader
{
public:
  FE_Instantiated_InterfaceHeader (
    UTL_ScopedName *n,
    AST_Template_Interface *template_ref,
    UTL_NameList *template_args);

  virtual ~FE_Instantiated_InterfaceHeader (void);

  // Destroy anything allocated for this class.
  virtual void destroy (void);
  
private:
  void match_args (void);

  // Data.
private:
  UTL_ScopedName *name_;
  AST_Template_Interface *template_ref_;
  UTL_NameList *template_args_;
};

#endif           // FE_INSTANTIATED_INTERFACE_HEADER_H
