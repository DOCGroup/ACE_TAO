
//=============================================================================
/**
 *  @file    facet_ex_idl.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for facets in the CIAO executor IDL.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_facet_ex_idl::be_visitor_facet_ex_idl (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_facet_ex_idl::~be_visitor_facet_ex_idl (void)
{
}

int
be_visitor_facet_ex_idl::visit_provides (be_provides *node)
{
  be_type *impl = node->provides_type ();

  if (impl->ex_idl_facet_gen ())
    {
      return 0;
    }

  be_util::gen_nesting_open (os_, impl);

  os_ << be_nl
      << "local interface CCM_"
      << impl->original_local_name ()->get_string ()
      << " : ::"
      << IdentifierHelper::orig_sn (impl->name ()).c_str ()
      << be_nl
      << "{" << be_idt;

  os_ << be_uidt_nl
      << "};";

  be_util::gen_nesting_close (os_, impl);

  impl->ex_idl_facet_gen (true);

  return 0;
}


