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
//    A visitor to generate the AMI implied IDL code.
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
be_visitor_ami_pre_proc::visit_module (be_module *node)
{
  ACE_UNUSED_ARG (node);
  ACE_DEBUG ((LM_DEBUG, "be_visitor_ami_pre_proc::visit_module\n"));
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
  ACE_DEBUG ((LM_DEBUG, "be_visitor_ami_pre_proc::visit_interface\n"));

  be_valuetype *excep_holder = this->create_exception_holder (node);

  if (excep_holder)
    {
      excep_holder->set_defined_in (node->defined_in ());
      (be_module::narrow_from_scope (node->defined_in ()))
        ->be_add_interface (excep_holder, node);
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_pre_proc::"
                         "visit_interface - "
                         "creating the exception holder failed\n"),
                        -1);
    }


  be_interface *reply_handler = this->create_reply_handler (node,
                                                            excep_holder);
  if (reply_handler)
    {
      reply_handler->set_defined_in (node->defined_in ());
      (be_module::narrow_from_scope (node->defined_in ()))
        ->be_add_interface (reply_handler, node);
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_pre_proc::"
                         "visit_interface - "
                         "creating the reply handler failed\n"),
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

  return 0;
}

int 
be_visitor_ami_pre_proc::visit_interface_fwd (be_interface_fwd *node)
{
  ACE_UNUSED_ARG (node);
  ACE_DEBUG ((LM_DEBUG, "be_visitor_ami_pre_proc::visit_interface_fwd\n"));

  return 0;
}

int 
be_visitor_ami_pre_proc::visit_operation (be_operation *node)
{
  ACE_DEBUG ((LM_DEBUG, "be_visitor_ami_pre_proc::visit_operation\n"));

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_pre_proc::"
                         "visit_operation - "
                         "visit scope failed\n"),
                        -1);
    }

  be_operation *sendc_operation = this->create_sendc_operation (node);

  if (sendc_operation)
    {
      sendc_operation->set_defined_in (node->defined_in ());

      // We do not copy the exceptions because the exceptions
      // are delivered by the excep methods.

      // After having generated the operation we insert it into the
      // exceptionholder valuetype.
      // @@ Michael: We use the hidden_operation () method
      // (be_interface::narrow_from_scope (node->defined_in ()))->be_add_operation (sendc_operation);

        // Set the proper strategy
      be_operation_strategy *old_strategy = 
        node->set_strategy (new be_operation_ami_sendc_strategy (node, 
                                                                 sendc_operation));
      if (old_strategy)
        delete old_strategy;
    }

  return 0;
}


int 
be_visitor_ami_pre_proc::visit_argument (be_argument *node)
{
  ACE_UNUSED_ARG (node);
  ACE_DEBUG ((LM_DEBUG, "be_visitor_ami_pre_proc::visit_argument\n"));

  return 0;
}

int
be_visitor_ami_pre_proc::visit_attribute (be_attribute *node)
{
  ACE_UNUSED_ARG (node);
  ACE_DEBUG ((LM_DEBUG, "be_visitor_ami_pre_proc::visit_attribute\n"));
  
  return 0;
}

