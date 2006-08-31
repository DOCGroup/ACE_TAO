#ifndef MOBIES_CADENA_SCENARIO_H
#define MOBIES_CADENA_SCENARIO_H
// header file cadena_scenario.h generated from diagram cadena_scenario
// generated on Mon Nov 15 23:04:57 2004

#ifndef MOBIES_UDMBASE_H
#include "UdmBase.h"
#endif

namespace PICML
{ 
	 class  PublishConnector;
	 class  invoke;
	 class  emit;
	 class  ComponentAssembly;
	 class  Property;
	 class  OutEventPort;
	 class  ProvidedRequestPort;
	 class  RequiredRequestPort;
	 class  InEventPort;
	 class  Port;
	 class  Component;
};
namespace PICMLCadena_Xchange_embedded_EIProperties
{ 
	 class  RootContainer;
};


namespace cadena_scenario {

	class  ESSConnection;
	class  Scenarios;
	class  FRConnection;
	class  EventSink;
	class  EventSource;
	class  Receptacle;
	class  Facet;
	class  ComponentPort;
	class  Property;
	class  ComponentInstance;
	class  Scenario;
	class  ScenarioElement;
	class  PortConnection;

	  void Initialize();

	  void Initialize(const Uml::Diagram &dgr);
	extern  Udm::UdmDiagram diagram;

	class  Scenarios :  public Udm::Object {
	 public:
		static Uml::Class meta;

		Scenarios() { }
		Scenarios(Udm::ObjectImpl *impl) : Object(impl) { }
		Scenarios(const Scenarios &master) : Object(master) { }
		static Scenarios Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Scenarios Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Scenarios CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Scenarios CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Scenarios> Instances() { return Udm::InstantiatedAttr<cadena_scenario::Scenarios>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Scenarios, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<cadena_scenario::Scenarios, Pred>(impl);}

		Udm::DerivedAttr<cadena_scenario::Scenarios> Derived() { return Udm::DerivedAttr<cadena_scenario::Scenarios>(impl);}
		template <class Pred> Udm::DerivedAttr<cadena_scenario::Scenarios, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<cadena_scenario::Scenarios, Pred>(impl);}

		Udm::ArchetypeAttr<cadena_scenario::Scenarios> Archetype() { return Udm::ArchetypeAttr<cadena_scenario::Scenarios>(impl);}

