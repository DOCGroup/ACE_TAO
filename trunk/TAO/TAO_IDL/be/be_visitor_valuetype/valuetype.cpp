//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype.cpp
//
// = DESCRIPTION
//    Visitor generating code for Valuetypes. This is a generic visitor.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    based on interface.cpp from Aniruddha Gokhale
//
// ============================================================================

//#include      "idl.h"
//#include      "idl_extern.h"
//#include      "be.h"

#include "be_visitor_valuetype.h"

ACE_RCSID(be_visitor_valuetype, valuetype, "$Id$")

be_visitor_valuetype::be_visitor_valuetype (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_valuetype::~be_visitor_valuetype (void)
{
}

// this method must be overridden by the derived valuetype visitors
int
be_visitor_valuetype::visit_valuetype (be_valuetype *)
{
//!  ACE_ASSERT (0);
  return -1;
}

// visit the scope of the valuetype node
// (in public/private field order)
int
be_visitor_valuetype::visit_valuetype_scope (be_valuetype *node)
{
  int n_processed = 0;

  this->elem_number_ = 0;
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      AST_Decl *d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "bad node in this scope\n"), -1);
        }

      AST_Field *field = AST_Field::narrow_from_decl (d);

      if (field && field->visibility() == AST_Field::vis_PRIVATE)
        {
          continue;      // ignore private fields in this run
          // AST_Attribute derives from AST_Field, so test for
          // vis_PRIVATE is ok (the attribute has it set to vis_NA)
        }

      be_decl *bd = be_decl::narrow_from_decl (d);
      // set the scope node as "node" in which the code is being
      // generated so that elements in the node's scope can use it
      // for code generation

      this->ctx_->scope (node->decl ());

      // set the node to be visited
      this->ctx_->node (bd);
      this->elem_number_++;

      if (bd == 0 || bd->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "codegen for scope failed\n"), -1);
          
        }
    } // end of for loop

  this->elem_number_ = 0;
  for (UTL_ScopeActiveIterator sj (node, UTL_Scope::IK_decls);
       !sj.is_done ();
       sj.next())
    {
      AST_Decl *d = sj.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "bad node in this scope\n"), -1);
        }

      AST_Field *field = AST_Field::narrow_from_decl (d);

      if (!field ||
          (field && field->visibility() != AST_Field::vis_PRIVATE))
        {
          continue;      // only private fields in this run
        }

      ++ n_processed;

      if (n_processed == 1)
        {
          this->begin_private ();
        }

      be_decl *bd = be_decl::narrow_from_decl (d);
      // set the scope node as "node" in which the code is being
      // generated so that elements in the node's scope can use it
      // for code generation

      this->ctx_->scope (node->decl ());

      // set the node to be visited
      this->ctx_->node (bd);
      this->elem_number_++;

      if (bd == 0 || bd->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "codegen for scope failed\n"),
                            -1);
        }
    }

  return 0;
}

// this two are called from visit_valuetype_scope()
void
be_visitor_valuetype::begin_public ()
{
  // in derived visitors print "public:" in class definition
}

void
be_visitor_valuetype::begin_private ()
{
  // in derived visitors print "protected:" in class definition
}

// =all common visit methods for valuetype visitor

// visit an attribute
int
be_visitor_valuetype::visit_attribute (be_attribute *node)
// was be_visitor_attribute::visit_attribute (be_attribute *node)
{
  this->ctx_->node (node); // save the node
  this->ctx_->attribute (node); // save this attribute node

  be_operation *op;

  // first the "get" operation
  op = new be_operation (node->field_type (),
                         AST_Operation::OP_noflags,
                         node->name (),
                         0,
                         0);
  op->set_name ((UTL_IdList *) node->name ()->copy ());
  if (!op || this->visit_operation (op) == -1)
    {
      delete op;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "codegen for get_attribute failed\n"),
                        -1);
    }
  delete op;
  if (node->readonly ())
    return 0;  // nothing else to do

  // the set method.
  // the return type  is "void"
  be_predefined_type *rt =
    new be_predefined_type (AST_PredefinedType::PT_void,
                            new UTL_ScopedName (new Identifier ("void"),
                                                0));
  // argument type is the same as the attribute type
  be_argument *arg = new be_argument (AST_Argument::dir_IN,
                                      node->field_type (),
                                      node->name ());
  arg->set_name ((UTL_IdList *) node->name ()->copy ());
  // create the operation
  op = new be_operation (rt,
                         AST_Operation::OP_noflags,
                         node->name (),
                         0,
                         0);
  op->set_name ((UTL_IdList *) node->name ()->copy ());
  op->add_argument_to_scope (arg);

 if (!op || this->visit_operation (op) == -1)
    {
      delete op;
      delete arg;
      delete rt;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "codegen for set_attribute failed\n"),
                        -1);
    }
  delete op;
  delete rt;
  delete arg;
  return 0;
}