int
be_visitor_ami_pre_proc::visit_root (be_root *node)
{
  ACE_DEBUG ((LM_DEBUG, "be_visitor_ami_pre_proc::visit_root\n"));


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





be_valuetype *
be_visitor_ami_pre_proc::create_exception_holder (be_interface *node)
{
  ACE_DEBUG ((LM_DEBUG, "be_visitor_ami_pre_proc::create_exception_holder\n"));
  // Create a virtual module named "Messaging" and an valuetype "ExceptionHolder"
  // from which we inherit.
  UTL_ScopedName *inherit_name = 
    new UTL_ScopedName (new Identifier ("Messaging", 0,0,0),
                        0);
  
  inherit_name->nconc (new UTL_ScopedName (new Identifier ("ExceptionHolder", 
                                                           0,
                                                           0,
                                                           0),
                                           0));
 
  be_valuetype *inherit_vt = new be_valuetype (inherit_name,
                                               0,
                                               0,
                                               0);
  inherit_vt->set_name (inherit_name);

  be_module *msg = new be_module (new UTL_ScopedName (new Identifier ("Messaging", 
                                                                      0,
                                                                      0,
                                                                      0),
                                                            0),
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

  UTL_ScopedName *excep_holder_name = (UTL_ScopedName *)node->name ()->copy ();
  excep_holder_name->last_component ()->replace_string (excep_holder_local_name.rep ());

  AST_Interface_ptr *p_intf = new AST_Interface_ptr[1];
  p_intf[0] = (AST_Interface *)inherit_vt;

  be_valuetype *excep_holder = new be_valuetype (excep_holder_name,  // name
                                                 p_intf,             // list of inherited
                                                 1,                  // number of inherited
                                                 0,                  // pragmas
                                                 0);                 // set abstract
  excep_holder->set_name (excep_holder_name);

  // Now our customized valuetype is created, we have to 
  // add now the operations and attributes to the scope.

  if (node->nmembers () > 0)
    {
      // initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (node,
                                               UTL_Scope::IK_decls),
                      0);
      // continue until each element is visited
      while (!si->is_done ())
        {
          AST_Decl *d = si->item ();
          if (!d)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_ami_pre_proc::visit_interface - "
                                 "bad node in this scope\n"),
                                0);

            }
          be_decl *op = be_decl::narrow_from_decl (d);

            {
              // @@ Support for attributes.
              //  if (d->node_type () == AST_Decl::NT_attr)
              //                                  attr = AST_Attribute::narrow_from_decl (d);
              //    if (!attr)
              //      return -1;

              //    if (!attr->readonly ())

              // Create the return type, which is "void"
              be_predefined_type *rt = new be_predefined_type (AST_PredefinedType::PT_void,
                                                               new UTL_ScopedName
                                                                 (new Identifier ("void", 
                                                                                  1, 
                                                                                  0, 
                                                                                  I_FALSE), 
                                                                  0),
                                                               0);

              // Name the operation properly
              UTL_ScopedName *new_name = (UTL_ScopedName *) node->name ()->copy ();

              ACE_CString new_local_name ("raise_");
              new_local_name += op->name ()->last_component ()->get_string ();
  
              new_name->nconc (new UTL_ScopedName (new Identifier (new_local_name.rep (), 
                                                                   0,
                                                                   0,
                                                                   0),
                                                   0));

              be_operation *operation = new be_operation (rt,
                                                          AST_Operation::OP_noflags,
                                                          new_name,
                                                          0);

              AST_Operation *op_base = ACE_reinterpret_cast (AST_Operation *,
                                                             op);

              // Copy the exceptions.
              if (op_base->exceptions ())
                {
                  UTL_ExceptList *exceptions = (UTL_ExceptList *)op_base->exceptions ()->copy ();
                  operation->be_add_exceptions (exceptions);
                }

              // Set the proper strategy
              be_operation_strategy *old_strategy = 
                operation->set_strategy (new be_operation_ami_exception_holder_raise_strategy (operation));
              if (old_strategy)
                delete old_strategy;

              // After having generated the operation we insert it into the
              // exceptionholder valuetype.
              excep_holder->be_add_operation (operation);
            }
          si->next ();
        } // end of while loop
      delete si;
    } // end of if

  return excep_holder;
}


