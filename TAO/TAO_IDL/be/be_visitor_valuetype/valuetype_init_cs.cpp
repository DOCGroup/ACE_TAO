
//=============================================================================
/**
 *  @file    valuetype_init_cs.cpp
 *
 *  Visitor generating code for Valuetypes factory in the client header
 *  (see IDL to C++ mapping)
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================

#include "valuetype.h"

#include <string>

be_visitor_valuetype_init_cs::be_visitor_valuetype_init_cs (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype_init (ctx)
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

  be_valuetype::FactoryStyle factory_style =
    node->determine_factory_style ();

  if (factory_style == be_valuetype::FS_NO_FACTORY)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  std::string fname (node->full_name ());
  fname += "_init";
  std::string lname (node->local_name ());
  lname += "_init";

  TAO_INSERT_COMMENT (os);

  // ctor
  *os << be_nl_2
      << fname << "::" << lname << " ()" << be_nl
      << "{" << be_nl << "}";

  // dtor
  *os << be_nl_2
      << fname << "::~" << lname << " ()" << be_nl
      << "{" << be_nl << "}";

  // The _downcast method.
  *os << be_nl_2
      << node->name () << "_init *" << be_nl << node->name ()
      << "_init::_downcast (::CORBA::ValueFactoryBase *v)" << be_nl
      << "{" << be_idt_nl
      << "return dynamic_cast<::" << node->name ()
      << "_init *> (v);" << be_uidt_nl
      << "}";

  // tao_repository_id
  *os << be_nl_2
      << "char const *" << be_nl
      << fname << "::tao_repository_id ()" << be_nl
      << "{" << be_idt_nl
      <<   "return ::" << node->full_name ()
      <<                "::_tao_obv_static_repository_id ();"
      << be_uidt_nl << "}";


  if (factory_style == be_valuetype::FS_CONCRETE_FACTORY)
    {
      // generate create_for_unmarshal()
      *os << be_nl_2
          << "::CORBA::ValueBase *" << be_nl
          << fname << "::create_for_unmarshal ()" << be_nl
          << "{" << be_idt_nl
          << "::CORBA::ValueBase *ret_val = nullptr;" << be_nl
          << "ACE_NEW_THROW_EX (" << be_idt << be_idt_nl
          << "ret_val," << be_nl
          << "OBV_" << node->full_name () << "," << be_nl
          << "::CORBA::NO_MEMORY ());" << be_uidt
          << be_uidt_nl
          << "return ret_val;"
          << be_uidt_nl << "}";

        if (node->supports_abstract ())
          {
            *os << be_nl_2
                << "::CORBA::AbstractBase_ptr" << be_nl
                << fname << "::create_for_unmarshal_abstract ()" << be_nl
                << "{" << be_idt_nl
                << "::CORBA::AbstractBase *ret_val = nullptr;" << be_nl
                << "ACE_NEW_THROW_EX (" << be_idt << be_idt_nl
                << "ret_val," << be_nl
                << "OBV_" << node->full_name () << "," << be_nl
                << "::CORBA::NO_MEMORY ()" << be_uidt_nl
                << ");" << be_uidt_nl
                << "return ret_val;"
                << be_uidt_nl << "}";
          }
    }

  return 0;
}

int
be_visitor_valuetype_init_cs::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

