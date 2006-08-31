// $Id$

#include "Source_File_Generator.h"

#if !defined (__CUTS_INLINE__)
#include "Source_File_Generator.inl"
#endif

#include "boost/bind.hpp"
#include "Uml.h"
#include <algorithm>
#include <sstream>

//
// CUTS_CIAO_Source_File_Generator
//
CUTS_CIAO_Source_File_Generator::
CUTS_CIAO_Source_File_Generator (void)
{

}

//
// ~CUTS_CIAO_Source_File_Generator
//
CUTS_CIAO_Source_File_Generator::
~CUTS_CIAO_Source_File_Generator (void)
{

}

//
// open
//
void CUTS_CIAO_Source_File_Generator::
open (const std::string & pathname)
{
  CUTS_CIAO_Source_File_Generator g;

  std::ostringstream filename;
  filename << pathname << "_exec_i.cpp";

  this->open_i (filename.str ());
}

//
// write_preamble
//
void CUTS_CIAO_Source_File_Generator::
write_preamble (const PICML::ComponentImplementationContainer & container)
{
  // Generate the hash definition for this file.
  std::string hashdef = container.name ();
  this->to_upper (hashdef);

  this->out_
    << "// $Id$" << std::endl
    << std::endl;

  // Make sure we include the header file.
  std::string include = container.name ();
  include.append ("_exec_i");

  this->write_include_file (include);
  this->write_include_file ("cuts/ActivationRecord");
  this->out_ << std::endl;
}

//
// write_component_begin
//
void CUTS_CIAO_Source_File_Generator::
write_component_begin (const PICML::Component & component)
{
  std::string name = component.name ();

  typedef std::vector <PICML::InEventPort> InEventPort_Set;
  InEventPort_Set sinks = component.InEventPort_kind_children ();


  // @@ constructor
  this->write_single_line_comment ("Default constructor.");
  this->out_
    << name << "::" << name << " (void)"
    << "{";

  for (InEventPort_Set::const_iterator iter = sinks.begin ();
       iter != sinks.end ();
       iter ++)
  {
    std::string name = iter->name ();

    this->out_
      << "this->push_" << name << "_handler_.name ("
      << "\"" << name << "\");"
      << "this->push_" << name << "_handler_.bind ("
      << "this, " << "&" << component.name ()
      << "::push_" << name << "_handler" << ");";

    // Set any additional properties for the event handler.
    // Verify that this InEventPort has some behavior attached
    // to it, else there is no need to generate 'init' code for it.
    PICML::Input input = iter->dstInput ();
    PICML::InputAction ia = PICML::InputAction::Cast (input.dstInput_end ());

    typedef std::set <PICML::Property> Property_Set;
    Property_Set properties = ia.Property_children ();

    for (Property_Set::const_iterator iter = properties.begin ();
         iter != properties.end ();
         iter ++)
    {
      std::string p_name = iter->name ();
      std::string p_value = iter->DataValue ();

      if (p_value.empty ())
      {
        this->out_
          << "// property <" << p_name << "> has no value" <<std::endl;
      }
      else if (p_name == "mode")
      {
        // Set the mode of the event handler.
        this->out_
          << "this->push_" << name << "_handler_."
          << p_name << " (CUTS_Event_Handler::";

        if (p_value == "async")
          this->out_ << "ASYNCHRONOUS";
        else if (p_value == "sync")
          this->out_ << "SYNCHRONOUS";
        else
          this->out_ << "UNDEFINED";

        this->out_ << ");";
      }
      else if (p_name == "thread_count" ||
               p_name == "priority")
      {
        // Print out a direct mapping of the <p_name> and
        // it <p_value>.
        this->out_
          << "this->push_" << name << "_handler_."
          << p_name << " (" << p_value << ");";
      }
    }

    // Register the port agent with the benchmark agent.
    this->out_
      << "this->benchmark_agent_->register_port_agent ("
      << "&this->push_" << name << "_handler_.port_agent ());"
      << std::endl;
  }

  this->out_
    << "}";

  // @@ destructor
  this->write_single_line_comment ("Destructor.");
  this->out_
    << name << "::~" << name << " (void)"
    << "{";

  for (InEventPort_Set::const_iterator iter = sinks.begin ();
       iter != sinks.end ();
       iter ++)
  {
    this->out_
      << "this->benchmark_agent_->unregister_port_agent ("
      << "&this->push_" << iter->name () << "_handler_.port_agent ());";
  }

  this->out_
    << "}";
}

//
// write_method_begin
//
void CUTS_CIAO_Source_File_Generator::
write_method_begin (const PICML::InEventPort & sink)
{
  std::string comment ("EventSink: ");
  comment.append (sink.name ());
  this->write_single_line_comment (comment);

  PICML::MgaObject parent = sink.parent ();
  PICML::Event event = sink.ref ();

  this->out_ << "void " << parent.name () << "::";
  CUTS_CIAO_File_Generator_Base::write_method_begin (sink);
  this->out_
    << "{"
    << "this->push_" << sink.name () << "_handler_.handle_event (ev);"
    << "}";

  // Write the event handler method.
  this->out_
    << "void " << parent.name () << "::"
    << "push_" << sink.name () << "_handler (" << std::endl;

  std::string scoped_name;
  this->write_scope (PICML::NamedType::Cast (event), "::", &scoped_name);

  this->out_
    << event.name () << " * ev, " << std::endl
    << "CUTS_Activation_Record * record) {";

  scoped_name.append (event.name ());
  this->events_.insert (scoped_name);
  this->event_sinks_.insert (sink.name ());
}