		static Uml::CompositionChildRole meta_Scenario_children;
		Udm::ChildrenAttr<cadena_scenario::Scenario> Scenario_children() const { return Udm::ChildrenAttr<cadena_scenario::Scenario>(impl, meta_Scenario_children); }
		 template <class Pred> Udm::ChildrenAttr<cadena_scenario::Scenario, Pred> Scenario_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::Scenario, Pred>(impl, meta_Scenario_children); }

		Udm::ChildrenAttr<cadena_scenario::Scenario> Scenario_kind_children() const { return Udm::ChildrenAttr<cadena_scenario::Scenario>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<cadena_scenario::Scenario, Pred> Scenario_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::Scenario, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<cadena_scenario::ScenarioElement> ScenarioElement_kind_children() const { return Udm::ChildrenAttr<cadena_scenario::ScenarioElement>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<cadena_scenario::ScenarioElement, Pred> ScenarioElement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::ScenarioElement, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class  ComponentPort :  public Udm::Object {
	 public:
		static Uml::Class meta;

		ComponentPort() { }
		ComponentPort(Udm::ObjectImpl *impl) : Object(impl) { }
		ComponentPort(const ComponentPort &master) : Object(master) { }
		static ComponentPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentPort Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentPort CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentPort CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentPort> Instances() { return Udm::InstantiatedAttr<cadena_scenario::ComponentPort>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<cadena_scenario::ComponentPort, Pred>(impl);}

		Udm::DerivedAttr<cadena_scenario::ComponentPort> Derived() { return Udm::DerivedAttr<cadena_scenario::ComponentPort>(impl);}
		template <class Pred> Udm::DerivedAttr<cadena_scenario::ComponentPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<cadena_scenario::ComponentPort, Pred>(impl);}

		Udm::ArchetypeAttr<cadena_scenario::ComponentPort> Archetype() { return Udm::ArchetypeAttr<cadena_scenario::ComponentPort>(impl);}

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::AssociationRole meta_picmlPort;
		Udm::CrossPointerAttr<PICML::Port> picmlPort() const { return Udm::CrossPointerAttr<PICML::Port>(impl, meta_picmlPort); }

		static Uml::CompositionParentRole meta_ComponentInstance_parent;
		Udm::ParentAttr<cadena_scenario::ComponentInstance> ComponentInstance_parent() const { return Udm::ParentAttr<cadena_scenario::ComponentInstance>(impl, meta_ComponentInstance_parent); }

		Udm::ParentAttr<cadena_scenario::ScenarioElement> parent() const { return Udm::ParentAttr<cadena_scenario::ScenarioElement>(impl, Udm::NULLPARENTROLE); }
	};

	class  EventSink :  public ComponentPort {
	 public:
		static Uml::Class meta;

		EventSink() { }
		EventSink(Udm::ObjectImpl *impl) : ComponentPort(impl) { }
		EventSink(const EventSink &master) : ComponentPort(master) { }
		static EventSink Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static EventSink Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		EventSink CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		EventSink CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<EventSink> Instances() { return Udm::InstantiatedAttr<cadena_scenario::EventSink>(impl);}
		template <class Pred> Udm::InstantiatedAttr<EventSink, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<cadena_scenario::EventSink, Pred>(impl);}

		Udm::DerivedAttr<cadena_scenario::EventSink> Derived() { return Udm::DerivedAttr<cadena_scenario::EventSink>(impl);}
		template <class Pred> Udm::DerivedAttr<cadena_scenario::EventSink, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<cadena_scenario::EventSink, Pred>(impl);}

		Udm::ArchetypeAttr<cadena_scenario::EventSink> Archetype() { return Udm::ArchetypeAttr<cadena_scenario::EventSink>(impl);}

		static Uml::AssociationRole meta_sourcesConnections;
		Udm::AssocAttr<cadena_scenario::ESSConnection> sourcesConnections() const { return Udm::AssocAttr<cadena_scenario::ESSConnection>(impl, meta_sourcesConnections); }
		template <class Pred> Udm::AssocAttr<cadena_scenario::ESSConnection, Pred > sourcesConnections_sorted(const Pred &) const { return Udm::AssocAttr<cadena_scenario::ESSConnection, Pred>(impl, meta_sourcesConnections); }

		Udm::ParentAttr<cadena_scenario::ScenarioElement> parent() const { return Udm::ParentAttr<cadena_scenario::ScenarioElement>(impl, Udm::NULLPARENTROLE); }
	};

	class  EventSource :  public ComponentPort {
	 public:
		static Uml::Class meta;

		EventSource() { }
		EventSource(Udm::ObjectImpl *impl) : ComponentPort(impl) { }
		EventSource(const EventSource &master) : ComponentPort(master) { }
		static EventSource Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static EventSource Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		EventSource CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		EventSource CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<EventSource> Instances() { return Udm::InstantiatedAttr<cadena_scenario::EventSource>(impl);}
		template <class Pred> Udm::InstantiatedAttr<EventSource, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<cadena_scenario::EventSource, Pred>(impl);}

		Udm::DerivedAttr<cadena_scenario::EventSource> Derived() { return Udm::DerivedAttr<cadena_scenario::EventSource>(impl);}
		template <class Pred> Udm::DerivedAttr<cadena_scenario::EventSource, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<cadena_scenario::EventSource, Pred>(impl);}

		Udm::ArchetypeAttr<cadena_scenario::EventSource> Archetype() { return Udm::ArchetypeAttr<cadena_scenario::EventSource>(impl);}

		static Uml::AssociationRole meta_sinksConnections;
		Udm::AssocAttr<cadena_scenario::ESSConnection> sinksConnections() const { return Udm::AssocAttr<cadena_scenario::ESSConnection>(impl, meta_sinksConnections); }
		template <class Pred> Udm::AssocAttr<cadena_scenario::ESSConnection, Pred > sinksConnections_sorted(const Pred &) const { return Udm::AssocAttr<cadena_scenario::ESSConnection, Pred>(impl, meta_sinksConnections); }

		Udm::ParentAttr<cadena_scenario::ScenarioElement> parent() const { return Udm::ParentAttr<cadena_scenario::ScenarioElement>(impl, Udm::NULLPARENTROLE); }
	};

	class  Receptacle :  public ComponentPort {
	 public:
		static Uml::Class meta;

		Receptacle() { }
		Receptacle(Udm::ObjectImpl *impl) : ComponentPort(impl) { }
		Receptacle(const Receptacle &master) : ComponentPort(master) { }
		static Receptacle Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Receptacle Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Receptacle CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Receptacle CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Receptacle> Instances() { return Udm::InstantiatedAttr<cadena_scenario::Receptacle>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Receptacle, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<cadena_scenario::Receptacle, Pred>(impl);}

		Udm::DerivedAttr<cadena_scenario::Receptacle> Derived() { return Udm::DerivedAttr<cadena_scenario::Receptacle>(impl);}
		template <class Pred> Udm::DerivedAttr<cadena_scenario::Receptacle, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<cadena_scenario::Receptacle, Pred>(impl);}

		Udm::ArchetypeAttr<cadena_scenario::Receptacle> Archetype() { return Udm::ArchetypeAttr<cadena_scenario::Receptacle>(impl);}

		static Uml::AssociationRole meta_facets, meta_facets_rev;
		Udm::AClassAssocAttr<cadena_scenario::FRConnection, cadena_scenario::Facet> facets() const { return Udm::AClassAssocAttr<cadena_scenario::FRConnection, cadena_scenario::Facet>(impl, meta_facets, meta_facets_rev); }
		template<class Pred> Udm::AClassAssocAttr<cadena_scenario::FRConnection, cadena_scenario::Facet, Pred> facets_sorted(const Pred &) const { return Udm::AClassAssocAttr<cadena_scenario::FRConnection, cadena_scenario::Facet, Pred>(impl, meta_facets, meta_facets_rev); }

		Udm::ParentAttr<cadena_scenario::ScenarioElement> parent() const { return Udm::ParentAttr<cadena_scenario::ScenarioElement>(impl, Udm::NULLPARENTROLE); }
	};

	class  Facet :  public ComponentPort {
	 public:
		static Uml::Class meta;

		Facet() { }
		Facet(Udm::ObjectImpl *impl) : ComponentPort(impl) { }
		Facet(const Facet &master) : ComponentPort(master) { }
		static Facet Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Facet Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Facet CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Facet CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Facet> Instances() { return Udm::InstantiatedAttr<cadena_scenario::Facet>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Facet, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<cadena_scenario::Facet, Pred>(impl);}

		Udm::DerivedAttr<cadena_scenario::Facet> Derived() { return Udm::DerivedAttr<cadena_scenario::Facet>(impl);}
		template <class Pred> Udm::DerivedAttr<cadena_scenario::Facet, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<cadena_scenario::Facet, Pred>(impl);}

		Udm::ArchetypeAttr<cadena_scenario::Facet> Archetype() { return Udm::ArchetypeAttr<cadena_scenario::Facet>(impl);}

		static Uml::AssociationRole meta_receptacles, meta_receptacles_rev;
		Udm::AClassAssocAttr<cadena_scenario::FRConnection, cadena_scenario::Receptacle> receptacles() const { return Udm::AClassAssocAttr<cadena_scenario::FRConnection, cadena_scenario::Receptacle>(impl, meta_receptacles, meta_receptacles_rev); }
		template<class Pred> Udm::AClassAssocAttr<cadena_scenario::FRConnection, cadena_scenario::Receptacle, Pred> receptacles_sorted(const Pred &) const { return Udm::AClassAssocAttr<cadena_scenario::FRConnection, cadena_scenario::Receptacle, Pred>(impl, meta_receptacles, meta_receptacles_rev); }

		Udm::ParentAttr<cadena_scenario::ScenarioElement> parent() const { return Udm::ParentAttr<cadena_scenario::ScenarioElement>(impl, Udm::NULLPARENTROLE); }
	};

	class  Property :  public Udm::Object {
	 public:
		static Uml::Class meta;

		Property() { }
		Property(Udm::ObjectImpl *impl) : Object(impl) { }
		Property(const Property &master) : Object(master) { }
		static Property Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Property Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Property CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Property CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Property> Instances() { return Udm::InstantiatedAttr<cadena_scenario::Property>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Property, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<cadena_scenario::Property, Pred>(impl);}

		Udm::DerivedAttr<cadena_scenario::Property> Derived() { return Udm::DerivedAttr<cadena_scenario::Property>(impl);}
		template <class Pred> Udm::DerivedAttr<cadena_scenario::Property, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<cadena_scenario::Property, Pred>(impl);}

		Udm::ArchetypeAttr<cadena_scenario::Property> Archetype() { return Udm::ArchetypeAttr<cadena_scenario::Property>(impl);}

		static Uml::Attribute meta_value;
		Udm::StringAttr value() const { return Udm::StringAttr(impl, meta_value); }

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::Attribute meta_type;
		Udm::StringAttr type() const { return Udm::StringAttr(impl, meta_type); }

		static Uml::AssociationRole meta_picmlProperty;
		Udm::CrossPointerAttr<PICML::Property> picmlProperty() const { return Udm::CrossPointerAttr<PICML::Property>(impl, meta_picmlProperty); }

		static Uml::CompositionParentRole meta_properties_ScenarioElement_parent;
		Udm::ParentAttr<cadena_scenario::ScenarioElement> properties_ScenarioElement_parent() const { return Udm::ParentAttr<cadena_scenario::ScenarioElement>(impl, meta_properties_ScenarioElement_parent); }

		Udm::ParentAttr<cadena_scenario::ScenarioElement> parent() const { return Udm::ParentAttr<cadena_scenario::ScenarioElement>(impl, Udm::NULLPARENTROLE); }
	};

	class  ScenarioElement :  public Udm::Object {
	 public:
		static Uml::Class meta;

		ScenarioElement() { }
		ScenarioElement(Udm::ObjectImpl *impl) : Object(impl) { }
		ScenarioElement(const ScenarioElement &master) : Object(master) { }
		static ScenarioElement Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ScenarioElement Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ScenarioElement CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ScenarioElement CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ScenarioElement> Instances() { return Udm::InstantiatedAttr<cadena_scenario::ScenarioElement>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ScenarioElement, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<cadena_scenario::ScenarioElement, Pred>(impl);}

		Udm::DerivedAttr<cadena_scenario::ScenarioElement> Derived() { return Udm::DerivedAttr<cadena_scenario::ScenarioElement>(impl);}
		template <class Pred> Udm::DerivedAttr<cadena_scenario::ScenarioElement, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<cadena_scenario::ScenarioElement, Pred>(impl);}

		Udm::ArchetypeAttr<cadena_scenario::ScenarioElement> Archetype() { return Udm::ArchetypeAttr<cadena_scenario::ScenarioElement>(impl);}

		static Uml::CompositionChildRole meta_properties;
		Udm::ChildrenAttr<cadena_scenario::Property> properties() const { return Udm::ChildrenAttr<cadena_scenario::Property>(impl, meta_properties); }
		 template <class Pred> Udm::ChildrenAttr<cadena_scenario::Property, Pred> properties_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::Property, Pred>(impl, meta_properties); }

		Udm::ChildrenAttr<cadena_scenario::Property> Property_kind_children() const { return Udm::ChildrenAttr<cadena_scenario::Property>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<cadena_scenario::Property, Pred> Property_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::Property, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class  ComponentInstance :  public ScenarioElement {
	 public:
		static Uml::Class meta;

		ComponentInstance() { }
		ComponentInstance(Udm::ObjectImpl *impl) : ScenarioElement(impl) { }
		ComponentInstance(const ComponentInstance &master) : ScenarioElement(master) { }
		static ComponentInstance Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentInstance Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentInstance CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentInstance CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentInstance> Instances() { return Udm::InstantiatedAttr<cadena_scenario::ComponentInstance>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentInstance, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<cadena_scenario::ComponentInstance, Pred>(impl);}

		Udm::DerivedAttr<cadena_scenario::ComponentInstance> Derived() { return Udm::DerivedAttr<cadena_scenario::ComponentInstance>(impl);}
		template <class Pred> Udm::DerivedAttr<cadena_scenario::ComponentInstance, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<cadena_scenario::ComponentInstance, Pred>(impl);}

		Udm::ArchetypeAttr<cadena_scenario::ComponentInstance> Archetype() { return Udm::ArchetypeAttr<cadena_scenario::ComponentInstance>(impl);}

		static Uml::Attribute meta_type;
		Udm::StringAttr type() const { return Udm::StringAttr(impl, meta_type); }

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::AssociationRole meta_picmlComponentDef;
		Udm::CrossPointerAttr<PICML::Component> picmlComponentDef() const { return Udm::CrossPointerAttr<PICML::Component>(impl, meta_picmlComponentDef); }

		static Uml::CompositionChildRole meta_ComponentPort_children;
		Udm::ChildrenAttr<cadena_scenario::ComponentPort> ComponentPort_children() const { return Udm::ChildrenAttr<cadena_scenario::ComponentPort>(impl, meta_ComponentPort_children); }
		 template <class Pred> Udm::ChildrenAttr<cadena_scenario::ComponentPort, Pred> ComponentPort_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::ComponentPort, Pred>(impl, meta_ComponentPort_children); }

		Udm::ChildrenAttr<cadena_scenario::EventSink> EventSink_kind_children() const { return Udm::ChildrenAttr<cadena_scenario::EventSink>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<cadena_scenario::EventSink, Pred> EventSink_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::EventSink, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<cadena_scenario::EventSource> EventSource_kind_children() const { return Udm::ChildrenAttr<cadena_scenario::EventSource>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<cadena_scenario::EventSource, Pred> EventSource_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::EventSource, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<cadena_scenario::Receptacle> Receptacle_kind_children() const { return Udm::ChildrenAttr<cadena_scenario::Receptacle>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<cadena_scenario::Receptacle, Pred> Receptacle_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::Receptacle, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<cadena_scenario::Facet> Facet_kind_children() const { return Udm::ChildrenAttr<cadena_scenario::Facet>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<cadena_scenario::Facet, Pred> Facet_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::Facet, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<cadena_scenario::ComponentPort> ComponentPort_kind_children() const { return Udm::ChildrenAttr<cadena_scenario::ComponentPort>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<cadena_scenario::ComponentPort, Pred> ComponentPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::ComponentPort, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_Scenario_parent;
		Udm::ParentAttr<cadena_scenario::Scenario> Scenario_parent() const { return Udm::ParentAttr<cadena_scenario::Scenario>(impl, meta_Scenario_parent); }

		Udm::ParentAttr<cadena_scenario::ScenarioElement> parent() const { return Udm::ParentAttr<cadena_scenario::ScenarioElement>(impl, Udm::NULLPARENTROLE); }
	};

	class  Scenario :  public ScenarioElement {
	 public:
		static Uml::Class meta;

		Scenario() { }
		Scenario(Udm::ObjectImpl *impl) : ScenarioElement(impl) { }
		Scenario(const Scenario &master) : ScenarioElement(master) { }
		static Scenario Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Scenario Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Scenario CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Scenario CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Scenario> Instances() { return Udm::InstantiatedAttr<cadena_scenario::Scenario>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Scenario, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<cadena_scenario::Scenario, Pred>(impl);}

		Udm::DerivedAttr<cadena_scenario::Scenario> Derived() { return Udm::DerivedAttr<cadena_scenario::Scenario>(impl);}
		template <class Pred> Udm::DerivedAttr<cadena_scenario::Scenario, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<cadena_scenario::Scenario, Pred>(impl);}

		Udm::ArchetypeAttr<cadena_scenario::Scenario> Archetype() { return Udm::ArchetypeAttr<cadena_scenario::Scenario>(impl);}

		static Uml::AssociationRole meta_picmlComponentAssembly;
		Udm::CrossAssocAttr<PICML::ComponentAssembly> picmlComponentAssembly() const { return Udm::CrossAssocAttr<PICML::ComponentAssembly>(impl, meta_picmlComponentAssembly); }
		template<class Pred> Udm::CrossAssocAttr<PICML::ComponentAssembly, Pred> picmlComponentAssembly_sorted(const Pred &) const { return Udm::CrossAssocAttr<PICML::ComponentAssembly, Pred>(impl, meta_picmlComponentAssembly); }

		static Uml::CompositionChildRole meta_ComponentInstance_children;
		Udm::ChildrenAttr<cadena_scenario::ComponentInstance> ComponentInstance_children() const { return Udm::ChildrenAttr<cadena_scenario::ComponentInstance>(impl, meta_ComponentInstance_children); }
		 template <class Pred> Udm::ChildrenAttr<cadena_scenario::ComponentInstance, Pred> ComponentInstance_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::ComponentInstance, Pred>(impl, meta_ComponentInstance_children); }

		static Uml::CompositionChildRole meta_PortConnection_children;
		Udm::ChildrenAttr<cadena_scenario::PortConnection> PortConnection_children() const { return Udm::ChildrenAttr<cadena_scenario::PortConnection>(impl, meta_PortConnection_children); }
		 template <class Pred> Udm::ChildrenAttr<cadena_scenario::PortConnection, Pred> PortConnection_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::PortConnection, Pred>(impl, meta_PortConnection_children); }

		Udm::ChildrenAttr<cadena_scenario::ESSConnection> ESSConnection_kind_children() const { return Udm::ChildrenAttr<cadena_scenario::ESSConnection>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<cadena_scenario::ESSConnection, Pred> ESSConnection_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::ESSConnection, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<cadena_scenario::FRConnection> FRConnection_kind_children() const { return Udm::ChildrenAttr<cadena_scenario::FRConnection>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<cadena_scenario::FRConnection, Pred> FRConnection_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::FRConnection, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<cadena_scenario::ComponentInstance> ComponentInstance_kind_children() const { return Udm::ChildrenAttr<cadena_scenario::ComponentInstance>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<cadena_scenario::ComponentInstance, Pred> ComponentInstance_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::ComponentInstance, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<cadena_scenario::ScenarioElement> ScenarioElement_kind_children() const { return Udm::ChildrenAttr<cadena_scenario::ScenarioElement>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<cadena_scenario::ScenarioElement, Pred> ScenarioElement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::ScenarioElement, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<cadena_scenario::PortConnection> PortConnection_kind_children() const { return Udm::ChildrenAttr<cadena_scenario::PortConnection>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<cadena_scenario::PortConnection, Pred> PortConnection_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<cadena_scenario::PortConnection, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_Scenarios_parent;
		Udm::ParentAttr<cadena_scenario::Scenarios> Scenarios_parent() const { return Udm::ParentAttr<cadena_scenario::Scenarios>(impl, meta_Scenarios_parent); }

		Udm::ParentAttr<cadena_scenario::Scenarios> parent() const { return Udm::ParentAttr<cadena_scenario::Scenarios>(impl, Udm::NULLPARENTROLE); }
	};

	class  PortConnection :  public ScenarioElement {
	 public:
		static Uml::Class meta;

		PortConnection() { }
		PortConnection(Udm::ObjectImpl *impl) : ScenarioElement(impl) { }
		PortConnection(const PortConnection &master) : ScenarioElement(master) { }
		static PortConnection Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PortConnection Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PortConnection CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PortConnection CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PortConnection> Instances() { return Udm::InstantiatedAttr<cadena_scenario::PortConnection>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PortConnection, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<cadena_scenario::PortConnection, Pred>(impl);}

		Udm::DerivedAttr<cadena_scenario::PortConnection> Derived() { return Udm::DerivedAttr<cadena_scenario::PortConnection>(impl);}
		template <class Pred> Udm::DerivedAttr<cadena_scenario::PortConnection, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<cadena_scenario::PortConnection, Pred>(impl);}

		Udm::ArchetypeAttr<cadena_scenario::PortConnection> Archetype() { return Udm::ArchetypeAttr<cadena_scenario::PortConnection>(impl);}

		static Uml::CompositionParentRole meta_Scenario_parent;
		Udm::ParentAttr<cadena_scenario::Scenario> Scenario_parent() const { return Udm::ParentAttr<cadena_scenario::Scenario>(impl, meta_Scenario_parent); }

		Udm::ParentAttr<cadena_scenario::ScenarioElement> parent() const { return Udm::ParentAttr<cadena_scenario::ScenarioElement>(impl, Udm::NULLPARENTROLE); }
	};

	class  ESSConnection :  public PortConnection {
	 public:
		static Uml::Class meta;

		ESSConnection() { }
		ESSConnection(Udm::ObjectImpl *impl) : PortConnection(impl) { }
		ESSConnection(const ESSConnection &master) : PortConnection(master) { }
		static ESSConnection Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ESSConnection Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ESSConnection CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ESSConnection CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ESSConnection> Instances() { return Udm::InstantiatedAttr<cadena_scenario::ESSConnection>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ESSConnection, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<cadena_scenario::ESSConnection, Pred>(impl);}

		Udm::DerivedAttr<cadena_scenario::ESSConnection> Derived() { return Udm::DerivedAttr<cadena_scenario::ESSConnection>(impl);}
		template <class Pred> Udm::DerivedAttr<cadena_scenario::ESSConnection, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<cadena_scenario::ESSConnection, Pred>(impl);}

		Udm::ArchetypeAttr<cadena_scenario::ESSConnection> Archetype() { return Udm::ArchetypeAttr<cadena_scenario::ESSConnection>(impl);}

		static Uml::Attribute meta_ConnectorID;
		Udm::StringAttr ConnectorID() const { return Udm::StringAttr(impl, meta_ConnectorID); }

		static Uml::AssociationRole meta_sources;
		Udm::AssocAttr<cadena_scenario::EventSource> sources() const { return Udm::AssocAttr<cadena_scenario::EventSource>(impl, meta_sources); }
		template <class Pred> Udm::AssocAttr<cadena_scenario::EventSource, Pred > sources_sorted(const Pred &) const { return Udm::AssocAttr<cadena_scenario::EventSource, Pred>(impl, meta_sources); }

		static Uml::AssociationRole meta_sinks;
		Udm::AssocAttr<cadena_scenario::EventSink> sinks() const { return Udm::AssocAttr<cadena_scenario::EventSink>(impl, meta_sinks); }
		template <class Pred> Udm::AssocAttr<cadena_scenario::EventSink, Pred > sinks_sorted(const Pred &) const { return Udm::AssocAttr<cadena_scenario::EventSink, Pred>(impl, meta_sinks); }

		static Uml::AssociationRole meta_picmlPublishConnector;
		Udm::CrossAssocAttr<PICML::PublishConnector> picmlPublishConnector() const { return Udm::CrossAssocAttr<PICML::PublishConnector>(impl, meta_picmlPublishConnector); }
		template<class Pred> Udm::CrossAssocAttr<PICML::PublishConnector, Pred> picmlPublishConnector_sorted(const Pred &) const { return Udm::CrossAssocAttr<PICML::PublishConnector, Pred>(impl, meta_picmlPublishConnector); }

		static Uml::AssociationRole meta_picmlEmit;
		Udm::CrossPointerAttr<PICML::emit> picmlEmit() const { return Udm::CrossPointerAttr<PICML::emit>(impl, meta_picmlEmit); }

		Udm::ParentAttr<cadena_scenario::ScenarioElement> parent() const { return Udm::ParentAttr<cadena_scenario::ScenarioElement>(impl, Udm::NULLPARENTROLE); }
	};

	class  FRConnection :  public PortConnection {
	 public:
		static Uml::Class meta;

		FRConnection() { }
		FRConnection(Udm::ObjectImpl *impl) : PortConnection(impl) { }
		FRConnection(const FRConnection &master) : PortConnection(master) { }
		static FRConnection Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static FRConnection Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		FRConnection CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		FRConnection CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<FRConnection> Instances() { return Udm::InstantiatedAttr<cadena_scenario::FRConnection>(impl);}
		template <class Pred> Udm::InstantiatedAttr<FRConnection, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<cadena_scenario::FRConnection, Pred>(impl);}

		Udm::DerivedAttr<cadena_scenario::FRConnection> Derived() { return Udm::DerivedAttr<cadena_scenario::FRConnection>(impl);}
		template <class Pred> Udm::DerivedAttr<cadena_scenario::FRConnection, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<cadena_scenario::FRConnection, Pred>(impl);}

		Udm::ArchetypeAttr<cadena_scenario::FRConnection> Archetype() { return Udm::ArchetypeAttr<cadena_scenario::FRConnection>(impl);}

		static Uml::AssociationRole meta_picmlInvoke;
		Udm::CrossPointerAttr<PICML::invoke> picmlInvoke() const { return Udm::CrossPointerAttr<PICML::invoke>(impl, meta_picmlInvoke); }

		Udm::ParentAttr<cadena_scenario::ScenarioElement> parent() const { return Udm::ParentAttr<cadena_scenario::ScenarioElement>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_facets_end_;
		Udm::AssocEndAttr<cadena_scenario::Facet> facets_end() const { return Udm::AssocEndAttr<cadena_scenario::Facet>(impl, meta_facets_end_); }

		static Uml::AssociationRole meta_receptacles_end_;
		Udm::AssocEndAttr<cadena_scenario::Receptacle> receptacles_end() const { return Udm::AssocEndAttr<cadena_scenario::Receptacle>(impl, meta_receptacles_end_); }

	};

}

#endif //MOBIES_CADENA_SCENARIO_H
