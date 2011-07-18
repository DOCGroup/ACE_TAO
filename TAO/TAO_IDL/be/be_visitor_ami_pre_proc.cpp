//
// $Id$
//

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_ami_pre_proc.cpp
//
// = DESCRIPTION
//    This visitor creates for AMI implied IDL constructs
//    the appropriate AST (Abstract Syntax Tree) node,
//    sets the corresponding interface or operation strategy
//    on it and enteres the nodes into the AST.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#include "be_visitor_ami_pre_proc.h"
#include "be_visitor_context.h"
#include "be_root.h"
#include "be_module.h"
#include "be_interface.h"
#include "be_interface_strategy.h"
#include "be_valuetype.h"
#include "be_operation.h"
#include "be_operation_strategy.h"
#include "be_attribute.h"
#include "be_predefined_type.h"
#include "be_argument.h"
#include "be_global.h"
#include "be_extern.h"
#include "utl_identifier.h"
#include "utl_exceptlist.h"
#include "nr_extern.h"
#include "global_extern.h"
#include "ace/Log_Msg.h"

ACE_RCSID (be,
           be_visitor_ami_pre_proc,
           "$Id$")


be_visitor_ami_pre_proc::be_visitor_ami_pre_proc (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{

}

be_visitor_ami_pre_proc::~be_visitor_ami_pre_proc (void)
{

}

int
be_visitor_ami_pre_proc::visit_root (be_root *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_pre_proc::"
                         "visit_root - "
                         "visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_ami_pre_proc::visit_module (be_module *node)
{
  // Skip this for now until we get AMI integrated with
  // CIAO.
  if (0 == ACE_OS::strcmp (node->local_name ()->get_string (), "Components"))
    {
      return 0;
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_pre_proc::"
                         "visit_module - "
                         "visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_ami_pre_proc::visit_interface (be_interface *node)
{
  // We check for an imported node after generating the reply handler.
  if (node->is_local () || node->is_abstract ())
    {
      return 0;
    }

  // The following 3 IF blocks are checks for CCM-related nodes, which
  // we want to skip until we get AMI integrated with CIAO.

  // Skip the *EventConsumer added for each eventtype.
  if (node->is_event_consumer ())
    {
      return 0;
    }

  // Check for home equivalent interface. The lookup will find the
  // home itself, which was declared first.
  Identifier *node_lname = node->AST_Decl::local_name ();
  AST_Decl *first_stored =
    node->defined_in ()->lookup_by_name_local (node_lname, 0);

  if (0 != first_stored && first_stored->node_type () == AST_Decl::NT_home)
    {
      return 0;
    }

  ACE_CString lname (node_lname->get_string ());

  // Skip the *Explict and *Implicit interfaces added for a home.
  if (lname.substr (lname.length () - 6) == "plicit")
    {
      UTL_Scope *s = node->defined_in ();
      Identifier local_id (lname.substr (0, lname.length () - 8).c_str ());
      AST_Decl *d = s->lookup_by_name_local (&local_id, 0);
      local_id.destroy ();

      if (0 != d)
        {
          return 0;
        }
    }

  AST_Module *module =
    AST_Module::narrow_from_scope (node->defined_in ());

  if (!module)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_pre_proc::"
                         "visit_interface - "
                         "module is null\n"),
                        -1);
    }

  be_valuetype *excep_holder = this->create_exception_holder (node);
  be_interface *reply_handler = this->create_reply_handler (node,
                                                            excep_holder);
  if (reply_handler)
    {
      reply_handler->set_defined_in (node->defined_in ());

      // Insert the ami handler after the node, the
      // exception holder will be placed between these two later.
      module->be_add_interface (reply_handler, node);

      // Remember from whom we were cloned
      reply_handler->original_interface (node);

      // If this was created for an imported node, it will be wrong
      // unless we set it.
      reply_handler->set_imported (node->imported ());
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_pre_proc::"
                         "visit_interface - "
                         "creating the reply handler failed\n"),
                        -1);
    }

  // Set the proper strategy.
  be_interface_ami_strategy *bias = 0;
  ACE_NEW_RETURN (bias,
                  be_interface_ami_strategy (node,
                                             reply_handler),
                  -1);
  be_interface_strategy *old_strategy = node->set_strategy (bias);

  if (old_strategy)
    {
      old_strategy->destroy ();
      delete old_strategy;
      old_strategy = 0;
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_pre_proc::"
                         "visit_interface - "
                         "visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_ami_pre_proc::visit_operation (be_operation *node)
{
  if (node->flags () == AST_Operation::OP_oneway)
    {
      // We do nothing for oneways!
      return 0;
    }

  // If we're here, we're sure that the arg traits specialization
  // for this will be needed.
  be_global->messaging_exceptionholder ()->seen_in_operation (true);

  be_operation *sendc_marshaling =
    this->create_sendc_operation (node,
                                  0); // for arguments = FALSE

  be_operation *sendc_arguments =
    this->create_sendc_operation (node,
                                  1); // for arguments = TRUE

  if (0 != sendc_marshaling && 0 != sendc_arguments)
    {
      sendc_marshaling->set_defined_in (node->defined_in ());

      sendc_arguments->set_defined_in (node->defined_in ());

      // We do not copy the exceptions because the exceptions
      // are delivered by the excep methods.

      // Set the proper strategy, and store the specialized
      // marshaling and arguments operations in it.
      be_operation_ami_sendc_strategy * boass = 0;
      ACE_NEW_RETURN (boass,
                      be_operation_ami_sendc_strategy (node,
                                                       sendc_marshaling,
                                                       sendc_arguments),
                      -1);

      be_operation_strategy *old_strategy = node->set_strategy (boass);

      if (old_strategy)
        {
          old_strategy->destroy ();
          delete old_strategy;
          old_strategy = 0;
        }
    }

  return 0;
}

int
be_visitor_ami_pre_proc::visit_attribute (be_attribute *node)
{
  // Temporerily generate the set operation.
  be_operation *set_operation =
    this->generate_set_operation (node);

  this->visit_operation (set_operation);

  // Retrieve the strategy set by the visit operation.
  be_operation_default_strategy *bods = 0;
  ACE_NEW_RETURN (bods,
                  be_operation_default_strategy (set_operation),
                  -1);

  be_operation_strategy *set_operation_strategy =
    set_operation->set_strategy (bods);

  set_operation->destroy ();
  delete set_operation;
  set_operation = 0;

  // Assign it to the attribute as set_operation strategy.
  if (0 != set_operation_strategy)
    {
      be_operation_strategy *bos =
        node->set_set_strategy (set_operation_strategy);

      if (0 != bos)
        {
          bos->destroy ();
          delete bos;
          bos = 0;
        }
    }

  // Temporerily generate the get operation.
  be_operation *get_operation =
    this->generate_get_operation (node);

  this->visit_operation (get_operation);

  // Retrieve the strategy set by the visit operation.
  ACE_NEW_RETURN (bods,
                  be_operation_default_strategy (get_operation),
                  -1);

  be_operation_strategy *get_operation_strategy =
    get_operation->set_strategy (bods);

  get_operation->destroy ();
  delete get_operation;
  get_operation = 0;

  // Assign it to the attribute as get_operation strategy.
  if (0 != get_operation_strategy)
    {
      be_operation_strategy *bos =
        node->set_get_strategy (get_operation_strategy);

      if (0 != bos)
        {
          bos->destroy ();
          delete bos;
          bos = 0;
        }
    }

  return 0;
}

be_valuetype *
be_visitor_ami_pre_proc::create_exception_holder (be_interface *)
{
  return be_global->messaging_exceptionholder ();
}

be_interface *
be_visitor_ami_pre_proc::create_reply_handler (be_interface *node,
                                               be_valuetype *excep_holder)
{
  // We're at global scope here so we need to fool the scope stack
  // for a minute so the correct repo id can be calculated at
  // interface construction time.
  UTL_Scope *s = node->defined_in ();
  idl_global->scopes ().push (s);

  // Create the reply handler name.
  ACE_CString reply_handler_local_name;
  this->generate_name (reply_handler_local_name,
                       "AMI_",
                       node->name ()->last_component ()->get_string(),
                       "Handler");

  UTL_ScopedName *reply_handler_name =
    static_cast<UTL_ScopedName *> (node->name ()->copy ());
  reply_handler_name->last_component ()->replace_string (
                                             reply_handler_local_name.c_str ()
                                           );

  long n_parents = 0;
  AST_Interface **p_intf = this->create_inheritance_list (node, n_parents);

  be_interface *reply_handler = 0;
  ACE_NEW_RETURN (reply_handler,
                  be_interface (reply_handler_name, // name
                                p_intf,             // list of inherited
                                n_parents,          // number of inherited
                                0,                  // list of all ancestors
                                0,                  // number of ancestors
                                0,                  // non-local
                                0),                 // non-abstract
                  0);

  // Back to reality.
  idl_global->scopes ().pop ();

  reply_handler->set_name (reply_handler_name);
  reply_handler->set_defined_in (s);
  
  // Set repo id to 0, so it will be recomputed on the next access,
  // and set the prefix to the node's prefix. All this is
  // necessary in case the node's prefix was modified after
  // its declaration.
  reply_handler->AST_Decl::repoID (0);
  reply_handler->prefix (const_cast<char*> (node->prefix ()));
  
  reply_handler->gen_fwd_helper_name ();

  // Now our customized reply handler is created, we have to
  // add the operations and attributes to the scope.
  // Imported nodes get admitted here, so they can get
  // the reply handler operations added, in case they are
  // needed in the inheritance graph traversal for a
  // child reply handler. However, no exception holder
  // stuff is executed for an imported node.
  if (node->nmembers () > 0)
    {
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
                                 "(%N:%l) be_visitor_ami_pre_proc::visit_interface - "
                                 "bad node in this scope\n"),
                                0);

            }

          if (d->node_type () == AST_Decl::NT_attr)
            {
              be_attribute *attribute = be_attribute::narrow_from_decl (d);

              if (attribute == 0)
                {
                  return 0;
                }

              be_operation *get_operation =
                this->generate_get_operation (attribute);

              this->create_reply_handler_operation (get_operation,
                                                    reply_handler);

              this->create_excep_operation (get_operation,
                                            reply_handler,
                                            excep_holder);

              get_operation->destroy ();
              delete get_operation;
              get_operation = 0;

              if (!attribute->readonly ())
                {
                  be_operation *set_operation =
                    this->generate_set_operation (attribute);

                  this->create_reply_handler_operation (set_operation,
                                                        reply_handler);

                  this->create_excep_operation (set_operation,
                                                reply_handler,
                                                excep_holder);

                  set_operation->destroy ();
                  delete set_operation;
                  set_operation = 0;
                }
            }
          else
            {
              be_operation* operation = be_operation::narrow_from_decl (d);

              if (operation)
                {
                  this->create_reply_handler_operation (operation,
                                                        reply_handler);

                  this->create_excep_operation (operation,
                                                reply_handler,
                                                excep_holder);
                }
            }
        } // end of while loop
    } // end of if

  return reply_handler;
}

