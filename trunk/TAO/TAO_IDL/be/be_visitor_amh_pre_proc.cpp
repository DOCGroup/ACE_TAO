/**
 *  @file   be_visitor_amh_pre_proc.cpp
 *
 *  $Id$
 *
 *  This visitor creates for AMH implied IDL constructs the appropriate AST
 *  (Abstract Syntax Tree) node, sets the corresponding interface or operation
 *  strategy on it and enteres the nodes into the AST.
*
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*  @author Mayur Deshpande <mayur@ics.uci.edu>
*  @author Carlos O'Ryan   <coryan@uci.edu>
*/
//=============================================================================

#include "be_visitor_amh_pre_proc.h"
#include "be_visitor_context.h"
#include "be_root.h"
#include "be_module.h"
#include "be_interface.h"
#include "be_valuetype.h"
#include "be_attribute.h"
#include "be_operation.h"
#include "be_predefined_type.h"
#include "be_argument.h"
#include "be_extern.h"
#include "utl_identifier.h"
#include "utl_exceptlist.h"
#include "global_extern.h"
#include "ace/Log_Msg.h"

be_visitor_amh_pre_proc::be_visitor_amh_pre_proc (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_amh_pre_proc::~be_visitor_amh_pre_proc (void)
{
}

int
be_visitor_amh_pre_proc::visit_root (be_root *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_amh_pre_proc::"
                         "visit_root - visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_amh_pre_proc::visit_module (be_module *node)
{
  if (!node->imported () && this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_amh_pre_proc::"
                         "visit_module - visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_amh_pre_proc::visit_interface (be_interface *node)
{
  // Do not generate AMH classes for any sort of implied IDL.
  if (node->original_interface () != 0)
    {
      return 0;
    }

  // Don't generate AMH classes for imported, local or abstract interfaces
  // either...
  // @@ Mayur, maybe we do want to insert the AMH node for imported
  // interfaces, not because we want to generate code for them, but
  // because the (imported-AMH-) node could be needed to generate a
  // non-imported, AMH node, for example, for a derived interface.

  // (JP) Havinq AMH_* nodes inherit directly from AMH_* base classes
  // is also a problem if the base interface is abstract, since at
  // the moment, no AMH code is generated for such interfaces. It
  // would be desirable to mimic the AMI design, where all AMI-related
  // classes inherit from a single base class, no matter what the
  // parentage of the IDL interface. Until we solve the problem of
  // AMH for abstract interfaces, I'm disabling all AMH generation
  // for interfaces having an abstract ancestor.
  if (node->imported ()
      || node->is_local ()
      || node->is_abstract ()
      || node->has_mixed_parentage ())
    {
      return 0;
    }

  // Create the exception holder, it needs to go before the response
  // handler, because the response handler uses an exception holder as
  // argument for some of its operations....
  be_valuetype *excep_holder =
    this->create_exception_holder (node);
  excep_holder->set_defined_in (node->defined_in ());
  excep_holder->original_interface (node);
  excep_holder->is_amh_excep_holder (true);

  AST_Module *module =
    AST_Module::narrow_from_scope (node->defined_in ());
  module->set_has_nested_valuetype ();

  // Create the ResponseHandler class
  be_interface *response_handler =
    this->create_response_handler (node,
                                   excep_holder);
  if (response_handler == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_amh_pre_proc::"
                         "visit_interface - "
                         "creating the response handler failed\n"),
                        -1);
    }
  response_handler->set_defined_in (node->defined_in ());

  // Insert the response handler after the node.
  module->be_add_interface (response_handler,
                            node);

  // Remember from whom we were cloned
  response_handler->original_interface (node);

  module->be_add_interface (excep_holder, node);

  return 0;
}

be_interface *
be_visitor_amh_pre_proc::create_response_handler (
    be_interface *node,
    be_valuetype *exception_holder
  )
{
  // Generate 'AMH_InterfaceResponseHandler'
  ACE_CString class_name;
  class_name += "AMH_";
  class_name += node->local_name ();
  class_name += "ResponseHandler";

  UTL_ScopedName * const amh_name =
    dynamic_cast<UTL_ScopedName*> (node->name ()->copy ());

  if (!amh_name)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "be_visitor_amh_pre_proc::create_response_handler"
                       " - Scope downcast failed.\n"),
                      0);
  Identifier *local_name = amh_name->last_component ();
  local_name->replace_string (class_name.c_str ());

  UTL_Scope *s = node->defined_in ();
  idl_global->scopes ().push (s);

  // @@ Mayur, you are not filling up the list of inherited classes,
  // however, you *are* using that same list in the amh_rh_sh.cpp and
  // amh_rh_sh.cpp file... you need to fill up the list, i.e. discover
  // the inherited classes in the original interface, change their
  // names and then use the symbol table to look up the
  // AMH-response-handler nodes.
  be_interface *response_handler = 0;
  ACE_NEW_RETURN (response_handler,
                  be_interface (amh_name,   // name
                                0,          // list of inherited
                                0,          // number of inherited
                                0,          // list of ancestors
                                0,          // number of ancestors
                                1,          // local
                                0),         // non-abstract
                  0);

  idl_global->scopes ().pop ();

  response_handler->set_name (amh_name);
  response_handler->set_defined_in (node->defined_in ());
  response_handler->set_imported (node->imported ());
  response_handler->set_line (node->line ());
  response_handler->set_file_name (node->file_name ());

  // Set repo id to 0, so it will be recomputed on the next access,
  // and set the prefix to the node's prefix. All this is
  // necessary in case the node's prefix was modified after
  // its declaration.
  response_handler->AST_Decl::repoID (0);
  response_handler->prefix (const_cast<char*> (node->prefix ()));

  response_handler->gen_fwd_helper_name ();
  this->add_rh_node_members (node, response_handler, exception_holder);
  return response_handler;
}

