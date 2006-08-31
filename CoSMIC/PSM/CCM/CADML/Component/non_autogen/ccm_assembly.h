#ifndef MOBIES_CCM_ASSEMBLY_H
#define MOBIES_CCM_ASSEMBLY_H
// header file ccm_assembly.h generated from diagram ccm_assembly
// generated on Wed Jul 30 14:52:48 2003

#ifndef MOBIES_UDMBASE_H
#include "UdmBase.h"
#endif

namespace ccm_assembly {

	typedef Udm::Object Object;

	class destination;
	class writeiortofile;
	class providesidentifier;
	class hostcollocation;
	class processcollocation;
	class location;
	class homeplacement;
	class providesport;
	class connections;
	class connectevent;
	class usesidentifier;
	class componentassembly;
	class description;
	class componentinstantiationref;
	class componentfileref;
	class eventoutputport;
	class consumesport;
	class fileinarchive;
	class componentfile;
	class publishesidentifier;
	class partitioning;
	class componentinstantiation;
	class connectinterface;
	class emitsport;
	class registercomponent;
	class publishesport;
	class componentfiles;
	class emitsidentifier;
	class consumesidentifier;
	class usesport;

	void Initialize();
	extern Udm::UdmDiagram diagram;

	class destination :  public Object {
	 public:
		static Uml::Class meta;

		destination() { }
		destination(Udm::ObjectImpl *impl) : Object(impl) { }
		destination(const destination &master) : Object(master) { }
		static destination Cast(const Object &a) { return __Cast(a, meta); }

		static destination Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		destination CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		destination CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<destination> Instances() { return Udm::InstantiatedAttr<ccm_assembly::destination>(impl);}

		Udm::DerivedAttr<ccm_assembly::destination> Derived() { return Udm::DerivedAttr<ccm_assembly::destination>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::destination> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::destination>(impl);}

		static Uml::Attribute meta_content;
		Udm::StringAttr content() const { return Udm::StringAttr(impl, meta_content); }

		static Uml::CompositionParentRole meta_processcollocation_parent;
		Udm::ParentAttr<ccm_assembly::processcollocation> processcollocation_parent() const { return Udm::ParentAttr<ccm_assembly::processcollocation>(impl, meta_processcollocation_parent); }

		static Uml::CompositionParentRole meta_hostcollocation_parent;
		Udm::ParentAttr<ccm_assembly::hostcollocation> hostcollocation_parent() const { return Udm::ParentAttr<ccm_assembly::hostcollocation>(impl, meta_hostcollocation_parent); }

