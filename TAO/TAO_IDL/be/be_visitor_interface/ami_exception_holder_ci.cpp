//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_exception_holder_ci.cpp
//
// = DESCRIPTION
//    Visitor generating exception holders for the AMI callback model.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"


// ******************************************************
// Interface visitor for client header
// ******************************************************

be_visitor_interface_ami_exception_holder_ci::be_visitor_interface_ami_exception_holder_ci (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ami_exception_holder_ci::~be_visitor_interface_ami_exception_holder_ci (void)
{
}

int
be_visitor_interface_ami_exception_holder_ci::visit_interface (be_interface *node)
{
  UTL_ScopedName *inherit_name = new UTL_ScopedName (new Identifier ("Messaging", 0,0,0),
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
 
  inherit_vt->set_defined_in (msg);

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


  // @@Michael: Set defined in.
  excep_holder_vt->set_defined_in (node->defined_in ());

  // proceed if the number of members in our scope is greater than 0
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
                                 "(%N:%l) be_visitor_scope::visit_scope - "
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

              UTL_ScopedName *new_name = (UTL_ScopedName *) node->name ()->copy ();

              ACE_CString new_last_name ("raise_");
              new_last_name += op->name ()->last_component ()->get_string ();
  
              new_name->nconc (new UTL_ScopedName (new Identifier (new_last_name.rep (), 0,0,0),
                                                   0));

              be_operation *op_vt = new be_operation (rt,
                                                      AST_Operation::OP_noflags,
                                                      new_name,
                                                      0);

              if (((AST_Operation *)op)->exceptions ())
                {
                  UTL_ExceptList *exceptions = (UTL_ExceptList *)((AST_Operation *)op)->exceptions ()->copy ();
                  op_vt->add_exceptions (exceptions);
                }

              excep_holder_vt->add_operation_x (op_vt);
            }
          si->next ();
        } // end of while loop
      delete si;
    } // end of if

    be_type *bt = be_type::narrow_from_decl (excep_holder_vt);

    // grab the right visitor to generate the code
    be_visitor_context ctx (*this->ctx_);
    ctx.state (TAO_CodeGen::TAO_VALUETYPE_CI);
    be_visitor *visitor = tao_cg->make_visitor (&ctx);

    if (!visitor)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "be_visitor_operation_cdr_op_ch::"
                           "visit_operation - "
                           "Bad visitor to return type\n"),
                          -1);
      }

    if (bt->accept (visitor) == -1)
      {
        delete visitor;
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_operation_cdr_op_ch::"
                           "visit_interface - "
                           "codegen for return type failed\n"),
                          -1);
      }
    delete visitor;

  return 0;
}