int
be_visitor_amh_pre_proc::add_rh_node_members ( be_interface *node,
                                               be_interface *response_handler,
                                               be_valuetype *exception_holder)
{
  // Now our customized valuetype is created, we have to
  // add now the operations and attributes to the scope.

  this->elem_number_ = 0;

  // Initialize an iterator to iterate thru our scope.
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_amh_pre_proc::"
                             "add_rh_node_members - "
                             "bad node in this scope\n"),
                            0);
        }

      AST_Decl::NodeType nt = d->node_type ();
      int status = 0;

      if (nt == AST_Decl::NT_attr)
        {
          be_attribute *attr = be_attribute::narrow_from_decl (d);

          if (attr != 0)
            {
              status =
                this->create_response_handler_attribute (attr,
                                                         response_handler,
                                                         exception_holder);

              if (status == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                    "(%N:%l) be_visitor_amh_pre_proc::"
                                    "add_rh_node_members - "
                                    "attribute creation failed\n"),
                                    0);
                }
            }
        }
      else if (nt == AST_Decl::NT_op)
        {
          be_operation* operation = be_operation::narrow_from_decl (d);

          if (operation != 0)
            {
              status =
                this->create_response_handler_operation (operation,
                                                         response_handler,
                                                         exception_holder);

              if (status == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                    "(%N:%l) be_visitor_amh_pre_proc::"
                                    "add_rh_node_members - "
                                    "operation creation failed\n"),
                                    0);
                }
            }
        }
      else
        {
          continue;
        }
    }

  return 1;
}

int
be_visitor_amh_pre_proc::create_response_handler_operation (
    be_operation *node,
    be_interface *response_handler,
    be_valuetype *exception_holder
  )
{
  if (node == 0)
    {
      return -1;
    }

  /// These are for the stub side only.
  if (node->is_sendc_ami ())
    {
      return 0;
    }

  if (this->add_normal_reply (node, response_handler) == -1)
    {
      return -1;
    }

  return this->add_exception_reply (node,
                                    response_handler,
                                    exception_holder);
}