//
// write_method_begin
//
void CUTS_CIAO_Source_File_Generator::
write_method_begin (const PICML::ProvidedRequestPort & facet)
{
  // Write an opening comment.
  std::string comment ("Facet: ");
  comment.append (facet.name ());
  this->write_single_line_comment (comment);

  PICML::MgaObject parent = facet.parent ();
  PICML::Object object = PICML::Object::Cast (facet.ref ());

  this->out_ << "void " << parent.name () << "::";
  CUTS_CIAO_File_Generator_Base::write_method_begin (facet);

  // Write the implementation for the method.
  this->out_
    << "{"
    << "return ";

  this->write_scope (PICML::NamedType::Cast (object), "::");

  this->out_
    << "CCM_" << object.name () << "::_nil ();";
}

//
// write_method_end
//
void CUTS_CIAO_Source_File_Generator::write_method_end (void)
{
  this->out_ << "}";
}

//
// write_set_session_context
//
void CUTS_CIAO_Source_File_Generator::
write_set_session_context (const PICML::Component & component)
{
  this->out_ << "void " << component.name () << "::";
  CUTS_CIAO_File_Generator_Base::write_set_session_context (component);
  this->out_
    << "{"
    << "CoWorkEr_Type::set_session_context (ctx);"
    << std::endl;

  std::for_each (this->events_.begin (),
                 this->events_.end (),
                 boost::bind (&CUTS_CIAO_Source_File_Generator::
                              write_register_obv_factory,
                              this,
                              _1));

  this->out_
    << "}";
}

//
// write_ciao_preactivate
//
void CUTS_CIAO_Source_File_Generator::
write_ciao_preactivate (const PICML::Component & component)
{
  this->out_ << "void " << component.name () << "::";
  CUTS_CIAO_File_Generator_Base::write_ciao_preactivate (component);
  this->out_
    << "{"
    << "CoWorkEr_Type::ciao_preactivate ();"
    << std::endl;

  // Set the parents for all the workers in this component.
  typedef std::vector <PICML::WorkerType> WorkerType_Set;
  WorkerType_Set workers = component.WorkerType_kind_children ();

  for (WorkerType_Set::const_iterator iter = workers.begin ();
       iter != workers.end ();
       iter ++)
  {
    this->out_
      << "this->" << iter->name () << "_.parent (this->registration_id_);";
  }

  this->out_ << std::endl;

  // Activate all the event handlers.
  for (CUTS_String_Set::const_iterator iter = this->event_sinks_.begin ();
       iter != this->event_sinks_.end ();
       iter ++)
  {
    this->out_
      << "this->push_" << *iter << "_handler_.activate ();";
  }

  this->out_ << "}";
}

//
// write_cmm_activate
//
void CUTS_CIAO_Source_File_Generator::
write_ccm_activate (const PICML::Component & component)
{
  this->out_ << "void " << component.name () << "::";
  CUTS_CIAO_File_Generator_Base::write_ccm_activate (component);
  this->out_
    << "{"
    << std::endl
    << "}";

  // @@ look for cuts_activate ()
}

//
// write_ciao_postactivate
//
void CUTS_CIAO_Source_File_Generator::
write_ciao_postactivate (const PICML::Component & component)
{
  this->out_ << "void " << component.name () << "::";
  CUTS_CIAO_File_Generator_Base::write_ciao_postactivate (component);
  this->out_
    << "{"
    << "CoWorkEr_Type::ciao_postactivate ();"
    << std::endl;

  // Generate the code to activate all of the periodic events
  // if there are any in this component.
  typedef std::set <PICML::PeriodicAction> Periodic_Set;
  Periodic_Set pset = component.PeriodicAction_kind_children ();

  for (Periodic_Set::iterator iter = pset.begin ();
       iter != pset.end ();
       iter ++)
  {
    this->out_
      << "this->periodic_" << iter->name () << "_.activate ("
      << iter->Period () << ");";
  }

  this->out_ << "}";
}

//
// write_ccm_passivate
//
void CUTS_CIAO_Source_File_Generator::
write_ccm_passivate (const PICML::Component & component)
{
  this->out_ << "void " << component.name () << "::";
  CUTS_CIAO_File_Generator_Base::write_ccm_passivate (component);
  this->out_
    << "{"
    << "CoWorkEr_Type::ccm_passivate ();"
    << std::endl;

  // Generate the code to deactivate all of the periodic events
  // if there are any in this component.
  typedef std::set <PICML::PeriodicAction> Periodic_Set;
  Periodic_Set pset = component.PeriodicAction_kind_children ();

  for (Periodic_Set::iterator iter = pset.begin ();
        iter != pset.end ();
        iter ++)
  {
    this->out_
      << "this->periodic_" << iter->name () << "_.deactivate ();";
  }

  this->out_ << std::endl;

  // Deactivate all the event handlers.
  for (CUTS_String_Set::const_iterator iter = this->event_sinks_.begin ();
       iter != this->event_sinks_.end ();
       iter ++)
  {
    this->out_
      << "this->push_" << *iter << "_handler_.deactivate ();";
  }

  this->out_ << "}";
}

