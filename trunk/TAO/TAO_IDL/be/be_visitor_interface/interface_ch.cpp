//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, interface_ch, "$Id$")


// ******************************************************
// Interface visitor for client header
// ******************************************************

be_visitor_interface_ch::be_visitor_interface_ch (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ch::~be_visitor_interface_ch (void)
{
}

int
be_visitor_interface_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream
  long i;            // loop index

  if (!node->cli_hdr_gen () && !node->imported ()) // not already generated and
                                                   // not imported
    {

      os = this->ctx_->stream ();

      // == STEP 1:  generate the class name and class names we inherit ==

      // generate the ifdefined macro for  the _ptr type
      os->gen_ifdef_macro (node->flatname (), "_ptr");


      // the following two are required to be under the ifdef macro to avoid
      // multiple declarations

      os->indent (); // start with whatever indentation level we are at
      // forward declaration
      *os << "class " << node->local_name () << ";" << be_nl;
      // generate the _ptr declaration
      *os << "typedef " << node->local_name () << " *" << node->local_name ()
          << "_ptr;" << be_nl;

      os->gen_endif ();

      // generate the ifdefined macro for the var type
      os->gen_ifdef_macro (node->flatname (), "_var");

      // generate the _var declaration
      if (node->gen_var_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ch::"
                             "visit_interface - "
                             "codegen for _var failed\n"), -1);
        }
      os->gen_endif ();

      // generate the ifdef macro for the _out class
      os->gen_ifdef_macro (node->flatname (), "_out");

      // generate the _out declaration - ORBOS/97-05-15 pg 16-20 spec
      if (node->gen_out_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ch::"
                             "visit_interface - "
                             "codegen for _out failed\n"), -1);
        }
      // generate the endif macro
      os->gen_endif ();

      // now the interface definition itself
      os->gen_ifdef_macro (node->flatname ());

      // now generate the class definition
      os->indent ();
      *os << "class " << idl_global->export_macro ()
	        << " " << node->local_name ();

      if (node->n_inherits () > 0)  // node interface inherits from other
                                    // interfaces
        {
          *os << ": ";
          for (i = 0; i < node->n_inherits (); i++)
            {
              be_interface *inherited =
                be_interface::narrow_from_decl (node->inherits ()[i]);
              be_decl *scope = 0;
              if (inherited->is_nested ())
                {
                  // inherited node is used in the scope of "node" node
                  scope = be_scope::narrow_from_scope (node->defined_in ())
                    ->decl ();
                }

              *os << "public virtual ";
              *os << inherited->nested_type_name (scope);  // dump the scoped name
              if (i < node->n_inherits () - 1) // node is the case of multiple
                                               // inheritance, so put a comma
                {
                  *os << ", ";
                }
            }  // end of for loop
          *os << be_nl;
        }
      else
        {
          // we do not inherit from anybody, hence we do so from the base
          // CORBA::Object class
          // Generate code that uses the macro. This is required to deal with
          // the MSVC++ insanity
          *os << " : public virtual ACE_CORBA_1 (Object)" << be_nl;
        }

      // generate the body

      *os << "{" << be_nl
          << "public:" << be_nl

          // generate the _ptr_type and _var_type typedefs
          // but we must protect against certain versions of g++
          << "#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8"
          << be_idt_nl
          << "typedef " << node->local_name () << "_ptr _ptr_type;" << be_nl
          << "typedef " << node->local_name () << "_var _var_type;" << be_uidt_nl
          << "#endif /* __GNUC__ */\n" << be_idt_nl

          // generate the static _duplicate, _narrow, and _nil operations
          << "// the static operations" << be_nl
          << "static " << node->local_name () << "_ptr " << "_duplicate ("
          << node->local_name () << "_ptr obj);" << be_nl
          << "static " << node->local_name () << "_ptr "
	        << "_narrow (" << be_idt << be_idt_nl
          << "CORBA::Object_ptr obj," << be_nl
	        << "CORBA::Environment &env = " << be_idt_nl
	        << "CORBA::default_environment ()"
	        << be_uidt << be_uidt_nl
	        << ");" << be_uidt_nl
          << "static " << node->local_name () << "_ptr "
	        << "_unchecked_narrow (" << be_idt << be_idt_nl
          << "CORBA::Object_ptr obj," << be_nl
	        << "CORBA::Environment &env = " << be_idt_nl
	        << "CORBA::default_environment ()"
	        << be_uidt << be_uidt_nl
	        << ");" << be_uidt_nl
          << "static " << node->local_name () << "_ptr " << "_nil (void);\n\n";

      // generate code for the interface definition by traversing thru the
      // elements of its scope. We depend on the front-end to have made sure
      // that only legal syntactic elements appear in our scope.
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ch::"
                             "visit_interface - "
                             "codegen for scope failed\n"), -1);
        }
      // the _is_a method
      os->indent ();
      *os << "virtual CORBA::Boolean _is_a (" << be_idt << be_idt_nl
          << "const CORBA::Char *type_id, " << be_nl
          << "CORBA::Environment &env = " << be_idt_nl
          << "CORBA::default_environment ()"
          << be_uidt << be_uidt_nl
          << ");" << be_uidt_nl
          << "virtual const char* "
          << "_interface_repository_id (void) const;\n" << be_uidt_nl;

      // generate the "protected" constructor so that users cannot instantiate
      // us
      *os << "protected:" << be_idt_nl
          << node->local_name () << " (void);" << be_nl
          << node->local_name ()
	        << " (TAO_Stub *objref, " << be_idt << be_idt_nl
          << "TAO_ServantBase *_tao_servant = 0, " << be_nl
          << "CORBA::Boolean _tao_collocated = 0" << be_uidt_nl
	        << ");" << be_uidt_nl
          << "virtual ~" << node->local_name () << " (void);" << be_uidt_nl;

      // private copy constructor and assignment operator. These are not
      // allowed, hence they are private.
      *os << "private:\n";
      os->incr_indent ();
      *os << node->local_name () << " (const " << node->local_name () << " &);"
          << be_nl
          << "void operator= (const " << node->local_name () << " &);\n";

      os->decr_indent ();
      *os << "};\n\n";
      os->gen_endif ();

      // by using a visitor to declare and define the TypeCode, we have the
      // added advantage to conditionally not generate any code. This will be
      // based on the command line options. This is still TO-DO
      be_visitor *visitor;
      be_visitor_context ctx (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_TYPECODE_DECL);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ch::"
                             "visit_interface - "
                             "TypeCode declaration failed\n"
                             ), -1);
        }


      node->cli_hdr_gen (I_TRUE);
    } // if !cli_hdr_gen
  return 0;
}