int
be_visitor_amh_pre_proc::create_response_handler_attribute (
    be_attribute *node,
    be_interface *response_handler,
    be_valuetype *exception_holder
  )
{
  // Temporarily generate the get operation.
  be_operation *get_operation = this->generate_get_operation (node);

  this->visit_operation (get_operation);

  int status =
    this->create_response_handler_operation (get_operation,
                                             response_handler,
                                             exception_holder);

  get_operation->destroy ();
  delete get_operation;
  get_operation = 0;

  if (status == -1)
    {
      return -1;
    }

  if (node->readonly ())
    {
      return 0;
    }

  // Temporarily generate the set operation.
  be_operation *set_operation = this->generate_set_operation (node);

  this->visit_operation (set_operation);

  status =
    this->create_response_handler_operation (set_operation,
                                             response_handler,
                                             exception_holder);

  set_operation->destroy ();
  delete set_operation;
  set_operation = 0;

  return status;
}

int
be_visitor_amh_pre_proc::add_exception_reply (be_operation *node,
                                              be_interface *response_handler,
                                              be_valuetype *exception_holder)
{
  UTL_ScopedName *operation_name = node->compute_name ("",
                                                       "_excep");

  be_operation *node_excep = 0;
  ACE_NEW_RETURN (node_excep,
                  be_operation (be_global->void_type (),
                                AST_Operation::OP_noflags,
                                operation_name,
                                1,
                                0),
                  -1);

  node_excep->set_name (operation_name);

  Identifier *arg_id = 0;
  ACE_NEW_RETURN (arg_id,
                  Identifier ("holder"),
                  -1);

  UTL_ScopedName *arg_name = 0;
  ACE_NEW_RETURN (arg_name,
                  UTL_ScopedName (arg_id, 0),
                  -1);

  be_argument *argument = 0;
  ACE_NEW_RETURN (argument,
                  be_argument (AST_Argument::dir_IN,
                               exception_holder,
                               arg_name),
                  -1);

  argument->set_name (arg_name);
  argument->set_defined_in (node_excep);
  node_excep->be_add_argument (argument);

  node_excep->set_defined_in (response_handler);

  if (0 == response_handler->be_add_operation (node_excep))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_amh_pre_proc::add_normal_reply (be_operation *node,
                                           be_interface *response_handler)
{
  Identifier *id = 0;
  UTL_ScopedName *sn = 0;

  ACE_CString original_op_name (
                  node->name ()->last_component ()->get_string ()
                );

  UTL_ScopedName *op_name =
    static_cast<UTL_ScopedName *> (response_handler->name ()->copy ());

  ACE_NEW_RETURN (id,
                  Identifier (original_op_name.c_str ()),
                  -1);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  -1);

  op_name->nconc (sn);

  // Create the operation
  be_operation *operation = 0;
  ACE_NEW_RETURN (operation,
                  be_operation (be_global->void_type (),
                                AST_Operation::OP_noflags,
                                op_name,
                                1,
                                0),
                  -1);

  operation->set_name (op_name);

  // If return type is non-void add it as first argument

  if (!node->void_return_type ())
    {
      Identifier *arg_id = 0;
      ACE_NEW_RETURN (arg_id,
                      Identifier ("return_value"),
                      -1);

      UTL_ScopedName *arg_name = 0;
      ACE_NEW_RETURN (arg_name,
                      UTL_ScopedName (arg_id, 0),
                      -1);

      // Create the argument
      be_argument *arg = 0;
      ACE_NEW_RETURN (arg,
                      be_argument (AST_Argument::dir_IN,
                                   node->return_type (),
                                   arg_name),
                      -1);

      arg->set_name (arg_name);

      // Add the response handler to the argument list
      operation->be_add_argument (arg);
    }

  // Iterate over the arguments and put all the out and inout arguments
  // into the new method.
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_amh_pre_proc::"
                             "add_normal_reply - "
                             "bad node in this scope\n"),
                            -1);

        }

      //be_decl *arg = be_decl::narrow_from_decl (d);
      AST_Argument *original_arg = AST_Argument::narrow_from_decl (d);

      if (original_arg->direction () == AST_Argument::dir_INOUT ||
          original_arg->direction () == AST_Argument::dir_OUT)
        {
          // Create the argument
          be_argument *arg = 0;
          ACE_NEW_RETURN (arg,
                          be_argument (AST_Argument::dir_IN,
                                       original_arg->field_type (),
                                       original_arg->name ()),
                          -1);

          operation->be_add_argument (arg);
        }
    }

  operation->set_defined_in (response_handler);

  // We do not copy the exceptions because the exceptions
  // are delivered by the excep methods.

  // After having generated the operation we insert it into the
  // response handler interface.
  if (0 == response_handler->be_add_operation (operation))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_amh_pre_proc::visit_operation (be_operation *node)
{
  // @@ Mayur, we do want to generate code for oneways!  This is
  // needed to support reliable oneways with the SYNC_WITH_TARGET
  // policy.
  // We do nothing for oneways!
  if (node->flags () == AST_Operation::OP_oneway)
    {
      return 0;
    }

  return 0;
}

