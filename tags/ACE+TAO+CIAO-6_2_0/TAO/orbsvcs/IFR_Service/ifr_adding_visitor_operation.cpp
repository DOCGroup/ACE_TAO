/* -*- c++ -*- */
// $Id$

#include "orbsvcs/Log_Macros.h"
#include "ast_argument.h"
#include "ast_exception.h"
#include "ast_expression.h"
#include "ast_operation.h"
#include "utl_identifier.h"
#include "utl_string.h"

#include "ifr_adding_visitor_operation.h"
#include "utl_exceptlist.h"
#include "utl_strlist.h"
#include "nr_extern.h"

ifr_adding_visitor_operation::ifr_adding_visitor_operation (AST_Decl *scope)
  : ifr_adding_visitor (scope),
    index_ (0)
{
}

ifr_adding_visitor_operation::~ifr_adding_visitor_operation (void)
{
}

int
ifr_adding_visitor_operation::visit_operation (AST_Operation *node)
{
  try
    {
      // If this operation is already in the repository (for example, if
      // we are processing the IDL file a second time inadvertently), we
      // just return 0. The IDL file must be legal, otherwise the IDL
      // compiler front end would have told us.

      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ());

      if (!CORBA::is_nil (prev_def.in ()))
        {
          return 0;
        }

      // Build the parameter list. Our overridden version of visit_argument
      // will look up each parameter and add its repository entry to
      // our params_ member.

      CORBA::ULong length = static_cast<CORBA::ULong> (node->argument_count ());

      this->params_.length (length);

      if (this->visit_scope (node) == -1)
        {
          ORBSVCS_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor_operation::")
              ACE_TEXT ("visit_operation -")
              ACE_TEXT (" visit_scope failed\n")
            ),
            -1
          );
        }

      this->index_ = 0;

      // Build the exception list.

      UTL_ExceptList *excepts = node->exceptions ();

      if (excepts != 0)
        {
          length = static_cast<CORBA::ULong> (excepts->length ());
        }
      else
        {
          length = 0;
        }

      CORBA::ExceptionDefSeq exceptions (length);
      exceptions.length (length);

      AST_Type *ex = 0;
      CORBA::ULong i = 0;

      for (UTL_ExceptlistActiveIterator ex_iter (excepts);
           !ex_iter.is_done ();
           ex_iter.next (), ++i)
        {
          ex = ex_iter.item ();

          prev_def =
            be_global->repository ()->lookup_id (ex->repoID ());

          exceptions[i] =
            CORBA::ExceptionDef::_narrow (prev_def.in ());
        }

      // Build the context list.

      UTL_StrList *ctx_list = node->context ();

      if (ctx_list != 0)
        {
          length = static_cast<CORBA::ULong> (ctx_list->length ());
        }
      else
        {
          length = 0;
        }

      CORBA::ContextIdSeq contexts (length);
      contexts.length (length);

      UTL_StrlistActiveIterator ctx_iter (ctx_list);
      UTL_String *str = 0;
      i = 0;

      while (!ctx_iter.is_done ())
        {
          str = ctx_iter.item ();

          contexts[i++] = str->get_string ();

          ctx_iter.next ();
        }

      // Get the return type.

      AST_Type *return_type = node->return_type ();

      // Updates ir_current_.
      this->get_referenced_type (return_type);

      // Is the operation oneway?
      CORBA::OperationMode mode = node->flags () == AST_Operation::OP_oneway
                                 ? CORBA::OP_ONEWAY
                                 : CORBA::OP_NORMAL;

      // Create the repository entry.

      CORBA::Container_ptr current_scope =
        CORBA::Container::_nil ();

      if (be_global->ifr_scopes ().top (current_scope) == 0)
        {
          AST_Decl *op_scope = ScopeAsDecl (node->defined_in ());
          AST_Decl::NodeType nt = op_scope->node_type ();

          if (nt == AST_Decl::NT_interface)
            {
              CORBA::InterfaceDef_var iface =
                CORBA::InterfaceDef::_narrow (current_scope);

              CORBA::OperationDef_var new_def =
                iface->create_operation (node->repoID (),
                                         node->local_name ()->get_string (),
                                         node->version (),
                                         this->ir_current_.in (),
                                         mode,
                                         this->params_,
                                         exceptions,
                                         contexts);
            }
          else
            {
              CORBA::ValueDef_var vtype =
                CORBA::ValueDef::_narrow (current_scope);

              CORBA::OperationDef_var new_def =
                vtype->create_operation (node->repoID (),
                                         node->local_name ()->get_string (),
                                         node->version (),
                                         this->ir_current_.in (),
                                         mode,
                                         this->params_,
                                         exceptions,
                                         contexts);
            }
        }
      else
        {
          ORBSVCS_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor_operation::")
              ACE_TEXT ("visit_operation -")
              ACE_TEXT (" scope stack is empty\n")
            ),
            -1
          );
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT (
          "ifr_adding_visitor_operation::visit_operation"));

      return -1;
    }

  return 0;
}

int
ifr_adding_visitor_operation::visit_argument (AST_Argument *node)
{
  // Get the parameter's name.
  this->params_[this->index_].name =
    CORBA::string_dup (node->local_name ()->get_string ());

  AST_Type *arg_type = node->field_type ();

  try
    {
      // Updates ir_current_.
      this->get_referenced_type (arg_type);

      this->params_[this->index_].type_def =
        CORBA::IDLType::_duplicate (this->ir_current_.in ());


      switch (node->direction ())
      {
        case AST_Argument::dir_IN:
          this->params_[this->index_].mode = CORBA::PARAM_IN;
          break;
        case AST_Argument::dir_OUT:
          this->params_[this->index_].mode = CORBA::PARAM_OUT;
          break;
        case AST_Argument::dir_INOUT:
          this->params_[this->index_].mode = CORBA::PARAM_INOUT;
          break;
      }

      // IfR method create_operation does not use this - it just needs
      // to be non-zero for marshaling.
      this->params_[this->index_].type =
        CORBA::TypeCode::_duplicate (CORBA::_tc_void);

      ++this->index_;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT (
          "ifr_adding_visitor_operation::visit_argument"));

      return -1;
    }

  return 0;
}
