//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_fwd_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interface_Fwd node in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface_fwd.h"

ACE_RCSID(be_visitor_interface_fwd, interface_fwd_ch, "$Id$")


// ********************************************************************
// Visitor implementation for the Interface_Fwd type
// This one for the client header file
// ********************************************************************

be_visitor_interface_fwd_ch::be_visitor_interface_fwd_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_interface_fwd_ch::~be_visitor_interface_fwd_ch (void)
{
}

// visit the Interface_Fwd_Ch node and its scope
int
be_visitor_interface_fwd_ch::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (!node->cli_hdr_gen () && !node->imported ())
    {
      os->indent (); // start from the current

      // all we do in this is generate a forward declaration of the class
      *os << "class " << node->local_name () << ";" << be_nl;

      // generate the ifdefined macro for the _ptr type
      os->gen_ifdef_macro (node->flat_name (), "_ptr");

      // generate the _ptr declaration
      *os << "typedef " << node->local_name () << " *" << node->local_name ()
          << "_ptr;\n";

      os->gen_endif ();

      // enclose under an ifdef macro
      os->gen_ifdef_macro (node->flat_name (), "_var");

      // generate the _var declaration
      if (node->gen_var_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_fwd_ch::"
                             "visit_interface_fwd - "
                             "codegen for _var failed\n"), -1);
        }
      // gen an endif
      os->gen_endif ();

      // enclose under an ifdef macro
      os->gen_ifdef_macro (node->flat_name (), "_out");

      // generate the _out declaration - ORBOS/97-05-15 pg 16-20 spec
      if (node->gen_out_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_fwd_ch::"
                             "visit_interface_fwd - "
                             "codegen for _out failed\n"), -1);
        }
      // generate the endif macro
      os->gen_endif ();

      // now the interface definition itself
      os->gen_ifdef_macro (node->flat_name ());

      // now generate the class definition
      *os << "// Declarations necessary for use of forward declared" << be_nl;
      *os << "// interface as a member or parameter." << be_nl;
      *os << "class " << idl_global->stub_export_macro ()
                << " " << node->local_name ();

      // We are here only if we are a forward declared interface that is
      // not defined later in the same file. We do not need to worry here
      // about what interface(s) we may actually inherit from, if any.
      *os << " : public virtual ACE_CORBA_1 (Object)" << be_nl;

      // generate the body

      *os << "{" << be_nl
          << "public:" << be_idt_nl

          // generate the static _duplicate, _narrow, and _nil operations
          << "// the static operations" << be_nl
          << "static " << node->local_name () << "_ptr " << "_duplicate ("
          << node->local_name () << "_ptr obj);" << be_nl
          << "static " << node->local_name () << "_ptr "
          << "_narrow (" << be_idt << be_idt_nl
          << "CORBA::Object_ptr obj," << be_nl
          << "CORBA::Environment &env = " << be_idt_nl
          << "TAO_default_environment ()"
          << be_uidt << be_uidt_nl
          << ");" << be_uidt_nl;

      // This method is defined in the header file to workaround old
      // g++ problems
      *os << "static " << node->local_name () << "_ptr _nil (void)"
          << be_idt_nl << "{" << be_idt_nl
          << "return (" << node->name () << "_ptr)0;" << be_uidt_nl
          << "}" << be_uidt;// << "\n\n";

      *os << be_uidt <<be_uidt_nl;
      *os << "};\n\n";
      
      os->gen_endif ();
      
      node->cli_hdr_gen (I_TRUE);
    }
  return 0;
}