be_valuetype *
be_visitor_amh_pre_proc::create_exception_holder (be_interface *node)
{
  // AMH exception holders require both of these.
  idl_global->valuetype_seen_ = true;

  idl_global->valuefactory_seen_ = true;

  const int inherit_count = 0;
  AST_Type **p_intf = 0;

  UTL_ScopedName *excep_holder_name =
    node->compute_name ("AMH_",
                        "ExceptionHolder");

  UTL_Scope *s = node->defined_in ();
  idl_global->scopes ().push (s);

  be_valuetype *excep_holder = 0;
  ACE_NEW_RETURN (excep_holder,
                  be_valuetype (excep_holder_name,
                                p_intf,
                                inherit_count,
                                0,
                                0,
                                0,
                                0,
                                0,
                                0,
                                0,
                                0,
                                0),
                  0);

  idl_global->scopes ().pop ();

  excep_holder->set_name (excep_holder_name);
  excep_holder->set_defined_in (node->defined_in ());

  // Set repo id to 0, so it will be recomputed on the next access,
  // and set the prefix to the node's prefix. All this is
  // necessary in case the node's prefix was modified after
  // its declaration.
  excep_holder->AST_Decl::repoID (0);
  excep_holder->prefix (const_cast<char*> (node->prefix ()));

  excep_holder->gen_fwd_helper_name ();

  // Now our customized valuetype is created, we have to
  // add now the operations and attributes to the scope.

  // initialize an iterator to iterate thru our scope
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_amh_pre_proc::"
                             "visit_interface - "
                             "bad node in this scope\n"),
                            0);
        }

      be_decl *op = be_decl::narrow_from_decl (d);
      AST_Decl::NodeType nt = d->node_type ();

      if (nt == AST_Decl::NT_attr)
        {
          AST_Attribute *attribute = AST_Attribute::narrow_from_decl (d);

          if (attribute == 0)
            {
              return 0;
            }

          this->create_raise_operation (op,
                                        excep_holder,
                                        GET_OPERATION);

          if (!attribute->readonly ())
            {
              this->create_raise_operation (op,
                                            excep_holder,
                                            SET_OPERATION);
            }
        }
      else if (nt == AST_Decl::NT_op)
        {
          this->create_raise_operation (op,
                                        excep_holder,
                                        NORMAL);
        }
      else
        {
          continue;
        }
    }

  return excep_holder;
}