be_interface *
be_visitor_ami_pre_proc::create_reply_handler (be_interface *node,
                                               be_valuetype *excep_holder)
{
  ACE_DEBUG ((LM_DEBUG, "be_visitor_ami_pre_proc::create_reply_handler\n"));

  // Create a virtual module named "Messaging" and an interface "ReplyHandler"
  // from which we inherit.
  UTL_ScopedName *inherit_name = 
    new UTL_ScopedName (new Identifier ("Messaging", 0,0,0),
                        0);
  
  inherit_name->nconc (new UTL_ScopedName (new Identifier ("ReplyHandler", 0,0,0),
                                           0));
 
  be_interface *inherit_intf = new be_interface (inherit_name,
                                                 0,  // inherited interfaces
                                                 0,  // number of inherited interfaces
                                                 0); // pragmas
  inherit_intf->set_name (inherit_name);

  be_module *msg = new be_module (new UTL_ScopedName (new Identifier ("Messaging", 0,0,0),
                                                      0),
                                  0);
 
  // Notice the interface "ReplyHandler" that it is defined in the 
  // "Messaging" module
  inherit_intf->set_defined_in (msg);


  // Create the reply handler name
  ACE_CString reply_handler_local_name;
  this->generate_name (reply_handler_local_name,
                       "AMI_",
                       node->name ()->last_component ()->get_string(),
                       "Handler");

  UTL_ScopedName *reply_handler_name = (UTL_ScopedName *)node->name ()->copy ();
  reply_handler_name->last_component ()->replace_string (reply_handler_local_name.rep ());

  AST_Interface_ptr *p_intf = new AST_Interface_ptr[1];
  p_intf[0] = (AST_Interface *)inherit_intf;

  be_interface *reply_handler = new be_interface (reply_handler_name,  // name
                                                       p_intf,             // list of inherited
                                                       1,                  // number of inherited
                                                       0);                 // pragmas
  reply_handler->set_name (reply_handler_name);

  // Now our customized valuetype is created, we have to 
  // add now the operations and attributes to the scope.

  if (node->nmembers () > 0)
    {
      // initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (node,
                                               UTL_Scope::IK_decls),
                      0);
      this->elem_number_ = 0;
      // continue until each element is visited
      while (!si->is_done ())
        {
          AST_Decl *d = si->item ();
          if (!d)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_ami_pre_proc::visit_interface - "
                                 "bad node in this scope\n"),
                                0);

            }

          // Copy the original operation with only the inout and out 
          // parameters.

          // @@ Support for attributes.
          //  if (d->node_type () == AST_Decl::NT_attr)
          //                                  attr = AST_Attribute::narrow_from_decl (d);
          //    if (!attr)
          //      return -1;

          //    if (!attr->readonly ())


          be_operation *operation =
            this->create_reply_handler_operation (be_operation::narrow_from_decl (d),
                                                  reply_handler);

          if (operation)
            {
              operation->set_defined_in (reply_handler);

              // We do not copy the exceptions because the exceptions
              // are delivered by the excep methods.

              // After having generated the operation we insert it into the
              // reply handler interface.
              reply_handler->be_add_operation (operation);
            }

          be_operation *excep_operation = 
            this->create_excep_operation (be_operation::narrow_from_decl (d),
                                          reply_handler,
                                          excep_holder);

          if (excep_operation)
            {
              excep_operation->set_defined_in (reply_handler);

              // We do not copy the exceptions because the exceptions
              // are delivered by the excep methods.

              // After having generated the operation we insert it into the
              // reply handler interface
              reply_handler->be_add_operation (excep_operation);
            }
    
          si->next ();
        } // end of while loop
      delete si;
    } // end of if

  return reply_handler;
}