int
be_visitor_ami_pre_proc::create_raise_operation (
    be_decl *node,
    be_valuetype *excep_holder,
    Operation_Kind operation_kind
  )
{
  be_operation *orig_op = 0;

  if (operation_kind == NORMAL)
    {
      orig_op = be_operation::narrow_from_decl (node);

      if (orig_op)
        {
          if (orig_op->flags () == AST_Operation::OP_oneway)
            {
              // We do nothing for oneways!
              return 0;
            }
        }
    }

  Identifier *id = 0;
  UTL_ScopedName *sn = 0;

  ACE_NEW_RETURN (id,
                  Identifier ("void"),
                  -1);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  -1);

  // Create the return type, which is "void"
  be_predefined_type *rt = 0;
  ACE_NEW_RETURN (rt,
                  be_predefined_type (AST_PredefinedType::PT_void,
                                      sn),
                  -1);

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
                  be_operation (rt,
                                AST_Operation::OP_noflags,
                                op_name,
                                0,
                                0),
                  -1);

  operation->set_name (op_name);
  operation->set_defined_in (excep_holder);

#if defined (TAO_HAS_DEPRECATED_EXCEPTION_HOLDER)
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
#endif

  // Set the proper strategy.
  be_operation_ami_exception_holder_raise_strategy *boaehrs = 0;
  ACE_NEW_RETURN (boaehrs,
                  be_operation_ami_exception_holder_raise_strategy (operation),
                  -1);

  be_operation_strategy *old_strategy = operation->set_strategy (boaehrs);

  if (old_strategy)
    {
      old_strategy->destroy ();
      delete old_strategy;
      old_strategy = 0;
    }

  // After having generated the operation we insert it into the
  // exceptionholder valuetype.
  if (0 == excep_holder->be_add_operation (operation))
    {
      return -1;
    }

  return 0;
}