// visit a constant
int
be_visitor_valuetype::visit_constant (be_constant *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_VALUETYPE_CH:
      ctx.state (TAO_CodeGen::TAO_CONSTANT_CH);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CS:
      ctx.state (TAO_CodeGen::TAO_CONSTANT_CS);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CH:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CI:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CS:
    case TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CH:
    case TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CS:
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CH:
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CI:
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CS:
    case TAO_CodeGen::TAO_VALUETYPE_CI:
    case TAO_CodeGen::TAO_VALUETYPE_SH:
    case TAO_CodeGen::TAO_VALUETYPE_IH:
    case TAO_CodeGen::TAO_VALUETYPE_IS:
    case TAO_CodeGen::TAO_VALUETYPE_SI:
    case TAO_CodeGen::TAO_VALUETYPE_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_valuetype::"
                           "visit_constant - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_constant - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_constant - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

//visit an enum
int
be_visitor_valuetype::visit_enum (be_enum *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_VALUETYPE_CH:
      ctx.state (TAO_CodeGen::TAO_ENUM_CH);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CS:
      ctx.state (TAO_CodeGen::TAO_ENUM_CS);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CH:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CI:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CS:
    case TAO_CodeGen::TAO_VALUETYPE_CI:
    case TAO_CodeGen::TAO_VALUETYPE_SH:
    case TAO_CodeGen::TAO_VALUETYPE_IH:
    case TAO_CodeGen::TAO_VALUETYPE_IS:
    case TAO_CodeGen::TAO_VALUETYPE_SI:
    case TAO_CodeGen::TAO_VALUETYPE_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_valuetype::"
                           "visit_enum - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_enum - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_enum - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}


// visit an operation
int
be_visitor_valuetype::visit_operation (be_operation *)
{
  // is overridden in derived visitors
  return 0;
}

// visit an exception (not used)
int
be_visitor_valuetype::visit_exception (be_exception *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
  // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_VALUETYPE_CH:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CH);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CI:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CI);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CS:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CS);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CS);
      break;

    case TAO_CodeGen::TAO_VALUETYPE_SH:
    case TAO_CodeGen::TAO_VALUETYPE_IH:
    case TAO_CodeGen::TAO_VALUETYPE_SI:
    case TAO_CodeGen::TAO_VALUETYPE_SS:
    case TAO_CodeGen::TAO_VALUETYPE_IS:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CH:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CI:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CS:
    case TAO_CodeGen::TAO_VALUETYPE_COLLOCATED_SH:
    case TAO_CodeGen::TAO_VALUETYPE_COLLOCATED_SS:
    case TAO_CodeGen::TAO_VALUETYPE_MARSHAL_CH:
    case TAO_CodeGen::TAO_VALUETYPE_MARSHAL_CS:
    case TAO_CodeGen::TAO_VALUETYPE_INIT_CH:
    case TAO_CodeGen::TAO_VALUETYPE_INIT_CI:
    case TAO_CodeGen::TAO_VALUETYPE_INIT_CS:
    case TAO_CodeGen::TAO_VALUETYPE_INIT_ARGLIST_CH:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
                           "visit_exception - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_exception - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_exception - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit a structure
int
be_visitor_valuetype::visit_structure (be_structure *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_VALUETYPE_CH:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CI:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CI);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CS:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CH:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CI:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CS:
    case TAO_CodeGen::TAO_VALUETYPE_SH:
    case TAO_CodeGen::TAO_VALUETYPE_IH:
    case TAO_CodeGen::TAO_VALUETYPE_IS:
    case TAO_CodeGen::TAO_VALUETYPE_SI:
    case TAO_CodeGen::TAO_VALUETYPE_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_valuetype::"
                           "visit_structure - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_structure - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_structure - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit a union
int
be_visitor_valuetype::visit_union (be_union *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_VALUETYPE_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_CH);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CI:
      ctx.state (TAO_CodeGen::TAO_UNION_CI);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_CS);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CH:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CI:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CS:
    case TAO_CodeGen::TAO_VALUETYPE_SH:
    case TAO_CodeGen::TAO_VALUETYPE_IH:
    case TAO_CodeGen::TAO_VALUETYPE_IS:
    case TAO_CodeGen::TAO_VALUETYPE_SI:
    case TAO_CodeGen::TAO_VALUETYPE_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_valuetype::"
                           "visit_union - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_union - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_union - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit a typedef
int
be_visitor_valuetype::visit_typedef (be_typedef *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_VALUETYPE_CH:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CH);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CI:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CI);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CS:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CS);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CH:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CI:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CS:
    case TAO_CodeGen::TAO_VALUETYPE_SH:
    case TAO_CodeGen::TAO_VALUETYPE_IH:
    case TAO_CodeGen::TAO_VALUETYPE_IS:
    case TAO_CodeGen::TAO_VALUETYPE_SI:
    case TAO_CodeGen::TAO_VALUETYPE_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_valuetype::"
                           "visit_typedef - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_typedef - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_typedef - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

