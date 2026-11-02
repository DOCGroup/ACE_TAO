
//=============================================================================
/**
 *  @file    attribute.cpp
 *
 *  Visitor for generation of code for Attribute
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "attribute.h"
#include "fe_utils.h"

#include <memory>
#include <string>

namespace
{
  struct AST_Decl_Destroyer
  {
    void operator() (AST_Decl *decl) const
    {
      if (decl != nullptr)
        {
          decl->destroy ();
        }
    }
  };
}

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

be_visitor_attribute::~be_visitor_attribute () = default;

int
be_visitor_attribute::visit_attribute (be_attribute *node)
{
  this->ctx_->node (node);
  this->ctx_->attribute (node);

  UTL_Scope *s = node->defined_in ();
  AST_Decl *d = ScopeAsDecl (s);
  std::string op_name (this->ctx_->port_prefix ());
  op_name += node->local_name ()->get_string ();
  // ACE_NEW_RETURN requires raw output pointers; adopt each allocation immediately.
  Identifier *op_id = nullptr;
  ACE_NEW_RETURN (op_id,
                  Identifier (op_name),
                  -1);
  std::unique_ptr<Identifier> op_id_owner (op_id);

  UTL_ScopedName *op_ln = nullptr;
  ACE_NEW_RETURN (op_ln,
                  UTL_ScopedName (op_id_owner.get (), nullptr),
                  -1);
  op_id_owner.release ();
  FE_Utils::Scoped_Name_Owner op_ln_owner (op_ln);

  FE_Utils::Scoped_Name_Owner op_sn (
    static_cast<UTL_ScopedName *> (d->name ()->copy ()));
  op_sn->nconc (op_ln_owner.get ());
  op_ln_owner.release ();

  // first the "get" operation
  be_operation get_op (node->field_type (),
                       AST_Operation::OP_noflags,
                       nullptr,
                       node->is_local (),
                       node->is_abstract ());
  std::unique_ptr<AST_Decl, AST_Decl_Destroyer> get_op_owner (&get_op);

  get_op.set_defined_in (s);
  get_op.set_name (op_sn.get ());
  op_sn.release ();
  UTL_ExceptList *get_exceptions = node->get_get_exceptions ();

  if (nullptr != get_exceptions)
    {
      get_op.be_add_exceptions (get_exceptions->copy ());
    }

  be_visitor_context ctx (*this->ctx_);
  int status = 1;

  switch (this->ctx_->state ())
    {
    // These two cases are the only ones that could involve a strategy.
    case TAO_CodeGen::TAO_ROOT_CH:
    case TAO_CodeGen::TAO_INTERFACE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
        be_visitor_operation_ch visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
        be_visitor_operation_cs visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
      {
        be_visitor_operation_sh visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IH:
      {
        be_visitor_operation_ih visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SS:
      {
        be_visitor_operation_ss visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IS:
      {
        be_visitor_operation_is visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
      {
        be_visitor_operation_proxy_impl_xh visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS:
      {
        be_visitor_operation_direct_proxy_impl_ss visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH:
      {
        be_visitor_operation_smart_proxy_ch visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS:
      {
        be_visitor_operation_smart_proxy_cs visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_TIE_SH:
      {
        be_visitor_operation_tie_sh visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_TIE_SS:
      {
        be_visitor_operation_tie_ss visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SVTH:
    case TAO_CodeGen::TAO_ROOT_SVH:
      {
        be_visitor_operation_ch visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SVTS:
    case TAO_CodeGen::TAO_ROOT_SVS:
      {
        be_visitor_operation_svs visitor (&ctx);
        visitor.scope (this->op_scope_);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_EXH:
      {
        be_visitor_operation_ch visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_EXS:
      {
        be_visitor_operation_exs visitor (&ctx);
        visitor.scope (this->op_scope_);
        visitor.class_extension (this->exec_class_extension_);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CNH:
    case TAO_CodeGen::TAO_ROOT_CNS:
      break;
    default:
      return 0;
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "codegen for get_attribute failed\n"),
                        -1);
    }

  // Do nothing for readonly attributes.
  if (node->readonly ())
    {
      return 0;
    }

  status = 1;

  // Create the set method.
  Identifier id ("void");
  UTL_ScopedName sn (&id,
                     nullptr);

  // The return type  is "void".
  be_predefined_type rt (AST_PredefinedType::PT_void,
                         &sn);
  std::unique_ptr<AST_Decl, AST_Decl_Destroyer> rt_owner (&rt);

  // Argument type is the same as the attribute type.
  AST_Argument *arg =
    idl_global->gen ()->create_argument (AST_Argument::dir_IN,
                                         node->field_type (),
                                         node->name ());
  std::unique_ptr<AST_Argument, AST_Decl_Destroyer> arg_owner (arg);

  FE_Utils::Scoped_Name_Owner arg_name (
    static_cast<UTL_ScopedName *> (node->name ()->copy ()));
  arg->set_name (arg_name.get ());
  arg_name.release ();

  // Create the operation.
  be_operation set_op (&rt,
                       AST_Operation::OP_noflags,
                       nullptr,
                       node->is_local (),
                       node->is_abstract ());
  std::unique_ptr<AST_Decl, AST_Decl_Destroyer> set_op_owner (&set_op);
  set_op.set_defined_in (node->defined_in ());
  FE_Utils::Scoped_Name_Owner set_op_name (
    static_cast<UTL_ScopedName *> (get_op.name ()->copy ()));
  set_op.set_name (set_op_name.get ());
  set_op_name.release ();
  if (set_op.be_add_argument (arg_owner.get ()) == nullptr)
    {
      return -1;
    }
  arg_owner.release ();

  UTL_ExceptList *set_exceptions = node->get_set_exceptions ();

  if (nullptr != set_exceptions)
    {
      set_op.be_add_exceptions (set_exceptions->copy ());
    }

  ctx = *this->ctx_;
  status = 1;

  switch (this->ctx_->state ())
    {
    // These two cases are the only ones that could involved a strategy.
    case TAO_CodeGen::TAO_ROOT_CH:
    case TAO_CodeGen::TAO_INTERFACE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
        be_visitor_operation_ch visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
        be_visitor_operation_cs visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
      {
        be_visitor_operation_sh visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IH:
      {
        be_visitor_operation_ih visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SS:
      {
        be_visitor_operation_ss visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IS:
      {
        be_visitor_operation_is visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
      {
        be_visitor_operation_proxy_impl_xh visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS:
      {
        be_visitor_operation_direct_proxy_impl_ss visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH:
      {
        be_visitor_operation_smart_proxy_ch visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS:
      {
        be_visitor_operation_smart_proxy_cs visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_TIE_SH:
      {
        be_visitor_operation_tie_sh visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_TIE_SS:
      {
        be_visitor_operation_tie_ss visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SVTH:
    case TAO_CodeGen::TAO_ROOT_SVH:
      {
        be_visitor_operation_ch visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SVTS:
    case TAO_CodeGen::TAO_ROOT_SVS:
      {
        be_visitor_operation_svs visitor (&ctx);
        visitor.scope (this->op_scope_);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_EXH:
      {
        be_visitor_operation_ch visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_EXS:
      {
        be_visitor_operation_exs visitor (&ctx);
        visitor.scope (this->op_scope_);
        visitor.class_extension (this->exec_class_extension_);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CNH:
    case TAO_CodeGen::TAO_ROOT_CNS:
      break;
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "bad codegen state\n"),
                        -1);
    }

  if (status == 0)
    {
      return 0;
    }
  else if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "codegen for get_attribute failed\n"),
                        -1);
    }

  return 0;
}

void
be_visitor_attribute::for_facets (bool val)
{
  this->for_facets_ = val;
}

void
be_visitor_attribute::op_scope (be_decl *node)
{
  this->op_scope_ = node;
}
