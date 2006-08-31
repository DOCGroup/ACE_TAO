// $Id$

#include "File_Generator_Base.h"
#include "Uml.h"
#include "CCF/CodeGenerationKit/IndentationCxx.hpp"
#include "CCF/CodeGenerationKit/IndentationImplanter.hpp"

//
// CUTS_CIAO_File_Generator_Base
//
CUTS_CIAO_File_Generator_Base::
CUTS_CIAO_File_Generator_Base (void)
: formatter_ (0)
{

}

//
// ~CUTS_CIAO_File_Generator_Base
//
CUTS_CIAO_File_Generator_Base::
~CUTS_CIAO_File_Generator_Base (void)
{

}

//
// write_single_line_comment
//
void CUTS_CIAO_File_Generator_Base::
write_single_line_comment (const std::string & comment)
{
  this->out_ << "// " << comment.c_str () << std::endl;
}

//
// close
//
void CUTS_CIAO_File_Generator_Base::close (void)
{
  this->formatter_.reset ();
  this->out_.close ();
}

//
// open_i
//
void CUTS_CIAO_File_Generator_Base::open_i (const std::string & filename)
{
  if (this->out_.is_open ())
    this->out_.close ();

  this->out_.open (filename.c_str ());
  this->formatter_.reset (new Formatter_Type (this->out_));
}

//
// write_monolithic_begin
//
void CUTS_CIAO_File_Generator_Base::
write_monolithic_begin (const PICML::MonolithicImplementation & mono)
{
  this->out_
    << "namespace CIDL_" << mono.name () << "{";
}

//
// write_monolithic_end
//
void CUTS_CIAO_File_Generator_Base::write_monolithic_end (void)
{
  this->out_ << "}";
}

//
// write_component_end
//
void CUTS_CIAO_File_Generator_Base::
write_component_end (const PICML::Component & component)
{
  this->write_single_line_comment ("SessionComponent: set_session_context");
  this->write_set_session_context (component);

  this->write_single_line_comment ("SessionComponent: ciao_preactivate");
  this->write_ciao_preactivate (component);

  this->write_single_line_comment ("SessionComponent: ccm_activate");
  this->write_ccm_activate (component);

  this->write_single_line_comment ("SessionComponent: ciao_postactivate");
  this->write_ciao_postactivate (component);

  this->write_single_line_comment ("SessionComponent: ccm_passivate");
  this->write_ccm_passivate (component);

  this->write_single_line_comment ("SessionComponent: ccm_remove");
  this->write_ccm_remove (component);
}

//
// write_include_file
//
void CUTS_CIAO_File_Generator_Base::
write_include_file (const std::string & str)
{
  this->out_
    << "#include \"" << str << ".h\"" << std::endl;
}

//
// write_method_begin
//
void CUTS_CIAO_File_Generator_Base::
write_method_begin (const PICML::InEventPort & sink)
{
  this->out_
    << "push_" << sink.name () << " (" << std::endl;

  PICML::Event event = sink.ref ();
  this->write_scope (PICML::NamedType::Cast (event), "::");

  this->out_
    << event.name ()
    << " * ev" << std::endl
    << "ACE_ENV_ARG_DECL_WITH_DEFAULTS)" << std::endl
    << "  ACE_THROW_SPEC ((::CORBA::SystemException))";
}

//
// write_method_decl
//
void CUTS_CIAO_File_Generator_Base::
write_method_begin (const PICML::ProvidedRequestPort & facet)
{
  this->out_
    << "get_" << facet.name () << " (" << std::endl
    << "(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)" << std::endl
    << "  ACE_THROW_SPEC ((::CORBA::SystemException))";
}

//
// write_scope
//
void CUTS_CIAO_File_Generator_Base::
write_scope (const PICML::NamedType & named_type,
             const std::string & seperator,
             std::string * out)
{
  // Keep getting the parent of the object until we reach a
  // PICML::File object.
  std::string name;
  PICML::MgaObject parent = named_type.parent ();

  while ((std::string) parent.type ().name () !=
         (std::string) PICML::File::meta.name ())
  {
    name.insert (0, seperator);
    name.insert (0, parent.name ());

    parent = PICML::MgaObject::Cast (parent.parent ());
  }

  if (out != 0)
    (*out) = name;

  this->out_ << name;
}

//
// write_set_session_context
//
void CUTS_CIAO_File_Generator_Base::
write_set_session_context (const PICML::Component & component)
{
  this->out_
    << "set_session_context (" << std::endl
    << "::Components::SessionContext_ptr ctx" << std::endl
    << "ACE_ENV_ARG_DECL_WITH_DEFAULTS)" << std::endl
    << "  ACE_THROW_SPEC ((::CORBA::SystemException," << std::endl
    << "::Components::CCMException))";
}

//
// write_ciao_preactivate
//
void CUTS_CIAO_File_Generator_Base::
write_ciao_preactivate (const PICML::Component & component)
{
  this->out_
    << "ciao_preactivate (" << std::endl
    << "ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)" << std::endl
    << "  ACE_THROW_SPEC ((::CORBA::SystemException," << std::endl
    << "::Components::CCMException))";
}

//
// write_ccm_activate
//
void CUTS_CIAO_File_Generator_Base::
write_ccm_activate (const PICML::Component & component)
{
  this->out_
    << "ccm_activate (" << std::endl
    << "ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)" << std::endl
    << "  ACE_THROW_SPEC ((::CORBA::SystemException," << std::endl
    << "::Components::CCMException))";
}

//
// write_ciao_postactivate
//
void CUTS_CIAO_File_Generator_Base::
write_ciao_postactivate (const PICML::Component & component)
{
  this->out_
    << "ciao_postactivate (" << std::endl
    << "ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)" << std::endl
    << "  ACE_THROW_SPEC ((::CORBA::SystemException," << std::endl
    << "::Components::CCMException))";
}

void CUTS_CIAO_File_Generator_Base::
write_ccm_passivate (const PICML::Component & component)
{
  this->out_
    << "ccm_passivate (" << std::endl
    << "ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)" << std::endl
    << "  ACE_THROW_SPEC ((::CORBA::SystemException," << std::endl
    << "::Components::CCMException))";
}

void CUTS_CIAO_File_Generator_Base::
write_ccm_remove (const PICML::Component & component)
{
  this->out_
    << "ccm_remove (" << std::endl
    << "ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)" << std::endl
    << "  ACE_THROW_SPEC ((::CORBA::SystemException," << std::endl
    << "::Components::CCMException))";
}
