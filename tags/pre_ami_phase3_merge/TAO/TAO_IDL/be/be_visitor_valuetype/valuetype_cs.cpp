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
    return 0;

  os = this->ctx_->stream ();

  os->indent (); // start with whatever indentation level we are at

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
              <<   "rval = " << inherited->name()
              <<                "::_tao_obv_narrow (type_id);" << be_uidt_nl;
        }
    }

  *os <<    "return rval;" << be_uidt_nl
      <<  "}\n\n";

  // Nothing to marshal if abstract valuetype.
  if (!node->is_abstract_valuetype ())
    {
      // The virtual _tao_marshal_v method
      *os << "CORBA::Boolean " << node->name()
          <<                   "::_tao_marshal_v (TAO_OutputCDR & strm)"
          << be_nl
          <<  "{" << be_idt_nl
          <<   "return ";
      if (node->opt_accessor ())
        {
          *os << node->name ()
              <<"::_tao_marshal_state (strm);" << be_uidt_nl;
        }
      else
        {
          *os << "this->_tao_marshal__" << node->flatname ()
              << " (strm);" << be_uidt_nl;
        }
      *os << "}\n" << be_nl;

      // The virtual _tao_unmarshal_v method
      *os << "CORBA::Boolean " << node->name()
          <<                   "::_tao_unmarshal_v (TAO_InputCDR & strm)"
          << be_nl
          <<  "{" << be_idt_nl
          <<   "return ";
      if (node->opt_accessor ())
        {
          *os << node->name ()
              <<"::_tao_unmarshal_state (strm);" << be_uidt_nl;
        }
      else
        {
          *os << "this->_tao_unmarshal__" << node->flatname ()
              << " (strm);" << be_uidt_nl;
        }
      *os << "}\n" << be_nl;
    }  // !node->is_abstract_valuetype ()

  // The static T::_tao_unmarshal method  ----------------------------

  *os << "CORBA::Boolean " << node->name()
      <<                   "::_tao_unmarshal (TAO_InputCDR &strm, "
      <<                   node->name() <<" *&new_object)" << be_nl
      <<  "{" << be_idt_nl
      <<    "CORBA::Boolean retval = 1;" << be_nl
      <<    "CORBA::ValueBase *base;   // %! should be a _var"
      <<       be_nl
      <<    "CORBA::ValueFactory_ptr factory;   // %! should be a _var"
      <<       be_nl

      <<    "if (!CORBA::ValueBase::_tao_unmarshal_pre (strm, factory, base," << be_idt_nl
      <<       "      " << node->name ()
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
      <<    "new_object = " << node->name () << "::_downcast (base);" << be_nl
      <<    "// %! unmarshal_post" << be_nl
      <<    "return 1;" << be_uidt_nl
      <<  "}\n" << be_nl;

  // The static T::_tao_unmarshal method  ------------------------ end

  // generate the ifdefined macro for  the _init type
  os->gen_ifdef_macro (node->flatname (), "_init");
  if (this->gen_init_impl (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cs::"
                         "visit_valuetype - "
                         "codegen for _init failed\n"), -1);
    }
  os->gen_endif ();

  // generate code for the elements of the valuetype
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cs::"
                         "visit_valuetype - "
                         "codegen for scope failed\n"), -1);
    }


  // by using a visitor to declare and define the TypeCode, we have the
  // added advantage to conditionally not generate any code. This will be
  // based on the command line options. This is still TO-DO
     // (see interface code how to do this. not yet impl.)

  return 0;
}
