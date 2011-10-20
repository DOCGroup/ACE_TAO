
//=============================================================================
/**
 *  @file    valuetype_sh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for value types in the server header
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


// ************************************************************
// Valuetype visitor for server header.
// ************************************************************

be_visitor_valuetype_sh::be_visitor_valuetype_sh (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_sh::~be_visitor_valuetype_sh (void)
{
}

int
be_visitor_valuetype_sh::visit_valuetype (be_valuetype *node)
{
  if (node->srv_hdr_gen () || node->imported () || node->is_abstract ())
    {
      return 0;
    }

  AST_Type *concrete = node->supports_concrete ();

  // We generate a skeleton class only if the valuetype supports a
  // non-abstract interface.
  if (concrete == 0)
    {
      return 0;
    }

  TAO_OutStream *os  = this->ctx_->stream ();

  os->indent ();
  ACE_CString class_name;

  // We shall have a POA_ prefix only if we are at the topmost level.
  if (!node->is_nested ())
    {
      // We are outermost.
      class_name += "POA_";
      class_name += node->local_name ();
    }
  else
    {
      class_name +=  node->local_name ();
    }

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Generate the skeleton class name.
  *os << "class " << class_name.c_str () << ";" << be_nl;

  // Generate the _ptr declaration.
  *os << "typedef " << class_name.c_str () << " *" << class_name.c_str ()
      << "_ptr;" << be_nl;

  // Forward class declaration.
  *os << "// Forward Classes Declaration" << be_nl;

  if (be_global->gen_direct_collocation ())
    {
      *os << "class " << node->direct_proxy_impl_name () << ";" << be_nl;
    }

  *os << be_nl;

  // Now generate the class definition.
  *os << "class " << be_global->skel_export_macro ()
      << " " << class_name.c_str () << be_idt_nl << ": " << be_idt;

  *os << "public virtual " << "POA_"
      << concrete->name () << ",";

        *os << be_nl << "public virtual " << "::" << node->full_name ();

  *os << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "protected:" << be_idt_nl
      << class_name.c_str () << " (void);" << be_uidt_nl << be_nl
      << "public:" << be_idt_nl;

  *os << "virtual ~" << class_name.c_str () << " (void);" << be_nl << be_uidt_nl;

  // No copy constructor for locality constraint interface.
  *os << "private:" << be_idt_nl
      << class_name.c_str () << " (const " << class_name.c_str ()
      << "& rhs);" << be_uidt_nl;


  *os << "};" << be_nl_2;

  return 0;
}

int
be_visitor_valuetype_sh::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}
