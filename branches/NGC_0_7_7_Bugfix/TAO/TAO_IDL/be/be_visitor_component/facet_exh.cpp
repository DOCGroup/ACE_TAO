
//=============================================================================
/**
 *  @file    facet_exh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for facets in the exec impl header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


be_visitor_facet_exh::be_visitor_facet_exh (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
  // This is initialized in the base class to svnt_export_macro()
  // or skel_export_macro(), since there are many more visitor
  // classes generating servant code. So we can just override
  // all that here.
  export_macro_ = be_global->exec_export_macro ();
}

be_visitor_facet_exh::~be_visitor_facet_exh (void)
{
}

int
be_visitor_facet_exh::visit_provides (be_provides *node)
{
  be_type *impl = node->provides_type ();
  
  if (impl->exec_hdr_facet_gen ())
    {
      return 0;
    }
    
  // We don't want a '_cxx_' prefix here.
  const char *lname =
    impl->original_local_name ()->get_string ();
  
  AST_Decl *s = ScopeAsDecl (impl->defined_in ());
  ACE_CString sname_str =
    IdentifierHelper::orig_sn (s->name (), false);
  const char *sname = sname_str.c_str ();
  const char *global = (sname_str == "" ? "" : "::");
  
  os_ << be_nl
      << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__
      << be_nl;

  os_ << be_nl
      << "class " << export_macro_.c_str () << " "
      << lname << "_exec_i" << be_idt_nl
      << ": public virtual " << global << sname << "::CCM_"
      << lname << "," << be_idt_nl
      << "public virtual ::CORBA::LocalObject"
      << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << lname << "_exec_i (void);" << be_nl
      << "virtual ~" << lname << "_exec_i (void);";
   
  if (impl->node_type () == AST_Decl::NT_interface)
    {
      be_interface *intf =
        be_interface::narrow_from_decl (impl);
            
      os_ << be_nl << be_nl
          << "// Operations and attributes from ::"
          << intf->full_name ();
         
      int status =
        intf->traverse_inheritance_graph (
          be_interface::op_attr_decl_helper,
          &os_);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_facet_exh::")
                             ACE_TEXT ("visit_provides - ")
                             ACE_TEXT ("traverse_inheritance_graph() ")
                             ACE_TEXT ("failed\n")),
                            -1);
        }
    }
    
  os_ << be_uidt_nl
      << "};" << be_nl;
      
  impl->exec_hdr_facet_gen (true);

  return 0;
}

