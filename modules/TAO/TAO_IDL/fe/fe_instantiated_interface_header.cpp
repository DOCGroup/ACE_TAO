// $Id$

#include "fe_instantiated_interface_header.h"
#include "fe_extern.h"
#include "ast_template_interface.h"
#include "utl_err.h"
#include "utl_namelist.h"
#include "global_extern.h"

FE_Instantiated_InterfaceHeader::FE_Instantiated_InterfaceHeader (
    UTL_ScopedName *n,
    AST_Template_Interface *template_ref,
    UTL_NameList *template_args)
  : name_ (n),
    template_ref_ (template_ref),
    template_args_ (template_args)
{
  this->match_args ();
}

FE_Instantiated_InterfaceHeader::~FE_Instantiated_InterfaceHeader (void)
{
}

void
FE_Instantiated_InterfaceHeader::destroy (void)
{
  this->name_->destroy ();
  delete this->name_;
  this->name_ = 0;
  
  this->template_args_->destroy ();
  delete this->template_args_;
  this->template_args_ = 0;
}

void
FE_Instantiated_InterfaceHeader::match_args (void)
{
}

