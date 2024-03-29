
//=============================================================================
/**
 *  @file    operation_svs.cpp
 *
 *  Visitor generating code for Operation in the CIAO servants file.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "operation.h"
#include "be_visitor_operation/operation_svs.h"

be_visitor_operation_svs::be_visitor_operation_svs (
      be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    scope_ (nullptr)
{
}

be_visitor_operation_svs::~be_visitor_operation_svs ()
{
}

int
be_visitor_operation_svs::visit_operation (be_operation *node)
{
  this->ctx_->node (node);

  os_ << be_nl_2;


  if (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_SVTS)
    {
      os_ << "template <typename BASE, typename EXEC, typename CONTEXT>"
          << be_nl;
    }
  // Retrieve the operation return type.
  be_type *bt = dynamic_cast<be_type*> (node->return_type ());

  if (bt == nullptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_svs::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Generate the return type mapping (same as in the header file)
  be_visitor_context ctx = *this->ctx_;
  be_visitor_operation_rettype rt_visitor (&ctx);

  if (bt->accept (&rt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_svs::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }

  // Generate the operation name, avoiding possible _cxx_ prefix.
  // USE STATE con the context!!!!!!!
  if (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_SVTS)
    {
      os_ << be_nl
          << scope_->original_local_name ()->get_string ()
          << "_Servant_T<BASE, EXEC, CONTEXT>";
    }
  else
    {
      os_ << be_nl
          << scope_->original_local_name ()->get_string ()
          << "_Servant";
    }

  os_ << "::" << node->local_name ();

  // Generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  be_visitor_operation_arglist al_visitor (&ctx);

  if (node->accept (&al_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_svs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  return this->gen_op_body (node);
}

int
be_visitor_operation_svs::gen_op_body (be_operation *node)
{
  os_ << be_nl
      << "{" << be_idt_nl;

  ACE_CString sname_str (ScopeAsDecl (scope_->defined_in ())->full_name ());
  const char *global = (sname_str == "" ? "" : "::");

  if (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_SVTS)
    {
      os_ << "typename EXEC::_var_type executor = " << be_idt_nl
          << "EXEC::_duplicate (this->executor_.in ());" << be_uidt;
    }
  else
    {
      os_ << "::" << sname_str << global << "CCM_" << scope_->original_local_name ()
          << "_var executor = " << be_idt_nl
          << "::" << sname_str << global << "CCM_" << scope_->original_local_name ()
          << "::_duplicate (this->executor_.in ());" << be_uidt;
    }

  os_ << be_nl_2
      << "if (::CORBA::is_nil (executor.in ()))"
      << be_idt_nl
      << "{"<< be_idt_nl
      << "throw ::CORBA::INV_OBJREF ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  bool const vrt = node->void_return_type ();

  if (!vrt)
    {
      os_ << "return ";
    }

  os_ << "executor->" << node->local_name () << " (";

  if (node->argument_count () == 0)
    {
      os_ << ");";
    }
  else
    {
      os_ << be_idt_nl;

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_operation_svs")
                             ACE_TEXT ("::gen_op_body - ")
                             ACE_TEXT ("visit_scope() failed\n")),
                            -1);
        }
    }

  os_ << be_uidt_nl
     << "}";

  return 0;
}

int
be_visitor_operation_svs::visit_argument (be_argument *node)
{
  os_ << node->local_name ();

  return 0;
}

int
be_visitor_operation_svs::post_process (be_decl *bd)
{
  if (this->last_node (bd))
    {
      os_ << ");" << be_uidt;
    }
  else
    {
      os_ << "," << be_nl;
    }

  return 0;
}

void
be_visitor_operation_svs::scope (be_decl *node)
{
  this->scope_ = node;
}
