
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_obv__ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Valuetypes in the client header
//    OBV_ class
//    (see C++ mapping OMG 20.17)
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>,
//    based on interface_ch.cpp from Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_valuetype, 
           valuetype_obv_ch, 
           "$Id$")

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

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ ;

  // OBV_ class maps only to a typedef if we are optimizing accessors.
  if (node->opt_accessor ())
    {
      *os << be_nl << be_nl << "typedef " << node->full_name () << " ";

      if (!node->is_nested ())
        {
          *os << "OBV_";
        }

      *os << node->local_name () << ";" << be_nl;
    }
  else
    {
      // STEP 1: Generate the class name and the class name we inherit.
      os->gen_ifdef_macro (node->flat_name (), "_OBV");

      *os << be_nl << be_nl << "// OBV_ class" << be_nl;
      *os << "class " << be_global->stub_export_macro() << " ";;

      if (!node->is_nested())
        {
          *os << "OBV_";
        }

      *os << node->local_name () << be_idt_nl
          << ": public virtual "
          << node->full_name ();

      // STEP 1a (about which previous implementer forgot ):
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
      AST_Interface *inherited = 0;

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
          *os << "  public virtual CORBA::DefaultValueRefCountBase";
        }

      *os << be_uidt_nl;

//------>>>

      // STEP 2: Generate the body ==

      *os << "{" << be_nl;

      this->begin_public ();

      *os << be_nl << "virtual ~";
      
      if (! node->is_nested ())
        {
          *os << "OBV_";
        }
        
      *os << node->local_name () << " (void);";

      // Generate code for the OBV_ class definition.
      if (this->visit_valuetype_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_obv_ch::"
                             "visit_valuetype - "
                             "codegen for scope failed\n"),
                            -1);
        }

      // If we inherit from both CORBA::ValueBase and CORBA::AbstractBase,
      // we have to add this to avoid ambiguity.
      if (node->supports_abstract ())
        {
          *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
              << "// " << __FILE__ << ":" << __LINE__ ;

          *os << be_nl << be_nl << "virtual void _add_ref (void);" << be_nl;
          *os << "virtual void _remove_ref (void);";
        }

      // Map fields to private data.
      if (!node->opt_accessor ())
        {
          *os << be_nl << be_uidt_nl << "protected:" << be_idt_nl;
          *os << "virtual CORBA::Boolean _tao_marshal__"
              <<    node->flat_name () << " (TAO_OutputCDR &);" << be_nl;
          *os << "virtual CORBA::Boolean _tao_unmarshal__"
              <<    node->flat_name () << " (TAO_InputCDR &);" << be_nl;
          *os << "CORBA::Boolean "
              << "_tao_marshal_state (TAO_OutputCDR &);" << be_nl
              << "CORBA::Boolean "
              << "_tao_unmarshal_state (TAO_InputCDR &);"
              << be_uidt_nl << be_nl;
          *os << "private:" << be_idt;

          this->gen_pd (node);
        }

      *os << be_uidt_nl;
      *os << "};";

      os->gen_endif ();
    }

  return 0;
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
  TAO_OutStream *os = this->ctx_->stream ();
  *os << "public:" << be_idt;
}

void
be_visitor_valuetype_obv_ch::begin_private (void)
{
  TAO_OutStream *os = this->ctx_->stream ();
  *os << be_uidt_nl << be_nl;
  *os << "protected:" << be_idt;
}
