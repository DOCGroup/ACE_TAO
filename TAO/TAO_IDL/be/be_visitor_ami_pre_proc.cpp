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

              AST_Operation *operation = new AST_Operation (rt,
                                                            AST_Operation::OP_noflags,
                                                            node->name (),
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