//
// write_ccm_remove
//
void CUTS_CIAO_Source_File_Generator::
write_ccm_remove (const PICML::Component & component)
{
  this->out_ << "void " << component.name () << "::";
  CUTS_CIAO_File_Generator_Base::write_ccm_remove (component);
  this->out_
    << "{"
    << "CoWorkEr_Type::ccm_remove ();"
    << "}";
}

//
// write_variable
//
void CUTS_CIAO_Source_File_Generator::
write_variable (const PICML::WorkerType & worker_type)
{
  //PICML::Worker worker = worker_type.ref ();

  //std::string comment ("Worker: ");
  //comment.append (worker_type.name ());

  //this->write_single_line_comment (comment);
  //this->out_
  //  << worker.name ()
  //  << " " << worker_type.name () << "_;"
  //  << std::endl;
}

//
// write_variable
//
void CUTS_CIAO_Source_File_Generator::
write_variable (const PICML::Variable & variable)
{
  //PICML::PredefinedType ptype = variable.ref ();
  //std::string _typename = ptype.type ().name ();

  //std::string comment ("Variable: ");
  //comment.append (variable.name ());

  //this->write_single_line_comment (comment);
  //this->out_
  //  << CUTS_Predefined_Type_Map::instance ()->c_str (_typename.c_str ())
  //  << " "
  //  << variable.name ()
  //  << "_;"
  //  << std::endl;
}

//
// write_variable
//
void CUTS_CIAO_Source_File_Generator::
write_variable (const PICML::InEventPort & sink)
{
  //// Write the leading comment.
  //std::string comment ("Event handler: ");
  //comment.append (sink.name ());
  //this->write_single_line_comment (comment);

  //PICML::Event event = sink.ref ();
  //if (event == Udm::null)
  //  return;

  //// Write the emulated event handler method.
  //this->out_
  //  << "void push_" << sink.name () << "_handler (" << std::endl;

  //this->write_scope (PICML::NamedType::Cast (event));

  //this->out_
  //  << event.name () << " * ev, CUTS_Activation_Record * record);"
  //  << std::endl

  //  // Write the event handler variable.
  //  << "CUTS_Event_Handler_Manager_T <" << std::endl
  //  << "  This_Component, ";

  //this->write_scope (PICML::NamedType::Cast (event));

  //this->out_
  //  << event.name () << "> "
  //  << "push_" << sink.name () << "_handler_;"
  //  << std::endl;
}

//
// write_register_obv_factory
//
void CUTS_CIAO_Source_File_Generator::
write_register_obv_factory (const std::string & event)
{
  std::string comment ("Event: ");
  comment.append (event);
  this->write_single_line_comment (comment);

  this->out_
    << "CIAO_REGISTER_OBV_FACTORY ("
    << event << "_init," << std::endl
    << event << ");"
    << std::endl;
}

//
// write_component_end
//
void CUTS_CIAO_Source_File_Generator::
write_component_end (const PICML::Component & component)
{
  CUTS_CIAO_File_Generator_Base::write_component_end (component);

  this->events_.clear ();
  this->event_sinks_.clear ();
}

//
// write_method_begin
//
void CUTS_CIAO_Source_File_Generator::
write_method_begin (const PICML::PeriodicAction & periodic)
{
  std::string comment ("Periodic event: ");
  comment.append (periodic.name ());
  this->write_single_line_comment (comment);

  PICML::MgaObject parent = periodic.parent ();

  this->out_
    << "void " << parent.name ()
    << "::periodic_"  << periodic.name () << " (void) {"
    << "CUTS_Activation_Record dummy_record;"
    << "CUTS_Activation_Record * record = &dummy_record;";
}

//
// write_component_factory_begin
//
void CUTS_CIAO_Source_File_Generator::
write_component_factory_begin (const PICML::ComponentFactory & factory,
                               const PICML::Component & component)
{
  std::string factory_name;

  if (factory != Udm::null)
    factory_name = factory.name ();
  else
  {
    factory_name = component.name ();
    factory_name.append ("PICMLDefaultHome");
  }

  this->out_
    // Generate the constructor/destructor.
    << "//" << std::endl
    << "// " << factory_name << std::endl
    << "//" << std::endl
    << factory_name << "::" << factory_name << " (void) { }"

    << "//" << std::endl
    << "// ~" << factory_name << std::endl
    << "//" << std::endl
    << factory_name << "::~" << factory_name << " (void) { }"

    // Generate the export method implementation for the factory.
    << "CUTS_FACTORY_EXPORT_IMPLEMENT ("
    << factory_name << "," << std::endl
    << "create_";

  this->write_scope (factory, "_");
  this->out_ << factory_name << "_Impl);";
}
