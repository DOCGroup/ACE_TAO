//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    attribute.cpp
//
// = DESCRIPTION
//    Visitor for generation of code for Attribute
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_attribute.h"

ACE_RCSID(be_visitor_attribute, attribute, "$Id$")


// Attribute gets mapped to one or possibly two operations based on whether
// it is readonly or not. The two operations "get" and "set" the value of the
// attribute.
//
// code generation for attributes is achieved by mapping an attribute node to
// a corresponding operation node. The reason is that we do not want to
// duplicate the code. However, an attribute node and an operation node have
// different interfaces. Hence we need some form of an adapter that can take us
// from an attribute node to an operation node. The following paragraph
// describes how this is accomplished.
//
// For a "get" operation on the attribute, we create an operation node with the
// same name as the attribute and a return type which is the same as the type
// of the attribute. This operation has no arguments.
//
// For a "set" operation on the attribute (if it is not readonly), we create an
// operation node with a "void" return type and an "in" parameter of the same
// type as the attribute.

// In both cases the context will hold sufficient information to indicate that
// the operation node is really an attribute node.

// *************************************************************************
// primary visitor for "attribute". The context state decides the next state
// *************************************************************************

be_visitor_attribute::be_visitor_attribute (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_attribute::~be_visitor_attribute (void)
{
}

int
be_visitor_attribute::visit_attribute (be_attribute *node)
{
  this->ctx_->node (node); // save the node
  this->ctx_->attribute (node); // save this attribute node

  be_operation *op;

  // first the "get" operation
  op = new be_operation (node->field_type (), AST_Operation::OP_noflags,
                         node->name (), 0);
  op->set_name (node->name ());
  op->set_defined_in (node->defined_in ());
  be_visitor_context ctx (*this->ctx_);

  // this switch statement eliminates the need for different classes that have
  // exactly the same code except different states.
  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ATTRIBUTE_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_IH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_IH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_IS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_IS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_TIE_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_TIE_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_TIE_SI:
      ctx.state (TAO_CodeGen::TAO_OPERATION_TIE_SI);
      break;
    // AMI Reply Handler code generation
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_CH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_CS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_CS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_SH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_SH);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_SS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_SS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_THRU_POA_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_THRU_POA_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_THRU_POA_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_THRU_POA_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_DIRECT_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_DIRECT_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_DIRECT_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_DIRECT_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_TIE_SH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_TIE_SH);
      break;
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_ATTRIBUTE_CH:
      ctx.state (TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_ATTRIBUTE_CS:
      ctx.state (TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_OPERATION_CS);
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "bad codegen state\n"),
                        -1);
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || !op || (op->accept (visitor) == -1))
    {
      delete visitor;
      delete op;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "codegen for get_attribute failed\n"),
                        -1);
    }

  delete visitor;

  //
  // AMI Call back code generation.
  //
#if 0
  // Only if AMI callbacks are enabled.
  if (idl_global->ami_call_back () == I_TRUE)
    {
      if (this->ctx_->state () == TAO_CodeGen::TAO_ATTRIBUTE_CH ||
          this->ctx_->state () == TAO_CodeGen::TAO_ATTRIBUTE_CS)
        {
          // Generate AMI <sendc_> method, for this operation, if you are
          // doing client header.

          switch (this->ctx_->state ())
            {
            case TAO_CodeGen::TAO_ATTRIBUTE_CH:
              ctx.state (TAO_CodeGen::TAO_AMI_OPERATION_CH);
              break;

            case TAO_CodeGen::TAO_ATTRIBUTE_CS:
              ctx.state (TAO_CodeGen::TAO_AMI_OPERATION_CS);
              break;

            default:
              return 0;
            }

            // Grab the appropriate visitor.
            visitor = tao_cg->make_visitor (&ctx);
            if (!visitor)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_visitor_interface::"
                                   "visit_operation - "
                                   "NUL visitor\n"),
                                  -1);
              }

            // Visit the node using this visitor
            if (op->accept (visitor) == -1)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_visitor_interface::"
                                   "visit_operation - "
                                   "failed to accept visitor\n"),
                                  -1);
              }
            delete visitor;
          }
        else
          {
            // Rename the existing "fake" operation 
            // to node, so that the following code things it is 
            // original
            be_operation *node = op;
            switch (this->ctx_->state ())
              {
              case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_SH:
              case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_SS:
              case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_CH:
              case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_CS:
              case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_THRU_POA_COLLOCATED_SS:
              case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_THRU_POA_COLLOCATED_SH:
              case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_TIE_SH:
                {
                  // Create new be_operation

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

                  // Grab the appropriate visitor.
                  visitor = tao_cg->make_visitor (&ctx);
                  if (!visitor)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "(%N:%l) be_visitor_attribute::"
                                         "visit_attribute - "
                                         "NUL visitor\n"),
                                        -1);
                    }

                  // Visit the node using this visitor
                  if (op->accept (visitor) == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "(%N:%l) be_visitor_attribute::"
                                         "visit_attribute - "
                                         "failed to accept visitor\n"),
                                        -1);
                    }
                  delete visitor;
                  delete op;
                } // case {
                break;
              default:
                return 0;
            } // switch
        } // else
    } // if AMI