be_operation *
be_visitor_ami_pre_proc::create_sendc_operation (be_operation *node,
                                                 int for_arguments)
{
  if (node->flags () == AST_Operation::OP_oneway)
    {
      // We do nothing for oneways!
      return 0;
    }

  Identifier *id = 0;
  UTL_ScopedName *sn = 0;

  // Create the new name
  // Prepend "sendc_" to the name of the operation
  ACE_CString original_op_name (
                  node->name ()->last_component ()->get_string ()
                );
  ACE_CString new_op_name = ACE_CString ("sendc_") + original_op_name;

  UTL_ScopedName *op_name =
    static_cast<UTL_ScopedName *> (node->name ()->copy ());
  op_name->last_component ()->replace_string (new_op_name.c_str ());

  // Create the operation
  be_operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (be_global->void_type (),
                                AST_Operation::OP_noflags,
                                op_name,
                                0,
                                0),
                  0);

  op->set_name (op_name);

  // Create the first argument, which is a Reply Handler

  if (for_arguments)
    {
      // Look up the field type.
      UTL_Scope *s = node->defined_in ();
      be_decl *parent = be_scope::narrow_from_scope (s)->decl ();

      // Add the pre- and suffix
      ACE_CString handler_local_name;
      this->generate_name (handler_local_name,
                           "AMI_",
                           parent->name ()->last_component ()->get_string (),
                           "Handler");

      UTL_ScopedName *field_name =
        static_cast<UTL_ScopedName *> (parent->name ()->copy ());
      field_name->last_component ()->replace_string (
                                         handler_local_name.c_str ()
                                       );

      AST_Decl *d = s->lookup_by_name (field_name, true);
      field_name->destroy ();
      delete field_name;
      field_name = 0;

      if (0 == d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_ami_pre_proc::"
                             "create_sendc_operation - "
                             "lookup of reply handler failed\n"),
                            0);
        }

      be_interface *field_type = be_interface::narrow_from_decl (d);

      // Create the argument.
      ACE_NEW_RETURN (id,
                      Identifier ("ami_handler"),
                      0);

      UTL_ScopedName *tmp = 0;

      ACE_NEW_RETURN (tmp,
                      UTL_ScopedName (id,
                                      0),
                      0);

      sn = (UTL_ScopedName *)op->name ()->copy ();
      sn->nconc (tmp);

      be_argument *arg = 0;
      ACE_NEW_RETURN (arg,
                      be_argument (AST_Argument::dir_IN,
                                   field_type, // is also a valuetype
                                   sn),
                      0);

      arg->set_defined_in (op);
      arg->set_name (sn);
      op->be_add_argument (arg);

      if (field_type->imported ())
        {
          field_type->seen_in_operation (false);
        }
    }

  // Iterate over the arguments and put all the in and inout
  // into the new method.
  if (node->nmembers () > 0)
    {
      // initialize an iterator to iterate thru our scope
      for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          AST_Decl *d = si.item ();

          if (!d)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  "(%N:%l) be_visitor_ami_pre_proc::create_sendc_method - "
                  "bad node in this scope\n"
                ),
                0
              );

            }

          AST_Argument *original_arg = AST_Argument::narrow_from_decl (d);

          if (original_arg->direction () == AST_Argument::dir_IN ||
              original_arg->direction () == AST_Argument::dir_INOUT)
            {
              // Create the argument.
              be_argument *arg = 0;
              UTL_ScopedName *new_name =
                (UTL_ScopedName *)original_arg->name ()->copy ();
              ACE_NEW_RETURN (arg,
                              be_argument (AST_Argument::dir_IN,
                                           original_arg->field_type (),
                                           new_name),
                              0);

              arg->set_defined_in (op);
              arg->set_name (new_name);
              op->be_add_argument (arg);
            }
        } // end of while loop
    } // end of if

  return op;
}

