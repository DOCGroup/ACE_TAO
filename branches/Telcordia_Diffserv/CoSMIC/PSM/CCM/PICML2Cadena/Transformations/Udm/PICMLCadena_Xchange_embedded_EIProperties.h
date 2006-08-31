#ifndef MOBIES_PICMLCADENA_XCHANGE_EMBEDDED_EIPROPERTIES_H
#define MOBIES_PICMLCADENA_XCHANGE_EMBEDDED_EIPROPERTIES_H
// header file PICMLCadena_Xchange_embedded_EIProperties.h generated from diagram PICMLCadena_Xchange_embedded_EIProperties
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
namespace cadena_scenario
{ 
	 class  ESSConnection;
	 class  FRConnection;
	 class  EventSink;
	 class  EventSource;
	 class  Receptacle;
	 class  Facet;
	 class  ComponentPort;
	 class  Property;
	 class  ComponentInstance;
	 class  Scenario;
};


namespace PICMLCadena_Xchange_embedded_EIProperties {

	class  Property;
	class  RootContainer;
	class  ConnectionSelector;

	  void Initialize();

	  void Initialize(const Uml::Diagram &dgr);
	extern  Udm::UdmDiagram diagram;

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

		Udm::InstantiatedAttr<Property> Instances() { return Udm::InstantiatedAttr<PICMLCadena_Xchange_embedded_EIProperties::Property>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Property, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICMLCadena_Xchange_embedded_EIProperties::Property, Pred>(impl);}