#endif /* 0 */

  if (op)
    {
      delete op;
      op = 0;
    }

  // Do nothing for readonly attributes.
  if (node->readonly ())
    return 0;  

  // the set method.
  // the return type  is "void"
  be_predefined_type *rt = new be_predefined_type (AST_PredefinedType::PT_void,
                                                   new UTL_ScopedName
                                                   (new Identifier
                                                    ("void", 1, 0, I_FALSE), 0),
                                                   0);
  // argument type is the same as the attribute type
  be_argument *arg = new be_argument (AST_Argument::dir_IN,
                                      node->field_type (),
                                      node->name (),
                                      0);
  arg->set_name (node->name ());
  // create the operation
  op = new be_operation (rt, AST_Operation::OP_noflags,
                         node->name (), 0);
  op->set_name (node->name ());
  op->set_defined_in (node->defined_in ());
  op->add_argument_to_scope (arg);

  ctx = *this->ctx_;
  // this switch statement eliminates the need for different classes that have
  // exactly the same code except different states.
  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ATTRIBUTE_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_IH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_IH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_IS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_IS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_TIE_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_TIE_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_TIE_SI:
      ctx.state (TAO_CodeGen::TAO_OPERATION_TIE_SI);
      break;
    // AMI Reply Handler code generation
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_CH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_CS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_CS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_SH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_SH);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_SS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_SS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_THRU_POA_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_THRU_POA_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_THRU_POA_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_THRU_POA_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_DIRECT_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_DIRECT_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_DIRECT_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_DIRECT_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_TIE_SH:
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_TIE_SH);
      break;
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_ATTRIBUTE_CH:
      ctx.state (TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_ATTRIBUTE_CS:
      ctx.state (TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_OPERATION_CS);
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "bad codegen state\n"),
                        -1);
    }

  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || !op || (op->accept (visitor) == -1))
    {
      delete visitor;
      delete op;
      delete arg;
      delete rt;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "codegen for set_attribute failed\n"),
                        -1);
    }
  delete visitor;


   //
  // AMI Call back code generation.
  //
#if 0
  // Only if AMI callbacks are enabled.
  if (idl_global->ami_call_back () == I_TRUE)
    {
      if (this->ctx_->state () == TAO_CodeGen::TAO_ATTRIBUTE_CH ||
          this->ctx_->state () == TAO_CodeGen::TAO_ATTRIBUTE_CS)
        {
          // Generate AMI <sendc_> method, for this operation, if you are
          // doing client header.

          switch (this->ctx_->state ())
            {
            case TAO_CodeGen::TAO_ATTRIBUTE_CH:
              ctx.state (TAO_CodeGen::TAO_AMI_OPERATION_CH);
              break;

            case TAO_CodeGen::TAO_ATTRIBUTE_CS:
              ctx.state (TAO_CodeGen::TAO_AMI_OPERATION_CS);
              break;

            default:
              return 0;
            }

            // Grab the appropriate visitor.
            visitor = tao_cg->make_visitor (&ctx);
            if (!visitor)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_visitor_interface::"
                                   "visit_operation - "
                                   "NUL visitor\n"),
                                  -1);
              }

            // Visit the node using this visitor
            if (op->accept (visitor) == -1)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_visitor_interface::"
                                   "visit_operation - "
                                   "failed to accept visitor\n"),
                                  -1);
              }
            delete visitor;
          }
        else
          {
            be_operation *node = op;
            switch (this->ctx_->state ())
              {
              case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_SH:
              case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_SS:
              case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_CH:
              case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_CS:
              case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_THRU_POA_COLLOCATED_SS:
              case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_THRU_POA_COLLOCATED_SH:
              case TAO_CodeGen::TAO_AMI_HANDLER_ATTRIBUTE_TIE_SH:
                {
                  // Create new be_operation

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

                  // Grab the appropriate visitor.
                  visitor = tao_cg->make_visitor (&ctx);
                  if (!visitor)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "(%N:%l) be_visitor_attribute::"
                                         "visit_attribute - "
                                         "NUL visitor\n"),
                                        -1);
                    }

                  // Visit the node using this visitor
                  if (op->accept (visitor) == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "(%N:%l) be_visitor_attribute::"
                                         "visit_attribute - "
                                         "failed to accept visitor\n"),
                                        -1);
                    }
                  delete visitor;
                  delete op;
                } // case {
                break;
              default:
                return 0;
            } // switch
        } // else
    } // if AMI
#endif /* 0 */
  
  delete op;
  delete rt;
  delete arg;
  return 0;
}