		static Uml::CompositionParentRole meta_homeplacement_parent;
		Udm::ParentAttr<ccm_assembly::homeplacement> homeplacement_parent() const { return Udm::ParentAttr<ccm_assembly::homeplacement>(impl, meta_homeplacement_parent); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class writeiortofile :  public Object {
	 public:
		static Uml::Class meta;

		writeiortofile() { }
		writeiortofile(Udm::ObjectImpl *impl) : Object(impl) { }
		writeiortofile(const writeiortofile &master) : Object(master) { }
		static writeiortofile Cast(const Object &a) { return __Cast(a, meta); }

		static writeiortofile Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		writeiortofile CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		writeiortofile CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<writeiortofile> Instances() { return Udm::InstantiatedAttr<ccm_assembly::writeiortofile>(impl);}

		Udm::DerivedAttr<ccm_assembly::writeiortofile> Derived() { return Udm::DerivedAttr<ccm_assembly::writeiortofile>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::writeiortofile> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::writeiortofile>(impl);}

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::CompositionParentRole meta_registercomponent_parent;
		Udm::ParentAttr<ccm_assembly::registercomponent> registercomponent_parent() const { return Udm::ParentAttr<ccm_assembly::registercomponent>(impl, meta_registercomponent_parent); }

		Udm::ParentAttr<ccm_assembly::registercomponent> parent() const { return Udm::ParentAttr<ccm_assembly::registercomponent>(impl, Udm::NULLPARENTROLE); }
	};

	class providesidentifier :  public Object {
	 public:
		static Uml::Class meta;

		providesidentifier() { }
		providesidentifier(Udm::ObjectImpl *impl) : Object(impl) { }
		providesidentifier(const providesidentifier &master) : Object(master) { }
		static providesidentifier Cast(const Object &a) { return __Cast(a, meta); }

		static providesidentifier Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		providesidentifier CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		providesidentifier CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<providesidentifier> Instances() { return Udm::InstantiatedAttr<ccm_assembly::providesidentifier>(impl);}

		Udm::DerivedAttr<ccm_assembly::providesidentifier> Derived() { return Udm::DerivedAttr<ccm_assembly::providesidentifier>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::providesidentifier> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::providesidentifier>(impl);}

		static Uml::Attribute meta_content;
		Udm::StringAttr content() const { return Udm::StringAttr(impl, meta_content); }

		static Uml::CompositionParentRole meta_providesport_parent;
		Udm::ParentAttr<ccm_assembly::providesport> providesport_parent() const { return Udm::ParentAttr<ccm_assembly::providesport>(impl, meta_providesport_parent); }

		Udm::ParentAttr<ccm_assembly::providesport> parent() const { return Udm::ParentAttr<ccm_assembly::providesport>(impl, Udm::NULLPARENTROLE); }
	};

	class hostcollocation :  public Object {
	 public:
		static Uml::Class meta;

		hostcollocation() { }
		hostcollocation(Udm::ObjectImpl *impl) : Object(impl) { }
		hostcollocation(const hostcollocation &master) : Object(master) { }
		static hostcollocation Cast(const Object &a) { return __Cast(a, meta); }

		static hostcollocation Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		hostcollocation CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		hostcollocation CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<hostcollocation> Instances() { return Udm::InstantiatedAttr<ccm_assembly::hostcollocation>(impl);}

		Udm::DerivedAttr<ccm_assembly::hostcollocation> Derived() { return Udm::DerivedAttr<ccm_assembly::hostcollocation>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::hostcollocation> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::hostcollocation>(impl);}

		static Uml::Attribute meta_id;
		Udm::StringAttr id() const { return Udm::StringAttr(impl, meta_id); }

		static Uml::CompositionChildRole meta_location_children;
		Udm::ChildrenAttr<ccm_assembly::location> location_children() const { return Udm::ChildrenAttr<ccm_assembly::location>(impl, meta_location_children); }

		static Uml::CompositionChildRole meta_destination_child;
		Udm::ChildAttr<ccm_assembly::destination> destination_child() const { return Udm::ChildAttr<ccm_assembly::destination>(impl, meta_destination_child); }

		Udm::ChildrenAttr<ccm_assembly::destination> destination_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::destination>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::processcollocation> processcollocation_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::processcollocation>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::location> location_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::location>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::homeplacement> homeplacement_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::homeplacement>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_partitioning_parent;
		Udm::ParentAttr<ccm_assembly::partitioning> partitioning_parent() const { return Udm::ParentAttr<ccm_assembly::partitioning>(impl, meta_partitioning_parent); }

		Udm::ParentAttr<ccm_assembly::partitioning> parent() const { return Udm::ParentAttr<ccm_assembly::partitioning>(impl, Udm::NULLPARENTROLE); }
	};

	class location :  public Object {
	 public:
		static Uml::Class meta;

		location() { }
		location(Udm::ObjectImpl *impl) : Object(impl) { }
		location(const location &master) : Object(master) { }
		static location Cast(const Object &a) { return __Cast(a, meta); }

		static location Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		location CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		location CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<location> Instances() { return Udm::InstantiatedAttr<ccm_assembly::location>(impl);}

		Udm::DerivedAttr<ccm_assembly::location> Derived() { return Udm::DerivedAttr<ccm_assembly::location>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::location> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::location>(impl);}

		static Uml::CompositionParentRole meta_hostcollocation_parent;
		Udm::ParentAttr<ccm_assembly::hostcollocation> hostcollocation_parent() const { return Udm::ParentAttr<ccm_assembly::hostcollocation>(impl, meta_hostcollocation_parent); }

		static Uml::CompositionParentRole meta_partitioning_parent;
		Udm::ParentAttr<ccm_assembly::partitioning> partitioning_parent() const { return Udm::ParentAttr<ccm_assembly::partitioning>(impl, meta_partitioning_parent); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class processcollocation :  public location {
	 public:
		static Uml::Class meta;

		processcollocation() { }
		processcollocation(Udm::ObjectImpl *impl) : location(impl) { }
		processcollocation(const processcollocation &master) : location(master) { }
		static processcollocation Cast(const Object &a) { return __Cast(a, meta); }

		static processcollocation Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		processcollocation CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		processcollocation CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<processcollocation> Instances() { return Udm::InstantiatedAttr<ccm_assembly::processcollocation>(impl);}

		Udm::DerivedAttr<ccm_assembly::processcollocation> Derived() { return Udm::DerivedAttr<ccm_assembly::processcollocation>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::processcollocation> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::processcollocation>(impl);}

		static Uml::Attribute meta_id;
		Udm::StringAttr id() const { return Udm::StringAttr(impl, meta_id); }

		static Uml::CompositionChildRole meta_homeplacement_children;
		Udm::ChildrenAttr<ccm_assembly::homeplacement> homeplacement_children() const { return Udm::ChildrenAttr<ccm_assembly::homeplacement>(impl, meta_homeplacement_children); }

		static Uml::CompositionChildRole meta_destination_child;
		Udm::ChildAttr<ccm_assembly::destination> destination_child() const { return Udm::ChildAttr<ccm_assembly::destination>(impl, meta_destination_child); }

		Udm::ChildrenAttr<ccm_assembly::destination> destination_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::destination>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::location> location_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::location>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::homeplacement> homeplacement_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::homeplacement>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class homeplacement :  public location {
	 public:
		static Uml::Class meta;

		homeplacement() { }
		homeplacement(Udm::ObjectImpl *impl) : location(impl) { }
		homeplacement(const homeplacement &master) : location(master) { }
		static homeplacement Cast(const Object &a) { return __Cast(a, meta); }

		static homeplacement Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		homeplacement CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		homeplacement CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<homeplacement> Instances() { return Udm::InstantiatedAttr<ccm_assembly::homeplacement>(impl);}

		Udm::DerivedAttr<ccm_assembly::homeplacement> Derived() { return Udm::DerivedAttr<ccm_assembly::homeplacement>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::homeplacement> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::homeplacement>(impl);}

		static Uml::Attribute meta_id;
		Udm::StringAttr id() const { return Udm::StringAttr(impl, meta_id); }

		static Uml::CompositionChildRole meta_componentfileref_child;
		Udm::ChildAttr<ccm_assembly::componentfileref> componentfileref_child() const { return Udm::ChildAttr<ccm_assembly::componentfileref>(impl, meta_componentfileref_child); }

		static Uml::CompositionChildRole meta_destination_child;
		Udm::ChildAttr<ccm_assembly::destination> destination_child() const { return Udm::ChildAttr<ccm_assembly::destination>(impl, meta_destination_child); }

		static Uml::CompositionChildRole meta_componentinstantiation_children;
		Udm::ChildrenAttr<ccm_assembly::componentinstantiation> componentinstantiation_children() const { return Udm::ChildrenAttr<ccm_assembly::componentinstantiation>(impl, meta_componentinstantiation_children); }

		Udm::ChildrenAttr<ccm_assembly::destination> destination_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::destination>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::componentfileref> componentfileref_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::componentfileref>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::componentinstantiation> componentinstantiation_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::componentinstantiation>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_processcollocation_parent;
		Udm::ParentAttr<ccm_assembly::processcollocation> processcollocation_parent() const { return Udm::ParentAttr<ccm_assembly::processcollocation>(impl, meta_processcollocation_parent); }

		Udm::ParentAttr<ccm_assembly::location> parent() const { return Udm::ParentAttr<ccm_assembly::location>(impl, Udm::NULLPARENTROLE); }
	};

	class providesport :  public Object {
	 public:
		static Uml::Class meta;

		providesport() { }
		providesport(Udm::ObjectImpl *impl) : Object(impl) { }
		providesport(const providesport &master) : Object(master) { }
		static providesport Cast(const Object &a) { return __Cast(a, meta); }

		static providesport Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		providesport CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		providesport CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<providesport> Instances() { return Udm::InstantiatedAttr<ccm_assembly::providesport>(impl);}

		Udm::DerivedAttr<ccm_assembly::providesport> Derived() { return Udm::DerivedAttr<ccm_assembly::providesport>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::providesport> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::providesport>(impl);}

		static Uml::CompositionChildRole meta_providesidentifier_child;
		Udm::ChildAttr<ccm_assembly::providesidentifier> providesidentifier_child() const { return Udm::ChildAttr<ccm_assembly::providesidentifier>(impl, meta_providesidentifier_child); }

		static Uml::CompositionChildRole meta_componentinstantiationref_child;
		Udm::ChildAttr<ccm_assembly::componentinstantiationref> componentinstantiationref_child() const { return Udm::ChildAttr<ccm_assembly::componentinstantiationref>(impl, meta_componentinstantiationref_child); }

		Udm::ChildrenAttr<ccm_assembly::providesidentifier> providesidentifier_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::providesidentifier>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::componentinstantiationref> componentinstantiationref_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::componentinstantiationref>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_connectinterface_parent;
		Udm::ParentAttr<ccm_assembly::connectinterface> connectinterface_parent() const { return Udm::ParentAttr<ccm_assembly::connectinterface>(impl, meta_connectinterface_parent); }

		Udm::ParentAttr<ccm_assembly::connectinterface> parent() const { return Udm::ParentAttr<ccm_assembly::connectinterface>(impl, Udm::NULLPARENTROLE); }
	};

	class connections :  public Object {
	 public:
		static Uml::Class meta;

		connections() { }
		connections(Udm::ObjectImpl *impl) : Object(impl) { }
		connections(const connections &master) : Object(master) { }
		static connections Cast(const Object &a) { return __Cast(a, meta); }

		static connections Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		connections CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		connections CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<connections> Instances() { return Udm::InstantiatedAttr<ccm_assembly::connections>(impl);}

		Udm::DerivedAttr<ccm_assembly::connections> Derived() { return Udm::DerivedAttr<ccm_assembly::connections>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::connections> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::connections>(impl);}

		static Uml::CompositionChildRole meta_connectevent_children;
		Udm::ChildrenAttr<ccm_assembly::connectevent> connectevent_children() const { return Udm::ChildrenAttr<ccm_assembly::connectevent>(impl, meta_connectevent_children); }

		static Uml::CompositionChildRole meta_connectinterface_children;
		Udm::ChildrenAttr<ccm_assembly::connectinterface> connectinterface_children() const { return Udm::ChildrenAttr<ccm_assembly::connectinterface>(impl, meta_connectinterface_children); }

		Udm::ChildrenAttr<ccm_assembly::connectevent> connectevent_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::connectevent>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::connectinterface> connectinterface_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::connectinterface>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_componentassembly_parent;
		Udm::ParentAttr<ccm_assembly::componentassembly> componentassembly_parent() const { return Udm::ParentAttr<ccm_assembly::componentassembly>(impl, meta_componentassembly_parent); }

		Udm::ParentAttr<ccm_assembly::componentassembly> parent() const { return Udm::ParentAttr<ccm_assembly::componentassembly>(impl, Udm::NULLPARENTROLE); }
	};

	class connectevent :  public Object {
	 public:
		static Uml::Class meta;

		connectevent() { }
		connectevent(Udm::ObjectImpl *impl) : Object(impl) { }
		connectevent(const connectevent &master) : Object(master) { }
		static connectevent Cast(const Object &a) { return __Cast(a, meta); }

		static connectevent Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		connectevent CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		connectevent CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<connectevent> Instances() { return Udm::InstantiatedAttr<ccm_assembly::connectevent>(impl);}

		Udm::DerivedAttr<ccm_assembly::connectevent> Derived() { return Udm::DerivedAttr<ccm_assembly::connectevent>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::connectevent> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::connectevent>(impl);}

		static Uml::Attribute meta_id;
		Udm::StringAttr id() const { return Udm::StringAttr(impl, meta_id); }

		static Uml::CompositionChildRole meta_eventoutputport_child;
		Udm::ChildAttr<ccm_assembly::eventoutputport> eventoutputport_child() const { return Udm::ChildAttr<ccm_assembly::eventoutputport>(impl, meta_eventoutputport_child); }

		static Uml::CompositionChildRole meta_consumesport_child;
		Udm::ChildAttr<ccm_assembly::consumesport> consumesport_child() const { return Udm::ChildAttr<ccm_assembly::consumesport>(impl, meta_consumesport_child); }

		Udm::ChildrenAttr<ccm_assembly::eventoutputport> eventoutputport_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::eventoutputport>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::consumesport> consumesport_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::consumesport>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::emitsport> emitsport_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::emitsport>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::publishesport> publishesport_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::publishesport>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_connections_parent;
		Udm::ParentAttr<ccm_assembly::connections> connections_parent() const { return Udm::ParentAttr<ccm_assembly::connections>(impl, meta_connections_parent); }

		Udm::ParentAttr<ccm_assembly::connections> parent() const { return Udm::ParentAttr<ccm_assembly::connections>(impl, Udm::NULLPARENTROLE); }
	};

	class usesidentifier :  public Object {
	 public:
		static Uml::Class meta;

		usesidentifier() { }
		usesidentifier(Udm::ObjectImpl *impl) : Object(impl) { }
		usesidentifier(const usesidentifier &master) : Object(master) { }
		static usesidentifier Cast(const Object &a) { return __Cast(a, meta); }

		static usesidentifier Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		usesidentifier CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		usesidentifier CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<usesidentifier> Instances() { return Udm::InstantiatedAttr<ccm_assembly::usesidentifier>(impl);}

		Udm::DerivedAttr<ccm_assembly::usesidentifier> Derived() { return Udm::DerivedAttr<ccm_assembly::usesidentifier>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::usesidentifier> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::usesidentifier>(impl);}

		static Uml::Attribute meta_content;
		Udm::StringAttr content() const { return Udm::StringAttr(impl, meta_content); }

		static Uml::CompositionParentRole meta_usesport_parent;
		Udm::ParentAttr<ccm_assembly::usesport> usesport_parent() const { return Udm::ParentAttr<ccm_assembly::usesport>(impl, meta_usesport_parent); }

		Udm::ParentAttr<ccm_assembly::usesport> parent() const { return Udm::ParentAttr<ccm_assembly::usesport>(impl, Udm::NULLPARENTROLE); }
	};

	class componentassembly :  public Object {
	 public:
		static Uml::Class meta;

		componentassembly() { }
		componentassembly(Udm::ObjectImpl *impl) : Object(impl) { }
		componentassembly(const componentassembly &master) : Object(master) { }
		static componentassembly Cast(const Object &a) { return __Cast(a, meta); }

		static componentassembly Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		componentassembly CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		componentassembly CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<componentassembly> Instances() { return Udm::InstantiatedAttr<ccm_assembly::componentassembly>(impl);}

		Udm::DerivedAttr<ccm_assembly::componentassembly> Derived() { return Udm::DerivedAttr<ccm_assembly::componentassembly>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::componentassembly> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::componentassembly>(impl);}

		static Uml::Attribute meta_derivedfrom;
		Udm::StringAttr derivedfrom() const { return Udm::StringAttr(impl, meta_derivedfrom); }

		static Uml::Attribute meta_id;
		Udm::StringAttr id() const { return Udm::StringAttr(impl, meta_id); }

		static Uml::CompositionChildRole meta_componentfiles_child;
		Udm::ChildAttr<ccm_assembly::componentfiles> componentfiles_child() const { return Udm::ChildAttr<ccm_assembly::componentfiles>(impl, meta_componentfiles_child); }

		static Uml::CompositionChildRole meta_description_child;
		Udm::ChildAttr<ccm_assembly::description> description_child() const { return Udm::ChildAttr<ccm_assembly::description>(impl, meta_description_child); }

		static Uml::CompositionChildRole meta_partitioning_child;
		Udm::ChildAttr<ccm_assembly::partitioning> partitioning_child() const { return Udm::ChildAttr<ccm_assembly::partitioning>(impl, meta_partitioning_child); }

		static Uml::CompositionChildRole meta_connections_child;
		Udm::ChildAttr<ccm_assembly::connections> connections_child() const { return Udm::ChildAttr<ccm_assembly::connections>(impl, meta_connections_child); }

		Udm::ChildrenAttr<ccm_assembly::connections> connections_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::connections>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::description> description_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::description>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::partitioning> partitioning_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::partitioning>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::componentfiles> componentfiles_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::componentfiles>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class description :  public Object {
	 public:
		static Uml::Class meta;

		description() { }
		description(Udm::ObjectImpl *impl) : Object(impl) { }
		description(const description &master) : Object(master) { }
		static description Cast(const Object &a) { return __Cast(a, meta); }

		static description Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		description CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		description CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<description> Instances() { return Udm::InstantiatedAttr<ccm_assembly::description>(impl);}

		Udm::DerivedAttr<ccm_assembly::description> Derived() { return Udm::DerivedAttr<ccm_assembly::description>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::description> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::description>(impl);}

		static Uml::Attribute meta_content;
		Udm::StringAttr content() const { return Udm::StringAttr(impl, meta_content); }

		static Uml::CompositionParentRole meta_componentassembly_parent;
		Udm::ParentAttr<ccm_assembly::componentassembly> componentassembly_parent() const { return Udm::ParentAttr<ccm_assembly::componentassembly>(impl, meta_componentassembly_parent); }

		Udm::ParentAttr<ccm_assembly::componentassembly> parent() const { return Udm::ParentAttr<ccm_assembly::componentassembly>(impl, Udm::NULLPARENTROLE); }
	};

	class componentinstantiationref :  public Object {
	 public:
		static Uml::Class meta;

		componentinstantiationref() { }
		componentinstantiationref(Udm::ObjectImpl *impl) : Object(impl) { }
		componentinstantiationref(const componentinstantiationref &master) : Object(master) { }
		static componentinstantiationref Cast(const Object &a) { return __Cast(a, meta); }

		static componentinstantiationref Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		componentinstantiationref CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		componentinstantiationref CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<componentinstantiationref> Instances() { return Udm::InstantiatedAttr<ccm_assembly::componentinstantiationref>(impl);}

		Udm::DerivedAttr<ccm_assembly::componentinstantiationref> Derived() { return Udm::DerivedAttr<ccm_assembly::componentinstantiationref>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::componentinstantiationref> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::componentinstantiationref>(impl);}

		static Uml::Attribute meta_idref;
		Udm::StringAttr idref() const { return Udm::StringAttr(impl, meta_idref); }

		static Uml::CompositionParentRole meta_providesport_parent;
		Udm::ParentAttr<ccm_assembly::providesport> providesport_parent() const { return Udm::ParentAttr<ccm_assembly::providesport>(impl, meta_providesport_parent); }

		static Uml::CompositionParentRole meta_consumesport_parent;
		Udm::ParentAttr<ccm_assembly::consumesport> consumesport_parent() const { return Udm::ParentAttr<ccm_assembly::consumesport>(impl, meta_consumesport_parent); }

		static Uml::CompositionParentRole meta_emitsport_parent;
		Udm::ParentAttr<ccm_assembly::emitsport> emitsport_parent() const { return Udm::ParentAttr<ccm_assembly::emitsport>(impl, meta_emitsport_parent); }

		static Uml::CompositionParentRole meta_publishesport_parent;
		Udm::ParentAttr<ccm_assembly::publishesport> publishesport_parent() const { return Udm::ParentAttr<ccm_assembly::publishesport>(impl, meta_publishesport_parent); }

		static Uml::CompositionParentRole meta_usesport_parent;
		Udm::ParentAttr<ccm_assembly::usesport> usesport_parent() const { return Udm::ParentAttr<ccm_assembly::usesport>(impl, meta_usesport_parent); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class componentfileref :  public Object {
	 public:
		static Uml::Class meta;

		componentfileref() { }
		componentfileref(Udm::ObjectImpl *impl) : Object(impl) { }
		componentfileref(const componentfileref &master) : Object(master) { }
		static componentfileref Cast(const Object &a) { return __Cast(a, meta); }

		static componentfileref Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		componentfileref CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		componentfileref CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<componentfileref> Instances() { return Udm::InstantiatedAttr<ccm_assembly::componentfileref>(impl);}

		Udm::DerivedAttr<ccm_assembly::componentfileref> Derived() { return Udm::DerivedAttr<ccm_assembly::componentfileref>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::componentfileref> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::componentfileref>(impl);}

		static Uml::Attribute meta_idref;
		Udm::StringAttr idref() const { return Udm::StringAttr(impl, meta_idref); }

		static Uml::CompositionParentRole meta_homeplacement_parent;
		Udm::ParentAttr<ccm_assembly::homeplacement> homeplacement_parent() const { return Udm::ParentAttr<ccm_assembly::homeplacement>(impl, meta_homeplacement_parent); }

		Udm::ParentAttr<ccm_assembly::location> parent() const { return Udm::ParentAttr<ccm_assembly::location>(impl, Udm::NULLPARENTROLE); }
	};

	class eventoutputport :  public Object {
	 public:
		static Uml::Class meta;

		eventoutputport() { }
		eventoutputport(Udm::ObjectImpl *impl) : Object(impl) { }
		eventoutputport(const eventoutputport &master) : Object(master) { }
		static eventoutputport Cast(const Object &a) { return __Cast(a, meta); }

		static eventoutputport Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		eventoutputport CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		eventoutputport CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<eventoutputport> Instances() { return Udm::InstantiatedAttr<ccm_assembly::eventoutputport>(impl);}

		Udm::DerivedAttr<ccm_assembly::eventoutputport> Derived() { return Udm::DerivedAttr<ccm_assembly::eventoutputport>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::eventoutputport> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::eventoutputport>(impl);}

		static Uml::CompositionParentRole meta_connectevent_parent;
		Udm::ParentAttr<ccm_assembly::connectevent> connectevent_parent() const { return Udm::ParentAttr<ccm_assembly::connectevent>(impl, meta_connectevent_parent); }

		Udm::ParentAttr<ccm_assembly::connectevent> parent() const { return Udm::ParentAttr<ccm_assembly::connectevent>(impl, Udm::NULLPARENTROLE); }
	};

	class consumesport :  public Object {
	 public:
		static Uml::Class meta;

		consumesport() { }
		consumesport(Udm::ObjectImpl *impl) : Object(impl) { }
		consumesport(const consumesport &master) : Object(master) { }
		static consumesport Cast(const Object &a) { return __Cast(a, meta); }

		static consumesport Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		consumesport CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		consumesport CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<consumesport> Instances() { return Udm::InstantiatedAttr<ccm_assembly::consumesport>(impl);}

		Udm::DerivedAttr<ccm_assembly::consumesport> Derived() { return Udm::DerivedAttr<ccm_assembly::consumesport>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::consumesport> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::consumesport>(impl);}

		static Uml::CompositionChildRole meta_componentinstantiationref_child;
		Udm::ChildAttr<ccm_assembly::componentinstantiationref> componentinstantiationref_child() const { return Udm::ChildAttr<ccm_assembly::componentinstantiationref>(impl, meta_componentinstantiationref_child); }

		static Uml::CompositionChildRole meta_consumesidentifier_child;
		Udm::ChildAttr<ccm_assembly::consumesidentifier> consumesidentifier_child() const { return Udm::ChildAttr<ccm_assembly::consumesidentifier>(impl, meta_consumesidentifier_child); }

		Udm::ChildrenAttr<ccm_assembly::componentinstantiationref> componentinstantiationref_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::componentinstantiationref>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::consumesidentifier> consumesidentifier_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::consumesidentifier>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_connectevent_parent;
		Udm::ParentAttr<ccm_assembly::connectevent> connectevent_parent() const { return Udm::ParentAttr<ccm_assembly::connectevent>(impl, meta_connectevent_parent); }

		Udm::ParentAttr<ccm_assembly::connectevent> parent() const { return Udm::ParentAttr<ccm_assembly::connectevent>(impl, Udm::NULLPARENTROLE); }
	};

	class fileinarchive :  public Object {
	 public:
		static Uml::Class meta;

		fileinarchive() { }
		fileinarchive(Udm::ObjectImpl *impl) : Object(impl) { }
		fileinarchive(const fileinarchive &master) : Object(master) { }
		static fileinarchive Cast(const Object &a) { return __Cast(a, meta); }

		static fileinarchive Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		fileinarchive CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		fileinarchive CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<fileinarchive> Instances() { return Udm::InstantiatedAttr<ccm_assembly::fileinarchive>(impl);}

		Udm::DerivedAttr<ccm_assembly::fileinarchive> Derived() { return Udm::DerivedAttr<ccm_assembly::fileinarchive>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::fileinarchive> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::fileinarchive>(impl);}

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::CompositionParentRole meta_componentfile_parent;
		Udm::ParentAttr<ccm_assembly::componentfile> componentfile_parent() const { return Udm::ParentAttr<ccm_assembly::componentfile>(impl, meta_componentfile_parent); }

		Udm::ParentAttr<ccm_assembly::componentfile> parent() const { return Udm::ParentAttr<ccm_assembly::componentfile>(impl, Udm::NULLPARENTROLE); }
	};

	class componentfile :  public Object {
	 public:
		static Uml::Class meta;

		componentfile() { }
		componentfile(Udm::ObjectImpl *impl) : Object(impl) { }
		componentfile(const componentfile &master) : Object(master) { }
		static componentfile Cast(const Object &a) { return __Cast(a, meta); }

		static componentfile Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		componentfile CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		componentfile CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<componentfile> Instances() { return Udm::InstantiatedAttr<ccm_assembly::componentfile>(impl);}

		Udm::DerivedAttr<ccm_assembly::componentfile> Derived() { return Udm::DerivedAttr<ccm_assembly::componentfile>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::componentfile> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::componentfile>(impl);}

		static Uml::Attribute meta_type;
		Udm::StringAttr type() const { return Udm::StringAttr(impl, meta_type); }

		static Uml::Attribute meta_id;
		Udm::StringAttr id() const { return Udm::StringAttr(impl, meta_id); }

		static Uml::CompositionChildRole meta_fileinarchive_child;
		Udm::ChildAttr<ccm_assembly::fileinarchive> fileinarchive_child() const { return Udm::ChildAttr<ccm_assembly::fileinarchive>(impl, meta_fileinarchive_child); }

		Udm::ChildrenAttr<ccm_assembly::fileinarchive> fileinarchive_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::fileinarchive>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_componentfiles_parent;
		Udm::ParentAttr<ccm_assembly::componentfiles> componentfiles_parent() const { return Udm::ParentAttr<ccm_assembly::componentfiles>(impl, meta_componentfiles_parent); }

		Udm::ParentAttr<ccm_assembly::componentfiles> parent() const { return Udm::ParentAttr<ccm_assembly::componentfiles>(impl, Udm::NULLPARENTROLE); }
	};

	class publishesidentifier :  public Object {
	 public:
		static Uml::Class meta;

		publishesidentifier() { }
		publishesidentifier(Udm::ObjectImpl *impl) : Object(impl) { }
		publishesidentifier(const publishesidentifier &master) : Object(master) { }
		static publishesidentifier Cast(const Object &a) { return __Cast(a, meta); }

		static publishesidentifier Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		publishesidentifier CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		publishesidentifier CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<publishesidentifier> Instances() { return Udm::InstantiatedAttr<ccm_assembly::publishesidentifier>(impl);}

		Udm::DerivedAttr<ccm_assembly::publishesidentifier> Derived() { return Udm::DerivedAttr<ccm_assembly::publishesidentifier>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::publishesidentifier> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::publishesidentifier>(impl);}

		static Uml::Attribute meta_content;
		Udm::StringAttr content() const { return Udm::StringAttr(impl, meta_content); }

		static Uml::CompositionParentRole meta_publishesport_parent;
		Udm::ParentAttr<ccm_assembly::publishesport> publishesport_parent() const { return Udm::ParentAttr<ccm_assembly::publishesport>(impl, meta_publishesport_parent); }

		Udm::ParentAttr<ccm_assembly::eventoutputport> parent() const { return Udm::ParentAttr<ccm_assembly::eventoutputport>(impl, Udm::NULLPARENTROLE); }
	};

	class partitioning :  public Object {
	 public:
		static Uml::Class meta;

		partitioning() { }
		partitioning(Udm::ObjectImpl *impl) : Object(impl) { }
		partitioning(const partitioning &master) : Object(master) { }
		static partitioning Cast(const Object &a) { return __Cast(a, meta); }

		static partitioning Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		partitioning CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		partitioning CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<partitioning> Instances() { return Udm::InstantiatedAttr<ccm_assembly::partitioning>(impl);}

		Udm::DerivedAttr<ccm_assembly::partitioning> Derived() { return Udm::DerivedAttr<ccm_assembly::partitioning>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::partitioning> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::partitioning>(impl);}

		static Uml::CompositionChildRole meta_hostcollocation_children;
		Udm::ChildrenAttr<ccm_assembly::hostcollocation> hostcollocation_children() const { return Udm::ChildrenAttr<ccm_assembly::hostcollocation>(impl, meta_hostcollocation_children); }

		static Uml::CompositionChildRole meta_location_children;
		Udm::ChildrenAttr<ccm_assembly::location> location_children() const { return Udm::ChildrenAttr<ccm_assembly::location>(impl, meta_location_children); }

		Udm::ChildrenAttr<ccm_assembly::hostcollocation> hostcollocation_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::hostcollocation>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::processcollocation> processcollocation_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::processcollocation>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::location> location_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::location>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::homeplacement> homeplacement_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::homeplacement>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_componentassembly_parent;
		Udm::ParentAttr<ccm_assembly::componentassembly> componentassembly_parent() const { return Udm::ParentAttr<ccm_assembly::componentassembly>(impl, meta_componentassembly_parent); }

		Udm::ParentAttr<ccm_assembly::componentassembly> parent() const { return Udm::ParentAttr<ccm_assembly::componentassembly>(impl, Udm::NULLPARENTROLE); }
	};

	class componentinstantiation :  public Object {
	 public:
		static Uml::Class meta;

		componentinstantiation() { }
		componentinstantiation(Udm::ObjectImpl *impl) : Object(impl) { }
		componentinstantiation(const componentinstantiation &master) : Object(master) { }
		static componentinstantiation Cast(const Object &a) { return __Cast(a, meta); }

		static componentinstantiation Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		componentinstantiation CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		componentinstantiation CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<componentinstantiation> Instances() { return Udm::InstantiatedAttr<ccm_assembly::componentinstantiation>(impl);}

		Udm::DerivedAttr<ccm_assembly::componentinstantiation> Derived() { return Udm::DerivedAttr<ccm_assembly::componentinstantiation>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::componentinstantiation> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::componentinstantiation>(impl);}

		static Uml::Attribute meta_id;
		Udm::StringAttr id() const { return Udm::StringAttr(impl, meta_id); }

		static Uml::CompositionChildRole meta_registercomponent_children;
		Udm::ChildrenAttr<ccm_assembly::registercomponent> registercomponent_children() const { return Udm::ChildrenAttr<ccm_assembly::registercomponent>(impl, meta_registercomponent_children); }

		Udm::ChildrenAttr<ccm_assembly::registercomponent> registercomponent_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::registercomponent>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_homeplacement_parent;
		Udm::ParentAttr<ccm_assembly::homeplacement> homeplacement_parent() const { return Udm::ParentAttr<ccm_assembly::homeplacement>(impl, meta_homeplacement_parent); }

		Udm::ParentAttr<ccm_assembly::location> parent() const { return Udm::ParentAttr<ccm_assembly::location>(impl, Udm::NULLPARENTROLE); }
	};

	class connectinterface :  public Object {
	 public:
		static Uml::Class meta;

		connectinterface() { }
		connectinterface(Udm::ObjectImpl *impl) : Object(impl) { }
		connectinterface(const connectinterface &master) : Object(master) { }
		static connectinterface Cast(const Object &a) { return __Cast(a, meta); }

		static connectinterface Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		connectinterface CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		connectinterface CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<connectinterface> Instances() { return Udm::InstantiatedAttr<ccm_assembly::connectinterface>(impl);}

		Udm::DerivedAttr<ccm_assembly::connectinterface> Derived() { return Udm::DerivedAttr<ccm_assembly::connectinterface>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::connectinterface> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::connectinterface>(impl);}

		static Uml::Attribute meta_id;
		Udm::StringAttr id() const { return Udm::StringAttr(impl, meta_id); }

		static Uml::CompositionChildRole meta_providesport_child;
		Udm::ChildAttr<ccm_assembly::providesport> providesport_child() const { return Udm::ChildAttr<ccm_assembly::providesport>(impl, meta_providesport_child); }

		static Uml::CompositionChildRole meta_usesport_child;
		Udm::ChildAttr<ccm_assembly::usesport> usesport_child() const { return Udm::ChildAttr<ccm_assembly::usesport>(impl, meta_usesport_child); }

		Udm::ChildrenAttr<ccm_assembly::providesport> providesport_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::providesport>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::usesport> usesport_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::usesport>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_connections_parent;
		Udm::ParentAttr<ccm_assembly::connections> connections_parent() const { return Udm::ParentAttr<ccm_assembly::connections>(impl, meta_connections_parent); }

		Udm::ParentAttr<ccm_assembly::connections> parent() const { return Udm::ParentAttr<ccm_assembly::connections>(impl, Udm::NULLPARENTROLE); }
	};

	class emitsport :  public eventoutputport {
	 public:
		static Uml::Class meta;

		emitsport() { }
		emitsport(Udm::ObjectImpl *impl) : eventoutputport(impl) { }
		emitsport(const emitsport &master) : eventoutputport(master) { }
		static emitsport Cast(const Object &a) { return __Cast(a, meta); }

		static emitsport Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		emitsport CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		emitsport CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<emitsport> Instances() { return Udm::InstantiatedAttr<ccm_assembly::emitsport>(impl);}

		Udm::DerivedAttr<ccm_assembly::emitsport> Derived() { return Udm::DerivedAttr<ccm_assembly::emitsport>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::emitsport> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::emitsport>(impl);}

		static Uml::CompositionChildRole meta_emitsidentifier_child;
		Udm::ChildAttr<ccm_assembly::emitsidentifier> emitsidentifier_child() const { return Udm::ChildAttr<ccm_assembly::emitsidentifier>(impl, meta_emitsidentifier_child); }

		static Uml::CompositionChildRole meta_componentinstantiationref_child;
		Udm::ChildAttr<ccm_assembly::componentinstantiationref> componentinstantiationref_child() const { return Udm::ChildAttr<ccm_assembly::componentinstantiationref>(impl, meta_componentinstantiationref_child); }

		Udm::ChildrenAttr<ccm_assembly::componentinstantiationref> componentinstantiationref_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::componentinstantiationref>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::emitsidentifier> emitsidentifier_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::emitsidentifier>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class registercomponent :  public Object {
	 public:
		static Uml::Class meta;

		registercomponent() { }
		registercomponent(Udm::ObjectImpl *impl) : Object(impl) { }
		registercomponent(const registercomponent &master) : Object(master) { }
		static registercomponent Cast(const Object &a) { return __Cast(a, meta); }

		static registercomponent Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		registercomponent CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		registercomponent CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<registercomponent> Instances() { return Udm::InstantiatedAttr<ccm_assembly::registercomponent>(impl);}

		Udm::DerivedAttr<ccm_assembly::registercomponent> Derived() { return Udm::DerivedAttr<ccm_assembly::registercomponent>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::registercomponent> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::registercomponent>(impl);}

		static Uml::CompositionChildRole meta_writeiortofile_children;
		Udm::ChildrenAttr<ccm_assembly::writeiortofile> writeiortofile_children() const { return Udm::ChildrenAttr<ccm_assembly::writeiortofile>(impl, meta_writeiortofile_children); }

		Udm::ChildrenAttr<ccm_assembly::writeiortofile> writeiortofile_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::writeiortofile>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_componentinstantiation_parent;
		Udm::ParentAttr<ccm_assembly::componentinstantiation> componentinstantiation_parent() const { return Udm::ParentAttr<ccm_assembly::componentinstantiation>(impl, meta_componentinstantiation_parent); }

		Udm::ParentAttr<ccm_assembly::componentinstantiation> parent() const { return Udm::ParentAttr<ccm_assembly::componentinstantiation>(impl, Udm::NULLPARENTROLE); }
	};

	class publishesport :  public eventoutputport {
	 public:
		static Uml::Class meta;

		publishesport() { }
		publishesport(Udm::ObjectImpl *impl) : eventoutputport(impl) { }
		publishesport(const publishesport &master) : eventoutputport(master) { }
		static publishesport Cast(const Object &a) { return __Cast(a, meta); }

		static publishesport Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		publishesport CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		publishesport CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<publishesport> Instances() { return Udm::InstantiatedAttr<ccm_assembly::publishesport>(impl);}

		Udm::DerivedAttr<ccm_assembly::publishesport> Derived() { return Udm::DerivedAttr<ccm_assembly::publishesport>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::publishesport> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::publishesport>(impl);}

		static Uml::CompositionChildRole meta_componentinstantiationref_child;
		Udm::ChildAttr<ccm_assembly::componentinstantiationref> componentinstantiationref_child() const { return Udm::ChildAttr<ccm_assembly::componentinstantiationref>(impl, meta_componentinstantiationref_child); }

		static Uml::CompositionChildRole meta_publishesidentifier_child;
		Udm::ChildAttr<ccm_assembly::publishesidentifier> publishesidentifier_child() const { return Udm::ChildAttr<ccm_assembly::publishesidentifier>(impl, meta_publishesidentifier_child); }

		Udm::ChildrenAttr<ccm_assembly::componentinstantiationref> componentinstantiationref_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::componentinstantiationref>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::publishesidentifier> publishesidentifier_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::publishesidentifier>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class componentfiles :  public Object {
	 public:
		static Uml::Class meta;

		componentfiles() { }
		componentfiles(Udm::ObjectImpl *impl) : Object(impl) { }
		componentfiles(const componentfiles &master) : Object(master) { }
		static componentfiles Cast(const Object &a) { return __Cast(a, meta); }

		static componentfiles Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		componentfiles CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		componentfiles CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<componentfiles> Instances() { return Udm::InstantiatedAttr<ccm_assembly::componentfiles>(impl);}

		Udm::DerivedAttr<ccm_assembly::componentfiles> Derived() { return Udm::DerivedAttr<ccm_assembly::componentfiles>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::componentfiles> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::componentfiles>(impl);}

		static Uml::CompositionChildRole meta_componentfile_children;
		Udm::ChildrenAttr<ccm_assembly::componentfile> componentfile_children() const { return Udm::ChildrenAttr<ccm_assembly::componentfile>(impl, meta_componentfile_children); }

		Udm::ChildrenAttr<ccm_assembly::componentfile> componentfile_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::componentfile>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_componentassembly_parent;
		Udm::ParentAttr<ccm_assembly::componentassembly> componentassembly_parent() const { return Udm::ParentAttr<ccm_assembly::componentassembly>(impl, meta_componentassembly_parent); }

		Udm::ParentAttr<ccm_assembly::componentassembly> parent() const { return Udm::ParentAttr<ccm_assembly::componentassembly>(impl, Udm::NULLPARENTROLE); }
	};

	class emitsidentifier :  public Object {
	 public:
		static Uml::Class meta;

		emitsidentifier() { }
		emitsidentifier(Udm::ObjectImpl *impl) : Object(impl) { }
		emitsidentifier(const emitsidentifier &master) : Object(master) { }
		static emitsidentifier Cast(const Object &a) { return __Cast(a, meta); }

		static emitsidentifier Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		emitsidentifier CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		emitsidentifier CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<emitsidentifier> Instances() { return Udm::InstantiatedAttr<ccm_assembly::emitsidentifier>(impl);}

		Udm::DerivedAttr<ccm_assembly::emitsidentifier> Derived() { return Udm::DerivedAttr<ccm_assembly::emitsidentifier>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::emitsidentifier> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::emitsidentifier>(impl);}

		static Uml::Attribute meta_content;
		Udm::StringAttr content() const { return Udm::StringAttr(impl, meta_content); }

		static Uml::CompositionParentRole meta_emitsport_parent;
		Udm::ParentAttr<ccm_assembly::emitsport> emitsport_parent() const { return Udm::ParentAttr<ccm_assembly::emitsport>(impl, meta_emitsport_parent); }

		Udm::ParentAttr<ccm_assembly::eventoutputport> parent() const { return Udm::ParentAttr<ccm_assembly::eventoutputport>(impl, Udm::NULLPARENTROLE); }
	};

	class consumesidentifier :  public Object {
	 public:
		static Uml::Class meta;

		consumesidentifier() { }
		consumesidentifier(Udm::ObjectImpl *impl) : Object(impl) { }
		consumesidentifier(const consumesidentifier &master) : Object(master) { }
		static consumesidentifier Cast(const Object &a) { return __Cast(a, meta); }

		static consumesidentifier Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		consumesidentifier CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		consumesidentifier CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<consumesidentifier> Instances() { return Udm::InstantiatedAttr<ccm_assembly::consumesidentifier>(impl);}

		Udm::DerivedAttr<ccm_assembly::consumesidentifier> Derived() { return Udm::DerivedAttr<ccm_assembly::consumesidentifier>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::consumesidentifier> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::consumesidentifier>(impl);}

		static Uml::Attribute meta_content;
		Udm::StringAttr content() const { return Udm::StringAttr(impl, meta_content); }

		static Uml::CompositionParentRole meta_consumesport_parent;
		Udm::ParentAttr<ccm_assembly::consumesport> consumesport_parent() const { return Udm::ParentAttr<ccm_assembly::consumesport>(impl, meta_consumesport_parent); }

		Udm::ParentAttr<ccm_assembly::consumesport> parent() const { return Udm::ParentAttr<ccm_assembly::consumesport>(impl, Udm::NULLPARENTROLE); }
	};

	class usesport :  public Object {
	 public:
		static Uml::Class meta;

		usesport() { }
		usesport(Udm::ObjectImpl *impl) : Object(impl) { }
		usesport(const usesport &master) : Object(master) { }
		static usesport Cast(const Object &a) { return __Cast(a, meta); }

		static usesport Create(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		usesport CreateInstance(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		usesport CreateDerived(const Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<usesport> Instances() { return Udm::InstantiatedAttr<ccm_assembly::usesport>(impl);}

		Udm::DerivedAttr<ccm_assembly::usesport> Derived() { return Udm::DerivedAttr<ccm_assembly::usesport>(impl);}

		Udm::ArchetypeAttr<ccm_assembly::usesport> Archetype() { return Udm::ArchetypeAttr<ccm_assembly::usesport>(impl);}

		static Uml::CompositionChildRole meta_usesidentifier_child;
		Udm::ChildAttr<ccm_assembly::usesidentifier> usesidentifier_child() const { return Udm::ChildAttr<ccm_assembly::usesidentifier>(impl, meta_usesidentifier_child); }

		static Uml::CompositionChildRole meta_componentinstantiationref_child;
		Udm::ChildAttr<ccm_assembly::componentinstantiationref> componentinstantiationref_child() const { return Udm::ChildAttr<ccm_assembly::componentinstantiationref>(impl, meta_componentinstantiationref_child); }

		Udm::ChildrenAttr<ccm_assembly::usesidentifier> usesidentifier_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::usesidentifier>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<ccm_assembly::componentinstantiationref> componentinstantiationref_kind_children() const { return Udm::ChildrenAttr<ccm_assembly::componentinstantiationref>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_connectinterface_parent;
		Udm::ParentAttr<ccm_assembly::connectinterface> connectinterface_parent() const { return Udm::ParentAttr<ccm_assembly::connectinterface>(impl, meta_connectinterface_parent); }

		Udm::ParentAttr<ccm_assembly::connectinterface> parent() const { return Udm::ParentAttr<ccm_assembly::connectinterface>(impl, Udm::NULLPARENTROLE); }
	};

}

#endif //MOBIES_CCM_ASSEMBLY_H
