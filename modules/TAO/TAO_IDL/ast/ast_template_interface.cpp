// $Id$

#include "ast_template_interface.h"
#include "ast_visitor.h"

#include "utl_identifier.h"
#include "utl_indenter.h"
#include "global_extern.h"

ACE_RCSID (ast,
           ast_template_interface,
           "$Id$")

AST_Template_Interface::AST_Template_Interface (
      UTL_ScopedName *n,
      AST_Interface **ih,
      long nih,
      AST_Interface **ih_flat,
      long nih_flat,
      const FE_Utils::T_PARAMLIST_INFO *template_params)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_interface,
              n),
    AST_Type (AST_Decl::NT_interface,
              n),
    UTL_Scope (AST_Decl::NT_interface),
    AST_Interface (n,
                   ih,
                   nih,
                   ih_flat,
                   nih_flat,
                   false,
                   false),
    template_params_ (*template_params)
{
}

AST_Template_Interface::~AST_Template_Interface (void)
{
}

FE_Utils::T_PARAMLIST_INFO &
AST_Template_Interface::template_params (void)
{
  return this->template_params_;
}

void
AST_Template_Interface::destroy (void)
{
  this->AST_Interface::destroy ();
}

void
AST_Template_Interface::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "interface ");

  this->local_name ()->dump (o);
  this->dump_i (o, "<");
  unsigned long index = 0UL;
  FE_Utils::T_Param_Info *item = 0;
  
  for (FE_Utils::T_PARAMLIST_INFO::ITERATOR iter (this->template_params_);
       !iter.done ();
       iter.advance (), ++index)
    {
      if (index != 0UL)
        {
          this->dump_i (o, ", ");
        }
        
      iter.next (item);
      
      this->dump_i (o, this->node_type_to_string (item->type_));
      this->dump_i (o, " ");
      this->dump_i (o, item->name_.c_str ());
    }
    
  this->dump_i (o, "> ");

  if (this->pd_n_inherits > 0)
    {
      this->dump_i (o, ": ");

      for (long i = 0; i < this->pd_n_inherits; ++i)
        {
          this->pd_inherits[i]->local_name ()->dump (o);
          this->dump_i (o, "<");
          
          AST_Template_Interface *ti =
            AST_Template_Interface::narrow_from_decl (pd_inherits[i]);
          index = 0UL;

          for (FE_Utils::T_PARAMLIST_INFO::ITERATOR iter (
                 ti->template_params_);
               !iter.done ();
               iter.advance (), ++index)
            {
              if (index != 0UL)
                {
                  this->dump_i (o, ",");
                }
                
              iter.next (item);
              this->dump_i (o, item->name_.c_str ());
            }
            
          this->dump_i (o, ">");
          
          if (i < this->pd_n_inherits - 1)
            {
              this->dump_i (o, ", ");
            }
        }
    }

  this->dump_i (o, " {\n");

  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);

  this->dump_i (o, "}\n");
}

int
AST_Template_Interface::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_template_interface (this);
}

IMPL_NARROW_FROM_DECL(AST_Template_Interface)
IMPL_NARROW_FROM_SCOPE(AST_Template_Interface)

