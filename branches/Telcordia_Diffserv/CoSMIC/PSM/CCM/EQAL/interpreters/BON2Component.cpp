//###############################################################################################################################################
//
//	Meta and Builder Object Network V2.0 for GME
//	BON2Component.cpp
//
//###############################################################################################################################################

/*
	Copyright (c) Vanderbilt University, 2000-2003
	ALL RIGHTS RESERVED

	Vanderbilt University disclaims all warranties with regard to this
	software, including all implied warranties of merchantability
	and fitness.  In no event shall Vanderbilt University be liable for
	any special, indirect or consequential damages or any damages
	whatsoever resulting from loss of use, data or profits, whether
	in an action of contract, negligence or other tortious action,
	arising out of or in connection with the use or performance of
	this software.
*/

#include "BON2Component.h"

namespace BON
{

//###############################################################################################################################################
//
// 	C L A S S : BON::Component
//
//###############################################################################################################################################

Component::Component()
	: m_bIsInteractive( false )
{
}

Component::~Component()
{
	finalize( m_project );
	m_project = NULL;
}

// ====================================================
// This method is called after all the generic initialization is done
// This should be empty unless application-specific initialization is needed

void Component::initialize( Project& project )
{
	// ======================
	// Insert application specific code here
}

// ====================================================
// This method is called before the whole BON2 project released and disposed
// This should be empty unless application-specific finalization is needed

void Component::finalize( Project& project )
{
	// ======================
	// Insert application specific code here
}

// ====================================================
// This is the obsolete component interface
// This present implementation either tries to call InvokeEx, or does nothing except of a notification

void Component::invoke( Project& project, const std::set<FCO>& setModels, long lParam )
{
	#ifdef SUPPORT_OLD_INVOKE
		Object focus;
		invokeEx( project, focus, setModels, lParam );
	#else
		if ( m_bIsInteractive )
			AfxMessageBox("This BON2 Component does not support the obsolete invoke mechanism!");
	#endif
}

// ====================================================
// This is the main component method for Interpereters and Plugins.
// May also be used in case of invokeable Add-Ons

void Component::invokeEx( Project& project, FCO& currentFCO, const std::set<FCO>& setSelectedFCOs, long lParam )
{

	Component_Assembly assembly (currentFCO);

	if(!assembly)
    {
		  AfxMessageBox("Interpretation must start from a component assembly!");
		  return;
	  }

  AssemblyVisitor visitor;

  visitor.visitModelImpl (assembly);

}

// ====================================================
// GME currently does not use this function
// You only need to implement it if other invokation mechanisms are used

void Component::objectInvokeEx( Project& project, Object& currentObject, const std::set<Object>& setSelectedObjects, long lParam )
	{
		if ( m_bIsInteractive )
			AfxMessageBox("This BON2 Component does not support objectInvokeEx method!");
	}

// ====================================================
// Implement application specific parameter-mechanism in these functions

Util::Variant Component::getParameter( const std::string& strName )
{
	// ======================
	// Insert application specific code here

	return Util::Variant();
}

void Component::setParameter( const std::string& strName, const Util::Variant& varValue )
{
	// ======================
	// Insert application specific code here
}

#ifdef GME_ADDON

// ====================================================
// If the component is an Add-On, then this method is called for every Global Event

void Component::globalEventPerformed( globalevent_enum event )
{
	// ======================
	// Insert application specific code here
}

// ====================================================
// If the component is an Add-On, then this method is called for every Object Event

void Component::objectEventPerformed( Object& object, unsigned long event, VARIANT v )
{
	// ======================
	// Insert application specific code here
}

#endif // GME_ADDON

IMPLEMENT_BONEXTENSION( Component_Assembly, "Component_Assembly" );

IMPLEMENT_BONEXTENSION( CCMComponent, "Component" );

IMPLEMENT_BONEXTENSION( Event_Source, "Event_Source" );

IMPLEMENT_BONEXTENSION( Event_Sink, "Event_Sink" );

IMPLEMENT_BONEXTENSION( NS_Resource_Factory, "NS_Resource_Factory" );

IMPLEMENT_BONEXTENSION( RTEC_Resource_Factory, "RTEC_Resource_Factory" );

IMPLEMENT_BONEXTENSION( NS_Resource_Factory_Reference, "NS_Resource_Factory_Reference" );

IMPLEMENT_BONEXTENSION( RTEC_Resource_Factory_Reference, "RTEC_Resource_Factory_Reference" );

IMPLEMENT_BONEXTENSION( NS_QoS, "NS_QoS" );

IMPLEMENT_BONEXTENSION( RT_Info_Param, "" );

IMPLEMENT_BONEXTENSION( RTEC_Filter_Group, "" );

IMPLEMENT_BONEXTENSION( RT_Info, "RT_Info" );

IMPLEMENT_BONEXTENSION( Null_RT_Info, "Null_RT_Info" );

IMPLEMENT_BONEXTENSION( DisjunctionGroup, "DisjunctionGroup" );

IMPLEMENT_BONEXTENSION( ConjunctionGroup, "ConjunctionGroup" );

IMPLEMENT_BONEXTENSION( LogicalANDGroup, "LogicalANDGroup" );

IMPLEMENT_BONEXTENSION( eventtype, "eventtype" );

void AssemblyVisitor::visitModelImpl (const Model& model)
{

  this->visitComponent_AssemblyImpl (Component_Assembly (model));

}

void AssemblyVisitor::visitComponent_AssemblyImpl (const Component_Assembly& assembly)
{

  if (!assembly) return;

	std::set<Atom> child_atoms = assembly->getChildAtoms ();
	for (std::set<Atom>::iterator atom_iter = child_atoms.begin ();
       atom_iter != child_atoms.end ();
       atom_iter++)
    {
       this->visitAtomImpl (*atom_iter);
    }

}

void AssemblyVisitor::visitAtomImpl (const Atom& atom)
{
  this->visitEvent_SourceImpl (Event_Source (atom));
  this->visitEvent_SinkImpl (Event_Sink (atom));
}

void AssemblyVisitor::visitEvent_SourceImpl (const Event_Source& event_source)
{

  if (!event_source) return;

  std::multiset<ConnectionEnd> eventtype_list = event_source->getInConnEnds ("Port_eventtype");
	std::multiset<ConnectionEnd>::iterator eventtype_iter = eventtype_list.begin ();
  CCMComponent component (FCO (*eventtype_iter)->getParent ());
  if (!component) return; //ERROR
  std::string component_name = component->getName ();

  std::multiset<ConnectionEnd> rt_info_list = event_source->getOutConnEnds ("RTEC_Connection");
	for (std::multiset<ConnectionEnd>::iterator rt_info_iter = rt_info_list.begin ();
       rt_info_iter != rt_info_list.end ();
       rt_info_iter++)
    {
      RT_Info rt_info = FCO (*rt_info_iter);
      if (!rt_info) return;

      std::string entry_point (rt_info->getAttribute("entry_point")->getStringValue());
      std::string source_name (event_source->getName ());
      std::string cpf_name (component_name);
      cpf_name += "-" + source_name + "-" + entry_point;

      // Write CPF file
      rt_info->generate_CPF (cpf_name.c_str (), component_name.c_str (), source_name.c_str ());

      // Write svc.conf
      std::multiset<ConnectionEnd> rtec_factory_list = rt_info->getOutConnEnds ("Use_RT_Info");
	    for (std::multiset<ConnectionEnd>::iterator rtec_factory_iter = rtec_factory_list.begin ();
           rtec_factory_iter != rtec_factory_list.end ();
           rtec_factory_iter++)
        {

          RTEC_Resource_Factory_Reference rtec_factory_ref = FCO (*rtec_factory_iter);
          if (!rtec_factory_ref) return;
          
          RTEC_Resource_Factory rtec_factory = rtec_factory_ref->getReferred ();
          if (!rtec_factory) return;

          rtec_factory->generate_SVC (rtec_factory->getName ().c_str ());
        }

    }

  std::multiset<ConnectionEnd> ns_info_list = event_source->getOutConnEnds ("Use_NS_QoS");
	for (std::multiset<ConnectionEnd>::iterator ns_info_iter = ns_info_list.begin ();
       ns_info_iter != ns_info_list.end ();
       ns_info_iter++)
    {
      NS_QoS ns_qos = FCO (*ns_info_iter);
      if (!ns_qos) return;

      std::string source_name (event_source->getName ());
      std::string cpf_name (component_name);
      cpf_name += "-" + source_name;

      // Write CPF file
      ns_qos->generate_CPF (cpf_name.c_str (), component_name.c_str (), source_name.c_str ());

      // Write svc.conf
      std::multiset<ConnectionEnd> ns_factory_list = ns_qos->getOutConnEnds ("Use_NS_Resource_Factory");
	    for (std::multiset<ConnectionEnd>::iterator ns_factory_iter = ns_factory_list.begin ();
           ns_factory_iter != ns_factory_list.end ();
           ns_factory_iter++)
        {

          NS_Resource_Factory_Reference ns_factory_ref = FCO (*ns_factory_iter);
          if (!ns_factory_ref) return;

          NS_Resource_Factory ns_factory = ns_factory_ref->getReferred ();
          if (!ns_factory) return;

          ns_factory->generate_SVC (ns_factory->getName ().c_str ());
        }

    }

}

void AssemblyVisitor::visitEvent_SinkImpl (const Event_Sink& event_sink)
{

  if (!event_sink) return;

  std::multiset<ConnectionEnd> eventtype_list = event_sink->getOutConnEnds ("Port_eventtype");
	std::multiset<ConnectionEnd>::iterator eventtype_iter = eventtype_list.begin ();
  CCMComponent component (FCO (*eventtype_iter)->getParent ());
  if (!component) return; //ERROR
  std::string component_name = component->getName ();

  std::string sink_name = event_sink->getName ();

  std::multiset<ConnectionEnd> rtec_filter_list = event_sink->getInConnEnds ("Use_RTEC_Filter");
	for (std::multiset<ConnectionEnd>::iterator filter_iter = rtec_filter_list.begin ();
       filter_iter != rtec_filter_list.end ();
       filter_iter++)
    {
       ECDVisitor visitor (component_name.c_str (), sink_name.c_str ());
       visitor.visitAtomImpl (FCO (*filter_iter));
    }

  std::multiset<ConnectionEnd> ns_qos_list = event_sink->getInConnEnds ("Use_NS_QoS");
	for (std::multiset<ConnectionEnd>::iterator ns_qos_iter = ns_qos_list.begin ();
       ns_qos_iter != ns_qos_list.end ();
       ns_qos_iter++)
    {
      NS_QoS ns_qos = FCO (*ns_qos_iter);
      if (!ns_qos) return;

      std::string cpf_name (component_name);
      cpf_name += "-" + sink_name;

      ns_qos->generate_CPF (cpf_name.c_str (), component_name.c_str (), sink_name.c_str ());
    }

}

ECDVisitor::ECDVisitor (const char * component_name, const char * sink_name)
  : indent_ (0),
    component_name_ (component_name),
    sink_name_ (sink_name)
{
  std::string filename (this->component_name_);
  filename += "-" + this->sink_name_ + ".ecd";
  this->file_out_.open (filename.c_str ());
  this->file_out_ << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
                  << "<!DOCTYPE filter SYSTEM \"eventchannel.dtd\">\n"
                  << "<filter>\n";
}

ECDVisitor::~ECDVisitor ()
{
  this->file_out_ << "</filter>\n";
  this->file_out_.close ();
}

void ECDVisitor::visitAtomImpl (const Atom& atom)
{
  this->visitDisjunctionGroupImpl (DisjunctionGroup (atom));
  this->visitConjunctionGroupImpl (ConjunctionGroup (atom));
  this->visitLogicalANDGroupImpl (LogicalANDGroup (atom));
  this->visitRT_InfoImpl (RT_Info (atom));
  this->visitNull_RT_InfoImpl (Null_RT_Info (atom));
}

void ECDVisitor::visitDisjunctionGroupImpl (const DisjunctionGroup& disjunction)
{

  if (!disjunction) return;

  this->indent_++;

  for (int i = 0; i < this->indent_; i++) this->file_out_ << "  ";

  this->file_out_ << "<disjunction>\n";

  std::multiset<ConnectionEnd> rtec_filter_list = disjunction->getInConnEnds ();
	for (std::multiset<ConnectionEnd>::iterator filter_iter = rtec_filter_list.begin ();
       filter_iter != rtec_filter_list.end ();
       filter_iter++)
    {
       this->visitAtomImpl (FCO (*filter_iter));
    }

  for (int j = 0; j < this->indent_; j++) this->file_out_ << "  ";

  this->file_out_ << "</disjunction>\n";

  this->indent_--;

}

void ECDVisitor::visitConjunctionGroupImpl (const ConjunctionGroup& conjunction)
{

  if (!conjunction) return;

  this->indent_++;

  for (int i = 0; i < this->indent_; i++) this->file_out_ << "  ";

  this->file_out_ << "<conjunction>\n";

  std::multiset<ConnectionEnd> rtec_filter_list = conjunction->getInConnEnds ();
	for (std::multiset<ConnectionEnd>::iterator filter_iter = rtec_filter_list.begin ();
       filter_iter != rtec_filter_list.end ();
       filter_iter++)
    {
       this->visitAtomImpl (FCO (*filter_iter));
    }

  for (int j = 0; j < this->indent_; j++) this->file_out_ << "  ";

  this->file_out_ << "</conjunction>\n";

  this->indent_--;

}

void ECDVisitor::visitLogicalANDGroupImpl (const LogicalANDGroup& logicaland)
{

  if (!logicaland) return;

  this->indent_++;

  for (int i = 0; i < this->indent_; i++) this->file_out_ << "  ";

  this->file_out_ << "<logicaland>\n";

  std::multiset<ConnectionEnd> rtec_filter_list = logicaland->getInConnEnds ();
	for (std::multiset<ConnectionEnd>::iterator filter_iter = rtec_filter_list.begin ();
       filter_iter != rtec_filter_list.end ();
       filter_iter++)
    {
       this->visitAtomImpl (FCO (*filter_iter));
    }

  for (int j = 0; j < this->indent_; j++) this->file_out_ << "  ";

  this->file_out_ << "</logicaland>\n";

  this->indent_--;

}

void ECDVisitor::visitNull_RT_InfoImpl (const Null_RT_Info& null_rt_info)
{

  if (!null_rt_info) return;

  std::multiset<ConnectionEnd> event_source_list = null_rt_info->getDirectInConnEnds ("Use_RT_Info");
	for (std::multiset<ConnectionEnd>::iterator event_source_iter = event_source_list.begin ();
       event_source_iter != event_source_list.end ();
       event_source_iter++)
    {
      std::multiset<ConnectionEnd> publisher_list = (*event_source_iter)->getInConnEnds ("RTEC_Connection");
	    for (std::multiset<ConnectionEnd>::iterator publisher_iter = publisher_list.begin ();
           publisher_iter != publisher_list.end ();
           publisher_iter++)
        {
          for (int i = 0; i < this->indent_ + 1; i++) this->file_out_ << "  ";
          this->file_out_ << "<id>" << FCO (*publisher_iter)->getName () << "</id>\n";
        }
    }

}

void ECDVisitor::visitRT_InfoImpl (const RT_Info& rt_info)
{
  if (!rt_info) return;

  std::string entry_point = rt_info->getAttribute("entry_point")->getStringValue();
  std::string publisher_name;
  std::multiset<ConnectionEnd> event_source_list = rt_info->getDirectInConnEnds ("Use_RT_Info");
	for (std::multiset<ConnectionEnd>::iterator event_source_iter = event_source_list.begin ();
       event_source_iter != event_source_list.end ();
       event_source_iter++)
    {
      std::multiset<ConnectionEnd> publisher_list = (*event_source_iter)->getInConnEnds ("RTEC_Connection");
	    for (std::multiset<ConnectionEnd>::iterator publisher_iter = publisher_list.begin ();
           publisher_iter != publisher_list.end ();
           publisher_iter++)
        {
          publisher_name = FCO (*publisher_iter)->getName ();
        }
    }

  std::string cpf_name (this->component_name_);
  cpf_name += "-" + this->sink_name_ + "-" + entry_point;

  for (int i = 0; i < this->indent_ + 1; i++) this->file_out_ << "  ";
  this->file_out_ << "<id>file://" << cpf_name << ".cpf" << "</id>\n";

  rt_info->generate_CPF (cpf_name.c_str (), this->component_name_.c_str (), publisher_name.c_str ());

}

void RTEC_Resource_FactoryImpl::generate_SVC (const char * file_name)
{

  std::ofstream svc_file;
  std::string svc_name (file_name);
  svc_name += ".conf";
  svc_file.open (svc_name.c_str ());

  svc_file << "static EC_Factory \"";

  std::set<Attribute> rtec_factory_attributes = this->getAttributes ();
	for (std::set<Attribute>::iterator attribute_iter = rtec_factory_attributes.begin ();
       attribute_iter != rtec_factory_attributes.end ();
       attribute_iter++)
    {
       std::string default_value ((*attribute_iter)->getAttributeMeta ().defaultValue ());
       if ((*attribute_iter)->getStringValue () != default_value)
         svc_file << " -" << (*attribute_iter)->getAttributeMeta ().name () << " " << (*attribute_iter)->getStringValue ();
    }

  svc_file << "\"\n";
  svc_file.close ();

}

void NS_Resource_FactoryImpl::generate_SVC (const char * file_name)
{
  std::ofstream svc_file;
  std::string svc_name (file_name);
  svc_name += ".conf";
  svc_file.open (svc_name.c_str ());

  svc_file << "static Notify_Default_Event_Manager_Objects_Factory \"";

  std::set<Attribute> ns_factory_attributes = this->getAttributes ();
	for (std::set<Attribute>::iterator attribute_iter = ns_factory_attributes.begin ();
       attribute_iter != ns_factory_attributes.end ();
       attribute_iter++)
    {
       std::string default_value ((*attribute_iter)->getAttributeMeta ().defaultValue ());
       if ((*attribute_iter)->getStringValue () != default_value)
         svc_file << " -" << (*attribute_iter)->getAttributeMeta ().name () << " " << (*attribute_iter)->getStringValue ();
    }

  svc_file << "\"\n";
  svc_file.close ();
}

void RT_InfoImpl::generate_CPF (const char * file_name, const char * component_name, const char * port_name)
{

  std::ofstream cpf_file;
  std::string cpf_name (file_name);
  cpf_name += ".cpf";
  cpf_file.open (cpf_name.c_str ());

  cpf_file << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
           << "<!DOCTYPE properties SYSTEM \"properties.dtd\">\n"
           << "<properties>\n"
           << "  <struct name=\"" << file_name << "\" type=\"ACEXML_RT_Info\">\n"
           << "    <description>TAO Real-time Scheduler info for " << component_name << "::" << port_name << "</description>\n"
           << "    <simple name=\"port\" type=\"string\">\n"
           << "      <value>" << port_name << "</value>\n"
           << "    </simple>\n";

  std::set<Attribute> rt_info_attributes = this->getAttributes ();
	for (std::set<Attribute>::iterator attribute_iter = rt_info_attributes.begin ();
       attribute_iter != rt_info_attributes.end ();
       attribute_iter++)
    {
       cpf_file << "    <simple name=\"" << (*attribute_iter)->getAttributeMeta ().name () << "\" type=\"string\">\n"
                << "      <value>" << (*attribute_iter)->getStringValue () << "</value>\n"
                << "    </simple>\n";
    }
  cpf_file << "  </struct>\n"
           << "</properties>\n";

  cpf_file.close ();

}

void NS_QoSImpl::generate_CPF (const char * file_name, const char * component_name, const char * port_name)
{

  std::ofstream cpf_file;
  std::string cpf_name (file_name);
  cpf_name += ".cpf";
  cpf_file.open (cpf_name.c_str ());

  cpf_file << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
           << "<!DOCTYPE properties SYSTEM \"properties.dtd\">\n"
           << "<properties>\n"
           << "  <struct name=\"" << file_name << "\" type=\"ACEXML_NS_QoS\">\n"
           << "    <description>TAO Real-time Scheduler info for " << component_name << "::" << port_name << "</description>\n"
           << "    <simple name=\"port\" type=\"string\">\n"
           << "      <value>" << port_name << "</value>\n"
           << "    </simple>\n";

  std::set<Attribute> rt_info_attributes = this->getAttributes ();
	for (std::set<Attribute>::iterator attribute_iter = rt_info_attributes.begin ();
       attribute_iter != rt_info_attributes.end ();
       attribute_iter++)
    {
       cpf_file << "    <simple name=\"" << (*attribute_iter)->getAttributeMeta ().name () << "\" type=\"string\">\n"
                << "      <value>" << (*attribute_iter)->getStringValue () << "</value>\n"
                << "    </simple>\n";
    }
  cpf_file << "  </struct>\n"
           << "</properties>\n";

  cpf_file.close ();

}

}; // namespace BON