int
be_visitor_ami_pre_proc::create_reply_handler_operation (
    be_operation *node,
    be_interface *reply_handler
  )
{
  if (!node)
    {
      return -1;
    }

  if (node->flags () == AST_Operation::OP_oneway)
    {
      // We do nothing for oneways!
      return 0;
    }

  Identifier *id = 0;
  UTL_ScopedName *sn = 0;

  ACE_CString original_op_name (
                  node->name ()->last_component ()->get_string ()
                );

  UTL_ScopedName *op_name =
    static_cast<UTL_ScopedName *> (reply_handler->name ()-> copy ());

  ACE_NEW_RETURN (id,
                  Identifier (original_op_name.c_str ()),
                  -1);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  -1);

  op_name->nconc (sn);

  // Create the operation.
  be_operation *operation = 0;
  ACE_NEW_RETURN (operation,
                  be_operation (be_global->void_type (),
                                AST_Operation::OP_noflags,
                                op_name,
                                0,
                                0),
                  -1);

  operation->set_name (op_name);

  // If return type is non-void add it as first argument.

  if (!node->void_return_type ())
    {
      ACE_NEW_RETURN (id,
                      Identifier ("ami_return_val"),
                      -1);

      UTL_ScopedName *tmp = 0;

      ACE_NEW_RETURN (tmp,
                      UTL_ScopedName (id,
                                      0),
                      -1);

      sn = (UTL_ScopedName *)operation->name ()->copy ();
      sn->nconc (tmp);

      // Create the argument.
      be_argument *arg = 0;
      ACE_NEW_RETURN (arg,
                      be_argument (AST_Argument::dir_IN,
                                   node->return_type (),
                                   sn),
                      -1);

      arg->set_defined_in (operation);
      arg->set_name (sn);

      // Add the reply handler to the argument list.
      operation->be_add_argument (arg);
    }

  // Iterate over the arguments and put all the in and inout
  // into the new method.
  if (node->nmembers () > 0)
    {
      // Initialize an iterator to iterate thru our scope.
      for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          AST_Decl *d = si.item ();

          if (d == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_ami_pre_proc::"
                                 "create_reply_handler_operation - "
                                 "bad node in this scope\n"),
                                -1);

            }

          AST_Argument *original_arg = AST_Argument::narrow_from_decl (d);

          if (original_arg->direction () == AST_Argument::dir_INOUT ||
              original_arg->direction () == AST_Argument::dir_OUT)
            {
              // Create the argument.
              be_argument *arg = 0;
              UTL_ScopedName *new_name =
                (UTL_ScopedName *)original_arg->name ()->copy ();
              ACE_NEW_RETURN (arg,
                              be_argument (AST_Argument::dir_IN,
                                           original_arg->field_type (),
                                           new_name),
                              -1);

              arg->set_defined_in (operation);
              arg->set_name (new_name);
              operation->be_add_argument (arg);
            }
        } // end of while loop
    } // end of if

  // Set the proper strategy.
  be_operation_ami_handler_reply_stub_strategy *boahrss = 0;
  ACE_NEW_RETURN (boahrss,
                  be_operation_ami_handler_reply_stub_strategy (operation),
                  -1);

  be_operation_strategy *old_strategy = operation->set_strategy (boahrss);

  if (old_strategy)
    {
      old_strategy->destroy ();
      delete old_strategy;
      old_strategy = 0;
    }

  operation->set_defined_in (reply_handler);

  // Copy the exceptions.
  if (node->exceptions ())
    {
      UTL_ExceptList *exceptions = node->exceptions ();

      if (0 != exceptions)
        {
          operation->be_add_exceptions (exceptions->copy ());
        }
    }

  // After having generated the operation we insert it into the
  // reply handler interface.
  if (0 == reply_handler->be_add_operation (operation))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ami_pre_proc::create_excep_operation (be_operation *node,
                                                 be_interface *reply_handler,
                                                 be_valuetype *excep_holder)
{
  if (!node)
    {
      return -1;
    }

  if (node->flags () == AST_Operation::OP_oneway)
    {
      // We do nothing for oneways!
      return 0;
    }

  // Create the return type, which is "void".
  Identifier *id = 0;
  UTL_ScopedName *sn = 0;

  // Create the argument.
  ACE_NEW_RETURN (id,
                  Identifier ("excep_holder"),
                  -1);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  -1);

  be_argument *arg = 0;
  ACE_NEW_RETURN (arg,
                  be_argument (AST_Argument::dir_IN,
                               excep_holder, // is also a valuetype
                               sn),
                  -1);

  arg->set_name (sn);

  UTL_ScopedName *tmp = (UTL_ScopedName *)sn->copy ();

  // Create the new name
  // Append _excep to the name of the operation
  ACE_CString original_op_name (
                  node->name ()->last_component ()->get_string ()
                );
  ACE_CString new_op_name = original_op_name + ACE_CString ("_excep");

  UTL_ScopedName *op_name =
    static_cast<UTL_ScopedName *> (reply_handler->name ()->copy ());

  ACE_NEW_RETURN (id,
                  Identifier (new_op_name.c_str ()),
                  -1);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  -1);

  op_name->nconc (sn);

  AST_PredefinedType *rt = be_global->void_type ();

  // Create the operation.
  be_operation *operation = 0;
  ACE_NEW_RETURN (operation,
                  be_operation (rt,
                                AST_Operation::OP_noflags,
                                op_name,
                                0,
                                0),
                  -1);

  operation->set_name (op_name);
  operation->be_add_argument (arg);
  operation->set_defined_in (reply_handler);

  UTL_ScopedName *arg_name =
    dynamic_cast<UTL_ScopedName *> (op_name->copy ());
  arg_name->nconc (tmp);
  arg->set_name (arg_name);
  arg->set_defined_in (operation);

  // Copy the exceptions since the user exception information may
  // be needed when collocation is disabled.
  UTL_ExceptList *exceptions = node->exceptions ();

  if (0 != exceptions)
    {
      operation->be_add_exceptions (exceptions->copy ());
    }

  // After having generated the operation we insert it into the
  // reply handler interface.
  if (0 == reply_handler->be_add_operation (operation))
    {
      return -1;
    }

  return 0;
}

