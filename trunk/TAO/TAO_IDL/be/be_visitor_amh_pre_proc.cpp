//=============================================================================
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
*/
//=============================================================================

#include "be_visitor_amh_pre_proc.h"

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
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) be_visitor_amh_pre_proc::"
                       "visit_root - visit scope failed\n"),
                      -1);

  return 0;
}



int
be_visitor_amh_pre_proc::visit_module (be_module *node)
{
  if (!node->imported () && this->visit_scope (node) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) be_visitor_amh_pre_proc::"
                       "visit_module - visit scope failed\n"),
                      -1);

  return 0;
}



int
be_visitor_amh_pre_proc::visit_interface (be_interface *node)
{
  // Do not generate AMH classes for any sort of implied IDL.
  if (node->original_interface () != 0)
    return 0;

  // don't generate AMH classes for imported or local interfaces
  // either...
  // @@ Mayur, maybe we do want to insert the AMH node for imported
  // interfaces, not because we want to generate code for them, but
  // because the (imported-AMH-) node could be needed to generate a
  // non-imported, AMH node, for example, for a derived interface.
  if (node->imported () || node->is_local ())
    return 0;

  AST_Module *module =
    AST_Module::narrow_from_scope (node->defined_in ());

  if (module == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) be_visitor_amh_pre_proc::"
                       "visit_interface - module is null\n"),
                      -1);

  // Create the ResponseHandler class
  be_interface *response_handler = this->create_response_handler (node);
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
  module->be_add_interface (response_handler, node);

  // Remember from whom we were cloned
  response_handler->original_interface (node);

  return 0;
}


be_interface *
be_visitor_amh_pre_proc::create_response_handler (be_interface *node)
{
  // Generate 'AMH_InterfaceResponseHandler'
  ACE_CString class_name;
  class_name += "AMH_";
  class_name += node->local_name ();
  class_name += "ResponseHandler";

  UTL_ScopedName *amh_name =
    ACE_dynamic_cast(UTL_ScopedName*,node->name ()->copy ());
  Identifier *local_name = amh_name->last_component ();
  local_name->replace_string (class_name.c_str ());

  // @@ Mayur, you are not filling up the list of inherited classes,
  // however, you *are* using that same list in the amh_rh_sh.cpp and
  // amh_rh_sh.cpp file... you need to fill up the list, i.e. discover
  // the inherited classes in the original interface, change their
  // names and then use the symbol table to look up the
  // AMH-response-handler nodes.
  be_interface *response_handler =
    new be_interface (amh_name,   // name
                      0,          // list of inherited
                      0,          // number of inherited
                      0,          // list of ancestors
                      0,          // number of ancestors
                      1,          // local
                      0);         // non-abstract

  response_handler->set_name (amh_name);
  response_handler->set_defined_in (node->defined_in ());
  response_handler->set_imported (node->imported ());
  response_handler->set_line (node->line ());
  response_handler->set_file_name (node->file_name ());

  this->add_rh_node_members (node, response_handler);

  return response_handler;
}


int
be_visitor_amh_pre_proc::add_rh_node_members ( be_interface *node,
                                               be_interface *response_handler)
{
  // Now our customized valuetype is created, we have to
  // add now the operations and attributes to the scope.

  this->elem_number_ = 0;

  // initialize an iterator to iterate thru our scope
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();
      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_amh_pre_proc::"
                             "visit_interface - "
                             "bad node in this scope\n"),
                            0);
        }

      if (d->node_type () == AST_Decl::NT_attr)
        {
          be_attribute *attribute = be_attribute::narrow_from_decl (d);

          if (!attribute)
            {
              return 0;
            }
        }
      else
        {
          be_operation* operation = be_operation::narrow_from_decl (d);

          if (operation)
            {
              this->create_response_handler_operation (operation,
                                                       response_handler);
            }
        }
    }
  return 1;
}


