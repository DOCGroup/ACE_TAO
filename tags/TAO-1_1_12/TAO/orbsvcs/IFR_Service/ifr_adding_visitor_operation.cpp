/* -*- c++ -*- */
// $Id$

#include "ifr_adding_visitor_operation.h"
#include "utl_exceptlist.h"
#include "utl_strlist.h"

ACE_RCSID(IFR_Service, ifr_adding_visitor_operation, "$Id$")

ifr_adding_visitor_operation::ifr_adding_visitor_operation (
    CORBA::Environment &ACE_TRY_ENV
  )
  : ifr_adding_visitor (ACE_TRY_ENV),
    index_ (0)
{
}

ifr_adding_visitor_operation::~ifr_adding_visitor_operation (void)
{
}

int 
ifr_adding_visitor_operation::visit_operation (AST_Operation *node)
{
  // If this operation is already in the repository (for example, if
  // we are processing the IDL file a second time inadvertently), we
  // just return 0. The IDL file must be legal, otherwise the IDL
  // compiler front end would have told us.

  IR_Contained_var prev_def =
    be_global->repository ()->lookup_id (node->repoID (),
                                         this->env_);
  TAO_IFR_CHECK_RETURN (-1);

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

  IR_ExceptionDefSeq exceptions (length);
  exceptions.length (length);

  UTL_ExceptlistActiveIterator ex_iter (excepts);
  AST_Exception *ex = 0;
  CORBA::ULong i = 0;

  while (!ex_iter.is_done ())
    {
      ex = ex_iter.item ();

      prev_def =
        be_global->repository ()->lookup_id (ex->repoID (),
                                             this->env_);
      TAO_IFR_CHECK_RETURN (-1);

      exceptions[i++] = IR_ExceptionDef::_narrow (prev_def.in (),
                                                  this->env_);
      TAO_IFR_CHECK_RETURN (-1);

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

  IR_ContextIdSeq contexts (length);
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

  // If one of the operations's args is an interface, we just update
  // the current IR object holder. The forward declaration
  // (if any) will create a repository entry, and the full
  // definition will take care of the interface's scope. Trying 
  // to take care of the interface's scope at this point could
  // cause problems, if the types of all its members have not yet
  // been declared.
  if (return_type->node_type () == AST_Decl::NT_interface)
    {
      IR_Contained_var prev_def =
        be_global->repository ()->lookup_id (return_type->repoID (),
                                             this->env_);
      TAO_IFR_CHECK_RETURN (-1);

      this->ir_current_ = IR_IDLType::_narrow (prev_def.in (),
                                               this->env_);
      TAO_IFR_CHECK_RETURN (-1);
    }
  else
    {
      // Since this entry should already be in the repository,
      // this call will just look it up and update the current 
      // IR object holder.
      if (return_type->ast_accept (this) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor_operation::")
              ACE_TEXT ("visit_operation -")
              ACE_TEXT (" failed to accept visitor\n")
            ),  
            -1
          );
        }
    }

  // Is the operation oneway?
  IR_OperationMode mode = node->flags () == AST_Operation::OP_oneway 
                             ? OP_ONEWAY 
                             : OP_NORMAL;

  // Create the repository entry.

  IR_Container_ptr current_scope = IR_Container::_nil ();

  if (be_global->ifr_scopes ().top (current_scope) == 0)
    {
      IR_InterfaceDef_var iface = 
        IR_InterfaceDef::_narrow (current_scope,
                                  this->env_);
      TAO_IFR_CHECK_RETURN (-1);

      IR_OperationDef_var new_def =
        iface->create_operation (node->repoID (),
                                 node->local_name ()->get_string (),
                                 this->gen_version (node),
                                 this->ir_current_.in (),
                                 mode,
                                 this->params_,
                                 exceptions,
                                 contexts,
                                 this->env_);
      TAO_IFR_CHECK_RETURN (-1);
    }
  else
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) ifr_adding_visitor_operation::visit_operation -")
          ACE_TEXT (" scope stack is empty\n")
        ),  
        -1
      );
    }

  return 0;
}

int 
ifr_adding_visitor_operation::visit_argument (AST_Argument *node)
{
  // Get the parameter's name.
  this->params_[this->index_].name = node->local_name ()->get_string ();

  AST_Type *arg_type = node->field_type ();

  // If one of the operations's args is an interface, we just update
  // the current IR object holder. The forward declaration
  // (if any) will create a repository entry, and the full
  // definition will take care of the interface's scope. Trying 
  // to take care of the interface's scope at this point could
  // cause problems, if the types of all its members have not yet
  // been declared.
  if (arg_type->node_type () == AST_Decl::NT_interface)
    {
      IR_Contained_var prev_def =
        be_global->repository ()->lookup_id (arg_type->repoID (),
                                             this->env_);
      TAO_IFR_CHECK_RETURN (-1);

      this->ir_current_ = IR_IDLType::_narrow (prev_def.in (),
                                               this->env_);
      TAO_IFR_CHECK_RETURN (-1);
    }
  else
    {
      // Since this type must already have a repository entry, the call
      // wil just update the current IR object holder.
      if (arg_type->ast_accept (this) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor_operation::")
              ACE_TEXT ("visit_argument - failed to accept visitor\n")
            ),  
            -1
          );
        }
    }

  this->params_[this->index_].type_def = 
    IR_IDLType::_duplicate (this->ir_current_.in ());

  // Fortunately, AST_Field::Direction and IR_ParameterMode 
  // are ordered identically.
  this->params_[this->index_].mode = (IR_ParameterMode) node->direction ();

  // IfR method create_operation does not use this - it just needs
  // to be non-null for marshaling.
  this->params_[this->index_].type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_null);

  ++this->index_;

  return 0;
}

