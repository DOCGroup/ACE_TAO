// cpp source file cadena_scenario.cpp generated from diagram cadena_scenario
// generated on Thu Aug 05 20:28:24 2004

#include "cadena_scenario.h"
#include "UmlExt.h"

namespace cadena_scenario {

	Uml::Diagram umldiagram;

	Uml::Class ESSConnection::meta;
	Uml::Class Scenarios::meta;
	Uml::Class FRConnection::meta;
	Uml::Class EventSink::meta;
	Uml::Class EventSource::meta;
	Uml::Class Receptacle::meta;
	Uml::Class Facet::meta;
	Uml::Class ComponentPort::meta;
	Uml::Class Property::meta;
	Uml::Class ComponentInstance::meta;
	Uml::Class Scenario::meta;
	Uml::Class ScenarioElement::meta;
	Uml::Class PortConnection::meta;
	Uml::Attribute ESSConnection::meta_ConnectorID;
	Uml::Attribute ComponentPort::meta_name;
	Uml::Attribute Property::meta_value;
	Uml::Attribute Property::meta_name;
	Uml::Attribute Property::meta_type;
	Uml::Attribute ComponentInstance::meta_type;
	Uml::Attribute ComponentInstance::meta_name;
	Uml::AssociationRole ESSConnection::meta_sources;
	Uml::AssociationRole ESSConnection::meta_sinks;
	Uml::AssociationRole FRConnection::meta_facets_end_;
	Uml::AssociationRole FRConnection::meta_receptacles_end_;
	Uml::AssociationRole EventSink::meta_sourcesConnections;
	Uml::AssociationRole EventSource::meta_sinksConnections;
	Uml::AssociationRole Receptacle::meta_facets, Receptacle::meta_facets_rev;
	Uml::AssociationRole Facet::meta_receptacles, Facet::meta_receptacles_rev;
	Uml::CompositionParentRole ComponentPort::meta_ComponentInstance_parent;
	Uml::CompositionParentRole Property::meta_properties_ScenarioElement_parent;
	Uml::CompositionParentRole ComponentInstance::meta_Scenario_parent;
	Uml::CompositionParentRole Scenario::meta_Scenarios_parent;
	Uml::CompositionParentRole PortConnection::meta_Scenario_parent;
	Uml::CompositionChildRole Scenarios::meta_Scenario_children;
	Uml::CompositionChildRole ComponentInstance::meta_ComponentPort_children;
	Uml::CompositionChildRole Scenario::meta_PortConnection_children;
	Uml::CompositionChildRole Scenario::meta_ComponentInstance_children;
	Uml::CompositionChildRole ScenarioElement::meta_properties;
	void Initialize()
	{
		static bool first = true;
		if(!first) return;
		first = false;
		Uml::Initialize();

		ASSERT( umldiagram == Udm::null );
		umldiagram = Uml::CreateCORBADiagram();

		ESSConnection::meta = Uml::CreateCORBAClass();
		Scenarios::meta = Uml::CreateCORBAClass();
		FRConnection::meta = Uml::CreateCORBAClass();
		EventSink::meta = Uml::CreateCORBAClass();
		EventSource::meta = Uml::CreateCORBAClass();
		Receptacle::meta = Uml::CreateCORBAClass();
		Facet::meta = Uml::CreateCORBAClass();
		ComponentPort::meta = Uml::CreateCORBAClass();
		Property::meta = Uml::CreateCORBAClass();
		ComponentInstance::meta = Uml::CreateCORBAClass();
		Scenario::meta = Uml::CreateCORBAClass();
		ScenarioElement::meta = Uml::CreateCORBAClass();
		PortConnection::meta = Uml::CreateCORBAClass();
		ESSConnection::meta_ConnectorID = Uml::CreateCORBAAttribute();
		ComponentPort::meta_name = Uml::CreateCORBAAttribute();
		Property::meta_value = Uml::CreateCORBAAttribute();
		Property::meta_name = Uml::CreateCORBAAttribute();
		Property::meta_type = Uml::CreateCORBAAttribute();
		ComponentInstance::meta_type = Uml::CreateCORBAAttribute();
		ComponentInstance::meta_name = Uml::CreateCORBAAttribute();
		ESSConnection::meta_sources = Uml::CreateCORBAAssociationRole();
		ESSConnection::meta_sinks = Uml::CreateCORBAAssociationRole();
		EventSink::meta_sourcesConnections = Uml::CreateCORBAAssociationRole();
		EventSource::meta_sinksConnections = Uml::CreateCORBAAssociationRole();
		Receptacle::meta_facets = Uml::CreateCORBAAssociationRole();
		Facet::meta_receptacles = Uml::CreateCORBAAssociationRole();
		ComponentPort::meta_ComponentInstance_parent = Uml::CreateCORBACompositionParentRole();
		Property::meta_properties_ScenarioElement_parent = Uml::CreateCORBACompositionParentRole();
		ComponentInstance::meta_Scenario_parent = Uml::CreateCORBACompositionParentRole();
		Scenario::meta_Scenarios_parent = Uml::CreateCORBACompositionParentRole();
		PortConnection::meta_Scenario_parent = Uml::CreateCORBACompositionParentRole();
		Scenarios::meta_Scenario_children = Uml::CreateCORBACompositionChildRole();
		ComponentInstance::meta_ComponentPort_children = Uml::CreateCORBACompositionChildRole();
		Scenario::meta_PortConnection_children = Uml::CreateCORBACompositionChildRole();
		Scenario::meta_ComponentInstance_children = Uml::CreateCORBACompositionChildRole();
		ScenarioElement::meta_properties = Uml::CreateCORBACompositionChildRole();
		Uml::InitCORBADiagram(umldiagram, "cadena_scenario");

		Uml::InitCORBAClass(ESSConnection::meta, umldiagram, "ESSConnection", false);
		Uml::InitCORBAClass(Scenarios::meta, umldiagram, "Scenarios", false);
		Uml::InitCORBAClass(FRConnection::meta, umldiagram, "FRConnection", false);
		Uml::InitCORBAClass(EventSink::meta, umldiagram, "EventSink", false);
		Uml::InitCORBAClass(EventSource::meta, umldiagram, "EventSource", false);
		Uml::InitCORBAClass(Receptacle::meta, umldiagram, "Receptacle", false);
		Uml::InitCORBAClass(Facet::meta, umldiagram, "Facet", false);
		Uml::InitCORBAClass(ComponentPort::meta, umldiagram, "ComponentPort", true);
		Uml::InitCORBAClass(Property::meta, umldiagram, "Property", false);
		Uml::InitCORBAClass(ComponentInstance::meta, umldiagram, "ComponentInstance", false);
		Uml::InitCORBAClass(Scenario::meta, umldiagram, "Scenario", false);
		Uml::InitCORBAClass(ScenarioElement::meta, umldiagram, "ScenarioElement", true);
		Uml::InitCORBAClass(PortConnection::meta, umldiagram, "PortConnection", true);
		
		vector<string> ESSConnection_ConnectorID_dva;
		Uml::InitCORBAAttribute(ESSConnection::meta_ConnectorID, ESSConnection::meta, "ConnectorID", "String", false, 1, 1, false, "public", ESSConnection_ConnectorID_dva);
		vector<string> ComponentPort_name_dva;
		Uml::InitCORBAAttribute(ComponentPort::meta_name, ComponentPort::meta, "name", "String", false, 1, 1, false, "public", ComponentPort_name_dva);
		vector<string> Property_value_dva;
		Uml::InitCORBAAttribute(Property::meta_value, Property::meta, "value", "String", false, 1, 1, false, "public", Property_value_dva);
		vector<string> Property_name_dva;
		Uml::InitCORBAAttribute(Property::meta_name, Property::meta, "name", "String", false, 1, 1, false, "public", Property_name_dva);
		vector<string> Property_type_dva;
		Uml::InitCORBAAttribute(Property::meta_type, Property::meta, "type", "String", false, 1, 1, false, "public", Property_type_dva);
		vector<string> ComponentInstance_type_dva;
		Uml::InitCORBAAttribute(ComponentInstance::meta_type, ComponentInstance::meta, "type", "String", false, 1, 1, false, "public", ComponentInstance_type_dva);
		vector<string> ComponentInstance_name_dva;
		Uml::InitCORBAAttribute(ComponentInstance::meta_name, ComponentInstance::meta, "name", "String", false, 1, 1, false, "public", ComponentInstance_name_dva);
		
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(ESSConnection::meta_sources, ass, "sources", true, false, 0, -1, EventSource::meta);
			Uml::InitCORBAAssociationRole(EventSource::meta_sinksConnections, ass, "sinksConnections", true, false, 0, -1, ESSConnection::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(ESSConnection::meta_sinks, ass, "sinks", true, false, 0, -1, EventSink::meta);
			Uml::InitCORBAAssociationRole(EventSink::meta_sourcesConnections, ass, "sourcesConnections", true, false, 0, -1, ESSConnection::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "FRConnection");
			Uml::InitCORBAAssociationClass(ass, FRConnection::meta);
			Uml::InitCORBAAssociationRole(Facet::meta_receptacles, ass, "receptacles", true, false, 0, -1, Receptacle::meta);
			FRConnection::meta_receptacles_end_ = Receptacle::meta_facets_rev = Facet::meta_receptacles;
			Uml::InitCORBAAssociationRole(Receptacle::meta_facets, ass, "facets", true, false, 0, -1, Facet::meta);
			FRConnection::meta_facets_end_ = Facet::meta_receptacles_rev = Receptacle::meta_facets;
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Scenario::meta_Scenarios_parent, comp, "Scenarios_parent", true, Scenarios::meta);
			Uml::InitCORBACompositionChildRole(Scenarios::meta_Scenario_children, comp, "Scenario", true, 0, -1, Scenario::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentPort::meta_ComponentInstance_parent, comp, "ComponentInstance_parent", true, ComponentInstance::meta);
			Uml::InitCORBACompositionChildRole(ComponentInstance::meta_ComponentPort_children, comp, "ComponentPort", true, 0, -1, ComponentPort::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PortConnection::meta_Scenario_parent, comp, "Scenario_parent", true, Scenario::meta);
			Uml::InitCORBACompositionChildRole(Scenario::meta_PortConnection_children, comp, "PortConnection", true, 0, -1, PortConnection::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentInstance::meta_Scenario_parent, comp, "Scenario_parent", true, Scenario::meta);
			Uml::InitCORBACompositionChildRole(Scenario::meta_ComponentInstance_children, comp, "ComponentInstance", true, 0, -1, ComponentInstance::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Property::meta_properties_ScenarioElement_parent, comp, "properties_ScenarioElement_parent", true, ScenarioElement::meta);
			Uml::InitCORBACompositionChildRole(ScenarioElement::meta_properties, comp, "properties", true, 0, -1, Property::meta);
		}
		Uml::AddCORBAInheritance(ComponentPort::meta, EventSink::meta);
		Uml::AddCORBAInheritance(ComponentPort::meta, EventSource::meta);
		Uml::AddCORBAInheritance(ComponentPort::meta, Receptacle::meta);
		Uml::AddCORBAInheritance(ComponentPort::meta, Facet::meta);
		Uml::AddCORBAInheritance(ScenarioElement::meta, ComponentInstance::meta);
		Uml::AddCORBAInheritance(ScenarioElement::meta, Scenario::meta);
		Uml::AddCORBAInheritance(ScenarioElement::meta, PortConnection::meta);
		Uml::AddCORBAInheritance(PortConnection::meta, ESSConnection::meta);
		Uml::AddCORBAInheritance(PortConnection::meta, FRConnection::meta);
		
	}
	Udm::UdmDiagram diagram = { &umldiagram, Initialize };
}
// END cadena_scenario.cpp
