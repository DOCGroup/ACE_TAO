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


ACE_RCSID(be, be_visitor_ami_pre_proc, "$Id$")


be_visitor_ami_pre_proc::be_visitor_ami_pre_proc (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{

}

  // constructor is protected
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
  if (!node->imported ())
    {
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_ami_pre_proc::"
                             "visit_module - "
                             "visit scope failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_ami_pre_proc::visit_interface (be_interface *node)
{
  if (!node->imported () && !node->is_local ())
    {
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
          delete old_strategy;
          old_strategy = 0;
        }

      if (excep_holder)
        {
          excep_holder->set_defined_in (node->defined_in ());
          // Insert the exception holder after the original node,
          // this way we ensure that it is *before* the
          // ami handler, which is the way we want to have it.
          module->be_add_interface (excep_holder, node);
          module->set_has_nested_valuetype ();
          // Remember from whom we were cloned.
          excep_holder->original_interface (node);

          // Set the strategy.
          be_interface_ami_exception_holder_strategy *biaehs = 0;
          ACE_NEW_RETURN (biaehs,
                          be_interface_ami_exception_holder_strategy (
                              excep_holder
                            ),
                          -1);

          be_interface_strategy *old_strategy =
            excep_holder->set_strategy (biaehs);

          if (old_strategy)
            {
              delete old_strategy;
              old_strategy = 0;
            }
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_ami_pre_proc::"
                             "visit_interface - "
                             "creating the exception holder failed\n"),
                            -1);
        }


      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_ami_pre_proc::"
                             "visit_interface - "
                             "visit scope failed\n"),
                            -1);
        }
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

  be_operation *sendc_marshaling =
    this->create_sendc_operation (node,
                                  0); // for arguments = FALSE

  be_operation *sendc_arguments =
    this->create_sendc_operation (node,
                                  1); // for arguments = TRUE

  if (sendc_marshaling && sendc_arguments)
    {
      sendc_marshaling->set_defined_in (node->defined_in ());

      sendc_arguments->set_defined_in (node->defined_in ());

      // We do not copy the exceptions because the exceptions
      // are delivered by the excep methods.

      // Set the proper strategy, and store the specialized
      // marshaling and arguments operations in it.
      be_operation_ami_sendc_strategy * boass= 0;
      ACE_NEW_RETURN (boass,
                      be_operation_ami_sendc_strategy (node,
                                                       sendc_marshaling,
                                                       sendc_arguments),
                      -1);

      be_operation_strategy *old_strategy = node->set_strategy (boass);

      if (old_strategy)
        {
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

  // Assign it to the attribute as set_operation strategy.
  if (set_operation_strategy)
    {
      be_operation_strategy *bos =
        node->set_set_strategy (set_operation_strategy);
      delete bos;
      bos = 0;
    }

  // Temporerily generate the get operation.
  be_operation *get_operation =
    this->generate_get_operation (node);

  this->visit_operation (get_operation);

  ACE_NEW_RETURN (bods,
                  be_operation_default_strategy (get_operation),
                  -1);

  be_operation_strategy *get_operation_strategy =
    get_operation->set_strategy (bods);

  if (get_operation_strategy)
    {
      be_operation_strategy *bos =
        node->set_get_strategy (get_operation_strategy);
      delete bos;
      bos = 0;
    }

  return 0;
}






be_valuetype *
be_visitor_ami_pre_proc::create_exception_holder (be_interface *node)
{
  Identifier *id = 0;
  ACE_NEW_RETURN (id,
                  Identifier ("Messaging"),
                  0);

  // Create a virtual module named "Messaging" and an valuetype "ExceptionHolder"
  // from which we inherit.
  UTL_ScopedName *inherit_name = 0;
  ACE_NEW_RETURN (inherit_name,
                  UTL_ScopedName (id,
                                  0),
                  0);

  ACE_NEW_RETURN (id,
                  Identifier ("ExceptionHolder"),
                  0);

  UTL_ScopedName *sn = 0;
  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  0);

  inherit_name->nconc (sn);

  be_valuetype *inherit_vt = 0;
  ACE_NEW_RETURN (inherit_vt,
                  be_valuetype (inherit_name,
                                0,
                                0,
                                0),
                  0);

  inherit_vt->set_name (inherit_name);

  ACE_NEW_RETURN (id,
                  Identifier ("Messaging"),
                  0);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  0);

  be_module *msg = 0;
  ACE_NEW_RETURN (msg,
                  be_module (sn),
                  0);

  // Notice the valuetype "ExceptionHolder" that it is defined in the
  // "Messaging" module
  inherit_vt->set_defined_in (msg);

  // Create the excpetion holder name
  ACE_CString excep_holder_local_name;
  this->generate_name (excep_holder_local_name,
                       "AMI_",
                       node->name ()->last_component ()->get_string(),
                       "ExceptionHolder");

  UTL_ScopedName *excep_holder_name =
    ACE_static_cast (UTL_ScopedName *, node->name ()->copy ());
  excep_holder_name->last_component ()->replace_string (
                                            excep_holder_local_name.rep ()
                                          );

  AST_Interface_ptr *p_intf = 0;
  ACE_NEW_RETURN (p_intf,
                  AST_Interface_ptr[1],
                  0);

  p_intf[0] = ACE_static_cast (AST_Interface *,
                               inherit_vt);

  be_valuetype *excep_holder = 0;
  ACE_NEW_RETURN (excep_holder,
                  be_valuetype (excep_holder_name,  // name
                                p_intf,             // list of inherited
                                1,                  // number of inherited
                                0),                 // set abstract
                  0);

  excep_holder->set_name (excep_holder_name);

  // Now our customized valuetype is created, we have to
  // add now the operations and attributes to the scope.

  if (node->nmembers () > 0)
    {
      // initialize an iterator to iterate thru our scope
      for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          AST_Decl *d = si.item ();

          if (d == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_ami_pre_proc::visit_interface - "
                                 "bad node in this scope\n"),
                                0);

            }
          be_decl *op = be_decl::narrow_from_decl (d);

          if (d->node_type () == AST_Decl::NT_attr)
            {
              AST_Attribute *attribute = AST_Attribute::narrow_from_decl (d);

              if (!attribute)
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
          else
            {
              this->create_raise_operation (op,
                                            excep_holder,
                                            NORMAL);
            }
        } // end of while loop
    } // end of if

  return excep_holder;
}