int
be_visitor_amh_pre_proc::create_raise_operation (
    be_decl *node,
    be_valuetype *excep_holder,
    Operation_Kind operation_kind
  )
{
  Identifier *id = 0;
  UTL_ScopedName *sn = 0;
  be_operation *orig_op = 0;

  if (operation_kind == NORMAL)
    {
      orig_op = be_operation::narrow_from_decl (node);
    }

  // Name the operation properly
  UTL_ScopedName *op_name =
    static_cast<UTL_ScopedName *> (excep_holder->name ()->copy ());

  ACE_CString new_local_name ("raise_");

  if (operation_kind == SET_OPERATION)
    {
      new_local_name += "set_";
    }
  else if (operation_kind == GET_OPERATION)
    {
      new_local_name += "get_";
    }

  new_local_name += node->name ()->last_component ()->get_string ();

  ACE_NEW_RETURN (id,
                  Identifier (new_local_name.c_str ()),
                  -1);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  -1);

  op_name->nconc (sn);

  be_operation *operation = 0;
  ACE_NEW_RETURN (operation,
                  be_operation (be_global->void_type (),
                                AST_Operation::OP_noflags,
                                op_name,
                                0,
                                0),
                  -1);

  operation->set_name (op_name);
  operation->set_defined_in (excep_holder);

  if (operation_kind == NORMAL)
    {
      if (orig_op)
        {
          // Copy the exceptions.
          UTL_ExceptList *exceptions = orig_op->exceptions ();

          if (0 != exceptions)
            {
              operation->be_add_exceptions (exceptions->copy ());
            }
        }
    }

  // After having generated the operation we insert it into the
  // exceptionholder valuetype.
  if (0 == excep_holder->be_add_operation (operation))
    {
      return -1;
    }

  return 0;
}


int
be_visitor_amh_pre_proc::generate_name (ACE_CString &destination,
                                        const char *prefix,
                                        const char *middle_name,
                                        const char *suffix)
{
  destination = prefix;
  destination += middle_name;
  destination += suffix;
  return 0;
}



be_operation *
be_visitor_amh_pre_proc::generate_get_operation (be_attribute *node)
{
  ACE_CString original_op_name (
      node->name ()->last_component ()->get_string ()
    );
  ACE_CString new_op_name = ACE_CString ("get_") + original_op_name;

  UTL_ScopedName *get_name =
    static_cast<UTL_ScopedName *> (node->name ()-> copy ());
  get_name->last_component ()->replace_string (new_op_name.c_str ());

  be_operation *operation = 0;
  ACE_NEW_RETURN (operation,
                  be_operation (node->field_type (),
                                AST_Operation::OP_noflags,
                                get_name,
                                1,
                                0),
                  0);

  operation->set_name (get_name);
  operation->set_defined_in (node->defined_in ());

  return operation;
}

be_operation *
be_visitor_amh_pre_proc::generate_set_operation (be_attribute *node)
{
  ACE_CString original_op_name (
      node->name ()->last_component ()->get_string ()
    );
  ACE_CString new_op_name = ACE_CString ("set_") + original_op_name;

  UTL_ScopedName *set_op_name =
    dynamic_cast<UTL_ScopedName *> (node->name ()-> copy ());
  set_op_name->last_component ()->replace_string (new_op_name.c_str ());

  // argument type is the same as the attribute type
  be_argument *arg = 0;
  ACE_NEW_RETURN (arg,
                  be_argument (AST_Argument::dir_IN,
                               node->field_type (),
                               set_op_name),
                  0);

  arg->set_name (dynamic_cast<UTL_ScopedName *> (node->name ()->copy ()));

  // create the operation
  be_operation *operation = 0;
  ACE_NEW_RETURN (operation,
                  be_operation (be_global->void_type (),
                                AST_Operation::OP_noflags,
                                set_op_name,
                                0,
                                0),
                  0);

  operation->set_name (set_op_name);
  operation->set_defined_in (node->defined_in ());
  operation->be_add_argument (arg);

  return operation;
}
