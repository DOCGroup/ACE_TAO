//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_init_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Valuetypes factory in the client header
//    (see IDL to C++ mapping)
//
// = AUTHOR
//   Boris Kolpackov <bosk@ipmce.ru>
//
//
// ============================================================================

ACE_RCSID (be_visitor_valuetype, 
           valuetype_init_cs, 
           "$Id$")

be_visitor_valuetype_init_cs::be_visitor_valuetype_init_cs (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype_init (ctx)
{
}

be_visitor_valuetype_init_cs::~be_visitor_valuetype_init_cs (void)
{
}

int
be_visitor_valuetype_init_cs::visit_valuetype (be_valuetype *node)
{
  if (node->is_abstract ())
    {
      return 0;
    }

  // There are three possible situations.
  // (1) If there is no initializers but at least one operation.
  //     In this case we don't need to bother about factory.
  //
  // (2) There are no (operations or initializers) (i.e. only state
  //     members) then we need a concrete type-specific factory
  //     class whose create_for_unmarshal creates OBV_ class.
  //
  // (3) There is at least one operation and at least one initializer.
  //     In this case we need to generate abstract factory class.

  FactoryStyle factory_style = determine_factory_style (node);

  if(factory_style == FS_NO_FACTORY) // nothing to do
    {
      return 0; // bail out
    }


  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();

  os->indent (); // start with whatever indentation level we are at

  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _out names

  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (fname, "%s_init", node->full_name ());

  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (lname, "%s_init", node->local_name ());

  // ctor
  *os << fname << "::" << lname << " ()" << be_nl
      << "{" << be_nl << "}\n";

  // dtor
  *os << be_nl
      << fname << "::~" << lname << " ()" << be_nl
      << "{" << be_nl << "}\n";

  //tao_repository_id
  *os << be_nl
      << "const char* " << be_nl
      << fname << "::tao_repository_id (void)" << be_nl
      << "{" << be_idt_nl
      <<   "return " << node->full_name ()
      <<                "::_tao_obv_static_repository_id ();"
      << be_uidt_nl << "}\n";


  if (factory_style == FS_CONCRETE_FACTORY)
    {
      // generate create_for_unmarshal()
      *os << be_nl
          << "CORBA_ValueBase* " << be_nl
          << fname << "::create_for_unmarshal" << " "
          << "(void)" << be_nl
          << "{" << be_idt_nl
          << "CORBA_ValueBase* ret_val = 0;" << be_nl
          << "ACE_NEW_RETURN(ret_val, " << be_nl
          << "               OBV_" << node->full_name () << ", " << be_nl
          << "               0);" << be_nl
          << "return ret_val;"
          << be_uidt_nl << "}\n";
    }

  *os << be_nl;

  return 0;
}

int
be_visitor_valuetype_init_cs::visit_factory (be_factory *)
{
  return 0;
}