be_interface *
be_visitor_ami_pre_proc::create_reply_handler (be_interface *node,
                                               be_valuetype *excep_holder)
{
  Identifier *id = 0;
  UTL_ScopedName *sn = 0;

  // Create a virtual module named "Messaging" and an interface "ReplyHandler"
  // from which we inherit.
  ACE_NEW_RETURN (id,
                  Identifier ("Messaging"),
                  0);

  UTL_ScopedName *inherit_name = 0;
  ACE_NEW_RETURN (inherit_name,
                  UTL_ScopedName (id,
                                  0),
                  0);

  ACE_NEW_RETURN (id,
                  Identifier ("ReplyHandler"),
                  0);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  0);

  inherit_name->nconc (sn);

  be_interface *inherit_intf = 0;
  ACE_NEW_RETURN (inherit_intf,
                  be_interface (inherit_name,
                                0,  // inherited interfaces
                                0,  // number of inherited interfaces
                                0,  // ancestors
                                0,  // number of ancestors
                                0,  // not local
                                0), // not abstract
                  0);

  inherit_intf->set_name (inherit_name);

  ACE_NEW_RETURN (id,
                  Identifier ("Messaging"),
                  0);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  0);

  be_module *msg = 0;
  ACE_NEW_RETURN (msg,
                  be_module (sn),
                  0);

  // Notice the interface "ReplyHandler" that it is defined in the
  // "Messaging" module.
  inherit_intf->set_defined_in (msg);


  // Create the reply handler name.
  ACE_CString reply_handler_local_name;
  this->generate_name (reply_handler_local_name,
                       "AMI_",
                       node->name ()->last_component ()->get_string(),
                       "Handler");

  UTL_ScopedName *reply_handler_name =
    ACE_static_cast (UTL_ScopedName *, node->name ()->copy ());
  reply_handler_name->last_component ()->replace_string (
                                             reply_handler_local_name.rep ()
                                           );

  AST_Interface_ptr *p_intf = 0;
  ACE_NEW_RETURN (p_intf,
                  AST_Interface_ptr[1],
                  0);

  p_intf[0] = ACE_static_cast (AST_Interface *,
                               inherit_intf);

  be_interface *reply_handler = 0;
  ACE_NEW_RETURN (reply_handler,
                  be_interface (reply_handler_name, // name
                                p_intf,             // list of inherited
                                1,                  // number of inherited
                                p_intf,             // list of ancestors
                                1,                  // number of ancestors
                                0,                  // non-local
                                0),                 // non-abstract
                  0);

  reply_handler->set_name (reply_handler_name);

  // Now our customized valuetype is created, we have to
  // add now the operations and attributes to the scope.

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

              if (!attribute->readonly ())
                {
                  be_operation *set_operation =
                    this->generate_set_operation (attribute);
                  this->create_reply_handler_operation (set_operation,
                                                        reply_handler);

                  this->create_excep_operation (set_operation,
                                                reply_handler,
                                                excep_holder);
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
  UTL_ScopedName *op_name = ACE_static_cast (UTL_ScopedName *,
                                             excep_holder->name ()-> copy ());

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
                  Identifier (new_local_name.rep ()),
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

  if (operation_kind == NORMAL)
    {
      if (orig_op)
        {
          // Copy the exceptions.
          if (orig_op->exceptions ())
            {
              UTL_ExceptList *exceptions = orig_op->exceptions ();
              operation->be_add_exceptions (exceptions);
            }
        }
    }

  // Set the proper strategy.
  be_operation_ami_exception_holder_raise_strategy *boaehrs = 0;
  ACE_NEW_RETURN (boaehrs,
                  be_operation_ami_exception_holder_raise_strategy (operation),
                  -1);

  be_operation_strategy *old_strategy = operation->set_strategy (boaehrs);

  if (old_strategy)
    {
      delete old_strategy;
      old_strategy = 0;
    }

  // After having generated the operation we insert it into the
  // exceptionholder valuetype.
  excep_holder->be_add_operation (operation);

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

  ACE_NEW_RETURN (id,
                  Identifier ("void"),
                  0);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  0);

  // Create the return type, which is "void"
  be_predefined_type *rt = 0;
  ACE_NEW_RETURN (rt,
                  be_predefined_type (
                                   AST_PredefinedType::PT_void,
                                   sn),
                  0);

  // Create the new name
  // Prepend "sendc_" to the name of the operation
  ACE_CString original_op_name (
                  node->name ()->last_component ()->get_string ()
                );
  ACE_CString new_op_name = ACE_CString ("sendc_") + original_op_name;

  UTL_ScopedName *op_name =
    ACE_static_cast (UTL_ScopedName *,
                     node->name ()->copy ());
  op_name->last_component ()->replace_string (new_op_name.rep ());

  // Create the operation
  be_operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (rt,
                                AST_Operation::OP_noflags,
                                op_name,
                                0,
                                0),
                  0);

  // Create the first argument, which is a Reply Handler

  if (for_arguments)
    {
      // Create the field type
      be_decl *parent =
        be_scope::narrow_from_scope (node->defined_in ())->decl ();

      // Add the pre- and suffix
      ACE_CString excep_holder_name;
      this->generate_name (excep_holder_name,
                           "AMI_",
                           parent->name ()->last_component ()->get_string (),
                           "Handler");

      UTL_ScopedName *field_name =
        ACE_static_cast (UTL_ScopedName *, parent->name ()->copy ());
      field_name->last_component ()->replace_string (excep_holder_name.rep ());

      be_interface *field_type = 0;
      ACE_NEW_RETURN (field_type,
                      be_interface (field_name,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0,
                                    0),
                      0);

      field_type->set_defined_in (node->defined_in ());

      // Create the argument.
      ACE_NEW_RETURN (id,
                      Identifier ("ami_handler"),
                      0);

      ACE_NEW_RETURN (sn,
                      UTL_ScopedName (id,
                                      0),
                      0);

      be_argument *arg = 0;
      ACE_NEW_RETURN (arg,
                      be_argument (AST_Argument::dir_IN,
                                   field_type, // is also a valuetype
                                   sn),
                      0);

      // Add the reply handler to the argument list
      op->add_argument_to_scope (arg);
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
            //be_decl *arg = be_decl::narrow_from_decl (d);
          AST_Argument *original_arg = AST_Argument::narrow_from_decl (d);

          if (original_arg->direction () == AST_Argument::dir_IN ||
              original_arg->direction () == AST_Argument::dir_INOUT)
            {
              // Create the argument.
              be_argument *arg = 0;
              ACE_NEW_RETURN (arg,
                              be_argument (AST_Argument::dir_IN,
                                           original_arg->field_type (),
                                           original_arg->name ()),
                              0);

              op->add_argument_to_scope (arg);
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

  ACE_CString original_op_name (
                  node->name ()->last_component ()->get_string ()
                );

  UTL_ScopedName *op_name =
    ACE_static_cast (UTL_ScopedName *,
                     reply_handler->name ()-> copy ());

  ACE_NEW_RETURN (id,
                  Identifier (original_op_name.rep ()),
                  -1);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  -1);

  op_name->nconc (sn);

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

  // If return type is non-void add it as first argument.

  if (!node->void_return_type ())
    {
      ACE_NEW_RETURN (id,
                      Identifier ("ami_return_val"),
                      -1);

      ACE_NEW_RETURN (sn,
                      UTL_ScopedName (id,
                                      0),
                      -1);

      // Create the argument.
      be_argument *arg = 0;
      ACE_NEW_RETURN (arg,
                      be_argument (AST_Argument::dir_IN,
                                   node->return_type (),
                                   sn),
                      -1);

      // Add the reply handler to the argument list.
      operation->add_argument_to_scope (arg);
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
              ACE_NEW_RETURN (arg,
                              be_argument (AST_Argument::dir_IN,
                                           original_arg->field_type (),
                                           original_arg->name ()),
                              -1);

              operation->add_argument_to_scope (arg);
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
      delete old_strategy;
      old_strategy = 0;
    }

  operation->set_defined_in (reply_handler);

  // We do not copy the exceptions because the exceptions
  // are delivered by the excep methods.

  // After having generated the operation we insert it into the
  // reply handler interface.
  reply_handler->be_add_operation (operation);

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

  ACE_NEW_RETURN (id,
                  Identifier ("void"),
                  -1);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  -1);

  be_predefined_type *rt = 0;
  ACE_NEW_RETURN (rt,
                  be_predefined_type (AST_PredefinedType::PT_void,
                                      sn),
                  -1);

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

  // Create the new name
  // Append _execp to the name of the operation
  ACE_CString original_op_name (
                  node->name ()->last_component ()->get_string ()
                );
  ACE_CString new_op_name = original_op_name + ACE_CString ("_excep");

  UTL_ScopedName *op_name =
    ACE_static_cast (UTL_ScopedName *, reply_handler->name ()->copy ());

  ACE_NEW_RETURN (id,
                  Identifier (new_op_name.rep ()),
                  -1);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  -1);

  op_name->nconc (sn);

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
  operation->add_argument_to_scope (arg);

  operation->set_defined_in (reply_handler);

  // We do not copy the exceptions because the exceptions
  // are delivered by the excep methods.

  // After having generated the operation we insert it into the
  // reply handler interface.
  reply_handler->be_add_operation (operation);

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

  UTL_ScopedName *get_name = ACE_static_cast (UTL_ScopedName *,
                                              node->name ()->copy ());
  get_name->last_component ()->replace_string (new_op_name.rep ());

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

  UTL_ScopedName *set_name = ACE_static_cast (UTL_ScopedName *,
                                              node->name ()-> copy ());
  set_name->last_component ()->replace_string (new_op_name.rep ());

  // The return type  is "void".
  Identifier *id = 0;
  UTL_ScopedName *sn = 0;

  ACE_NEW_RETURN (id,
                  Identifier ("void"),
                  0);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  0);

  be_predefined_type *rt = 0;
  ACE_NEW_RETURN (rt,
                  be_predefined_type (AST_PredefinedType::PT_void,
                                      sn),
                  0);

  // Argument type is the same as the attribute type.
  be_argument *arg = 0;
  ACE_NEW_RETURN (arg,
                  be_argument (AST_Argument::dir_IN,
                               node->field_type (),
                               set_name),
                  0);

  arg->set_name (node->name ());

  // Create the operation.
  be_operation *operation = 0;
  ACE_NEW_RETURN (operation,
                  be_operation (rt,
                                AST_Operation::OP_noflags,
                                set_name,
                                0,
                                0),
                  0);

  operation->set_name (set_name);
  operation->set_defined_in (node->defined_in ());
  operation->add_argument_to_scope (arg);

  return operation;
}
