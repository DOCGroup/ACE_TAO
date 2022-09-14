#include "ast_template_module_inst.h"
#include "ast_template_module.h"
#include "ast_visitor.h"

AST_Decl::NodeType const
AST_Template_Module_Inst::NT = AST_Decl::NT_module;

AST_Template_Module_Inst::AST_Template_Module_Inst (
      UTL_ScopedName *n,
      AST_Template_Module *ref,
      FE_Utils::T_ARGLIST *template_args)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_module,
              n),
    AST_Field (AST_Decl::NT_module,
               ref,
               n),
    template_args_ (template_args)
{
}

AST_Template_Module_Inst::~AST_Template_Module_Inst ()
{
}

AST_Template_Module *
AST_Template_Module_Inst::ref () const
{
  return dynamic_cast<AST_Template_Module*> (this->field_type ());
}

FE_Utils::T_ARGLIST const *
AST_Template_Module_Inst::template_args () const
{
  return this->template_args_;
}

void
AST_Template_Module_Inst::dump (ACE_OSTREAM_TYPE &)
{
  // TODO
}

void
AST_Template_Module_Inst::destroy ()
{
  this->template_args_->destroy ();
  delete this->template_args_;
  this->template_args_ = nullptr;

  this->AST_Field::destroy ();
}

int
AST_Template_Module_Inst::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_template_module_inst (this);
}