		Udm::DerivedAttr<PICMLCadena_Xchange_embedded_EIProperties::Property> Derived() { return Udm::DerivedAttr<PICMLCadena_Xchange_embedded_EIProperties::Property>(impl);}
		template <class Pred> Udm::DerivedAttr<PICMLCadena_Xchange_embedded_EIProperties::Property, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICMLCadena_Xchange_embedded_EIProperties::Property, Pred>(impl);}

		Udm::ArchetypeAttr<PICMLCadena_Xchange_embedded_EIProperties::Property> Archetype() { return Udm::ArchetypeAttr<PICMLCadena_Xchange_embedded_EIProperties::Property>(impl);}

		static Uml::Attribute meta_type;
		Udm::StringAttr type() const { return Udm::StringAttr(impl, meta_type); }

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::Attribute meta_value;
		Udm::StringAttr value() const { return Udm::StringAttr(impl, meta_value); }

		static Uml::CompositionParentRole meta_ConnectionSelector_parent;
		Udm::ParentAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector> ConnectionSelector_parent() const { return Udm::ParentAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector>(impl, meta_ConnectionSelector_parent); }

		Udm::ParentAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector> parent() const { return Udm::ParentAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector>(impl, Udm::NULLPARENTROLE); }
	};

	class  RootContainer :  public Udm::Object {
	 public:
		static Uml::Class meta;

		RootContainer() { }
		RootContainer(Udm::ObjectImpl *impl) : Object(impl) { }
		RootContainer(const RootContainer &master) : Object(master) { }
		static RootContainer Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static RootContainer Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		RootContainer CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		RootContainer CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<RootContainer> Instances() { return Udm::InstantiatedAttr<PICMLCadena_Xchange_embedded_EIProperties::RootContainer>(impl);}
		template <class Pred> Udm::InstantiatedAttr<RootContainer, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICMLCadena_Xchange_embedded_EIProperties::RootContainer, Pred>(impl);}

		Udm::DerivedAttr<PICMLCadena_Xchange_embedded_EIProperties::RootContainer> Derived() { return Udm::DerivedAttr<PICMLCadena_Xchange_embedded_EIProperties::RootContainer>(impl);}
		template <class Pred> Udm::DerivedAttr<PICMLCadena_Xchange_embedded_EIProperties::RootContainer, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICMLCadena_Xchange_embedded_EIProperties::RootContainer, Pred>(impl);}

		Udm::ArchetypeAttr<PICMLCadena_Xchange_embedded_EIProperties::RootContainer> Archetype() { return Udm::ArchetypeAttr<PICMLCadena_Xchange_embedded_EIProperties::RootContainer>(impl);}

		static Uml::AssociationRole meta_componentAssembly;
		Udm::CrossPointerAttr<PICML::ComponentAssembly> componentAssembly() const { return Udm::CrossPointerAttr<PICML::ComponentAssembly>(impl, meta_componentAssembly); }

		static Uml::CompositionChildRole meta_ConnectionSelector_children;
		Udm::ChildrenAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector> ConnectionSelector_children() const { return Udm::ChildrenAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector>(impl, meta_ConnectionSelector_children); }
		 template <class Pred> Udm::ChildrenAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector, Pred> ConnectionSelector_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector, Pred>(impl, meta_ConnectionSelector_children); }

		Udm::ChildrenAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector> ConnectionSelector_kind_children() const { return Udm::ChildrenAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector, Pred> ConnectionSelector_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class  ConnectionSelector :  public Udm::Object {
	 public:
		static Uml::Class meta;

		ConnectionSelector() { }
		ConnectionSelector(Udm::ObjectImpl *impl) : Object(impl) { }
		ConnectionSelector(const ConnectionSelector &master) : Object(master) { }
		static ConnectionSelector Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ConnectionSelector Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ConnectionSelector CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ConnectionSelector CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ConnectionSelector> Instances() { return Udm::InstantiatedAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ConnectionSelector, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector, Pred>(impl);}

		Udm::DerivedAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector> Derived() { return Udm::DerivedAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector>(impl);}
		template <class Pred> Udm::DerivedAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector, Pred>(impl);}

		Udm::ArchetypeAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector> Archetype() { return Udm::ArchetypeAttr<PICMLCadena_Xchange_embedded_EIProperties::ConnectionSelector>(impl);}

		static Uml::Attribute meta_slaveComponentName;
		Udm::StringAttr slaveComponentName() const { return Udm::StringAttr(impl, meta_slaveComponentName); }

		static Uml::Attribute meta_masterPortName;
		Udm::StringAttr masterPortName() const { return Udm::StringAttr(impl, meta_masterPortName); }

		static Uml::Attribute meta_slavePortName;
		Udm::StringAttr slavePortName() const { return Udm::StringAttr(impl, meta_slavePortName); }

		static Uml::CompositionChildRole meta_Property_children;
		Udm::ChildrenAttr<PICMLCadena_Xchange_embedded_EIProperties::Property> Property_children() const { return Udm::ChildrenAttr<PICMLCadena_Xchange_embedded_EIProperties::Property>(impl, meta_Property_children); }
		 template <class Pred> Udm::ChildrenAttr<PICMLCadena_Xchange_embedded_EIProperties::Property, Pred> Property_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICMLCadena_Xchange_embedded_EIProperties::Property, Pred>(impl, meta_Property_children); }

		Udm::ChildrenAttr<PICMLCadena_Xchange_embedded_EIProperties::Property> Property_kind_children() const { return Udm::ChildrenAttr<PICMLCadena_Xchange_embedded_EIProperties::Property>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICMLCadena_Xchange_embedded_EIProperties::Property, Pred> Property_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICMLCadena_Xchange_embedded_EIProperties::Property, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_RootContainer_parent;
		Udm::ParentAttr<PICMLCadena_Xchange_embedded_EIProperties::RootContainer> RootContainer_parent() const { return Udm::ParentAttr<PICMLCadena_Xchange_embedded_EIProperties::RootContainer>(impl, meta_RootContainer_parent); }

		Udm::ParentAttr<PICMLCadena_Xchange_embedded_EIProperties::RootContainer> parent() const { return Udm::ParentAttr<PICMLCadena_Xchange_embedded_EIProperties::RootContainer>(impl, Udm::NULLPARENTROLE); }
	};

}

#endif //MOBIES_PICMLCADENA_XCHANGE_EMBEDDED_EIPROPERTIES_H