be_operation *
be_visitor_ami_pre_proc::create_sendc_operation (be_operation *node)
{
  ACE_DEBUG ((LM_DEBUG, "be_visitor_ami_pre_proc::create_sendc_operation\n"));
  // Create the return type, which is "void"
  be_predefined_type *rt = new be_predefined_type (AST_PredefinedType::PT_void,
                                                   new UTL_ScopedName
                                                     (new Identifier ("void", 1, 0, I_FALSE), 
                                                      0),
                                                   0);

  // Create the new name
  // Prepend "sendc_" to the name of the operation
  ACE_CString original_op_name (node
                                  ->name ()
                                    ->last_component ()
                                      ->get_string ());
  ACE_CString new_op_name = ACE_CString ("sendc_") + original_op_name;

  UTL_ScopedName *op_name = (UTL_ScopedName *)node->name ()-> copy ();
  op_name->last_component ()->replace_string (new_op_name.rep ());

  // Create the operation
  be_operation * op = new be_operation (rt, 
                                        AST_Operation::OP_noflags,
                                        op_name, 
                                        0);

  // Create the first argument, which is a Reply Handler

  // Create the field type
  be_decl *parent = be_scope::narrow_from_scope (node->defined_in ())->decl ();

  // Add the pre- and suffix
  ACE_CString excep_holder_name;
  this->generate_name (excep_holder_name,
                       "AMI_",
                       parent
                         ->name ()
                           ->last_component ()
                             ->get_string (),
                       "Handler");
  UTL_ScopedName *field_name = (UTL_ScopedName *)parent->name ()->copy ();
  field_name->last_component ()->replace_string (excep_holder_name.rep ());
  be_interface *field_type= new be_interface (field_name,
                                              0,
                                              0,
                                              0);
  field_type->set_defined_in (node->defined_in ());

  // Create the argument
  be_argument *arg = new be_argument (AST_Argument::dir_IN,
                                      field_type, // is also a valuetype
                                      new UTL_ScopedName 
                                      (new Identifier
                                          ("ami_handler", 1, 0, I_FALSE),
                                        0),
                                      0);

  // Add the reply handler to the argument list
  op->add_argument_to_scope (arg);

  // Iterate over the arguments and put all the in and inout 
  // into the new method.
  if (node->nmembers () > 0)
    {
      // initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (node,
                                               UTL_Scope::IK_decls),
                      0);

      // continue until each element is visited
      while (!si->is_done ())
        {
          AST_Decl *d = si->item ();
          if (!d)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_ami_pre_proc::create_sendc_method - "
                                 "bad node in this scope\n"),
                                0);

            }
            //be_decl *arg = be_decl::narrow_from_decl (d);
          AST_Argument *original_arg = AST_Argument::narrow_from_decl (d);
          
          if (original_arg->direction () == AST_Argument::dir_IN ||
              original_arg->direction () == AST_Argument::dir_INOUT)
            {
              // Create the argument
              be_argument *arg = new be_argument (AST_Argument::dir_IN,
                                                  original_arg->field_type (), 
                                                  original_arg->name (),
                                                  0);

              op->add_argument_to_scope (arg);
            }
          si->next ();
        } // end of while loop
      delete si;
    } // end of if
  
  return op;
}


be_operation * 
be_visitor_ami_pre_proc::create_reply_handler_operation (be_operation *node,
                                                         be_interface *reply_handler)
{
  ACE_DEBUG ((LM_DEBUG, "be_visitor_ami_pre_proc::create_reply_handler_operation\n"));
  // Create the return type, which is "void"
  be_predefined_type *rt = new be_predefined_type (AST_PredefinedType::PT_void,
                                                   new UTL_ScopedName
                                                     (new Identifier ("void", 1, 0, I_FALSE), 
                                                      0),
                                                   0);

  ACE_CString original_op_name (node
                                  ->name ()
                                    ->last_component ()
                                      ->get_string ());

  UTL_ScopedName *op_name = (UTL_ScopedName *)reply_handler->name ()-> copy ();
  op_name->nconc (new UTL_ScopedName (
                    new Identifier (
                      original_op_name.rep (), 1, 0, I_FALSE),
                    0));

  // Create the operation
  be_operation * op = new be_operation (rt, 
                                        AST_Operation::OP_noflags,
                                        op_name, 
                                        0);
  op->set_name (op_name);

  // If return type is non-void add it as first argument

  if (!node->void_return_type ())
    {

      // Create the argument
      be_argument *arg = new be_argument (AST_Argument::dir_IN,
                                          node->return_type (), 
                                          new UTL_ScopedName 
                                          (new Identifier
                                              ("_tao_retval", 1, 0, I_FALSE),
                                            0),
                                          0);

      // Add the reply handler to the argument list
      op->add_argument_to_scope (arg);
    }

  // Iterate over the arguments and put all the in and inout 
  // into the new method.
  if (node->nmembers () > 0)
    {
      // initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (node,
                                               UTL_Scope::IK_decls),
                      0);

      // continue until each element is visited
      while (!si->is_done ())
        {
          AST_Decl *d = si->item ();
          if (!d)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_ami_pre_proc::create_reply_handler_operation - "
                                 "bad node in this scope\n"),
                                0);

            }
            //be_decl *arg = be_decl::narrow_from_decl (d);
          AST_Argument *original_arg = AST_Argument::narrow_from_decl (d);
          
          if (original_arg->direction () == AST_Argument::dir_INOUT ||
              original_arg->direction () == AST_Argument::dir_OUT)
            {
              // Create the argument
              be_argument *arg = new be_argument (AST_Argument::dir_IN,
                                                  original_arg->field_type (), 
                                                  original_arg->name (),
                                                  0);

              op->add_argument_to_scope (arg);
            }
          si->next ();
        } // end of while loop
      delete si;
    } // end of if

  // Set the proper strategy
  be_operation_strategy *old_strategy = 
    op->set_strategy (new be_operation_ami_handler_reply_stub_strategy (op));
  if (old_strategy)
    delete old_strategy;

  return op;
}