// Visit the scope and its elements.
int
be_visitor_ami_pre_proc::visit_scope (be_scope *node)
{
  // proceed if the number of members in our scope is greater than 0
  if (node->nmembers () > 0)
    {
      int number_of_elements = 0;

      {
        // initialize an iterator to iterate thru our scope
        for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
             !si.is_done ();
             si.next ())
          {
            ++number_of_elements;
          }
      }

      AST_Decl **elements;
      ACE_NEW_RETURN (elements,
                      AST_Decl *[number_of_elements],
                      -1);

      {
        int position = 0;

        // Initialize an iterator to iterate thru our scope.
        for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
             !si.is_done ();
             si.next ())
          {
            elements[position++] = si.item ();
          }
      }

      int elem_number = 0;

      // Continue until each element is visited.
      while (elem_number < number_of_elements)
        {
          AST_Decl *d = elements[elem_number];

          if (!d)
            {
              delete [] elements;
              elements = 0;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "bad node in this scope\n"),
                                -1);

            }

          be_decl *bd = be_decl::narrow_from_decl (d);

          // Set the scope node as "node" in which the code is being
          // generated so that elements in the node's scope can use it
          // for code generation.
          this->ctx_->scope (node->decl ());

          // Set the node to be visited.
          this->ctx_->node (bd);
          ++elem_number;


          // Send the visitor.
          if (bd == 0 ||  bd->accept (this) == -1)
            {
              delete [] elements;
              elements = 0;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "codegen for scope failed\n"),
                                -1);
            }
        } // end of while loop

      delete [] elements;
      elements = 0;
    } // end of if

  return 0;
}

