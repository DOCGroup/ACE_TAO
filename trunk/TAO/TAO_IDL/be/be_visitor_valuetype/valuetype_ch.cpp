//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Valuetypes in the client header
//    (see C++ mapping OMG 20.17)
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>,
//    based on interface_ch.cpp from Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype.h"

ACE_RCSID(be_visitor_valuetype, valuetype_ch, "$Id$")


// ******************************************************
// Valuetype visitor for client header
// ******************************************************

be_visitor_valuetype_ch::be_visitor_valuetype_ch (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_ch::~be_visitor_valuetype_ch (void)
{
}

int
be_visitor_valuetype_ch::visit_valuetype (be_valuetype *node)
{
  TAO_OutStream *os; // output stream

  if (!node->cli_hdr_gen () && !node->imported ())
    {
      os = this->ctx_->stream ();
      *os << "// Valuetype class" << be_nl;

      // == STEP 1: Generate the class name and class names we inherit ==

      // Forward declaration.
      *os << "class " << node->local_name () << ";" << be_nl;

      *os << "typedef " << node->local_name ()
          << " *" << node->local_name () << "_ptr;" << be_nl;

      // Generate the ifdefined macro for the _var type.
      os->gen_ifdef_macro (node->flat_name (), "_var");

      // Generate the _var declaration.
      if (node->gen_var_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_ch::"
                             "visit_valuetype - "
                             "codegen for _var failed\n"), 
                            -1);
        }

      os->gen_endif ();

      // Generate the ifdef macro for the _out class.
      os->gen_ifdef_macro (node->flat_name (), "_out");

      // Generate the _out declaration - ORBOS/97-05-15 pg 16-20 spec.
      if (node->gen_out_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_ch::"
                             "visit_valuetype - "
                             "codegen for _out failed\n"), -1);
        }

      // generate the endif macro.
      os->gen_endif ();

      // Generate the ifdef macro for the _init class.
      os->gen_ifdef_macro (node->flat_name (), "_init");

      // Generate the _init declaration - ptc/98-09-03 20.17.10 p.20-93.
      if (this->gen_init_defn (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_ch::"
                             "visit_valuetype - "
                             "codegen for _init failed\n"), -1);
        }

      // Generate the endif macro.
      os->gen_endif ();

      // Now the valuetype definition itself.
      os->gen_ifdef_macro (node->flat_name ());

      // Now generate the class definition.
      *os << "class " << be_global->stub_export_macro ()
                << " " << node->local_name ();

      // Node valuetype inherits from other valuetypes (OMG 20.17.9)
      // (ordinary (not abstract) interfaces ignored).

      *os << be_idt_nl <<": ";
      int i;  // loop index
      int n_inherits_valuetypes = 0;
      idl_bool valuebase_inherited = 0;

      if (node->n_inherits () > 0)
        {
          for (i = 0; i < node->n_inherits (); i++)
            {
              // %! move is_nested() and nested_type_name() to
              // AST_Interface, then type AST_Interface can be used
              be_interface *inherited =
                be_interface::narrow_from_decl (node->inherits ()[i]);

              if (!inherited->is_valuetype () 
                  && !inherited->is_abstract ())
                {
                  continue;
                }

              ++ n_inherits_valuetypes;

              if (inherited->is_valuetype())
                {
                  valuebase_inherited = 1;
                }

              if (n_inherits_valuetypes > 1)
                {
                  *os << "," << be_nl;
                }

              be_decl *scope = 0;

              if (inherited->is_nested ())
                {
                  // Inherited node is used in the scope of "node" node.
                  scope = 
                    be_scope::narrow_from_scope (node->defined_in ())->decl ();
                }

              // Dump the scoped name.
              *os << "public virtual ";
              *os << inherited->nested_type_name (scope);
            }  // end of for loop

          if (n_inherits_valuetypes > 0)
            {
              *os << be_uidt_nl;
            }
        }

      if (!valuebase_inherited)
        {
          // We do not inherit from any valuetype, hence we do so from the base
          // CORBA::ValueBase class.
          if (n_inherits_valuetypes > 1)
            {
              *os << ", ";
            }

          *os << "public virtual CORBA_ValueBase" << be_uidt_nl;
        }

      // Generate the body.
      *os << "{" << be_nl
          << "public:" << be_idt_nl

          // Generate the _ptr_type and _var_type typedef
          // but we must protect against certain versions of g++
          << "\n#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)"
          << be_nl
          << "typedef " << node->local_name () << "* _ptr_type;" << be_nl
          << "typedef " << node->local_name () << "_var _var_type;\n"
          << "#endif /* ! __GNUC__ || g++ >= 2.8 */" << be_nl << be_nl

          // Generate the static _downcast operation.
          // (see OMG 20.17.{4,5}).
          << "static " << node->local_name () << "* "
                << "_downcast (CORBA::ValueBase* );" << be_nl
          << "// The address of static _downcast is implicit used as type id\n"
          << be_nl
          << "// (TAO extensions or internals)" << be_nl
          << "static CORBA::Boolean _tao_unmarshal (" << be_idt << be_idt_nl
          << "TAO_InputCDR &," << be_nl
          << node->local_name () << " *&" << be_uidt_nl
          << ");" << be_uidt_nl
          << "virtual const char* "
          << "_tao_obv_repository_id () const;"
          << be_nl
          << "static const char* "
          << "_tao_obv_static_repository_id ();" << be_nl << be_nl;

      // Generate code for the valuetype definition.
      if (this->visit_valuetype_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_ch::"
                             "visit_valuetype - "
                             "codegen for scope failed\n"), 
                            -1);
        }

      // Protected member:

      // Generate the "protected" constructor so that users cannot
      // instantiate us.
      *os << be_uidt_nl << "protected:" << be_idt_nl
          << node->local_name ()
          << " ();" << be_nl
          << "virtual ~" << node->local_name () << " ();\n" << be_nl;

      *os << "// TAO internals" << be_nl
          << "virtual void *_tao_obv_narrow (ptr_arith_t);" << be_nl;

      // Support for marshalling.
      if (!node->is_abstract_valuetype ())
        {
          *os << "virtual CORBA::Boolean "
              << "_tao_marshal_v (TAO_OutputCDR &);" << be_nl;
          *os << "virtual CORBA::Boolean "
              << "_tao_unmarshal_v (TAO_InputCDR &);" << be_nl;
          // %! optimize _downcast away: extra parameter with type info
          // set (void *) in CDR Stream with the right derived pointer.
        }


      // Private member:

      // Private copy constructor and assignment operator. These are not
      // allowed, hence they are private.
      *os << be_uidt_nl << "private:" << be_idt_nl;
      *os << node->local_name () << " (const " << node->local_name () << " &);"
          << be_nl
          << "void operator= (const " << node->local_name () << " &);"
          << be_nl;

      // Map fields to private data (if optimizing).
      if (node->opt_accessor ())
        {
          *os << be_uidt_nl << "protected:" << be_idt_nl;
          *os << "CORBA::Boolean "
              << "_tao_marshal_state (TAO_OutputCDR &);" << be_nl
              << "CORBA::Boolean "
              << "_tao_unmarshal_state (TAO_InputCDR &);" 
              << be_uidt_nl << be_nl;
          *os << "private:" << be_idt_nl;
          this->gen_pd (node);
        }
      else // Need a way to access the state of derived OBV_ classes.
        {
          if (!node->is_abstract_valuetype ())
            {
              *os << be_uidt_nl << "protected:" << be_idt_nl;
              *os << "virtual CORBA::Boolean _tao_marshal__"
                  <<    node->flat_name () << " (TAO_OutputCDR &) = 0;"
                  << be_nl;
              *os << "virtual CORBA::Boolean _tao_unmarshal__"
                  <<    node->flat_name () << " (TAO_InputCDR &) = 0;"
                  << be_nl;
            }
        }

      *os << be_uidt_nl << "};" << be_nl;
      os->gen_endif ();

      node->cli_hdr_gen (I_TRUE);
    }

  return 0;
}