int
be_visitor_amh_pre_proc::create_response_handler_operation (be_operation *node,
                                                            be_interface *response_handler
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

  // Create the return type, which is "void"
  be_predefined_type *rt =
    new be_predefined_type (AST_PredefinedType::PT_void,
                            new UTL_ScopedName (new Identifier ("void"),
                                                0));

  ACE_CString original_op_name (node->name ()->last_component ()->get_string ());

  UTL_ScopedName *op_name =
    ACE_static_cast (UTL_ScopedName *, response_handler->name ()-> copy ());
  op_name->nconc (new UTL_ScopedName (new Identifier (original_op_name.rep ()),
                                      0));

  // Create the operation
  be_operation *operation = new be_operation (rt,
                                              AST_Operation::OP_noflags,
                                              op_name,
                                              1,
                                              0);
  operation->set_name (op_name);

  // If return type is non-void add it as first argument

  if (!node->void_return_type ())
    {

      // Create the argument
      be_argument *arg =
        new be_argument (AST_Argument::dir_IN,
                         node->return_type (),
                         new UTL_ScopedName (new Identifier ("return_value"),
                                             0));

      // Add the response handler to the argument list
      operation->add_argument_to_scope (arg);
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
                             "create_response_handler_operation - "
                             "bad node in this scope\n"),
                            -1);

        }

      //be_decl *arg = be_decl::narrow_from_decl (d);
      AST_Argument *original_arg = AST_Argument::narrow_from_decl (d);

      if (original_arg->direction () == AST_Argument::dir_INOUT ||
          original_arg->direction () == AST_Argument::dir_OUT)
        {
          // Create the argument
          be_argument *arg = new be_argument (AST_Argument::dir_IN,
                                              original_arg->field_type (),
                                              original_arg->name ());

          operation->add_argument_to_scope (arg);
        }
    }

  operation->set_defined_in (response_handler);

  // We do not copy the exceptions because the exceptions
  // are delivered by the excep methods.

  // After having generated the operation we insert it into the
  // response handler interface.
  response_handler->be_add_operation (operation);

  return 0;
}


int
be_visitor_amh_pre_proc::visit_operation (be_operation *node)
{
  // We do nothing for oneways!
  if (node->flags () == AST_Operation::OP_oneway)
    return 0;

  // Set the proper strategy
  be_operation_strategy *old_strategy =
    node->set_strategy (new be_operation_amh_strategy (node));

  if (old_strategy)
    delete old_strategy;

  return 0;
}


int
be_visitor_amh_pre_proc::visit_attribute (be_attribute *node)
{
  // Temporarily generate the set operation.
  be_operation *set_operation =
    this->generate_set_operation (node);

  this->visit_operation (set_operation);

  // Retrieve the strategy set by the visit operation
  be_operation_strategy *set_operation_strategy =
    set_operation->set_strategy (
                                 new be_operation_default_strategy (set_operation)
                                 );

  // Assign it to the attribute as set_operation strategy
  if (set_operation_strategy)
    delete node->set_set_strategy (set_operation_strategy);

  // Temporerily generate the get operation.
  be_operation *get_operation =
    this->generate_get_operation (node);

  this->visit_operation (get_operation);

  be_operation_strategy *get_operation_strategy =
    get_operation->set_strategy (
                                 new be_operation_default_strategy (get_operation)
                                 );

  if (get_operation_strategy)
    delete node->set_get_strategy (get_operation_strategy);


  return 0;
}

int
be_visitor_amh_pre_proc::visit_scope (be_scope *node)
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
            number_of_elements++;
          }
      }

      AST_Decl **elements;
      ACE_NEW_RETURN (elements, AST_Decl *[number_of_elements], -1);

      {
        int position = 0;
        // initialize an iterator to iterate thru our scope
        for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
             !si.is_done ();
             si.next ())
          {
            elements[position++] = si.item ();
            si.next ();
          }
      }


      int elem_number = 0;

      // continue until each element is visited
      while (elem_number < number_of_elements)
        {
          AST_Decl *d = elements[elem_number];
          if (!d)
            {
              delete [] elements;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "bad node in this scope\n"), -1);

            }

          be_decl *bd = be_decl::narrow_from_decl (d);

          // set the scope node as "node" in which the code is being
          // generated so that elements in the node's scope can use it
          // for code generation
          this->ctx_->scope (node->decl ());

          // set the node to be visited
          this->ctx_->node (bd);
          elem_number++;


          // Send the visitor.
          if (bd == 0 ||  bd->accept (this) == -1)
            {
              delete [] elements;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "codegen for scope failed\n"), -1);

            }
        } // end of while loop
      delete [] elements;
    } // end of if

  return 0;
}

