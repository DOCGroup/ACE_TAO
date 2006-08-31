// cpp(meta datanetwork format) source file ccm_assembly.cpp generated from diagram ccm_assembly
// generated on Wed Jul 30 14:52:48 2003

#include "ccm_assembly.h"
#include "UmlExt.h"

#include "UdmStatic.h"

namespace ccm_assembly {

	Uml::Diagram umldiagram;

	Uml::Class destination::meta;
	Uml::Class writeiortofile::meta;
	Uml::Class providesidentifier::meta;
	Uml::Class hostcollocation::meta;
	Uml::Class processcollocation::meta;
	Uml::Class location::meta;
	Uml::Class homeplacement::meta;
	Uml::Class providesport::meta;
	Uml::Class connections::meta;
	Uml::Class connectevent::meta;
	Uml::Class usesidentifier::meta;
	Uml::Class componentassembly::meta;
	Uml::Class description::meta;
	Uml::Class componentinstantiationref::meta;
	Uml::Class componentfileref::meta;
	Uml::Class eventoutputport::meta;
	Uml::Class consumesport::meta;
	Uml::Class fileinarchive::meta;
	Uml::Class componentfile::meta;
	Uml::Class publishesidentifier::meta;
	Uml::Class partitioning::meta;
	Uml::Class componentinstantiation::meta;
	Uml::Class connectinterface::meta;
	Uml::Class emitsport::meta;
	Uml::Class registercomponent::meta;
	Uml::Class publishesport::meta;
	Uml::Class componentfiles::meta;
	Uml::Class emitsidentifier::meta;
	Uml::Class consumesidentifier::meta;
	Uml::Class usesport::meta;
	Uml::Attribute destination::meta_content;
	Uml::Attribute writeiortofile::meta_name;
	Uml::Attribute providesidentifier::meta_content;
	Uml::Attribute hostcollocation::meta_id;
	Uml::Attribute processcollocation::meta_id;
	Uml::Attribute homeplacement::meta_id;
	Uml::Attribute connectevent::meta_id;
	Uml::Attribute usesidentifier::meta_content;
	Uml::Attribute componentassembly::meta_derivedfrom;
	Uml::Attribute componentassembly::meta_id;
	Uml::Attribute description::meta_content;
	Uml::Attribute componentinstantiationref::meta_idref;
	Uml::Attribute componentfileref::meta_idref;
	Uml::Attribute fileinarchive::meta_name;
	Uml::Attribute componentfile::meta_type;
	Uml::Attribute componentfile::meta_id;
	Uml::Attribute publishesidentifier::meta_content;
	Uml::Attribute componentinstantiation::meta_id;
	Uml::Attribute connectinterface::meta_id;
	Uml::Attribute emitsidentifier::meta_content;
	Uml::Attribute consumesidentifier::meta_content;
	Uml::CompositionParentRole destination::meta_processcollocation_parent;
	Uml::CompositionParentRole destination::meta_hostcollocation_parent;
	Uml::CompositionParentRole destination::meta_homeplacement_parent;
	Uml::CompositionParentRole writeiortofile::meta_registercomponent_parent;
	Uml::CompositionParentRole providesidentifier::meta_providesport_parent;
	Uml::CompositionParentRole hostcollocation::meta_partitioning_parent;
	Uml::CompositionParentRole location::meta_hostcollocation_parent;
	Uml::CompositionParentRole location::meta_partitioning_parent;
	Uml::CompositionParentRole homeplacement::meta_processcollocation_parent;
	Uml::CompositionParentRole providesport::meta_connectinterface_parent;
	Uml::CompositionParentRole connections::meta_componentassembly_parent;
	Uml::CompositionParentRole connectevent::meta_connections_parent;
	Uml::CompositionParentRole usesidentifier::meta_usesport_parent;
	Uml::CompositionParentRole description::meta_componentassembly_parent;
	Uml::CompositionParentRole componentinstantiationref::meta_providesport_parent;
	Uml::CompositionParentRole componentinstantiationref::meta_consumesport_parent;
	Uml::CompositionParentRole componentinstantiationref::meta_emitsport_parent;
	Uml::CompositionParentRole componentinstantiationref::meta_publishesport_parent;
	Uml::CompositionParentRole componentinstantiationref::meta_usesport_parent;
	Uml::CompositionParentRole componentfileref::meta_homeplacement_parent;
	Uml::CompositionParentRole eventoutputport::meta_connectevent_parent;
	Uml::CompositionParentRole consumesport::meta_connectevent_parent;
	Uml::CompositionParentRole fileinarchive::meta_componentfile_parent;
	Uml::CompositionParentRole componentfile::meta_componentfiles_parent;
	Uml::CompositionParentRole publishesidentifier::meta_publishesport_parent;
	Uml::CompositionParentRole partitioning::meta_componentassembly_parent;
	Uml::CompositionParentRole componentinstantiation::meta_homeplacement_parent;
	Uml::CompositionParentRole connectinterface::meta_connections_parent;
	Uml::CompositionParentRole registercomponent::meta_componentinstantiation_parent;
	Uml::CompositionParentRole componentfiles::meta_componentassembly_parent;
	Uml::CompositionParentRole emitsidentifier::meta_emitsport_parent;
	Uml::CompositionParentRole consumesidentifier::meta_consumesport_parent;
	Uml::CompositionParentRole usesport::meta_connectinterface_parent;
	Uml::CompositionChildRole hostcollocation::meta_location_children;
	Uml::CompositionChildRole hostcollocation::meta_destination_child;
	Uml::CompositionChildRole processcollocation::meta_homeplacement_children;
	Uml::CompositionChildRole processcollocation::meta_destination_child;
	Uml::CompositionChildRole homeplacement::meta_componentfileref_child;
	Uml::CompositionChildRole homeplacement::meta_destination_child;
	Uml::CompositionChildRole homeplacement::meta_componentinstantiation_children;
	Uml::CompositionChildRole providesport::meta_providesidentifier_child;
	Uml::CompositionChildRole providesport::meta_componentinstantiationref_child;
	Uml::CompositionChildRole connections::meta_connectevent_children;
	Uml::CompositionChildRole connections::meta_connectinterface_children;
	Uml::CompositionChildRole connectevent::meta_eventoutputport_child;
	Uml::CompositionChildRole connectevent::meta_consumesport_child;
	Uml::CompositionChildRole componentassembly::meta_componentfiles_child;
	Uml::CompositionChildRole componentassembly::meta_description_child;
	Uml::CompositionChildRole componentassembly::meta_partitioning_child;
	Uml::CompositionChildRole componentassembly::meta_connections_child;
	Uml::CompositionChildRole consumesport::meta_componentinstantiationref_child;
	Uml::CompositionChildRole consumesport::meta_consumesidentifier_child;
	Uml::CompositionChildRole componentfile::meta_fileinarchive_child;
	Uml::CompositionChildRole partitioning::meta_hostcollocation_children;
	Uml::CompositionChildRole partitioning::meta_location_children;
	Uml::CompositionChildRole componentinstantiation::meta_registercomponent_children;
	Uml::CompositionChildRole connectinterface::meta_providesport_child;
	Uml::CompositionChildRole connectinterface::meta_usesport_child;
	Uml::CompositionChildRole emitsport::meta_emitsidentifier_child;
	Uml::CompositionChildRole emitsport::meta_componentinstantiationref_child;
	Uml::CompositionChildRole registercomponent::meta_writeiortofile_children;
	Uml::CompositionChildRole publishesport::meta_componentinstantiationref_child;
	Uml::CompositionChildRole publishesport::meta_publishesidentifier_child;
	Uml::CompositionChildRole componentfiles::meta_componentfile_children;
	Uml::CompositionChildRole usesport::meta_usesidentifier_child;
	Uml::CompositionChildRole usesport::meta_componentinstantiationref_child;
	void Initialize()
	{
		static bool first = true;
		if(!first) return;
		first = false;
		Uml::Initialize();

		ASSERT( umldiagram == Udm::null );
		UdmStatic::StaticDataNetwork * meta_dn = new UdmStatic::StaticDataNetwork(Uml::diagram);
		meta_dn->CreateNew("ccm_assembly.mem", "", Uml::Diagram::meta, Udm::CHANGES_LOST_DEFAULT);
		umldiagram = Uml::Diagram::Cast(meta_dn->GetRootObject());
		umldiagram.name() ="ccm_assembly";
		destination::meta = Uml::Class::Create(umldiagram);
		writeiortofile::meta = Uml::Class::Create(umldiagram);
		providesidentifier::meta = Uml::Class::Create(umldiagram);
		hostcollocation::meta = Uml::Class::Create(umldiagram);
		processcollocation::meta = Uml::Class::Create(umldiagram);
		location::meta = Uml::Class::Create(umldiagram);
		homeplacement::meta = Uml::Class::Create(umldiagram);
		providesport::meta = Uml::Class::Create(umldiagram);
		connections::meta = Uml::Class::Create(umldiagram);
		connectevent::meta = Uml::Class::Create(umldiagram);
		usesidentifier::meta = Uml::Class::Create(umldiagram);
		componentassembly::meta = Uml::Class::Create(umldiagram);
		description::meta = Uml::Class::Create(umldiagram);
		componentinstantiationref::meta = Uml::Class::Create(umldiagram);
		componentfileref::meta = Uml::Class::Create(umldiagram);
		eventoutputport::meta = Uml::Class::Create(umldiagram);
		consumesport::meta = Uml::Class::Create(umldiagram);
		fileinarchive::meta = Uml::Class::Create(umldiagram);
		componentfile::meta = Uml::Class::Create(umldiagram);
		publishesidentifier::meta = Uml::Class::Create(umldiagram);
		partitioning::meta = Uml::Class::Create(umldiagram);
		componentinstantiation::meta = Uml::Class::Create(umldiagram);
		connectinterface::meta = Uml::Class::Create(umldiagram);
		emitsport::meta = Uml::Class::Create(umldiagram);
		registercomponent::meta = Uml::Class::Create(umldiagram);
		publishesport::meta = Uml::Class::Create(umldiagram);
		componentfiles::meta = Uml::Class::Create(umldiagram);
		emitsidentifier::meta = Uml::Class::Create(umldiagram);
		consumesidentifier::meta = Uml::Class::Create(umldiagram);
		usesport::meta = Uml::Class::Create(umldiagram);
		destination::meta_content = Uml::Attribute::Create(destination::meta);
		writeiortofile::meta_name = Uml::Attribute::Create(writeiortofile::meta);
		providesidentifier::meta_content = Uml::Attribute::Create(providesidentifier::meta);
		hostcollocation::meta_id = Uml::Attribute::Create(hostcollocation::meta);
		processcollocation::meta_id = Uml::Attribute::Create(processcollocation::meta);
		homeplacement::meta_id = Uml::Attribute::Create(homeplacement::meta);
		connectevent::meta_id = Uml::Attribute::Create(connectevent::meta);
		usesidentifier::meta_content = Uml::Attribute::Create(usesidentifier::meta);
		componentassembly::meta_derivedfrom = Uml::Attribute::Create(componentassembly::meta);
		componentassembly::meta_id = Uml::Attribute::Create(componentassembly::meta);
		description::meta_content = Uml::Attribute::Create(description::meta);
		componentinstantiationref::meta_idref = Uml::Attribute::Create(componentinstantiationref::meta);
		componentfileref::meta_idref = Uml::Attribute::Create(componentfileref::meta);
		fileinarchive::meta_name = Uml::Attribute::Create(fileinarchive::meta);
		componentfile::meta_type = Uml::Attribute::Create(componentfile::meta);
		componentfile::meta_id = Uml::Attribute::Create(componentfile::meta);
		publishesidentifier::meta_content = Uml::Attribute::Create(publishesidentifier::meta);
		componentinstantiation::meta_id = Uml::Attribute::Create(componentinstantiation::meta);
		connectinterface::meta_id = Uml::Attribute::Create(connectinterface::meta);
		emitsidentifier::meta_content = Uml::Attribute::Create(emitsidentifier::meta);
		consumesidentifier::meta_content = Uml::Attribute::Create(consumesidentifier::meta);
		Uml::InitDiagram(umldiagram, "ccm_assembly");

		destination::meta.name() = "destination";
		destination::meta.isAbstract() = false;
		writeiortofile::meta.name() = "writeiortofile";
		writeiortofile::meta.isAbstract() = false;
		providesidentifier::meta.name() = "providesidentifier";
		providesidentifier::meta.isAbstract() = false;
		hostcollocation::meta.name() = "hostcollocation";
		hostcollocation::meta.isAbstract() = false;
		processcollocation::meta.name() = "processcollocation";
		processcollocation::meta.isAbstract() = false;
		location::meta.name() = "location";
		location::meta.isAbstract() = true;
		homeplacement::meta.name() = "homeplacement";
		homeplacement::meta.isAbstract() = false;
		providesport::meta.name() = "providesport";
		providesport::meta.isAbstract() = false;
		connections::meta.name() = "connections";
		connections::meta.isAbstract() = false;
		connectevent::meta.name() = "connectevent";
		connectevent::meta.isAbstract() = false;
		usesidentifier::meta.name() = "usesidentifier";
		usesidentifier::meta.isAbstract() = false;
		componentassembly::meta.name() = "componentassembly";
		componentassembly::meta.isAbstract() = false;
		description::meta.name() = "description";
		description::meta.isAbstract() = false;
		componentinstantiationref::meta.name() = "componentinstantiationref";
		componentinstantiationref::meta.isAbstract() = false;
		componentfileref::meta.name() = "componentfileref";
		componentfileref::meta.isAbstract() = false;
		eventoutputport::meta.name() = "eventoutputport";
		eventoutputport::meta.isAbstract() = true;
		consumesport::meta.name() = "consumesport";
		consumesport::meta.isAbstract() = false;
		fileinarchive::meta.name() = "fileinarchive";
		fileinarchive::meta.isAbstract() = false;
		componentfile::meta.name() = "componentfile";
		componentfile::meta.isAbstract() = false;
		publishesidentifier::meta.name() = "publishesidentifier";
		publishesidentifier::meta.isAbstract() = false;
		partitioning::meta.name() = "partitioning";
		partitioning::meta.isAbstract() = false;
		componentinstantiation::meta.name() = "componentinstantiation";
		componentinstantiation::meta.isAbstract() = false;
		connectinterface::meta.name() = "connectinterface";
		connectinterface::meta.isAbstract() = false;
		emitsport::meta.name() = "emitsport";
		emitsport::meta.isAbstract() = false;
		registercomponent::meta.name() = "registercomponent";
		registercomponent::meta.isAbstract() = false;
		publishesport::meta.name() = "publishesport";
		publishesport::meta.isAbstract() = false;
		componentfiles::meta.name() = "componentfiles";
		componentfiles::meta.isAbstract() = false;
		emitsidentifier::meta.name() = "emitsidentifier";
		emitsidentifier::meta.isAbstract() = false;
		consumesidentifier::meta.name() = "consumesidentifier";
		consumesidentifier::meta.isAbstract() = false;
		usesport::meta.name() = "usesport";
		usesport::meta.isAbstract() = false;

		destination::meta_content.name() = "content";
		destination::meta_content.type() = "String";
		destination::meta_content.min() = 1;
		destination::meta_content.max() = 1;
		destination::meta_content.nonpersistent() = false;
		destination::meta_content.ordered() = false;
		destination::meta_content.visibility() = "public";
		destination::meta_content.defvalue() = vector<string>();
		writeiortofile::meta_name.name() = "name";
		writeiortofile::meta_name.type() = "String";
		writeiortofile::meta_name.min() = 1;
		writeiortofile::meta_name.max() = 1;
		writeiortofile::meta_name.nonpersistent() = false;
		writeiortofile::meta_name.ordered() = false;
		writeiortofile::meta_name.visibility() = "public";
		writeiortofile::meta_name.defvalue() = vector<string>();
		providesidentifier::meta_content.name() = "content";
		providesidentifier::meta_content.type() = "String";
		providesidentifier::meta_content.min() = 1;
		providesidentifier::meta_content.max() = 1;
		providesidentifier::meta_content.nonpersistent() = false;
		providesidentifier::meta_content.ordered() = false;
		providesidentifier::meta_content.visibility() = "public";
		providesidentifier::meta_content.defvalue() = vector<string>();
		hostcollocation::meta_id.name() = "id";
		hostcollocation::meta_id.type() = "String";
		hostcollocation::meta_id.min() = 1;
		hostcollocation::meta_id.max() = 1;
		hostcollocation::meta_id.nonpersistent() = false;
		hostcollocation::meta_id.ordered() = false;
		hostcollocation::meta_id.visibility() = "public";
		hostcollocation::meta_id.defvalue() = vector<string>();
		processcollocation::meta_id.name() = "id";
		processcollocation::meta_id.type() = "String";
		processcollocation::meta_id.min() = 1;
		processcollocation::meta_id.max() = 1;
		processcollocation::meta_id.nonpersistent() = false;
		processcollocation::meta_id.ordered() = false;
		processcollocation::meta_id.visibility() = "public";
		processcollocation::meta_id.defvalue() = vector<string>();
		homeplacement::meta_id.name() = "id";
		homeplacement::meta_id.type() = "String";
		homeplacement::meta_id.min() = 1;
		homeplacement::meta_id.max() = 1;
		homeplacement::meta_id.nonpersistent() = false;
		homeplacement::meta_id.ordered() = false;
		homeplacement::meta_id.visibility() = "public";
		homeplacement::meta_id.defvalue() = vector<string>();
		connectevent::meta_id.name() = "id";
		connectevent::meta_id.type() = "String";
		connectevent::meta_id.min() = 0;
		connectevent::meta_id.max() = 1;
		connectevent::meta_id.nonpersistent() = false;
		connectevent::meta_id.ordered() = false;
		connectevent::meta_id.visibility() = "public";
		connectevent::meta_id.defvalue() = vector<string>();
		usesidentifier::meta_content.name() = "content";
		usesidentifier::meta_content.type() = "String";
		usesidentifier::meta_content.min() = 1;
		usesidentifier::meta_content.max() = 1;
		usesidentifier::meta_content.nonpersistent() = false;
		usesidentifier::meta_content.ordered() = false;
		usesidentifier::meta_content.visibility() = "public";
		usesidentifier::meta_content.defvalue() = vector<string>();
		componentassembly::meta_derivedfrom.name() = "derivedfrom";
		componentassembly::meta_derivedfrom.type() = "String";
		componentassembly::meta_derivedfrom.min() = 0;
		componentassembly::meta_derivedfrom.max() = 1;
		componentassembly::meta_derivedfrom.nonpersistent() = false;
		componentassembly::meta_derivedfrom.ordered() = false;
		componentassembly::meta_derivedfrom.visibility() = "public";
		componentassembly::meta_derivedfrom.defvalue() = vector<string>();
		componentassembly::meta_id.name() = "id";
		componentassembly::meta_id.type() = "String";
		componentassembly::meta_id.min() = 1;
		componentassembly::meta_id.max() = 1;
		componentassembly::meta_id.nonpersistent() = false;
		componentassembly::meta_id.ordered() = false;
		componentassembly::meta_id.visibility() = "public";
		componentassembly::meta_id.defvalue() = vector<string>();
		description::meta_content.name() = "content";
		description::meta_content.type() = "String";
		description::meta_content.min() = 1;
		description::meta_content.max() = 1;
		description::meta_content.nonpersistent() = false;
		description::meta_content.ordered() = false;
		description::meta_content.visibility() = "public";
		description::meta_content.defvalue() = vector<string>();
		componentinstantiationref::meta_idref.name() = "idref";
		componentinstantiationref::meta_idref.type() = "String";
		componentinstantiationref::meta_idref.min() = 1;
		componentinstantiationref::meta_idref.max() = 1;
		componentinstantiationref::meta_idref.nonpersistent() = false;
		componentinstantiationref::meta_idref.ordered() = false;
		componentinstantiationref::meta_idref.visibility() = "public";
		componentinstantiationref::meta_idref.defvalue() = vector<string>();
		componentfileref::meta_idref.name() = "idref";
		componentfileref::meta_idref.type() = "String";
		componentfileref::meta_idref.min() = 1;
		componentfileref::meta_idref.max() = 1;
		componentfileref::meta_idref.nonpersistent() = false;
		componentfileref::meta_idref.ordered() = false;
		componentfileref::meta_idref.visibility() = "public";
		componentfileref::meta_idref.defvalue() = vector<string>();
		fileinarchive::meta_name.name() = "name";
		fileinarchive::meta_name.type() = "String";
		fileinarchive::meta_name.min() = 1;
		fileinarchive::meta_name.max() = 1;
		fileinarchive::meta_name.nonpersistent() = false;
		fileinarchive::meta_name.ordered() = false;
		fileinarchive::meta_name.visibility() = "public";
		fileinarchive::meta_name.defvalue() = vector<string>();
		componentfile::meta_type.name() = "type";
		componentfile::meta_type.type() = "String";
		componentfile::meta_type.min() = 0;
		componentfile::meta_type.max() = 1;
		componentfile::meta_type.nonpersistent() = false;
		componentfile::meta_type.ordered() = false;
		componentfile::meta_type.visibility() = "public";
		componentfile::meta_type.defvalue() = vector<string>();
		componentfile::meta_id.name() = "id";
		componentfile::meta_id.type() = "String";
		componentfile::meta_id.min() = 1;
		componentfile::meta_id.max() = 1;
		componentfile::meta_id.nonpersistent() = false;
		componentfile::meta_id.ordered() = false;
		componentfile::meta_id.visibility() = "public";
		componentfile::meta_id.defvalue() = vector<string>();
		publishesidentifier::meta_content.name() = "content";
		publishesidentifier::meta_content.type() = "String";
		publishesidentifier::meta_content.min() = 1;
		publishesidentifier::meta_content.max() = 1;
		publishesidentifier::meta_content.nonpersistent() = false;
		publishesidentifier::meta_content.ordered() = false;
		publishesidentifier::meta_content.visibility() = "public";
		publishesidentifier::meta_content.defvalue() = vector<string>();
		componentinstantiation::meta_id.name() = "id";
		componentinstantiation::meta_id.type() = "String";
		componentinstantiation::meta_id.min() = 1;
		componentinstantiation::meta_id.max() = 1;
		componentinstantiation::meta_id.nonpersistent() = false;
		componentinstantiation::meta_id.ordered() = false;
		componentinstantiation::meta_id.visibility() = "public";
		componentinstantiation::meta_id.defvalue() = vector<string>();
		connectinterface::meta_id.name() = "id";
		connectinterface::meta_id.type() = "String";
		connectinterface::meta_id.min() = 0;
		connectinterface::meta_id.max() = 1;
		connectinterface::meta_id.nonpersistent() = false;
		connectinterface::meta_id.ordered() = false;
		connectinterface::meta_id.visibility() = "public";
		connectinterface::meta_id.defvalue() = vector<string>();
		emitsidentifier::meta_content.name() = "content";
		emitsidentifier::meta_content.type() = "String";
		emitsidentifier::meta_content.min() = 1;
		emitsidentifier::meta_content.max() = 1;
		emitsidentifier::meta_content.nonpersistent() = false;
		emitsidentifier::meta_content.ordered() = false;
		emitsidentifier::meta_content.visibility() = "public";
		emitsidentifier::meta_content.defvalue() = vector<string>();
		consumesidentifier::meta_content.name() = "content";
		consumesidentifier::meta_content.type() = "String";
		consumesidentifier::meta_content.min() = 1;
		consumesidentifier::meta_content.max() = 1;
		consumesidentifier::meta_content.nonpersistent() = false;
		consumesidentifier::meta_content.ordered() = false;
		consumesidentifier::meta_content.visibility() = "public";
		consumesidentifier::meta_content.defvalue() = vector<string>();

		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			location::meta_hostcollocation_parent = Uml::CompositionParentRole::Create(comp);
			location::meta_hostcollocation_parent.name() = "hostcollocation_parent";
			location::meta_hostcollocation_parent.isNavigable() = true;
			location::meta_hostcollocation_parent.target() = hostcollocation::meta;
			hostcollocation::meta_location_children = Uml::CompositionChildRole::Create(comp);
			hostcollocation::meta_location_children.name() = "location";
			hostcollocation::meta_location_children.min() = 1;
			hostcollocation::meta_location_children.max() = -1;
			hostcollocation::meta_location_children.isNavigable() = true;
			hostcollocation::meta_location_children.target() = location::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			destination::meta_hostcollocation_parent = Uml::CompositionParentRole::Create(comp);
			destination::meta_hostcollocation_parent.name() = "hostcollocation_parent";
			destination::meta_hostcollocation_parent.isNavigable() = true;
			destination::meta_hostcollocation_parent.target() = hostcollocation::meta;
			hostcollocation::meta_destination_child = Uml::CompositionChildRole::Create(comp);
			hostcollocation::meta_destination_child.name() = "destination";
			hostcollocation::meta_destination_child.min() = 0;
			hostcollocation::meta_destination_child.max() = 1;
			hostcollocation::meta_destination_child.isNavigable() = true;
			hostcollocation::meta_destination_child.target() = destination::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			homeplacement::meta_processcollocation_parent = Uml::CompositionParentRole::Create(comp);
			homeplacement::meta_processcollocation_parent.name() = "processcollocation_parent";
			homeplacement::meta_processcollocation_parent.isNavigable() = true;
			homeplacement::meta_processcollocation_parent.target() = processcollocation::meta;
			processcollocation::meta_homeplacement_children = Uml::CompositionChildRole::Create(comp);
			processcollocation::meta_homeplacement_children.name() = "homeplacement";
			processcollocation::meta_homeplacement_children.min() = 1;
			processcollocation::meta_homeplacement_children.max() = -1;
			processcollocation::meta_homeplacement_children.isNavigable() = true;
			processcollocation::meta_homeplacement_children.target() = homeplacement::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			destination::meta_processcollocation_parent = Uml::CompositionParentRole::Create(comp);
			destination::meta_processcollocation_parent.name() = "processcollocation_parent";
			destination::meta_processcollocation_parent.isNavigable() = true;
			destination::meta_processcollocation_parent.target() = processcollocation::meta;
			processcollocation::meta_destination_child = Uml::CompositionChildRole::Create(comp);
			processcollocation::meta_destination_child.name() = "destination";
			processcollocation::meta_destination_child.min() = 0;
			processcollocation::meta_destination_child.max() = 1;
			processcollocation::meta_destination_child.isNavigable() = true;
			processcollocation::meta_destination_child.target() = destination::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			componentfileref::meta_homeplacement_parent = Uml::CompositionParentRole::Create(comp);
			componentfileref::meta_homeplacement_parent.name() = "homeplacement_parent";
			componentfileref::meta_homeplacement_parent.isNavigable() = true;
			componentfileref::meta_homeplacement_parent.target() = homeplacement::meta;
			homeplacement::meta_componentfileref_child = Uml::CompositionChildRole::Create(comp);
			homeplacement::meta_componentfileref_child.name() = "componentfileref";
			homeplacement::meta_componentfileref_child.min() = 1;
			homeplacement::meta_componentfileref_child.max() = 1;
			homeplacement::meta_componentfileref_child.isNavigable() = true;
			homeplacement::meta_componentfileref_child.target() = componentfileref::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			destination::meta_homeplacement_parent = Uml::CompositionParentRole::Create(comp);
			destination::meta_homeplacement_parent.name() = "homeplacement_parent";
			destination::meta_homeplacement_parent.isNavigable() = true;
			destination::meta_homeplacement_parent.target() = homeplacement::meta;
			homeplacement::meta_destination_child = Uml::CompositionChildRole::Create(comp);
			homeplacement::meta_destination_child.name() = "destination";
			homeplacement::meta_destination_child.min() = 0;
			homeplacement::meta_destination_child.max() = 1;
			homeplacement::meta_destination_child.isNavigable() = true;
			homeplacement::meta_destination_child.target() = destination::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			componentinstantiation::meta_homeplacement_parent = Uml::CompositionParentRole::Create(comp);
			componentinstantiation::meta_homeplacement_parent.name() = "homeplacement_parent";
			componentinstantiation::meta_homeplacement_parent.isNavigable() = true;
			componentinstantiation::meta_homeplacement_parent.target() = homeplacement::meta;
			homeplacement::meta_componentinstantiation_children = Uml::CompositionChildRole::Create(comp);
			homeplacement::meta_componentinstantiation_children.name() = "componentinstantiation";
			homeplacement::meta_componentinstantiation_children.min() = 0;
			homeplacement::meta_componentinstantiation_children.max() = -1;
			homeplacement::meta_componentinstantiation_children.isNavigable() = true;
			homeplacement::meta_componentinstantiation_children.target() = componentinstantiation::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			providesidentifier::meta_providesport_parent = Uml::CompositionParentRole::Create(comp);
			providesidentifier::meta_providesport_parent.name() = "providesport_parent";
			providesidentifier::meta_providesport_parent.isNavigable() = true;
			providesidentifier::meta_providesport_parent.target() = providesport::meta;
			providesport::meta_providesidentifier_child = Uml::CompositionChildRole::Create(comp);
			providesport::meta_providesidentifier_child.name() = "providesidentifier";
			providesport::meta_providesidentifier_child.min() = 1;
			providesport::meta_providesidentifier_child.max() = 1;
			providesport::meta_providesidentifier_child.isNavigable() = true;
			providesport::meta_providesidentifier_child.target() = providesidentifier::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			componentinstantiationref::meta_providesport_parent = Uml::CompositionParentRole::Create(comp);
			componentinstantiationref::meta_providesport_parent.name() = "providesport_parent";
			componentinstantiationref::meta_providesport_parent.isNavigable() = true;
			componentinstantiationref::meta_providesport_parent.target() = providesport::meta;
			providesport::meta_componentinstantiationref_child = Uml::CompositionChildRole::Create(comp);
			providesport::meta_componentinstantiationref_child.name() = "componentinstantiationref";
			providesport::meta_componentinstantiationref_child.min() = 1;
			providesport::meta_componentinstantiationref_child.max() = 1;
			providesport::meta_componentinstantiationref_child.isNavigable() = true;
			providesport::meta_componentinstantiationref_child.target() = componentinstantiationref::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			connectevent::meta_connections_parent = Uml::CompositionParentRole::Create(comp);
			connectevent::meta_connections_parent.name() = "connections_parent";
			connectevent::meta_connections_parent.isNavigable() = true;
			connectevent::meta_connections_parent.target() = connections::meta;
			connections::meta_connectevent_children = Uml::CompositionChildRole::Create(comp);
			connections::meta_connectevent_children.name() = "connectevent";
			connections::meta_connectevent_children.min() = 0;
			connections::meta_connectevent_children.max() = -1;
			connections::meta_connectevent_children.isNavigable() = true;
			connections::meta_connectevent_children.target() = connectevent::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			connectinterface::meta_connections_parent = Uml::CompositionParentRole::Create(comp);
			connectinterface::meta_connections_parent.name() = "connections_parent";
			connectinterface::meta_connections_parent.isNavigable() = true;
			connectinterface::meta_connections_parent.target() = connections::meta;
			connections::meta_connectinterface_children = Uml::CompositionChildRole::Create(comp);
			connections::meta_connectinterface_children.name() = "connectinterface";
			connections::meta_connectinterface_children.min() = 0;
			connections::meta_connectinterface_children.max() = -1;
			connections::meta_connectinterface_children.isNavigable() = true;
			connections::meta_connectinterface_children.target() = connectinterface::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			eventoutputport::meta_connectevent_parent = Uml::CompositionParentRole::Create(comp);
			eventoutputport::meta_connectevent_parent.name() = "connectevent_parent";
			eventoutputport::meta_connectevent_parent.isNavigable() = true;
			eventoutputport::meta_connectevent_parent.target() = connectevent::meta;
			connectevent::meta_eventoutputport_child = Uml::CompositionChildRole::Create(comp);
			connectevent::meta_eventoutputport_child.name() = "eventoutputport";
			connectevent::meta_eventoutputport_child.min() = 1;
			connectevent::meta_eventoutputport_child.max() = 1;
			connectevent::meta_eventoutputport_child.isNavigable() = true;
			connectevent::meta_eventoutputport_child.target() = eventoutputport::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			consumesport::meta_connectevent_parent = Uml::CompositionParentRole::Create(comp);
			consumesport::meta_connectevent_parent.name() = "connectevent_parent";
			consumesport::meta_connectevent_parent.isNavigable() = true;
			consumesport::meta_connectevent_parent.target() = connectevent::meta;
			connectevent::meta_consumesport_child = Uml::CompositionChildRole::Create(comp);
			connectevent::meta_consumesport_child.name() = "consumesport";
			connectevent::meta_consumesport_child.min() = 1;
			connectevent::meta_consumesport_child.max() = 1;
			connectevent::meta_consumesport_child.isNavigable() = true;
			connectevent::meta_consumesport_child.target() = consumesport::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			componentfiles::meta_componentassembly_parent = Uml::CompositionParentRole::Create(comp);
			componentfiles::meta_componentassembly_parent.name() = "componentassembly_parent";
			componentfiles::meta_componentassembly_parent.isNavigable() = true;
			componentfiles::meta_componentassembly_parent.target() = componentassembly::meta;
			componentassembly::meta_componentfiles_child = Uml::CompositionChildRole::Create(comp);
			componentassembly::meta_componentfiles_child.name() = "componentfiles";
			componentassembly::meta_componentfiles_child.min() = 1;
			componentassembly::meta_componentfiles_child.max() = 1;
			componentassembly::meta_componentfiles_child.isNavigable() = true;
			componentassembly::meta_componentfiles_child.target() = componentfiles::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			description::meta_componentassembly_parent = Uml::CompositionParentRole::Create(comp);
			description::meta_componentassembly_parent.name() = "componentassembly_parent";
			description::meta_componentassembly_parent.isNavigable() = true;
			description::meta_componentassembly_parent.target() = componentassembly::meta;
			componentassembly::meta_description_child = Uml::CompositionChildRole::Create(comp);
			componentassembly::meta_description_child.name() = "description";
			componentassembly::meta_description_child.min() = 0;
			componentassembly::meta_description_child.max() = 1;
			componentassembly::meta_description_child.isNavigable() = true;
			componentassembly::meta_description_child.target() = description::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			partitioning::meta_componentassembly_parent = Uml::CompositionParentRole::Create(comp);
			partitioning::meta_componentassembly_parent.name() = "componentassembly_parent";
			partitioning::meta_componentassembly_parent.isNavigable() = true;
			partitioning::meta_componentassembly_parent.target() = componentassembly::meta;
			componentassembly::meta_partitioning_child = Uml::CompositionChildRole::Create(comp);
			componentassembly::meta_partitioning_child.name() = "partitioning";
			componentassembly::meta_partitioning_child.min() = 1;
			componentassembly::meta_partitioning_child.max() = 1;
			componentassembly::meta_partitioning_child.isNavigable() = true;
			componentassembly::meta_partitioning_child.target() = partitioning::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			connections::meta_componentassembly_parent = Uml::CompositionParentRole::Create(comp);
			connections::meta_componentassembly_parent.name() = "componentassembly_parent";
			connections::meta_componentassembly_parent.isNavigable() = true;
			connections::meta_componentassembly_parent.target() = componentassembly::meta;
			componentassembly::meta_connections_child = Uml::CompositionChildRole::Create(comp);
			componentassembly::meta_connections_child.name() = "connections";
			componentassembly::meta_connections_child.min() = 0;
			componentassembly::meta_connections_child.max() = 1;
			componentassembly::meta_connections_child.isNavigable() = true;
			componentassembly::meta_connections_child.target() = connections::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			componentinstantiationref::meta_consumesport_parent = Uml::CompositionParentRole::Create(comp);
			componentinstantiationref::meta_consumesport_parent.name() = "consumesport_parent";
			componentinstantiationref::meta_consumesport_parent.isNavigable() = true;
			componentinstantiationref::meta_consumesport_parent.target() = consumesport::meta;
			consumesport::meta_componentinstantiationref_child = Uml::CompositionChildRole::Create(comp);
			consumesport::meta_componentinstantiationref_child.name() = "componentinstantiationref";
			consumesport::meta_componentinstantiationref_child.min() = 1;
			consumesport::meta_componentinstantiationref_child.max() = 1;
			consumesport::meta_componentinstantiationref_child.isNavigable() = true;
			consumesport::meta_componentinstantiationref_child.target() = componentinstantiationref::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			consumesidentifier::meta_consumesport_parent = Uml::CompositionParentRole::Create(comp);
			consumesidentifier::meta_consumesport_parent.name() = "consumesport_parent";
			consumesidentifier::meta_consumesport_parent.isNavigable() = true;
			consumesidentifier::meta_consumesport_parent.target() = consumesport::meta;
			consumesport::meta_consumesidentifier_child = Uml::CompositionChildRole::Create(comp);
			consumesport::meta_consumesidentifier_child.name() = "consumesidentifier";
			consumesport::meta_consumesidentifier_child.min() = 1;
			consumesport::meta_consumesidentifier_child.max() = 1;
			consumesport::meta_consumesidentifier_child.isNavigable() = true;
			consumesport::meta_consumesidentifier_child.target() = consumesidentifier::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			fileinarchive::meta_componentfile_parent = Uml::CompositionParentRole::Create(comp);
			fileinarchive::meta_componentfile_parent.name() = "componentfile_parent";
			fileinarchive::meta_componentfile_parent.isNavigable() = true;
			fileinarchive::meta_componentfile_parent.target() = componentfile::meta;
			componentfile::meta_fileinarchive_child = Uml::CompositionChildRole::Create(comp);
			componentfile::meta_fileinarchive_child.name() = "fileinarchive";
			componentfile::meta_fileinarchive_child.min() = 1;
			componentfile::meta_fileinarchive_child.max() = 1;
			componentfile::meta_fileinarchive_child.isNavigable() = true;
			componentfile::meta_fileinarchive_child.target() = fileinarchive::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			hostcollocation::meta_partitioning_parent = Uml::CompositionParentRole::Create(comp);
			hostcollocation::meta_partitioning_parent.name() = "partitioning_parent";
			hostcollocation::meta_partitioning_parent.isNavigable() = true;
			hostcollocation::meta_partitioning_parent.target() = partitioning::meta;
			partitioning::meta_hostcollocation_children = Uml::CompositionChildRole::Create(comp);
			partitioning::meta_hostcollocation_children.name() = "hostcollocation";
			partitioning::meta_hostcollocation_children.min() = 0;
			partitioning::meta_hostcollocation_children.max() = -1;
			partitioning::meta_hostcollocation_children.isNavigable() = true;
			partitioning::meta_hostcollocation_children.target() = hostcollocation::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			location::meta_partitioning_parent = Uml::CompositionParentRole::Create(comp);
			location::meta_partitioning_parent.name() = "partitioning_parent";
			location::meta_partitioning_parent.isNavigable() = true;
			location::meta_partitioning_parent.target() = partitioning::meta;
			partitioning::meta_location_children = Uml::CompositionChildRole::Create(comp);
			partitioning::meta_location_children.name() = "location";
			partitioning::meta_location_children.min() = 0;
			partitioning::meta_location_children.max() = -1;
			partitioning::meta_location_children.isNavigable() = true;
			partitioning::meta_location_children.target() = location::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			registercomponent::meta_componentinstantiation_parent = Uml::CompositionParentRole::Create(comp);
			registercomponent::meta_componentinstantiation_parent.name() = "componentinstantiation_parent";
			registercomponent::meta_componentinstantiation_parent.isNavigable() = true;
			registercomponent::meta_componentinstantiation_parent.target() = componentinstantiation::meta;
			componentinstantiation::meta_registercomponent_children = Uml::CompositionChildRole::Create(comp);
			componentinstantiation::meta_registercomponent_children.name() = "registercomponent";
			componentinstantiation::meta_registercomponent_children.min() = 0;
			componentinstantiation::meta_registercomponent_children.max() = -1;
			componentinstantiation::meta_registercomponent_children.isNavigable() = true;
			componentinstantiation::meta_registercomponent_children.target() = registercomponent::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			providesport::meta_connectinterface_parent = Uml::CompositionParentRole::Create(comp);
			providesport::meta_connectinterface_parent.name() = "connectinterface_parent";
			providesport::meta_connectinterface_parent.isNavigable() = true;
			providesport::meta_connectinterface_parent.target() = connectinterface::meta;
			connectinterface::meta_providesport_child = Uml::CompositionChildRole::Create(comp);
			connectinterface::meta_providesport_child.name() = "providesport";
			connectinterface::meta_providesport_child.min() = 1;
			connectinterface::meta_providesport_child.max() = 1;
			connectinterface::meta_providesport_child.isNavigable() = true;
			connectinterface::meta_providesport_child.target() = providesport::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			usesport::meta_connectinterface_parent = Uml::CompositionParentRole::Create(comp);
			usesport::meta_connectinterface_parent.name() = "connectinterface_parent";
			usesport::meta_connectinterface_parent.isNavigable() = true;
			usesport::meta_connectinterface_parent.target() = connectinterface::meta;
			connectinterface::meta_usesport_child = Uml::CompositionChildRole::Create(comp);
			connectinterface::meta_usesport_child.name() = "usesport";
			connectinterface::meta_usesport_child.min() = 1;
			connectinterface::meta_usesport_child.max() = 1;
			connectinterface::meta_usesport_child.isNavigable() = true;
			connectinterface::meta_usesport_child.target() = usesport::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			emitsidentifier::meta_emitsport_parent = Uml::CompositionParentRole::Create(comp);
			emitsidentifier::meta_emitsport_parent.name() = "emitsport_parent";
			emitsidentifier::meta_emitsport_parent.isNavigable() = true;
			emitsidentifier::meta_emitsport_parent.target() = emitsport::meta;
			emitsport::meta_emitsidentifier_child = Uml::CompositionChildRole::Create(comp);
			emitsport::meta_emitsidentifier_child.name() = "emitsidentifier";
			emitsport::meta_emitsidentifier_child.min() = 1;
			emitsport::meta_emitsidentifier_child.max() = 1;
			emitsport::meta_emitsidentifier_child.isNavigable() = true;
			emitsport::meta_emitsidentifier_child.target() = emitsidentifier::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			componentinstantiationref::meta_emitsport_parent = Uml::CompositionParentRole::Create(comp);
			componentinstantiationref::meta_emitsport_parent.name() = "emitsport_parent";
			componentinstantiationref::meta_emitsport_parent.isNavigable() = true;
			componentinstantiationref::meta_emitsport_parent.target() = emitsport::meta;
			emitsport::meta_componentinstantiationref_child = Uml::CompositionChildRole::Create(comp);
			emitsport::meta_componentinstantiationref_child.name() = "componentinstantiationref";
			emitsport::meta_componentinstantiationref_child.min() = 1;
			emitsport::meta_componentinstantiationref_child.max() = 1;
			emitsport::meta_componentinstantiationref_child.isNavigable() = true;
			emitsport::meta_componentinstantiationref_child.target() = componentinstantiationref::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			writeiortofile::meta_registercomponent_parent = Uml::CompositionParentRole::Create(comp);
			writeiortofile::meta_registercomponent_parent.name() = "registercomponent_parent";
			writeiortofile::meta_registercomponent_parent.isNavigable() = true;
			writeiortofile::meta_registercomponent_parent.target() = registercomponent::meta;
			registercomponent::meta_writeiortofile_children = Uml::CompositionChildRole::Create(comp);
			registercomponent::meta_writeiortofile_children.name() = "writeiortofile";
			registercomponent::meta_writeiortofile_children.min() = 0;
			registercomponent::meta_writeiortofile_children.max() = -1;
			registercomponent::meta_writeiortofile_children.isNavigable() = true;
			registercomponent::meta_writeiortofile_children.target() = writeiortofile::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			componentinstantiationref::meta_publishesport_parent = Uml::CompositionParentRole::Create(comp);
			componentinstantiationref::meta_publishesport_parent.name() = "publishesport_parent";
			componentinstantiationref::meta_publishesport_parent.isNavigable() = true;
			componentinstantiationref::meta_publishesport_parent.target() = publishesport::meta;
			publishesport::meta_componentinstantiationref_child = Uml::CompositionChildRole::Create(comp);
			publishesport::meta_componentinstantiationref_child.name() = "componentinstantiationref";
			publishesport::meta_componentinstantiationref_child.min() = 1;
			publishesport::meta_componentinstantiationref_child.max() = 1;
			publishesport::meta_componentinstantiationref_child.isNavigable() = true;
			publishesport::meta_componentinstantiationref_child.target() = componentinstantiationref::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			publishesidentifier::meta_publishesport_parent = Uml::CompositionParentRole::Create(comp);
			publishesidentifier::meta_publishesport_parent.name() = "publishesport_parent";
			publishesidentifier::meta_publishesport_parent.isNavigable() = true;
			publishesidentifier::meta_publishesport_parent.target() = publishesport::meta;
			publishesport::meta_publishesidentifier_child = Uml::CompositionChildRole::Create(comp);
			publishesport::meta_publishesidentifier_child.name() = "publishesidentifier";
			publishesport::meta_publishesidentifier_child.min() = 1;
			publishesport::meta_publishesidentifier_child.max() = 1;
			publishesport::meta_publishesidentifier_child.isNavigable() = true;
			publishesport::meta_publishesidentifier_child.target() = publishesidentifier::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			componentfile::meta_componentfiles_parent = Uml::CompositionParentRole::Create(comp);
			componentfile::meta_componentfiles_parent.name() = "componentfiles_parent";
			componentfile::meta_componentfiles_parent.isNavigable() = true;
			componentfile::meta_componentfiles_parent.target() = componentfiles::meta;
			componentfiles::meta_componentfile_children = Uml::CompositionChildRole::Create(comp);
			componentfiles::meta_componentfile_children.name() = "componentfile";
			componentfiles::meta_componentfile_children.min() = 1;
			componentfiles::meta_componentfile_children.max() = -1;
			componentfiles::meta_componentfile_children.isNavigable() = true;
			componentfiles::meta_componentfile_children.target() = componentfile::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			usesidentifier::meta_usesport_parent = Uml::CompositionParentRole::Create(comp);
			usesidentifier::meta_usesport_parent.name() = "usesport_parent";
			usesidentifier::meta_usesport_parent.isNavigable() = true;
			usesidentifier::meta_usesport_parent.target() = usesport::meta;
			usesport::meta_usesidentifier_child = Uml::CompositionChildRole::Create(comp);
			usesport::meta_usesidentifier_child.name() = "usesidentifier";
			usesport::meta_usesidentifier_child.min() = 1;
			usesport::meta_usesidentifier_child.max() = 1;
			usesport::meta_usesidentifier_child.isNavigable() = true;
			usesport::meta_usesidentifier_child.target() = usesidentifier::meta;
		}
		{
			Uml::Composition comp = Uml::Composition::Create(umldiagram);
			comp.nonpersistent() =false;

			componentinstantiationref::meta_usesport_parent = Uml::CompositionParentRole::Create(comp);
			componentinstantiationref::meta_usesport_parent.name() = "usesport_parent";
			componentinstantiationref::meta_usesport_parent.isNavigable() = true;
			componentinstantiationref::meta_usesport_parent.target() = usesport::meta;
			usesport::meta_componentinstantiationref_child = Uml::CompositionChildRole::Create(comp);
			usesport::meta_componentinstantiationref_child.name() = "componentinstantiationref";
			usesport::meta_componentinstantiationref_child.min() = 1;
			usesport::meta_componentinstantiationref_child.max() = 1;
			usesport::meta_componentinstantiationref_child.isNavigable() = true;
			usesport::meta_componentinstantiationref_child.target() = componentinstantiationref::meta;
		}
		location::meta.subTypes() +=  processcollocation::meta;
		location::meta.subTypes() +=  homeplacement::meta;
		eventoutputport::meta.subTypes() +=  emitsport::meta;
		eventoutputport::meta.subTypes() +=  publishesport::meta;

	}
	Udm::UdmDiagram diagram = { &umldiagram, Initialize };
}
// END ccm_assembly.cpp
