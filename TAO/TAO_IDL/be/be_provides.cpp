// $Id$

#include "be_provides.h"
#include "be_interface.h"
#include "be_visitor.h"
#include "be_helper.h"

#include "utl_identifier.h"

#include "nr_extern.h"

be_provides::be_provides (UTL_ScopedName *n,
                          AST_Type *provides_type)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_provides,
              n),
    AST_Field (AST_Decl::NT_provides,
               provides_type,
               n),
    AST_Provides (n,
                  provides_type),
    be_decl (AST_Decl::NT_provides,
             n),
    be_field (provides_type,
              n)
{
}

be_provides::~be_provides (void)
{
}

int
be_provides::gen_facet (TAO_OutStream &os)
{
   be_type *impl =
    be_type::narrow_from_decl (this->provides_type ());

  if (impl->svnt_hdr_facet_gen ())
    {
      return 0;
    }

  // No '_cxx_' prefix>
  const char *lname =
    impl->original_local_name ()->get_string ();

  be_decl *scope =
    be_scope::narrow_from_scope (impl->defined_in ())->decl ();
  ACE_CString suffix (scope->flat_name ());

  if (suffix != "")
    {
      suffix = ACE_CString ("_") + suffix;
    }

  os << be_nl << be_nl
     << "namespace CIAO_FACET" << suffix.c_str () << be_nl
     << "{" << be_idt_nl;

  const char *impl_name = "::CORBA::Object";
  bool is_intf = impl->node_type () == AST_Decl::NT_interface;

  if (is_intf)
    {
      impl_name =
        be_interface::narrow_from_decl (impl)->full_skel_name ();
    }

  os << "class " << lname << "_Servant" << be_idt_nl
     << ": public virtual " << impl_name << be_uidt_nl
     << "{" << be_nl
     << "public:" << be_idt_nl;

  AST_Decl *s = ScopeAsDecl (impl->defined_in ());
  ACE_CString sname_str (s->full_name ());
  const char *sname = sname_str.c_str ();
  const char *global = (sname_str == "" ? "" : "::");

  os << lname << "_Servant (" << be_idt_nl
     << global << sname << "::CCM_"
     << lname << "_ptr executor," << be_nl
     << "::Components::CCMContext_ptr ctx);" << be_uidt_nl << be_nl;

  os << "virtual ~" << lname << "_Servant (void);";

  if (is_intf)
    {
      be_interface *intf =
        be_interface::narrow_from_decl (impl);

      int status =
        intf->traverse_inheritance_graph (
          be_interface::facet_op_attr_decl_helper,
          &os);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_provides::gen_facet - ")
                             ACE_TEXT ("traverse_inheritance_graph() failed\n")),
                            -1);
        }
    }

  os << be_nl << be_nl << "// Get component implementation." << be_nl
     << "virtual CORBA::Object_ptr _get_component (void);"
     << be_uidt_nl << be_nl;

  os << "protected:" << be_idt_nl;

  os << "// Facet executor." << be_nl
     << global << sname << "::CCM_"
     << lname << "_var executor_;" << be_nl << be_nl;

  os << "// Context object." << be_nl
     << "::Components::CCMContext_var ctx_;" << be_uidt_nl;

  os << "};" << be_nl << be_uidt_nl;

  os << "}";

  impl->svnt_hdr_facet_gen (true);
  return 0;
}

int
be_provides::accept (be_visitor *visitor)
{
  return visitor->visit_provides (this);
}

void
be_provides::destroy (void)
{
  this->AST_Provides::destroy ();
  this->be_field::destroy ();
}

IMPL_NARROW_FROM_DECL (be_provides)
