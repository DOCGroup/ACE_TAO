

//=============================================================================
/**
 *  @file    valuetype_obv_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Valuetypes in the client header
 *  OBV_ class
 *  (see C++ mapping OMG 20.17)
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 *  @author based on interface_ch.cpp from Aniruddha Gokhale
 */
//=============================================================================


// ******************************************************
// Valuetype visitor for client header
// ******************************************************

be_visitor_valuetype_obv_ch::be_visitor_valuetype_obv_ch (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_obv_ch::~be_visitor_valuetype_obv_ch (void)
{
}


// OBV_ class must be in OBV_ namespace.
int
be_visitor_valuetype_obv_ch::visit_valuetype (be_valuetype *node)
{
  // Only visit non-abstract non-imported valuetype.
  if (node->is_abstract () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ ;

  // OBV_ class maps only to a typedef if we are optimizing accessors.
  if (node->opt_accessor ())
    {
      *os << be_nl_2 << "typedef " << node->full_name () << " ";

      if (!node->is_nested ())
        {
          *os << "OBV_";
        }

      *os << node->local_name () << ";" << be_nl;
    }
  else
    {
      *os << be_nl_2 << "// OBV_ class" << be_nl;
      *os << "class " << be_global->stub_export_macro() << " ";;

      if (!node->is_nested())
        {
          *os << "OBV_";
        }

      *os << node->local_name () << be_idt_nl
          << ": public virtual "
          << node->full_name ();

      // STEP 1 (about which previous implementer forgot ):
      // Generate inheritance from corresponding OBV_ classes.

//------>>>

      // Here we need to be careful. There are few cases to consider:
      //
      // (1) We have VT with concrete factory and no inhereted VT with
      //     concrete factory then we need to mix-in RefCounter
      //
      // (2) We have VT with concerete factory and inheretence from
      //     another (not abstract or empty abstract <would like to
      //     know how to go there>) then its OBV_ already has mix-in
      //
      // (3) We have VT that supports an abstract interface. In this case,
      //     we will add implementations of _add_ref and _remove_ref that
      //     call this->DefaultValueRefCountBase and so it must be mixed in
      //
      // (4) The rest. Don't need to bother about anything, just inherit
      //     whatever there is.
      //

      int i = 0;
      AST_Type *inherited = 0;

      for (; i < node->n_inherits (); ++i)
        {
          inherited = node->inherits ()[i];

          // We need only concrete valuetypes.
          if (inherited->is_abstract ())
            {
              continue;
            }

          *os << "," << be_nl;

          // dump the scoped name.
          *os << "  public virtual OBV_";
          *os << inherited->full_name();
        }  // end of for loop

      if (this->obv_need_ref_counter (node))
        {
          *os << "," << be_nl;

          // dump the scoped name.
          *os << "  public virtual ::CORBA::DefaultValueRefCountBase";
        }

      *os << be_uidt_nl;

//------>>>

      // STEP 2: Generate the body ==

      *os << "{";

      // Generate code for the OBV_ class definition.
      if (this->visit_valuetype_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_obv_ch::"
                             "visit_valuetype - "
                             "codegen for scope failed\n"),
                            -1);
        }

      // If we inherit from CORBA::Object and/or CORBA::AbstractBase
      // (in addition to CORBA::ValueBase) we have to add these
      // to avoid ambiguity.
      if (node->n_supports () > 0)
        {
          *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
              << "// " << __FILE__ << ":" << __LINE__ ;

          *os << be_nl_2 << "virtual void _add_ref (void);" << be_nl;
          *os << "virtual void _remove_ref (void);";
        }

      if (node->have_operation ())
        {
          this->begin_private ();
        }
      else
        {
          this->begin_public ();
        }

      *os << be_nl;

      // Default constructor.
      if (! node->is_nested ())
        {
          *os << "OBV_";
        }

      *os << node->local_name () << " (void);";

      // Initializing constructor.
      if (node->has_member ())
        {
          *os << be_nl;

          if (! node->is_nested ())
            {
              *os << "OBV_";
            }

          *os << node->local_name () << " (" << be_idt << be_idt;

          unsigned long index = 0;
          this->gen_obv_init_constructor_args (node, index);

          *os << be_uidt_nl
              << ");" << be_uidt;
        }

      // Virtual destructor.
      *os << be_nl << "virtual ~";

      if (! node->is_nested ())
        {
          *os << "OBV_";
        }

      *os << node->local_name () << " (void);";

      // Map fields to private data.
      if (!node->opt_accessor ())
        {
          *os << be_nl << be_uidt_nl << "protected:" << be_idt_nl;

          *os << "virtual ::CORBA::Boolean" << be_nl
              << "_tao_marshal__" << node->flat_name ()
              << " (TAO_OutputCDR &, TAO_ChunkInfo &) const;" << be_nl_2;

          *os << "virtual ::CORBA::Boolean" << be_nl
              << "_tao_unmarshal__" << node->flat_name ()
              << " (TAO_InputCDR &, TAO_ChunkInfo &);" << be_nl_2;

          *os << "::CORBA::Boolean "
              << "_tao_marshal_state (TAO_OutputCDR &, TAO_ChunkInfo &) const;"
              << be_nl
              << "::CORBA::Boolean "
              << "_tao_unmarshal_state (TAO_InputCDR &, TAO_ChunkInfo &);"
              << be_nl
              << "virtual void "
              << "truncation_hook (void);"
              << be_uidt_nl << be_nl;

          *os << "private:" << be_idt;

          this->gen_pd (node);
        }

      *os << be_nl
          << "CORBA::Boolean require_truncation_;" << be_uidt_nl
          << "};";
    }

  return 0;
}

int
be_visitor_valuetype_obv_ch::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_valuetype_obv_ch::visit_field (be_field *node)
{
  be_valuetype *vt = be_valuetype::narrow_from_scope (node->defined_in ());

  if (!vt)
    {
      return -1;
    }

  // Only in OBV_ class, if we are not optimizing accessors (and modifiers).
  if (!vt->opt_accessor ())
    {
      be_visitor_context ctx (*this->ctx_);
      be_visitor_valuetype_field_ch visitor (&ctx);

      visitor.setenclosings ("virtual ",";");

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_obv_ch::"
                            "visit_field - codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

void
be_visitor_valuetype_obv_ch::begin_public (void)
{
  AST_Decl::NodeType nt = this->ctx_->node ()->node_type ();

  // These types are skipped in the OBV class.
  if (nt == AST_Decl::NT_attr || nt == AST_Decl::NT_op)
    {
      return;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  *os << be_uidt_nl << be_nl
      << "public:" << be_idt;
}

void
be_visitor_valuetype_obv_ch::begin_private (void)
{
  TAO_OutStream *os = this->ctx_->stream ();
  *os << be_uidt_nl << be_nl
      << "protected:" << be_idt;
}