be_operation *
be_visitor_ami_pre_proc::create_excep_operation (be_operation *node,
                                                 be_interface *reply_handler,
                                                 be_valuetype *excep_holder)
{
  ACE_DEBUG ((LM_DEBUG, "be_visitor_ami_pre_proc::create_excep_operation\n"));
  // Create the return type, which is "void"
  be_predefined_type *rt = new be_predefined_type (AST_PredefinedType::PT_void,
                                                   new UTL_ScopedName
                                                     (new Identifier ("void", 1, 0, I_FALSE), 
                                                      0),
                                                   0);

  // Create the argument
#if 0
  // Create the field type
  // Get the name of the interface
  // Get the scope name.
  be_decl *parent = be_scope::narrow_from_scope (node->defined_in ())->decl ();
  
  // Add the pre- and suffix
  ACE_CString excep_holder_local_name;
  this->generate_name (excep_holder_name,
                       "AMI_",
                       parent
                         ->name ()
                           ->last_component ()
                             ->get_string (),
                       "ExceptionHolder");
  UTL_ScopedName *field_name = (UTL_ScopedName *)reply_handler->name ()->copy ();
  field_name->last_component ()->replace_string (excep_holder_local_name.rep ());
  be_interface *field_type= new be_interface (field_name,
                                             0,
                                             0,
                                             0);
  field_type->set_name (field_name);
  field_type->set_defined_in (node->defined_in ());
#endif /* 0 */
  // Create the argument
  be_argument *arg = new be_argument (AST_Argument::dir_OUT,
                                      excep_holder, // is also a valuetype
                                      new UTL_ScopedName (
                                        new Identifier (
                                          "excep_holder", 1, 0, I_FALSE),
                                        0),
                                      0);

  // Create the new name
  // Append _execp to the name of the operation
  ACE_CString original_op_name (node
                                  ->name ()
                                    ->last_component ()
                                      ->get_string ());
  ACE_CString new_op_name = original_op_name + ACE_CString ("_excep");

  UTL_ScopedName *op_name = (UTL_ScopedName *)reply_handler->name ()-> copy ();
  op_name->nconc (new UTL_ScopedName (
                    new Identifier (
                      new_op_name.rep (), 1, 0, I_FALSE),
                    0));

  // create the operation
  be_operation * op = new be_operation (rt, 
                                        AST_Operation::OP_noflags,
                                        op_name, 
                                        0);
  op->set_name (op_name);
  op->add_argument_to_scope (arg);

  return op;
}


// visit the scope and its elements
int
be_visitor_ami_pre_proc::visit_scope (be_scope *node)
{
  ACE_DEBUG ((LM_DEBUG, "be_visitor_ami_pre_proc::visit_scope\n"));
  // proceed if the number of members in our scope is greater than 0
  if (node->nmembers () > 0)
    {
      int number_of_elements = 0;

      {
        // initialize an iterator to iterate thru our scope
        UTL_ScopeActiveIterator *si;
        ACE_NEW_RETURN (si,
                        UTL_ScopeActiveIterator (node,
                                                 UTL_Scope::IK_decls),
                        -1);

        while (!si->is_done ())
          {
            number_of_elements++;
            si->next ();
          }
        delete si;
      }
        
      ACE_DEBUG ((LM_DEBUG, 
                  "be_visitor_ami_pre_proc::visit_scope - %d elements\n",
                  number_of_elements));

      // initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (node,
                                               UTL_Scope::IK_decls),
                      -1);
      int elem_number = 0;
      
      // continue until each element is visited
      while (elem_number < number_of_elements)
        {
          AST_Decl *d = si->item ();
          if (!d)
            {
              delete si;
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
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "codegen for scope failed\n"), -1);

            }

          si->next ();
        } // end of while loop
      delete si;
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
