// cpp(meta datanetwork format) source file PICML2Cadena.cpp generated from diagram PICML2Cadena
// generated on Mon Nov 15 02:46:30 2004

#include "PICML2Cadena.h"
#include "UmlExt.h"

#include "UdmStatic.h"

namespace PICML2Cadena {

	Uml::Diagram umldiagram;

	Uml::Class PublishConnector_cross_ph_PICML::meta;
	Uml::Class invoke_cross_ph_PICML::meta;
	Uml::Class emit_cross_ph_PICML::meta;
	Uml::Class ComponentAssembly_cross_ph_PICML::meta;
	Uml::Class OutEventPort_cross_ph_PICML::meta;
	Uml::Class ProvidedRequestPort_cross_ph_PICML::meta;
	Uml::Class RequiredRequestPort_cross_ph_PICML::meta;
	Uml::Class InEventPort_cross_ph_PICML::meta;
	Uml::Class Port_cross_ph_PICML::meta;
	Uml::Class Component_cross_ph_PICML::meta;
	Uml::Class RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta;
	Uml::Class ESSConnection_cross_ph_cadena_scenario::meta;
	Uml::Class FRConnection_cross_ph_cadena_scenario::meta;
	Uml::Class EventSink_cross_ph_cadena_scenario::meta;
	Uml::Class EventSource_cross_ph_cadena_scenario::meta;
	Uml::Class Receptacle_cross_ph_cadena_scenario::meta;
	Uml::Class Facet_cross_ph_cadena_scenario::meta;
	Uml::Class ComponentPort_cross_ph_cadena_scenario::meta;
	Uml::Class ComponentInstance_cross_ph_cadena_scenario::meta;
	Uml::Class _gen_cont::meta;
	Uml::Attribute PublishConnector_cross_ph_PICML::meta_rem_id;
	Uml::Attribute PublishConnector_cross_ph_PICML::meta_rem_sysname;
	Uml::Attribute invoke_cross_ph_PICML::meta_rem_id;
	Uml::Attribute invoke_cross_ph_PICML::meta_rem_sysname;
	Uml::Attribute emit_cross_ph_PICML::meta_rem_id;
	Uml::Attribute emit_cross_ph_PICML::meta_rem_sysname;
	Uml::Attribute ComponentAssembly_cross_ph_PICML::meta_rem_id;
	Uml::Attribute ComponentAssembly_cross_ph_PICML::meta_rem_sysname;
	Uml::Attribute Port_cross_ph_PICML::meta_rem_id;
	Uml::Attribute Port_cross_ph_PICML::meta_rem_sysname;
	Uml::Attribute Component_cross_ph_PICML::meta_rem_id;
	Uml::Attribute Component_cross_ph_PICML::meta_rem_sysname;
	Uml::Attribute RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_id;
	Uml::Attribute RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_sysname;
	Uml::Attribute ESSConnection_cross_ph_cadena_scenario::meta_rem_id;
	Uml::Attribute ESSConnection_cross_ph_cadena_scenario::meta_rem_sysname;
	Uml::Attribute FRConnection_cross_ph_cadena_scenario::meta_rem_id;
	Uml::Attribute FRConnection_cross_ph_cadena_scenario::meta_rem_sysname;
	Uml::Attribute ComponentPort_cross_ph_cadena_scenario::meta_rem_id;
	Uml::Attribute ComponentPort_cross_ph_cadena_scenario::meta_rem_sysname;
	Uml::Attribute ComponentInstance_cross_ph_cadena_scenario::meta_rem_id;
	Uml::Attribute ComponentInstance_cross_ph_cadena_scenario::meta_rem_sysname;
	Uml::AssociationRole PublishConnector_cross_ph_PICML::meta_cadenaESSConnection;
	Uml::AssociationRole invoke_cross_ph_PICML::meta_cadenaInvoke;
	Uml::AssociationRole emit_cross_ph_PICML::meta_cadenaEmit;
	Uml::AssociationRole ComponentAssembly_cross_ph_PICML::meta_rootContainer;
	Uml::AssociationRole Port_cross_ph_PICML::meta_cadenaPort;
	Uml::AssociationRole Component_cross_ph_PICML::meta_cadenaComponentInstance;
	Uml::AssociationRole RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_componentAssembly;
	Uml::AssociationRole ESSConnection_cross_ph_cadena_scenario::meta_picmlPublishConnector;
	Uml::AssociationRole ESSConnection_cross_ph_cadena_scenario::meta_picmlEmit;
	Uml::AssociationRole FRConnection_cross_ph_cadena_scenario::meta_picmlInvoke;
	Uml::AssociationRole ComponentPort_cross_ph_cadena_scenario::meta_picmlPort;
	Uml::AssociationRole ComponentInstance_cross_ph_cadena_scenario::meta_picmlComponentDef;
	Uml::CompositionParentRole PublishConnector_cross_ph_PICML::meta__gen_cont_parent;
	Uml::CompositionParentRole invoke_cross_ph_PICML::meta__gen_cont_parent;
	Uml::CompositionParentRole emit_cross_ph_PICML::meta__gen_cont_parent;
	Uml::CompositionParentRole ComponentAssembly_cross_ph_PICML::meta__gen_cont_parent;
	Uml::CompositionParentRole Port_cross_ph_PICML::meta__gen_cont_parent;
	Uml::CompositionParentRole Component_cross_ph_PICML::meta__gen_cont_parent;
	Uml::CompositionParentRole RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta__gen_cont_parent;
	Uml::CompositionParentRole ESSConnection_cross_ph_cadena_scenario::meta__gen_cont_parent;
	Uml::CompositionParentRole FRConnection_cross_ph_cadena_scenario::meta__gen_cont_parent;
	Uml::CompositionParentRole ComponentPort_cross_ph_cadena_scenario::meta__gen_cont_parent;
	Uml::CompositionParentRole ComponentInstance_cross_ph_cadena_scenario::meta__gen_cont_parent;
	Uml::CompositionChildRole _gen_cont::meta_PublishConnector_cross_ph_PICML_children;
	Uml::CompositionChildRole _gen_cont::meta_invoke_cross_ph_PICML_children;
	Uml::CompositionChildRole _gen_cont::meta_emit_cross_ph_PICML_children;
	Uml::CompositionChildRole _gen_cont::meta_ComponentAssembly_cross_ph_PICML_children;
	Uml::CompositionChildRole _gen_cont::meta_Port_cross_ph_PICML_children;
	Uml::CompositionChildRole _gen_cont::meta_Component_cross_ph_PICML_children;
	Uml::CompositionChildRole _gen_cont::meta_RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties_children;
	Uml::CompositionChildRole _gen_cont::meta_ESSConnection_cross_ph_cadena_scenario_children;
	Uml::CompositionChildRole _gen_cont::meta_FRConnection_cross_ph_cadena_scenario_children;
	Uml::CompositionChildRole _gen_cont::meta_ComponentPort_cross_ph_cadena_scenario_children;
	Uml::CompositionChildRole _gen_cont::meta_ComponentInstance_cross_ph_cadena_scenario_children;
	void Creates()
	{
		PublishConnector_cross_ph_PICML::meta = Uml::Class::Create(umldiagram);
		invoke_cross_ph_PICML::meta = Uml::Class::Create(umldiagram);
		emit_cross_ph_PICML::meta = Uml::Class::Create(umldiagram);
		ComponentAssembly_cross_ph_PICML::meta = Uml::Class::Create(umldiagram);
		OutEventPort_cross_ph_PICML::meta = Uml::Class::Create(umldiagram);
		ProvidedRequestPort_cross_ph_PICML::meta = Uml::Class::Create(umldiagram);
		RequiredRequestPort_cross_ph_PICML::meta = Uml::Class::Create(umldiagram);
		InEventPort_cross_ph_PICML::meta = Uml::Class::Create(umldiagram);
		Port_cross_ph_PICML::meta = Uml::Class::Create(umldiagram);
		Component_cross_ph_PICML::meta = Uml::Class::Create(umldiagram);
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta = Uml::Class::Create(umldiagram);
		ESSConnection_cross_ph_cadena_scenario::meta = Uml::Class::Create(umldiagram);
		FRConnection_cross_ph_cadena_scenario::meta = Uml::Class::Create(umldiagram);
		EventSink_cross_ph_cadena_scenario::meta = Uml::Class::Create(umldiagram);
		EventSource_cross_ph_cadena_scenario::meta = Uml::Class::Create(umldiagram);
		Receptacle_cross_ph_cadena_scenario::meta = Uml::Class::Create(umldiagram);
		Facet_cross_ph_cadena_scenario::meta = Uml::Class::Create(umldiagram);
		ComponentPort_cross_ph_cadena_scenario::meta = Uml::Class::Create(umldiagram);
		ComponentInstance_cross_ph_cadena_scenario::meta = Uml::Class::Create(umldiagram);
		_gen_cont::meta = Uml::Class::Create(umldiagram);
		PublishConnector_cross_ph_PICML::meta_rem_id = Uml::Attribute::Create(PublishConnector_cross_ph_PICML::meta);
		PublishConnector_cross_ph_PICML::meta_rem_sysname = Uml::Attribute::Create(PublishConnector_cross_ph_PICML::meta);
		invoke_cross_ph_PICML::meta_rem_id = Uml::Attribute::Create(invoke_cross_ph_PICML::meta);
		invoke_cross_ph_PICML::meta_rem_sysname = Uml::Attribute::Create(invoke_cross_ph_PICML::meta);
		emit_cross_ph_PICML::meta_rem_id = Uml::Attribute::Create(emit_cross_ph_PICML::meta);
		emit_cross_ph_PICML::meta_rem_sysname = Uml::Attribute::Create(emit_cross_ph_PICML::meta);
		ComponentAssembly_cross_ph_PICML::meta_rem_id = Uml::Attribute::Create(ComponentAssembly_cross_ph_PICML::meta);
		ComponentAssembly_cross_ph_PICML::meta_rem_sysname = Uml::Attribute::Create(ComponentAssembly_cross_ph_PICML::meta);
		Port_cross_ph_PICML::meta_rem_id = Uml::Attribute::Create(Port_cross_ph_PICML::meta);
		Port_cross_ph_PICML::meta_rem_sysname = Uml::Attribute::Create(Port_cross_ph_PICML::meta);
		Component_cross_ph_PICML::meta_rem_id = Uml::Attribute::Create(Component_cross_ph_PICML::meta);
		Component_cross_ph_PICML::meta_rem_sysname = Uml::Attribute::Create(Component_cross_ph_PICML::meta);
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_id = Uml::Attribute::Create(RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta);
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_sysname = Uml::Attribute::Create(RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta);
		ESSConnection_cross_ph_cadena_scenario::meta_rem_id = Uml::Attribute::Create(ESSConnection_cross_ph_cadena_scenario::meta);
		ESSConnection_cross_ph_cadena_scenario::meta_rem_sysname = Uml::Attribute::Create(ESSConnection_cross_ph_cadena_scenario::meta);
		FRConnection_cross_ph_cadena_scenario::meta_rem_id = Uml::Attribute::Create(FRConnection_cross_ph_cadena_scenario::meta);
		FRConnection_cross_ph_cadena_scenario::meta_rem_sysname = Uml::Attribute::Create(FRConnection_cross_ph_cadena_scenario::meta);
		ComponentPort_cross_ph_cadena_scenario::meta_rem_id = Uml::Attribute::Create(ComponentPort_cross_ph_cadena_scenario::meta);
		ComponentPort_cross_ph_cadena_scenario::meta_rem_sysname = Uml::Attribute::Create(ComponentPort_cross_ph_cadena_scenario::meta);
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_id = Uml::Attribute::Create(ComponentInstance_cross_ph_cadena_scenario::meta);
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_sysname = Uml::Attribute::Create(ComponentInstance_cross_ph_cadena_scenario::meta);
	}
	void InitClassesAttributes()
	{
		PublishConnector_cross_ph_PICML::meta.name() = "PublishConnector_cross_ph_PICML";
		PublishConnector_cross_ph_PICML::meta.isAbstract() = false;
		PublishConnector_cross_ph_PICML::meta.from() = "PICML";
		invoke_cross_ph_PICML::meta.name() = "invoke_cross_ph_PICML";
		invoke_cross_ph_PICML::meta.isAbstract() = false;
		invoke_cross_ph_PICML::meta.from() = "PICML";
		emit_cross_ph_PICML::meta.name() = "emit_cross_ph_PICML";
		emit_cross_ph_PICML::meta.isAbstract() = false;
		emit_cross_ph_PICML::meta.from() = "PICML";
		ComponentAssembly_cross_ph_PICML::meta.name() = "ComponentAssembly_cross_ph_PICML";
		ComponentAssembly_cross_ph_PICML::meta.isAbstract() = false;
		ComponentAssembly_cross_ph_PICML::meta.from() = "PICML";
		OutEventPort_cross_ph_PICML::meta.name() = "OutEventPort_cross_ph_PICML";
		OutEventPort_cross_ph_PICML::meta.isAbstract() = false;
		OutEventPort_cross_ph_PICML::meta.from() = "PICML";
		ProvidedRequestPort_cross_ph_PICML::meta.name() = "ProvidedRequestPort_cross_ph_PICML";
		ProvidedRequestPort_cross_ph_PICML::meta.isAbstract() = false;
		ProvidedRequestPort_cross_ph_PICML::meta.from() = "PICML";
		RequiredRequestPort_cross_ph_PICML::meta.name() = "RequiredRequestPort_cross_ph_PICML";
		RequiredRequestPort_cross_ph_PICML::meta.isAbstract() = false;
		RequiredRequestPort_cross_ph_PICML::meta.from() = "PICML";
		InEventPort_cross_ph_PICML::meta.name() = "InEventPort_cross_ph_PICML";
		InEventPort_cross_ph_PICML::meta.isAbstract() = false;
		InEventPort_cross_ph_PICML::meta.from() = "PICML";
		Port_cross_ph_PICML::meta.name() = "Port_cross_ph_PICML";
		Port_cross_ph_PICML::meta.isAbstract() = false;
		Port_cross_ph_PICML::meta.from() = "PICML";
		Component_cross_ph_PICML::meta.name() = "Component_cross_ph_PICML";
		Component_cross_ph_PICML::meta.isAbstract() = false;
		Component_cross_ph_PICML::meta.from() = "PICML";
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta.name() = "RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties";
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta.isAbstract() = false;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta.from() = "PICMLCadena_Xchange_embedded_EIProperties";
		ESSConnection_cross_ph_cadena_scenario::meta.name() = "ESSConnection_cross_ph_cadena_scenario";
		ESSConnection_cross_ph_cadena_scenario::meta.isAbstract() = false;
		ESSConnection_cross_ph_cadena_scenario::meta.from() = "cadena_scenario";
		FRConnection_cross_ph_cadena_scenario::meta.name() = "FRConnection_cross_ph_cadena_scenario";
		FRConnection_cross_ph_cadena_scenario::meta.isAbstract() = false;
		FRConnection_cross_ph_cadena_scenario::meta.from() = "cadena_scenario";
		EventSink_cross_ph_cadena_scenario::meta.name() = "EventSink_cross_ph_cadena_scenario";
		EventSink_cross_ph_cadena_scenario::meta.isAbstract() = false;
		EventSink_cross_ph_cadena_scenario::meta.from() = "cadena_scenario";
		EventSource_cross_ph_cadena_scenario::meta.name() = "EventSource_cross_ph_cadena_scenario";
		EventSource_cross_ph_cadena_scenario::meta.isAbstract() = false;
		EventSource_cross_ph_cadena_scenario::meta.from() = "cadena_scenario";
		Receptacle_cross_ph_cadena_scenario::meta.name() = "Receptacle_cross_ph_cadena_scenario";
		Receptacle_cross_ph_cadena_scenario::meta.isAbstract() = false;
		Receptacle_cross_ph_cadena_scenario::meta.from() = "cadena_scenario";
		Facet_cross_ph_cadena_scenario::meta.name() = "Facet_cross_ph_cadena_scenario";
		Facet_cross_ph_cadena_scenario::meta.isAbstract() = false;
		Facet_cross_ph_cadena_scenario::meta.from() = "cadena_scenario";
		ComponentPort_cross_ph_cadena_scenario::meta.name() = "ComponentPort_cross_ph_cadena_scenario";
		ComponentPort_cross_ph_cadena_scenario::meta.isAbstract() = false;
		ComponentPort_cross_ph_cadena_scenario::meta.from() = "cadena_scenario";
		ComponentInstance_cross_ph_cadena_scenario::meta.name() = "ComponentInstance_cross_ph_cadena_scenario";
		ComponentInstance_cross_ph_cadena_scenario::meta.isAbstract() = false;
		ComponentInstance_cross_ph_cadena_scenario::meta.from() = "cadena_scenario";
		_gen_cont::meta.name() = "_gen_cont";
		_gen_cont::meta.isAbstract() = false;
		
		PublishConnector_cross_ph_PICML::meta_rem_id.name() = "rem_id";
		PublishConnector_cross_ph_PICML::meta_rem_id.type() = "Integer";
		PublishConnector_cross_ph_PICML::meta_rem_id.min() = 1;
		PublishConnector_cross_ph_PICML::meta_rem_id.max() = 1;
		PublishConnector_cross_ph_PICML::meta_rem_id.nonpersistent() = false;
		PublishConnector_cross_ph_PICML::meta_rem_id.registry() = false;
		PublishConnector_cross_ph_PICML::meta_rem_id.ordered() = false;
		PublishConnector_cross_ph_PICML::meta_rem_id.visibility() = "";
		PublishConnector_cross_ph_PICML::meta_rem_id.defvalue() = vector<string>();
		PublishConnector_cross_ph_PICML::meta_rem_sysname.name() = "rem_sysname";
		PublishConnector_cross_ph_PICML::meta_rem_sysname.type() = "String";
		PublishConnector_cross_ph_PICML::meta_rem_sysname.min() = 1;
		PublishConnector_cross_ph_PICML::meta_rem_sysname.max() = 1;
		PublishConnector_cross_ph_PICML::meta_rem_sysname.nonpersistent() = false;
		PublishConnector_cross_ph_PICML::meta_rem_sysname.registry() = false;
		PublishConnector_cross_ph_PICML::meta_rem_sysname.ordered() = false;
		PublishConnector_cross_ph_PICML::meta_rem_sysname.visibility() = "";
		PublishConnector_cross_ph_PICML::meta_rem_sysname.defvalue() = vector<string>();
		invoke_cross_ph_PICML::meta_rem_id.name() = "rem_id";
		invoke_cross_ph_PICML::meta_rem_id.type() = "Integer";
		invoke_cross_ph_PICML::meta_rem_id.min() = 1;
		invoke_cross_ph_PICML::meta_rem_id.max() = 1;
		invoke_cross_ph_PICML::meta_rem_id.nonpersistent() = false;
		invoke_cross_ph_PICML::meta_rem_id.registry() = false;
		invoke_cross_ph_PICML::meta_rem_id.ordered() = false;
		invoke_cross_ph_PICML::meta_rem_id.visibility() = "";
		invoke_cross_ph_PICML::meta_rem_id.defvalue() = vector<string>();
		invoke_cross_ph_PICML::meta_rem_sysname.name() = "rem_sysname";
		invoke_cross_ph_PICML::meta_rem_sysname.type() = "String";
		invoke_cross_ph_PICML::meta_rem_sysname.min() = 1;
		invoke_cross_ph_PICML::meta_rem_sysname.max() = 1;
		invoke_cross_ph_PICML::meta_rem_sysname.nonpersistent() = false;
		invoke_cross_ph_PICML::meta_rem_sysname.registry() = false;
		invoke_cross_ph_PICML::meta_rem_sysname.ordered() = false;
		invoke_cross_ph_PICML::meta_rem_sysname.visibility() = "";
		invoke_cross_ph_PICML::meta_rem_sysname.defvalue() = vector<string>();
		emit_cross_ph_PICML::meta_rem_id.name() = "rem_id";
		emit_cross_ph_PICML::meta_rem_id.type() = "Integer";
		emit_cross_ph_PICML::meta_rem_id.min() = 1;
		emit_cross_ph_PICML::meta_rem_id.max() = 1;
		emit_cross_ph_PICML::meta_rem_id.nonpersistent() = false;
		emit_cross_ph_PICML::meta_rem_id.registry() = false;
		emit_cross_ph_PICML::meta_rem_id.ordered() = false;
		emit_cross_ph_PICML::meta_rem_id.visibility() = "";
		emit_cross_ph_PICML::meta_rem_id.defvalue() = vector<string>();
		emit_cross_ph_PICML::meta_rem_sysname.name() = "rem_sysname";
		emit_cross_ph_PICML::meta_rem_sysname.type() = "String";
		emit_cross_ph_PICML::meta_rem_sysname.min() = 1;
		emit_cross_ph_PICML::meta_rem_sysname.max() = 1;
		emit_cross_ph_PICML::meta_rem_sysname.nonpersistent() = false;
		emit_cross_ph_PICML::meta_rem_sysname.registry() = false;
		emit_cross_ph_PICML::meta_rem_sysname.ordered() = false;
		emit_cross_ph_PICML::meta_rem_sysname.visibility() = "";
		emit_cross_ph_PICML::meta_rem_sysname.defvalue() = vector<string>();
		ComponentAssembly_cross_ph_PICML::meta_rem_id.name() = "rem_id";
		ComponentAssembly_cross_ph_PICML::meta_rem_id.type() = "Integer";
		ComponentAssembly_cross_ph_PICML::meta_rem_id.min() = 1;
		ComponentAssembly_cross_ph_PICML::meta_rem_id.max() = 1;
		ComponentAssembly_cross_ph_PICML::meta_rem_id.nonpersistent() = false;
		ComponentAssembly_cross_ph_PICML::meta_rem_id.registry() = false;
		ComponentAssembly_cross_ph_PICML::meta_rem_id.ordered() = false;
		ComponentAssembly_cross_ph_PICML::meta_rem_id.visibility() = "";
		ComponentAssembly_cross_ph_PICML::meta_rem_id.defvalue() = vector<string>();
		ComponentAssembly_cross_ph_PICML::meta_rem_sysname.name() = "rem_sysname";
		ComponentAssembly_cross_ph_PICML::meta_rem_sysname.type() = "String";
		ComponentAssembly_cross_ph_PICML::meta_rem_sysname.min() = 1;
		ComponentAssembly_cross_ph_PICML::meta_rem_sysname.max() = 1;
		ComponentAssembly_cross_ph_PICML::meta_rem_sysname.nonpersistent() = false;
		ComponentAssembly_cross_ph_PICML::meta_rem_sysname.registry() = false;
		ComponentAssembly_cross_ph_PICML::meta_rem_sysname.ordered() = false;
		ComponentAssembly_cross_ph_PICML::meta_rem_sysname.visibility() = "";
		ComponentAssembly_cross_ph_PICML::meta_rem_sysname.defvalue() = vector<string>();
		Port_cross_ph_PICML::meta_rem_id.name() = "rem_id";
		Port_cross_ph_PICML::meta_rem_id.type() = "Integer";
		Port_cross_ph_PICML::meta_rem_id.min() = 1;
		Port_cross_ph_PICML::meta_rem_id.max() = 1;
		Port_cross_ph_PICML::meta_rem_id.nonpersistent() = false;
		Port_cross_ph_PICML::meta_rem_id.registry() = false;
		Port_cross_ph_PICML::meta_rem_id.ordered() = false;
		Port_cross_ph_PICML::meta_rem_id.visibility() = "";
		Port_cross_ph_PICML::meta_rem_id.defvalue() = vector<string>();
		Port_cross_ph_PICML::meta_rem_sysname.name() = "rem_sysname";
		Port_cross_ph_PICML::meta_rem_sysname.type() = "String";
		Port_cross_ph_PICML::meta_rem_sysname.min() = 1;
		Port_cross_ph_PICML::meta_rem_sysname.max() = 1;
		Port_cross_ph_PICML::meta_rem_sysname.nonpersistent() = false;
		Port_cross_ph_PICML::meta_rem_sysname.registry() = false;
		Port_cross_ph_PICML::meta_rem_sysname.ordered() = false;
		Port_cross_ph_PICML::meta_rem_sysname.visibility() = "";
		Port_cross_ph_PICML::meta_rem_sysname.defvalue() = vector<string>();
		Component_cross_ph_PICML::meta_rem_id.name() = "rem_id";
		Component_cross_ph_PICML::meta_rem_id.type() = "Integer";
		Component_cross_ph_PICML::meta_rem_id.min() = 1;
		Component_cross_ph_PICML::meta_rem_id.max() = 1;
		Component_cross_ph_PICML::meta_rem_id.nonpersistent() = false;
		Component_cross_ph_PICML::meta_rem_id.registry() = false;
		Component_cross_ph_PICML::meta_rem_id.ordered() = false;
		Component_cross_ph_PICML::meta_rem_id.visibility() = "";
		Component_cross_ph_PICML::meta_rem_id.defvalue() = vector<string>();
		Component_cross_ph_PICML::meta_rem_sysname.name() = "rem_sysname";
		Component_cross_ph_PICML::meta_rem_sysname.type() = "String";
		Component_cross_ph_PICML::meta_rem_sysname.min() = 1;
		Component_cross_ph_PICML::meta_rem_sysname.max() = 1;
		Component_cross_ph_PICML::meta_rem_sysname.nonpersistent() = false;
		Component_cross_ph_PICML::meta_rem_sysname.registry() = false;
		Component_cross_ph_PICML::meta_rem_sysname.ordered() = false;
		Component_cross_ph_PICML::meta_rem_sysname.visibility() = "";
		Component_cross_ph_PICML::meta_rem_sysname.defvalue() = vector<string>();
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_id.name() = "rem_id";
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_id.type() = "Integer";
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_id.min() = 1;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_id.max() = 1;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_id.nonpersistent() = false;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_id.registry() = false;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_id.ordered() = false;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_id.visibility() = "";
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_id.defvalue() = vector<string>();
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_sysname.name() = "rem_sysname";
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_sysname.type() = "String";
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_sysname.min() = 1;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_sysname.max() = 1;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_sysname.nonpersistent() = false;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_sysname.registry() = false;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_sysname.ordered() = false;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_sysname.visibility() = "";
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_sysname.defvalue() = vector<string>();
		ESSConnection_cross_ph_cadena_scenario::meta_rem_id.name() = "rem_id";
		ESSConnection_cross_ph_cadena_scenario::meta_rem_id.type() = "Integer";
		ESSConnection_cross_ph_cadena_scenario::meta_rem_id.min() = 1;
		ESSConnection_cross_ph_cadena_scenario::meta_rem_id.max() = 1;
		ESSConnection_cross_ph_cadena_scenario::meta_rem_id.nonpersistent() = false;
		ESSConnection_cross_ph_cadena_scenario::meta_rem_id.registry() = false;
		ESSConnection_cross_ph_cadena_scenario::meta_rem_id.ordered() = false;
		ESSConnection_cross_ph_cadena_scenario::meta_rem_id.visibility() = "";
		ESSConnection_cross_ph_cadena_scenario::meta_rem_id.defvalue() = vector<string>();
		ESSConnection_cross_ph_cadena_scenario::meta_rem_sysname.name() = "rem_sysname";
		ESSConnection_cross_ph_cadena_scenario::meta_rem_sysname.type() = "String";
		ESSConnection_cross_ph_cadena_scenario::meta_rem_sysname.min() = 1;
		ESSConnection_cross_ph_cadena_scenario::meta_rem_sysname.max() = 1;
		ESSConnection_cross_ph_cadena_scenario::meta_rem_sysname.nonpersistent() = false;
		ESSConnection_cross_ph_cadena_scenario::meta_rem_sysname.registry() = false;
		ESSConnection_cross_ph_cadena_scenario::meta_rem_sysname.ordered() = false;
		ESSConnection_cross_ph_cadena_scenario::meta_rem_sysname.visibility() = "";
		ESSConnection_cross_ph_cadena_scenario::meta_rem_sysname.defvalue() = vector<string>();
		FRConnection_cross_ph_cadena_scenario::meta_rem_id.name() = "rem_id";
		FRConnection_cross_ph_cadena_scenario::meta_rem_id.type() = "Integer";
		FRConnection_cross_ph_cadena_scenario::meta_rem_id.min() = 1;
		FRConnection_cross_ph_cadena_scenario::meta_rem_id.max() = 1;
		FRConnection_cross_ph_cadena_scenario::meta_rem_id.nonpersistent() = false;
		FRConnection_cross_ph_cadena_scenario::meta_rem_id.registry() = false;
		FRConnection_cross_ph_cadena_scenario::meta_rem_id.ordered() = false;
		FRConnection_cross_ph_cadena_scenario::meta_rem_id.visibility() = "";
		FRConnection_cross_ph_cadena_scenario::meta_rem_id.defvalue() = vector<string>();
		FRConnection_cross_ph_cadena_scenario::meta_rem_sysname.name() = "rem_sysname";
		FRConnection_cross_ph_cadena_scenario::meta_rem_sysname.type() = "String";
		FRConnection_cross_ph_cadena_scenario::meta_rem_sysname.min() = 1;
		FRConnection_cross_ph_cadena_scenario::meta_rem_sysname.max() = 1;
		FRConnection_cross_ph_cadena_scenario::meta_rem_sysname.nonpersistent() = false;
		FRConnection_cross_ph_cadena_scenario::meta_rem_sysname.registry() = false;
		FRConnection_cross_ph_cadena_scenario::meta_rem_sysname.ordered() = false;
		FRConnection_cross_ph_cadena_scenario::meta_rem_sysname.visibility() = "";
		FRConnection_cross_ph_cadena_scenario::meta_rem_sysname.defvalue() = vector<string>();
		ComponentPort_cross_ph_cadena_scenario::meta_rem_id.name() = "rem_id";
		ComponentPort_cross_ph_cadena_scenario::meta_rem_id.type() = "Integer";
		ComponentPort_cross_ph_cadena_scenario::meta_rem_id.min() = 1;
		ComponentPort_cross_ph_cadena_scenario::meta_rem_id.max() = 1;
		ComponentPort_cross_ph_cadena_scenario::meta_rem_id.nonpersistent() = false;
		ComponentPort_cross_ph_cadena_scenario::meta_rem_id.registry() = false;
		ComponentPort_cross_ph_cadena_scenario::meta_rem_id.ordered() = false;
		ComponentPort_cross_ph_cadena_scenario::meta_rem_id.visibility() = "";
		ComponentPort_cross_ph_cadena_scenario::meta_rem_id.defvalue() = vector<string>();
		ComponentPort_cross_ph_cadena_scenario::meta_rem_sysname.name() = "rem_sysname";
		ComponentPort_cross_ph_cadena_scenario::meta_rem_sysname.type() = "String";
		ComponentPort_cross_ph_cadena_scenario::meta_rem_sysname.min() = 1;
		ComponentPort_cross_ph_cadena_scenario::meta_rem_sysname.max() = 1;
		ComponentPort_cross_ph_cadena_scenario::meta_rem_sysname.nonpersistent() = false;
		ComponentPort_cross_ph_cadena_scenario::meta_rem_sysname.registry() = false;
		ComponentPort_cross_ph_cadena_scenario::meta_rem_sysname.ordered() = false;
		ComponentPort_cross_ph_cadena_scenario::meta_rem_sysname.visibility() = "";
		ComponentPort_cross_ph_cadena_scenario::meta_rem_sysname.defvalue() = vector<string>();
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_id.name() = "rem_id";
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_id.type() = "Integer";
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_id.min() = 1;
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_id.max() = 1;
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_id.nonpersistent() = false;
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_id.registry() = false;
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_id.ordered() = false;
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_id.visibility() = "";
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_id.defvalue() = vector<string>();
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_sysname.name() = "rem_sysname";
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_sysname.type() = "String";
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_sysname.min() = 1;
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_sysname.max() = 1;
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_sysname.nonpersistent() = false;
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_sysname.registry() = false;
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_sysname.ordered() = false;
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_sysname.visibility() = "";
		ComponentInstance_cross_ph_cadena_scenario::meta_rem_sysname.defvalue() = vector<string>();
		
	}
	void InitAssociation0()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ESSConnection_cross_ph_cadena_scenario::meta_picmlPublishConnector = Uml::AssociationRole::Create(ass);
		ESSConnection_cross_ph_cadena_scenario::meta_picmlPublishConnector.name() = "picmlPublishConnector";
		ESSConnection_cross_ph_cadena_scenario::meta_picmlPublishConnector.min() = 0;
		ESSConnection_cross_ph_cadena_scenario::meta_picmlPublishConnector.max() = -1;
		ESSConnection_cross_ph_cadena_scenario::meta_picmlPublishConnector.isNavigable() = true;
		ESSConnection_cross_ph_cadena_scenario::meta_picmlPublishConnector.isPrimary() = false;
		ESSConnection_cross_ph_cadena_scenario::meta_picmlPublishConnector.target() = PublishConnector_cross_ph_PICML::meta;
		PublishConnector_cross_ph_PICML::meta_cadenaESSConnection = Uml::AssociationRole::Create(ass);
		PublishConnector_cross_ph_PICML::meta_cadenaESSConnection.name() = "cadenaESSConnection";
		PublishConnector_cross_ph_PICML::meta_cadenaESSConnection.min() = 1;
		PublishConnector_cross_ph_PICML::meta_cadenaESSConnection.max() = 1;
		PublishConnector_cross_ph_PICML::meta_cadenaESSConnection.isNavigable() = true;
		PublishConnector_cross_ph_PICML::meta_cadenaESSConnection.isPrimary() = false;
		PublishConnector_cross_ph_PICML::meta_cadenaESSConnection.target() = ESSConnection_cross_ph_cadena_scenario::meta;
	};
	void InitAssociation1()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		FRConnection_cross_ph_cadena_scenario::meta_picmlInvoke = Uml::AssociationRole::Create(ass);
		FRConnection_cross_ph_cadena_scenario::meta_picmlInvoke.name() = "picmlInvoke";
		FRConnection_cross_ph_cadena_scenario::meta_picmlInvoke.min() = 1;
		FRConnection_cross_ph_cadena_scenario::meta_picmlInvoke.max() = 1;
		FRConnection_cross_ph_cadena_scenario::meta_picmlInvoke.isNavigable() = true;
		FRConnection_cross_ph_cadena_scenario::meta_picmlInvoke.isPrimary() = false;
		FRConnection_cross_ph_cadena_scenario::meta_picmlInvoke.target() = invoke_cross_ph_PICML::meta;
		invoke_cross_ph_PICML::meta_cadenaInvoke = Uml::AssociationRole::Create(ass);
		invoke_cross_ph_PICML::meta_cadenaInvoke.name() = "cadenaInvoke";
		invoke_cross_ph_PICML::meta_cadenaInvoke.min() = 1;
		invoke_cross_ph_PICML::meta_cadenaInvoke.max() = 1;
		invoke_cross_ph_PICML::meta_cadenaInvoke.isNavigable() = true;
		invoke_cross_ph_PICML::meta_cadenaInvoke.isPrimary() = false;
		invoke_cross_ph_PICML::meta_cadenaInvoke.target() = FRConnection_cross_ph_cadena_scenario::meta;
	};
	void InitAssociation2()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ESSConnection_cross_ph_cadena_scenario::meta_picmlEmit = Uml::AssociationRole::Create(ass);
		ESSConnection_cross_ph_cadena_scenario::meta_picmlEmit.name() = "picmlEmit";
		ESSConnection_cross_ph_cadena_scenario::meta_picmlEmit.min() = 0;
		ESSConnection_cross_ph_cadena_scenario::meta_picmlEmit.max() = -1;
		ESSConnection_cross_ph_cadena_scenario::meta_picmlEmit.isNavigable() = true;
		ESSConnection_cross_ph_cadena_scenario::meta_picmlEmit.isPrimary() = false;
		ESSConnection_cross_ph_cadena_scenario::meta_picmlEmit.target() = emit_cross_ph_PICML::meta;
		emit_cross_ph_PICML::meta_cadenaEmit = Uml::AssociationRole::Create(ass);
		emit_cross_ph_PICML::meta_cadenaEmit.name() = "cadenaEmit";
		emit_cross_ph_PICML::meta_cadenaEmit.min() = 0;
		emit_cross_ph_PICML::meta_cadenaEmit.max() = -1;
		emit_cross_ph_PICML::meta_cadenaEmit.isNavigable() = true;
		emit_cross_ph_PICML::meta_cadenaEmit.isPrimary() = false;
		emit_cross_ph_PICML::meta_cadenaEmit.target() = ESSConnection_cross_ph_cadena_scenario::meta;
	};
	void InitAssociation3()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_componentAssembly = Uml::AssociationRole::Create(ass);
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_componentAssembly.name() = "componentAssembly";
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_componentAssembly.min() = 1;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_componentAssembly.max() = 1;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_componentAssembly.isNavigable() = true;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_componentAssembly.isPrimary() = false;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_componentAssembly.target() = ComponentAssembly_cross_ph_PICML::meta;
		ComponentAssembly_cross_ph_PICML::meta_rootContainer = Uml::AssociationRole::Create(ass);
		ComponentAssembly_cross_ph_PICML::meta_rootContainer.name() = "rootContainer";
		ComponentAssembly_cross_ph_PICML::meta_rootContainer.min() = 1;
		ComponentAssembly_cross_ph_PICML::meta_rootContainer.max() = 1;
		ComponentAssembly_cross_ph_PICML::meta_rootContainer.isNavigable() = true;
		ComponentAssembly_cross_ph_PICML::meta_rootContainer.isPrimary() = false;
		ComponentAssembly_cross_ph_PICML::meta_rootContainer.target() = RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta;
	};
	void InitAssociation4()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ComponentPort_cross_ph_cadena_scenario::meta_picmlPort = Uml::AssociationRole::Create(ass);
		ComponentPort_cross_ph_cadena_scenario::meta_picmlPort.name() = "picmlPort";
		ComponentPort_cross_ph_cadena_scenario::meta_picmlPort.min() = 1;
		ComponentPort_cross_ph_cadena_scenario::meta_picmlPort.max() = 1;
		ComponentPort_cross_ph_cadena_scenario::meta_picmlPort.isNavigable() = true;
		ComponentPort_cross_ph_cadena_scenario::meta_picmlPort.isPrimary() = false;
		ComponentPort_cross_ph_cadena_scenario::meta_picmlPort.target() = Port_cross_ph_PICML::meta;
		Port_cross_ph_PICML::meta_cadenaPort = Uml::AssociationRole::Create(ass);
		Port_cross_ph_PICML::meta_cadenaPort.name() = "cadenaPort";
		Port_cross_ph_PICML::meta_cadenaPort.min() = 1;
		Port_cross_ph_PICML::meta_cadenaPort.max() = 1;
		Port_cross_ph_PICML::meta_cadenaPort.isNavigable() = true;
		Port_cross_ph_PICML::meta_cadenaPort.isPrimary() = false;
		Port_cross_ph_PICML::meta_cadenaPort.target() = ComponentPort_cross_ph_cadena_scenario::meta;
	};
	void InitAssociation5()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ComponentInstance_cross_ph_cadena_scenario::meta_picmlComponentDef = Uml::AssociationRole::Create(ass);
		ComponentInstance_cross_ph_cadena_scenario::meta_picmlComponentDef.name() = "picmlComponentDef";
		ComponentInstance_cross_ph_cadena_scenario::meta_picmlComponentDef.min() = 1;
		ComponentInstance_cross_ph_cadena_scenario::meta_picmlComponentDef.max() = 1;
		ComponentInstance_cross_ph_cadena_scenario::meta_picmlComponentDef.isNavigable() = true;
		ComponentInstance_cross_ph_cadena_scenario::meta_picmlComponentDef.isPrimary() = false;
		ComponentInstance_cross_ph_cadena_scenario::meta_picmlComponentDef.target() = Component_cross_ph_PICML::meta;
		Component_cross_ph_PICML::meta_cadenaComponentInstance = Uml::AssociationRole::Create(ass);
		Component_cross_ph_PICML::meta_cadenaComponentInstance.name() = "cadenaComponentInstance";
		Component_cross_ph_PICML::meta_cadenaComponentInstance.min() = 1;
		Component_cross_ph_PICML::meta_cadenaComponentInstance.max() = 1;
		Component_cross_ph_PICML::meta_cadenaComponentInstance.isNavigable() = true;
		Component_cross_ph_PICML::meta_cadenaComponentInstance.isPrimary() = false;
		Component_cross_ph_PICML::meta_cadenaComponentInstance.target() = ComponentInstance_cross_ph_cadena_scenario::meta;
	};
	void InitComposition0()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PublishConnector_cross_ph_PICML::meta__gen_cont_parent = Uml::CompositionParentRole::Create(comp);
		PublishConnector_cross_ph_PICML::meta__gen_cont_parent.name() = "_gen_cont_parent";
		PublishConnector_cross_ph_PICML::meta__gen_cont_parent.isNavigable() = true;
		PublishConnector_cross_ph_PICML::meta__gen_cont_parent.target() = _gen_cont::meta;
		_gen_cont::meta_PublishConnector_cross_ph_PICML_children = Uml::CompositionChildRole::Create(comp);
		_gen_cont::meta_PublishConnector_cross_ph_PICML_children.name() = "PublishConnector_cross_ph_PICML";
		_gen_cont::meta_PublishConnector_cross_ph_PICML_children.min() = 0;
		_gen_cont::meta_PublishConnector_cross_ph_PICML_children.max() = -1;
		_gen_cont::meta_PublishConnector_cross_ph_PICML_children.isNavigable() = true;
		_gen_cont::meta_PublishConnector_cross_ph_PICML_children.target() = PublishConnector_cross_ph_PICML::meta;
	}
	void InitComposition1()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		invoke_cross_ph_PICML::meta__gen_cont_parent = Uml::CompositionParentRole::Create(comp);
		invoke_cross_ph_PICML::meta__gen_cont_parent.name() = "_gen_cont_parent";
		invoke_cross_ph_PICML::meta__gen_cont_parent.isNavigable() = true;
		invoke_cross_ph_PICML::meta__gen_cont_parent.target() = _gen_cont::meta;
		_gen_cont::meta_invoke_cross_ph_PICML_children = Uml::CompositionChildRole::Create(comp);
		_gen_cont::meta_invoke_cross_ph_PICML_children.name() = "invoke_cross_ph_PICML";
		_gen_cont::meta_invoke_cross_ph_PICML_children.min() = 0;
		_gen_cont::meta_invoke_cross_ph_PICML_children.max() = -1;
		_gen_cont::meta_invoke_cross_ph_PICML_children.isNavigable() = true;
		_gen_cont::meta_invoke_cross_ph_PICML_children.target() = invoke_cross_ph_PICML::meta;
	}
	void InitComposition2()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		emit_cross_ph_PICML::meta__gen_cont_parent = Uml::CompositionParentRole::Create(comp);
		emit_cross_ph_PICML::meta__gen_cont_parent.name() = "_gen_cont_parent";
		emit_cross_ph_PICML::meta__gen_cont_parent.isNavigable() = true;
		emit_cross_ph_PICML::meta__gen_cont_parent.target() = _gen_cont::meta;
		_gen_cont::meta_emit_cross_ph_PICML_children = Uml::CompositionChildRole::Create(comp);
		_gen_cont::meta_emit_cross_ph_PICML_children.name() = "emit_cross_ph_PICML";
		_gen_cont::meta_emit_cross_ph_PICML_children.min() = 0;
		_gen_cont::meta_emit_cross_ph_PICML_children.max() = -1;
		_gen_cont::meta_emit_cross_ph_PICML_children.isNavigable() = true;
		_gen_cont::meta_emit_cross_ph_PICML_children.target() = emit_cross_ph_PICML::meta;
	}
	void InitComposition3()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentAssembly_cross_ph_PICML::meta__gen_cont_parent = Uml::CompositionParentRole::Create(comp);
		ComponentAssembly_cross_ph_PICML::meta__gen_cont_parent.name() = "_gen_cont_parent";
		ComponentAssembly_cross_ph_PICML::meta__gen_cont_parent.isNavigable() = true;
		ComponentAssembly_cross_ph_PICML::meta__gen_cont_parent.target() = _gen_cont::meta;
		_gen_cont::meta_ComponentAssembly_cross_ph_PICML_children = Uml::CompositionChildRole::Create(comp);
		_gen_cont::meta_ComponentAssembly_cross_ph_PICML_children.name() = "ComponentAssembly_cross_ph_PICML";
		_gen_cont::meta_ComponentAssembly_cross_ph_PICML_children.min() = 0;
		_gen_cont::meta_ComponentAssembly_cross_ph_PICML_children.max() = -1;
		_gen_cont::meta_ComponentAssembly_cross_ph_PICML_children.isNavigable() = true;
		_gen_cont::meta_ComponentAssembly_cross_ph_PICML_children.target() = ComponentAssembly_cross_ph_PICML::meta;
	}
	void InitComposition4()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Port_cross_ph_PICML::meta__gen_cont_parent = Uml::CompositionParentRole::Create(comp);
		Port_cross_ph_PICML::meta__gen_cont_parent.name() = "_gen_cont_parent";
		Port_cross_ph_PICML::meta__gen_cont_parent.isNavigable() = true;
		Port_cross_ph_PICML::meta__gen_cont_parent.target() = _gen_cont::meta;
		_gen_cont::meta_Port_cross_ph_PICML_children = Uml::CompositionChildRole::Create(comp);
		_gen_cont::meta_Port_cross_ph_PICML_children.name() = "Port_cross_ph_PICML";
		_gen_cont::meta_Port_cross_ph_PICML_children.min() = 0;
		_gen_cont::meta_Port_cross_ph_PICML_children.max() = -1;
		_gen_cont::meta_Port_cross_ph_PICML_children.isNavigable() = true;
		_gen_cont::meta_Port_cross_ph_PICML_children.target() = Port_cross_ph_PICML::meta;
	}
	void InitComposition5()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Component_cross_ph_PICML::meta__gen_cont_parent = Uml::CompositionParentRole::Create(comp);
		Component_cross_ph_PICML::meta__gen_cont_parent.name() = "_gen_cont_parent";
		Component_cross_ph_PICML::meta__gen_cont_parent.isNavigable() = true;
		Component_cross_ph_PICML::meta__gen_cont_parent.target() = _gen_cont::meta;
		_gen_cont::meta_Component_cross_ph_PICML_children = Uml::CompositionChildRole::Create(comp);
		_gen_cont::meta_Component_cross_ph_PICML_children.name() = "Component_cross_ph_PICML";
		_gen_cont::meta_Component_cross_ph_PICML_children.min() = 0;
		_gen_cont::meta_Component_cross_ph_PICML_children.max() = -1;
		_gen_cont::meta_Component_cross_ph_PICML_children.isNavigable() = true;
		_gen_cont::meta_Component_cross_ph_PICML_children.target() = Component_cross_ph_PICML::meta;
	}
	void InitComposition6()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta__gen_cont_parent = Uml::CompositionParentRole::Create(comp);
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta__gen_cont_parent.name() = "_gen_cont_parent";
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta__gen_cont_parent.isNavigable() = true;
		RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta__gen_cont_parent.target() = _gen_cont::meta;
		_gen_cont::meta_RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties_children = Uml::CompositionChildRole::Create(comp);
		_gen_cont::meta_RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties_children.name() = "RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties";
		_gen_cont::meta_RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties_children.min() = 0;
		_gen_cont::meta_RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties_children.max() = -1;
		_gen_cont::meta_RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties_children.isNavigable() = true;
		_gen_cont::meta_RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties_children.target() = RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta;
	}
	void InitComposition7()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ESSConnection_cross_ph_cadena_scenario::meta__gen_cont_parent = Uml::CompositionParentRole::Create(comp);
		ESSConnection_cross_ph_cadena_scenario::meta__gen_cont_parent.name() = "_gen_cont_parent";
		ESSConnection_cross_ph_cadena_scenario::meta__gen_cont_parent.isNavigable() = true;
		ESSConnection_cross_ph_cadena_scenario::meta__gen_cont_parent.target() = _gen_cont::meta;
		_gen_cont::meta_ESSConnection_cross_ph_cadena_scenario_children = Uml::CompositionChildRole::Create(comp);
		_gen_cont::meta_ESSConnection_cross_ph_cadena_scenario_children.name() = "ESSConnection_cross_ph_cadena_scenario";
		_gen_cont::meta_ESSConnection_cross_ph_cadena_scenario_children.min() = 0;
		_gen_cont::meta_ESSConnection_cross_ph_cadena_scenario_children.max() = -1;
		_gen_cont::meta_ESSConnection_cross_ph_cadena_scenario_children.isNavigable() = true;
		_gen_cont::meta_ESSConnection_cross_ph_cadena_scenario_children.target() = ESSConnection_cross_ph_cadena_scenario::meta;
	}
	void InitComposition8()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		FRConnection_cross_ph_cadena_scenario::meta__gen_cont_parent = Uml::CompositionParentRole::Create(comp);
		FRConnection_cross_ph_cadena_scenario::meta__gen_cont_parent.name() = "_gen_cont_parent";
		FRConnection_cross_ph_cadena_scenario::meta__gen_cont_parent.isNavigable() = true;
		FRConnection_cross_ph_cadena_scenario::meta__gen_cont_parent.target() = _gen_cont::meta;
		_gen_cont::meta_FRConnection_cross_ph_cadena_scenario_children = Uml::CompositionChildRole::Create(comp);
		_gen_cont::meta_FRConnection_cross_ph_cadena_scenario_children.name() = "FRConnection_cross_ph_cadena_scenario";
		_gen_cont::meta_FRConnection_cross_ph_cadena_scenario_children.min() = 0;
		_gen_cont::meta_FRConnection_cross_ph_cadena_scenario_children.max() = -1;
		_gen_cont::meta_FRConnection_cross_ph_cadena_scenario_children.isNavigable() = true;
		_gen_cont::meta_FRConnection_cross_ph_cadena_scenario_children.target() = FRConnection_cross_ph_cadena_scenario::meta;
	}
	void InitComposition9()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentPort_cross_ph_cadena_scenario::meta__gen_cont_parent = Uml::CompositionParentRole::Create(comp);
		ComponentPort_cross_ph_cadena_scenario::meta__gen_cont_parent.name() = "_gen_cont_parent";
		ComponentPort_cross_ph_cadena_scenario::meta__gen_cont_parent.isNavigable() = true;
		ComponentPort_cross_ph_cadena_scenario::meta__gen_cont_parent.target() = _gen_cont::meta;
		_gen_cont::meta_ComponentPort_cross_ph_cadena_scenario_children = Uml::CompositionChildRole::Create(comp);
		_gen_cont::meta_ComponentPort_cross_ph_cadena_scenario_children.name() = "ComponentPort_cross_ph_cadena_scenario";
		_gen_cont::meta_ComponentPort_cross_ph_cadena_scenario_children.min() = 0;
		_gen_cont::meta_ComponentPort_cross_ph_cadena_scenario_children.max() = -1;
		_gen_cont::meta_ComponentPort_cross_ph_cadena_scenario_children.isNavigable() = true;
		_gen_cont::meta_ComponentPort_cross_ph_cadena_scenario_children.target() = ComponentPort_cross_ph_cadena_scenario::meta;
	}
	void InitComposition10()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentInstance_cross_ph_cadena_scenario::meta__gen_cont_parent = Uml::CompositionParentRole::Create(comp);
		ComponentInstance_cross_ph_cadena_scenario::meta__gen_cont_parent.name() = "_gen_cont_parent";
		ComponentInstance_cross_ph_cadena_scenario::meta__gen_cont_parent.isNavigable() = true;
		ComponentInstance_cross_ph_cadena_scenario::meta__gen_cont_parent.target() = _gen_cont::meta;
		_gen_cont::meta_ComponentInstance_cross_ph_cadena_scenario_children = Uml::CompositionChildRole::Create(comp);
		_gen_cont::meta_ComponentInstance_cross_ph_cadena_scenario_children.name() = "ComponentInstance_cross_ph_cadena_scenario";
		_gen_cont::meta_ComponentInstance_cross_ph_cadena_scenario_children.min() = 0;
		_gen_cont::meta_ComponentInstance_cross_ph_cadena_scenario_children.max() = -1;
		_gen_cont::meta_ComponentInstance_cross_ph_cadena_scenario_children.isNavigable() = true;
		_gen_cont::meta_ComponentInstance_cross_ph_cadena_scenario_children.target() = ComponentInstance_cross_ph_cadena_scenario::meta;
	}
	 void Initialize()
	{
		static bool first = true;
		if(!first) return;
		first = false;
		Uml::Initialize();

		ASSERT( umldiagram == Udm::null );
		UdmStatic::StaticDataNetwork * meta_dn = new UdmStatic::StaticDataNetwork(Uml::diagram);
		meta_dn->CreateNew("PICML2Cadena.mem", "", Uml::Diagram::meta, Udm::CHANGES_LOST_DEFAULT);
		umldiagram = Uml::Diagram::Cast(meta_dn->GetRootObject());
		umldiagram.name() ="PICML2Cadena";
		umldiagram.version() ="1.00";
		Creates();
		InitClassesAttributes();
		Uml::InitDiagram(umldiagram, "PICML2Cadena", "1.00");

		InitAssociation0();
		InitAssociation1();
		InitAssociation2();
		InitAssociation3();
		InitAssociation4();
		InitAssociation5();
		InitComposition0();
		InitComposition1();
		InitComposition2();
		InitComposition3();
		InitComposition4();
		InitComposition5();
		InitComposition6();
		InitComposition7();
		InitComposition8();
		InitComposition9();
		InitComposition10();
		Port_cross_ph_PICML::meta.subTypes() +=  OutEventPort_cross_ph_PICML::meta;
		Port_cross_ph_PICML::meta.subTypes() +=  ProvidedRequestPort_cross_ph_PICML::meta;
		Port_cross_ph_PICML::meta.subTypes() +=  RequiredRequestPort_cross_ph_PICML::meta;
		Port_cross_ph_PICML::meta.subTypes() +=  InEventPort_cross_ph_PICML::meta;
		ComponentPort_cross_ph_cadena_scenario::meta.subTypes() +=  EventSink_cross_ph_cadena_scenario::meta;
		ComponentPort_cross_ph_cadena_scenario::meta.subTypes() +=  EventSource_cross_ph_cadena_scenario::meta;
		ComponentPort_cross_ph_cadena_scenario::meta.subTypes() +=  Receptacle_cross_ph_cadena_scenario::meta;
		ComponentPort_cross_ph_cadena_scenario::meta.subTypes() +=  Facet_cross_ph_cadena_scenario::meta;
		
	}
	void Initialize(const Uml::Diagram & dgr)
	{
		umldiagram = dgr;
		Uml::SetClass(PublishConnector_cross_ph_PICML::meta, umldiagram, "PublishConnector_cross_ph_PICML");
		Uml::SetAttribute(PublishConnector_cross_ph_PICML::meta_rem_id,PublishConnector_cross_ph_PICML::meta, "rem_id");
		Uml::SetAttribute(PublishConnector_cross_ph_PICML::meta_rem_sysname,PublishConnector_cross_ph_PICML::meta, "rem_sysname");
		Uml::SetClass(invoke_cross_ph_PICML::meta, umldiagram, "invoke_cross_ph_PICML");
		Uml::SetAttribute(invoke_cross_ph_PICML::meta_rem_id,invoke_cross_ph_PICML::meta, "rem_id");
		Uml::SetAttribute(invoke_cross_ph_PICML::meta_rem_sysname,invoke_cross_ph_PICML::meta, "rem_sysname");
		Uml::SetClass(emit_cross_ph_PICML::meta, umldiagram, "emit_cross_ph_PICML");
		Uml::SetAttribute(emit_cross_ph_PICML::meta_rem_id,emit_cross_ph_PICML::meta, "rem_id");
		Uml::SetAttribute(emit_cross_ph_PICML::meta_rem_sysname,emit_cross_ph_PICML::meta, "rem_sysname");
		Uml::SetClass(ComponentAssembly_cross_ph_PICML::meta, umldiagram, "ComponentAssembly_cross_ph_PICML");
		Uml::SetAttribute(ComponentAssembly_cross_ph_PICML::meta_rem_id,ComponentAssembly_cross_ph_PICML::meta, "rem_id");
		Uml::SetAttribute(ComponentAssembly_cross_ph_PICML::meta_rem_sysname,ComponentAssembly_cross_ph_PICML::meta, "rem_sysname");
		Uml::SetClass(OutEventPort_cross_ph_PICML::meta, umldiagram, "OutEventPort_cross_ph_PICML");
		Uml::SetClass(ProvidedRequestPort_cross_ph_PICML::meta, umldiagram, "ProvidedRequestPort_cross_ph_PICML");
		Uml::SetClass(RequiredRequestPort_cross_ph_PICML::meta, umldiagram, "RequiredRequestPort_cross_ph_PICML");
		Uml::SetClass(InEventPort_cross_ph_PICML::meta, umldiagram, "InEventPort_cross_ph_PICML");
		Uml::SetClass(Port_cross_ph_PICML::meta, umldiagram, "Port_cross_ph_PICML");
		Uml::SetAttribute(Port_cross_ph_PICML::meta_rem_id,Port_cross_ph_PICML::meta, "rem_id");
		Uml::SetAttribute(Port_cross_ph_PICML::meta_rem_sysname,Port_cross_ph_PICML::meta, "rem_sysname");
		Uml::SetClass(Component_cross_ph_PICML::meta, umldiagram, "Component_cross_ph_PICML");
		Uml::SetAttribute(Component_cross_ph_PICML::meta_rem_id,Component_cross_ph_PICML::meta, "rem_id");
		Uml::SetAttribute(Component_cross_ph_PICML::meta_rem_sysname,Component_cross_ph_PICML::meta, "rem_sysname");
		Uml::SetClass(RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta, umldiagram, "RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties");
		Uml::SetAttribute(RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_id,RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta, "rem_id");
		Uml::SetAttribute(RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_rem_sysname,RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta, "rem_sysname");
		Uml::SetClass(ESSConnection_cross_ph_cadena_scenario::meta, umldiagram, "ESSConnection_cross_ph_cadena_scenario");
		Uml::SetAttribute(ESSConnection_cross_ph_cadena_scenario::meta_rem_id,ESSConnection_cross_ph_cadena_scenario::meta, "rem_id");
		Uml::SetAttribute(ESSConnection_cross_ph_cadena_scenario::meta_rem_sysname,ESSConnection_cross_ph_cadena_scenario::meta, "rem_sysname");
		Uml::SetClass(FRConnection_cross_ph_cadena_scenario::meta, umldiagram, "FRConnection_cross_ph_cadena_scenario");
		Uml::SetAttribute(FRConnection_cross_ph_cadena_scenario::meta_rem_id,FRConnection_cross_ph_cadena_scenario::meta, "rem_id");
		Uml::SetAttribute(FRConnection_cross_ph_cadena_scenario::meta_rem_sysname,FRConnection_cross_ph_cadena_scenario::meta, "rem_sysname");
		Uml::SetClass(EventSink_cross_ph_cadena_scenario::meta, umldiagram, "EventSink_cross_ph_cadena_scenario");
		Uml::SetClass(EventSource_cross_ph_cadena_scenario::meta, umldiagram, "EventSource_cross_ph_cadena_scenario");
		Uml::SetClass(Receptacle_cross_ph_cadena_scenario::meta, umldiagram, "Receptacle_cross_ph_cadena_scenario");
		Uml::SetClass(Facet_cross_ph_cadena_scenario::meta, umldiagram, "Facet_cross_ph_cadena_scenario");
		Uml::SetClass(ComponentPort_cross_ph_cadena_scenario::meta, umldiagram, "ComponentPort_cross_ph_cadena_scenario");
		Uml::SetAttribute(ComponentPort_cross_ph_cadena_scenario::meta_rem_id,ComponentPort_cross_ph_cadena_scenario::meta, "rem_id");
		Uml::SetAttribute(ComponentPort_cross_ph_cadena_scenario::meta_rem_sysname,ComponentPort_cross_ph_cadena_scenario::meta, "rem_sysname");
		Uml::SetClass(ComponentInstance_cross_ph_cadena_scenario::meta, umldiagram, "ComponentInstance_cross_ph_cadena_scenario");
		Uml::SetAttribute(ComponentInstance_cross_ph_cadena_scenario::meta_rem_id,ComponentInstance_cross_ph_cadena_scenario::meta, "rem_id");
		Uml::SetAttribute(ComponentInstance_cross_ph_cadena_scenario::meta_rem_sysname,ComponentInstance_cross_ph_cadena_scenario::meta, "rem_sysname");
		Uml::SetClass(_gen_cont::meta, umldiagram, "_gen_cont");
		//composition child roles 
		Uml::SetParentRole(PublishConnector_cross_ph_PICML::meta__gen_cont_parent,PublishConnector_cross_ph_PICML::meta,_gen_cont::meta, "", "");
		Uml::SetParentRole(invoke_cross_ph_PICML::meta__gen_cont_parent,invoke_cross_ph_PICML::meta,_gen_cont::meta, "", "");
		Uml::SetParentRole(emit_cross_ph_PICML::meta__gen_cont_parent,emit_cross_ph_PICML::meta,_gen_cont::meta, "", "");
		Uml::SetParentRole(ComponentAssembly_cross_ph_PICML::meta__gen_cont_parent,ComponentAssembly_cross_ph_PICML::meta,_gen_cont::meta, "", "");
		Uml::SetParentRole(Port_cross_ph_PICML::meta__gen_cont_parent,Port_cross_ph_PICML::meta,_gen_cont::meta, "", "");
		Uml::SetParentRole(Component_cross_ph_PICML::meta__gen_cont_parent,Component_cross_ph_PICML::meta,_gen_cont::meta, "", "");
		Uml::SetParentRole(RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta__gen_cont_parent,RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta,_gen_cont::meta, "", "");
		Uml::SetParentRole(ESSConnection_cross_ph_cadena_scenario::meta__gen_cont_parent,ESSConnection_cross_ph_cadena_scenario::meta,_gen_cont::meta, "", "");
		Uml::SetParentRole(FRConnection_cross_ph_cadena_scenario::meta__gen_cont_parent,FRConnection_cross_ph_cadena_scenario::meta,_gen_cont::meta, "", "");
		Uml::SetParentRole(ComponentPort_cross_ph_cadena_scenario::meta__gen_cont_parent,ComponentPort_cross_ph_cadena_scenario::meta,_gen_cont::meta, "", "");
		Uml::SetParentRole(ComponentInstance_cross_ph_cadena_scenario::meta__gen_cont_parent,ComponentInstance_cross_ph_cadena_scenario::meta,_gen_cont::meta, "", "");
		// composition parentroles 
		Uml::SetChildRole(_gen_cont::meta_PublishConnector_cross_ph_PICML_children,_gen_cont::meta,PublishConnector_cross_ph_PICML::meta, "", "");
		Uml::SetChildRole(_gen_cont::meta_invoke_cross_ph_PICML_children,_gen_cont::meta,invoke_cross_ph_PICML::meta, "", "");
		Uml::SetChildRole(_gen_cont::meta_emit_cross_ph_PICML_children,_gen_cont::meta,emit_cross_ph_PICML::meta, "", "");
		Uml::SetChildRole(_gen_cont::meta_ComponentAssembly_cross_ph_PICML_children,_gen_cont::meta,ComponentAssembly_cross_ph_PICML::meta, "", "");
		Uml::SetChildRole(_gen_cont::meta_Port_cross_ph_PICML_children,_gen_cont::meta,Port_cross_ph_PICML::meta, "", "");
		Uml::SetChildRole(_gen_cont::meta_Component_cross_ph_PICML_children,_gen_cont::meta,Component_cross_ph_PICML::meta, "", "");
		Uml::SetChildRole(_gen_cont::meta_RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties_children,_gen_cont::meta,RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta, "", "");
		Uml::SetChildRole(_gen_cont::meta_ESSConnection_cross_ph_cadena_scenario_children,_gen_cont::meta,ESSConnection_cross_ph_cadena_scenario::meta, "", "");
		Uml::SetChildRole(_gen_cont::meta_FRConnection_cross_ph_cadena_scenario_children,_gen_cont::meta,FRConnection_cross_ph_cadena_scenario::meta, "", "");
		Uml::SetChildRole(_gen_cont::meta_ComponentPort_cross_ph_cadena_scenario_children,_gen_cont::meta,ComponentPort_cross_ph_cadena_scenario::meta, "", "");
		Uml::SetChildRole(_gen_cont::meta_ComponentInstance_cross_ph_cadena_scenario_children,_gen_cont::meta,ComponentInstance_cross_ph_cadena_scenario::meta, "", "");
		// Association roles 
		Uml::SetAssocRole(PublishConnector_cross_ph_PICML::meta_cadenaESSConnection,PublishConnector_cross_ph_PICML::meta,ESSConnection_cross_ph_cadena_scenario::meta, "picmlPublishConnector");

		Uml::SetAssocRole(invoke_cross_ph_PICML::meta_cadenaInvoke,invoke_cross_ph_PICML::meta,FRConnection_cross_ph_cadena_scenario::meta, "picmlInvoke");

		Uml::SetAssocRole(emit_cross_ph_PICML::meta_cadenaEmit,emit_cross_ph_PICML::meta,ESSConnection_cross_ph_cadena_scenario::meta, "picmlEmit");

		Uml::SetAssocRole(ComponentAssembly_cross_ph_PICML::meta_rootContainer,ComponentAssembly_cross_ph_PICML::meta,RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta, "componentAssembly");





		Uml::SetAssocRole(Port_cross_ph_PICML::meta_cadenaPort,Port_cross_ph_PICML::meta,ComponentPort_cross_ph_cadena_scenario::meta, "picmlPort");

		Uml::SetAssocRole(Component_cross_ph_PICML::meta_cadenaComponentInstance,Component_cross_ph_PICML::meta,ComponentInstance_cross_ph_cadena_scenario::meta, "picmlComponentDef");

		Uml::SetAssocRole(RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta_componentAssembly,RootContainer_cross_ph_PICMLCadena_Xchange_embedded_EIProperties::meta,ComponentAssembly_cross_ph_PICML::meta, "rootContainer");

		Uml::SetAssocRole(ESSConnection_cross_ph_cadena_scenario::meta_picmlPublishConnector,ESSConnection_cross_ph_cadena_scenario::meta,PublishConnector_cross_ph_PICML::meta, "cadenaESSConnection");
		Uml::SetAssocRole(ESSConnection_cross_ph_cadena_scenario::meta_picmlEmit,ESSConnection_cross_ph_cadena_scenario::meta,emit_cross_ph_PICML::meta, "cadenaEmit");

		Uml::SetAssocRole(FRConnection_cross_ph_cadena_scenario::meta_picmlInvoke,FRConnection_cross_ph_cadena_scenario::meta,invoke_cross_ph_PICML::meta, "cadenaInvoke");





		Uml::SetAssocRole(ComponentPort_cross_ph_cadena_scenario::meta_picmlPort,ComponentPort_cross_ph_cadena_scenario::meta,Port_cross_ph_PICML::meta, "cadenaPort");

		Uml::SetAssocRole(ComponentInstance_cross_ph_cadena_scenario::meta_picmlComponentDef,ComponentInstance_cross_ph_cadena_scenario::meta,Component_cross_ph_PICML::meta, "cadenaComponentInstance");


		
	}
	 Udm::UdmDiagram diagram = { &umldiagram, Initialize };
	static struct _regClass
	{
		_regClass()
		{
			Udm::MetaDepository::StoreDiagram("PICML2Cadena", diagram);
		};
		~_regClass()
		{
			Udm::MetaDepository::RemoveDiagram("PICML2Cadena");
		};
	} __regUnUsed;

}
// END PICML2Cadena.cpp
