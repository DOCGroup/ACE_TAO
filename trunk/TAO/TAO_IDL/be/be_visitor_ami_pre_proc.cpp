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
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_pre_proc::"
                         "visit_interface - "
                         "visit scope failed\n"),
                        -1);
    }

  // Create a virtual module named "Messaging" and an valuetype "ExceptionHolder"
  // from which we inherit.
  UTL_ScopedName *inherit_name = 
    new UTL_ScopedName (new Identifier ("Messaging", 0,0,0),
                        0);
  
  inherit_name->nconc (new UTL_ScopedName (new Identifier ("ExceptionHolder", 0,0,0),
                                           0));
 
  be_valuetype *inherit_vt = new be_valuetype (inherit_name,
                                               0,
                                               0,
                                               0);
  inherit_vt->set_name (inherit_name);

  be_module *msg = new be_module (new UTL_ScopedName (new Identifier ("Messaging", 0,0,0),
                                                            0),
                                  0);
 
  // Notice the valuetype "ExceptionHolder" that it is defined in the 
  // "Messaging" module
  inherit_vt->set_defined_in (msg);

  // Create the excpetion holder name
  // @@ This should be done by a strategy though.
  ACE_CString excep_holder_last_name ("AMI_");
  excep_holder_last_name += node->name ()->last_component ()->get_string();
  excep_holder_last_name += "ExceptionHolder";

  UTL_ScopedName *excep_holder_name = (UTL_ScopedName *)node->name ()->copy ();
  excep_holder_name->last_component ()->replace_string (excep_holder_last_name.rep ());

  typedef AST_Interface *AST_Interface_ptr; 

  AST_Interface_ptr *p_intf = new AST_Interface_ptr[1];
  
  p_intf[0] = (AST_Interface *)inherit_vt;

  be_valuetype *excep_holder_vt = new be_valuetype (excep_holder_name,  // name
                                                    p_intf,             // list of inherited
                                                    1,                  // number of inherited
                                                    0,                  // pragmas
                                                    0);                 // set abstract
  excep_holder_vt->set_name (excep_holder_name);

  excep_holder_vt->set_defined_in (node->defined_in ());

  // Now our customized valuetype is created, we have to 
  // add now the operations and attributes to the scope.

  if (node->nmembers () > 0)
    {
      // initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (node,
                                               UTL_Scope::IK_decls),
                      -1);
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
                                 "bad node in this scope\n"), -1);

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
                                                                 (new Identifier ("void", 1, 0, I_FALSE), 
                                                                  0),
                                                               0);

              // Name the operation properly
              UTL_ScopedName *new_name = (UTL_ScopedName *) node->name ()->copy ();

              ACE_CString new_last_name ("raise_");
              new_last_name += op->name ()->last_component ()->get_string ();
  
              new_name->nconc (new UTL_ScopedName (new Identifier (new_last_name.rep (), 0,0,0),
                                                   0));

              be_operation *op_vt = new be_operation (rt,
                                                      AST_Operation::OP_noflags,
                                                      new_name,
                                                      0);

              // Copy the exceptions.
              if (((AST_Operation *)op)->exceptions ())
                {
                  UTL_ExceptList *exceptions = (UTL_ExceptList *)((AST_Operation *)op)->exceptions ()->copy ();
                  op_vt->add_exceptions (exceptions);
                }

              // After having generated the operation we insert it into the
              // exceptionholder valuetype.
              excep_holder_vt->add_operation_x (op_vt);
            }
          si->next ();
        } // end of while loop
      delete si;
    } // end of if


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

  this->create_sendc_method (node);

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


int 
be_visitor_ami_pre_proc::create_sendc_method (be_operation *node)
{
  ACE_DEBUG ((LM_DEBUG, "be_visitor_ami_pre_proc::create_sendc_method\n"));
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
  op->set_defined_in (node->defined_in ());


  // Create the first argument, which is a Reply Handler

  // Create the field type
  be_decl *parent = be_scope::narrow_from_scope (node->defined_in ())->decl ();
  ACE_CString interface_name (parent
                                ->name ()
                                  ->last_component ()
                                    ->get_string ());
  // Add the pre- and suffix
  ACE_CString excep_holder_name ("AMI_");
  excep_holder_name += interface_name;
  excep_holder_name += "Handler";
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
                                          ("reply_handler", 1, 0, I_FALSE),
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
                      -1);
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
                                 "bad node in this scope\n"), -1);

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

  AST_Interface *parent_interface =
    AST_Interface::narrow_from_scope (node->defined_in ());

  // Add the operation to the interface.
  parent_interface->add_operation_x (op);

  return 0;
}


int 
be_visitor_ami_pre_proc::create_excep_method (be_operation *node)
{
  ACE_DEBUG ((LM_DEBUG, "be_visitor_ami_pre_proc::create_excep_method\n"));
  // Create the return type, which is "void"
  be_predefined_type *rt = new be_predefined_type (AST_PredefinedType::PT_void,
                                                   new UTL_ScopedName
                                                     (new Identifier ("void", 1, 0, I_FALSE), 
                                                      0),
                                                   0);

  // Create the argument

  // Create the field type
  // Get the name of the interface
  // Get the scope name.
  be_decl *parent = be_scope::narrow_from_scope (node->defined_in ())->decl ();
  ACE_CString interface_name (parent
                                ->name ()
                                  ->last_component ()
                                    ->get_string ());

  // Add the pre- and suffix
  ACE_CString excep_holder_name ("AMI_");
  excep_holder_name += interface_name;
  excep_holder_name += "ExceptionHolder";
  UTL_ScopedName *field_name = (UTL_ScopedName *)parent->name ()->copy ();
  field_name->last_component ()->replace_string (excep_holder_name.rep ());
  be_interface *field_type= new be_interface (field_name,
                                             0,
                                             0,
                                             0);
  field_type->set_name (field_name);
  field_type->set_defined_in (node->defined_in ());

  // Create the argument
  be_argument *arg = new be_argument (AST_Argument::dir_OUT,
                                      field_type, // is also a valuetype
                                      new UTL_ScopedName 
                                      (new Identifier
                                          ("excep_holder", 1, 0, I_FALSE),
                                        0),
                                      0);

  // Create the new name
  // Append _execp to the name of the operation
  ACE_CString original_op_name (node
                                  ->name ()
                                    ->last_component ()
                                      ->get_string ());
  ACE_CString new_op_name = original_op_name + ACE_CString ("_excep");

  UTL_ScopedName *op_name = (UTL_ScopedName *)node->name ()-> copy ();
  op_name->last_component ()->replace_string (new_op_name.rep ());

  // create the operation
  be_operation * op = new be_operation (rt, 
                                        AST_Operation::OP_noflags,
                                        op_name, 
                                        0);
  op->set_defined_in (node->defined_in ());
  op->add_argument_to_scope (arg);

  // Add the operation to the interface.
  AST_Interface *parent_interface =
    AST_Interface::narrow_from_scope (node->defined_in ());

  parent_interface->add_operation_x (op);

  return 0;
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
