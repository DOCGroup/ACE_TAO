// cpp(meta datanetwork format) source file PICMLCadena_Xchange_embedded_EIProperties.cpp generated from diagram PICMLCadena_Xchange_embedded_EIProperties
// generated on Mon Nov 15 23:04:57 2004

#include "PICMLCadena_Xchange_embedded_EIProperties.h"
#include "UmlExt.h"

#include "UdmStatic.h"

// cross-package metainformation header file
#include "Cadena2PICML.h"

namespace PICMLCadena_Xchange_embedded_EIProperties {

	Uml::Diagram umldiagram;

	Uml::Class Property::meta;
	Uml::Class RootContainer::meta;
	Uml::Class ConnectionSelector::meta;
	Uml::Attribute Property::meta_type;
	Uml::Attribute Property::meta_name;
	Uml::Attribute Property::meta_value;
	Uml::Attribute ConnectionSelector::meta_slaveComponentName;
	Uml::Attribute ConnectionSelector::meta_masterPortName;
	Uml::Attribute ConnectionSelector::meta_slavePortName;
	Uml::AssociationRole RootContainer::meta_componentAssembly;
	Uml::CompositionParentRole Property::meta_ConnectionSelector_parent;
	Uml::CompositionParentRole ConnectionSelector::meta_RootContainer_parent;
	Uml::CompositionChildRole RootContainer::meta_ConnectionSelector_children;
	Uml::CompositionChildRole ConnectionSelector::meta_Property_children;
	void Creates()
	{
		Property::meta = Uml::Class::Create(umldiagram);
		RootContainer::meta = Uml::Class::Create(umldiagram);
		ConnectionSelector::meta = Uml::Class::Create(umldiagram);
		Property::meta_type = Uml::Attribute::Create(Property::meta);
		Property::meta_name = Uml::Attribute::Create(Property::meta);
		Property::meta_value = Uml::Attribute::Create(Property::meta);
		ConnectionSelector::meta_slaveComponentName = Uml::Attribute::Create(ConnectionSelector::meta);
		ConnectionSelector::meta_masterPortName = Uml::Attribute::Create(ConnectionSelector::meta);
		ConnectionSelector::meta_slavePortName = Uml::Attribute::Create(ConnectionSelector::meta);
	}
	void InitClassesAttributes()
	{
		Property::meta.name() = "Property";
		Property::meta.isAbstract() = false;
		RootContainer::meta.name() = "RootContainer";
		RootContainer::meta.isAbstract() = false;
		ConnectionSelector::meta.name() = "ConnectionSelector";
		ConnectionSelector::meta.isAbstract() = false;
		
		Property::meta_type.name() = "type";
		Property::meta_type.type() = "String";
		Property::meta_type.min() = 1;
		Property::meta_type.max() = 1;
		Property::meta_type.nonpersistent() = false;
		Property::meta_type.registry() = false;
		Property::meta_type.ordered() = false;
		Property::meta_type.visibility() = "public";
		Property::meta_type.defvalue() = vector<string>();
		Property::meta_name.name() = "name";
		Property::meta_name.type() = "String";
		Property::meta_name.min() = 1;
		Property::meta_name.max() = 1;
		Property::meta_name.nonpersistent() = false;
		Property::meta_name.registry() = false;
		Property::meta_name.ordered() = false;
		Property::meta_name.visibility() = "public";
		Property::meta_name.defvalue() = vector<string>();
		Property::meta_value.name() = "value";
		Property::meta_value.type() = "String";
		Property::meta_value.min() = 1;
		Property::meta_value.max() = 1;
		Property::meta_value.nonpersistent() = false;
		Property::meta_value.registry() = false;
		Property::meta_value.ordered() = false;
		Property::meta_value.visibility() = "public";
		Property::meta_value.defvalue() = vector<string>();
		ConnectionSelector::meta_slaveComponentName.name() = "slaveComponentName";
		ConnectionSelector::meta_slaveComponentName.type() = "String";
		ConnectionSelector::meta_slaveComponentName.min() = 1;
		ConnectionSelector::meta_slaveComponentName.max() = 1;
		ConnectionSelector::meta_slaveComponentName.nonpersistent() = false;
		ConnectionSelector::meta_slaveComponentName.registry() = false;
		ConnectionSelector::meta_slaveComponentName.ordered() = false;
		ConnectionSelector::meta_slaveComponentName.visibility() = "public";
		ConnectionSelector::meta_slaveComponentName.defvalue() = vector<string>();
		ConnectionSelector::meta_masterPortName.name() = "masterPortName";
		ConnectionSelector::meta_masterPortName.type() = "String";
		ConnectionSelector::meta_masterPortName.min() = 1;
		ConnectionSelector::meta_masterPortName.max() = 1;
		ConnectionSelector::meta_masterPortName.nonpersistent() = false;
		ConnectionSelector::meta_masterPortName.registry() = false;
		ConnectionSelector::meta_masterPortName.ordered() = false;
		ConnectionSelector::meta_masterPortName.visibility() = "public";
		ConnectionSelector::meta_masterPortName.defvalue() = vector<string>();
		ConnectionSelector::meta_slavePortName.name() = "slavePortName";
		ConnectionSelector::meta_slavePortName.type() = "String";
		ConnectionSelector::meta_slavePortName.min() = 1;
		ConnectionSelector::meta_slavePortName.max() = 1;
		ConnectionSelector::meta_slavePortName.nonpersistent() = false;
		ConnectionSelector::meta_slavePortName.registry() = false;
		ConnectionSelector::meta_slavePortName.ordered() = false;
		ConnectionSelector::meta_slavePortName.visibility() = "public";
		ConnectionSelector::meta_slavePortName.defvalue() = vector<string>();
		
	}
	void InitComposition0()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Property::meta_ConnectionSelector_parent = Uml::CompositionParentRole::Create(comp);
		Property::meta_ConnectionSelector_parent.name() = "ConnectionSelector_parent";
		Property::meta_ConnectionSelector_parent.isNavigable() = true;
		Property::meta_ConnectionSelector_parent.target() = ConnectionSelector::meta;
		ConnectionSelector::meta_Property_children = Uml::CompositionChildRole::Create(comp);
		ConnectionSelector::meta_Property_children.name() = "Property";
		ConnectionSelector::meta_Property_children.min() = 0;
		ConnectionSelector::meta_Property_children.max() = -1;
		ConnectionSelector::meta_Property_children.isNavigable() = true;
		ConnectionSelector::meta_Property_children.target() = Property::meta;
	}
	void InitComposition1()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ConnectionSelector::meta_RootContainer_parent = Uml::CompositionParentRole::Create(comp);
		ConnectionSelector::meta_RootContainer_parent.name() = "RootContainer_parent";
		ConnectionSelector::meta_RootContainer_parent.isNavigable() = true;
		ConnectionSelector::meta_RootContainer_parent.target() = RootContainer::meta;
		RootContainer::meta_ConnectionSelector_children = Uml::CompositionChildRole::Create(comp);
		RootContainer::meta_ConnectionSelector_children.name() = "ConnectionSelector";
		RootContainer::meta_ConnectionSelector_children.min() = 0;
		RootContainer::meta_ConnectionSelector_children.max() = -1;
		RootContainer::meta_ConnectionSelector_children.isNavigable() = true;
		RootContainer::meta_ConnectionSelector_children.target() = ConnectionSelector::meta;
	}
	 void Initialize()
	{
		static bool first = true;
		if(!first) return;
		first = false;
		Uml::Initialize();

		Cadena2PICML::Initialize();

		ASSERT( umldiagram == Udm::null );
		UdmStatic::StaticDataNetwork * meta_dn = new UdmStatic::StaticDataNetwork(Uml::diagram);
		meta_dn->CreateNew("PICMLCadena_Xchange_embedded_EIProperties.mem", "", Uml::Diagram::meta, Udm::CHANGES_LOST_DEFAULT);
		umldiagram = Uml::Diagram::Cast(meta_dn->GetRootObject());
		umldiagram.name() ="PICMLCadena_Xchange_embedded_EIProperties";
		umldiagram.version() ="1.00";
		Creates();
		InitClassesAttributes();
		Uml::InitDiagram(umldiagram, "PICMLCadena_Xchange_embedded_EIProperties", "1.00");

		RootContainer::meta_componentAssembly = Cadena2PICML::RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_componentAssembly;
		InitComposition0();
		InitComposition1();
		
	}
	void Initialize(const Uml::Diagram & dgr)
	{
		umldiagram = dgr;
		Uml::SetClass(Property::meta, umldiagram, "Property");
		Uml::SetAttribute(Property::meta_type,Property::meta, "type");
		Uml::SetAttribute(Property::meta_name,Property::meta, "name");
		Uml::SetAttribute(Property::meta_value,Property::meta, "value");
		Uml::SetClass(RootContainer::meta, umldiagram, "RootContainer");
		Uml::SetClass(ConnectionSelector::meta, umldiagram, "ConnectionSelector");
		Uml::SetAttribute(ConnectionSelector::meta_slaveComponentName,ConnectionSelector::meta, "slaveComponentName");
		Uml::SetAttribute(ConnectionSelector::meta_masterPortName,ConnectionSelector::meta, "masterPortName");
		Uml::SetAttribute(ConnectionSelector::meta_slavePortName,ConnectionSelector::meta, "slavePortName");
		//composition child roles 
		Uml::SetParentRole(Property::meta_ConnectionSelector_parent,Property::meta,ConnectionSelector::meta, "", "");
		Uml::SetParentRole(ConnectionSelector::meta_RootContainer_parent,ConnectionSelector::meta,RootContainer::meta, "", "");
		// composition parentroles 
		Uml::SetChildRole(RootContainer::meta_ConnectionSelector_children,RootContainer::meta,ConnectionSelector::meta, "", "");
		Uml::SetChildRole(ConnectionSelector::meta_Property_children,ConnectionSelector::meta,Property::meta, "", "");
		// Association roles 



		
	}
	 Udm::UdmDiagram diagram = { &umldiagram, Initialize };
	static struct _regClass
	{
		_regClass()
		{
			Udm::MetaDepository::StoreDiagram("PICMLCadena_Xchange_embedded_EIProperties", diagram);
		};
		~_regClass()
		{
			Udm::MetaDepository::RemoveDiagram("PICMLCadena_Xchange_embedded_EIProperties");
		};
	} __regUnUsed;

}
// END PICMLCadena_Xchange_embedded_EIProperties.cpp
