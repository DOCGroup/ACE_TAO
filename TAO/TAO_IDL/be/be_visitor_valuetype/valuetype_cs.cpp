//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Valuetypes in the client stubs file.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    based on code from Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype.h"

ACE_RCSID(be_visitor_valuetype, valuetype_cs, "$Id$")


// ************************************************************
// Valuetype visitor for client stubs
// ************************************************************

be_visitor_valuetype_cs::be_visitor_valuetype_cs (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_cs::~be_visitor_valuetype_cs (void)
{
}

int
be_visitor_valuetype_cs::visit_valuetype (be_valuetype *node)
{
  TAO_OutStream *os; // output stream

  if (node->cli_stub_gen () || node->imported ())
    {
      return 0;
    }

  os = this->ctx_->stream ();

  os->indent (); // start with whatever indentation level we are at

  // Generate methods for _var class
  if (node->gen_var_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cs::"
                         "visit_valuetype - "
                         "codegen for _var failed\n"), -1);
    }

  // Generate methods for _out class
  if (node->gen_out_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cs::"
                         "visit_valuetype - "
                         "codegen for _out failed\n"), -1);
    }


  // The _downcast method    // %! use ACE_xxx_cast here ?
  *os << node->name() << "* " << node->name()
      <<                 "::_downcast (CORBA::ValueBase* v)" << be_nl
      <<  "{" << be_idt_nl
      <<     "if (v == 0) return 0;" << be_nl
      <<     "return (" << node->local_name() << "* ) "
      <<          "v->_tao_obv_narrow ((ptr_arith_t) &_downcast);" << be_uidt_nl
      <<  "}\n" << be_nl

  // The _tao_obv_repository_id method
      << "const char* " << node->name()
      <<                   "::_tao_obv_repository_id () const" << be_nl
      <<  "{" << be_idt_nl
      <<     "return this->_tao_obv_static_repository_id ();" << be_uidt_nl
      <<  "}\n" << be_nl

  // The _tao_obv_narrow method
      << "void* " << node->name()
      << "::_tao_obv_narrow (ptr_arith_t type_id)" << be_nl
      <<  "{" << be_idt_nl
      <<    "if (type_id == (ptr_arith_t) &_downcast)" << be_idt_nl
      <<       "return this;" << be_uidt_nl
      <<    "void *rval = 0;" << be_nl;

  // Find the possible base classes.

  int n_inherits_downcastable = 0;

  for (int i = 0; i < node->n_inherits (); i++)
    {
      AST_Interface *inherited =
          AST_Interface::narrow_from_decl (node->inherits ()[i]);

      if (inherited->is_valuetype())
        {
          ++n_inherits_downcastable;
          *os << "if (rval == 0)" << be_idt_nl
              << "rval = ";

          if (inherited->defined_in ()->scope_node_type () == AST_Decl::NT_module)
            {
              be_decl *scope = be_scope::narrow_from_scope (inherited->defined_in ())->decl ();
              *os << "ACE_NESTED_CLASS ("
                  << scope->name() << ","
                  << inherited->local_name () << ")";
            }
          else
            {
              *os << inherited->name ();
            }

          *os << "::_tao_obv_narrow (type_id);" << be_uidt_nl;
        }
    }

  *os <<    "return rval;" << be_uidt_nl
      <<  "}\n\n";

  // Nothing to marshal if abstract valuetype.
  if (!node->is_abstract_valuetype ())
    {
      // The virtual _tao_marshal_v method
      *os << "CORBA::Boolean " << node->name ()
          <<                   "::_tao_marshal_v (TAO_OutputCDR & strm)"
          << be_nl
          <<  "{" << be_idt_nl
          <<   "return ";
      if (node->opt_accessor ())
        {
          be_decl *scope = be_scope::narrow_from_scope (node->defined_in ())->decl ();
          *os << "ACE_NESTED_CLASS ("
              << scope->name () << ","
              << node->local_name () << ")"
              <<"::_tao_marshal_state (strm);" << be_uidt_nl;
        }
      else
        {
          *os << "this->_tao_marshal__" << node->flat_name ()
              << " (strm);" << be_uidt_nl;
        }

      *os << "}\n" << be_nl;

      // The virtual _tao_unmarshal_v method
      *os << "CORBA::Boolean " << node->name ()
          <<                   "::_tao_unmarshal_v (TAO_InputCDR & strm)"
          << be_nl
          <<  "{" << be_idt_nl
          <<   "return ";
      if (node->opt_accessor ())
        {
          be_decl *scope = be_scope::narrow_from_scope (node->defined_in ())->decl ();
          *os << "ACE_NESTED_CLASS ("
              << scope->name () << ","
              << node->local_name () << ")"
              <<"::_tao_unmarshal_state (strm);" << be_uidt_nl;
        }
      else
        {
          *os << "this->_tao_unmarshal__" << node->flat_name ()
              << " (strm);" << be_uidt_nl;
        }

      *os << "}\n" << be_nl;
    }

  // The static T::_tao_unmarshal method  ----------------------------

  *os << "CORBA::Boolean " << node->name()
      <<                   "::_tao_unmarshal (TAO_InputCDR &strm, "
      <<                   node->local_name () <<" *&new_object)" << be_nl
      <<  "{" << be_idt_nl
      <<    "CORBA::Boolean retval = 1;" << be_nl
      <<    "CORBA::ValueBase *base;   // %! should be a _var"
      <<       be_nl
      <<    "CORBA::ValueFactory_ptr factory;   // %! should be a _var"
      <<       be_nl

      <<    "if (!CORBA::ValueBase::_tao_unmarshal_pre (strm, factory, base," << be_idt_nl
      <<       "      " << node->local_name ()
      << "::_tao_obv_static_repository_id ()) )" << be_nl
      <<       "{" << be_idt_nl
      <<          "return 0;" << be_uidt_nl
      <<       "}" << be_uidt_nl
      <<    "if (factory != 0)" << be_idt_nl
      <<      "{" << be_idt_nl

      <<        "base = factory->create_for_unmarshal ();" << be_nl
      <<        "factory->_remove_ref ();" << be_nl
      <<        "if (base == 0)  return 0;  // %! except.?" << be_nl
      <<        "//%! ACE_DEBUG ((LM_DEBUG, \"" << node->name()
      <<            "::_tao_unmarshal %s\\n\", "
      <<            "base->_tao_obv_repository_id () ));" << be_nl
      <<        "retval = base->_tao_unmarshal_v (strm);" << be_nl
      <<        "//%! ACE_DEBUG ((LM_DEBUG, \"" << node->name()
      <<            "::_tao_unmarshal retval unmarshal_v is %d\\n\", "
      <<            "retval));" << be_nl
      <<         "if (!retval) return 0;"

      <<       be_uidt_nl << "}" << be_uidt_nl
      <<    "// Now base must be null or point to the unmarshaled object."
      <<      be_nl
      <<    "// Align the pointer to the right subobject." << be_nl
      <<    "new_object = " << node->local_name () << "::_downcast (base);" << be_nl
      <<    "// %! unmarshal_post" << be_nl
      <<    "return 1;" << be_uidt_nl
      <<  "}\n" << be_nl;

  // The static T::_tao_unmarshal method  ------------------------ end

  // generate code for the elements of the valuetype
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cs::"
                         "visit_valuetype - "
                         "codegen for scope failed\n"), -1);
    }

  // Generate the _init -related code.
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_VALUETYPE_INIT_CS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ch::"
                         "visit_valuetype - "
                         "NULL visitor.\n"
                         ),  -1);
    }

  if (visitor->visit_valuetype(node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ch::"
                         "visit_valuetype - "
                         "failed to generate _init construct.\n"
                         ),  -1);
    }

  delete visitor;


  // by using a visitor to declare and define the TypeCode, we have the
  // added advantage to conditionally not generate any code. This will be
  // based on the command line options. This is still TO-DO
  // (see interface code how to do this. not yet impl.)

  return 0;
}
