/* -*- c++ -*- */
// $Id$

#include "ast_argument.h"
#include "ast_exception.h"
#include "ast_expression.h"
#include "ast_operation.h"
#include "utl_identifier.h"
#include "utl_string.h"

#include "ifr_adding_visitor_operation.h"
#include "utl_exceptlist.h"
#include "utl_strlist.h"

ACE_RCSID (IFR_Service,
           ifr_adding_visitor_operation,
           "$Id$")

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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // If this operation is already in the repository (for example, if
      // we are processing the IDL file a second time inadvertently), we
      // just return 0. The IDL file must be legal, otherwise the IDL
      // compiler front end would have told us.

      CORBA::Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!CORBA::is_nil (prev_def.in ()))
        {
          return 0;
        }

      // Build the parameter list. Our overridden version of visit_argument
      // will look up each parameter and add its repository entry to
      // our params_ member.

      CORBA::ULong length = ACE_static_cast (CORBA::ULong,
                                             node->argument_count ());

      this->params_.length (length);

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((
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
          length = ACE_static_cast (CORBA::ULong,
                                    excepts->length ());
        }
      else
        {
          length = 0;
        }

      CORBA::ExceptionDefSeq exceptions (length);
      exceptions.length (length);

      UTL_ExceptlistActiveIterator ex_iter (excepts);
      AST_Exception *ex = 0;
      CORBA::ULong i = 0;

      while (!ex_iter.is_done ())
        {
          ex = ex_iter.item ();

          prev_def =
            be_global->repository ()->lookup_id (ex->repoID ()
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          exceptions[i++] = CORBA::ExceptionDef::_narrow (prev_def.in ()
                                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ex_iter.next ();
        }

      // Build the context list.

      UTL_StrList *ctx_list = node->context ();

      if (ctx_list != 0)
        {
          length = ACE_static_cast (CORBA::ULong,
                                    ctx_list->length ());
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
      this->get_referenced_type (return_type
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Is the operation oneway?
      CORBA::OperationMode mode = node->flags () == AST_Operation::OP_oneway
                                 ? CORBA::OP_ONEWAY
                                 : CORBA::OP_NORMAL;

      // Create the repository entry.

      CORBA::Container_ptr current_scope = CORBA::Container::_nil ();

      if (be_global->ifr_scopes ().top (current_scope) == 0)
        {
          CORBA::InterfaceDef_var iface =
            CORBA::InterfaceDef::_narrow (current_scope
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::OperationDef_var new_def =
            iface->create_operation (node->repoID (),
                                     node->local_name ()->get_string (),
                                     node->version (),
                                     this->ir_current_.in (),
                                     mode,
                                     this->params_,
                                     exceptions,
                                     contexts
                                     ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor_operation::")
              ACE_TEXT ("visit_operation -")
              ACE_TEXT (" scope stack is empty\n")
            ),
            -1
          );
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          ACE_TEXT ("ifr_adding_visitor_operation::visit_operation")
        );

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
ifr_adding_visitor_operation::visit_argument (AST_Argument *node)
{
  // Get the parameter's name.
  this->params_[this->index_].name = node->local_name ()->get_string ();

  AST_Type *arg_type = node->field_type ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Updates ir_current_.
      this->get_referenced_type (arg_type
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (
          ACE_ANY_EXCEPTION,
          ACE_TEXT ("ifr_adding_visitor_operation::visit_argument")
        );

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