be_valuetype *
be_visitor_amh_pre_proc::create_exception_holder (be_interface *node)
{

  // Create a virtual module named "Messaging" and a valuetype
  // "ExceptionHolder" from which we inherit.
  UTL_ScopedName *inherit_name =
    new UTL_ScopedName (new Identifier ("Messaging"),
                        0);

  inherit_name->nconc (new UTL_ScopedName (new Identifier ("ExceptionHolder"),
                                           0));

  be_valuetype *inherit_vt = new be_valuetype (inherit_name,
                                               0,
                                               0,
                                               0);
  inherit_vt->set_name (inherit_name);

  //  be_module *msg = new be_module (new UTL_ScopedName (new Identifier ("Messaging"),
  //                                                      0),
  //                                  0);

  be_module *msg =
    new be_module (new UTL_ScopedName (new Identifier ("Messaging"),
                                       0));

  // Notice the valuetype "ExceptionHolder" that it is defined in the
  // "Messaging" module
  inherit_vt->set_defined_in (msg);

  // Create the excpetion holder name
  ACE_CString excep_holder_local_name;
  this->generate_name (excep_holder_local_name,
                       "AMH_",
                       node->name ()->last_component ()->get_string(),
                       "");

  UTL_ScopedName *excep_holder_name =
    ACE_static_cast (UTL_ScopedName *, node->name ()->copy ());
  excep_holder_name->last_component ()->replace_string (
                                                        excep_holder_local_name.rep ()
                                                        );

  AST_Interface_ptr *p_intf = new AST_Interface_ptr[1];
  p_intf[0] = ACE_static_cast (AST_Interface *, inherit_vt);

  be_valuetype *excep_holder =
    new be_valuetype (excep_holder_name,  // name
                      p_intf,             // list of inherited
                      1,                  // number of inherited
                      0);                 // set abstract
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
                                 "(%N:%l) be_visitor_amh_pre_proc::"
                                 "visit_interface - "
                                 "bad node in this scope\n"),
                                0);

            }

          be_decl *op = be_decl::narrow_from_decl (d);

          if (d->node_type () == AST_Decl::NT_attr)
            {
              AST_Attribute *attribute = AST_Attribute::narrow_from_decl (d);

              if (!attribute)
                return 0;

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




int
be_visitor_amh_pre_proc::create_raise_operation (be_decl *node,
                                                 be_valuetype *excep_holder,
                                                 Operation_Kind operation_kind)
{
  be_operation *orig_op = 0;
  if (operation_kind == NORMAL)
    {
      orig_op = be_operation::narrow_from_decl (node);
      if (orig_op)
        {
          if (orig_op->flags () == AST_Operation::OP_oneway)
            // We do nothing for oneways!
            return 0;
        }
    }

  // Create the return type, which is "void"
  be_predefined_type *rt =
    new be_predefined_type (AST_PredefinedType::PT_void,
                            new UTL_ScopedName (new Identifier ("void"),
                                                0));

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

  op_name->nconc (new UTL_ScopedName (new Identifier (new_local_name.rep ()),
                                      0));

  be_operation *operation = new be_operation (rt,
                                              AST_Operation::OP_noflags,
                                              op_name,
                                              1,
                                              0);
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

  // Set the proper strategy
  be_operation_strategy *old_strategy =
    operation->set_strategy (
                             new be_operation_ami_exception_holder_raise_strategy (operation)
                             );

  if (old_strategy)
    {
      delete old_strategy;
    }

  // After having generated the operation we insert it into the
  // exceptionholder valuetype.
  excep_holder->be_add_operation (operation);

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
  ACE_CString original_op_name (node
                                ->name ()
                                ->last_component ()
                                ->get_string ());
  ACE_CString new_op_name = ACE_CString ("get_") + original_op_name;

  UTL_ScopedName *get_name = ACE_static_cast (UTL_ScopedName *,
                                              node->name ()-> copy ());
  get_name->last_component ()->replace_string (new_op_name.rep ());

  be_operation *operation =
    new be_operation (node->field_type (),
                      AST_Operation::OP_noflags,
                      get_name,
                      1,
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

  UTL_ScopedName *set_name = ACE_static_cast (UTL_ScopedName *,
                                              node->name ()-> copy ());
  set_name->last_component ()->replace_string (new_op_name.rep ());

  // the return type  is "void"
  be_predefined_type *rt =
    new be_predefined_type (AST_PredefinedType::PT_void,
                            new UTL_ScopedName (new Identifier ("void"),
                                                0));

  // argument type is the same as the attribute type
  be_argument *arg = new be_argument (AST_Argument::dir_IN,
                                      node->field_type (),
                                      set_name);
  arg->set_name (node->name ());

  // create the operation
  be_operation *operation = new be_operation (rt,
                                              AST_Operation::OP_noflags,
                                              set_name,
                                              0,
                                              0);
  operation->set_name (set_name);
  operation->set_defined_in (node->defined_in ());
  operation->add_argument_to_scope (arg);

  return operation;
}