int
be_visitor_valuetype::visit_field (be_field *)
{
  // is overridden in derived visitors
  return 0;
}


// private data fields for scope
int
be_visitor_valuetype::gen_pd (be_valuetype *node)
{
  int n_processed = 0;

  this->elem_number_ = 0;

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      AST_Decl *d = si.item ();
      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "bad node in this scope\n"), -1);
        }
      be_field *field = be_field::narrow_from_decl (d);
      if (!field)
        {
          continue;
        }
      ++ n_processed;
      // set the scope node as "node" in which the code is being
      // generated so that elements in the node's scope can use it
      // for code generation

      this->ctx_->scope (node->decl ());

      // set the node to be visited
      this->ctx_->node (field);
      this->elem_number_++;

      if (this->gen_field_pd (field) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "codegen for scope failed\n"), -1);

        }
    }
  return 0;
}

// private data for field
int
be_visitor_valuetype::gen_field_pd (be_field *node)
// derived from be_visitor_field_ch
{
  TAO_OutStream *os; // output stream
  be_type *bt; // field's type
  be_valuetype *vt;  // field declared in this scope

  os = this->ctx_->stream ();
  // first generate the type information
  bt = be_type::narrow_from_decl (node->field_type ());
  vt = be_valuetype::narrow_from_scope (node->defined_in ());
  if (!bt || !vt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_ch::"
                         "visit_field - "
                         "Bad field type\n"
                         ), -1);
    }

  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  ctx.sub_state (TAO_CodeGen::TAO_USE_FULL_NAME);
  ctx.state (TAO_CodeGen::TAO_FIELD_CH);

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure::"
                         "visit_field - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (bt->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_ch::"
                         "visit_field - "
                         "codegen for field type failed\n"
                         ), -1);
    }
  delete visitor;

  // now output the field name.
  *os << " " << vt->field_pd_prefix ()
             << node->local_name ()
             << vt->field_pd_postfix() << ";" << be_nl;
  return 0;

}


// generate the _init definition
int
be_visitor_valuetype::gen_init_defn (be_valuetype *node)
{
  if (node->is_abstract_valuetype ())
    return 0;   // no instance of it can created

  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();

  *os << "class " << be_global->stub_export_macro ()
      << " " << node->local_name ()
      << "_init : public CORBA_ValueFactoryBase" << be_nl;

  // generate the body

  *os << "{" << be_nl
      << "public:" << be_idt_nl
      << "virtual ~" << node->local_name () << "_init ();" << be_nl;

      /* %!     << "static " << node->local_name () << "* "
              << "_downcast (CORBA::ValueFactoryBase* );" << be_nl */

  *os << "virtual const char* tao_repository_id (void);\n" << be_nl;
  *os << "// create () goes here" << be_nl;  // %!

  *os << be_uidt_nl << "};" << be_nl;

  return 0;
}

int
be_visitor_valuetype::gen_init_impl (be_valuetype *node)
{
  if (node->is_abstract_valuetype ())
    return 0;   // no instance of it can created

  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();
  os->indent (); // start with whatever indentation level we are at

  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _out names

  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (fname, "%s_init", node->full_name ());

  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (lname, "%s_init", node->local_name ());

  // destructor
  *os << fname << "::~" << lname << " (void)" << be_nl
      << "{" << be_nl << "}\n\n";

  *os << "const char* " << be_nl
      << fname << "::tao_repository_id (void)" << be_nl
      << "{" << be_idt_nl
      <<   "return " << node->local_name ()
      <<                "::_tao_obv_static_repository_id ();"
      << be_uidt_nl << "}\n\n";


  return 0;
}