// Helper methods

int
be_visitor_ami_pre_proc::generate_name (ACE_CString &destination,
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
be_visitor_ami_pre_proc::generate_get_operation (be_attribute *node)
{
  ACE_CString original_op_name (
                  node->name ()->last_component ()->get_string ()
                );
  ACE_CString new_op_name = ACE_CString ("get_") + original_op_name;

  UTL_ScopedName *get_name =
    static_cast<UTL_ScopedName *> (node->name ()->copy ());
  get_name->last_component ()->replace_string (new_op_name.c_str ());

  be_operation *operation = 0;
  ACE_NEW_RETURN (operation,
                  be_operation (node->field_type (),
                                AST_Operation::OP_noflags,
                                get_name,
                                0,
                                0),
                  0);

  operation->set_name (get_name);
  operation->set_defined_in (node->defined_in ());

  return operation;
}

be_operation *
be_visitor_ami_pre_proc::generate_set_operation (be_attribute *node)
{
  ACE_CString original_op_name (
                  node->name ()->last_component ()->get_string ()
                );
  ACE_CString new_op_name = ACE_CString ("set_") + original_op_name;

  UTL_ScopedName *set_name =
    static_cast<UTL_ScopedName *> (node->name ()-> copy ());
  set_name->last_component ()->replace_string (new_op_name.c_str ());

  // Argument type is the same as the attribute type.
  be_argument *arg = 0;
  ACE_NEW_RETURN (arg,
                  be_argument (AST_Argument::dir_IN,
                               node->field_type (),
                               set_name),
                  0);

  arg->set_name ((UTL_ScopedName *) node->name ()->copy ());

  // Create the operation.
  be_operation *operation = 0;
  ACE_NEW_RETURN (operation,
                  be_operation (be_global->void_type (),
                                AST_Operation::OP_noflags,
                                set_name,
                                0,
                                0),
                  0);

  operation->set_name (set_name);
  operation->set_defined_in (node->defined_in ());
  operation->be_add_argument (arg);

  return operation;
}

AST_Interface **
be_visitor_ami_pre_proc::create_inheritance_list (be_interface *node,
                                                  long &n_rh_parents)
{
  AST_Interface **retval = 0;

  long n_parents = node->n_inherits ();
  AST_Interface **parents = node->inherits ();
  AST_Interface *parent = 0;

  for (long i = 0; i < n_parents; ++i)
    {
      parent = parents[i];

      if (parent->is_abstract ())
        {
          continue;
        }

      ++n_rh_parents;
    }

  if (n_rh_parents == 0)
    {
      be_interface *inherit_intf = be_global->messaging_replyhandler ();

      ACE_NEW_RETURN (retval,
                      AST_Interface *[1],
                      0);

      retval[0] = inherit_intf;
      n_rh_parents = 1;
    }
  else
    {
      ACE_NEW_RETURN (retval,
                      AST_Interface *[n_rh_parents],
                      0);

      ACE_CString prefix ("AMI_");
      ACE_CString suffix ("Handler");
      long index = 0;

      for (long j = 0; j < n_parents; ++j)
        {
          parent = parents[j];

          if (parent->is_abstract ())
            {
              continue;
            }

          ACE_CString rh_local_name =
            prefix + parent->local_name ()->get_string () + suffix;

          UTL_ScopedName *rh_parent_name =
            static_cast<UTL_ScopedName *> (parent->name ()->copy ());

          rh_parent_name->last_component ()->replace_string (
                                                 rh_local_name.c_str ()
                                               );

          AST_Decl *d =
            node->defined_in ()->lookup_by_name (rh_parent_name,
                                                 true);

          if (d != 0)
            {
              retval[index] = AST_Interface::narrow_from_decl (d);
              retval[index++]->set_prefix_with_typeprefix (parent->prefix () ?
                                                           const_cast<char*> (parent->prefix()) :
                                                           const_cast<char*> (""));
            }

          rh_parent_name->destroy ();
          delete rh_parent_name;
          rh_parent_name = 0;
        }

      // Just a sanity check until we're sure this works in all use cases.
      if (n_rh_parents != index)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "reply handler parent iteration mismatch\n"),
                            0);
        }
    }

  return retval;
}