int
be_visitor_valuetype_ch::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  this->ctx_->node (node); // save the node

  // Every operation is declared virtual in the client code.
  *os << "virtual ";

  // STEP I: Generate the return type.
  bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ch::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Grab the right visitor to generate the return type.
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_CH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_valuetype_ch::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (bt->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ch::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }

  delete visitor;

  // STEP 2: Generate the operation name.
  *os << " " << node->local_name ();

  // STEP 3: Generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing.
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_CH);
  visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_valuetype_ch::"
                         "visit_operation - "
                         "Bad visitor to argument list\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ch::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  delete visitor;

  // @@ Michael: We need to put the exception throw spec here

  return 0;
}

int
be_visitor_valuetype_ch::visit_field (be_field *node)
{
  be_valuetype *vt = be_valuetype::narrow_from_scope (node->defined_in ());

  if (!vt)
    {
      return -1;
    }

  be_visitor_context* ctx = new be_visitor_context (*this->ctx_);
  ctx->state (TAO_CodeGen::TAO_FIELD_OBV_CH);
  be_visitor_valuetype_field_ch *visitor =
    new be_visitor_valuetype_field_ch (ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_valuetype_obv_ch::"
                         "visit_field - bad visitor\n"),
                        -1);
    }

  if (vt->opt_accessor ())
    {
      visitor->setenclosings ("",";");
    }
  else
    {
      visitor->setenclosings ("virtual "," = 0;");
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_obv_ch::"
                        "visit_field - codegen failed\n"),
                        -1);
    }

  delete visitor;
  return 0;
}


void
be_visitor_valuetype_ch::begin_public (void)
{
  TAO_OutStream *os = this->ctx_->stream ();
  *os << "public:" << be_idt_nl;
}

void
be_visitor_valuetype_ch::begin_private (void)
{
  TAO_OutStream *os = this->ctx_->stream ();
  *os << "protected:" << be_idt_nl;
}