be_visitor_valuetype::FactoryStyle
be_visitor_valuetype::determine_factory_style (be_valuetype* node)
{
  FactoryStyle factory_style = FS_UNKNOWN;

  if (node == 0)
    {
      return factory_style;
    }

  // Check whether we have at least one operation or not
  idl_bool have_operation = be_visitor_valuetype::have_operation(node);


  idl_bool have_factory = 0;

  // Try only our own scope
  if (node->nmembers () > 0)
    {
      // initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator si (node,
                                  UTL_Scope::IK_decls);

      // Continue until each element is visited.
      for (; !si.is_done (); si.next())
        {
          AST_Decl *d = si.item ();

          if (!d)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_valuetype_init::"
                                 "determine_factory_style"
                                 "bad node in this scope\n"),
                                factory_style);

            }

          AST_Decl::NodeType node_type = d->node_type();

          if (node_type == AST_Decl::NT_factory)
            {
              have_factory = 1;
              break;
            }

        } // end of for loop
    } // end of if

  if(!have_operation && !have_factory)
    {
      factory_style = FS_CONCRETE_FACTORY;
    }
  else if(have_operation && !have_factory)
    {
      factory_style = FS_NO_FACTORY;
    }
  else
    {
      factory_style = FS_ABSTRACT_FACTORY;
    }

  return factory_style;
}

idl_bool
be_visitor_valuetype::have_operation(be_valuetype* node)
{
  // Check whatever scope we get for operations/attributes

  if (node == 0)
    {
      return 0;
    }

  idl_bool have_operation = 0;

  // Operations are either operations or attributes of:
  // -its own
  // -derived (abstract VT | VT | abstract iface | iface)
  //

  // First try our own scope
  if (node->nmembers () > 0)
    {
      // Initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator si (node,
                                  UTL_Scope::IK_decls);

      // Continue until each element is checked.
      for (; !si.is_done (); si.next())
        {
          AST_Decl *d = si.item ();

            if (!d)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_valuetype_init::"
                                 "has_operation"
                                 "bad node in this scope\n"),
                                0);

            }

          AST_Decl::NodeType node_type = d->node_type();

          if(node_type == AST_Decl::NT_op)
            {
              have_operation = 1;
              continue;
            }

          if(node_type == AST_Decl::NT_attr)
            {
              have_operation = 1;
              continue;
            }

          if(node_type == AST_Decl::NT_factory)
            {
              continue;
            }

          if(node_type == AST_Decl::NT_field)
            {
              continue;
            }

        } // end of for loop
    } // end of if

  //Now traverse inheritance tree.
  int i;  // loop index

  AST_Interface *iface =
    AST_Interface::narrow_from_scope (node);

  for (i = 0; i < iface->n_inherits (); ++i)
    {
      AST_Interface *inherited =
        AST_Interface::narrow_from_decl (iface->inherits ()[i]);

      if (!inherited || !inherited->is_valuetype())
        {
          continue;
        }

      be_valuetype *vt = be_valuetype::narrow_from_decl(node->inherits ()[i]);

      if (vt != 0)
        {
          have_operation = have_operation ||
            be_visitor_valuetype::have_operation(vt);

          if(have_operation)
            {
              break;
            }
        }
    }  // end of for loop

  return have_operation;
}

idl_bool
be_visitor_valuetype::obv_need_ref_counter (be_valuetype* node)
{
  // VT needs RefCounter if it has concrete factory and
  // none of its base VT has ref_counter

  if (determine_factory_style(node) != FS_CONCRETE_FACTORY)
    {
      return 0;
    }

  // now go thru our base VTs and see if one has already

  int i;  // loop index

  for (i = 0; i < node->n_inherits (); ++i)
    {
      AST_Interface *inherited =
        AST_Interface::narrow_from_decl (node->inherits ()[i]);

      if (!inherited || !inherited->is_valuetype())
        {
          continue;
        }

      be_valuetype *vt = be_valuetype::narrow_from_decl(node->inherits ()[i]);

      if (vt != 0)
        {
          if (obv_have_ref_counter (vt))
            {
              return 0;
            }
        }
    }  // end of for loop

  return 1;
}

idl_bool
be_visitor_valuetype::obv_have_ref_counter (be_valuetype* node)
{

  // just try to find a VT with concrete factory in inheritance tree
  if(node == 0)
    {
      return 0;
    }

  if (determine_factory_style(node) == FS_CONCRETE_FACTORY)
    {
      return 1;
    }

  // now go thru our base VTs

  int i;  // loop index

  for (i = 0; i < node->n_inherits (); ++i)
    {
      AST_Interface *inherited =
        AST_Interface::narrow_from_decl (node->inherits ()[i]);

      if (!inherited || !inherited->is_valuetype())
        {
          continue;
        }

      be_valuetype *vt = be_valuetype::narrow_from_decl (node->inherits ()[i]);

      if (vt != 0)
        {
          if (obv_have_ref_counter (vt))
            {
              return 1;
            }
        }
    }  // end of for loop

  return 0;
}
