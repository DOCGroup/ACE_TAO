#ifndef MOBIES_PICML_H
#define MOBIES_PICML_H
// header file PICML.h generated from diagram PICML
// generated on Thu Aug 05 20:23:41 2004

#ifndef MOBIES_UDMBASE_H
#include "UdmBase.h"
#endif



namespace PICML {

	class  package;
	class  TopLevelPackageContainer;
	class  TopLevelPackages;
	class  TopLevelPackage;
	class  ComponentInfoProperty;
	class  ComponentContainer;
	class  ComponentProperty;
	class  CommonPortAttrs;
	class  ComponentType;
	class  ComponentTypes;
	class  ComponentConfigProperty;
	class  ComponentPropertyDescription;
	class  ConfigProperty;
	class  Implements;
	class  ImplementationCapability;
	class  ImplementationDependsOn;
	class  InfoProperty;
	class  ComponentImplementations;
	class  MonolithprimaryArtifact;
	class  ComponentImplementationReference;
	class  MonolithExecParameter;
	class  ComponentImplementationContainer;
	class  MonolithDeployRequirement;
	class  ComponentImplementation;
	class  MonolithicImplementation;
	class  DeploymentPlans;
	class  DeploymentPlan;
	class  CollocationGroup;
	class  InstanceMapping;
	class  PackageConfSpecializedConfig;
	class  PackageConfigurationContainer;
	class  PackageConfConfigProperty;
	class  PackageConfBasePackage;
	class  PackageConfigurationReference;
	class  PackageConfigurations;
	class  PackageConfSelectRequirement;
	class  PackageConfReference;
	class  PackageConfiguration;
	class  PackageInterface;
	class  PackageInfoProperty;
	class  PackageConfigProperty;
	class  ComponentPackages;
	class  PackageContainer;
	class  Implementation;
	class  ComponentPackage;
	class  ComponentPackageReference;
	class  Resource;
	class  Node;
	class  SharedResource;
	class  Interconnect;
	class  NodeReference;
	class  Bridge;
	class  SatisfierProperty;
	class  Requirement;
	class  DataType;
	class  ImplementationDependency;
	class  Property;
	class  Capability;
	class  RequirementSatisfier;
	class  ImplementationRequirement;
	class  RequirementBase;
	class  Elements;
	class  Domain;
	class  Interconnect2Node;
	class  Bridge2Interconnect;
	class  Shares;
	class  Node2Interconnect;
	class  Interconnect2Bridge;
	class  Targets;
	class  AssemblyDeployRequirement;
	class  invoke;
	class  ComponentAssembly;
	class  AssemblyselectRequirement;
	class  emit;
	class  PublishConnector;
	class  deliverTo;
	class  publish;
	class  AssemblyConfigProperty;
	class  ArtifactExecParameter;
	class  ImplementationArtifactReference;
	class  ArtifactDependsOn;
	class  ImplementationArtifact;
	class  ImplementationArtifacts;
	class  ArtifactDeployRequirement;
	class  ArtifactInfoProperty;
	class  ArtifactContainer;
	class  Package;
	class  File;
	class  Exception;
	class  ExceptionRef;
	class  FileRef;
	class  InterfaceDefinitions;
	class  ConstantType;
	class  Constant;
	class  PredefinedTypes;
	class  PredefinedType;
	class  Byte;
	class  Boolean;
	class  ShortInteger;
	class  LongInteger;
	class  RealNumber;
	class  String;
	class  GenericObject;
	class  GenericValueObject;
	class  GenericValue;
	class  TypeEncoding;
	class  TypeKind;
	class  Enum;
	class  EnumValue;
	class  Aggregate;
	class  SwitchedAggregate;
	class  Label;
	class  LabelConnection;
	class  MemberType;
	class  Discriminator;
	class  NamedType;
	class  Collection;
	class  Alias;
	class  Boxed;
	class  Member;
	class  NoInheritable;
	class  Object;
	class  Inherits;
	class  Event;
	class  ValueObject;
	class  Inheritable;
	class  HasOperations;
	class  SupportsInterfaces;
	class  ReadonlyAttribute;
	class  LookupKey;
	class  SetException;
	class  ObjectByValue;
	class  GetException;
	class  PrivateFlag;
	class  MakeMemberPrivate;
	class  AttributeMember;
	class  Supports;
	class  Attribute;
	class  OperationBase;
	class  FactoryOperation;
	class  LookupOperation;
	class  TwowayOperation;
	class  OnewayOperation;
	class  HasExceptions;
	class  InParameter;
	class  OutParameter;
	class  ReturnType;
	class  InoutParameter;
	class  Component;
	class  OutEventPort;
	class  ProvidedRequestPort;
	class  RequiredRequestPort;
	class  InEventPort;
	class  Port;
	class  ComponentRef;
	class  Manageable;
	class  ComponentFactory;
	class  ManagesComponent;
	class  Provideable;
	class  Prefixable;
	class  Taggable;
	class  RootFolder;
	class  MgaObject;

	  void Initialize();

	  void Initialize(const Uml::Diagram &dgr);
	extern  Udm::UdmDiagram diagram;

	class  TopLevelPackages :  public Udm::Object {
	 public:
		static Uml::Class meta;

		TopLevelPackages() { }
		TopLevelPackages(Udm::ObjectImpl *impl) : Object(impl) { }
		TopLevelPackages(const TopLevelPackages &master) : Object(master) { }
		static TopLevelPackages Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static TopLevelPackages Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		TopLevelPackages CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		TopLevelPackages CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<TopLevelPackages> Instances() { return Udm::InstantiatedAttr<PICML::TopLevelPackages>(impl);}
		template <class Pred> Udm::InstantiatedAttr<TopLevelPackages, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::TopLevelPackages, Pred>(impl);}

		Udm::DerivedAttr<PICML::TopLevelPackages> Derived() { return Udm::DerivedAttr<PICML::TopLevelPackages>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::TopLevelPackages, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::TopLevelPackages, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::TopLevelPackages> Archetype() { return Udm::ArchetypeAttr<PICML::TopLevelPackages>(impl);}

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::CompositionChildRole meta_TopLevelPackageContainer_children;
		Udm::ChildrenAttr<PICML::TopLevelPackageContainer> TopLevelPackageContainer_children() const { return Udm::ChildrenAttr<PICML::TopLevelPackageContainer>(impl, meta_TopLevelPackageContainer_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::TopLevelPackageContainer, Pred> TopLevelPackageContainer_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::TopLevelPackageContainer, Pred>(impl, meta_TopLevelPackageContainer_children); }

		Udm::ChildrenAttr<PICML::TopLevelPackageContainer> TopLevelPackageContainer_kind_children() const { return Udm::ChildrenAttr<PICML::TopLevelPackageContainer>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::TopLevelPackageContainer, Pred> TopLevelPackageContainer_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::TopLevelPackageContainer, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_RootFolder_parent;
		Udm::ParentAttr<PICML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, meta_RootFolder_parent); }

		Udm::ParentAttr<PICML::RootFolder> parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, Udm::NULLPARENTROLE); }
	};

	class  ComponentTypes :  public Udm::Object {
	 public:
		static Uml::Class meta;

		ComponentTypes() { }
		ComponentTypes(Udm::ObjectImpl *impl) : Object(impl) { }
		ComponentTypes(const ComponentTypes &master) : Object(master) { }
		static ComponentTypes Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentTypes Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentTypes CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentTypes CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentTypes> Instances() { return Udm::InstantiatedAttr<PICML::ComponentTypes>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentTypes, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentTypes, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentTypes> Derived() { return Udm::DerivedAttr<PICML::ComponentTypes>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentTypes, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentTypes, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentTypes> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentTypes>(impl);}

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::CompositionChildRole meta_ComponentContainer_children;
		Udm::ChildrenAttr<PICML::ComponentContainer> ComponentContainer_children() const { return Udm::ChildrenAttr<PICML::ComponentContainer>(impl, meta_ComponentContainer_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentContainer, Pred> ComponentContainer_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentContainer, Pred>(impl, meta_ComponentContainer_children); }

		Udm::ChildrenAttr<PICML::ComponentContainer> ComponentContainer_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentContainer>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentContainer, Pred> ComponentContainer_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentContainer, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_RootFolder_parent;
		Udm::ParentAttr<PICML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, meta_RootFolder_parent); }

		Udm::ParentAttr<PICML::RootFolder> parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, Udm::NULLPARENTROLE); }
	};

	class  ComponentImplementations :  public Udm::Object {
	 public:
		static Uml::Class meta;

		ComponentImplementations() { }
		ComponentImplementations(Udm::ObjectImpl *impl) : Object(impl) { }
		ComponentImplementations(const ComponentImplementations &master) : Object(master) { }
		static ComponentImplementations Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentImplementations Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentImplementations CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentImplementations CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentImplementations> Instances() { return Udm::InstantiatedAttr<PICML::ComponentImplementations>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentImplementations, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentImplementations, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentImplementations> Derived() { return Udm::DerivedAttr<PICML::ComponentImplementations>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentImplementations, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentImplementations, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentImplementations> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentImplementations>(impl);}

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::CompositionChildRole meta_ComponentImplementationContainer_children;
		Udm::ChildrenAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_children() const { return Udm::ChildrenAttr<PICML::ComponentImplementationContainer>(impl, meta_ComponentImplementationContainer_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentImplementationContainer, Pred> ComponentImplementationContainer_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentImplementationContainer, Pred>(impl, meta_ComponentImplementationContainer_children); }

		Udm::ChildrenAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentImplementationContainer>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentImplementationContainer, Pred> ComponentImplementationContainer_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentImplementationContainer, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_RootFolder_parent;
		Udm::ParentAttr<PICML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, meta_RootFolder_parent); }

		Udm::ParentAttr<PICML::RootFolder> parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, Udm::NULLPARENTROLE); }
	};

	class  DeploymentPlans :  public Udm::Object {
	 public:
		static Uml::Class meta;

		DeploymentPlans() { }
		DeploymentPlans(Udm::ObjectImpl *impl) : Object(impl) { }
		DeploymentPlans(const DeploymentPlans &master) : Object(master) { }
		static DeploymentPlans Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static DeploymentPlans Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		DeploymentPlans CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		DeploymentPlans CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<DeploymentPlans> Instances() { return Udm::InstantiatedAttr<PICML::DeploymentPlans>(impl);}
		template <class Pred> Udm::InstantiatedAttr<DeploymentPlans, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::DeploymentPlans, Pred>(impl);}

		Udm::DerivedAttr<PICML::DeploymentPlans> Derived() { return Udm::DerivedAttr<PICML::DeploymentPlans>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::DeploymentPlans, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::DeploymentPlans, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::DeploymentPlans> Archetype() { return Udm::ArchetypeAttr<PICML::DeploymentPlans>(impl);}

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::CompositionChildRole meta_DeploymentPlan_children;
		Udm::ChildrenAttr<PICML::DeploymentPlan> DeploymentPlan_children() const { return Udm::ChildrenAttr<PICML::DeploymentPlan>(impl, meta_DeploymentPlan_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::DeploymentPlan, Pred> DeploymentPlan_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::DeploymentPlan, Pred>(impl, meta_DeploymentPlan_children); }

		Udm::ChildrenAttr<PICML::DeploymentPlan> DeploymentPlan_kind_children() const { return Udm::ChildrenAttr<PICML::DeploymentPlan>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::DeploymentPlan, Pred> DeploymentPlan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::DeploymentPlan, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_RootFolder_parent;
		Udm::ParentAttr<PICML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, meta_RootFolder_parent); }

		Udm::ParentAttr<PICML::RootFolder> parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, Udm::NULLPARENTROLE); }
	};

	class  PackageConfigurations :  public Udm::Object {
	 public:
		static Uml::Class meta;

		PackageConfigurations() { }
		PackageConfigurations(Udm::ObjectImpl *impl) : Object(impl) { }
		PackageConfigurations(const PackageConfigurations &master) : Object(master) { }
		static PackageConfigurations Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PackageConfigurations Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PackageConfigurations CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PackageConfigurations CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PackageConfigurations> Instances() { return Udm::InstantiatedAttr<PICML::PackageConfigurations>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PackageConfigurations, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PackageConfigurations, Pred>(impl);}

		Udm::DerivedAttr<PICML::PackageConfigurations> Derived() { return Udm::DerivedAttr<PICML::PackageConfigurations>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PackageConfigurations, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PackageConfigurations, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PackageConfigurations> Archetype() { return Udm::ArchetypeAttr<PICML::PackageConfigurations>(impl);}

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::CompositionChildRole meta_PackageConfigurationContainer_children;
		Udm::ChildrenAttr<PICML::PackageConfigurationContainer> PackageConfigurationContainer_children() const { return Udm::ChildrenAttr<PICML::PackageConfigurationContainer>(impl, meta_PackageConfigurationContainer_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::PackageConfigurationContainer, Pred> PackageConfigurationContainer_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageConfigurationContainer, Pred>(impl, meta_PackageConfigurationContainer_children); }

		Udm::ChildrenAttr<PICML::PackageConfigurationContainer> PackageConfigurationContainer_kind_children() const { return Udm::ChildrenAttr<PICML::PackageConfigurationContainer>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PackageConfigurationContainer, Pred> PackageConfigurationContainer_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageConfigurationContainer, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_RootFolder_parent;
		Udm::ParentAttr<PICML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, meta_RootFolder_parent); }

		Udm::ParentAttr<PICML::RootFolder> parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, Udm::NULLPARENTROLE); }
	};

	class  ComponentPackages :  public Udm::Object {
	 public:
		static Uml::Class meta;

		ComponentPackages() { }
		ComponentPackages(Udm::ObjectImpl *impl) : Object(impl) { }
		ComponentPackages(const ComponentPackages &master) : Object(master) { }
		static ComponentPackages Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentPackages Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentPackages CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentPackages CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentPackages> Instances() { return Udm::InstantiatedAttr<PICML::ComponentPackages>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentPackages, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentPackages, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentPackages> Derived() { return Udm::DerivedAttr<PICML::ComponentPackages>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentPackages, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentPackages, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentPackages> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentPackages>(impl);}

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::CompositionChildRole meta_PackageContainer_children;
		Udm::ChildrenAttr<PICML::PackageContainer> PackageContainer_children() const { return Udm::ChildrenAttr<PICML::PackageContainer>(impl, meta_PackageContainer_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::PackageContainer, Pred> PackageContainer_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageContainer, Pred>(impl, meta_PackageContainer_children); }

		Udm::ChildrenAttr<PICML::PackageContainer> PackageContainer_kind_children() const { return Udm::ChildrenAttr<PICML::PackageContainer>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PackageContainer, Pred> PackageContainer_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageContainer, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_RootFolder_parent;
		Udm::ParentAttr<PICML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, meta_RootFolder_parent); }

		Udm::ParentAttr<PICML::RootFolder> parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, Udm::NULLPARENTROLE); }
	};

	class  Targets :  public Udm::Object {
	 public:
		static Uml::Class meta;

		Targets() { }
		Targets(Udm::ObjectImpl *impl) : Object(impl) { }
		Targets(const Targets &master) : Object(master) { }
		static Targets Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Targets Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Targets CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Targets CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Targets> Instances() { return Udm::InstantiatedAttr<PICML::Targets>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Targets, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Targets, Pred>(impl);}

		Udm::DerivedAttr<PICML::Targets> Derived() { return Udm::DerivedAttr<PICML::Targets>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Targets, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Targets, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Targets> Archetype() { return Udm::ArchetypeAttr<PICML::Targets>(impl);}

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::CompositionChildRole meta_Domain_children;
		Udm::ChildrenAttr<PICML::Domain> Domain_children() const { return Udm::ChildrenAttr<PICML::Domain>(impl, meta_Domain_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Domain, Pred> Domain_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Domain, Pred>(impl, meta_Domain_children); }

		Udm::ChildrenAttr<PICML::Domain> Domain_kind_children() const { return Udm::ChildrenAttr<PICML::Domain>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Domain, Pred> Domain_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Domain, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_RootFolder_parent;
		Udm::ParentAttr<PICML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, meta_RootFolder_parent); }

		Udm::ParentAttr<PICML::RootFolder> parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, Udm::NULLPARENTROLE); }
	};

	class  ImplementationArtifacts :  public Udm::Object {
	 public:
		static Uml::Class meta;

		ImplementationArtifacts() { }
		ImplementationArtifacts(Udm::ObjectImpl *impl) : Object(impl) { }
		ImplementationArtifacts(const ImplementationArtifacts &master) : Object(master) { }
		static ImplementationArtifacts Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ImplementationArtifacts Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ImplementationArtifacts CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ImplementationArtifacts CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ImplementationArtifacts> Instances() { return Udm::InstantiatedAttr<PICML::ImplementationArtifacts>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ImplementationArtifacts, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ImplementationArtifacts, Pred>(impl);}

		Udm::DerivedAttr<PICML::ImplementationArtifacts> Derived() { return Udm::DerivedAttr<PICML::ImplementationArtifacts>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ImplementationArtifacts, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ImplementationArtifacts, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ImplementationArtifacts> Archetype() { return Udm::ArchetypeAttr<PICML::ImplementationArtifacts>(impl);}

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::CompositionChildRole meta_ArtifactContainer_children;
		Udm::ChildrenAttr<PICML::ArtifactContainer> ArtifactContainer_children() const { return Udm::ChildrenAttr<PICML::ArtifactContainer>(impl, meta_ArtifactContainer_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ArtifactContainer, Pred> ArtifactContainer_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ArtifactContainer, Pred>(impl, meta_ArtifactContainer_children); }

		Udm::ChildrenAttr<PICML::ArtifactContainer> ArtifactContainer_kind_children() const { return Udm::ChildrenAttr<PICML::ArtifactContainer>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ArtifactContainer, Pred> ArtifactContainer_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ArtifactContainer, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_RootFolder_parent;
		Udm::ParentAttr<PICML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, meta_RootFolder_parent); }

		Udm::ParentAttr<PICML::RootFolder> parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, Udm::NULLPARENTROLE); }
	};

	class  InterfaceDefinitions :  public Udm::Object {
	 public:
		static Uml::Class meta;

		InterfaceDefinitions() { }
		InterfaceDefinitions(Udm::ObjectImpl *impl) : Object(impl) { }
		InterfaceDefinitions(const InterfaceDefinitions &master) : Object(master) { }
		static InterfaceDefinitions Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static InterfaceDefinitions Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		InterfaceDefinitions CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		InterfaceDefinitions CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<InterfaceDefinitions> Instances() { return Udm::InstantiatedAttr<PICML::InterfaceDefinitions>(impl);}
		template <class Pred> Udm::InstantiatedAttr<InterfaceDefinitions, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::InterfaceDefinitions, Pred>(impl);}

		Udm::DerivedAttr<PICML::InterfaceDefinitions> Derived() { return Udm::DerivedAttr<PICML::InterfaceDefinitions>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::InterfaceDefinitions, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::InterfaceDefinitions, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::InterfaceDefinitions> Archetype() { return Udm::ArchetypeAttr<PICML::InterfaceDefinitions>(impl);}

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::CompositionChildRole meta_File_children;
		Udm::ChildrenAttr<PICML::File> File_children() const { return Udm::ChildrenAttr<PICML::File>(impl, meta_File_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::File, Pred> File_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::File, Pred>(impl, meta_File_children); }

		Udm::ChildrenAttr<PICML::File> File_kind_children() const { return Udm::ChildrenAttr<PICML::File>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::File, Pred> File_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::File, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Prefixable> Prefixable_kind_children() const { return Udm::ChildrenAttr<PICML::Prefixable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Prefixable, Pred> Prefixable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Prefixable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_RootFolder_parent;
		Udm::ParentAttr<PICML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, meta_RootFolder_parent); }

		Udm::ParentAttr<PICML::RootFolder> parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, Udm::NULLPARENTROLE); }
	};

	class  PredefinedTypes :  public Udm::Object {
	 public:
		static Uml::Class meta;

		PredefinedTypes() { }
		PredefinedTypes(Udm::ObjectImpl *impl) : Object(impl) { }
		PredefinedTypes(const PredefinedTypes &master) : Object(master) { }
		static PredefinedTypes Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PredefinedTypes Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PredefinedTypes CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PredefinedTypes CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PredefinedTypes> Instances() { return Udm::InstantiatedAttr<PICML::PredefinedTypes>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PredefinedTypes, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PredefinedTypes, Pred>(impl);}

		Udm::DerivedAttr<PICML::PredefinedTypes> Derived() { return Udm::DerivedAttr<PICML::PredefinedTypes>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PredefinedTypes, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PredefinedTypes, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PredefinedTypes> Archetype() { return Udm::ArchetypeAttr<PICML::PredefinedTypes>(impl);}

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::CompositionChildRole meta_PredefinedType_children;
		Udm::ChildrenAttr<PICML::PredefinedType> PredefinedType_children() const { return Udm::ChildrenAttr<PICML::PredefinedType>(impl, meta_PredefinedType_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::PredefinedType, Pred> PredefinedType_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PredefinedType, Pred>(impl, meta_PredefinedType_children); }

		Udm::ChildrenAttr<PICML::ConstantType> ConstantType_kind_children() const { return Udm::ChildrenAttr<PICML::ConstantType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ConstantType, Pred> ConstantType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ConstantType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::PredefinedType> PredefinedType_kind_children() const { return Udm::ChildrenAttr<PICML::PredefinedType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PredefinedType, Pred> PredefinedType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PredefinedType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Byte> Byte_kind_children() const { return Udm::ChildrenAttr<PICML::Byte>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Byte, Pred> Byte_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Byte, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Boolean> Boolean_kind_children() const { return Udm::ChildrenAttr<PICML::Boolean>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Boolean, Pred> Boolean_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Boolean, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ShortInteger> ShortInteger_kind_children() const { return Udm::ChildrenAttr<PICML::ShortInteger>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ShortInteger, Pred> ShortInteger_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ShortInteger, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::LongInteger> LongInteger_kind_children() const { return Udm::ChildrenAttr<PICML::LongInteger>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::LongInteger, Pred> LongInteger_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::LongInteger, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::RealNumber> RealNumber_kind_children() const { return Udm::ChildrenAttr<PICML::RealNumber>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::RealNumber, Pred> RealNumber_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::RealNumber, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::String> String_kind_children() const { return Udm::ChildrenAttr<PICML::String>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::String, Pred> String_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::String, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::GenericObject> GenericObject_kind_children() const { return Udm::ChildrenAttr<PICML::GenericObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::GenericObject, Pred> GenericObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::GenericObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::GenericValueObject> GenericValueObject_kind_children() const { return Udm::ChildrenAttr<PICML::GenericValueObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::GenericValueObject, Pred> GenericValueObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::GenericValueObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::GenericValue> GenericValue_kind_children() const { return Udm::ChildrenAttr<PICML::GenericValue>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::GenericValue, Pred> GenericValue_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::GenericValue, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::TypeEncoding> TypeEncoding_kind_children() const { return Udm::ChildrenAttr<PICML::TypeEncoding>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::TypeEncoding, Pred> TypeEncoding_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::TypeEncoding, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::TypeKind> TypeKind_kind_children() const { return Udm::ChildrenAttr<PICML::TypeKind>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::TypeKind, Pred> TypeKind_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::TypeKind, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MemberType> MemberType_kind_children() const { return Udm::ChildrenAttr<PICML::MemberType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MemberType, Pred> MemberType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MemberType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_RootFolder_parent;
		Udm::ParentAttr<PICML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, meta_RootFolder_parent); }

		Udm::ParentAttr<PICML::RootFolder> parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, Udm::NULLPARENTROLE); }
	};

	class  RootFolder :  public Udm::Object {
	 public:
		static Uml::Class meta;

		RootFolder() { }
		RootFolder(Udm::ObjectImpl *impl) : Object(impl) { }
		RootFolder(const RootFolder &master) : Object(master) { }
		static RootFolder Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static RootFolder Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		RootFolder CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		RootFolder CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<RootFolder> Instances() { return Udm::InstantiatedAttr<PICML::RootFolder>(impl);}
		template <class Pred> Udm::InstantiatedAttr<RootFolder, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::RootFolder, Pred>(impl);}

		Udm::DerivedAttr<PICML::RootFolder> Derived() { return Udm::DerivedAttr<PICML::RootFolder>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::RootFolder, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::RootFolder, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::RootFolder> Archetype() { return Udm::ArchetypeAttr<PICML::RootFolder>(impl);}

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		static Uml::CompositionChildRole meta_ComponentTypes_children;
		Udm::ChildrenAttr<PICML::ComponentTypes> ComponentTypes_children() const { return Udm::ChildrenAttr<PICML::ComponentTypes>(impl, meta_ComponentTypes_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentTypes, Pred> ComponentTypes_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentTypes, Pred>(impl, meta_ComponentTypes_children); }

		static Uml::CompositionChildRole meta_TopLevelPackages_children;
		Udm::ChildrenAttr<PICML::TopLevelPackages> TopLevelPackages_children() const { return Udm::ChildrenAttr<PICML::TopLevelPackages>(impl, meta_TopLevelPackages_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::TopLevelPackages, Pred> TopLevelPackages_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::TopLevelPackages, Pred>(impl, meta_TopLevelPackages_children); }

		static Uml::CompositionChildRole meta_ComponentImplementations_children;
		Udm::ChildrenAttr<PICML::ComponentImplementations> ComponentImplementations_children() const { return Udm::ChildrenAttr<PICML::ComponentImplementations>(impl, meta_ComponentImplementations_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentImplementations, Pred> ComponentImplementations_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentImplementations, Pred>(impl, meta_ComponentImplementations_children); }

		static Uml::CompositionChildRole meta_DeploymentPlans_children;
		Udm::ChildrenAttr<PICML::DeploymentPlans> DeploymentPlans_children() const { return Udm::ChildrenAttr<PICML::DeploymentPlans>(impl, meta_DeploymentPlans_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::DeploymentPlans, Pred> DeploymentPlans_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::DeploymentPlans, Pred>(impl, meta_DeploymentPlans_children); }

		static Uml::CompositionChildRole meta_PackageConfigurations_children;
		Udm::ChildrenAttr<PICML::PackageConfigurations> PackageConfigurations_children() const { return Udm::ChildrenAttr<PICML::PackageConfigurations>(impl, meta_PackageConfigurations_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::PackageConfigurations, Pred> PackageConfigurations_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageConfigurations, Pred>(impl, meta_PackageConfigurations_children); }

		static Uml::CompositionChildRole meta_ComponentPackages_children;
		Udm::ChildrenAttr<PICML::ComponentPackages> ComponentPackages_children() const { return Udm::ChildrenAttr<PICML::ComponentPackages>(impl, meta_ComponentPackages_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentPackages, Pred> ComponentPackages_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentPackages, Pred>(impl, meta_ComponentPackages_children); }

		static Uml::CompositionChildRole meta_Targets_children;
		Udm::ChildrenAttr<PICML::Targets> Targets_children() const { return Udm::ChildrenAttr<PICML::Targets>(impl, meta_Targets_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Targets, Pred> Targets_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Targets, Pred>(impl, meta_Targets_children); }

		static Uml::CompositionChildRole meta_ImplementationArtifacts_children;
		Udm::ChildrenAttr<PICML::ImplementationArtifacts> ImplementationArtifacts_children() const { return Udm::ChildrenAttr<PICML::ImplementationArtifacts>(impl, meta_ImplementationArtifacts_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ImplementationArtifacts, Pred> ImplementationArtifacts_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ImplementationArtifacts, Pred>(impl, meta_ImplementationArtifacts_children); }

		static Uml::CompositionChildRole meta_InterfaceDefinitions_children;
		Udm::ChildrenAttr<PICML::InterfaceDefinitions> InterfaceDefinitions_children() const { return Udm::ChildrenAttr<PICML::InterfaceDefinitions>(impl, meta_InterfaceDefinitions_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::InterfaceDefinitions, Pred> InterfaceDefinitions_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::InterfaceDefinitions, Pred>(impl, meta_InterfaceDefinitions_children); }

		static Uml::CompositionChildRole meta_PredefinedTypes_children;
		Udm::ChildrenAttr<PICML::PredefinedTypes> PredefinedTypes_children() const { return Udm::ChildrenAttr<PICML::PredefinedTypes>(impl, meta_PredefinedTypes_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::PredefinedTypes, Pred> PredefinedTypes_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PredefinedTypes, Pred>(impl, meta_PredefinedTypes_children); }

		static Uml::CompositionChildRole meta_RootFolder_children;
		Udm::ChildrenAttr<PICML::RootFolder> RootFolder_children() const { return Udm::ChildrenAttr<PICML::RootFolder>(impl, meta_RootFolder_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::RootFolder, Pred> RootFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::RootFolder, Pred>(impl, meta_RootFolder_children); }

		Udm::ChildrenAttr<PICML::TopLevelPackages> TopLevelPackages_kind_children() const { return Udm::ChildrenAttr<PICML::TopLevelPackages>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::TopLevelPackages, Pred> TopLevelPackages_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::TopLevelPackages, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentTypes> ComponentTypes_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentTypes>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentTypes, Pred> ComponentTypes_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentTypes, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentImplementations> ComponentImplementations_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentImplementations>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentImplementations, Pred> ComponentImplementations_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentImplementations, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::DeploymentPlans> DeploymentPlans_kind_children() const { return Udm::ChildrenAttr<PICML::DeploymentPlans>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::DeploymentPlans, Pred> DeploymentPlans_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::DeploymentPlans, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::PackageConfigurations> PackageConfigurations_kind_children() const { return Udm::ChildrenAttr<PICML::PackageConfigurations>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PackageConfigurations, Pred> PackageConfigurations_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageConfigurations, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentPackages> ComponentPackages_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentPackages>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentPackages, Pred> ComponentPackages_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentPackages, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Targets> Targets_kind_children() const { return Udm::ChildrenAttr<PICML::Targets>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Targets, Pred> Targets_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Targets, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ImplementationArtifacts> ImplementationArtifacts_kind_children() const { return Udm::ChildrenAttr<PICML::ImplementationArtifacts>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ImplementationArtifacts, Pred> ImplementationArtifacts_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ImplementationArtifacts, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::InterfaceDefinitions> InterfaceDefinitions_kind_children() const { return Udm::ChildrenAttr<PICML::InterfaceDefinitions>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::InterfaceDefinitions, Pred> InterfaceDefinitions_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::InterfaceDefinitions, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::PredefinedTypes> PredefinedTypes_kind_children() const { return Udm::ChildrenAttr<PICML::PredefinedTypes>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PredefinedTypes, Pred> PredefinedTypes_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PredefinedTypes, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::RootFolder> RootFolder_kind_children() const { return Udm::ChildrenAttr<PICML::RootFolder>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::RootFolder, Pred> RootFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::RootFolder, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_RootFolder_parent;
		Udm::ParentAttr<PICML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, meta_RootFolder_parent); }

		Udm::ParentAttr<PICML::RootFolder> parent() const { return Udm::ParentAttr<PICML::RootFolder>(impl, Udm::NULLPARENTROLE); }
	};

	class  MgaObject :  public Udm::Object {
	 public:
		static Uml::Class meta;

		MgaObject() { }
		MgaObject(Udm::ObjectImpl *impl) : Object(impl) { }
		MgaObject(const MgaObject &master) : Object(master) { }
		static MgaObject Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static MgaObject Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		MgaObject CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		MgaObject CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<MgaObject> Instances() { return Udm::InstantiatedAttr<PICML::MgaObject>(impl);}
		template <class Pred> Udm::InstantiatedAttr<MgaObject, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::MgaObject, Pred>(impl);}

		Udm::DerivedAttr<PICML::MgaObject> Derived() { return Udm::DerivedAttr<PICML::MgaObject>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::MgaObject, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::MgaObject, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::MgaObject> Archetype() { return Udm::ArchetypeAttr<PICML::MgaObject>(impl);}

		static Uml::Attribute meta_position;
		Udm::StringAttr position() const { return Udm::StringAttr(impl, meta_position); }

		static Uml::Attribute meta_name;
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class  package :  public MgaObject {
	 public:
		static Uml::Class meta;

		package() { }
		package(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		package(const package &master) : MgaObject(master) { }
		static package Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static package Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		package CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		package CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<package> Instances() { return Udm::InstantiatedAttr<PICML::package>(impl);}
		template <class Pred> Udm::InstantiatedAttr<package, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::package, Pred>(impl);}

		Udm::DerivedAttr<PICML::package> Derived() { return Udm::DerivedAttr<PICML::package>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::package, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::package, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::package> Archetype() { return Udm::ArchetypeAttr<PICML::package>(impl);}

		static Uml::CompositionParentRole meta_TopLevelPackageContainer_parent;
		Udm::ParentAttr<PICML::TopLevelPackageContainer> TopLevelPackageContainer_parent() const { return Udm::ParentAttr<PICML::TopLevelPackageContainer>(impl, meta_TopLevelPackageContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstpackage_end_;
		Udm::AssocEndAttr<PICML::PackageConfigurationReference> dstpackage_end() const { return Udm::AssocEndAttr<PICML::PackageConfigurationReference>(impl, meta_dstpackage_end_); }

		static Uml::AssociationRole meta_srcpackage_end_;
		Udm::AssocEndAttr<PICML::TopLevelPackage> srcpackage_end() const { return Udm::AssocEndAttr<PICML::TopLevelPackage>(impl, meta_srcpackage_end_); }

	};

	class  TopLevelPackageContainer :  public MgaObject {
	 public:
		static Uml::Class meta;

		TopLevelPackageContainer() { }
		TopLevelPackageContainer(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		TopLevelPackageContainer(const TopLevelPackageContainer &master) : MgaObject(master) { }
		static TopLevelPackageContainer Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static TopLevelPackageContainer Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		TopLevelPackageContainer CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		TopLevelPackageContainer CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<TopLevelPackageContainer> Instances() { return Udm::InstantiatedAttr<PICML::TopLevelPackageContainer>(impl);}
		template <class Pred> Udm::InstantiatedAttr<TopLevelPackageContainer, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::TopLevelPackageContainer, Pred>(impl);}

		Udm::DerivedAttr<PICML::TopLevelPackageContainer> Derived() { return Udm::DerivedAttr<PICML::TopLevelPackageContainer>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::TopLevelPackageContainer, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::TopLevelPackageContainer, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::TopLevelPackageContainer> Archetype() { return Udm::ArchetypeAttr<PICML::TopLevelPackageContainer>(impl);}

		static Uml::CompositionChildRole meta_TopLevelPackage_child;
		Udm::ChildAttr<PICML::TopLevelPackage> TopLevelPackage_child() const { return Udm::ChildAttr<PICML::TopLevelPackage>(impl, meta_TopLevelPackage_child); }

		static Uml::CompositionChildRole meta_PackageConfigurationReference_child;
		Udm::ChildAttr<PICML::PackageConfigurationReference> PackageConfigurationReference_child() const { return Udm::ChildAttr<PICML::PackageConfigurationReference>(impl, meta_PackageConfigurationReference_child); }

		static Uml::CompositionChildRole meta_package_child;
		Udm::ChildAttr<PICML::package> package_child() const { return Udm::ChildAttr<PICML::package>(impl, meta_package_child); }

		Udm::ChildrenAttr<PICML::package> package_kind_children() const { return Udm::ChildrenAttr<PICML::package>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::package, Pred> package_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::package, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::TopLevelPackage> TopLevelPackage_kind_children() const { return Udm::ChildrenAttr<PICML::TopLevelPackage>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::TopLevelPackage, Pred> TopLevelPackage_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::TopLevelPackage, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::PackageConfigurationReference> PackageConfigurationReference_kind_children() const { return Udm::ChildrenAttr<PICML::PackageConfigurationReference>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PackageConfigurationReference, Pred> PackageConfigurationReference_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageConfigurationReference, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_TopLevelPackages_parent;
		Udm::ParentAttr<PICML::TopLevelPackages> TopLevelPackages_parent() const { return Udm::ParentAttr<PICML::TopLevelPackages>(impl, meta_TopLevelPackages_parent); }

		Udm::ParentAttr<PICML::TopLevelPackages> parent() const { return Udm::ParentAttr<PICML::TopLevelPackages>(impl, Udm::NULLPARENTROLE); }
	};

	class  TopLevelPackage :  public MgaObject {
	 public:
		static Uml::Class meta;

		TopLevelPackage() { }
		TopLevelPackage(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		TopLevelPackage(const TopLevelPackage &master) : MgaObject(master) { }
		static TopLevelPackage Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static TopLevelPackage Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		TopLevelPackage CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		TopLevelPackage CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<TopLevelPackage> Instances() { return Udm::InstantiatedAttr<PICML::TopLevelPackage>(impl);}
		template <class Pred> Udm::InstantiatedAttr<TopLevelPackage, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::TopLevelPackage, Pred>(impl);}

		Udm::DerivedAttr<PICML::TopLevelPackage> Derived() { return Udm::DerivedAttr<PICML::TopLevelPackage>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::TopLevelPackage, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::TopLevelPackage, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::TopLevelPackage> Archetype() { return Udm::ArchetypeAttr<PICML::TopLevelPackage>(impl);}

		static Uml::AssociationRole meta_dstpackage, meta_dstpackage_rev;
		Udm::AClassPointerAttr<PICML::package, PICML::PackageConfigurationReference> dstpackage() const { return Udm::AClassPointerAttr<PICML::package, PICML::PackageConfigurationReference>(impl, meta_dstpackage, meta_dstpackage_rev); }

		static Uml::CompositionParentRole meta_TopLevelPackageContainer_parent;
		Udm::ParentAttr<PICML::TopLevelPackageContainer> TopLevelPackageContainer_parent() const { return Udm::ParentAttr<PICML::TopLevelPackageContainer>(impl, meta_TopLevelPackageContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  ComponentInfoProperty :  public MgaObject {
	 public:
		static Uml::Class meta;

		ComponentInfoProperty() { }
		ComponentInfoProperty(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ComponentInfoProperty(const ComponentInfoProperty &master) : MgaObject(master) { }
		static ComponentInfoProperty Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentInfoProperty Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentInfoProperty CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentInfoProperty CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentInfoProperty> Instances() { return Udm::InstantiatedAttr<PICML::ComponentInfoProperty>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentInfoProperty, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentInfoProperty, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentInfoProperty> Derived() { return Udm::DerivedAttr<PICML::ComponentInfoProperty>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentInfoProperty, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentInfoProperty, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentInfoProperty> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentInfoProperty>(impl);}

		static Uml::CompositionParentRole meta_ComponentContainer_parent;
		Udm::ParentAttr<PICML::ComponentContainer> ComponentContainer_parent() const { return Udm::ParentAttr<PICML::ComponentContainer>(impl, meta_ComponentContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcComponentInfoProperty_end_;
		Udm::AssocEndAttr<PICML::ComponentType> srcComponentInfoProperty_end() const { return Udm::AssocEndAttr<PICML::ComponentType>(impl, meta_srcComponentInfoProperty_end_); }

		static Uml::AssociationRole meta_dstComponentInfoProperty_end_;
		Udm::AssocEndAttr<PICML::Property> dstComponentInfoProperty_end() const { return Udm::AssocEndAttr<PICML::Property>(impl, meta_dstComponentInfoProperty_end_); }

	};

	class  ComponentContainer :  public MgaObject {
	 public:
		static Uml::Class meta;

		ComponentContainer() { }
		ComponentContainer(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ComponentContainer(const ComponentContainer &master) : MgaObject(master) { }
		static ComponentContainer Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentContainer Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentContainer CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentContainer CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentContainer> Instances() { return Udm::InstantiatedAttr<PICML::ComponentContainer>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentContainer, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentContainer, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentContainer> Derived() { return Udm::DerivedAttr<PICML::ComponentContainer>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentContainer, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentContainer, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentContainer> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentContainer>(impl);}

		static Uml::CompositionChildRole meta_ComponentConfigProperty_children;
		Udm::ChildrenAttr<PICML::ComponentConfigProperty> ComponentConfigProperty_children() const { return Udm::ChildrenAttr<PICML::ComponentConfigProperty>(impl, meta_ComponentConfigProperty_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentConfigProperty, Pred> ComponentConfigProperty_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentConfigProperty, Pred>(impl, meta_ComponentConfigProperty_children); }

		static Uml::CompositionChildRole meta_ComponentProperty_children;
		Udm::ChildrenAttr<PICML::ComponentProperty> ComponentProperty_children() const { return Udm::ChildrenAttr<PICML::ComponentProperty>(impl, meta_ComponentProperty_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentProperty, Pred> ComponentProperty_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentProperty, Pred>(impl, meta_ComponentProperty_children); }

		static Uml::CompositionChildRole meta_ComponentInfoProperty_children;
		Udm::ChildrenAttr<PICML::ComponentInfoProperty> ComponentInfoProperty_children() const { return Udm::ChildrenAttr<PICML::ComponentInfoProperty>(impl, meta_ComponentInfoProperty_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentInfoProperty, Pred> ComponentInfoProperty_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentInfoProperty, Pred>(impl, meta_ComponentInfoProperty_children); }

		static Uml::CompositionChildRole meta_Property_children;
		Udm::ChildrenAttr<PICML::Property> Property_children() const { return Udm::ChildrenAttr<PICML::Property>(impl, meta_Property_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Property, Pred> Property_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Property, Pred>(impl, meta_Property_children); }

		static Uml::CompositionChildRole meta_ComponentType_child;
		Udm::ChildAttr<PICML::ComponentType> ComponentType_child() const { return Udm::ChildAttr<PICML::ComponentType>(impl, meta_ComponentType_child); }

		static Uml::CompositionChildRole meta_ComponentPropertyDescription_children;
		Udm::ChildrenAttr<PICML::ComponentPropertyDescription> ComponentPropertyDescription_children() const { return Udm::ChildrenAttr<PICML::ComponentPropertyDescription>(impl, meta_ComponentPropertyDescription_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentPropertyDescription, Pred> ComponentPropertyDescription_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentPropertyDescription, Pred>(impl, meta_ComponentPropertyDescription_children); }

		Udm::ChildrenAttr<PICML::ComponentInfoProperty> ComponentInfoProperty_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentInfoProperty>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentInfoProperty, Pred> ComponentInfoProperty_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentInfoProperty, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentProperty> ComponentProperty_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentProperty>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentProperty, Pred> ComponentProperty_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentProperty, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentType> ComponentType_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentType, Pred> ComponentType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentConfigProperty> ComponentConfigProperty_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentConfigProperty>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentConfigProperty, Pred> ComponentConfigProperty_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentConfigProperty, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentPropertyDescription> ComponentPropertyDescription_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentPropertyDescription>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentPropertyDescription, Pred> ComponentPropertyDescription_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentPropertyDescription, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Property> Property_kind_children() const { return Udm::ChildrenAttr<PICML::Property>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Property, Pred> Property_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Property, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_ComponentTypes_parent;
		Udm::ParentAttr<PICML::ComponentTypes> ComponentTypes_parent() const { return Udm::ParentAttr<PICML::ComponentTypes>(impl, meta_ComponentTypes_parent); }

		Udm::ParentAttr<PICML::ComponentTypes> parent() const { return Udm::ParentAttr<PICML::ComponentTypes>(impl, Udm::NULLPARENTROLE); }
	};

	class  ComponentProperty :  public MgaObject {
	 public:
		static Uml::Class meta;

		ComponentProperty() { }
		ComponentProperty(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ComponentProperty(const ComponentProperty &master) : MgaObject(master) { }
		static ComponentProperty Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentProperty Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentProperty CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentProperty CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentProperty> Instances() { return Udm::InstantiatedAttr<PICML::ComponentProperty>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentProperty, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentProperty, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentProperty> Derived() { return Udm::DerivedAttr<PICML::ComponentProperty>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentProperty, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentProperty, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentProperty> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentProperty>(impl);}

		static Uml::CompositionParentRole meta_ComponentContainer_parent;
		Udm::ParentAttr<PICML::ComponentContainer> ComponentContainer_parent() const { return Udm::ParentAttr<PICML::ComponentContainer>(impl, meta_ComponentContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcComponentProperty_end_;
		Udm::AssocEndAttr<PICML::ComponentType> srcComponentProperty_end() const { return Udm::AssocEndAttr<PICML::ComponentType>(impl, meta_srcComponentProperty_end_); }

		static Uml::AssociationRole meta_dstComponentProperty_end_;
		Udm::AssocEndAttr<PICML::ComponentPropertyDescription> dstComponentProperty_end() const { return Udm::AssocEndAttr<PICML::ComponentPropertyDescription>(impl, meta_dstComponentProperty_end_); }

	};

	class  CommonPortAttrs :  virtual public MgaObject {
	 public:
		static Uml::Class meta;

		CommonPortAttrs() { }
		CommonPortAttrs(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		CommonPortAttrs(const CommonPortAttrs &master) : MgaObject(master) { }
		static CommonPortAttrs Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static CommonPortAttrs Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		CommonPortAttrs CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		CommonPortAttrs CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<CommonPortAttrs> Instances() { return Udm::InstantiatedAttr<PICML::CommonPortAttrs>(impl);}
		template <class Pred> Udm::InstantiatedAttr<CommonPortAttrs, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::CommonPortAttrs, Pred>(impl);}

		Udm::DerivedAttr<PICML::CommonPortAttrs> Derived() { return Udm::DerivedAttr<PICML::CommonPortAttrs>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::CommonPortAttrs, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::CommonPortAttrs, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::CommonPortAttrs> Archetype() { return Udm::ArchetypeAttr<PICML::CommonPortAttrs>(impl);}

		static Uml::Attribute meta_exclusiveUser;
		Udm::BooleanAttr exclusiveUser() const { return Udm::BooleanAttr(impl, meta_exclusiveUser); }

		static Uml::Attribute meta_optional;
		Udm::BooleanAttr optional() const { return Udm::BooleanAttr(impl, meta_optional); }

		static Uml::Attribute meta_exclusiveProvider;
		Udm::BooleanAttr exclusiveProvider() const { return Udm::BooleanAttr(impl, meta_exclusiveProvider); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class  ComponentType :  public MgaObject {
	 public:
		static Uml::Class meta;

		ComponentType() { }
		ComponentType(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ComponentType(const ComponentType &master) : MgaObject(master) { }
		static ComponentType Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentType Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentType CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentType CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentType> Instances() { return Udm::InstantiatedAttr<PICML::ComponentType>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentType, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentType, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentType> Derived() { return Udm::DerivedAttr<PICML::ComponentType>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentType, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentType, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentType> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentType>(impl);}

		static Uml::AssociationRole meta_dstComponentProperty, meta_dstComponentProperty_rev;
		Udm::AClassAssocAttr<PICML::ComponentProperty, PICML::ComponentPropertyDescription> dstComponentProperty() const { return Udm::AClassAssocAttr<PICML::ComponentProperty, PICML::ComponentPropertyDescription>(impl, meta_dstComponentProperty, meta_dstComponentProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ComponentProperty, PICML::ComponentPropertyDescription, Pred> dstComponentProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ComponentProperty, PICML::ComponentPropertyDescription, Pred>(impl, meta_dstComponentProperty, meta_dstComponentProperty_rev); }

		static Uml::AssociationRole meta_dstComponentConfigProperty, meta_dstComponentConfigProperty_rev;
		Udm::AClassAssocAttr<PICML::ComponentConfigProperty, PICML::Property> dstComponentConfigProperty() const { return Udm::AClassAssocAttr<PICML::ComponentConfigProperty, PICML::Property>(impl, meta_dstComponentConfigProperty, meta_dstComponentConfigProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ComponentConfigProperty, PICML::Property, Pred> dstComponentConfigProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ComponentConfigProperty, PICML::Property, Pred>(impl, meta_dstComponentConfigProperty, meta_dstComponentConfigProperty_rev); }

		static Uml::AssociationRole meta_dstComponentInfoProperty, meta_dstComponentInfoProperty_rev;
		Udm::AClassAssocAttr<PICML::ComponentInfoProperty, PICML::Property> dstComponentInfoProperty() const { return Udm::AClassAssocAttr<PICML::ComponentInfoProperty, PICML::Property>(impl, meta_dstComponentInfoProperty, meta_dstComponentInfoProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ComponentInfoProperty, PICML::Property, Pred> dstComponentInfoProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ComponentInfoProperty, PICML::Property, Pred>(impl, meta_dstComponentInfoProperty, meta_dstComponentInfoProperty_rev); }

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::Component> ref() const { return Udm::PointerAttr<PICML::Component>(impl, meta_ref); }

		static Uml::AssociationRole meta_CollocationGroup;
		Udm::AssocAttr<PICML::CollocationGroup> CollocationGroup() const { return Udm::AssocAttr<PICML::CollocationGroup>(impl, meta_CollocationGroup); }
		template <class Pred> Udm::AssocAttr<PICML::CollocationGroup, Pred > CollocationGroup_sorted(const Pred &) const { return Udm::AssocAttr<PICML::CollocationGroup, Pred>(impl, meta_CollocationGroup); }

		static Uml::AssociationRole meta_srcImplements, meta_srcImplements_rev;
		Udm::AClassAssocAttr<PICML::Implements, PICML::ComponentImplementation> srcImplements() const { return Udm::AClassAssocAttr<PICML::Implements, PICML::ComponentImplementation>(impl, meta_srcImplements, meta_srcImplements_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::Implements, PICML::ComponentImplementation, Pred> srcImplements_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::Implements, PICML::ComponentImplementation, Pred>(impl, meta_srcImplements, meta_srcImplements_rev); }

		static Uml::AssociationRole meta_srcPackageInterface, meta_srcPackageInterface_rev;
		Udm::AClassPointerAttr<PICML::PackageInterface, PICML::ComponentPackage> srcPackageInterface() const { return Udm::AClassPointerAttr<PICML::PackageInterface, PICML::ComponentPackage>(impl, meta_srcPackageInterface, meta_srcPackageInterface_rev); }

		static Uml::CompositionParentRole meta_ComponentContainer_parent;
		Udm::ParentAttr<PICML::ComponentContainer> ComponentContainer_parent() const { return Udm::ParentAttr<PICML::ComponentContainer>(impl, meta_ComponentContainer_parent); }

		static Uml::CompositionParentRole meta_ComponentImplementationContainer_parent;
		Udm::ParentAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_parent() const { return Udm::ParentAttr<PICML::ComponentImplementationContainer>(impl, meta_ComponentImplementationContainer_parent); }

		static Uml::CompositionParentRole meta_DeploymentPlan_parent;
		Udm::ParentAttr<PICML::DeploymentPlan> DeploymentPlan_parent() const { return Udm::ParentAttr<PICML::DeploymentPlan>(impl, meta_DeploymentPlan_parent); }

		static Uml::CompositionParentRole meta_PackageContainer_parent;
		Udm::ParentAttr<PICML::PackageContainer> PackageContainer_parent() const { return Udm::ParentAttr<PICML::PackageContainer>(impl, meta_PackageContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  ComponentConfigProperty :  public MgaObject {
	 public:
		static Uml::Class meta;

		ComponentConfigProperty() { }
		ComponentConfigProperty(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ComponentConfigProperty(const ComponentConfigProperty &master) : MgaObject(master) { }
		static ComponentConfigProperty Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentConfigProperty Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentConfigProperty CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentConfigProperty CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentConfigProperty> Instances() { return Udm::InstantiatedAttr<PICML::ComponentConfigProperty>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentConfigProperty, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentConfigProperty, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentConfigProperty> Derived() { return Udm::DerivedAttr<PICML::ComponentConfigProperty>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentConfigProperty, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentConfigProperty, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentConfigProperty> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentConfigProperty>(impl);}

		static Uml::CompositionParentRole meta_ComponentContainer_parent;
		Udm::ParentAttr<PICML::ComponentContainer> ComponentContainer_parent() const { return Udm::ParentAttr<PICML::ComponentContainer>(impl, meta_ComponentContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcComponentConfigProperty_end_;
		Udm::AssocEndAttr<PICML::ComponentType> srcComponentConfigProperty_end() const { return Udm::AssocEndAttr<PICML::ComponentType>(impl, meta_srcComponentConfigProperty_end_); }

		static Uml::AssociationRole meta_dstComponentConfigProperty_end_;
		Udm::AssocEndAttr<PICML::Property> dstComponentConfigProperty_end() const { return Udm::AssocEndAttr<PICML::Property>(impl, meta_dstComponentConfigProperty_end_); }

	};

	class  ComponentPropertyDescription :  public MgaObject {
	 public:
		static Uml::Class meta;

		ComponentPropertyDescription() { }
		ComponentPropertyDescription(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ComponentPropertyDescription(const ComponentPropertyDescription &master) : MgaObject(master) { }
		static ComponentPropertyDescription Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentPropertyDescription Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentPropertyDescription CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentPropertyDescription CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentPropertyDescription> Instances() { return Udm::InstantiatedAttr<PICML::ComponentPropertyDescription>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentPropertyDescription, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentPropertyDescription, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentPropertyDescription> Derived() { return Udm::DerivedAttr<PICML::ComponentPropertyDescription>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentPropertyDescription, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentPropertyDescription, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentPropertyDescription> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentPropertyDescription>(impl);}

		static Uml::AssociationRole meta_srcComponentProperty, meta_srcComponentProperty_rev;
		Udm::AClassAssocAttr<PICML::ComponentProperty, PICML::ComponentType> srcComponentProperty() const { return Udm::AClassAssocAttr<PICML::ComponentProperty, PICML::ComponentType>(impl, meta_srcComponentProperty, meta_srcComponentProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ComponentProperty, PICML::ComponentType, Pred> srcComponentProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ComponentProperty, PICML::ComponentType, Pred>(impl, meta_srcComponentProperty, meta_srcComponentProperty_rev); }

		static Uml::CompositionChildRole meta_DataType_child;
		Udm::ChildAttr<PICML::DataType> DataType_child() const { return Udm::ChildAttr<PICML::DataType>(impl, meta_DataType_child); }

		Udm::ChildrenAttr<PICML::DataType> DataType_kind_children() const { return Udm::ChildrenAttr<PICML::DataType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::DataType, Pred> DataType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::DataType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_ComponentContainer_parent;
		Udm::ParentAttr<PICML::ComponentContainer> ComponentContainer_parent() const { return Udm::ParentAttr<PICML::ComponentContainer>(impl, meta_ComponentContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  ConfigProperty :  public MgaObject {
	 public:
		static Uml::Class meta;

		ConfigProperty() { }
		ConfigProperty(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ConfigProperty(const ConfigProperty &master) : MgaObject(master) { }
		static ConfigProperty Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ConfigProperty Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ConfigProperty CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ConfigProperty CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ConfigProperty> Instances() { return Udm::InstantiatedAttr<PICML::ConfigProperty>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ConfigProperty, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ConfigProperty, Pred>(impl);}

		Udm::DerivedAttr<PICML::ConfigProperty> Derived() { return Udm::DerivedAttr<PICML::ConfigProperty>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ConfigProperty, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ConfigProperty, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ConfigProperty> Archetype() { return Udm::ArchetypeAttr<PICML::ConfigProperty>(impl);}

		static Uml::CompositionParentRole meta_ComponentImplementationContainer_parent;
		Udm::ParentAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_parent() const { return Udm::ParentAttr<PICML::ComponentImplementationContainer>(impl, meta_ComponentImplementationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcConfigProperty_end_;
		Udm::AssocEndAttr<PICML::ComponentImplementation> srcConfigProperty_end() const { return Udm::AssocEndAttr<PICML::ComponentImplementation>(impl, meta_srcConfigProperty_end_); }

		static Uml::AssociationRole meta_dstConfigProperty_end_;
		Udm::AssocEndAttr<PICML::Property> dstConfigProperty_end() const { return Udm::AssocEndAttr<PICML::Property>(impl, meta_dstConfigProperty_end_); }

	};

	class  Implements :  public MgaObject {
	 public:
		static Uml::Class meta;

		Implements() { }
		Implements(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Implements(const Implements &master) : MgaObject(master) { }
		static Implements Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Implements Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Implements CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Implements CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Implements> Instances() { return Udm::InstantiatedAttr<PICML::Implements>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Implements, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Implements, Pred>(impl);}

		Udm::DerivedAttr<PICML::Implements> Derived() { return Udm::DerivedAttr<PICML::Implements>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Implements, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Implements, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Implements> Archetype() { return Udm::ArchetypeAttr<PICML::Implements>(impl);}

		static Uml::CompositionParentRole meta_ComponentImplementationContainer_parent;
		Udm::ParentAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_parent() const { return Udm::ParentAttr<PICML::ComponentImplementationContainer>(impl, meta_ComponentImplementationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstImplements_end_;
		Udm::AssocEndAttr<PICML::ComponentType> dstImplements_end() const { return Udm::AssocEndAttr<PICML::ComponentType>(impl, meta_dstImplements_end_); }

		static Uml::AssociationRole meta_srcImplements_end_;
		Udm::AssocEndAttr<PICML::ComponentImplementation> srcImplements_end() const { return Udm::AssocEndAttr<PICML::ComponentImplementation>(impl, meta_srcImplements_end_); }

	};

	class  ImplementationCapability :  public MgaObject {
	 public:
		static Uml::Class meta;

		ImplementationCapability() { }
		ImplementationCapability(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ImplementationCapability(const ImplementationCapability &master) : MgaObject(master) { }
		static ImplementationCapability Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ImplementationCapability Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ImplementationCapability CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ImplementationCapability CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ImplementationCapability> Instances() { return Udm::InstantiatedAttr<PICML::ImplementationCapability>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ImplementationCapability, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ImplementationCapability, Pred>(impl);}

		Udm::DerivedAttr<PICML::ImplementationCapability> Derived() { return Udm::DerivedAttr<PICML::ImplementationCapability>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ImplementationCapability, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ImplementationCapability, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ImplementationCapability> Archetype() { return Udm::ArchetypeAttr<PICML::ImplementationCapability>(impl);}

		static Uml::CompositionParentRole meta_ComponentImplementationContainer_parent;
		Udm::ParentAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_parent() const { return Udm::ParentAttr<PICML::ComponentImplementationContainer>(impl, meta_ComponentImplementationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcImplementationCapability_end_;
		Udm::AssocEndAttr<PICML::ComponentImplementation> srcImplementationCapability_end() const { return Udm::AssocEndAttr<PICML::ComponentImplementation>(impl, meta_srcImplementationCapability_end_); }

		static Uml::AssociationRole meta_dstImplementationCapability_end_;
		Udm::AssocEndAttr<PICML::Capability> dstImplementationCapability_end() const { return Udm::AssocEndAttr<PICML::Capability>(impl, meta_dstImplementationCapability_end_); }

	};

	class  ImplementationDependsOn :  public MgaObject {
	 public:
		static Uml::Class meta;

		ImplementationDependsOn() { }
		ImplementationDependsOn(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ImplementationDependsOn(const ImplementationDependsOn &master) : MgaObject(master) { }
		static ImplementationDependsOn Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ImplementationDependsOn Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ImplementationDependsOn CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ImplementationDependsOn CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ImplementationDependsOn> Instances() { return Udm::InstantiatedAttr<PICML::ImplementationDependsOn>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ImplementationDependsOn, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ImplementationDependsOn, Pred>(impl);}

		Udm::DerivedAttr<PICML::ImplementationDependsOn> Derived() { return Udm::DerivedAttr<PICML::ImplementationDependsOn>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ImplementationDependsOn, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ImplementationDependsOn, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ImplementationDependsOn> Archetype() { return Udm::ArchetypeAttr<PICML::ImplementationDependsOn>(impl);}

		static Uml::CompositionParentRole meta_ComponentImplementationContainer_parent;
		Udm::ParentAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_parent() const { return Udm::ParentAttr<PICML::ComponentImplementationContainer>(impl, meta_ComponentImplementationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcImplementationDependsOn_end_;
		Udm::AssocEndAttr<PICML::ComponentImplementation> srcImplementationDependsOn_end() const { return Udm::AssocEndAttr<PICML::ComponentImplementation>(impl, meta_srcImplementationDependsOn_end_); }

		static Uml::AssociationRole meta_dstImplementationDependsOn_end_;
		Udm::AssocEndAttr<PICML::ImplementationDependency> dstImplementationDependsOn_end() const { return Udm::AssocEndAttr<PICML::ImplementationDependency>(impl, meta_dstImplementationDependsOn_end_); }

	};

	class  InfoProperty :  public MgaObject {
	 public:
		static Uml::Class meta;

		InfoProperty() { }
		InfoProperty(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		InfoProperty(const InfoProperty &master) : MgaObject(master) { }
		static InfoProperty Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static InfoProperty Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		InfoProperty CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		InfoProperty CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<InfoProperty> Instances() { return Udm::InstantiatedAttr<PICML::InfoProperty>(impl);}
		template <class Pred> Udm::InstantiatedAttr<InfoProperty, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::InfoProperty, Pred>(impl);}

		Udm::DerivedAttr<PICML::InfoProperty> Derived() { return Udm::DerivedAttr<PICML::InfoProperty>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::InfoProperty, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::InfoProperty, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::InfoProperty> Archetype() { return Udm::ArchetypeAttr<PICML::InfoProperty>(impl);}

		static Uml::CompositionParentRole meta_ComponentImplementationContainer_parent;
		Udm::ParentAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_parent() const { return Udm::ParentAttr<PICML::ComponentImplementationContainer>(impl, meta_ComponentImplementationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcInfoProperty_end_;
		Udm::AssocEndAttr<PICML::ComponentImplementation> srcInfoProperty_end() const { return Udm::AssocEndAttr<PICML::ComponentImplementation>(impl, meta_srcInfoProperty_end_); }

		static Uml::AssociationRole meta_dstInfoProperty_end_;
		Udm::AssocEndAttr<PICML::Property> dstInfoProperty_end() const { return Udm::AssocEndAttr<PICML::Property>(impl, meta_dstInfoProperty_end_); }

	};

	class  MonolithprimaryArtifact :  public MgaObject {
	 public:
		static Uml::Class meta;

		MonolithprimaryArtifact() { }
		MonolithprimaryArtifact(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		MonolithprimaryArtifact(const MonolithprimaryArtifact &master) : MgaObject(master) { }
		static MonolithprimaryArtifact Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static MonolithprimaryArtifact Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		MonolithprimaryArtifact CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		MonolithprimaryArtifact CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<MonolithprimaryArtifact> Instances() { return Udm::InstantiatedAttr<PICML::MonolithprimaryArtifact>(impl);}
		template <class Pred> Udm::InstantiatedAttr<MonolithprimaryArtifact, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::MonolithprimaryArtifact, Pred>(impl);}

		Udm::DerivedAttr<PICML::MonolithprimaryArtifact> Derived() { return Udm::DerivedAttr<PICML::MonolithprimaryArtifact>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::MonolithprimaryArtifact, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::MonolithprimaryArtifact, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::MonolithprimaryArtifact> Archetype() { return Udm::ArchetypeAttr<PICML::MonolithprimaryArtifact>(impl);}

		static Uml::CompositionParentRole meta_ComponentImplementationContainer_parent;
		Udm::ParentAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_parent() const { return Udm::ParentAttr<PICML::ComponentImplementationContainer>(impl, meta_ComponentImplementationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcMonolithprimaryArtifact_end_;
		Udm::AssocEndAttr<PICML::MonolithicImplementation> srcMonolithprimaryArtifact_end() const { return Udm::AssocEndAttr<PICML::MonolithicImplementation>(impl, meta_srcMonolithprimaryArtifact_end_); }

		static Uml::AssociationRole meta_dstMonolithprimaryArtifact_end_;
		Udm::AssocEndAttr<PICML::ImplementationArtifactReference> dstMonolithprimaryArtifact_end() const { return Udm::AssocEndAttr<PICML::ImplementationArtifactReference>(impl, meta_dstMonolithprimaryArtifact_end_); }

	};

	class  ComponentImplementationReference :  public MgaObject {
	 public:
		static Uml::Class meta;

		ComponentImplementationReference() { }
		ComponentImplementationReference(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ComponentImplementationReference(const ComponentImplementationReference &master) : MgaObject(master) { }
		static ComponentImplementationReference Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentImplementationReference Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentImplementationReference CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentImplementationReference CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentImplementationReference> Instances() { return Udm::InstantiatedAttr<PICML::ComponentImplementationReference>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentImplementationReference, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentImplementationReference, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentImplementationReference> Derived() { return Udm::DerivedAttr<PICML::ComponentImplementationReference>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentImplementationReference, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentImplementationReference, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentImplementationReference> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentImplementationReference>(impl);}

		static Uml::AssociationRole meta_srcImplementation, meta_srcImplementation_rev;
		Udm::AClassAssocAttr<PICML::Implementation, PICML::ComponentPackage> srcImplementation() const { return Udm::AClassAssocAttr<PICML::Implementation, PICML::ComponentPackage>(impl, meta_srcImplementation, meta_srcImplementation_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::Implementation, PICML::ComponentPackage, Pred> srcImplementation_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::Implementation, PICML::ComponentPackage, Pred>(impl, meta_srcImplementation, meta_srcImplementation_rev); }

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::ComponentImplementation> ref() const { return Udm::PointerAttr<PICML::ComponentImplementation>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_PackageContainer_parent;
		Udm::ParentAttr<PICML::PackageContainer> PackageContainer_parent() const { return Udm::ParentAttr<PICML::PackageContainer>(impl, meta_PackageContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  MonolithExecParameter :  public MgaObject {
	 public:
		static Uml::Class meta;

		MonolithExecParameter() { }
		MonolithExecParameter(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		MonolithExecParameter(const MonolithExecParameter &master) : MgaObject(master) { }
		static MonolithExecParameter Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static MonolithExecParameter Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		MonolithExecParameter CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		MonolithExecParameter CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<MonolithExecParameter> Instances() { return Udm::InstantiatedAttr<PICML::MonolithExecParameter>(impl);}
		template <class Pred> Udm::InstantiatedAttr<MonolithExecParameter, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::MonolithExecParameter, Pred>(impl);}

		Udm::DerivedAttr<PICML::MonolithExecParameter> Derived() { return Udm::DerivedAttr<PICML::MonolithExecParameter>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::MonolithExecParameter, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::MonolithExecParameter, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::MonolithExecParameter> Archetype() { return Udm::ArchetypeAttr<PICML::MonolithExecParameter>(impl);}

		static Uml::CompositionParentRole meta_ComponentImplementationContainer_parent;
		Udm::ParentAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_parent() const { return Udm::ParentAttr<PICML::ComponentImplementationContainer>(impl, meta_ComponentImplementationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcMonolithExecParameter_end_;
		Udm::AssocEndAttr<PICML::MonolithicImplementation> srcMonolithExecParameter_end() const { return Udm::AssocEndAttr<PICML::MonolithicImplementation>(impl, meta_srcMonolithExecParameter_end_); }

		static Uml::AssociationRole meta_dstMonolithExecParameter_end_;
		Udm::AssocEndAttr<PICML::Property> dstMonolithExecParameter_end() const { return Udm::AssocEndAttr<PICML::Property>(impl, meta_dstMonolithExecParameter_end_); }

	};

	class  ComponentImplementationContainer :  public MgaObject {
	 public:
		static Uml::Class meta;

		ComponentImplementationContainer() { }
		ComponentImplementationContainer(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ComponentImplementationContainer(const ComponentImplementationContainer &master) : MgaObject(master) { }
		static ComponentImplementationContainer Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentImplementationContainer Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentImplementationContainer CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentImplementationContainer CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentImplementationContainer> Instances() { return Udm::InstantiatedAttr<PICML::ComponentImplementationContainer>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentImplementationContainer, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentImplementationContainer, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentImplementationContainer> Derived() { return Udm::DerivedAttr<PICML::ComponentImplementationContainer>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentImplementationContainer, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentImplementationContainer, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentImplementationContainer> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentImplementationContainer>(impl);}

		static Uml::CompositionChildRole meta_ComponentType_children;
		Udm::ChildrenAttr<PICML::ComponentType> ComponentType_children() const { return Udm::ChildrenAttr<PICML::ComponentType>(impl, meta_ComponentType_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentType, Pred> ComponentType_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentType, Pred>(impl, meta_ComponentType_children); }

		static Uml::CompositionChildRole meta_ConfigProperty_children;
		Udm::ChildrenAttr<PICML::ConfigProperty> ConfigProperty_children() const { return Udm::ChildrenAttr<PICML::ConfigProperty>(impl, meta_ConfigProperty_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ConfigProperty, Pred> ConfigProperty_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ConfigProperty, Pred>(impl, meta_ConfigProperty_children); }

		static Uml::CompositionChildRole meta_MonolithprimaryArtifact_children;
		Udm::ChildrenAttr<PICML::MonolithprimaryArtifact> MonolithprimaryArtifact_children() const { return Udm::ChildrenAttr<PICML::MonolithprimaryArtifact>(impl, meta_MonolithprimaryArtifact_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::MonolithprimaryArtifact, Pred> MonolithprimaryArtifact_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MonolithprimaryArtifact, Pred>(impl, meta_MonolithprimaryArtifact_children); }

		static Uml::CompositionChildRole meta_Property_children;
		Udm::ChildrenAttr<PICML::Property> Property_children() const { return Udm::ChildrenAttr<PICML::Property>(impl, meta_Property_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Property, Pred> Property_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Property, Pred>(impl, meta_Property_children); }

		static Uml::CompositionChildRole meta_ImplementationCapability_children;
		Udm::ChildrenAttr<PICML::ImplementationCapability> ImplementationCapability_children() const { return Udm::ChildrenAttr<PICML::ImplementationCapability>(impl, meta_ImplementationCapability_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ImplementationCapability, Pred> ImplementationCapability_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ImplementationCapability, Pred>(impl, meta_ImplementationCapability_children); }

		static Uml::CompositionChildRole meta_ImplementationDependsOn_children;
		Udm::ChildrenAttr<PICML::ImplementationDependsOn> ImplementationDependsOn_children() const { return Udm::ChildrenAttr<PICML::ImplementationDependsOn>(impl, meta_ImplementationDependsOn_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ImplementationDependsOn, Pred> ImplementationDependsOn_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ImplementationDependsOn, Pred>(impl, meta_ImplementationDependsOn_children); }

		static Uml::CompositionChildRole meta_Implements_children;
		Udm::ChildrenAttr<PICML::Implements> Implements_children() const { return Udm::ChildrenAttr<PICML::Implements>(impl, meta_Implements_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Implements, Pred> Implements_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Implements, Pred>(impl, meta_Implements_children); }

		static Uml::CompositionChildRole meta_MonolithDeployRequirement_children;
		Udm::ChildrenAttr<PICML::MonolithDeployRequirement> MonolithDeployRequirement_children() const { return Udm::ChildrenAttr<PICML::MonolithDeployRequirement>(impl, meta_MonolithDeployRequirement_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::MonolithDeployRequirement, Pred> MonolithDeployRequirement_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MonolithDeployRequirement, Pred>(impl, meta_MonolithDeployRequirement_children); }

		static Uml::CompositionChildRole meta_ImplementationRequirement_children;
		Udm::ChildrenAttr<PICML::ImplementationRequirement> ImplementationRequirement_children() const { return Udm::ChildrenAttr<PICML::ImplementationRequirement>(impl, meta_ImplementationRequirement_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ImplementationRequirement, Pred> ImplementationRequirement_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ImplementationRequirement, Pred>(impl, meta_ImplementationRequirement_children); }

		static Uml::CompositionChildRole meta_ImplementationArtifactReference_children;
		Udm::ChildrenAttr<PICML::ImplementationArtifactReference> ImplementationArtifactReference_children() const { return Udm::ChildrenAttr<PICML::ImplementationArtifactReference>(impl, meta_ImplementationArtifactReference_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ImplementationArtifactReference, Pred> ImplementationArtifactReference_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ImplementationArtifactReference, Pred>(impl, meta_ImplementationArtifactReference_children); }

		static Uml::CompositionChildRole meta_InfoProperty_children;
		Udm::ChildrenAttr<PICML::InfoProperty> InfoProperty_children() const { return Udm::ChildrenAttr<PICML::InfoProperty>(impl, meta_InfoProperty_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::InfoProperty, Pred> InfoProperty_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::InfoProperty, Pred>(impl, meta_InfoProperty_children); }

		static Uml::CompositionChildRole meta_Capability_children;
		Udm::ChildrenAttr<PICML::Capability> Capability_children() const { return Udm::ChildrenAttr<PICML::Capability>(impl, meta_Capability_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Capability, Pred> Capability_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Capability, Pred>(impl, meta_Capability_children); }

		static Uml::CompositionChildRole meta_MonolithExecParameter_children;
		Udm::ChildrenAttr<PICML::MonolithExecParameter> MonolithExecParameter_children() const { return Udm::ChildrenAttr<PICML::MonolithExecParameter>(impl, meta_MonolithExecParameter_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::MonolithExecParameter, Pred> MonolithExecParameter_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MonolithExecParameter, Pred>(impl, meta_MonolithExecParameter_children); }

		static Uml::CompositionChildRole meta_ComponentImplementation_child;
		Udm::ChildAttr<PICML::ComponentImplementation> ComponentImplementation_child() const { return Udm::ChildAttr<PICML::ComponentImplementation>(impl, meta_ComponentImplementation_child); }

		static Uml::CompositionChildRole meta_ImplementationDependency_children;
		Udm::ChildrenAttr<PICML::ImplementationDependency> ImplementationDependency_children() const { return Udm::ChildrenAttr<PICML::ImplementationDependency>(impl, meta_ImplementationDependency_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ImplementationDependency, Pred> ImplementationDependency_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ImplementationDependency, Pred>(impl, meta_ImplementationDependency_children); }

		Udm::ChildrenAttr<PICML::ComponentType> ComponentType_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentType, Pred> ComponentType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ConfigProperty> ConfigProperty_kind_children() const { return Udm::ChildrenAttr<PICML::ConfigProperty>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ConfigProperty, Pred> ConfigProperty_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ConfigProperty, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Implements> Implements_kind_children() const { return Udm::ChildrenAttr<PICML::Implements>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Implements, Pred> Implements_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Implements, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ImplementationCapability> ImplementationCapability_kind_children() const { return Udm::ChildrenAttr<PICML::ImplementationCapability>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ImplementationCapability, Pred> ImplementationCapability_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ImplementationCapability, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ImplementationDependsOn> ImplementationDependsOn_kind_children() const { return Udm::ChildrenAttr<PICML::ImplementationDependsOn>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ImplementationDependsOn, Pred> ImplementationDependsOn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ImplementationDependsOn, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::InfoProperty> InfoProperty_kind_children() const { return Udm::ChildrenAttr<PICML::InfoProperty>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::InfoProperty, Pred> InfoProperty_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::InfoProperty, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MonolithprimaryArtifact> MonolithprimaryArtifact_kind_children() const { return Udm::ChildrenAttr<PICML::MonolithprimaryArtifact>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MonolithprimaryArtifact, Pred> MonolithprimaryArtifact_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MonolithprimaryArtifact, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MonolithExecParameter> MonolithExecParameter_kind_children() const { return Udm::ChildrenAttr<PICML::MonolithExecParameter>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MonolithExecParameter, Pred> MonolithExecParameter_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MonolithExecParameter, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MonolithDeployRequirement> MonolithDeployRequirement_kind_children() const { return Udm::ChildrenAttr<PICML::MonolithDeployRequirement>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MonolithDeployRequirement, Pred> MonolithDeployRequirement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MonolithDeployRequirement, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentImplementation> ComponentImplementation_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentImplementation>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentImplementation, Pred> ComponentImplementation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentImplementation, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MonolithicImplementation> MonolithicImplementation_kind_children() const { return Udm::ChildrenAttr<PICML::MonolithicImplementation>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MonolithicImplementation, Pred> MonolithicImplementation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MonolithicImplementation, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ImplementationDependency> ImplementationDependency_kind_children() const { return Udm::ChildrenAttr<PICML::ImplementationDependency>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ImplementationDependency, Pred> ImplementationDependency_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ImplementationDependency, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Property> Property_kind_children() const { return Udm::ChildrenAttr<PICML::Property>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Property, Pred> Property_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Property, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Capability> Capability_kind_children() const { return Udm::ChildrenAttr<PICML::Capability>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Capability, Pred> Capability_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Capability, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::RequirementSatisfier> RequirementSatisfier_kind_children() const { return Udm::ChildrenAttr<PICML::RequirementSatisfier>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::RequirementSatisfier, Pred> RequirementSatisfier_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::RequirementSatisfier, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ImplementationRequirement> ImplementationRequirement_kind_children() const { return Udm::ChildrenAttr<PICML::ImplementationRequirement>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ImplementationRequirement, Pred> ImplementationRequirement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ImplementationRequirement, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::RequirementBase> RequirementBase_kind_children() const { return Udm::ChildrenAttr<PICML::RequirementBase>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::RequirementBase, Pred> RequirementBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::RequirementBase, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentAssembly> ComponentAssembly_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentAssembly>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentAssembly, Pred> ComponentAssembly_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentAssembly, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ImplementationArtifactReference> ImplementationArtifactReference_kind_children() const { return Udm::ChildrenAttr<PICML::ImplementationArtifactReference>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ImplementationArtifactReference, Pred> ImplementationArtifactReference_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ImplementationArtifactReference, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_ComponentImplementations_parent;
		Udm::ParentAttr<PICML::ComponentImplementations> ComponentImplementations_parent() const { return Udm::ParentAttr<PICML::ComponentImplementations>(impl, meta_ComponentImplementations_parent); }

		Udm::ParentAttr<PICML::ComponentImplementations> parent() const { return Udm::ParentAttr<PICML::ComponentImplementations>(impl, Udm::NULLPARENTROLE); }
	};

	class  MonolithDeployRequirement :  public MgaObject {
	 public:
		static Uml::Class meta;

		MonolithDeployRequirement() { }
		MonolithDeployRequirement(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		MonolithDeployRequirement(const MonolithDeployRequirement &master) : MgaObject(master) { }
		static MonolithDeployRequirement Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static MonolithDeployRequirement Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		MonolithDeployRequirement CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		MonolithDeployRequirement CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<MonolithDeployRequirement> Instances() { return Udm::InstantiatedAttr<PICML::MonolithDeployRequirement>(impl);}
		template <class Pred> Udm::InstantiatedAttr<MonolithDeployRequirement, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::MonolithDeployRequirement, Pred>(impl);}

		Udm::DerivedAttr<PICML::MonolithDeployRequirement> Derived() { return Udm::DerivedAttr<PICML::MonolithDeployRequirement>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::MonolithDeployRequirement, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::MonolithDeployRequirement, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::MonolithDeployRequirement> Archetype() { return Udm::ArchetypeAttr<PICML::MonolithDeployRequirement>(impl);}

		static Uml::CompositionParentRole meta_ComponentImplementationContainer_parent;
		Udm::ParentAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_parent() const { return Udm::ParentAttr<PICML::ComponentImplementationContainer>(impl, meta_ComponentImplementationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcMonolithDeployRequirement_end_;
		Udm::AssocEndAttr<PICML::MonolithicImplementation> srcMonolithDeployRequirement_end() const { return Udm::AssocEndAttr<PICML::MonolithicImplementation>(impl, meta_srcMonolithDeployRequirement_end_); }

		static Uml::AssociationRole meta_dstMonolithDeployRequirement_end_;
		Udm::AssocEndAttr<PICML::ImplementationRequirement> dstMonolithDeployRequirement_end() const { return Udm::AssocEndAttr<PICML::ImplementationRequirement>(impl, meta_dstMonolithDeployRequirement_end_); }

	};

	class  ComponentImplementation :  public MgaObject {
	 public:
		static Uml::Class meta;

		ComponentImplementation() { }
		ComponentImplementation(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ComponentImplementation(const ComponentImplementation &master) : MgaObject(master) { }
		static ComponentImplementation Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentImplementation Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentImplementation CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentImplementation CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentImplementation> Instances() { return Udm::InstantiatedAttr<PICML::ComponentImplementation>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentImplementation, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentImplementation, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentImplementation> Derived() { return Udm::DerivedAttr<PICML::ComponentImplementation>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentImplementation, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentImplementation, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentImplementation> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentImplementation>(impl);}

		static Uml::Attribute meta_UUID;
		Udm::StringAttr UUID() const { return Udm::StringAttr(impl, meta_UUID); }

		static Uml::Attribute meta_label;
		Udm::StringAttr label() const { return Udm::StringAttr(impl, meta_label); }

		static Uml::AssociationRole meta_dstImplements, meta_dstImplements_rev;
		Udm::AClassAssocAttr<PICML::Implements, PICML::ComponentType> dstImplements() const { return Udm::AClassAssocAttr<PICML::Implements, PICML::ComponentType>(impl, meta_dstImplements, meta_dstImplements_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::Implements, PICML::ComponentType, Pred> dstImplements_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::Implements, PICML::ComponentType, Pred>(impl, meta_dstImplements, meta_dstImplements_rev); }

		static Uml::AssociationRole meta_referedbyComponentImplementationReference;
		Udm::AssocAttr<PICML::ComponentImplementationReference> referedbyComponentImplementationReference() const { return Udm::AssocAttr<PICML::ComponentImplementationReference>(impl, meta_referedbyComponentImplementationReference); }
		template <class Pred> Udm::AssocAttr<PICML::ComponentImplementationReference, Pred > referedbyComponentImplementationReference_sorted(const Pred &) const { return Udm::AssocAttr<PICML::ComponentImplementationReference, Pred>(impl, meta_referedbyComponentImplementationReference); }

		static Uml::AssociationRole meta_dstInfoProperty, meta_dstInfoProperty_rev;
		Udm::AClassAssocAttr<PICML::InfoProperty, PICML::Property> dstInfoProperty() const { return Udm::AClassAssocAttr<PICML::InfoProperty, PICML::Property>(impl, meta_dstInfoProperty, meta_dstInfoProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::InfoProperty, PICML::Property, Pred> dstInfoProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::InfoProperty, PICML::Property, Pred>(impl, meta_dstInfoProperty, meta_dstInfoProperty_rev); }

		static Uml::AssociationRole meta_dstImplementationCapability, meta_dstImplementationCapability_rev;
		Udm::AClassAssocAttr<PICML::ImplementationCapability, PICML::Capability> dstImplementationCapability() const { return Udm::AClassAssocAttr<PICML::ImplementationCapability, PICML::Capability>(impl, meta_dstImplementationCapability, meta_dstImplementationCapability_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ImplementationCapability, PICML::Capability, Pred> dstImplementationCapability_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ImplementationCapability, PICML::Capability, Pred>(impl, meta_dstImplementationCapability, meta_dstImplementationCapability_rev); }

		static Uml::AssociationRole meta_dstConfigProperty, meta_dstConfigProperty_rev;
		Udm::AClassAssocAttr<PICML::ConfigProperty, PICML::Property> dstConfigProperty() const { return Udm::AClassAssocAttr<PICML::ConfigProperty, PICML::Property>(impl, meta_dstConfigProperty, meta_dstConfigProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ConfigProperty, PICML::Property, Pred> dstConfigProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ConfigProperty, PICML::Property, Pred>(impl, meta_dstConfigProperty, meta_dstConfigProperty_rev); }

		static Uml::AssociationRole meta_dstImplementationDependsOn, meta_dstImplementationDependsOn_rev;
		Udm::AClassAssocAttr<PICML::ImplementationDependsOn, PICML::ImplementationDependency> dstImplementationDependsOn() const { return Udm::AClassAssocAttr<PICML::ImplementationDependsOn, PICML::ImplementationDependency>(impl, meta_dstImplementationDependsOn, meta_dstImplementationDependsOn_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ImplementationDependsOn, PICML::ImplementationDependency, Pred> dstImplementationDependsOn_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ImplementationDependsOn, PICML::ImplementationDependency, Pred>(impl, meta_dstImplementationDependsOn, meta_dstImplementationDependsOn_rev); }

		static Uml::CompositionParentRole meta_ComponentImplementationContainer_parent;
		Udm::ParentAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_parent() const { return Udm::ParentAttr<PICML::ComponentImplementationContainer>(impl, meta_ComponentImplementationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  MonolithicImplementation :  public ComponentImplementation {
	 public:
		static Uml::Class meta;

		MonolithicImplementation() { }
		MonolithicImplementation(Udm::ObjectImpl *impl) : ComponentImplementation(impl) { }
		MonolithicImplementation(const MonolithicImplementation &master) : ComponentImplementation(master) { }
		static MonolithicImplementation Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static MonolithicImplementation Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		MonolithicImplementation CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		MonolithicImplementation CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<MonolithicImplementation> Instances() { return Udm::InstantiatedAttr<PICML::MonolithicImplementation>(impl);}
		template <class Pred> Udm::InstantiatedAttr<MonolithicImplementation, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::MonolithicImplementation, Pred>(impl);}

		Udm::DerivedAttr<PICML::MonolithicImplementation> Derived() { return Udm::DerivedAttr<PICML::MonolithicImplementation>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::MonolithicImplementation, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::MonolithicImplementation, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::MonolithicImplementation> Archetype() { return Udm::ArchetypeAttr<PICML::MonolithicImplementation>(impl);}

		static Uml::AssociationRole meta_dstMonolithExecParameter, meta_dstMonolithExecParameter_rev;
		Udm::AClassAssocAttr<PICML::MonolithExecParameter, PICML::Property> dstMonolithExecParameter() const { return Udm::AClassAssocAttr<PICML::MonolithExecParameter, PICML::Property>(impl, meta_dstMonolithExecParameter, meta_dstMonolithExecParameter_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::MonolithExecParameter, PICML::Property, Pred> dstMonolithExecParameter_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::MonolithExecParameter, PICML::Property, Pred>(impl, meta_dstMonolithExecParameter, meta_dstMonolithExecParameter_rev); }

		static Uml::AssociationRole meta_dstMonolithDeployRequirement, meta_dstMonolithDeployRequirement_rev;
		Udm::AClassAssocAttr<PICML::MonolithDeployRequirement, PICML::ImplementationRequirement> dstMonolithDeployRequirement() const { return Udm::AClassAssocAttr<PICML::MonolithDeployRequirement, PICML::ImplementationRequirement>(impl, meta_dstMonolithDeployRequirement, meta_dstMonolithDeployRequirement_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::MonolithDeployRequirement, PICML::ImplementationRequirement, Pred> dstMonolithDeployRequirement_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::MonolithDeployRequirement, PICML::ImplementationRequirement, Pred>(impl, meta_dstMonolithDeployRequirement, meta_dstMonolithDeployRequirement_rev); }

		static Uml::AssociationRole meta_dstMonolithprimaryArtifact, meta_dstMonolithprimaryArtifact_rev;
		Udm::AClassAssocAttr<PICML::MonolithprimaryArtifact, PICML::ImplementationArtifactReference> dstMonolithprimaryArtifact() const { return Udm::AClassAssocAttr<PICML::MonolithprimaryArtifact, PICML::ImplementationArtifactReference>(impl, meta_dstMonolithprimaryArtifact, meta_dstMonolithprimaryArtifact_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::MonolithprimaryArtifact, PICML::ImplementationArtifactReference, Pred> dstMonolithprimaryArtifact_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::MonolithprimaryArtifact, PICML::ImplementationArtifactReference, Pred>(impl, meta_dstMonolithprimaryArtifact, meta_dstMonolithprimaryArtifact_rev); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  DeploymentPlan :  public MgaObject {
	 public:
		static Uml::Class meta;

		DeploymentPlan() { }
		DeploymentPlan(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		DeploymentPlan(const DeploymentPlan &master) : MgaObject(master) { }
		static DeploymentPlan Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static DeploymentPlan Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		DeploymentPlan CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		DeploymentPlan CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<DeploymentPlan> Instances() { return Udm::InstantiatedAttr<PICML::DeploymentPlan>(impl);}
		template <class Pred> Udm::InstantiatedAttr<DeploymentPlan, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::DeploymentPlan, Pred>(impl);}

		Udm::DerivedAttr<PICML::DeploymentPlan> Derived() { return Udm::DerivedAttr<PICML::DeploymentPlan>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::DeploymentPlan, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::DeploymentPlan, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::DeploymentPlan> Archetype() { return Udm::ArchetypeAttr<PICML::DeploymentPlan>(impl);}

		static Uml::Attribute meta_label;
		Udm::StringAttr label() const { return Udm::StringAttr(impl, meta_label); }

		static Uml::CompositionChildRole meta_InstanceMapping_children;
		Udm::ChildrenAttr<PICML::InstanceMapping> InstanceMapping_children() const { return Udm::ChildrenAttr<PICML::InstanceMapping>(impl, meta_InstanceMapping_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::InstanceMapping, Pred> InstanceMapping_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::InstanceMapping, Pred>(impl, meta_InstanceMapping_children); }

		static Uml::CompositionChildRole meta_CollocationGroup_children;
		Udm::ChildrenAttr<PICML::CollocationGroup> CollocationGroup_children() const { return Udm::ChildrenAttr<PICML::CollocationGroup>(impl, meta_CollocationGroup_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::CollocationGroup, Pred> CollocationGroup_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::CollocationGroup, Pred>(impl, meta_CollocationGroup_children); }

		static Uml::CompositionChildRole meta_ComponentType_children;
		Udm::ChildrenAttr<PICML::ComponentType> ComponentType_children() const { return Udm::ChildrenAttr<PICML::ComponentType>(impl, meta_ComponentType_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentType, Pred> ComponentType_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentType, Pred>(impl, meta_ComponentType_children); }

		static Uml::CompositionChildRole meta_NodeReference_children;
		Udm::ChildrenAttr<PICML::NodeReference> NodeReference_children() const { return Udm::ChildrenAttr<PICML::NodeReference>(impl, meta_NodeReference_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::NodeReference, Pred> NodeReference_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::NodeReference, Pred>(impl, meta_NodeReference_children); }

		Udm::ChildrenAttr<PICML::ComponentType> ComponentType_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentType, Pred> ComponentType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::CollocationGroup> CollocationGroup_kind_children() const { return Udm::ChildrenAttr<PICML::CollocationGroup>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::CollocationGroup, Pred> CollocationGroup_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::CollocationGroup, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::InstanceMapping> InstanceMapping_kind_children() const { return Udm::ChildrenAttr<PICML::InstanceMapping>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::InstanceMapping, Pred> InstanceMapping_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::InstanceMapping, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::NodeReference> NodeReference_kind_children() const { return Udm::ChildrenAttr<PICML::NodeReference>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::NodeReference, Pred> NodeReference_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::NodeReference, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_DeploymentPlans_parent;
		Udm::ParentAttr<PICML::DeploymentPlans> DeploymentPlans_parent() const { return Udm::ParentAttr<PICML::DeploymentPlans>(impl, meta_DeploymentPlans_parent); }

		Udm::ParentAttr<PICML::DeploymentPlans> parent() const { return Udm::ParentAttr<PICML::DeploymentPlans>(impl, Udm::NULLPARENTROLE); }
	};

	class  CollocationGroup :  public MgaObject {
	 public:
		static Uml::Class meta;

		CollocationGroup() { }
		CollocationGroup(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		CollocationGroup(const CollocationGroup &master) : MgaObject(master) { }
		static CollocationGroup Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static CollocationGroup Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		CollocationGroup CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		CollocationGroup CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<CollocationGroup> Instances() { return Udm::InstantiatedAttr<PICML::CollocationGroup>(impl);}
		template <class Pred> Udm::InstantiatedAttr<CollocationGroup, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::CollocationGroup, Pred>(impl);}

		Udm::DerivedAttr<PICML::CollocationGroup> Derived() { return Udm::DerivedAttr<PICML::CollocationGroup>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::CollocationGroup, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::CollocationGroup, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::CollocationGroup> Archetype() { return Udm::ArchetypeAttr<PICML::CollocationGroup>(impl);}

		static Uml::AssociationRole meta_members;
		Udm::AssocAttr<PICML::ComponentType> members() const { return Udm::AssocAttr<PICML::ComponentType>(impl, meta_members); }
		template <class Pred> Udm::AssocAttr<PICML::ComponentType, Pred > members_sorted(const Pred &) const { return Udm::AssocAttr<PICML::ComponentType, Pred>(impl, meta_members); }

		static Uml::AssociationRole meta_dstInstanceMapping, meta_dstInstanceMapping_rev;
		Udm::AClassAssocAttr<PICML::InstanceMapping, PICML::NodeReference> dstInstanceMapping() const { return Udm::AClassAssocAttr<PICML::InstanceMapping, PICML::NodeReference>(impl, meta_dstInstanceMapping, meta_dstInstanceMapping_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::InstanceMapping, PICML::NodeReference, Pred> dstInstanceMapping_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::InstanceMapping, PICML::NodeReference, Pred>(impl, meta_dstInstanceMapping, meta_dstInstanceMapping_rev); }

		static Uml::CompositionParentRole meta_DeploymentPlan_parent;
		Udm::ParentAttr<PICML::DeploymentPlan> DeploymentPlan_parent() const { return Udm::ParentAttr<PICML::DeploymentPlan>(impl, meta_DeploymentPlan_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  InstanceMapping :  public MgaObject {
	 public:
		static Uml::Class meta;

		InstanceMapping() { }
		InstanceMapping(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		InstanceMapping(const InstanceMapping &master) : MgaObject(master) { }
		static InstanceMapping Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static InstanceMapping Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		InstanceMapping CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		InstanceMapping CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<InstanceMapping> Instances() { return Udm::InstantiatedAttr<PICML::InstanceMapping>(impl);}
		template <class Pred> Udm::InstantiatedAttr<InstanceMapping, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::InstanceMapping, Pred>(impl);}

		Udm::DerivedAttr<PICML::InstanceMapping> Derived() { return Udm::DerivedAttr<PICML::InstanceMapping>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::InstanceMapping, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::InstanceMapping, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::InstanceMapping> Archetype() { return Udm::ArchetypeAttr<PICML::InstanceMapping>(impl);}

		static Uml::CompositionParentRole meta_DeploymentPlan_parent;
		Udm::ParentAttr<PICML::DeploymentPlan> DeploymentPlan_parent() const { return Udm::ParentAttr<PICML::DeploymentPlan>(impl, meta_DeploymentPlan_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcInstanceMapping_end_;
		Udm::AssocEndAttr<PICML::CollocationGroup> srcInstanceMapping_end() const { return Udm::AssocEndAttr<PICML::CollocationGroup>(impl, meta_srcInstanceMapping_end_); }

		static Uml::AssociationRole meta_dstInstanceMapping_end_;
		Udm::AssocEndAttr<PICML::NodeReference> dstInstanceMapping_end() const { return Udm::AssocEndAttr<PICML::NodeReference>(impl, meta_dstInstanceMapping_end_); }

	};

	class  PackageConfSpecializedConfig :  public MgaObject {
	 public:
		static Uml::Class meta;

		PackageConfSpecializedConfig() { }
		PackageConfSpecializedConfig(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		PackageConfSpecializedConfig(const PackageConfSpecializedConfig &master) : MgaObject(master) { }
		static PackageConfSpecializedConfig Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PackageConfSpecializedConfig Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PackageConfSpecializedConfig CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PackageConfSpecializedConfig CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PackageConfSpecializedConfig> Instances() { return Udm::InstantiatedAttr<PICML::PackageConfSpecializedConfig>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PackageConfSpecializedConfig, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PackageConfSpecializedConfig, Pred>(impl);}

		Udm::DerivedAttr<PICML::PackageConfSpecializedConfig> Derived() { return Udm::DerivedAttr<PICML::PackageConfSpecializedConfig>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PackageConfSpecializedConfig, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PackageConfSpecializedConfig, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PackageConfSpecializedConfig> Archetype() { return Udm::ArchetypeAttr<PICML::PackageConfSpecializedConfig>(impl);}

		static Uml::CompositionParentRole meta_PackageConfigurationContainer_parent;
		Udm::ParentAttr<PICML::PackageConfigurationContainer> PackageConfigurationContainer_parent() const { return Udm::ParentAttr<PICML::PackageConfigurationContainer>(impl, meta_PackageConfigurationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstPackageConfSpecializedConfig_end_;
		Udm::AssocEndAttr<PICML::PackageConfigurationReference> dstPackageConfSpecializedConfig_end() const { return Udm::AssocEndAttr<PICML::PackageConfigurationReference>(impl, meta_dstPackageConfSpecializedConfig_end_); }

		static Uml::AssociationRole meta_srcPackageConfSpecializedConfig_end_;
		Udm::AssocEndAttr<PICML::PackageConfiguration> srcPackageConfSpecializedConfig_end() const { return Udm::AssocEndAttr<PICML::PackageConfiguration>(impl, meta_srcPackageConfSpecializedConfig_end_); }

	};

	class  PackageConfigurationContainer :  public MgaObject {
	 public:
		static Uml::Class meta;

		PackageConfigurationContainer() { }
		PackageConfigurationContainer(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		PackageConfigurationContainer(const PackageConfigurationContainer &master) : MgaObject(master) { }
		static PackageConfigurationContainer Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PackageConfigurationContainer Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PackageConfigurationContainer CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PackageConfigurationContainer CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PackageConfigurationContainer> Instances() { return Udm::InstantiatedAttr<PICML::PackageConfigurationContainer>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PackageConfigurationContainer, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PackageConfigurationContainer, Pred>(impl);}

		Udm::DerivedAttr<PICML::PackageConfigurationContainer> Derived() { return Udm::DerivedAttr<PICML::PackageConfigurationContainer>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PackageConfigurationContainer, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PackageConfigurationContainer, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PackageConfigurationContainer> Archetype() { return Udm::ArchetypeAttr<PICML::PackageConfigurationContainer>(impl);}

		static Uml::CompositionChildRole meta_PackageConfSelectRequirement_children;
		Udm::ChildrenAttr<PICML::PackageConfSelectRequirement> PackageConfSelectRequirement_children() const { return Udm::ChildrenAttr<PICML::PackageConfSelectRequirement>(impl, meta_PackageConfSelectRequirement_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::PackageConfSelectRequirement, Pred> PackageConfSelectRequirement_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageConfSelectRequirement, Pred>(impl, meta_PackageConfSelectRequirement_children); }

		static Uml::CompositionChildRole meta_PackageConfiguration_children;
		Udm::ChildrenAttr<PICML::PackageConfiguration> PackageConfiguration_children() const { return Udm::ChildrenAttr<PICML::PackageConfiguration>(impl, meta_PackageConfiguration_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::PackageConfiguration, Pred> PackageConfiguration_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageConfiguration, Pred>(impl, meta_PackageConfiguration_children); }

		static Uml::CompositionChildRole meta_PackageConfBasePackage_child;
		Udm::ChildAttr<PICML::PackageConfBasePackage> PackageConfBasePackage_child() const { return Udm::ChildAttr<PICML::PackageConfBasePackage>(impl, meta_PackageConfBasePackage_child); }

		static Uml::CompositionChildRole meta_PackageConfReference_child;
		Udm::ChildAttr<PICML::PackageConfReference> PackageConfReference_child() const { return Udm::ChildAttr<PICML::PackageConfReference>(impl, meta_PackageConfReference_child); }

		static Uml::CompositionChildRole meta_PackageConfConfigProperty_children;
		Udm::ChildrenAttr<PICML::PackageConfConfigProperty> PackageConfConfigProperty_children() const { return Udm::ChildrenAttr<PICML::PackageConfConfigProperty>(impl, meta_PackageConfConfigProperty_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::PackageConfConfigProperty, Pred> PackageConfConfigProperty_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageConfConfigProperty, Pred>(impl, meta_PackageConfConfigProperty_children); }

		static Uml::CompositionChildRole meta_Property_children;
		Udm::ChildrenAttr<PICML::Property> Property_children() const { return Udm::ChildrenAttr<PICML::Property>(impl, meta_Property_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Property, Pred> Property_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Property, Pred>(impl, meta_Property_children); }

		static Uml::CompositionChildRole meta_Requirement_children;
		Udm::ChildrenAttr<PICML::Requirement> Requirement_children() const { return Udm::ChildrenAttr<PICML::Requirement>(impl, meta_Requirement_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Requirement, Pred> Requirement_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Requirement, Pred>(impl, meta_Requirement_children); }

		static Uml::CompositionChildRole meta_ComponentPackageReference_child;
		Udm::ChildAttr<PICML::ComponentPackageReference> ComponentPackageReference_child() const { return Udm::ChildAttr<PICML::ComponentPackageReference>(impl, meta_ComponentPackageReference_child); }

		static Uml::CompositionChildRole meta_PackageConfSpecializedConfig_child;
		Udm::ChildAttr<PICML::PackageConfSpecializedConfig> PackageConfSpecializedConfig_child() const { return Udm::ChildAttr<PICML::PackageConfSpecializedConfig>(impl, meta_PackageConfSpecializedConfig_child); }

		static Uml::CompositionChildRole meta_ComponentPackage_child;
		Udm::ChildAttr<PICML::ComponentPackage> ComponentPackage_child() const { return Udm::ChildAttr<PICML::ComponentPackage>(impl, meta_ComponentPackage_child); }

		Udm::ChildrenAttr<PICML::PackageConfSpecializedConfig> PackageConfSpecializedConfig_kind_children() const { return Udm::ChildrenAttr<PICML::PackageConfSpecializedConfig>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PackageConfSpecializedConfig, Pred> PackageConfSpecializedConfig_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageConfSpecializedConfig, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::PackageConfConfigProperty> PackageConfConfigProperty_kind_children() const { return Udm::ChildrenAttr<PICML::PackageConfConfigProperty>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PackageConfConfigProperty, Pred> PackageConfConfigProperty_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageConfConfigProperty, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::PackageConfBasePackage> PackageConfBasePackage_kind_children() const { return Udm::ChildrenAttr<PICML::PackageConfBasePackage>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PackageConfBasePackage, Pred> PackageConfBasePackage_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageConfBasePackage, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::PackageConfSelectRequirement> PackageConfSelectRequirement_kind_children() const { return Udm::ChildrenAttr<PICML::PackageConfSelectRequirement>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PackageConfSelectRequirement, Pred> PackageConfSelectRequirement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageConfSelectRequirement, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::PackageConfReference> PackageConfReference_kind_children() const { return Udm::ChildrenAttr<PICML::PackageConfReference>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PackageConfReference, Pred> PackageConfReference_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageConfReference, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::PackageConfiguration> PackageConfiguration_kind_children() const { return Udm::ChildrenAttr<PICML::PackageConfiguration>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PackageConfiguration, Pred> PackageConfiguration_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageConfiguration, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentPackage> ComponentPackage_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentPackage>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentPackage, Pred> ComponentPackage_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentPackage, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentPackageReference> ComponentPackageReference_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentPackageReference>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentPackageReference, Pred> ComponentPackageReference_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentPackageReference, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Requirement> Requirement_kind_children() const { return Udm::ChildrenAttr<PICML::Requirement>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Requirement, Pred> Requirement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Requirement, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Property> Property_kind_children() const { return Udm::ChildrenAttr<PICML::Property>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Property, Pred> Property_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Property, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::RequirementBase> RequirementBase_kind_children() const { return Udm::ChildrenAttr<PICML::RequirementBase>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::RequirementBase, Pred> RequirementBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::RequirementBase, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_PackageConfigurations_parent;
		Udm::ParentAttr<PICML::PackageConfigurations> PackageConfigurations_parent() const { return Udm::ParentAttr<PICML::PackageConfigurations>(impl, meta_PackageConfigurations_parent); }

		Udm::ParentAttr<PICML::PackageConfigurations> parent() const { return Udm::ParentAttr<PICML::PackageConfigurations>(impl, Udm::NULLPARENTROLE); }
	};

	class  PackageConfConfigProperty :  public MgaObject {
	 public:
		static Uml::Class meta;

		PackageConfConfigProperty() { }
		PackageConfConfigProperty(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		PackageConfConfigProperty(const PackageConfConfigProperty &master) : MgaObject(master) { }
		static PackageConfConfigProperty Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PackageConfConfigProperty Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PackageConfConfigProperty CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PackageConfConfigProperty CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PackageConfConfigProperty> Instances() { return Udm::InstantiatedAttr<PICML::PackageConfConfigProperty>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PackageConfConfigProperty, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PackageConfConfigProperty, Pred>(impl);}

		Udm::DerivedAttr<PICML::PackageConfConfigProperty> Derived() { return Udm::DerivedAttr<PICML::PackageConfConfigProperty>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PackageConfConfigProperty, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PackageConfConfigProperty, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PackageConfConfigProperty> Archetype() { return Udm::ArchetypeAttr<PICML::PackageConfConfigProperty>(impl);}

		static Uml::CompositionParentRole meta_PackageConfigurationContainer_parent;
		Udm::ParentAttr<PICML::PackageConfigurationContainer> PackageConfigurationContainer_parent() const { return Udm::ParentAttr<PICML::PackageConfigurationContainer>(impl, meta_PackageConfigurationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcPackageConfConfigProperty_end_;
		Udm::AssocEndAttr<PICML::PackageConfiguration> srcPackageConfConfigProperty_end() const { return Udm::AssocEndAttr<PICML::PackageConfiguration>(impl, meta_srcPackageConfConfigProperty_end_); }

		static Uml::AssociationRole meta_dstPackageConfConfigProperty_end_;
		Udm::AssocEndAttr<PICML::Property> dstPackageConfConfigProperty_end() const { return Udm::AssocEndAttr<PICML::Property>(impl, meta_dstPackageConfConfigProperty_end_); }

	};

	class  PackageConfBasePackage :  public MgaObject {
	 public:
		static Uml::Class meta;

		PackageConfBasePackage() { }
		PackageConfBasePackage(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		PackageConfBasePackage(const PackageConfBasePackage &master) : MgaObject(master) { }
		static PackageConfBasePackage Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PackageConfBasePackage Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PackageConfBasePackage CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PackageConfBasePackage CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PackageConfBasePackage> Instances() { return Udm::InstantiatedAttr<PICML::PackageConfBasePackage>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PackageConfBasePackage, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PackageConfBasePackage, Pred>(impl);}

		Udm::DerivedAttr<PICML::PackageConfBasePackage> Derived() { return Udm::DerivedAttr<PICML::PackageConfBasePackage>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PackageConfBasePackage, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PackageConfBasePackage, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PackageConfBasePackage> Archetype() { return Udm::ArchetypeAttr<PICML::PackageConfBasePackage>(impl);}

		static Uml::CompositionParentRole meta_PackageConfigurationContainer_parent;
		Udm::ParentAttr<PICML::PackageConfigurationContainer> PackageConfigurationContainer_parent() const { return Udm::ParentAttr<PICML::PackageConfigurationContainer>(impl, meta_PackageConfigurationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcPackageConfBasePackage_end_;
		Udm::AssocEndAttr<PICML::PackageConfiguration> srcPackageConfBasePackage_end() const { return Udm::AssocEndAttr<PICML::PackageConfiguration>(impl, meta_srcPackageConfBasePackage_end_); }

		static Uml::AssociationRole meta_dstPackageConfBasePackage_end_;
		Udm::AssocEndAttr<PICML::ComponentPackage> dstPackageConfBasePackage_end() const { return Udm::AssocEndAttr<PICML::ComponentPackage>(impl, meta_dstPackageConfBasePackage_end_); }

	};

	class  PackageConfigurationReference :  public MgaObject {
	 public:
		static Uml::Class meta;

		PackageConfigurationReference() { }
		PackageConfigurationReference(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		PackageConfigurationReference(const PackageConfigurationReference &master) : MgaObject(master) { }
		static PackageConfigurationReference Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PackageConfigurationReference Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PackageConfigurationReference CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PackageConfigurationReference CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PackageConfigurationReference> Instances() { return Udm::InstantiatedAttr<PICML::PackageConfigurationReference>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PackageConfigurationReference, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PackageConfigurationReference, Pred>(impl);}

		Udm::DerivedAttr<PICML::PackageConfigurationReference> Derived() { return Udm::DerivedAttr<PICML::PackageConfigurationReference>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PackageConfigurationReference, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PackageConfigurationReference, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PackageConfigurationReference> Archetype() { return Udm::ArchetypeAttr<PICML::PackageConfigurationReference>(impl);}

		static Uml::AssociationRole meta_srcpackage, meta_srcpackage_rev;
		Udm::AClassPointerAttr<PICML::package, PICML::TopLevelPackage> srcpackage() const { return Udm::AClassPointerAttr<PICML::package, PICML::TopLevelPackage>(impl, meta_srcpackage, meta_srcpackage_rev); }

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::PackageConfiguration> ref() const { return Udm::PointerAttr<PICML::PackageConfiguration>(impl, meta_ref); }

		static Uml::AssociationRole meta_srcPackageConfSpecializedConfig, meta_srcPackageConfSpecializedConfig_rev;
		Udm::AClassAssocAttr<PICML::PackageConfSpecializedConfig, PICML::PackageConfiguration> srcPackageConfSpecializedConfig() const { return Udm::AClassAssocAttr<PICML::PackageConfSpecializedConfig, PICML::PackageConfiguration>(impl, meta_srcPackageConfSpecializedConfig, meta_srcPackageConfSpecializedConfig_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::PackageConfSpecializedConfig, PICML::PackageConfiguration, Pred> srcPackageConfSpecializedConfig_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::PackageConfSpecializedConfig, PICML::PackageConfiguration, Pred>(impl, meta_srcPackageConfSpecializedConfig, meta_srcPackageConfSpecializedConfig_rev); }

		static Uml::CompositionParentRole meta_TopLevelPackageContainer_parent;
		Udm::ParentAttr<PICML::TopLevelPackageContainer> TopLevelPackageContainer_parent() const { return Udm::ParentAttr<PICML::TopLevelPackageContainer>(impl, meta_TopLevelPackageContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  PackageConfSelectRequirement :  public MgaObject {
	 public:
		static Uml::Class meta;

		PackageConfSelectRequirement() { }
		PackageConfSelectRequirement(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		PackageConfSelectRequirement(const PackageConfSelectRequirement &master) : MgaObject(master) { }
		static PackageConfSelectRequirement Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PackageConfSelectRequirement Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PackageConfSelectRequirement CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PackageConfSelectRequirement CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PackageConfSelectRequirement> Instances() { return Udm::InstantiatedAttr<PICML::PackageConfSelectRequirement>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PackageConfSelectRequirement, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PackageConfSelectRequirement, Pred>(impl);}

		Udm::DerivedAttr<PICML::PackageConfSelectRequirement> Derived() { return Udm::DerivedAttr<PICML::PackageConfSelectRequirement>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PackageConfSelectRequirement, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PackageConfSelectRequirement, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PackageConfSelectRequirement> Archetype() { return Udm::ArchetypeAttr<PICML::PackageConfSelectRequirement>(impl);}

		static Uml::CompositionParentRole meta_PackageConfigurationContainer_parent;
		Udm::ParentAttr<PICML::PackageConfigurationContainer> PackageConfigurationContainer_parent() const { return Udm::ParentAttr<PICML::PackageConfigurationContainer>(impl, meta_PackageConfigurationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcPackageConfSelectRequirement_end_;
		Udm::AssocEndAttr<PICML::PackageConfiguration> srcPackageConfSelectRequirement_end() const { return Udm::AssocEndAttr<PICML::PackageConfiguration>(impl, meta_srcPackageConfSelectRequirement_end_); }

		static Uml::AssociationRole meta_dstPackageConfSelectRequirement_end_;
		Udm::AssocEndAttr<PICML::Requirement> dstPackageConfSelectRequirement_end() const { return Udm::AssocEndAttr<PICML::Requirement>(impl, meta_dstPackageConfSelectRequirement_end_); }

	};

	class  PackageConfReference :  public MgaObject {
	 public:
		static Uml::Class meta;

		PackageConfReference() { }
		PackageConfReference(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		PackageConfReference(const PackageConfReference &master) : MgaObject(master) { }
		static PackageConfReference Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PackageConfReference Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PackageConfReference CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PackageConfReference CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PackageConfReference> Instances() { return Udm::InstantiatedAttr<PICML::PackageConfReference>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PackageConfReference, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PackageConfReference, Pred>(impl);}

		Udm::DerivedAttr<PICML::PackageConfReference> Derived() { return Udm::DerivedAttr<PICML::PackageConfReference>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PackageConfReference, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PackageConfReference, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PackageConfReference> Archetype() { return Udm::ArchetypeAttr<PICML::PackageConfReference>(impl);}

		static Uml::CompositionParentRole meta_PackageConfigurationContainer_parent;
		Udm::ParentAttr<PICML::PackageConfigurationContainer> PackageConfigurationContainer_parent() const { return Udm::ParentAttr<PICML::PackageConfigurationContainer>(impl, meta_PackageConfigurationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcPackageConfReference_end_;
		Udm::AssocEndAttr<PICML::PackageConfiguration> srcPackageConfReference_end() const { return Udm::AssocEndAttr<PICML::PackageConfiguration>(impl, meta_srcPackageConfReference_end_); }

		static Uml::AssociationRole meta_dstPackageConfReference_end_;
		Udm::AssocEndAttr<PICML::ComponentPackageReference> dstPackageConfReference_end() const { return Udm::AssocEndAttr<PICML::ComponentPackageReference>(impl, meta_dstPackageConfReference_end_); }

	};

	class  PackageConfiguration :  public MgaObject {
	 public:
		static Uml::Class meta;

		PackageConfiguration() { }
		PackageConfiguration(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		PackageConfiguration(const PackageConfiguration &master) : MgaObject(master) { }
		static PackageConfiguration Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PackageConfiguration Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PackageConfiguration CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PackageConfiguration CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PackageConfiguration> Instances() { return Udm::InstantiatedAttr<PICML::PackageConfiguration>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PackageConfiguration, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PackageConfiguration, Pred>(impl);}

		Udm::DerivedAttr<PICML::PackageConfiguration> Derived() { return Udm::DerivedAttr<PICML::PackageConfiguration>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PackageConfiguration, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PackageConfiguration, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PackageConfiguration> Archetype() { return Udm::ArchetypeAttr<PICML::PackageConfiguration>(impl);}

		static Uml::Attribute meta_UUID;
		Udm::StringAttr UUID() const { return Udm::StringAttr(impl, meta_UUID); }

		static Uml::Attribute meta_label;
		Udm::StringAttr label() const { return Udm::StringAttr(impl, meta_label); }

		static Uml::AssociationRole meta_referedbyPackageConfigurationReference;
		Udm::AssocAttr<PICML::PackageConfigurationReference> referedbyPackageConfigurationReference() const { return Udm::AssocAttr<PICML::PackageConfigurationReference>(impl, meta_referedbyPackageConfigurationReference); }
		template <class Pred> Udm::AssocAttr<PICML::PackageConfigurationReference, Pred > referedbyPackageConfigurationReference_sorted(const Pred &) const { return Udm::AssocAttr<PICML::PackageConfigurationReference, Pred>(impl, meta_referedbyPackageConfigurationReference); }

		static Uml::AssociationRole meta_dstPackageConfSpecializedConfig, meta_dstPackageConfSpecializedConfig_rev;
		Udm::AClassPointerAttr<PICML::PackageConfSpecializedConfig, PICML::PackageConfigurationReference> dstPackageConfSpecializedConfig() const { return Udm::AClassPointerAttr<PICML::PackageConfSpecializedConfig, PICML::PackageConfigurationReference>(impl, meta_dstPackageConfSpecializedConfig, meta_dstPackageConfSpecializedConfig_rev); }

		static Uml::AssociationRole meta_dstPackageConfReference, meta_dstPackageConfReference_rev;
		Udm::AClassPointerAttr<PICML::PackageConfReference, PICML::ComponentPackageReference> dstPackageConfReference() const { return Udm::AClassPointerAttr<PICML::PackageConfReference, PICML::ComponentPackageReference>(impl, meta_dstPackageConfReference, meta_dstPackageConfReference_rev); }

		static Uml::AssociationRole meta_dstPackageConfBasePackage, meta_dstPackageConfBasePackage_rev;
		Udm::AClassPointerAttr<PICML::PackageConfBasePackage, PICML::ComponentPackage> dstPackageConfBasePackage() const { return Udm::AClassPointerAttr<PICML::PackageConfBasePackage, PICML::ComponentPackage>(impl, meta_dstPackageConfBasePackage, meta_dstPackageConfBasePackage_rev); }

		static Uml::AssociationRole meta_dstPackageConfConfigProperty, meta_dstPackageConfConfigProperty_rev;
		Udm::AClassAssocAttr<PICML::PackageConfConfigProperty, PICML::Property> dstPackageConfConfigProperty() const { return Udm::AClassAssocAttr<PICML::PackageConfConfigProperty, PICML::Property>(impl, meta_dstPackageConfConfigProperty, meta_dstPackageConfConfigProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::PackageConfConfigProperty, PICML::Property, Pred> dstPackageConfConfigProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::PackageConfConfigProperty, PICML::Property, Pred>(impl, meta_dstPackageConfConfigProperty, meta_dstPackageConfConfigProperty_rev); }

		static Uml::AssociationRole meta_dstPackageConfSelectRequirement, meta_dstPackageConfSelectRequirement_rev;
		Udm::AClassAssocAttr<PICML::PackageConfSelectRequirement, PICML::Requirement> dstPackageConfSelectRequirement() const { return Udm::AClassAssocAttr<PICML::PackageConfSelectRequirement, PICML::Requirement>(impl, meta_dstPackageConfSelectRequirement, meta_dstPackageConfSelectRequirement_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::PackageConfSelectRequirement, PICML::Requirement, Pred> dstPackageConfSelectRequirement_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::PackageConfSelectRequirement, PICML::Requirement, Pred>(impl, meta_dstPackageConfSelectRequirement, meta_dstPackageConfSelectRequirement_rev); }

		static Uml::CompositionParentRole meta_PackageConfigurationContainer_parent;
		Udm::ParentAttr<PICML::PackageConfigurationContainer> PackageConfigurationContainer_parent() const { return Udm::ParentAttr<PICML::PackageConfigurationContainer>(impl, meta_PackageConfigurationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  PackageInterface :  public MgaObject {
	 public:
		static Uml::Class meta;

		PackageInterface() { }
		PackageInterface(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		PackageInterface(const PackageInterface &master) : MgaObject(master) { }
		static PackageInterface Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PackageInterface Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PackageInterface CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PackageInterface CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PackageInterface> Instances() { return Udm::InstantiatedAttr<PICML::PackageInterface>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PackageInterface, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PackageInterface, Pred>(impl);}

		Udm::DerivedAttr<PICML::PackageInterface> Derived() { return Udm::DerivedAttr<PICML::PackageInterface>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PackageInterface, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PackageInterface, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PackageInterface> Archetype() { return Udm::ArchetypeAttr<PICML::PackageInterface>(impl);}

		static Uml::CompositionParentRole meta_PackageContainer_parent;
		Udm::ParentAttr<PICML::PackageContainer> PackageContainer_parent() const { return Udm::ParentAttr<PICML::PackageContainer>(impl, meta_PackageContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstPackageInterface_end_;
		Udm::AssocEndAttr<PICML::ComponentType> dstPackageInterface_end() const { return Udm::AssocEndAttr<PICML::ComponentType>(impl, meta_dstPackageInterface_end_); }

		static Uml::AssociationRole meta_srcPackageInterface_end_;
		Udm::AssocEndAttr<PICML::ComponentPackage> srcPackageInterface_end() const { return Udm::AssocEndAttr<PICML::ComponentPackage>(impl, meta_srcPackageInterface_end_); }

	};

	class  PackageInfoProperty :  public MgaObject {
	 public:
		static Uml::Class meta;

		PackageInfoProperty() { }
		PackageInfoProperty(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		PackageInfoProperty(const PackageInfoProperty &master) : MgaObject(master) { }
		static PackageInfoProperty Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PackageInfoProperty Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PackageInfoProperty CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PackageInfoProperty CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PackageInfoProperty> Instances() { return Udm::InstantiatedAttr<PICML::PackageInfoProperty>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PackageInfoProperty, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PackageInfoProperty, Pred>(impl);}

		Udm::DerivedAttr<PICML::PackageInfoProperty> Derived() { return Udm::DerivedAttr<PICML::PackageInfoProperty>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PackageInfoProperty, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PackageInfoProperty, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PackageInfoProperty> Archetype() { return Udm::ArchetypeAttr<PICML::PackageInfoProperty>(impl);}

		static Uml::CompositionParentRole meta_PackageContainer_parent;
		Udm::ParentAttr<PICML::PackageContainer> PackageContainer_parent() const { return Udm::ParentAttr<PICML::PackageContainer>(impl, meta_PackageContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcPackageInfoProperty_end_;
		Udm::AssocEndAttr<PICML::ComponentPackage> srcPackageInfoProperty_end() const { return Udm::AssocEndAttr<PICML::ComponentPackage>(impl, meta_srcPackageInfoProperty_end_); }

		static Uml::AssociationRole meta_dstPackageInfoProperty_end_;
		Udm::AssocEndAttr<PICML::Property> dstPackageInfoProperty_end() const { return Udm::AssocEndAttr<PICML::Property>(impl, meta_dstPackageInfoProperty_end_); }

	};

	class  PackageConfigProperty :  public MgaObject {
	 public:
		static Uml::Class meta;

		PackageConfigProperty() { }
		PackageConfigProperty(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		PackageConfigProperty(const PackageConfigProperty &master) : MgaObject(master) { }
		static PackageConfigProperty Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PackageConfigProperty Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PackageConfigProperty CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PackageConfigProperty CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PackageConfigProperty> Instances() { return Udm::InstantiatedAttr<PICML::PackageConfigProperty>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PackageConfigProperty, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PackageConfigProperty, Pred>(impl);}

		Udm::DerivedAttr<PICML::PackageConfigProperty> Derived() { return Udm::DerivedAttr<PICML::PackageConfigProperty>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PackageConfigProperty, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PackageConfigProperty, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PackageConfigProperty> Archetype() { return Udm::ArchetypeAttr<PICML::PackageConfigProperty>(impl);}

		static Uml::CompositionParentRole meta_PackageContainer_parent;
		Udm::ParentAttr<PICML::PackageContainer> PackageContainer_parent() const { return Udm::ParentAttr<PICML::PackageContainer>(impl, meta_PackageContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcPackageConfigProperty_end_;
		Udm::AssocEndAttr<PICML::ComponentPackage> srcPackageConfigProperty_end() const { return Udm::AssocEndAttr<PICML::ComponentPackage>(impl, meta_srcPackageConfigProperty_end_); }

		static Uml::AssociationRole meta_dstPackageConfigProperty_end_;
		Udm::AssocEndAttr<PICML::Property> dstPackageConfigProperty_end() const { return Udm::AssocEndAttr<PICML::Property>(impl, meta_dstPackageConfigProperty_end_); }

	};

	class  PackageContainer :  public MgaObject {
	 public:
		static Uml::Class meta;

		PackageContainer() { }
		PackageContainer(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		PackageContainer(const PackageContainer &master) : MgaObject(master) { }
		static PackageContainer Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PackageContainer Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PackageContainer CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PackageContainer CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PackageContainer> Instances() { return Udm::InstantiatedAttr<PICML::PackageContainer>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PackageContainer, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PackageContainer, Pred>(impl);}

		Udm::DerivedAttr<PICML::PackageContainer> Derived() { return Udm::DerivedAttr<PICML::PackageContainer>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PackageContainer, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PackageContainer, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PackageContainer> Archetype() { return Udm::ArchetypeAttr<PICML::PackageContainer>(impl);}

		static Uml::CompositionChildRole meta_Implementation_children;
		Udm::ChildrenAttr<PICML::Implementation> Implementation_children() const { return Udm::ChildrenAttr<PICML::Implementation>(impl, meta_Implementation_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Implementation, Pred> Implementation_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Implementation, Pred>(impl, meta_Implementation_children); }

		static Uml::CompositionChildRole meta_PackageInterface_child;
		Udm::ChildAttr<PICML::PackageInterface> PackageInterface_child() const { return Udm::ChildAttr<PICML::PackageInterface>(impl, meta_PackageInterface_child); }

		static Uml::CompositionChildRole meta_PackageConfigProperty_children;
		Udm::ChildrenAttr<PICML::PackageConfigProperty> PackageConfigProperty_children() const { return Udm::ChildrenAttr<PICML::PackageConfigProperty>(impl, meta_PackageConfigProperty_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::PackageConfigProperty, Pred> PackageConfigProperty_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageConfigProperty, Pred>(impl, meta_PackageConfigProperty_children); }

		static Uml::CompositionChildRole meta_Property_children;
		Udm::ChildrenAttr<PICML::Property> Property_children() const { return Udm::ChildrenAttr<PICML::Property>(impl, meta_Property_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Property, Pred> Property_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Property, Pred>(impl, meta_Property_children); }

		static Uml::CompositionChildRole meta_ComponentType_children;
		Udm::ChildrenAttr<PICML::ComponentType> ComponentType_children() const { return Udm::ChildrenAttr<PICML::ComponentType>(impl, meta_ComponentType_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentType, Pred> ComponentType_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentType, Pred>(impl, meta_ComponentType_children); }

		static Uml::CompositionChildRole meta_ComponentImplementationReference_children;
		Udm::ChildrenAttr<PICML::ComponentImplementationReference> ComponentImplementationReference_children() const { return Udm::ChildrenAttr<PICML::ComponentImplementationReference>(impl, meta_ComponentImplementationReference_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentImplementationReference, Pred> ComponentImplementationReference_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentImplementationReference, Pred>(impl, meta_ComponentImplementationReference_children); }

		static Uml::CompositionChildRole meta_ComponentPackage_child;
		Udm::ChildAttr<PICML::ComponentPackage> ComponentPackage_child() const { return Udm::ChildAttr<PICML::ComponentPackage>(impl, meta_ComponentPackage_child); }

		static Uml::CompositionChildRole meta_PackageInfoProperty_children;
		Udm::ChildrenAttr<PICML::PackageInfoProperty> PackageInfoProperty_children() const { return Udm::ChildrenAttr<PICML::PackageInfoProperty>(impl, meta_PackageInfoProperty_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::PackageInfoProperty, Pred> PackageInfoProperty_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageInfoProperty, Pred>(impl, meta_PackageInfoProperty_children); }

		Udm::ChildrenAttr<PICML::ComponentType> ComponentType_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentType, Pred> ComponentType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentImplementationReference> ComponentImplementationReference_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentImplementationReference>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentImplementationReference, Pred> ComponentImplementationReference_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentImplementationReference, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::PackageInterface> PackageInterface_kind_children() const { return Udm::ChildrenAttr<PICML::PackageInterface>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PackageInterface, Pred> PackageInterface_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageInterface, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::PackageInfoProperty> PackageInfoProperty_kind_children() const { return Udm::ChildrenAttr<PICML::PackageInfoProperty>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PackageInfoProperty, Pred> PackageInfoProperty_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageInfoProperty, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::PackageConfigProperty> PackageConfigProperty_kind_children() const { return Udm::ChildrenAttr<PICML::PackageConfigProperty>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PackageConfigProperty, Pred> PackageConfigProperty_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PackageConfigProperty, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Implementation> Implementation_kind_children() const { return Udm::ChildrenAttr<PICML::Implementation>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Implementation, Pred> Implementation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Implementation, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentPackage> ComponentPackage_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentPackage>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentPackage, Pred> ComponentPackage_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentPackage, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Property> Property_kind_children() const { return Udm::ChildrenAttr<PICML::Property>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Property, Pred> Property_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Property, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_ComponentPackages_parent;
		Udm::ParentAttr<PICML::ComponentPackages> ComponentPackages_parent() const { return Udm::ParentAttr<PICML::ComponentPackages>(impl, meta_ComponentPackages_parent); }

		Udm::ParentAttr<PICML::ComponentPackages> parent() const { return Udm::ParentAttr<PICML::ComponentPackages>(impl, Udm::NULLPARENTROLE); }
	};

	class  Implementation :  public MgaObject {
	 public:
		static Uml::Class meta;

		Implementation() { }
		Implementation(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Implementation(const Implementation &master) : MgaObject(master) { }
		static Implementation Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Implementation Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Implementation CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Implementation CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Implementation> Instances() { return Udm::InstantiatedAttr<PICML::Implementation>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Implementation, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Implementation, Pred>(impl);}

		Udm::DerivedAttr<PICML::Implementation> Derived() { return Udm::DerivedAttr<PICML::Implementation>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Implementation, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Implementation, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Implementation> Archetype() { return Udm::ArchetypeAttr<PICML::Implementation>(impl);}

		static Uml::CompositionParentRole meta_PackageContainer_parent;
		Udm::ParentAttr<PICML::PackageContainer> PackageContainer_parent() const { return Udm::ParentAttr<PICML::PackageContainer>(impl, meta_PackageContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstImplementation_end_;
		Udm::AssocEndAttr<PICML::ComponentImplementationReference> dstImplementation_end() const { return Udm::AssocEndAttr<PICML::ComponentImplementationReference>(impl, meta_dstImplementation_end_); }

		static Uml::AssociationRole meta_srcImplementation_end_;
		Udm::AssocEndAttr<PICML::ComponentPackage> srcImplementation_end() const { return Udm::AssocEndAttr<PICML::ComponentPackage>(impl, meta_srcImplementation_end_); }

	};

	class  ComponentPackage :  public MgaObject {
	 public:
		static Uml::Class meta;

		ComponentPackage() { }
		ComponentPackage(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ComponentPackage(const ComponentPackage &master) : MgaObject(master) { }
		static ComponentPackage Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentPackage Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentPackage CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentPackage CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentPackage> Instances() { return Udm::InstantiatedAttr<PICML::ComponentPackage>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentPackage, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentPackage, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentPackage> Derived() { return Udm::DerivedAttr<PICML::ComponentPackage>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentPackage, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentPackage, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentPackage> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentPackage>(impl);}

		static Uml::Attribute meta_UUID;
		Udm::StringAttr UUID() const { return Udm::StringAttr(impl, meta_UUID); }

		static Uml::Attribute meta_label;
		Udm::StringAttr label() const { return Udm::StringAttr(impl, meta_label); }

		static Uml::AssociationRole meta_dstPackageInterface, meta_dstPackageInterface_rev;
		Udm::AClassPointerAttr<PICML::PackageInterface, PICML::ComponentType> dstPackageInterface() const { return Udm::AClassPointerAttr<PICML::PackageInterface, PICML::ComponentType>(impl, meta_dstPackageInterface, meta_dstPackageInterface_rev); }

		static Uml::AssociationRole meta_dstImplementation, meta_dstImplementation_rev;
		Udm::AClassAssocAttr<PICML::Implementation, PICML::ComponentImplementationReference> dstImplementation() const { return Udm::AClassAssocAttr<PICML::Implementation, PICML::ComponentImplementationReference>(impl, meta_dstImplementation, meta_dstImplementation_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::Implementation, PICML::ComponentImplementationReference, Pred> dstImplementation_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::Implementation, PICML::ComponentImplementationReference, Pred>(impl, meta_dstImplementation, meta_dstImplementation_rev); }

		static Uml::AssociationRole meta_srcPackageConfBasePackage, meta_srcPackageConfBasePackage_rev;
		Udm::AClassAssocAttr<PICML::PackageConfBasePackage, PICML::PackageConfiguration> srcPackageConfBasePackage() const { return Udm::AClassAssocAttr<PICML::PackageConfBasePackage, PICML::PackageConfiguration>(impl, meta_srcPackageConfBasePackage, meta_srcPackageConfBasePackage_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::PackageConfBasePackage, PICML::PackageConfiguration, Pred> srcPackageConfBasePackage_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::PackageConfBasePackage, PICML::PackageConfiguration, Pred>(impl, meta_srcPackageConfBasePackage, meta_srcPackageConfBasePackage_rev); }

		static Uml::AssociationRole meta_dstPackageInfoProperty, meta_dstPackageInfoProperty_rev;
		Udm::AClassAssocAttr<PICML::PackageInfoProperty, PICML::Property> dstPackageInfoProperty() const { return Udm::AClassAssocAttr<PICML::PackageInfoProperty, PICML::Property>(impl, meta_dstPackageInfoProperty, meta_dstPackageInfoProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::PackageInfoProperty, PICML::Property, Pred> dstPackageInfoProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::PackageInfoProperty, PICML::Property, Pred>(impl, meta_dstPackageInfoProperty, meta_dstPackageInfoProperty_rev); }

		static Uml::AssociationRole meta_dstPackageConfigProperty, meta_dstPackageConfigProperty_rev;
		Udm::AClassAssocAttr<PICML::PackageConfigProperty, PICML::Property> dstPackageConfigProperty() const { return Udm::AClassAssocAttr<PICML::PackageConfigProperty, PICML::Property>(impl, meta_dstPackageConfigProperty, meta_dstPackageConfigProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::PackageConfigProperty, PICML::Property, Pred> dstPackageConfigProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::PackageConfigProperty, PICML::Property, Pred>(impl, meta_dstPackageConfigProperty, meta_dstPackageConfigProperty_rev); }

		static Uml::AssociationRole meta_referedbyComponentPackageReference;
		Udm::AssocAttr<PICML::ComponentPackageReference> referedbyComponentPackageReference() const { return Udm::AssocAttr<PICML::ComponentPackageReference>(impl, meta_referedbyComponentPackageReference); }
		template <class Pred> Udm::AssocAttr<PICML::ComponentPackageReference, Pred > referedbyComponentPackageReference_sorted(const Pred &) const { return Udm::AssocAttr<PICML::ComponentPackageReference, Pred>(impl, meta_referedbyComponentPackageReference); }

		static Uml::CompositionParentRole meta_PackageConfigurationContainer_parent;
		Udm::ParentAttr<PICML::PackageConfigurationContainer> PackageConfigurationContainer_parent() const { return Udm::ParentAttr<PICML::PackageConfigurationContainer>(impl, meta_PackageConfigurationContainer_parent); }

		static Uml::CompositionParentRole meta_PackageContainer_parent;
		Udm::ParentAttr<PICML::PackageContainer> PackageContainer_parent() const { return Udm::ParentAttr<PICML::PackageContainer>(impl, meta_PackageContainer_parent); }

		static Uml::CompositionParentRole meta_ComponentAssembly_parent;
		Udm::ParentAttr<PICML::ComponentAssembly> ComponentAssembly_parent() const { return Udm::ParentAttr<PICML::ComponentAssembly>(impl, meta_ComponentAssembly_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  ComponentPackageReference :  public MgaObject {
	 public:
		static Uml::Class meta;

		ComponentPackageReference() { }
		ComponentPackageReference(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ComponentPackageReference(const ComponentPackageReference &master) : MgaObject(master) { }
		static ComponentPackageReference Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentPackageReference Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentPackageReference CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentPackageReference CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentPackageReference> Instances() { return Udm::InstantiatedAttr<PICML::ComponentPackageReference>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentPackageReference, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentPackageReference, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentPackageReference> Derived() { return Udm::DerivedAttr<PICML::ComponentPackageReference>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentPackageReference, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentPackageReference, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentPackageReference> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentPackageReference>(impl);}

		static Uml::Attribute meta_requiredName;
		Udm::StringAttr requiredName() const { return Udm::StringAttr(impl, meta_requiredName); }

		static Uml::Attribute meta_requiredUUID;
		Udm::StringAttr requiredUUID() const { return Udm::StringAttr(impl, meta_requiredUUID); }

		static Uml::Attribute meta_requiredType;
		Udm::StringAttr requiredType() const { return Udm::StringAttr(impl, meta_requiredType); }

		static Uml::AssociationRole meta_srcPackageConfReference, meta_srcPackageConfReference_rev;
		Udm::AClassAssocAttr<PICML::PackageConfReference, PICML::PackageConfiguration> srcPackageConfReference() const { return Udm::AClassAssocAttr<PICML::PackageConfReference, PICML::PackageConfiguration>(impl, meta_srcPackageConfReference, meta_srcPackageConfReference_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::PackageConfReference, PICML::PackageConfiguration, Pred> srcPackageConfReference_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::PackageConfReference, PICML::PackageConfiguration, Pred>(impl, meta_srcPackageConfReference, meta_srcPackageConfReference_rev); }

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::ComponentPackage> ref() const { return Udm::PointerAttr<PICML::ComponentPackage>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_PackageConfigurationContainer_parent;
		Udm::ParentAttr<PICML::PackageConfigurationContainer> PackageConfigurationContainer_parent() const { return Udm::ParentAttr<PICML::PackageConfigurationContainer>(impl, meta_PackageConfigurationContainer_parent); }

		static Uml::CompositionParentRole meta_ComponentAssembly_parent;
		Udm::ParentAttr<PICML::ComponentAssembly> ComponentAssembly_parent() const { return Udm::ParentAttr<PICML::ComponentAssembly>(impl, meta_ComponentAssembly_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Resource :  public MgaObject {
	 public:
		static Uml::Class meta;

		Resource() { }
		Resource(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Resource(const Resource &master) : MgaObject(master) { }
		static Resource Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Resource Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Resource CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Resource CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Resource> Instances() { return Udm::InstantiatedAttr<PICML::Resource>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Resource, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Resource, Pred>(impl);}

		Udm::DerivedAttr<PICML::Resource> Derived() { return Udm::DerivedAttr<PICML::Resource>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Resource, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Resource, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Resource> Archetype() { return Udm::ArchetypeAttr<PICML::Resource>(impl);}

		static Uml::Attribute meta_resourceType;
		Udm::StringAttr resourceType() const { return Udm::StringAttr(impl, meta_resourceType); }

		static Uml::CompositionChildRole meta_SatisfierProperty_children;
		Udm::ChildrenAttr<PICML::SatisfierProperty> SatisfierProperty_children() const { return Udm::ChildrenAttr<PICML::SatisfierProperty>(impl, meta_SatisfierProperty_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::SatisfierProperty, Pred> SatisfierProperty_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::SatisfierProperty, Pred>(impl, meta_SatisfierProperty_children); }

		Udm::ChildrenAttr<PICML::SatisfierProperty> SatisfierProperty_kind_children() const { return Udm::ChildrenAttr<PICML::SatisfierProperty>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::SatisfierProperty, Pred> SatisfierProperty_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::SatisfierProperty, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_Interconnect_parent;
		Udm::ParentAttr<PICML::Interconnect> Interconnect_parent() const { return Udm::ParentAttr<PICML::Interconnect>(impl, meta_Interconnect_parent); }

		static Uml::CompositionParentRole meta_Node_parent;
		Udm::ParentAttr<PICML::Node> Node_parent() const { return Udm::ParentAttr<PICML::Node>(impl, meta_Node_parent); }

		static Uml::CompositionParentRole meta_Bridge_parent;
		Udm::ParentAttr<PICML::Bridge> Bridge_parent() const { return Udm::ParentAttr<PICML::Bridge>(impl, meta_Bridge_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  NodeReference :  public MgaObject {
	 public:
		static Uml::Class meta;

		NodeReference() { }
		NodeReference(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		NodeReference(const NodeReference &master) : MgaObject(master) { }
		static NodeReference Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static NodeReference Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		NodeReference CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		NodeReference CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<NodeReference> Instances() { return Udm::InstantiatedAttr<PICML::NodeReference>(impl);}
		template <class Pred> Udm::InstantiatedAttr<NodeReference, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::NodeReference, Pred>(impl);}

		Udm::DerivedAttr<PICML::NodeReference> Derived() { return Udm::DerivedAttr<PICML::NodeReference>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::NodeReference, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::NodeReference, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::NodeReference> Archetype() { return Udm::ArchetypeAttr<PICML::NodeReference>(impl);}

		static Uml::AssociationRole meta_srcInstanceMapping, meta_srcInstanceMapping_rev;
		Udm::AClassAssocAttr<PICML::InstanceMapping, PICML::CollocationGroup> srcInstanceMapping() const { return Udm::AClassAssocAttr<PICML::InstanceMapping, PICML::CollocationGroup>(impl, meta_srcInstanceMapping, meta_srcInstanceMapping_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::InstanceMapping, PICML::CollocationGroup, Pred> srcInstanceMapping_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::InstanceMapping, PICML::CollocationGroup, Pred>(impl, meta_srcInstanceMapping, meta_srcInstanceMapping_rev); }

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::Node> ref() const { return Udm::PointerAttr<PICML::Node>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_DeploymentPlan_parent;
		Udm::ParentAttr<PICML::DeploymentPlan> DeploymentPlan_parent() const { return Udm::ParentAttr<PICML::DeploymentPlan>(impl, meta_DeploymentPlan_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  SatisfierProperty :  public MgaObject {
	 public:
		static Uml::Class meta;

		SatisfierProperty() { }
		SatisfierProperty(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		SatisfierProperty(const SatisfierProperty &master) : MgaObject(master) { }
		static SatisfierProperty Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static SatisfierProperty Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		SatisfierProperty CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		SatisfierProperty CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<SatisfierProperty> Instances() { return Udm::InstantiatedAttr<PICML::SatisfierProperty>(impl);}
		template <class Pred> Udm::InstantiatedAttr<SatisfierProperty, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::SatisfierProperty, Pred>(impl);}

		Udm::DerivedAttr<PICML::SatisfierProperty> Derived() { return Udm::DerivedAttr<PICML::SatisfierProperty>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::SatisfierProperty, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::SatisfierProperty, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::SatisfierProperty> Archetype() { return Udm::ArchetypeAttr<PICML::SatisfierProperty>(impl);}

		static Uml::Attribute meta_DataValue;
		Udm::StringAttr DataValue() const { return Udm::StringAttr(impl, meta_DataValue); }

		static Uml::Attribute meta_SatisfierPropertyKind;
		Udm::StringAttr SatisfierPropertyKind() const { return Udm::StringAttr(impl, meta_SatisfierPropertyKind); }

		static Uml::CompositionChildRole meta_DataType_child;
		Udm::ChildAttr<PICML::DataType> DataType_child() const { return Udm::ChildAttr<PICML::DataType>(impl, meta_DataType_child); }

		Udm::ChildrenAttr<PICML::DataType> DataType_kind_children() const { return Udm::ChildrenAttr<PICML::DataType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::DataType, Pred> DataType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::DataType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_Resource_parent;
		Udm::ParentAttr<PICML::Resource> Resource_parent() const { return Udm::ParentAttr<PICML::Resource>(impl, meta_Resource_parent); }

		static Uml::CompositionParentRole meta_SharedResource_parent;
		Udm::ParentAttr<PICML::SharedResource> SharedResource_parent() const { return Udm::ParentAttr<PICML::SharedResource>(impl, meta_SharedResource_parent); }

		static Uml::CompositionParentRole meta_RequirementSatisfier_parent;
		Udm::ParentAttr<PICML::RequirementSatisfier> RequirementSatisfier_parent() const { return Udm::ParentAttr<PICML::RequirementSatisfier>(impl, meta_RequirementSatisfier_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  DataType :  public MgaObject {
	 public:
		static Uml::Class meta;

		DataType() { }
		DataType(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		DataType(const DataType &master) : MgaObject(master) { }
		static DataType Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static DataType Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		DataType CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		DataType CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<DataType> Instances() { return Udm::InstantiatedAttr<PICML::DataType>(impl);}
		template <class Pred> Udm::InstantiatedAttr<DataType, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::DataType, Pred>(impl);}

		Udm::DerivedAttr<PICML::DataType> Derived() { return Udm::DerivedAttr<PICML::DataType>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::DataType, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::DataType, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::DataType> Archetype() { return Udm::ArchetypeAttr<PICML::DataType>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::PredefinedType> ref() const { return Udm::PointerAttr<PICML::PredefinedType>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_ComponentPropertyDescription_parent;
		Udm::ParentAttr<PICML::ComponentPropertyDescription> ComponentPropertyDescription_parent() const { return Udm::ParentAttr<PICML::ComponentPropertyDescription>(impl, meta_ComponentPropertyDescription_parent); }

		static Uml::CompositionParentRole meta_SatisfierProperty_parent;
		Udm::ParentAttr<PICML::SatisfierProperty> SatisfierProperty_parent() const { return Udm::ParentAttr<PICML::SatisfierProperty>(impl, meta_SatisfierProperty_parent); }

		static Uml::CompositionParentRole meta_Property_parent;
		Udm::ParentAttr<PICML::Property> Property_parent() const { return Udm::ParentAttr<PICML::Property>(impl, meta_Property_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  ImplementationDependency :  public MgaObject {
	 public:
		static Uml::Class meta;

		ImplementationDependency() { }
		ImplementationDependency(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ImplementationDependency(const ImplementationDependency &master) : MgaObject(master) { }
		static ImplementationDependency Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ImplementationDependency Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ImplementationDependency CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ImplementationDependency CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ImplementationDependency> Instances() { return Udm::InstantiatedAttr<PICML::ImplementationDependency>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ImplementationDependency, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ImplementationDependency, Pred>(impl);}

		Udm::DerivedAttr<PICML::ImplementationDependency> Derived() { return Udm::DerivedAttr<PICML::ImplementationDependency>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ImplementationDependency, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ImplementationDependency, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ImplementationDependency> Archetype() { return Udm::ArchetypeAttr<PICML::ImplementationDependency>(impl);}

		static Uml::Attribute meta_requiredType;
		Udm::StringAttr requiredType() const { return Udm::StringAttr(impl, meta_requiredType); }

		static Uml::AssociationRole meta_srcImplementationDependsOn, meta_srcImplementationDependsOn_rev;
		Udm::AClassAssocAttr<PICML::ImplementationDependsOn, PICML::ComponentImplementation> srcImplementationDependsOn() const { return Udm::AClassAssocAttr<PICML::ImplementationDependsOn, PICML::ComponentImplementation>(impl, meta_srcImplementationDependsOn, meta_srcImplementationDependsOn_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ImplementationDependsOn, PICML::ComponentImplementation, Pred> srcImplementationDependsOn_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ImplementationDependsOn, PICML::ComponentImplementation, Pred>(impl, meta_srcImplementationDependsOn, meta_srcImplementationDependsOn_rev); }

		static Uml::CompositionParentRole meta_ComponentImplementationContainer_parent;
		Udm::ParentAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_parent() const { return Udm::ParentAttr<PICML::ComponentImplementationContainer>(impl, meta_ComponentImplementationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Property :  public MgaObject {
	 public:
		static Uml::Class meta;

		Property() { }
		Property(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Property(const Property &master) : MgaObject(master) { }
		static Property Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Property Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Property CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Property CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Property> Instances() { return Udm::InstantiatedAttr<PICML::Property>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Property, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Property, Pred>(impl);}

		Udm::DerivedAttr<PICML::Property> Derived() { return Udm::DerivedAttr<PICML::Property>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Property, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Property, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Property> Archetype() { return Udm::ArchetypeAttr<PICML::Property>(impl);}

		static Uml::Attribute meta_DataValue;
		Udm::StringAttr DataValue() const { return Udm::StringAttr(impl, meta_DataValue); }

		static Uml::AssociationRole meta_srcComponentConfigProperty, meta_srcComponentConfigProperty_rev;
		Udm::AClassAssocAttr<PICML::ComponentConfigProperty, PICML::ComponentType> srcComponentConfigProperty() const { return Udm::AClassAssocAttr<PICML::ComponentConfigProperty, PICML::ComponentType>(impl, meta_srcComponentConfigProperty, meta_srcComponentConfigProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ComponentConfigProperty, PICML::ComponentType, Pred> srcComponentConfigProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ComponentConfigProperty, PICML::ComponentType, Pred>(impl, meta_srcComponentConfigProperty, meta_srcComponentConfigProperty_rev); }

		static Uml::AssociationRole meta_srcComponentInfoProperty, meta_srcComponentInfoProperty_rev;
		Udm::AClassAssocAttr<PICML::ComponentInfoProperty, PICML::ComponentType> srcComponentInfoProperty() const { return Udm::AClassAssocAttr<PICML::ComponentInfoProperty, PICML::ComponentType>(impl, meta_srcComponentInfoProperty, meta_srcComponentInfoProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ComponentInfoProperty, PICML::ComponentType, Pred> srcComponentInfoProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ComponentInfoProperty, PICML::ComponentType, Pred>(impl, meta_srcComponentInfoProperty, meta_srcComponentInfoProperty_rev); }

		static Uml::AssociationRole meta_srcInfoProperty, meta_srcInfoProperty_rev;
		Udm::AClassAssocAttr<PICML::InfoProperty, PICML::ComponentImplementation> srcInfoProperty() const { return Udm::AClassAssocAttr<PICML::InfoProperty, PICML::ComponentImplementation>(impl, meta_srcInfoProperty, meta_srcInfoProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::InfoProperty, PICML::ComponentImplementation, Pred> srcInfoProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::InfoProperty, PICML::ComponentImplementation, Pred>(impl, meta_srcInfoProperty, meta_srcInfoProperty_rev); }

		static Uml::AssociationRole meta_srcConfigProperty, meta_srcConfigProperty_rev;
		Udm::AClassAssocAttr<PICML::ConfigProperty, PICML::ComponentImplementation> srcConfigProperty() const { return Udm::AClassAssocAttr<PICML::ConfigProperty, PICML::ComponentImplementation>(impl, meta_srcConfigProperty, meta_srcConfigProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ConfigProperty, PICML::ComponentImplementation, Pred> srcConfigProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ConfigProperty, PICML::ComponentImplementation, Pred>(impl, meta_srcConfigProperty, meta_srcConfigProperty_rev); }

		static Uml::AssociationRole meta_srcMonolithExecParameter, meta_srcMonolithExecParameter_rev;
		Udm::AClassAssocAttr<PICML::MonolithExecParameter, PICML::MonolithicImplementation> srcMonolithExecParameter() const { return Udm::AClassAssocAttr<PICML::MonolithExecParameter, PICML::MonolithicImplementation>(impl, meta_srcMonolithExecParameter, meta_srcMonolithExecParameter_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::MonolithExecParameter, PICML::MonolithicImplementation, Pred> srcMonolithExecParameter_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::MonolithExecParameter, PICML::MonolithicImplementation, Pred>(impl, meta_srcMonolithExecParameter, meta_srcMonolithExecParameter_rev); }

		static Uml::AssociationRole meta_srcPackageConfConfigProperty, meta_srcPackageConfConfigProperty_rev;
		Udm::AClassAssocAttr<PICML::PackageConfConfigProperty, PICML::PackageConfiguration> srcPackageConfConfigProperty() const { return Udm::AClassAssocAttr<PICML::PackageConfConfigProperty, PICML::PackageConfiguration>(impl, meta_srcPackageConfConfigProperty, meta_srcPackageConfConfigProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::PackageConfConfigProperty, PICML::PackageConfiguration, Pred> srcPackageConfConfigProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::PackageConfConfigProperty, PICML::PackageConfiguration, Pred>(impl, meta_srcPackageConfConfigProperty, meta_srcPackageConfConfigProperty_rev); }

		static Uml::AssociationRole meta_srcPackageInfoProperty, meta_srcPackageInfoProperty_rev;
		Udm::AClassAssocAttr<PICML::PackageInfoProperty, PICML::ComponentPackage> srcPackageInfoProperty() const { return Udm::AClassAssocAttr<PICML::PackageInfoProperty, PICML::ComponentPackage>(impl, meta_srcPackageInfoProperty, meta_srcPackageInfoProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::PackageInfoProperty, PICML::ComponentPackage, Pred> srcPackageInfoProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::PackageInfoProperty, PICML::ComponentPackage, Pred>(impl, meta_srcPackageInfoProperty, meta_srcPackageInfoProperty_rev); }

		static Uml::AssociationRole meta_srcPackageConfigProperty, meta_srcPackageConfigProperty_rev;
		Udm::AClassAssocAttr<PICML::PackageConfigProperty, PICML::ComponentPackage> srcPackageConfigProperty() const { return Udm::AClassAssocAttr<PICML::PackageConfigProperty, PICML::ComponentPackage>(impl, meta_srcPackageConfigProperty, meta_srcPackageConfigProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::PackageConfigProperty, PICML::ComponentPackage, Pred> srcPackageConfigProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::PackageConfigProperty, PICML::ComponentPackage, Pred>(impl, meta_srcPackageConfigProperty, meta_srcPackageConfigProperty_rev); }

		static Uml::AssociationRole meta_srcArtifactExecParameter, meta_srcArtifactExecParameter_rev;
		Udm::AClassAssocAttr<PICML::ArtifactExecParameter, PICML::ImplementationArtifact> srcArtifactExecParameter() const { return Udm::AClassAssocAttr<PICML::ArtifactExecParameter, PICML::ImplementationArtifact>(impl, meta_srcArtifactExecParameter, meta_srcArtifactExecParameter_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ArtifactExecParameter, PICML::ImplementationArtifact, Pred> srcArtifactExecParameter_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ArtifactExecParameter, PICML::ImplementationArtifact, Pred>(impl, meta_srcArtifactExecParameter, meta_srcArtifactExecParameter_rev); }

		static Uml::AssociationRole meta_srcArtifactInfoProperty, meta_srcArtifactInfoProperty_rev;
		Udm::AClassAssocAttr<PICML::ArtifactInfoProperty, PICML::ImplementationArtifact> srcArtifactInfoProperty() const { return Udm::AClassAssocAttr<PICML::ArtifactInfoProperty, PICML::ImplementationArtifact>(impl, meta_srcArtifactInfoProperty, meta_srcArtifactInfoProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ArtifactInfoProperty, PICML::ImplementationArtifact, Pred> srcArtifactInfoProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ArtifactInfoProperty, PICML::ImplementationArtifact, Pred>(impl, meta_srcArtifactInfoProperty, meta_srcArtifactInfoProperty_rev); }

		static Uml::AssociationRole meta_srcAssemblyConfigProperty, meta_srcAssemblyConfigProperty_rev;
		Udm::AClassAssocAttr<PICML::AssemblyConfigProperty, PICML::Component> srcAssemblyConfigProperty() const { return Udm::AClassAssocAttr<PICML::AssemblyConfigProperty, PICML::Component>(impl, meta_srcAssemblyConfigProperty, meta_srcAssemblyConfigProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::AssemblyConfigProperty, PICML::Component, Pred> srcAssemblyConfigProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::AssemblyConfigProperty, PICML::Component, Pred>(impl, meta_srcAssemblyConfigProperty, meta_srcAssemblyConfigProperty_rev); }

		static Uml::CompositionChildRole meta_DataType_child;
		Udm::ChildAttr<PICML::DataType> DataType_child() const { return Udm::ChildAttr<PICML::DataType>(impl, meta_DataType_child); }

		Udm::ChildrenAttr<PICML::DataType> DataType_kind_children() const { return Udm::ChildrenAttr<PICML::DataType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::DataType, Pred> DataType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::DataType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_ComponentContainer_parent;
		Udm::ParentAttr<PICML::ComponentContainer> ComponentContainer_parent() const { return Udm::ParentAttr<PICML::ComponentContainer>(impl, meta_ComponentContainer_parent); }

		static Uml::CompositionParentRole meta_ComponentImplementationContainer_parent;
		Udm::ParentAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_parent() const { return Udm::ParentAttr<PICML::ComponentImplementationContainer>(impl, meta_ComponentImplementationContainer_parent); }

		static Uml::CompositionParentRole meta_PackageConfigurationContainer_parent;
		Udm::ParentAttr<PICML::PackageConfigurationContainer> PackageConfigurationContainer_parent() const { return Udm::ParentAttr<PICML::PackageConfigurationContainer>(impl, meta_PackageConfigurationContainer_parent); }

		static Uml::CompositionParentRole meta_PackageContainer_parent;
		Udm::ParentAttr<PICML::PackageContainer> PackageContainer_parent() const { return Udm::ParentAttr<PICML::PackageContainer>(impl, meta_PackageContainer_parent); }

		static Uml::CompositionParentRole meta_RequirementBase_parent;
		Udm::ParentAttr<PICML::RequirementBase> RequirementBase_parent() const { return Udm::ParentAttr<PICML::RequirementBase>(impl, meta_RequirementBase_parent); }

		static Uml::CompositionParentRole meta_Domain_parent;
		Udm::ParentAttr<PICML::Domain> Domain_parent() const { return Udm::ParentAttr<PICML::Domain>(impl, meta_Domain_parent); }

		static Uml::CompositionParentRole meta_ComponentAssembly_parent;
		Udm::ParentAttr<PICML::ComponentAssembly> ComponentAssembly_parent() const { return Udm::ParentAttr<PICML::ComponentAssembly>(impl, meta_ComponentAssembly_parent); }

		static Uml::CompositionParentRole meta_ArtifactContainer_parent;
		Udm::ParentAttr<PICML::ArtifactContainer> ArtifactContainer_parent() const { return Udm::ParentAttr<PICML::ArtifactContainer>(impl, meta_ArtifactContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  RequirementSatisfier :  public MgaObject {
	 public:
		static Uml::Class meta;

		RequirementSatisfier() { }
		RequirementSatisfier(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		RequirementSatisfier(const RequirementSatisfier &master) : MgaObject(master) { }
		static RequirementSatisfier Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static RequirementSatisfier Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		RequirementSatisfier CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		RequirementSatisfier CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<RequirementSatisfier> Instances() { return Udm::InstantiatedAttr<PICML::RequirementSatisfier>(impl);}
		template <class Pred> Udm::InstantiatedAttr<RequirementSatisfier, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::RequirementSatisfier, Pred>(impl);}

		Udm::DerivedAttr<PICML::RequirementSatisfier> Derived() { return Udm::DerivedAttr<PICML::RequirementSatisfier>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::RequirementSatisfier, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::RequirementSatisfier, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::RequirementSatisfier> Archetype() { return Udm::ArchetypeAttr<PICML::RequirementSatisfier>(impl);}

		static Uml::Attribute meta_resourceType;
		Udm::StringAttr resourceType() const { return Udm::StringAttr(impl, meta_resourceType); }

		static Uml::CompositionChildRole meta_SatisfierProperty_children;
		Udm::ChildrenAttr<PICML::SatisfierProperty> SatisfierProperty_children() const { return Udm::ChildrenAttr<PICML::SatisfierProperty>(impl, meta_SatisfierProperty_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::SatisfierProperty, Pred> SatisfierProperty_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::SatisfierProperty, Pred>(impl, meta_SatisfierProperty_children); }

		Udm::ChildrenAttr<PICML::SatisfierProperty> SatisfierProperty_kind_children() const { return Udm::ChildrenAttr<PICML::SatisfierProperty>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::SatisfierProperty, Pred> SatisfierProperty_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::SatisfierProperty, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class  Capability :  public RequirementSatisfier {
	 public:
		static Uml::Class meta;

		Capability() { }
		Capability(Udm::ObjectImpl *impl) : RequirementSatisfier(impl) { }
		Capability(const Capability &master) : RequirementSatisfier(master) { }
		static Capability Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Capability Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Capability CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Capability CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Capability> Instances() { return Udm::InstantiatedAttr<PICML::Capability>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Capability, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Capability, Pred>(impl);}

		Udm::DerivedAttr<PICML::Capability> Derived() { return Udm::DerivedAttr<PICML::Capability>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Capability, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Capability, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Capability> Archetype() { return Udm::ArchetypeAttr<PICML::Capability>(impl);}

		static Uml::AssociationRole meta_srcImplementationCapability, meta_srcImplementationCapability_rev;
		Udm::AClassAssocAttr<PICML::ImplementationCapability, PICML::ComponentImplementation> srcImplementationCapability() const { return Udm::AClassAssocAttr<PICML::ImplementationCapability, PICML::ComponentImplementation>(impl, meta_srcImplementationCapability, meta_srcImplementationCapability_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ImplementationCapability, PICML::ComponentImplementation, Pred> srcImplementationCapability_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ImplementationCapability, PICML::ComponentImplementation, Pred>(impl, meta_srcImplementationCapability, meta_srcImplementationCapability_rev); }

		static Uml::CompositionParentRole meta_ComponentImplementationContainer_parent;
		Udm::ParentAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_parent() const { return Udm::ParentAttr<PICML::ComponentImplementationContainer>(impl, meta_ComponentImplementationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  RequirementBase :  public MgaObject {
	 public:
		static Uml::Class meta;

		RequirementBase() { }
		RequirementBase(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		RequirementBase(const RequirementBase &master) : MgaObject(master) { }
		static RequirementBase Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static RequirementBase Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		RequirementBase CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		RequirementBase CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<RequirementBase> Instances() { return Udm::InstantiatedAttr<PICML::RequirementBase>(impl);}
		template <class Pred> Udm::InstantiatedAttr<RequirementBase, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::RequirementBase, Pred>(impl);}

		Udm::DerivedAttr<PICML::RequirementBase> Derived() { return Udm::DerivedAttr<PICML::RequirementBase>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::RequirementBase, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::RequirementBase, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::RequirementBase> Archetype() { return Udm::ArchetypeAttr<PICML::RequirementBase>(impl);}

		static Uml::Attribute meta_resourceType;
		Udm::StringAttr resourceType() const { return Udm::StringAttr(impl, meta_resourceType); }

		static Uml::CompositionChildRole meta_Property_children;
		Udm::ChildrenAttr<PICML::Property> Property_children() const { return Udm::ChildrenAttr<PICML::Property>(impl, meta_Property_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Property, Pred> Property_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Property, Pred>(impl, meta_Property_children); }

		Udm::ChildrenAttr<PICML::Property> Property_kind_children() const { return Udm::ChildrenAttr<PICML::Property>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Property, Pred> Property_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Property, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class  Requirement :  public RequirementBase {
	 public:
		static Uml::Class meta;

		Requirement() { }
		Requirement(Udm::ObjectImpl *impl) : RequirementBase(impl) { }
		Requirement(const Requirement &master) : RequirementBase(master) { }
		static Requirement Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Requirement Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Requirement CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Requirement CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Requirement> Instances() { return Udm::InstantiatedAttr<PICML::Requirement>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Requirement, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Requirement, Pred>(impl);}

		Udm::DerivedAttr<PICML::Requirement> Derived() { return Udm::DerivedAttr<PICML::Requirement>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Requirement, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Requirement, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Requirement> Archetype() { return Udm::ArchetypeAttr<PICML::Requirement>(impl);}

		static Uml::AssociationRole meta_srcPackageConfSelectRequirement, meta_srcPackageConfSelectRequirement_rev;
		Udm::AClassAssocAttr<PICML::PackageConfSelectRequirement, PICML::PackageConfiguration> srcPackageConfSelectRequirement() const { return Udm::AClassAssocAttr<PICML::PackageConfSelectRequirement, PICML::PackageConfiguration>(impl, meta_srcPackageConfSelectRequirement, meta_srcPackageConfSelectRequirement_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::PackageConfSelectRequirement, PICML::PackageConfiguration, Pred> srcPackageConfSelectRequirement_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::PackageConfSelectRequirement, PICML::PackageConfiguration, Pred>(impl, meta_srcPackageConfSelectRequirement, meta_srcPackageConfSelectRequirement_rev); }

		static Uml::AssociationRole meta_srcAssemblyDeployRequirement, meta_srcAssemblyDeployRequirement_rev;
		Udm::AClassAssocAttr<PICML::AssemblyDeployRequirement, PICML::PublishConnector> srcAssemblyDeployRequirement() const { return Udm::AClassAssocAttr<PICML::AssemblyDeployRequirement, PICML::PublishConnector>(impl, meta_srcAssemblyDeployRequirement, meta_srcAssemblyDeployRequirement_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::AssemblyDeployRequirement, PICML::PublishConnector, Pred> srcAssemblyDeployRequirement_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::AssemblyDeployRequirement, PICML::PublishConnector, Pred>(impl, meta_srcAssemblyDeployRequirement, meta_srcAssemblyDeployRequirement_rev); }

		static Uml::AssociationRole meta_srcArtifactDeployRequirement, meta_srcArtifactDeployRequirement_rev;
		Udm::AClassAssocAttr<PICML::ArtifactDeployRequirement, PICML::ImplementationArtifact> srcArtifactDeployRequirement() const { return Udm::AClassAssocAttr<PICML::ArtifactDeployRequirement, PICML::ImplementationArtifact>(impl, meta_srcArtifactDeployRequirement, meta_srcArtifactDeployRequirement_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ArtifactDeployRequirement, PICML::ImplementationArtifact, Pred> srcArtifactDeployRequirement_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ArtifactDeployRequirement, PICML::ImplementationArtifact, Pred>(impl, meta_srcArtifactDeployRequirement, meta_srcArtifactDeployRequirement_rev); }

		static Uml::AssociationRole meta_srcAssemblyselectRequirement, meta_srcAssemblyselectRequirement_rev;
		Udm::AClassAssocAttr<PICML::AssemblyselectRequirement, PICML::Component> srcAssemblyselectRequirement() const { return Udm::AClassAssocAttr<PICML::AssemblyselectRequirement, PICML::Component>(impl, meta_srcAssemblyselectRequirement, meta_srcAssemblyselectRequirement_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::AssemblyselectRequirement, PICML::Component, Pred> srcAssemblyselectRequirement_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::AssemblyselectRequirement, PICML::Component, Pred>(impl, meta_srcAssemblyselectRequirement, meta_srcAssemblyselectRequirement_rev); }

		static Uml::CompositionParentRole meta_PackageConfigurationContainer_parent;
		Udm::ParentAttr<PICML::PackageConfigurationContainer> PackageConfigurationContainer_parent() const { return Udm::ParentAttr<PICML::PackageConfigurationContainer>(impl, meta_PackageConfigurationContainer_parent); }

		static Uml::CompositionParentRole meta_ComponentAssembly_parent;
		Udm::ParentAttr<PICML::ComponentAssembly> ComponentAssembly_parent() const { return Udm::ParentAttr<PICML::ComponentAssembly>(impl, meta_ComponentAssembly_parent); }

		static Uml::CompositionParentRole meta_ArtifactContainer_parent;
		Udm::ParentAttr<PICML::ArtifactContainer> ArtifactContainer_parent() const { return Udm::ParentAttr<PICML::ArtifactContainer>(impl, meta_ArtifactContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  ImplementationRequirement :  public RequirementBase {
	 public:
		static Uml::Class meta;

		ImplementationRequirement() { }
		ImplementationRequirement(Udm::ObjectImpl *impl) : RequirementBase(impl) { }
		ImplementationRequirement(const ImplementationRequirement &master) : RequirementBase(master) { }
		static ImplementationRequirement Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ImplementationRequirement Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ImplementationRequirement CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ImplementationRequirement CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ImplementationRequirement> Instances() { return Udm::InstantiatedAttr<PICML::ImplementationRequirement>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ImplementationRequirement, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ImplementationRequirement, Pred>(impl);}

		Udm::DerivedAttr<PICML::ImplementationRequirement> Derived() { return Udm::DerivedAttr<PICML::ImplementationRequirement>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ImplementationRequirement, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ImplementationRequirement, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ImplementationRequirement> Archetype() { return Udm::ArchetypeAttr<PICML::ImplementationRequirement>(impl);}

		static Uml::Attribute meta_ResourceUsageKind;
		Udm::StringAttr ResourceUsageKind() const { return Udm::StringAttr(impl, meta_ResourceUsageKind); }

		static Uml::Attribute meta_componentPort;
		Udm::StringAttr componentPort() const { return Udm::StringAttr(impl, meta_componentPort); }

		static Uml::Attribute meta_resourcePort;
		Udm::StringAttr resourcePort() const { return Udm::StringAttr(impl, meta_resourcePort); }

		static Uml::AssociationRole meta_srcMonolithDeployRequirement, meta_srcMonolithDeployRequirement_rev;
		Udm::AClassAssocAttr<PICML::MonolithDeployRequirement, PICML::MonolithicImplementation> srcMonolithDeployRequirement() const { return Udm::AClassAssocAttr<PICML::MonolithDeployRequirement, PICML::MonolithicImplementation>(impl, meta_srcMonolithDeployRequirement, meta_srcMonolithDeployRequirement_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::MonolithDeployRequirement, PICML::MonolithicImplementation, Pred> srcMonolithDeployRequirement_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::MonolithDeployRequirement, PICML::MonolithicImplementation, Pred>(impl, meta_srcMonolithDeployRequirement, meta_srcMonolithDeployRequirement_rev); }

		static Uml::CompositionParentRole meta_ComponentImplementationContainer_parent;
		Udm::ParentAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_parent() const { return Udm::ParentAttr<PICML::ComponentImplementationContainer>(impl, meta_ComponentImplementationContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Elements :  public MgaObject {
	 public:
		static Uml::Class meta;

		Elements() { }
		Elements(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Elements(const Elements &master) : MgaObject(master) { }
		static Elements Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Elements Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Elements CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Elements CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Elements> Instances() { return Udm::InstantiatedAttr<PICML::Elements>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Elements, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Elements, Pred>(impl);}

		Udm::DerivedAttr<PICML::Elements> Derived() { return Udm::DerivedAttr<PICML::Elements>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Elements, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Elements, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Elements> Archetype() { return Udm::ArchetypeAttr<PICML::Elements>(impl);}

		static Uml::CompositionParentRole meta_Domain_parent;
		Udm::ParentAttr<PICML::Domain> Domain_parent() const { return Udm::ParentAttr<PICML::Domain>(impl, meta_Domain_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Node :  public Elements {
	 public:
		static Uml::Class meta;

		Node() { }
		Node(Udm::ObjectImpl *impl) : Elements(impl) { }
		Node(const Node &master) : Elements(master) { }
		static Node Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Node Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Node CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Node CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Node> Instances() { return Udm::InstantiatedAttr<PICML::Node>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Node, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Node, Pred>(impl);}

		Udm::DerivedAttr<PICML::Node> Derived() { return Udm::DerivedAttr<PICML::Node>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Node, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Node, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Node> Archetype() { return Udm::ArchetypeAttr<PICML::Node>(impl);}

		static Uml::Attribute meta_label;
		Udm::StringAttr label() const { return Udm::StringAttr(impl, meta_label); }

		static Uml::AssociationRole meta_dstNode2Interconnect, meta_dstNode2Interconnect_rev;
		Udm::AClassAssocAttr<PICML::Node2Interconnect, PICML::Interconnect> dstNode2Interconnect() const { return Udm::AClassAssocAttr<PICML::Node2Interconnect, PICML::Interconnect>(impl, meta_dstNode2Interconnect, meta_dstNode2Interconnect_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::Node2Interconnect, PICML::Interconnect, Pred> dstNode2Interconnect_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::Node2Interconnect, PICML::Interconnect, Pred>(impl, meta_dstNode2Interconnect, meta_dstNode2Interconnect_rev); }

		static Uml::AssociationRole meta_dstShares, meta_dstShares_rev;
		Udm::AClassAssocAttr<PICML::Shares, PICML::SharedResource> dstShares() const { return Udm::AClassAssocAttr<PICML::Shares, PICML::SharedResource>(impl, meta_dstShares, meta_dstShares_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::Shares, PICML::SharedResource, Pred> dstShares_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::Shares, PICML::SharedResource, Pred>(impl, meta_dstShares, meta_dstShares_rev); }

		static Uml::AssociationRole meta_srcInterconnect2Node, meta_srcInterconnect2Node_rev;
		Udm::AClassAssocAttr<PICML::Interconnect2Node, PICML::Interconnect> srcInterconnect2Node() const { return Udm::AClassAssocAttr<PICML::Interconnect2Node, PICML::Interconnect>(impl, meta_srcInterconnect2Node, meta_srcInterconnect2Node_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::Interconnect2Node, PICML::Interconnect, Pred> srcInterconnect2Node_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::Interconnect2Node, PICML::Interconnect, Pred>(impl, meta_srcInterconnect2Node, meta_srcInterconnect2Node_rev); }

		static Uml::AssociationRole meta_referedbyNodeReference;
		Udm::AssocAttr<PICML::NodeReference> referedbyNodeReference() const { return Udm::AssocAttr<PICML::NodeReference>(impl, meta_referedbyNodeReference); }
		template <class Pred> Udm::AssocAttr<PICML::NodeReference, Pred > referedbyNodeReference_sorted(const Pred &) const { return Udm::AssocAttr<PICML::NodeReference, Pred>(impl, meta_referedbyNodeReference); }

		static Uml::CompositionChildRole meta_Resource_children;
		Udm::ChildrenAttr<PICML::Resource> Resource_children() const { return Udm::ChildrenAttr<PICML::Resource>(impl, meta_Resource_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Resource, Pred> Resource_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Resource, Pred>(impl, meta_Resource_children); }

		Udm::ChildrenAttr<PICML::Resource> Resource_kind_children() const { return Udm::ChildrenAttr<PICML::Resource>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Resource, Pred> Resource_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Resource, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  SharedResource :  public Elements {
	 public:
		static Uml::Class meta;

		SharedResource() { }
		SharedResource(Udm::ObjectImpl *impl) : Elements(impl) { }
		SharedResource(const SharedResource &master) : Elements(master) { }
		static SharedResource Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static SharedResource Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		SharedResource CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		SharedResource CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<SharedResource> Instances() { return Udm::InstantiatedAttr<PICML::SharedResource>(impl);}
		template <class Pred> Udm::InstantiatedAttr<SharedResource, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::SharedResource, Pred>(impl);}

		Udm::DerivedAttr<PICML::SharedResource> Derived() { return Udm::DerivedAttr<PICML::SharedResource>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::SharedResource, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::SharedResource, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::SharedResource> Archetype() { return Udm::ArchetypeAttr<PICML::SharedResource>(impl);}

		static Uml::Attribute meta_resourceType;
		Udm::StringAttr resourceType() const { return Udm::StringAttr(impl, meta_resourceType); }

		static Uml::AssociationRole meta_srcShares, meta_srcShares_rev;
		Udm::AClassAssocAttr<PICML::Shares, PICML::Node> srcShares() const { return Udm::AClassAssocAttr<PICML::Shares, PICML::Node>(impl, meta_srcShares, meta_srcShares_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::Shares, PICML::Node, Pred> srcShares_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::Shares, PICML::Node, Pred>(impl, meta_srcShares, meta_srcShares_rev); }

		static Uml::CompositionChildRole meta_SatisfierProperty_children;
		Udm::ChildrenAttr<PICML::SatisfierProperty> SatisfierProperty_children() const { return Udm::ChildrenAttr<PICML::SatisfierProperty>(impl, meta_SatisfierProperty_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::SatisfierProperty, Pred> SatisfierProperty_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::SatisfierProperty, Pred>(impl, meta_SatisfierProperty_children); }

		Udm::ChildrenAttr<PICML::SatisfierProperty> SatisfierProperty_kind_children() const { return Udm::ChildrenAttr<PICML::SatisfierProperty>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::SatisfierProperty, Pred> SatisfierProperty_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::SatisfierProperty, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Interconnect :  public Elements {
	 public:
		static Uml::Class meta;

		Interconnect() { }
		Interconnect(Udm::ObjectImpl *impl) : Elements(impl) { }
		Interconnect(const Interconnect &master) : Elements(master) { }
		static Interconnect Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Interconnect Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Interconnect CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Interconnect CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Interconnect> Instances() { return Udm::InstantiatedAttr<PICML::Interconnect>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Interconnect, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Interconnect, Pred>(impl);}

		Udm::DerivedAttr<PICML::Interconnect> Derived() { return Udm::DerivedAttr<PICML::Interconnect>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Interconnect, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Interconnect, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Interconnect> Archetype() { return Udm::ArchetypeAttr<PICML::Interconnect>(impl);}

		static Uml::Attribute meta_label;
		Udm::StringAttr label() const { return Udm::StringAttr(impl, meta_label); }

		static Uml::AssociationRole meta_srcNode2Interconnect, meta_srcNode2Interconnect_rev;
		Udm::AClassAssocAttr<PICML::Node2Interconnect, PICML::Node> srcNode2Interconnect() const { return Udm::AClassAssocAttr<PICML::Node2Interconnect, PICML::Node>(impl, meta_srcNode2Interconnect, meta_srcNode2Interconnect_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::Node2Interconnect, PICML::Node, Pred> srcNode2Interconnect_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::Node2Interconnect, PICML::Node, Pred>(impl, meta_srcNode2Interconnect, meta_srcNode2Interconnect_rev); }

		static Uml::AssociationRole meta_dstInterconnect2Node, meta_dstInterconnect2Node_rev;
		Udm::AClassAssocAttr<PICML::Interconnect2Node, PICML::Node> dstInterconnect2Node() const { return Udm::AClassAssocAttr<PICML::Interconnect2Node, PICML::Node>(impl, meta_dstInterconnect2Node, meta_dstInterconnect2Node_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::Interconnect2Node, PICML::Node, Pred> dstInterconnect2Node_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::Interconnect2Node, PICML::Node, Pred>(impl, meta_dstInterconnect2Node, meta_dstInterconnect2Node_rev); }

		static Uml::AssociationRole meta_dstInterconnect2Bridge, meta_dstInterconnect2Bridge_rev;
		Udm::AClassAssocAttr<PICML::Interconnect2Bridge, PICML::Bridge> dstInterconnect2Bridge() const { return Udm::AClassAssocAttr<PICML::Interconnect2Bridge, PICML::Bridge>(impl, meta_dstInterconnect2Bridge, meta_dstInterconnect2Bridge_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::Interconnect2Bridge, PICML::Bridge, Pred> dstInterconnect2Bridge_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::Interconnect2Bridge, PICML::Bridge, Pred>(impl, meta_dstInterconnect2Bridge, meta_dstInterconnect2Bridge_rev); }

		static Uml::AssociationRole meta_srcBridge2Interconnect, meta_srcBridge2Interconnect_rev;
		Udm::AClassAssocAttr<PICML::Bridge2Interconnect, PICML::Bridge> srcBridge2Interconnect() const { return Udm::AClassAssocAttr<PICML::Bridge2Interconnect, PICML::Bridge>(impl, meta_srcBridge2Interconnect, meta_srcBridge2Interconnect_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::Bridge2Interconnect, PICML::Bridge, Pred> srcBridge2Interconnect_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::Bridge2Interconnect, PICML::Bridge, Pred>(impl, meta_srcBridge2Interconnect, meta_srcBridge2Interconnect_rev); }

		static Uml::CompositionChildRole meta_Resource_children;
		Udm::ChildrenAttr<PICML::Resource> Resource_children() const { return Udm::ChildrenAttr<PICML::Resource>(impl, meta_Resource_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Resource, Pred> Resource_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Resource, Pred>(impl, meta_Resource_children); }

		Udm::ChildrenAttr<PICML::Resource> Resource_kind_children() const { return Udm::ChildrenAttr<PICML::Resource>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Resource, Pred> Resource_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Resource, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Bridge :  public Elements {
	 public:
		static Uml::Class meta;

		Bridge() { }
		Bridge(Udm::ObjectImpl *impl) : Elements(impl) { }
		Bridge(const Bridge &master) : Elements(master) { }
		static Bridge Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Bridge Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Bridge CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Bridge CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Bridge> Instances() { return Udm::InstantiatedAttr<PICML::Bridge>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Bridge, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Bridge, Pred>(impl);}

		Udm::DerivedAttr<PICML::Bridge> Derived() { return Udm::DerivedAttr<PICML::Bridge>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Bridge, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Bridge, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Bridge> Archetype() { return Udm::ArchetypeAttr<PICML::Bridge>(impl);}

		static Uml::Attribute meta_label;
		Udm::StringAttr label() const { return Udm::StringAttr(impl, meta_label); }

		static Uml::AssociationRole meta_srcInterconnect2Bridge, meta_srcInterconnect2Bridge_rev;
		Udm::AClassAssocAttr<PICML::Interconnect2Bridge, PICML::Interconnect> srcInterconnect2Bridge() const { return Udm::AClassAssocAttr<PICML::Interconnect2Bridge, PICML::Interconnect>(impl, meta_srcInterconnect2Bridge, meta_srcInterconnect2Bridge_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::Interconnect2Bridge, PICML::Interconnect, Pred> srcInterconnect2Bridge_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::Interconnect2Bridge, PICML::Interconnect, Pred>(impl, meta_srcInterconnect2Bridge, meta_srcInterconnect2Bridge_rev); }

		static Uml::AssociationRole meta_dstBridge2Interconnect, meta_dstBridge2Interconnect_rev;
		Udm::AClassAssocAttr<PICML::Bridge2Interconnect, PICML::Interconnect> dstBridge2Interconnect() const { return Udm::AClassAssocAttr<PICML::Bridge2Interconnect, PICML::Interconnect>(impl, meta_dstBridge2Interconnect, meta_dstBridge2Interconnect_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::Bridge2Interconnect, PICML::Interconnect, Pred> dstBridge2Interconnect_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::Bridge2Interconnect, PICML::Interconnect, Pred>(impl, meta_dstBridge2Interconnect, meta_dstBridge2Interconnect_rev); }

		static Uml::CompositionChildRole meta_Resource_children;
		Udm::ChildrenAttr<PICML::Resource> Resource_children() const { return Udm::ChildrenAttr<PICML::Resource>(impl, meta_Resource_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Resource, Pred> Resource_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Resource, Pred>(impl, meta_Resource_children); }

		Udm::ChildrenAttr<PICML::Resource> Resource_kind_children() const { return Udm::ChildrenAttr<PICML::Resource>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Resource, Pred> Resource_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Resource, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Domain :  public MgaObject {
	 public:
		static Uml::Class meta;

		Domain() { }
		Domain(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Domain(const Domain &master) : MgaObject(master) { }
		static Domain Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Domain Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Domain CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Domain CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Domain> Instances() { return Udm::InstantiatedAttr<PICML::Domain>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Domain, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Domain, Pred>(impl);}

		Udm::DerivedAttr<PICML::Domain> Derived() { return Udm::DerivedAttr<PICML::Domain>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Domain, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Domain, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Domain> Archetype() { return Udm::ArchetypeAttr<PICML::Domain>(impl);}

		static Uml::Attribute meta_label;
		Udm::StringAttr label() const { return Udm::StringAttr(impl, meta_label); }

		static Uml::Attribute meta_UUID;
		Udm::StringAttr UUID() const { return Udm::StringAttr(impl, meta_UUID); }

		static Uml::CompositionChildRole meta_Node2Interconnect_children;
		Udm::ChildrenAttr<PICML::Node2Interconnect> Node2Interconnect_children() const { return Udm::ChildrenAttr<PICML::Node2Interconnect>(impl, meta_Node2Interconnect_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Node2Interconnect, Pred> Node2Interconnect_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Node2Interconnect, Pred>(impl, meta_Node2Interconnect_children); }

		static Uml::CompositionChildRole meta_Interconnect2Bridge_children;
		Udm::ChildrenAttr<PICML::Interconnect2Bridge> Interconnect2Bridge_children() const { return Udm::ChildrenAttr<PICML::Interconnect2Bridge>(impl, meta_Interconnect2Bridge_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Interconnect2Bridge, Pred> Interconnect2Bridge_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Interconnect2Bridge, Pred>(impl, meta_Interconnect2Bridge_children); }

		static Uml::CompositionChildRole meta_Bridge2Interconnect_children;
		Udm::ChildrenAttr<PICML::Bridge2Interconnect> Bridge2Interconnect_children() const { return Udm::ChildrenAttr<PICML::Bridge2Interconnect>(impl, meta_Bridge2Interconnect_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Bridge2Interconnect, Pred> Bridge2Interconnect_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Bridge2Interconnect, Pred>(impl, meta_Bridge2Interconnect_children); }

		static Uml::CompositionChildRole meta_Property_children;
		Udm::ChildrenAttr<PICML::Property> Property_children() const { return Udm::ChildrenAttr<PICML::Property>(impl, meta_Property_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Property, Pred> Property_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Property, Pred>(impl, meta_Property_children); }

		static Uml::CompositionChildRole meta_Shares_children;
		Udm::ChildrenAttr<PICML::Shares> Shares_children() const { return Udm::ChildrenAttr<PICML::Shares>(impl, meta_Shares_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Shares, Pred> Shares_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Shares, Pred>(impl, meta_Shares_children); }

		static Uml::CompositionChildRole meta_Interconnect2Node_children;
		Udm::ChildrenAttr<PICML::Interconnect2Node> Interconnect2Node_children() const { return Udm::ChildrenAttr<PICML::Interconnect2Node>(impl, meta_Interconnect2Node_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Interconnect2Node, Pred> Interconnect2Node_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Interconnect2Node, Pred>(impl, meta_Interconnect2Node_children); }

		static Uml::CompositionChildRole meta_Elements_children;
		Udm::ChildrenAttr<PICML::Elements> Elements_children() const { return Udm::ChildrenAttr<PICML::Elements>(impl, meta_Elements_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Elements, Pred> Elements_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Elements, Pred>(impl, meta_Elements_children); }

		Udm::ChildrenAttr<PICML::Node> Node_kind_children() const { return Udm::ChildrenAttr<PICML::Node>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Node, Pred> Node_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Node, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::SharedResource> SharedResource_kind_children() const { return Udm::ChildrenAttr<PICML::SharedResource>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::SharedResource, Pred> SharedResource_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::SharedResource, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Interconnect> Interconnect_kind_children() const { return Udm::ChildrenAttr<PICML::Interconnect>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Interconnect, Pred> Interconnect_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Interconnect, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Bridge> Bridge_kind_children() const { return Udm::ChildrenAttr<PICML::Bridge>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Bridge, Pred> Bridge_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Bridge, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Property> Property_kind_children() const { return Udm::ChildrenAttr<PICML::Property>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Property, Pred> Property_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Property, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Elements> Elements_kind_children() const { return Udm::ChildrenAttr<PICML::Elements>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Elements, Pred> Elements_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Elements, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Interconnect2Node> Interconnect2Node_kind_children() const { return Udm::ChildrenAttr<PICML::Interconnect2Node>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Interconnect2Node, Pred> Interconnect2Node_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Interconnect2Node, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Bridge2Interconnect> Bridge2Interconnect_kind_children() const { return Udm::ChildrenAttr<PICML::Bridge2Interconnect>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Bridge2Interconnect, Pred> Bridge2Interconnect_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Bridge2Interconnect, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Shares> Shares_kind_children() const { return Udm::ChildrenAttr<PICML::Shares>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Shares, Pred> Shares_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Shares, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Node2Interconnect> Node2Interconnect_kind_children() const { return Udm::ChildrenAttr<PICML::Node2Interconnect>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Node2Interconnect, Pred> Node2Interconnect_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Node2Interconnect, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Interconnect2Bridge> Interconnect2Bridge_kind_children() const { return Udm::ChildrenAttr<PICML::Interconnect2Bridge>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Interconnect2Bridge, Pred> Interconnect2Bridge_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Interconnect2Bridge, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_Targets_parent;
		Udm::ParentAttr<PICML::Targets> Targets_parent() const { return Udm::ParentAttr<PICML::Targets>(impl, meta_Targets_parent); }

		Udm::ParentAttr<PICML::Targets> parent() const { return Udm::ParentAttr<PICML::Targets>(impl, Udm::NULLPARENTROLE); }
	};

	class  Interconnect2Node :  public MgaObject {
	 public:
		static Uml::Class meta;

		Interconnect2Node() { }
		Interconnect2Node(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Interconnect2Node(const Interconnect2Node &master) : MgaObject(master) { }
		static Interconnect2Node Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Interconnect2Node Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Interconnect2Node CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Interconnect2Node CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Interconnect2Node> Instances() { return Udm::InstantiatedAttr<PICML::Interconnect2Node>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Interconnect2Node, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Interconnect2Node, Pred>(impl);}

		Udm::DerivedAttr<PICML::Interconnect2Node> Derived() { return Udm::DerivedAttr<PICML::Interconnect2Node>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Interconnect2Node, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Interconnect2Node, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Interconnect2Node> Archetype() { return Udm::ArchetypeAttr<PICML::Interconnect2Node>(impl);}

		static Uml::CompositionParentRole meta_Domain_parent;
		Udm::ParentAttr<PICML::Domain> Domain_parent() const { return Udm::ParentAttr<PICML::Domain>(impl, meta_Domain_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstInterconnect2Node_end_;
		Udm::AssocEndAttr<PICML::Node> dstInterconnect2Node_end() const { return Udm::AssocEndAttr<PICML::Node>(impl, meta_dstInterconnect2Node_end_); }

		static Uml::AssociationRole meta_srcInterconnect2Node_end_;
		Udm::AssocEndAttr<PICML::Interconnect> srcInterconnect2Node_end() const { return Udm::AssocEndAttr<PICML::Interconnect>(impl, meta_srcInterconnect2Node_end_); }

	};

	class  Bridge2Interconnect :  public MgaObject {
	 public:
		static Uml::Class meta;

		Bridge2Interconnect() { }
		Bridge2Interconnect(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Bridge2Interconnect(const Bridge2Interconnect &master) : MgaObject(master) { }
		static Bridge2Interconnect Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Bridge2Interconnect Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Bridge2Interconnect CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Bridge2Interconnect CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Bridge2Interconnect> Instances() { return Udm::InstantiatedAttr<PICML::Bridge2Interconnect>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Bridge2Interconnect, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Bridge2Interconnect, Pred>(impl);}

		Udm::DerivedAttr<PICML::Bridge2Interconnect> Derived() { return Udm::DerivedAttr<PICML::Bridge2Interconnect>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Bridge2Interconnect, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Bridge2Interconnect, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Bridge2Interconnect> Archetype() { return Udm::ArchetypeAttr<PICML::Bridge2Interconnect>(impl);}

		static Uml::CompositionParentRole meta_Domain_parent;
		Udm::ParentAttr<PICML::Domain> Domain_parent() const { return Udm::ParentAttr<PICML::Domain>(impl, meta_Domain_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstBridge2Interconnect_end_;
		Udm::AssocEndAttr<PICML::Interconnect> dstBridge2Interconnect_end() const { return Udm::AssocEndAttr<PICML::Interconnect>(impl, meta_dstBridge2Interconnect_end_); }

		static Uml::AssociationRole meta_srcBridge2Interconnect_end_;
		Udm::AssocEndAttr<PICML::Bridge> srcBridge2Interconnect_end() const { return Udm::AssocEndAttr<PICML::Bridge>(impl, meta_srcBridge2Interconnect_end_); }

	};

	class  Shares :  public MgaObject {
	 public:
		static Uml::Class meta;

		Shares() { }
		Shares(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Shares(const Shares &master) : MgaObject(master) { }
		static Shares Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Shares Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Shares CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Shares CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Shares> Instances() { return Udm::InstantiatedAttr<PICML::Shares>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Shares, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Shares, Pred>(impl);}

		Udm::DerivedAttr<PICML::Shares> Derived() { return Udm::DerivedAttr<PICML::Shares>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Shares, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Shares, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Shares> Archetype() { return Udm::ArchetypeAttr<PICML::Shares>(impl);}

		static Uml::CompositionParentRole meta_Domain_parent;
		Udm::ParentAttr<PICML::Domain> Domain_parent() const { return Udm::ParentAttr<PICML::Domain>(impl, meta_Domain_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcShares_end_;
		Udm::AssocEndAttr<PICML::Node> srcShares_end() const { return Udm::AssocEndAttr<PICML::Node>(impl, meta_srcShares_end_); }

		static Uml::AssociationRole meta_dstShares_end_;
		Udm::AssocEndAttr<PICML::SharedResource> dstShares_end() const { return Udm::AssocEndAttr<PICML::SharedResource>(impl, meta_dstShares_end_); }

	};

	class  Node2Interconnect :  public MgaObject {
	 public:
		static Uml::Class meta;

		Node2Interconnect() { }
		Node2Interconnect(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Node2Interconnect(const Node2Interconnect &master) : MgaObject(master) { }
		static Node2Interconnect Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Node2Interconnect Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Node2Interconnect CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Node2Interconnect CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Node2Interconnect> Instances() { return Udm::InstantiatedAttr<PICML::Node2Interconnect>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Node2Interconnect, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Node2Interconnect, Pred>(impl);}

		Udm::DerivedAttr<PICML::Node2Interconnect> Derived() { return Udm::DerivedAttr<PICML::Node2Interconnect>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Node2Interconnect, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Node2Interconnect, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Node2Interconnect> Archetype() { return Udm::ArchetypeAttr<PICML::Node2Interconnect>(impl);}

		static Uml::CompositionParentRole meta_Domain_parent;
		Udm::ParentAttr<PICML::Domain> Domain_parent() const { return Udm::ParentAttr<PICML::Domain>(impl, meta_Domain_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcNode2Interconnect_end_;
		Udm::AssocEndAttr<PICML::Node> srcNode2Interconnect_end() const { return Udm::AssocEndAttr<PICML::Node>(impl, meta_srcNode2Interconnect_end_); }

		static Uml::AssociationRole meta_dstNode2Interconnect_end_;
		Udm::AssocEndAttr<PICML::Interconnect> dstNode2Interconnect_end() const { return Udm::AssocEndAttr<PICML::Interconnect>(impl, meta_dstNode2Interconnect_end_); }

	};

	class  Interconnect2Bridge :  public MgaObject {
	 public:
		static Uml::Class meta;

		Interconnect2Bridge() { }
		Interconnect2Bridge(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Interconnect2Bridge(const Interconnect2Bridge &master) : MgaObject(master) { }
		static Interconnect2Bridge Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Interconnect2Bridge Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Interconnect2Bridge CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Interconnect2Bridge CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Interconnect2Bridge> Instances() { return Udm::InstantiatedAttr<PICML::Interconnect2Bridge>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Interconnect2Bridge, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Interconnect2Bridge, Pred>(impl);}

		Udm::DerivedAttr<PICML::Interconnect2Bridge> Derived() { return Udm::DerivedAttr<PICML::Interconnect2Bridge>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Interconnect2Bridge, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Interconnect2Bridge, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Interconnect2Bridge> Archetype() { return Udm::ArchetypeAttr<PICML::Interconnect2Bridge>(impl);}

		static Uml::CompositionParentRole meta_Domain_parent;
		Udm::ParentAttr<PICML::Domain> Domain_parent() const { return Udm::ParentAttr<PICML::Domain>(impl, meta_Domain_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcInterconnect2Bridge_end_;
		Udm::AssocEndAttr<PICML::Interconnect> srcInterconnect2Bridge_end() const { return Udm::AssocEndAttr<PICML::Interconnect>(impl, meta_srcInterconnect2Bridge_end_); }

		static Uml::AssociationRole meta_dstInterconnect2Bridge_end_;
		Udm::AssocEndAttr<PICML::Bridge> dstInterconnect2Bridge_end() const { return Udm::AssocEndAttr<PICML::Bridge>(impl, meta_dstInterconnect2Bridge_end_); }

	};

	class  AssemblyDeployRequirement :  public MgaObject {
	 public:
		static Uml::Class meta;

		AssemblyDeployRequirement() { }
		AssemblyDeployRequirement(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		AssemblyDeployRequirement(const AssemblyDeployRequirement &master) : MgaObject(master) { }
		static AssemblyDeployRequirement Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static AssemblyDeployRequirement Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		AssemblyDeployRequirement CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		AssemblyDeployRequirement CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<AssemblyDeployRequirement> Instances() { return Udm::InstantiatedAttr<PICML::AssemblyDeployRequirement>(impl);}
		template <class Pred> Udm::InstantiatedAttr<AssemblyDeployRequirement, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::AssemblyDeployRequirement, Pred>(impl);}

		Udm::DerivedAttr<PICML::AssemblyDeployRequirement> Derived() { return Udm::DerivedAttr<PICML::AssemblyDeployRequirement>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::AssemblyDeployRequirement, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::AssemblyDeployRequirement, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::AssemblyDeployRequirement> Archetype() { return Udm::ArchetypeAttr<PICML::AssemblyDeployRequirement>(impl);}

		static Uml::CompositionParentRole meta_ComponentAssembly_parent;
		Udm::ParentAttr<PICML::ComponentAssembly> ComponentAssembly_parent() const { return Udm::ParentAttr<PICML::ComponentAssembly>(impl, meta_ComponentAssembly_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstAssemblyDeployRequirement_end_;
		Udm::AssocEndAttr<PICML::Requirement> dstAssemblyDeployRequirement_end() const { return Udm::AssocEndAttr<PICML::Requirement>(impl, meta_dstAssemblyDeployRequirement_end_); }

		static Uml::AssociationRole meta_srcAssemblyDeployRequirement_end_;
		Udm::AssocEndAttr<PICML::PublishConnector> srcAssemblyDeployRequirement_end() const { return Udm::AssocEndAttr<PICML::PublishConnector>(impl, meta_srcAssemblyDeployRequirement_end_); }

	};

	class  invoke :  public MgaObject {
	 public:
		static Uml::Class meta;

		invoke() { }
		invoke(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		invoke(const invoke &master) : MgaObject(master) { }
		static invoke Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static invoke Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		invoke CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		invoke CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<invoke> Instances() { return Udm::InstantiatedAttr<PICML::invoke>(impl);}
		template <class Pred> Udm::InstantiatedAttr<invoke, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::invoke, Pred>(impl);}

		Udm::DerivedAttr<PICML::invoke> Derived() { return Udm::DerivedAttr<PICML::invoke>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::invoke, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::invoke, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::invoke> Archetype() { return Udm::ArchetypeAttr<PICML::invoke>(impl);}

		static Uml::CompositionParentRole meta_ComponentAssembly_parent;
		Udm::ParentAttr<PICML::ComponentAssembly> ComponentAssembly_parent() const { return Udm::ParentAttr<PICML::ComponentAssembly>(impl, meta_ComponentAssembly_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstinvoke_end_;
		Udm::AssocEndAttr<PICML::ProvidedRequestPort> dstinvoke_end() const { return Udm::AssocEndAttr<PICML::ProvidedRequestPort>(impl, meta_dstinvoke_end_); }

		static Uml::AssociationRole meta_srcinvoke_end_;
		Udm::AssocEndAttr<PICML::RequiredRequestPort> srcinvoke_end() const { return Udm::AssocEndAttr<PICML::RequiredRequestPort>(impl, meta_srcinvoke_end_); }

	};

	class  ComponentAssembly :  public ComponentImplementation {
	 public:
		static Uml::Class meta;

		ComponentAssembly() { }
		ComponentAssembly(Udm::ObjectImpl *impl) : ComponentImplementation(impl) { }
		ComponentAssembly(const ComponentAssembly &master) : ComponentImplementation(master) { }
		static ComponentAssembly Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentAssembly Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentAssembly CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentAssembly CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentAssembly> Instances() { return Udm::InstantiatedAttr<PICML::ComponentAssembly>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentAssembly, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentAssembly, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentAssembly> Derived() { return Udm::DerivedAttr<PICML::ComponentAssembly>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentAssembly, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentAssembly, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentAssembly> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentAssembly>(impl);}

		static Uml::CompositionChildRole meta_PublishConnector_children;
		Udm::ChildrenAttr<PICML::PublishConnector> PublishConnector_children() const { return Udm::ChildrenAttr<PICML::PublishConnector>(impl, meta_PublishConnector_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::PublishConnector, Pred> PublishConnector_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PublishConnector, Pred>(impl, meta_PublishConnector_children); }

		static Uml::CompositionChildRole meta_AssemblyConfigProperty_children;
		Udm::ChildrenAttr<PICML::AssemblyConfigProperty> AssemblyConfigProperty_children() const { return Udm::ChildrenAttr<PICML::AssemblyConfigProperty>(impl, meta_AssemblyConfigProperty_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::AssemblyConfigProperty, Pred> AssemblyConfigProperty_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::AssemblyConfigProperty, Pred>(impl, meta_AssemblyConfigProperty_children); }

		static Uml::CompositionChildRole meta_Requirement_children;
		Udm::ChildrenAttr<PICML::Requirement> Requirement_children() const { return Udm::ChildrenAttr<PICML::Requirement>(impl, meta_Requirement_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Requirement, Pred> Requirement_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Requirement, Pred>(impl, meta_Requirement_children); }

		static Uml::CompositionChildRole meta_ComponentPackageReference_children;
		Udm::ChildrenAttr<PICML::ComponentPackageReference> ComponentPackageReference_children() const { return Udm::ChildrenAttr<PICML::ComponentPackageReference>(impl, meta_ComponentPackageReference_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentPackageReference, Pred> ComponentPackageReference_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentPackageReference, Pred>(impl, meta_ComponentPackageReference_children); }

		static Uml::CompositionChildRole meta_emit_children;
		Udm::ChildrenAttr<PICML::emit> emit_children() const { return Udm::ChildrenAttr<PICML::emit>(impl, meta_emit_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::emit, Pred> emit_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::emit, Pred>(impl, meta_emit_children); }

		static Uml::CompositionChildRole meta_Component_children;
		Udm::ChildrenAttr<PICML::Component> Component_children() const { return Udm::ChildrenAttr<PICML::Component>(impl, meta_Component_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Component, Pred> Component_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Component, Pred>(impl, meta_Component_children); }

		static Uml::CompositionChildRole meta_AssemblyDeployRequirement_children;
		Udm::ChildrenAttr<PICML::AssemblyDeployRequirement> AssemblyDeployRequirement_children() const { return Udm::ChildrenAttr<PICML::AssemblyDeployRequirement>(impl, meta_AssemblyDeployRequirement_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::AssemblyDeployRequirement, Pred> AssemblyDeployRequirement_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::AssemblyDeployRequirement, Pred>(impl, meta_AssemblyDeployRequirement_children); }

		static Uml::CompositionChildRole meta_publish_children;
		Udm::ChildrenAttr<PICML::publish> publish_children() const { return Udm::ChildrenAttr<PICML::publish>(impl, meta_publish_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::publish, Pred> publish_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::publish, Pred>(impl, meta_publish_children); }

		static Uml::CompositionChildRole meta_AssemblyselectRequirement_children;
		Udm::ChildrenAttr<PICML::AssemblyselectRequirement> AssemblyselectRequirement_children() const { return Udm::ChildrenAttr<PICML::AssemblyselectRequirement>(impl, meta_AssemblyselectRequirement_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::AssemblyselectRequirement, Pred> AssemblyselectRequirement_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::AssemblyselectRequirement, Pred>(impl, meta_AssemblyselectRequirement_children); }

		static Uml::CompositionChildRole meta_ComponentPackage_children;
		Udm::ChildrenAttr<PICML::ComponentPackage> ComponentPackage_children() const { return Udm::ChildrenAttr<PICML::ComponentPackage>(impl, meta_ComponentPackage_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentPackage, Pred> ComponentPackage_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentPackage, Pred>(impl, meta_ComponentPackage_children); }

		static Uml::CompositionChildRole meta_invoke_children;
		Udm::ChildrenAttr<PICML::invoke> invoke_children() const { return Udm::ChildrenAttr<PICML::invoke>(impl, meta_invoke_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::invoke, Pred> invoke_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::invoke, Pred>(impl, meta_invoke_children); }

		static Uml::CompositionChildRole meta_deliverTo_children;
		Udm::ChildrenAttr<PICML::deliverTo> deliverTo_children() const { return Udm::ChildrenAttr<PICML::deliverTo>(impl, meta_deliverTo_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::deliverTo, Pred> deliverTo_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::deliverTo, Pred>(impl, meta_deliverTo_children); }

		static Uml::CompositionChildRole meta_Property_children;
		Udm::ChildrenAttr<PICML::Property> Property_children() const { return Udm::ChildrenAttr<PICML::Property>(impl, meta_Property_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Property, Pred> Property_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Property, Pred>(impl, meta_Property_children); }

		Udm::ChildrenAttr<PICML::ComponentPackage> ComponentPackage_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentPackage>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentPackage, Pred> ComponentPackage_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentPackage, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentPackageReference> ComponentPackageReference_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentPackageReference>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentPackageReference, Pred> ComponentPackageReference_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentPackageReference, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Requirement> Requirement_kind_children() const { return Udm::ChildrenAttr<PICML::Requirement>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Requirement, Pred> Requirement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Requirement, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Property> Property_kind_children() const { return Udm::ChildrenAttr<PICML::Property>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Property, Pred> Property_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Property, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::RequirementBase> RequirementBase_kind_children() const { return Udm::ChildrenAttr<PICML::RequirementBase>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::RequirementBase, Pred> RequirementBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::RequirementBase, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::AssemblyDeployRequirement> AssemblyDeployRequirement_kind_children() const { return Udm::ChildrenAttr<PICML::AssemblyDeployRequirement>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::AssemblyDeployRequirement, Pred> AssemblyDeployRequirement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::AssemblyDeployRequirement, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::invoke> invoke_kind_children() const { return Udm::ChildrenAttr<PICML::invoke>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::invoke, Pred> invoke_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::invoke, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::AssemblyselectRequirement> AssemblyselectRequirement_kind_children() const { return Udm::ChildrenAttr<PICML::AssemblyselectRequirement>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::AssemblyselectRequirement, Pred> AssemblyselectRequirement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::AssemblyselectRequirement, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::emit> emit_kind_children() const { return Udm::ChildrenAttr<PICML::emit>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::emit, Pred> emit_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::emit, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::PublishConnector> PublishConnector_kind_children() const { return Udm::ChildrenAttr<PICML::PublishConnector>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PublishConnector, Pred> PublishConnector_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PublishConnector, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::deliverTo> deliverTo_kind_children() const { return Udm::ChildrenAttr<PICML::deliverTo>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::deliverTo, Pred> deliverTo_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::deliverTo, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::publish> publish_kind_children() const { return Udm::ChildrenAttr<PICML::publish>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::publish, Pred> publish_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::publish, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::AssemblyConfigProperty> AssemblyConfigProperty_kind_children() const { return Udm::ChildrenAttr<PICML::AssemblyConfigProperty>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::AssemblyConfigProperty, Pred> AssemblyConfigProperty_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::AssemblyConfigProperty, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MemberType> MemberType_kind_children() const { return Udm::ChildrenAttr<PICML::MemberType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MemberType, Pred> MemberType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MemberType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::NamedType> NamedType_kind_children() const { return Udm::ChildrenAttr<PICML::NamedType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::NamedType, Pred> NamedType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::NamedType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Inheritable> Inheritable_kind_children() const { return Udm::ChildrenAttr<PICML::Inheritable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Inheritable, Pred> Inheritable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Inheritable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::SupportsInterfaces> SupportsInterfaces_kind_children() const { return Udm::ChildrenAttr<PICML::SupportsInterfaces>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::SupportsInterfaces, Pred> SupportsInterfaces_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::SupportsInterfaces, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Component> Component_kind_children() const { return Udm::ChildrenAttr<PICML::Component>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Component, Pred> Component_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Component, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Manageable> Manageable_kind_children() const { return Udm::ChildrenAttr<PICML::Manageable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Manageable, Pred> Manageable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Manageable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Taggable> Taggable_kind_children() const { return Udm::ChildrenAttr<PICML::Taggable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Taggable, Pred> Taggable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Taggable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  AssemblyselectRequirement :  public MgaObject {
	 public:
		static Uml::Class meta;

		AssemblyselectRequirement() { }
		AssemblyselectRequirement(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		AssemblyselectRequirement(const AssemblyselectRequirement &master) : MgaObject(master) { }
		static AssemblyselectRequirement Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static AssemblyselectRequirement Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		AssemblyselectRequirement CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		AssemblyselectRequirement CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<AssemblyselectRequirement> Instances() { return Udm::InstantiatedAttr<PICML::AssemblyselectRequirement>(impl);}
		template <class Pred> Udm::InstantiatedAttr<AssemblyselectRequirement, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::AssemblyselectRequirement, Pred>(impl);}

		Udm::DerivedAttr<PICML::AssemblyselectRequirement> Derived() { return Udm::DerivedAttr<PICML::AssemblyselectRequirement>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::AssemblyselectRequirement, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::AssemblyselectRequirement, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::AssemblyselectRequirement> Archetype() { return Udm::ArchetypeAttr<PICML::AssemblyselectRequirement>(impl);}

		static Uml::CompositionParentRole meta_ComponentAssembly_parent;
		Udm::ParentAttr<PICML::ComponentAssembly> ComponentAssembly_parent() const { return Udm::ParentAttr<PICML::ComponentAssembly>(impl, meta_ComponentAssembly_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstAssemblyselectRequirement_end_;
		Udm::AssocEndAttr<PICML::Requirement> dstAssemblyselectRequirement_end() const { return Udm::AssocEndAttr<PICML::Requirement>(impl, meta_dstAssemblyselectRequirement_end_); }

		static Uml::AssociationRole meta_srcAssemblyselectRequirement_end_;
		Udm::AssocEndAttr<PICML::Component> srcAssemblyselectRequirement_end() const { return Udm::AssocEndAttr<PICML::Component>(impl, meta_srcAssemblyselectRequirement_end_); }

	};

	class  emit :  public MgaObject {
	 public:
		static Uml::Class meta;

		emit() { }
		emit(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		emit(const emit &master) : MgaObject(master) { }
		static emit Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static emit Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		emit CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		emit CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<emit> Instances() { return Udm::InstantiatedAttr<PICML::emit>(impl);}
		template <class Pred> Udm::InstantiatedAttr<emit, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::emit, Pred>(impl);}

		Udm::DerivedAttr<PICML::emit> Derived() { return Udm::DerivedAttr<PICML::emit>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::emit, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::emit, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::emit> Archetype() { return Udm::ArchetypeAttr<PICML::emit>(impl);}

		static Uml::CompositionParentRole meta_ComponentAssembly_parent;
		Udm::ParentAttr<PICML::ComponentAssembly> ComponentAssembly_parent() const { return Udm::ParentAttr<PICML::ComponentAssembly>(impl, meta_ComponentAssembly_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcemit_end_;
		Udm::AssocEndAttr<PICML::OutEventPort> srcemit_end() const { return Udm::AssocEndAttr<PICML::OutEventPort>(impl, meta_srcemit_end_); }

		static Uml::AssociationRole meta_dstemit_end_;
		Udm::AssocEndAttr<PICML::InEventPort> dstemit_end() const { return Udm::AssocEndAttr<PICML::InEventPort>(impl, meta_dstemit_end_); }

	};

	class  PublishConnector :  public MgaObject {
	 public:
		static Uml::Class meta;

		PublishConnector() { }
		PublishConnector(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		PublishConnector(const PublishConnector &master) : MgaObject(master) { }
		static PublishConnector Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PublishConnector Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PublishConnector CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PublishConnector CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PublishConnector> Instances() { return Udm::InstantiatedAttr<PICML::PublishConnector>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PublishConnector, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PublishConnector, Pred>(impl);}

		Udm::DerivedAttr<PICML::PublishConnector> Derived() { return Udm::DerivedAttr<PICML::PublishConnector>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PublishConnector, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PublishConnector, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PublishConnector> Archetype() { return Udm::ArchetypeAttr<PICML::PublishConnector>(impl);}

		static Uml::AssociationRole meta_dstAssemblyDeployRequirement, meta_dstAssemblyDeployRequirement_rev;
		Udm::AClassAssocAttr<PICML::AssemblyDeployRequirement, PICML::Requirement> dstAssemblyDeployRequirement() const { return Udm::AClassAssocAttr<PICML::AssemblyDeployRequirement, PICML::Requirement>(impl, meta_dstAssemblyDeployRequirement, meta_dstAssemblyDeployRequirement_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::AssemblyDeployRequirement, PICML::Requirement, Pred> dstAssemblyDeployRequirement_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::AssemblyDeployRequirement, PICML::Requirement, Pred>(impl, meta_dstAssemblyDeployRequirement, meta_dstAssemblyDeployRequirement_rev); }

		static Uml::AssociationRole meta_dstdeliverTo, meta_dstdeliverTo_rev;
		Udm::AClassAssocAttr<PICML::deliverTo, PICML::InEventPort> dstdeliverTo() const { return Udm::AClassAssocAttr<PICML::deliverTo, PICML::InEventPort>(impl, meta_dstdeliverTo, meta_dstdeliverTo_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::deliverTo, PICML::InEventPort, Pred> dstdeliverTo_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::deliverTo, PICML::InEventPort, Pred>(impl, meta_dstdeliverTo, meta_dstdeliverTo_rev); }

		static Uml::AssociationRole meta_srcpublish, meta_srcpublish_rev;
		Udm::AClassAssocAttr<PICML::publish, PICML::OutEventPort> srcpublish() const { return Udm::AClassAssocAttr<PICML::publish, PICML::OutEventPort>(impl, meta_srcpublish, meta_srcpublish_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::publish, PICML::OutEventPort, Pred> srcpublish_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::publish, PICML::OutEventPort, Pred>(impl, meta_srcpublish, meta_srcpublish_rev); }

		static Uml::CompositionParentRole meta_ComponentAssembly_parent;
		Udm::ParentAttr<PICML::ComponentAssembly> ComponentAssembly_parent() const { return Udm::ParentAttr<PICML::ComponentAssembly>(impl, meta_ComponentAssembly_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  deliverTo :  public MgaObject {
	 public:
		static Uml::Class meta;

		deliverTo() { }
		deliverTo(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		deliverTo(const deliverTo &master) : MgaObject(master) { }
		static deliverTo Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static deliverTo Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		deliverTo CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		deliverTo CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<deliverTo> Instances() { return Udm::InstantiatedAttr<PICML::deliverTo>(impl);}
		template <class Pred> Udm::InstantiatedAttr<deliverTo, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::deliverTo, Pred>(impl);}

		Udm::DerivedAttr<PICML::deliverTo> Derived() { return Udm::DerivedAttr<PICML::deliverTo>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::deliverTo, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::deliverTo, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::deliverTo> Archetype() { return Udm::ArchetypeAttr<PICML::deliverTo>(impl);}

		static Uml::CompositionParentRole meta_ComponentAssembly_parent;
		Udm::ParentAttr<PICML::ComponentAssembly> ComponentAssembly_parent() const { return Udm::ParentAttr<PICML::ComponentAssembly>(impl, meta_ComponentAssembly_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcdeliverTo_end_;
		Udm::AssocEndAttr<PICML::PublishConnector> srcdeliverTo_end() const { return Udm::AssocEndAttr<PICML::PublishConnector>(impl, meta_srcdeliverTo_end_); }

		static Uml::AssociationRole meta_dstdeliverTo_end_;
		Udm::AssocEndAttr<PICML::InEventPort> dstdeliverTo_end() const { return Udm::AssocEndAttr<PICML::InEventPort>(impl, meta_dstdeliverTo_end_); }

	};

	class  publish :  public MgaObject {
	 public:
		static Uml::Class meta;

		publish() { }
		publish(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		publish(const publish &master) : MgaObject(master) { }
		static publish Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static publish Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		publish CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		publish CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<publish> Instances() { return Udm::InstantiatedAttr<PICML::publish>(impl);}
		template <class Pred> Udm::InstantiatedAttr<publish, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::publish, Pred>(impl);}

		Udm::DerivedAttr<PICML::publish> Derived() { return Udm::DerivedAttr<PICML::publish>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::publish, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::publish, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::publish> Archetype() { return Udm::ArchetypeAttr<PICML::publish>(impl);}

		static Uml::CompositionParentRole meta_ComponentAssembly_parent;
		Udm::ParentAttr<PICML::ComponentAssembly> ComponentAssembly_parent() const { return Udm::ParentAttr<PICML::ComponentAssembly>(impl, meta_ComponentAssembly_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstpublish_end_;
		Udm::AssocEndAttr<PICML::PublishConnector> dstpublish_end() const { return Udm::AssocEndAttr<PICML::PublishConnector>(impl, meta_dstpublish_end_); }

		static Uml::AssociationRole meta_srcpublish_end_;
		Udm::AssocEndAttr<PICML::OutEventPort> srcpublish_end() const { return Udm::AssocEndAttr<PICML::OutEventPort>(impl, meta_srcpublish_end_); }

	};

	class  AssemblyConfigProperty :  public MgaObject {
	 public:
		static Uml::Class meta;

		AssemblyConfigProperty() { }
		AssemblyConfigProperty(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		AssemblyConfigProperty(const AssemblyConfigProperty &master) : MgaObject(master) { }
		static AssemblyConfigProperty Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static AssemblyConfigProperty Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		AssemblyConfigProperty CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		AssemblyConfigProperty CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<AssemblyConfigProperty> Instances() { return Udm::InstantiatedAttr<PICML::AssemblyConfigProperty>(impl);}
		template <class Pred> Udm::InstantiatedAttr<AssemblyConfigProperty, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::AssemblyConfigProperty, Pred>(impl);}

		Udm::DerivedAttr<PICML::AssemblyConfigProperty> Derived() { return Udm::DerivedAttr<PICML::AssemblyConfigProperty>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::AssemblyConfigProperty, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::AssemblyConfigProperty, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::AssemblyConfigProperty> Archetype() { return Udm::ArchetypeAttr<PICML::AssemblyConfigProperty>(impl);}

		static Uml::CompositionParentRole meta_ComponentAssembly_parent;
		Udm::ParentAttr<PICML::ComponentAssembly> ComponentAssembly_parent() const { return Udm::ParentAttr<PICML::ComponentAssembly>(impl, meta_ComponentAssembly_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstAssemblyConfigProperty_end_;
		Udm::AssocEndAttr<PICML::Property> dstAssemblyConfigProperty_end() const { return Udm::AssocEndAttr<PICML::Property>(impl, meta_dstAssemblyConfigProperty_end_); }

		static Uml::AssociationRole meta_srcAssemblyConfigProperty_end_;
		Udm::AssocEndAttr<PICML::Component> srcAssemblyConfigProperty_end() const { return Udm::AssocEndAttr<PICML::Component>(impl, meta_srcAssemblyConfigProperty_end_); }

	};

	class  ArtifactExecParameter :  public MgaObject {
	 public:
		static Uml::Class meta;

		ArtifactExecParameter() { }
		ArtifactExecParameter(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ArtifactExecParameter(const ArtifactExecParameter &master) : MgaObject(master) { }
		static ArtifactExecParameter Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ArtifactExecParameter Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ArtifactExecParameter CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ArtifactExecParameter CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ArtifactExecParameter> Instances() { return Udm::InstantiatedAttr<PICML::ArtifactExecParameter>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ArtifactExecParameter, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ArtifactExecParameter, Pred>(impl);}

		Udm::DerivedAttr<PICML::ArtifactExecParameter> Derived() { return Udm::DerivedAttr<PICML::ArtifactExecParameter>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ArtifactExecParameter, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ArtifactExecParameter, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ArtifactExecParameter> Archetype() { return Udm::ArchetypeAttr<PICML::ArtifactExecParameter>(impl);}

		static Uml::CompositionParentRole meta_ArtifactContainer_parent;
		Udm::ParentAttr<PICML::ArtifactContainer> ArtifactContainer_parent() const { return Udm::ParentAttr<PICML::ArtifactContainer>(impl, meta_ArtifactContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstArtifactExecParameter_end_;
		Udm::AssocEndAttr<PICML::Property> dstArtifactExecParameter_end() const { return Udm::AssocEndAttr<PICML::Property>(impl, meta_dstArtifactExecParameter_end_); }

		static Uml::AssociationRole meta_srcArtifactExecParameter_end_;
		Udm::AssocEndAttr<PICML::ImplementationArtifact> srcArtifactExecParameter_end() const { return Udm::AssocEndAttr<PICML::ImplementationArtifact>(impl, meta_srcArtifactExecParameter_end_); }

	};

	class  ImplementationArtifactReference :  public MgaObject {
	 public:
		static Uml::Class meta;

		ImplementationArtifactReference() { }
		ImplementationArtifactReference(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ImplementationArtifactReference(const ImplementationArtifactReference &master) : MgaObject(master) { }
		static ImplementationArtifactReference Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ImplementationArtifactReference Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ImplementationArtifactReference CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ImplementationArtifactReference CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ImplementationArtifactReference> Instances() { return Udm::InstantiatedAttr<PICML::ImplementationArtifactReference>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ImplementationArtifactReference, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ImplementationArtifactReference, Pred>(impl);}

		Udm::DerivedAttr<PICML::ImplementationArtifactReference> Derived() { return Udm::DerivedAttr<PICML::ImplementationArtifactReference>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ImplementationArtifactReference, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ImplementationArtifactReference, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ImplementationArtifactReference> Archetype() { return Udm::ArchetypeAttr<PICML::ImplementationArtifactReference>(impl);}

		static Uml::AssociationRole meta_srcMonolithprimaryArtifact, meta_srcMonolithprimaryArtifact_rev;
		Udm::AClassAssocAttr<PICML::MonolithprimaryArtifact, PICML::MonolithicImplementation> srcMonolithprimaryArtifact() const { return Udm::AClassAssocAttr<PICML::MonolithprimaryArtifact, PICML::MonolithicImplementation>(impl, meta_srcMonolithprimaryArtifact, meta_srcMonolithprimaryArtifact_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::MonolithprimaryArtifact, PICML::MonolithicImplementation, Pred> srcMonolithprimaryArtifact_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::MonolithprimaryArtifact, PICML::MonolithicImplementation, Pred>(impl, meta_srcMonolithprimaryArtifact, meta_srcMonolithprimaryArtifact_rev); }

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::ImplementationArtifact> ref() const { return Udm::PointerAttr<PICML::ImplementationArtifact>(impl, meta_ref); }

		static Uml::AssociationRole meta_srcArtifactDependsOn, meta_srcArtifactDependsOn_rev;
		Udm::AClassAssocAttr<PICML::ArtifactDependsOn, PICML::ImplementationArtifact> srcArtifactDependsOn() const { return Udm::AClassAssocAttr<PICML::ArtifactDependsOn, PICML::ImplementationArtifact>(impl, meta_srcArtifactDependsOn, meta_srcArtifactDependsOn_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ArtifactDependsOn, PICML::ImplementationArtifact, Pred> srcArtifactDependsOn_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ArtifactDependsOn, PICML::ImplementationArtifact, Pred>(impl, meta_srcArtifactDependsOn, meta_srcArtifactDependsOn_rev); }

		static Uml::CompositionParentRole meta_ComponentImplementationContainer_parent;
		Udm::ParentAttr<PICML::ComponentImplementationContainer> ComponentImplementationContainer_parent() const { return Udm::ParentAttr<PICML::ComponentImplementationContainer>(impl, meta_ComponentImplementationContainer_parent); }

		static Uml::CompositionParentRole meta_ArtifactContainer_parent;
		Udm::ParentAttr<PICML::ArtifactContainer> ArtifactContainer_parent() const { return Udm::ParentAttr<PICML::ArtifactContainer>(impl, meta_ArtifactContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  ArtifactDependsOn :  public MgaObject {
	 public:
		static Uml::Class meta;

		ArtifactDependsOn() { }
		ArtifactDependsOn(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ArtifactDependsOn(const ArtifactDependsOn &master) : MgaObject(master) { }
		static ArtifactDependsOn Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ArtifactDependsOn Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ArtifactDependsOn CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ArtifactDependsOn CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ArtifactDependsOn> Instances() { return Udm::InstantiatedAttr<PICML::ArtifactDependsOn>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ArtifactDependsOn, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ArtifactDependsOn, Pred>(impl);}

		Udm::DerivedAttr<PICML::ArtifactDependsOn> Derived() { return Udm::DerivedAttr<PICML::ArtifactDependsOn>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ArtifactDependsOn, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ArtifactDependsOn, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ArtifactDependsOn> Archetype() { return Udm::ArchetypeAttr<PICML::ArtifactDependsOn>(impl);}

		static Uml::CompositionParentRole meta_ArtifactContainer_parent;
		Udm::ParentAttr<PICML::ArtifactContainer> ArtifactContainer_parent() const { return Udm::ParentAttr<PICML::ArtifactContainer>(impl, meta_ArtifactContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstArtifactDependsOn_end_;
		Udm::AssocEndAttr<PICML::ImplementationArtifactReference> dstArtifactDependsOn_end() const { return Udm::AssocEndAttr<PICML::ImplementationArtifactReference>(impl, meta_dstArtifactDependsOn_end_); }

		static Uml::AssociationRole meta_srcArtifactDependsOn_end_;
		Udm::AssocEndAttr<PICML::ImplementationArtifact> srcArtifactDependsOn_end() const { return Udm::AssocEndAttr<PICML::ImplementationArtifact>(impl, meta_srcArtifactDependsOn_end_); }

	};

	class  ImplementationArtifact :  public MgaObject {
	 public:
		static Uml::Class meta;

		ImplementationArtifact() { }
		ImplementationArtifact(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ImplementationArtifact(const ImplementationArtifact &master) : MgaObject(master) { }
		static ImplementationArtifact Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ImplementationArtifact Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ImplementationArtifact CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ImplementationArtifact CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ImplementationArtifact> Instances() { return Udm::InstantiatedAttr<PICML::ImplementationArtifact>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ImplementationArtifact, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ImplementationArtifact, Pred>(impl);}

		Udm::DerivedAttr<PICML::ImplementationArtifact> Derived() { return Udm::DerivedAttr<PICML::ImplementationArtifact>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ImplementationArtifact, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ImplementationArtifact, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ImplementationArtifact> Archetype() { return Udm::ArchetypeAttr<PICML::ImplementationArtifact>(impl);}

		static Uml::Attribute meta_UUID;
		Udm::StringAttr UUID() const { return Udm::StringAttr(impl, meta_UUID); }

		static Uml::Attribute meta_label;
		Udm::StringAttr label() const { return Udm::StringAttr(impl, meta_label); }

		static Uml::Attribute meta_location;
		Udm::StringAttr location() const { return Udm::StringAttr(impl, meta_location); }

		static Uml::AssociationRole meta_dstArtifactDeployRequirement, meta_dstArtifactDeployRequirement_rev;
		Udm::AClassAssocAttr<PICML::ArtifactDeployRequirement, PICML::Requirement> dstArtifactDeployRequirement() const { return Udm::AClassAssocAttr<PICML::ArtifactDeployRequirement, PICML::Requirement>(impl, meta_dstArtifactDeployRequirement, meta_dstArtifactDeployRequirement_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ArtifactDeployRequirement, PICML::Requirement, Pred> dstArtifactDeployRequirement_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ArtifactDeployRequirement, PICML::Requirement, Pred>(impl, meta_dstArtifactDeployRequirement, meta_dstArtifactDeployRequirement_rev); }

		static Uml::AssociationRole meta_dstArtifactExecParameter, meta_dstArtifactExecParameter_rev;
		Udm::AClassAssocAttr<PICML::ArtifactExecParameter, PICML::Property> dstArtifactExecParameter() const { return Udm::AClassAssocAttr<PICML::ArtifactExecParameter, PICML::Property>(impl, meta_dstArtifactExecParameter, meta_dstArtifactExecParameter_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ArtifactExecParameter, PICML::Property, Pred> dstArtifactExecParameter_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ArtifactExecParameter, PICML::Property, Pred>(impl, meta_dstArtifactExecParameter, meta_dstArtifactExecParameter_rev); }

		static Uml::AssociationRole meta_dstArtifactInfoProperty, meta_dstArtifactInfoProperty_rev;
		Udm::AClassAssocAttr<PICML::ArtifactInfoProperty, PICML::Property> dstArtifactInfoProperty() const { return Udm::AClassAssocAttr<PICML::ArtifactInfoProperty, PICML::Property>(impl, meta_dstArtifactInfoProperty, meta_dstArtifactInfoProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ArtifactInfoProperty, PICML::Property, Pred> dstArtifactInfoProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ArtifactInfoProperty, PICML::Property, Pred>(impl, meta_dstArtifactInfoProperty, meta_dstArtifactInfoProperty_rev); }

		static Uml::AssociationRole meta_referedbyImplementationArtifactReference;
		Udm::AssocAttr<PICML::ImplementationArtifactReference> referedbyImplementationArtifactReference() const { return Udm::AssocAttr<PICML::ImplementationArtifactReference>(impl, meta_referedbyImplementationArtifactReference); }
		template <class Pred> Udm::AssocAttr<PICML::ImplementationArtifactReference, Pred > referedbyImplementationArtifactReference_sorted(const Pred &) const { return Udm::AssocAttr<PICML::ImplementationArtifactReference, Pred>(impl, meta_referedbyImplementationArtifactReference); }

		static Uml::AssociationRole meta_dstArtifactDependsOn, meta_dstArtifactDependsOn_rev;
		Udm::AClassAssocAttr<PICML::ArtifactDependsOn, PICML::ImplementationArtifactReference> dstArtifactDependsOn() const { return Udm::AClassAssocAttr<PICML::ArtifactDependsOn, PICML::ImplementationArtifactReference>(impl, meta_dstArtifactDependsOn, meta_dstArtifactDependsOn_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ArtifactDependsOn, PICML::ImplementationArtifactReference, Pred> dstArtifactDependsOn_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ArtifactDependsOn, PICML::ImplementationArtifactReference, Pred>(impl, meta_dstArtifactDependsOn, meta_dstArtifactDependsOn_rev); }

		static Uml::CompositionParentRole meta_ArtifactContainer_parent;
		Udm::ParentAttr<PICML::ArtifactContainer> ArtifactContainer_parent() const { return Udm::ParentAttr<PICML::ArtifactContainer>(impl, meta_ArtifactContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  ArtifactDeployRequirement :  public MgaObject {
	 public:
		static Uml::Class meta;

		ArtifactDeployRequirement() { }
		ArtifactDeployRequirement(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ArtifactDeployRequirement(const ArtifactDeployRequirement &master) : MgaObject(master) { }
		static ArtifactDeployRequirement Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ArtifactDeployRequirement Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ArtifactDeployRequirement CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ArtifactDeployRequirement CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ArtifactDeployRequirement> Instances() { return Udm::InstantiatedAttr<PICML::ArtifactDeployRequirement>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ArtifactDeployRequirement, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ArtifactDeployRequirement, Pred>(impl);}

		Udm::DerivedAttr<PICML::ArtifactDeployRequirement> Derived() { return Udm::DerivedAttr<PICML::ArtifactDeployRequirement>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ArtifactDeployRequirement, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ArtifactDeployRequirement, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ArtifactDeployRequirement> Archetype() { return Udm::ArchetypeAttr<PICML::ArtifactDeployRequirement>(impl);}

		static Uml::CompositionParentRole meta_ArtifactContainer_parent;
		Udm::ParentAttr<PICML::ArtifactContainer> ArtifactContainer_parent() const { return Udm::ParentAttr<PICML::ArtifactContainer>(impl, meta_ArtifactContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstArtifactDeployRequirement_end_;
		Udm::AssocEndAttr<PICML::Requirement> dstArtifactDeployRequirement_end() const { return Udm::AssocEndAttr<PICML::Requirement>(impl, meta_dstArtifactDeployRequirement_end_); }

		static Uml::AssociationRole meta_srcArtifactDeployRequirement_end_;
		Udm::AssocEndAttr<PICML::ImplementationArtifact> srcArtifactDeployRequirement_end() const { return Udm::AssocEndAttr<PICML::ImplementationArtifact>(impl, meta_srcArtifactDeployRequirement_end_); }

	};

	class  ArtifactInfoProperty :  public MgaObject {
	 public:
		static Uml::Class meta;

		ArtifactInfoProperty() { }
		ArtifactInfoProperty(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ArtifactInfoProperty(const ArtifactInfoProperty &master) : MgaObject(master) { }
		static ArtifactInfoProperty Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ArtifactInfoProperty Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ArtifactInfoProperty CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ArtifactInfoProperty CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ArtifactInfoProperty> Instances() { return Udm::InstantiatedAttr<PICML::ArtifactInfoProperty>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ArtifactInfoProperty, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ArtifactInfoProperty, Pred>(impl);}

		Udm::DerivedAttr<PICML::ArtifactInfoProperty> Derived() { return Udm::DerivedAttr<PICML::ArtifactInfoProperty>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ArtifactInfoProperty, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ArtifactInfoProperty, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ArtifactInfoProperty> Archetype() { return Udm::ArchetypeAttr<PICML::ArtifactInfoProperty>(impl);}

		static Uml::CompositionParentRole meta_ArtifactContainer_parent;
		Udm::ParentAttr<PICML::ArtifactContainer> ArtifactContainer_parent() const { return Udm::ParentAttr<PICML::ArtifactContainer>(impl, meta_ArtifactContainer_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstArtifactInfoProperty_end_;
		Udm::AssocEndAttr<PICML::Property> dstArtifactInfoProperty_end() const { return Udm::AssocEndAttr<PICML::Property>(impl, meta_dstArtifactInfoProperty_end_); }

		static Uml::AssociationRole meta_srcArtifactInfoProperty_end_;
		Udm::AssocEndAttr<PICML::ImplementationArtifact> srcArtifactInfoProperty_end() const { return Udm::AssocEndAttr<PICML::ImplementationArtifact>(impl, meta_srcArtifactInfoProperty_end_); }

	};

	class  ArtifactContainer :  public MgaObject {
	 public:
		static Uml::Class meta;

		ArtifactContainer() { }
		ArtifactContainer(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ArtifactContainer(const ArtifactContainer &master) : MgaObject(master) { }
		static ArtifactContainer Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ArtifactContainer Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ArtifactContainer CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ArtifactContainer CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ArtifactContainer> Instances() { return Udm::InstantiatedAttr<PICML::ArtifactContainer>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ArtifactContainer, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ArtifactContainer, Pred>(impl);}

		Udm::DerivedAttr<PICML::ArtifactContainer> Derived() { return Udm::DerivedAttr<PICML::ArtifactContainer>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ArtifactContainer, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ArtifactContainer, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ArtifactContainer> Archetype() { return Udm::ArchetypeAttr<PICML::ArtifactContainer>(impl);}

		static Uml::CompositionChildRole meta_Requirement_children;
		Udm::ChildrenAttr<PICML::Requirement> Requirement_children() const { return Udm::ChildrenAttr<PICML::Requirement>(impl, meta_Requirement_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Requirement, Pred> Requirement_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Requirement, Pred>(impl, meta_Requirement_children); }

		static Uml::CompositionChildRole meta_ArtifactExecParameter_children;
		Udm::ChildrenAttr<PICML::ArtifactExecParameter> ArtifactExecParameter_children() const { return Udm::ChildrenAttr<PICML::ArtifactExecParameter>(impl, meta_ArtifactExecParameter_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ArtifactExecParameter, Pred> ArtifactExecParameter_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ArtifactExecParameter, Pred>(impl, meta_ArtifactExecParameter_children); }

		static Uml::CompositionChildRole meta_Property_children;
		Udm::ChildrenAttr<PICML::Property> Property_children() const { return Udm::ChildrenAttr<PICML::Property>(impl, meta_Property_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Property, Pred> Property_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Property, Pred>(impl, meta_Property_children); }

		static Uml::CompositionChildRole meta_ArtifactDeployRequirement_children;
		Udm::ChildrenAttr<PICML::ArtifactDeployRequirement> ArtifactDeployRequirement_children() const { return Udm::ChildrenAttr<PICML::ArtifactDeployRequirement>(impl, meta_ArtifactDeployRequirement_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ArtifactDeployRequirement, Pred> ArtifactDeployRequirement_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ArtifactDeployRequirement, Pred>(impl, meta_ArtifactDeployRequirement_children); }

		static Uml::CompositionChildRole meta_ImplementationArtifactReference_children;
		Udm::ChildrenAttr<PICML::ImplementationArtifactReference> ImplementationArtifactReference_children() const { return Udm::ChildrenAttr<PICML::ImplementationArtifactReference>(impl, meta_ImplementationArtifactReference_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ImplementationArtifactReference, Pred> ImplementationArtifactReference_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ImplementationArtifactReference, Pred>(impl, meta_ImplementationArtifactReference_children); }

		static Uml::CompositionChildRole meta_ImplementationArtifact_children;
		Udm::ChildrenAttr<PICML::ImplementationArtifact> ImplementationArtifact_children() const { return Udm::ChildrenAttr<PICML::ImplementationArtifact>(impl, meta_ImplementationArtifact_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ImplementationArtifact, Pred> ImplementationArtifact_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ImplementationArtifact, Pred>(impl, meta_ImplementationArtifact_children); }

		static Uml::CompositionChildRole meta_ArtifactInfoProperty_children;
		Udm::ChildrenAttr<PICML::ArtifactInfoProperty> ArtifactInfoProperty_children() const { return Udm::ChildrenAttr<PICML::ArtifactInfoProperty>(impl, meta_ArtifactInfoProperty_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ArtifactInfoProperty, Pred> ArtifactInfoProperty_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ArtifactInfoProperty, Pred>(impl, meta_ArtifactInfoProperty_children); }

		static Uml::CompositionChildRole meta_ArtifactDependsOn_children;
		Udm::ChildrenAttr<PICML::ArtifactDependsOn> ArtifactDependsOn_children() const { return Udm::ChildrenAttr<PICML::ArtifactDependsOn>(impl, meta_ArtifactDependsOn_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ArtifactDependsOn, Pred> ArtifactDependsOn_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ArtifactDependsOn, Pred>(impl, meta_ArtifactDependsOn_children); }

		Udm::ChildrenAttr<PICML::Requirement> Requirement_kind_children() const { return Udm::ChildrenAttr<PICML::Requirement>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Requirement, Pred> Requirement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Requirement, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Property> Property_kind_children() const { return Udm::ChildrenAttr<PICML::Property>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Property, Pred> Property_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Property, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::RequirementBase> RequirementBase_kind_children() const { return Udm::ChildrenAttr<PICML::RequirementBase>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::RequirementBase, Pred> RequirementBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::RequirementBase, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ArtifactExecParameter> ArtifactExecParameter_kind_children() const { return Udm::ChildrenAttr<PICML::ArtifactExecParameter>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ArtifactExecParameter, Pred> ArtifactExecParameter_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ArtifactExecParameter, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ImplementationArtifactReference> ImplementationArtifactReference_kind_children() const { return Udm::ChildrenAttr<PICML::ImplementationArtifactReference>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ImplementationArtifactReference, Pred> ImplementationArtifactReference_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ImplementationArtifactReference, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ArtifactDependsOn> ArtifactDependsOn_kind_children() const { return Udm::ChildrenAttr<PICML::ArtifactDependsOn>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ArtifactDependsOn, Pred> ArtifactDependsOn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ArtifactDependsOn, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ImplementationArtifact> ImplementationArtifact_kind_children() const { return Udm::ChildrenAttr<PICML::ImplementationArtifact>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ImplementationArtifact, Pred> ImplementationArtifact_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ImplementationArtifact, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ArtifactDeployRequirement> ArtifactDeployRequirement_kind_children() const { return Udm::ChildrenAttr<PICML::ArtifactDeployRequirement>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ArtifactDeployRequirement, Pred> ArtifactDeployRequirement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ArtifactDeployRequirement, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ArtifactInfoProperty> ArtifactInfoProperty_kind_children() const { return Udm::ChildrenAttr<PICML::ArtifactInfoProperty>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ArtifactInfoProperty, Pred> ArtifactInfoProperty_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ArtifactInfoProperty, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_ImplementationArtifacts_parent;
		Udm::ParentAttr<PICML::ImplementationArtifacts> ImplementationArtifacts_parent() const { return Udm::ParentAttr<PICML::ImplementationArtifacts>(impl, meta_ImplementationArtifacts_parent); }

		Udm::ParentAttr<PICML::ImplementationArtifacts> parent() const { return Udm::ParentAttr<PICML::ImplementationArtifacts>(impl, Udm::NULLPARENTROLE); }
	};

	class  ExceptionRef :  public MgaObject {
	 public:
		static Uml::Class meta;

		ExceptionRef() { }
		ExceptionRef(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ExceptionRef(const ExceptionRef &master) : MgaObject(master) { }
		static ExceptionRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ExceptionRef Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ExceptionRef CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ExceptionRef CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ExceptionRef> Instances() { return Udm::InstantiatedAttr<PICML::ExceptionRef>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ExceptionRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ExceptionRef, Pred>(impl);}

		Udm::DerivedAttr<PICML::ExceptionRef> Derived() { return Udm::DerivedAttr<PICML::ExceptionRef>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ExceptionRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ExceptionRef, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ExceptionRef> Archetype() { return Udm::ArchetypeAttr<PICML::ExceptionRef>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::Exception> ref() const { return Udm::PointerAttr<PICML::Exception>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_HasExceptions_parent;
		Udm::ParentAttr<PICML::HasExceptions> HasExceptions_parent() const { return Udm::ParentAttr<PICML::HasExceptions>(impl, meta_HasExceptions_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  FileRef :  public MgaObject {
	 public:
		static Uml::Class meta;

		FileRef() { }
		FileRef(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		FileRef(const FileRef &master) : MgaObject(master) { }
		static FileRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static FileRef Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		FileRef CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		FileRef CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<FileRef> Instances() { return Udm::InstantiatedAttr<PICML::FileRef>(impl);}
		template <class Pred> Udm::InstantiatedAttr<FileRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::FileRef, Pred>(impl);}

		Udm::DerivedAttr<PICML::FileRef> Derived() { return Udm::DerivedAttr<PICML::FileRef>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::FileRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::FileRef, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::FileRef> Archetype() { return Udm::ArchetypeAttr<PICML::FileRef>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::File> ref() const { return Udm::PointerAttr<PICML::File>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_File_parent;
		Udm::ParentAttr<PICML::File> File_parent() const { return Udm::ParentAttr<PICML::File>(impl, meta_File_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  ConstantType :  virtual public MgaObject {
	 public:
		static Uml::Class meta;

		ConstantType() { }
		ConstantType(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ConstantType(const ConstantType &master) : MgaObject(master) { }
		static ConstantType Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ConstantType Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ConstantType CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ConstantType CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ConstantType> Instances() { return Udm::InstantiatedAttr<PICML::ConstantType>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ConstantType, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ConstantType, Pred>(impl);}

		Udm::DerivedAttr<PICML::ConstantType> Derived() { return Udm::DerivedAttr<PICML::ConstantType>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ConstantType, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ConstantType, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ConstantType> Archetype() { return Udm::ArchetypeAttr<PICML::ConstantType>(impl);}

		static Uml::AssociationRole meta_referedbyDiscriminator;
		Udm::AssocAttr<PICML::Discriminator> referedbyDiscriminator() const { return Udm::AssocAttr<PICML::Discriminator>(impl, meta_referedbyDiscriminator); }
		template <class Pred> Udm::AssocAttr<PICML::Discriminator, Pred > referedbyDiscriminator_sorted(const Pred &) const { return Udm::AssocAttr<PICML::Discriminator, Pred>(impl, meta_referedbyDiscriminator); }

		static Uml::AssociationRole meta_referedbyConstant;
		Udm::AssocAttr<PICML::Constant> referedbyConstant() const { return Udm::AssocAttr<PICML::Constant>(impl, meta_referedbyConstant); }
		template <class Pred> Udm::AssocAttr<PICML::Constant, Pred > referedbyConstant_sorted(const Pred &) const { return Udm::AssocAttr<PICML::Constant, Pred>(impl, meta_referedbyConstant); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class  EnumValue :  public MgaObject {
	 public:
		static Uml::Class meta;

		EnumValue() { }
		EnumValue(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		EnumValue(const EnumValue &master) : MgaObject(master) { }
		static EnumValue Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static EnumValue Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		EnumValue CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		EnumValue CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<EnumValue> Instances() { return Udm::InstantiatedAttr<PICML::EnumValue>(impl);}
		template <class Pred> Udm::InstantiatedAttr<EnumValue, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::EnumValue, Pred>(impl);}

		Udm::DerivedAttr<PICML::EnumValue> Derived() { return Udm::DerivedAttr<PICML::EnumValue>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::EnumValue, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::EnumValue, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::EnumValue> Archetype() { return Udm::ArchetypeAttr<PICML::EnumValue>(impl);}

		static Uml::CompositionParentRole meta_Enum_parent;
		Udm::ParentAttr<PICML::Enum> Enum_parent() const { return Udm::ParentAttr<PICML::Enum>(impl, meta_Enum_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Label :  public MgaObject {
	 public:
		static Uml::Class meta;

		Label() { }
		Label(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Label(const Label &master) : MgaObject(master) { }
		static Label Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Label Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Label CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Label CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Label> Instances() { return Udm::InstantiatedAttr<PICML::Label>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Label, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Label, Pred>(impl);}

		Udm::DerivedAttr<PICML::Label> Derived() { return Udm::DerivedAttr<PICML::Label>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Label, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Label, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Label> Archetype() { return Udm::ArchetypeAttr<PICML::Label>(impl);}

		static Uml::AssociationRole meta_srcLabelConnection, meta_srcLabelConnection_rev;
		Udm::AClassPointerAttr<PICML::LabelConnection, PICML::Member> srcLabelConnection() const { return Udm::AClassPointerAttr<PICML::LabelConnection, PICML::Member>(impl, meta_srcLabelConnection, meta_srcLabelConnection_rev); }

		static Uml::CompositionParentRole meta_SwitchedAggregate_parent;
		Udm::ParentAttr<PICML::SwitchedAggregate> SwitchedAggregate_parent() const { return Udm::ParentAttr<PICML::SwitchedAggregate>(impl, meta_SwitchedAggregate_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  LabelConnection :  public MgaObject {
	 public:
		static Uml::Class meta;

		LabelConnection() { }
		LabelConnection(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		LabelConnection(const LabelConnection &master) : MgaObject(master) { }
		static LabelConnection Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static LabelConnection Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		LabelConnection CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		LabelConnection CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<LabelConnection> Instances() { return Udm::InstantiatedAttr<PICML::LabelConnection>(impl);}
		template <class Pred> Udm::InstantiatedAttr<LabelConnection, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::LabelConnection, Pred>(impl);}

		Udm::DerivedAttr<PICML::LabelConnection> Derived() { return Udm::DerivedAttr<PICML::LabelConnection>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::LabelConnection, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::LabelConnection, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::LabelConnection> Archetype() { return Udm::ArchetypeAttr<PICML::LabelConnection>(impl);}

		static Uml::CompositionParentRole meta_SwitchedAggregate_parent;
		Udm::ParentAttr<PICML::SwitchedAggregate> SwitchedAggregate_parent() const { return Udm::ParentAttr<PICML::SwitchedAggregate>(impl, meta_SwitchedAggregate_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstLabelConnection_end_;
		Udm::AssocEndAttr<PICML::Label> dstLabelConnection_end() const { return Udm::AssocEndAttr<PICML::Label>(impl, meta_dstLabelConnection_end_); }

		static Uml::AssociationRole meta_srcLabelConnection_end_;
		Udm::AssocEndAttr<PICML::Member> srcLabelConnection_end() const { return Udm::AssocEndAttr<PICML::Member>(impl, meta_srcLabelConnection_end_); }

	};

	class  MemberType :  virtual public MgaObject {
	 public:
		static Uml::Class meta;

		MemberType() { }
		MemberType(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		MemberType(const MemberType &master) : MgaObject(master) { }
		static MemberType Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static MemberType Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		MemberType CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		MemberType CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<MemberType> Instances() { return Udm::InstantiatedAttr<PICML::MemberType>(impl);}
		template <class Pred> Udm::InstantiatedAttr<MemberType, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::MemberType, Pred>(impl);}

		Udm::DerivedAttr<PICML::MemberType> Derived() { return Udm::DerivedAttr<PICML::MemberType>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::MemberType, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::MemberType, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::MemberType> Archetype() { return Udm::ArchetypeAttr<PICML::MemberType>(impl);}

		static Uml::AssociationRole meta_referedbyAlias;
		Udm::AssocAttr<PICML::Alias> referedbyAlias() const { return Udm::AssocAttr<PICML::Alias>(impl, meta_referedbyAlias); }
		template <class Pred> Udm::AssocAttr<PICML::Alias, Pred > referedbyAlias_sorted(const Pred &) const { return Udm::AssocAttr<PICML::Alias, Pred>(impl, meta_referedbyAlias); }

		static Uml::AssociationRole meta_referedbyCollection;
		Udm::AssocAttr<PICML::Collection> referedbyCollection() const { return Udm::AssocAttr<PICML::Collection>(impl, meta_referedbyCollection); }
		template <class Pred> Udm::AssocAttr<PICML::Collection, Pred > referedbyCollection_sorted(const Pred &) const { return Udm::AssocAttr<PICML::Collection, Pred>(impl, meta_referedbyCollection); }

		static Uml::AssociationRole meta_referedbyBoxed;
		Udm::AssocAttr<PICML::Boxed> referedbyBoxed() const { return Udm::AssocAttr<PICML::Boxed>(impl, meta_referedbyBoxed); }
		template <class Pred> Udm::AssocAttr<PICML::Boxed, Pred > referedbyBoxed_sorted(const Pred &) const { return Udm::AssocAttr<PICML::Boxed, Pred>(impl, meta_referedbyBoxed); }

		static Uml::AssociationRole meta_referedbyMember;
		Udm::AssocAttr<PICML::Member> referedbyMember() const { return Udm::AssocAttr<PICML::Member>(impl, meta_referedbyMember); }
		template <class Pred> Udm::AssocAttr<PICML::Member, Pred > referedbyMember_sorted(const Pred &) const { return Udm::AssocAttr<PICML::Member, Pred>(impl, meta_referedbyMember); }

		static Uml::AssociationRole meta_referedbyAttributeMember;
		Udm::AssocAttr<PICML::AttributeMember> referedbyAttributeMember() const { return Udm::AssocAttr<PICML::AttributeMember>(impl, meta_referedbyAttributeMember); }
		template <class Pred> Udm::AssocAttr<PICML::AttributeMember, Pred > referedbyAttributeMember_sorted(const Pred &) const { return Udm::AssocAttr<PICML::AttributeMember, Pred>(impl, meta_referedbyAttributeMember); }

		static Uml::AssociationRole meta_referedbyInParameter;
		Udm::AssocAttr<PICML::InParameter> referedbyInParameter() const { return Udm::AssocAttr<PICML::InParameter>(impl, meta_referedbyInParameter); }
		template <class Pred> Udm::AssocAttr<PICML::InParameter, Pred > referedbyInParameter_sorted(const Pred &) const { return Udm::AssocAttr<PICML::InParameter, Pred>(impl, meta_referedbyInParameter); }

		static Uml::AssociationRole meta_referedbyOutParameter;
		Udm::AssocAttr<PICML::OutParameter> referedbyOutParameter() const { return Udm::AssocAttr<PICML::OutParameter>(impl, meta_referedbyOutParameter); }
		template <class Pred> Udm::AssocAttr<PICML::OutParameter, Pred > referedbyOutParameter_sorted(const Pred &) const { return Udm::AssocAttr<PICML::OutParameter, Pred>(impl, meta_referedbyOutParameter); }

		static Uml::AssociationRole meta_referedbyReturnType;
		Udm::AssocAttr<PICML::ReturnType> referedbyReturnType() const { return Udm::AssocAttr<PICML::ReturnType>(impl, meta_referedbyReturnType); }
		template <class Pred> Udm::AssocAttr<PICML::ReturnType, Pred > referedbyReturnType_sorted(const Pred &) const { return Udm::AssocAttr<PICML::ReturnType, Pred>(impl, meta_referedbyReturnType); }

		static Uml::AssociationRole meta_referedbyInoutParameter;
		Udm::AssocAttr<PICML::InoutParameter> referedbyInoutParameter() const { return Udm::AssocAttr<PICML::InoutParameter>(impl, meta_referedbyInoutParameter); }
		template <class Pred> Udm::AssocAttr<PICML::InoutParameter, Pred > referedbyInoutParameter_sorted(const Pred &) const { return Udm::AssocAttr<PICML::InoutParameter, Pred>(impl, meta_referedbyInoutParameter); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class  PredefinedType :  public ConstantType, public MemberType {
	 public:
		static Uml::Class meta;

		PredefinedType() { }
		PredefinedType(Udm::ObjectImpl *impl) : ConstantType(impl),MemberType(impl), MgaObject(impl) { }
		PredefinedType(const PredefinedType &master) : ConstantType(master),MemberType(master), MgaObject(master) { }
		static PredefinedType Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PredefinedType Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PredefinedType CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PredefinedType CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PredefinedType> Instances() { return Udm::InstantiatedAttr<PICML::PredefinedType>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PredefinedType, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PredefinedType, Pred>(impl);}

		Udm::DerivedAttr<PICML::PredefinedType> Derived() { return Udm::DerivedAttr<PICML::PredefinedType>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PredefinedType, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PredefinedType, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PredefinedType> Archetype() { return Udm::ArchetypeAttr<PICML::PredefinedType>(impl);}

		static Uml::AssociationRole meta_referedbyDataType;
		Udm::AssocAttr<PICML::DataType> referedbyDataType() const { return Udm::AssocAttr<PICML::DataType>(impl, meta_referedbyDataType); }
		template <class Pred> Udm::AssocAttr<PICML::DataType, Pred > referedbyDataType_sorted(const Pred &) const { return Udm::AssocAttr<PICML::DataType, Pred>(impl, meta_referedbyDataType); }

		static Uml::CompositionParentRole meta_PredefinedTypes_parent;
		Udm::ParentAttr<PICML::PredefinedTypes> PredefinedTypes_parent() const { return Udm::ParentAttr<PICML::PredefinedTypes>(impl, meta_PredefinedTypes_parent); }

		Udm::ParentAttr<PICML::PredefinedTypes> parent() const { return Udm::ParentAttr<PICML::PredefinedTypes>(impl, Udm::NULLPARENTROLE); }
	};

	class  Byte :  public PredefinedType {
	 public:
		static Uml::Class meta;

		Byte() { }
		Byte(Udm::ObjectImpl *impl) : PredefinedType(impl), MgaObject(impl) { }
		Byte(const Byte &master) : PredefinedType(master), MgaObject(master) { }
		static Byte Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Byte Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Byte CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Byte CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Byte> Instances() { return Udm::InstantiatedAttr<PICML::Byte>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Byte, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Byte, Pred>(impl);}

		Udm::DerivedAttr<PICML::Byte> Derived() { return Udm::DerivedAttr<PICML::Byte>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Byte, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Byte, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Byte> Archetype() { return Udm::ArchetypeAttr<PICML::Byte>(impl);}

		Udm::ParentAttr<PICML::PredefinedTypes> parent() const { return Udm::ParentAttr<PICML::PredefinedTypes>(impl, Udm::NULLPARENTROLE); }
	};

	class  Boolean :  public PredefinedType {
	 public:
		static Uml::Class meta;

		Boolean() { }
		Boolean(Udm::ObjectImpl *impl) : PredefinedType(impl), MgaObject(impl) { }
		Boolean(const Boolean &master) : PredefinedType(master), MgaObject(master) { }
		static Boolean Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Boolean Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Boolean CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Boolean CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Boolean> Instances() { return Udm::InstantiatedAttr<PICML::Boolean>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Boolean, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Boolean, Pred>(impl);}

		Udm::DerivedAttr<PICML::Boolean> Derived() { return Udm::DerivedAttr<PICML::Boolean>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Boolean, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Boolean, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Boolean> Archetype() { return Udm::ArchetypeAttr<PICML::Boolean>(impl);}

		Udm::ParentAttr<PICML::PredefinedTypes> parent() const { return Udm::ParentAttr<PICML::PredefinedTypes>(impl, Udm::NULLPARENTROLE); }
	};

	class  ShortInteger :  public PredefinedType {
	 public:
		static Uml::Class meta;

		ShortInteger() { }
		ShortInteger(Udm::ObjectImpl *impl) : PredefinedType(impl), MgaObject(impl) { }
		ShortInteger(const ShortInteger &master) : PredefinedType(master), MgaObject(master) { }
		static ShortInteger Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ShortInteger Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ShortInteger CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ShortInteger CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ShortInteger> Instances() { return Udm::InstantiatedAttr<PICML::ShortInteger>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ShortInteger, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ShortInteger, Pred>(impl);}

		Udm::DerivedAttr<PICML::ShortInteger> Derived() { return Udm::DerivedAttr<PICML::ShortInteger>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ShortInteger, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ShortInteger, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ShortInteger> Archetype() { return Udm::ArchetypeAttr<PICML::ShortInteger>(impl);}

		Udm::ParentAttr<PICML::PredefinedTypes> parent() const { return Udm::ParentAttr<PICML::PredefinedTypes>(impl, Udm::NULLPARENTROLE); }
	};

	class  LongInteger :  public PredefinedType {
	 public:
		static Uml::Class meta;

		LongInteger() { }
		LongInteger(Udm::ObjectImpl *impl) : PredefinedType(impl), MgaObject(impl) { }
		LongInteger(const LongInteger &master) : PredefinedType(master), MgaObject(master) { }
		static LongInteger Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static LongInteger Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		LongInteger CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		LongInteger CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<LongInteger> Instances() { return Udm::InstantiatedAttr<PICML::LongInteger>(impl);}
		template <class Pred> Udm::InstantiatedAttr<LongInteger, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::LongInteger, Pred>(impl);}

		Udm::DerivedAttr<PICML::LongInteger> Derived() { return Udm::DerivedAttr<PICML::LongInteger>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::LongInteger, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::LongInteger, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::LongInteger> Archetype() { return Udm::ArchetypeAttr<PICML::LongInteger>(impl);}

		Udm::ParentAttr<PICML::PredefinedTypes> parent() const { return Udm::ParentAttr<PICML::PredefinedTypes>(impl, Udm::NULLPARENTROLE); }
	};

	class  RealNumber :  public PredefinedType {
	 public:
		static Uml::Class meta;

		RealNumber() { }
		RealNumber(Udm::ObjectImpl *impl) : PredefinedType(impl), MgaObject(impl) { }
		RealNumber(const RealNumber &master) : PredefinedType(master), MgaObject(master) { }
		static RealNumber Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static RealNumber Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		RealNumber CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		RealNumber CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<RealNumber> Instances() { return Udm::InstantiatedAttr<PICML::RealNumber>(impl);}
		template <class Pred> Udm::InstantiatedAttr<RealNumber, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::RealNumber, Pred>(impl);}

		Udm::DerivedAttr<PICML::RealNumber> Derived() { return Udm::DerivedAttr<PICML::RealNumber>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::RealNumber, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::RealNumber, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::RealNumber> Archetype() { return Udm::ArchetypeAttr<PICML::RealNumber>(impl);}

		Udm::ParentAttr<PICML::PredefinedTypes> parent() const { return Udm::ParentAttr<PICML::PredefinedTypes>(impl, Udm::NULLPARENTROLE); }
	};

	class  String :  public PredefinedType {
	 public:
		static Uml::Class meta;

		String() { }
		String(Udm::ObjectImpl *impl) : PredefinedType(impl), MgaObject(impl) { }
		String(const String &master) : PredefinedType(master), MgaObject(master) { }
		static String Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static String Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		String CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		String CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<String> Instances() { return Udm::InstantiatedAttr<PICML::String>(impl);}
		template <class Pred> Udm::InstantiatedAttr<String, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::String, Pred>(impl);}

		Udm::DerivedAttr<PICML::String> Derived() { return Udm::DerivedAttr<PICML::String>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::String, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::String, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::String> Archetype() { return Udm::ArchetypeAttr<PICML::String>(impl);}

		Udm::ParentAttr<PICML::PredefinedTypes> parent() const { return Udm::ParentAttr<PICML::PredefinedTypes>(impl, Udm::NULLPARENTROLE); }
	};

	class  GenericValueObject :  public PredefinedType {
	 public:
		static Uml::Class meta;

		GenericValueObject() { }
		GenericValueObject(Udm::ObjectImpl *impl) : PredefinedType(impl), MgaObject(impl) { }
		GenericValueObject(const GenericValueObject &master) : PredefinedType(master), MgaObject(master) { }
		static GenericValueObject Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static GenericValueObject Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		GenericValueObject CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		GenericValueObject CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<GenericValueObject> Instances() { return Udm::InstantiatedAttr<PICML::GenericValueObject>(impl);}
		template <class Pred> Udm::InstantiatedAttr<GenericValueObject, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::GenericValueObject, Pred>(impl);}

		Udm::DerivedAttr<PICML::GenericValueObject> Derived() { return Udm::DerivedAttr<PICML::GenericValueObject>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::GenericValueObject, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::GenericValueObject, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::GenericValueObject> Archetype() { return Udm::ArchetypeAttr<PICML::GenericValueObject>(impl);}

		Udm::ParentAttr<PICML::PredefinedTypes> parent() const { return Udm::ParentAttr<PICML::PredefinedTypes>(impl, Udm::NULLPARENTROLE); }
	};

	class  GenericValue :  public PredefinedType {
	 public:
		static Uml::Class meta;

		GenericValue() { }
		GenericValue(Udm::ObjectImpl *impl) : PredefinedType(impl), MgaObject(impl) { }
		GenericValue(const GenericValue &master) : PredefinedType(master), MgaObject(master) { }
		static GenericValue Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static GenericValue Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		GenericValue CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		GenericValue CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<GenericValue> Instances() { return Udm::InstantiatedAttr<PICML::GenericValue>(impl);}
		template <class Pred> Udm::InstantiatedAttr<GenericValue, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::GenericValue, Pred>(impl);}

		Udm::DerivedAttr<PICML::GenericValue> Derived() { return Udm::DerivedAttr<PICML::GenericValue>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::GenericValue, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::GenericValue, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::GenericValue> Archetype() { return Udm::ArchetypeAttr<PICML::GenericValue>(impl);}

		Udm::ParentAttr<PICML::PredefinedTypes> parent() const { return Udm::ParentAttr<PICML::PredefinedTypes>(impl, Udm::NULLPARENTROLE); }
	};

	class  TypeEncoding :  public PredefinedType {
	 public:
		static Uml::Class meta;

		TypeEncoding() { }
		TypeEncoding(Udm::ObjectImpl *impl) : PredefinedType(impl), MgaObject(impl) { }
		TypeEncoding(const TypeEncoding &master) : PredefinedType(master), MgaObject(master) { }
		static TypeEncoding Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static TypeEncoding Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		TypeEncoding CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		TypeEncoding CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<TypeEncoding> Instances() { return Udm::InstantiatedAttr<PICML::TypeEncoding>(impl);}
		template <class Pred> Udm::InstantiatedAttr<TypeEncoding, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::TypeEncoding, Pred>(impl);}

		Udm::DerivedAttr<PICML::TypeEncoding> Derived() { return Udm::DerivedAttr<PICML::TypeEncoding>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::TypeEncoding, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::TypeEncoding, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::TypeEncoding> Archetype() { return Udm::ArchetypeAttr<PICML::TypeEncoding>(impl);}

		Udm::ParentAttr<PICML::PredefinedTypes> parent() const { return Udm::ParentAttr<PICML::PredefinedTypes>(impl, Udm::NULLPARENTROLE); }
	};

	class  TypeKind :  public PredefinedType {
	 public:
		static Uml::Class meta;

		TypeKind() { }
		TypeKind(Udm::ObjectImpl *impl) : PredefinedType(impl), MgaObject(impl) { }
		TypeKind(const TypeKind &master) : PredefinedType(master), MgaObject(master) { }
		static TypeKind Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static TypeKind Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		TypeKind CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		TypeKind CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<TypeKind> Instances() { return Udm::InstantiatedAttr<PICML::TypeKind>(impl);}
		template <class Pred> Udm::InstantiatedAttr<TypeKind, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::TypeKind, Pred>(impl);}

		Udm::DerivedAttr<PICML::TypeKind> Derived() { return Udm::DerivedAttr<PICML::TypeKind>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::TypeKind, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::TypeKind, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::TypeKind> Archetype() { return Udm::ArchetypeAttr<PICML::TypeKind>(impl);}

		Udm::ParentAttr<PICML::PredefinedTypes> parent() const { return Udm::ParentAttr<PICML::PredefinedTypes>(impl, Udm::NULLPARENTROLE); }
	};

	class  Discriminator :  public MgaObject {
	 public:
		static Uml::Class meta;

		Discriminator() { }
		Discriminator(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Discriminator(const Discriminator &master) : MgaObject(master) { }
		static Discriminator Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Discriminator Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Discriminator CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Discriminator CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Discriminator> Instances() { return Udm::InstantiatedAttr<PICML::Discriminator>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Discriminator, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Discriminator, Pred>(impl);}

		Udm::DerivedAttr<PICML::Discriminator> Derived() { return Udm::DerivedAttr<PICML::Discriminator>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Discriminator, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Discriminator, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Discriminator> Archetype() { return Udm::ArchetypeAttr<PICML::Discriminator>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::ConstantType> ref() const { return Udm::PointerAttr<PICML::ConstantType>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_SwitchedAggregate_parent;
		Udm::ParentAttr<PICML::SwitchedAggregate> SwitchedAggregate_parent() const { return Udm::ParentAttr<PICML::SwitchedAggregate>(impl, meta_SwitchedAggregate_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Member :  public MgaObject {
	 public:
		static Uml::Class meta;

		Member() { }
		Member(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Member(const Member &master) : MgaObject(master) { }
		static Member Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Member Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Member CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Member CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Member> Instances() { return Udm::InstantiatedAttr<PICML::Member>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Member, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Member, Pred>(impl);}

		Udm::DerivedAttr<PICML::Member> Derived() { return Udm::DerivedAttr<PICML::Member>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Member, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Member, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Member> Archetype() { return Udm::ArchetypeAttr<PICML::Member>(impl);}

		static Uml::AssociationRole meta_dstLabelConnection, meta_dstLabelConnection_rev;
		Udm::AClassAssocAttr<PICML::LabelConnection, PICML::Label> dstLabelConnection() const { return Udm::AClassAssocAttr<PICML::LabelConnection, PICML::Label>(impl, meta_dstLabelConnection, meta_dstLabelConnection_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::LabelConnection, PICML::Label, Pred> dstLabelConnection_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::LabelConnection, PICML::Label, Pred>(impl, meta_dstLabelConnection, meta_dstLabelConnection_rev); }

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::MemberType> ref() const { return Udm::PointerAttr<PICML::MemberType>(impl, meta_ref); }

		static Uml::AssociationRole meta_dstMakeMemberPrivate, meta_dstMakeMemberPrivate_rev;
		Udm::AClassPointerAttr<PICML::MakeMemberPrivate, PICML::PrivateFlag> dstMakeMemberPrivate() const { return Udm::AClassPointerAttr<PICML::MakeMemberPrivate, PICML::PrivateFlag>(impl, meta_dstMakeMemberPrivate, meta_dstMakeMemberPrivate_rev); }

		static Uml::CompositionParentRole meta_Exception_parent;
		Udm::ParentAttr<PICML::Exception> Exception_parent() const { return Udm::ParentAttr<PICML::Exception>(impl, meta_Exception_parent); }

		static Uml::CompositionParentRole meta_Aggregate_parent;
		Udm::ParentAttr<PICML::Aggregate> Aggregate_parent() const { return Udm::ParentAttr<PICML::Aggregate>(impl, meta_Aggregate_parent); }

		static Uml::CompositionParentRole meta_SwitchedAggregate_parent;
		Udm::ParentAttr<PICML::SwitchedAggregate> SwitchedAggregate_parent() const { return Udm::ParentAttr<PICML::SwitchedAggregate>(impl, meta_SwitchedAggregate_parent); }

		static Uml::CompositionParentRole meta_ObjectByValue_parent;
		Udm::ParentAttr<PICML::ObjectByValue> ObjectByValue_parent() const { return Udm::ParentAttr<PICML::ObjectByValue>(impl, meta_ObjectByValue_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Inherits :  public MgaObject {
	 public:
		static Uml::Class meta;

		Inherits() { }
		Inherits(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Inherits(const Inherits &master) : MgaObject(master) { }
		static Inherits Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Inherits Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Inherits CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Inherits CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Inherits> Instances() { return Udm::InstantiatedAttr<PICML::Inherits>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Inherits, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Inherits, Pred>(impl);}

		Udm::DerivedAttr<PICML::Inherits> Derived() { return Udm::DerivedAttr<PICML::Inherits>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Inherits, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Inherits, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Inherits> Archetype() { return Udm::ArchetypeAttr<PICML::Inherits>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::Inheritable> ref() const { return Udm::PointerAttr<PICML::Inheritable>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_Inheritable_parent;
		Udm::ParentAttr<PICML::Inheritable> Inheritable_parent() const { return Udm::ParentAttr<PICML::Inheritable>(impl, meta_Inheritable_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  LookupKey :  public MgaObject {
	 public:
		static Uml::Class meta;

		LookupKey() { }
		LookupKey(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		LookupKey(const LookupKey &master) : MgaObject(master) { }
		static LookupKey Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static LookupKey Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		LookupKey CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		LookupKey CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<LookupKey> Instances() { return Udm::InstantiatedAttr<PICML::LookupKey>(impl);}
		template <class Pred> Udm::InstantiatedAttr<LookupKey, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::LookupKey, Pred>(impl);}

		Udm::DerivedAttr<PICML::LookupKey> Derived() { return Udm::DerivedAttr<PICML::LookupKey>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::LookupKey, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::LookupKey, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::LookupKey> Archetype() { return Udm::ArchetypeAttr<PICML::LookupKey>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::ValueObject> ref() const { return Udm::PointerAttr<PICML::ValueObject>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_ComponentFactory_parent;
		Udm::ParentAttr<PICML::ComponentFactory> ComponentFactory_parent() const { return Udm::ParentAttr<PICML::ComponentFactory>(impl, meta_ComponentFactory_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  SetException :  public MgaObject {
	 public:
		static Uml::Class meta;

		SetException() { }
		SetException(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		SetException(const SetException &master) : MgaObject(master) { }
		static SetException Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static SetException Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		SetException CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		SetException CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<SetException> Instances() { return Udm::InstantiatedAttr<PICML::SetException>(impl);}
		template <class Pred> Udm::InstantiatedAttr<SetException, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::SetException, Pred>(impl);}

		Udm::DerivedAttr<PICML::SetException> Derived() { return Udm::DerivedAttr<PICML::SetException>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::SetException, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::SetException, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::SetException> Archetype() { return Udm::ArchetypeAttr<PICML::SetException>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::Exception> ref() const { return Udm::PointerAttr<PICML::Exception>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_Attribute_parent;
		Udm::ParentAttr<PICML::Attribute> Attribute_parent() const { return Udm::ParentAttr<PICML::Attribute>(impl, meta_Attribute_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  GetException :  public MgaObject {
	 public:
		static Uml::Class meta;

		GetException() { }
		GetException(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		GetException(const GetException &master) : MgaObject(master) { }
		static GetException Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static GetException Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		GetException CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		GetException CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<GetException> Instances() { return Udm::InstantiatedAttr<PICML::GetException>(impl);}
		template <class Pred> Udm::InstantiatedAttr<GetException, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::GetException, Pred>(impl);}

		Udm::DerivedAttr<PICML::GetException> Derived() { return Udm::DerivedAttr<PICML::GetException>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::GetException, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::GetException, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::GetException> Archetype() { return Udm::ArchetypeAttr<PICML::GetException>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::Exception> ref() const { return Udm::PointerAttr<PICML::Exception>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_ReadonlyAttribute_parent;
		Udm::ParentAttr<PICML::ReadonlyAttribute> ReadonlyAttribute_parent() const { return Udm::ParentAttr<PICML::ReadonlyAttribute>(impl, meta_ReadonlyAttribute_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  PrivateFlag :  public MgaObject {
	 public:
		static Uml::Class meta;

		PrivateFlag() { }
		PrivateFlag(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		PrivateFlag(const PrivateFlag &master) : MgaObject(master) { }
		static PrivateFlag Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static PrivateFlag Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		PrivateFlag CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		PrivateFlag CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<PrivateFlag> Instances() { return Udm::InstantiatedAttr<PICML::PrivateFlag>(impl);}
		template <class Pred> Udm::InstantiatedAttr<PrivateFlag, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::PrivateFlag, Pred>(impl);}

		Udm::DerivedAttr<PICML::PrivateFlag> Derived() { return Udm::DerivedAttr<PICML::PrivateFlag>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::PrivateFlag, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::PrivateFlag, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::PrivateFlag> Archetype() { return Udm::ArchetypeAttr<PICML::PrivateFlag>(impl);}

		static Uml::AssociationRole meta_srcMakeMemberPrivate, meta_srcMakeMemberPrivate_rev;
		Udm::AClassPointerAttr<PICML::MakeMemberPrivate, PICML::Member> srcMakeMemberPrivate() const { return Udm::AClassPointerAttr<PICML::MakeMemberPrivate, PICML::Member>(impl, meta_srcMakeMemberPrivate, meta_srcMakeMemberPrivate_rev); }

		static Uml::CompositionParentRole meta_ObjectByValue_parent;
		Udm::ParentAttr<PICML::ObjectByValue> ObjectByValue_parent() const { return Udm::ParentAttr<PICML::ObjectByValue>(impl, meta_ObjectByValue_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  MakeMemberPrivate :  public MgaObject {
	 public:
		static Uml::Class meta;

		MakeMemberPrivate() { }
		MakeMemberPrivate(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		MakeMemberPrivate(const MakeMemberPrivate &master) : MgaObject(master) { }
		static MakeMemberPrivate Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static MakeMemberPrivate Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		MakeMemberPrivate CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		MakeMemberPrivate CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<MakeMemberPrivate> Instances() { return Udm::InstantiatedAttr<PICML::MakeMemberPrivate>(impl);}
		template <class Pred> Udm::InstantiatedAttr<MakeMemberPrivate, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::MakeMemberPrivate, Pred>(impl);}

		Udm::DerivedAttr<PICML::MakeMemberPrivate> Derived() { return Udm::DerivedAttr<PICML::MakeMemberPrivate>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::MakeMemberPrivate, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::MakeMemberPrivate, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::MakeMemberPrivate> Archetype() { return Udm::ArchetypeAttr<PICML::MakeMemberPrivate>(impl);}

		static Uml::CompositionParentRole meta_ObjectByValue_parent;
		Udm::ParentAttr<PICML::ObjectByValue> ObjectByValue_parent() const { return Udm::ParentAttr<PICML::ObjectByValue>(impl, meta_ObjectByValue_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_srcMakeMemberPrivate_end_;
		Udm::AssocEndAttr<PICML::Member> srcMakeMemberPrivate_end() const { return Udm::AssocEndAttr<PICML::Member>(impl, meta_srcMakeMemberPrivate_end_); }

		static Uml::AssociationRole meta_dstMakeMemberPrivate_end_;
		Udm::AssocEndAttr<PICML::PrivateFlag> dstMakeMemberPrivate_end() const { return Udm::AssocEndAttr<PICML::PrivateFlag>(impl, meta_dstMakeMemberPrivate_end_); }

	};

	class  AttributeMember :  public MgaObject {
	 public:
		static Uml::Class meta;

		AttributeMember() { }
		AttributeMember(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		AttributeMember(const AttributeMember &master) : MgaObject(master) { }
		static AttributeMember Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static AttributeMember Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		AttributeMember CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		AttributeMember CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<AttributeMember> Instances() { return Udm::InstantiatedAttr<PICML::AttributeMember>(impl);}
		template <class Pred> Udm::InstantiatedAttr<AttributeMember, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::AttributeMember, Pred>(impl);}

		Udm::DerivedAttr<PICML::AttributeMember> Derived() { return Udm::DerivedAttr<PICML::AttributeMember>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::AttributeMember, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::AttributeMember, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::AttributeMember> Archetype() { return Udm::ArchetypeAttr<PICML::AttributeMember>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::MemberType> ref() const { return Udm::PointerAttr<PICML::MemberType>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_ReadonlyAttribute_parent;
		Udm::ParentAttr<PICML::ReadonlyAttribute> ReadonlyAttribute_parent() const { return Udm::ParentAttr<PICML::ReadonlyAttribute>(impl, meta_ReadonlyAttribute_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Supports :  public MgaObject {
	 public:
		static Uml::Class meta;

		Supports() { }
		Supports(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Supports(const Supports &master) : MgaObject(master) { }
		static Supports Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Supports Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Supports CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Supports CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Supports> Instances() { return Udm::InstantiatedAttr<PICML::Supports>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Supports, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Supports, Pred>(impl);}

		Udm::DerivedAttr<PICML::Supports> Derived() { return Udm::DerivedAttr<PICML::Supports>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Supports, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Supports, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Supports> Archetype() { return Udm::ArchetypeAttr<PICML::Supports>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::Object> ref() const { return Udm::PointerAttr<PICML::Object>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_SupportsInterfaces_parent;
		Udm::ParentAttr<PICML::SupportsInterfaces> SupportsInterfaces_parent() const { return Udm::ParentAttr<PICML::SupportsInterfaces>(impl, meta_SupportsInterfaces_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  InParameter :  public MgaObject {
	 public:
		static Uml::Class meta;

		InParameter() { }
		InParameter(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		InParameter(const InParameter &master) : MgaObject(master) { }
		static InParameter Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static InParameter Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		InParameter CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		InParameter CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<InParameter> Instances() { return Udm::InstantiatedAttr<PICML::InParameter>(impl);}
		template <class Pred> Udm::InstantiatedAttr<InParameter, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::InParameter, Pred>(impl);}

		Udm::DerivedAttr<PICML::InParameter> Derived() { return Udm::DerivedAttr<PICML::InParameter>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::InParameter, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::InParameter, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::InParameter> Archetype() { return Udm::ArchetypeAttr<PICML::InParameter>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::MemberType> ref() const { return Udm::PointerAttr<PICML::MemberType>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_OperationBase_parent;
		Udm::ParentAttr<PICML::OperationBase> OperationBase_parent() const { return Udm::ParentAttr<PICML::OperationBase>(impl, meta_OperationBase_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  OutParameter :  public MgaObject {
	 public:
		static Uml::Class meta;

		OutParameter() { }
		OutParameter(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		OutParameter(const OutParameter &master) : MgaObject(master) { }
		static OutParameter Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static OutParameter Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		OutParameter CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		OutParameter CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<OutParameter> Instances() { return Udm::InstantiatedAttr<PICML::OutParameter>(impl);}
		template <class Pred> Udm::InstantiatedAttr<OutParameter, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::OutParameter, Pred>(impl);}

		Udm::DerivedAttr<PICML::OutParameter> Derived() { return Udm::DerivedAttr<PICML::OutParameter>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::OutParameter, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::OutParameter, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::OutParameter> Archetype() { return Udm::ArchetypeAttr<PICML::OutParameter>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::MemberType> ref() const { return Udm::PointerAttr<PICML::MemberType>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_TwowayOperation_parent;
		Udm::ParentAttr<PICML::TwowayOperation> TwowayOperation_parent() const { return Udm::ParentAttr<PICML::TwowayOperation>(impl, meta_TwowayOperation_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  ReturnType :  public MgaObject {
	 public:
		static Uml::Class meta;

		ReturnType() { }
		ReturnType(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ReturnType(const ReturnType &master) : MgaObject(master) { }
		static ReturnType Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ReturnType Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ReturnType CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ReturnType CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ReturnType> Instances() { return Udm::InstantiatedAttr<PICML::ReturnType>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ReturnType, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ReturnType, Pred>(impl);}

		Udm::DerivedAttr<PICML::ReturnType> Derived() { return Udm::DerivedAttr<PICML::ReturnType>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ReturnType, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ReturnType, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ReturnType> Archetype() { return Udm::ArchetypeAttr<PICML::ReturnType>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::MemberType> ref() const { return Udm::PointerAttr<PICML::MemberType>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_TwowayOperation_parent;
		Udm::ParentAttr<PICML::TwowayOperation> TwowayOperation_parent() const { return Udm::ParentAttr<PICML::TwowayOperation>(impl, meta_TwowayOperation_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  InoutParameter :  public MgaObject {
	 public:
		static Uml::Class meta;

		InoutParameter() { }
		InoutParameter(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		InoutParameter(const InoutParameter &master) : MgaObject(master) { }
		static InoutParameter Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static InoutParameter Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		InoutParameter CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		InoutParameter CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<InoutParameter> Instances() { return Udm::InstantiatedAttr<PICML::InoutParameter>(impl);}
		template <class Pred> Udm::InstantiatedAttr<InoutParameter, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::InoutParameter, Pred>(impl);}

		Udm::DerivedAttr<PICML::InoutParameter> Derived() { return Udm::DerivedAttr<PICML::InoutParameter>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::InoutParameter, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::InoutParameter, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::InoutParameter> Archetype() { return Udm::ArchetypeAttr<PICML::InoutParameter>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::MemberType> ref() const { return Udm::PointerAttr<PICML::MemberType>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_TwowayOperation_parent;
		Udm::ParentAttr<PICML::TwowayOperation> TwowayOperation_parent() const { return Udm::ParentAttr<PICML::TwowayOperation>(impl, meta_TwowayOperation_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Manageable :  virtual public MgaObject {
	 public:
		static Uml::Class meta;

		Manageable() { }
		Manageable(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Manageable(const Manageable &master) : MgaObject(master) { }
		static Manageable Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Manageable Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Manageable CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Manageable CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Manageable> Instances() { return Udm::InstantiatedAttr<PICML::Manageable>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Manageable, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Manageable, Pred>(impl);}

		Udm::DerivedAttr<PICML::Manageable> Derived() { return Udm::DerivedAttr<PICML::Manageable>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Manageable, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Manageable, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Manageable> Archetype() { return Udm::ArchetypeAttr<PICML::Manageable>(impl);}

		static Uml::AssociationRole meta_srcManagesComponent, meta_srcManagesComponent_rev;
		Udm::AClassAssocAttr<PICML::ManagesComponent, PICML::ComponentFactory> srcManagesComponent() const { return Udm::AClassAssocAttr<PICML::ManagesComponent, PICML::ComponentFactory>(impl, meta_srcManagesComponent, meta_srcManagesComponent_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::ManagesComponent, PICML::ComponentFactory, Pred> srcManagesComponent_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::ManagesComponent, PICML::ComponentFactory, Pred>(impl, meta_srcManagesComponent, meta_srcManagesComponent_rev); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class  ComponentRef :  public Manageable {
	 public:
		static Uml::Class meta;

		ComponentRef() { }
		ComponentRef(Udm::ObjectImpl *impl) : Manageable(impl), MgaObject(impl) { }
		ComponentRef(const ComponentRef &master) : Manageable(master), MgaObject(master) { }
		static ComponentRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentRef Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentRef CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentRef CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentRef> Instances() { return Udm::InstantiatedAttr<PICML::ComponentRef>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentRef, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentRef> Derived() { return Udm::DerivedAttr<PICML::ComponentRef>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentRef, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentRef> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentRef>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::Component> ref() const { return Udm::PointerAttr<PICML::Component>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_Package_parent;
		Udm::ParentAttr<PICML::Package> Package_parent() const { return Udm::ParentAttr<PICML::Package>(impl, meta_Package_parent); }

		static Uml::CompositionParentRole meta_File_parent;
		Udm::ParentAttr<PICML::File> File_parent() const { return Udm::ParentAttr<PICML::File>(impl, meta_File_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  ManagesComponent :  public MgaObject {
	 public:
		static Uml::Class meta;

		ManagesComponent() { }
		ManagesComponent(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		ManagesComponent(const ManagesComponent &master) : MgaObject(master) { }
		static ManagesComponent Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ManagesComponent Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ManagesComponent CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ManagesComponent CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ManagesComponent> Instances() { return Udm::InstantiatedAttr<PICML::ManagesComponent>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ManagesComponent, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ManagesComponent, Pred>(impl);}

		Udm::DerivedAttr<PICML::ManagesComponent> Derived() { return Udm::DerivedAttr<PICML::ManagesComponent>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ManagesComponent, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ManagesComponent, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ManagesComponent> Archetype() { return Udm::ArchetypeAttr<PICML::ManagesComponent>(impl);}

		static Uml::CompositionParentRole meta_Package_parent;
		Udm::ParentAttr<PICML::Package> Package_parent() const { return Udm::ParentAttr<PICML::Package>(impl, meta_Package_parent); }

		static Uml::CompositionParentRole meta_File_parent;
		Udm::ParentAttr<PICML::File> File_parent() const { return Udm::ParentAttr<PICML::File>(impl, meta_File_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		static Uml::AssociationRole meta_dstManagesComponent_end_;
		Udm::AssocEndAttr<PICML::Manageable> dstManagesComponent_end() const { return Udm::AssocEndAttr<PICML::Manageable>(impl, meta_dstManagesComponent_end_); }

		static Uml::AssociationRole meta_srcManagesComponent_end_;
		Udm::AssocEndAttr<PICML::ComponentFactory> srcManagesComponent_end() const { return Udm::AssocEndAttr<PICML::ComponentFactory>(impl, meta_srcManagesComponent_end_); }

	};

	class  Provideable :  virtual public MgaObject {
	 public:
		static Uml::Class meta;

		Provideable() { }
		Provideable(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Provideable(const Provideable &master) : MgaObject(master) { }
		static Provideable Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Provideable Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Provideable CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Provideable CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Provideable> Instances() { return Udm::InstantiatedAttr<PICML::Provideable>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Provideable, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Provideable, Pred>(impl);}

		Udm::DerivedAttr<PICML::Provideable> Derived() { return Udm::DerivedAttr<PICML::Provideable>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Provideable, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Provideable, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Provideable> Archetype() { return Udm::ArchetypeAttr<PICML::Provideable>(impl);}

		static Uml::AssociationRole meta_referedbyProvidedRequestPort;
		Udm::AssocAttr<PICML::ProvidedRequestPort> referedbyProvidedRequestPort() const { return Udm::AssocAttr<PICML::ProvidedRequestPort>(impl, meta_referedbyProvidedRequestPort); }
		template <class Pred> Udm::AssocAttr<PICML::ProvidedRequestPort, Pred > referedbyProvidedRequestPort_sorted(const Pred &) const { return Udm::AssocAttr<PICML::ProvidedRequestPort, Pred>(impl, meta_referedbyProvidedRequestPort); }

		static Uml::AssociationRole meta_referedbyRequiredRequestPort;
		Udm::AssocAttr<PICML::RequiredRequestPort> referedbyRequiredRequestPort() const { return Udm::AssocAttr<PICML::RequiredRequestPort>(impl, meta_referedbyRequiredRequestPort); }
		template <class Pred> Udm::AssocAttr<PICML::RequiredRequestPort, Pred > referedbyRequiredRequestPort_sorted(const Pred &) const { return Udm::AssocAttr<PICML::RequiredRequestPort, Pred>(impl, meta_referedbyRequiredRequestPort); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class  GenericObject :  public PredefinedType, public Provideable {
	 public:
		static Uml::Class meta;

		GenericObject() { }
		GenericObject(Udm::ObjectImpl *impl) : PredefinedType(impl),Provideable(impl), MgaObject(impl) { }
		GenericObject(const GenericObject &master) : PredefinedType(master),Provideable(master), MgaObject(master) { }
		static GenericObject Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static GenericObject Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		GenericObject CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		GenericObject CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<GenericObject> Instances() { return Udm::InstantiatedAttr<PICML::GenericObject>(impl);}
		template <class Pred> Udm::InstantiatedAttr<GenericObject, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::GenericObject, Pred>(impl);}

		Udm::DerivedAttr<PICML::GenericObject> Derived() { return Udm::DerivedAttr<PICML::GenericObject>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::GenericObject, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::GenericObject, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::GenericObject> Archetype() { return Udm::ArchetypeAttr<PICML::GenericObject>(impl);}

		Udm::ParentAttr<PICML::PredefinedTypes> parent() const { return Udm::ParentAttr<PICML::PredefinedTypes>(impl, Udm::NULLPARENTROLE); }
	};

	class  Prefixable :  virtual public MgaObject {
	 public:
		static Uml::Class meta;

		Prefixable() { }
		Prefixable(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Prefixable(const Prefixable &master) : MgaObject(master) { }
		static Prefixable Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Prefixable Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Prefixable CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Prefixable CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Prefixable> Instances() { return Udm::InstantiatedAttr<PICML::Prefixable>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Prefixable, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Prefixable, Pred>(impl);}

		Udm::DerivedAttr<PICML::Prefixable> Derived() { return Udm::DerivedAttr<PICML::Prefixable>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Prefixable, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Prefixable, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Prefixable> Archetype() { return Udm::ArchetypeAttr<PICML::Prefixable>(impl);}

		static Uml::Attribute meta_PrefixTag;
		Udm::StringAttr PrefixTag() const { return Udm::StringAttr(impl, meta_PrefixTag); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class  File :  public Prefixable {
	 public:
		static Uml::Class meta;

		File() { }
		File(Udm::ObjectImpl *impl) : Prefixable(impl), MgaObject(impl) { }
		File(const File &master) : Prefixable(master), MgaObject(master) { }
		static File Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static File Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		File CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		File CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<File> Instances() { return Udm::InstantiatedAttr<PICML::File>(impl);}
		template <class Pred> Udm::InstantiatedAttr<File, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::File, Pred>(impl);}

		Udm::DerivedAttr<PICML::File> Derived() { return Udm::DerivedAttr<PICML::File>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::File, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::File, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::File> Archetype() { return Udm::ArchetypeAttr<PICML::File>(impl);}

		static Uml::AssociationRole meta_referedbyFileRef;
		Udm::AssocAttr<PICML::FileRef> referedbyFileRef() const { return Udm::AssocAttr<PICML::FileRef>(impl, meta_referedbyFileRef); }
		template <class Pred> Udm::AssocAttr<PICML::FileRef, Pred > referedbyFileRef_sorted(const Pred &) const { return Udm::AssocAttr<PICML::FileRef, Pred>(impl, meta_referedbyFileRef); }

		static Uml::CompositionChildRole meta_Exception_children;
		Udm::ChildrenAttr<PICML::Exception> Exception_children() const { return Udm::ChildrenAttr<PICML::Exception>(impl, meta_Exception_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Exception, Pred> Exception_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Exception, Pred>(impl, meta_Exception_children); }

		static Uml::CompositionChildRole meta_Package_children;
		Udm::ChildrenAttr<PICML::Package> Package_children() const { return Udm::ChildrenAttr<PICML::Package>(impl, meta_Package_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Package, Pred> Package_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Package, Pred>(impl, meta_Package_children); }

		static Uml::CompositionChildRole meta_FileRef_children;
		Udm::ChildrenAttr<PICML::FileRef> FileRef_children() const { return Udm::ChildrenAttr<PICML::FileRef>(impl, meta_FileRef_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::FileRef, Pred> FileRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::FileRef, Pred>(impl, meta_FileRef_children); }

		static Uml::CompositionChildRole meta_Constant_children;
		Udm::ChildrenAttr<PICML::Constant> Constant_children() const { return Udm::ChildrenAttr<PICML::Constant>(impl, meta_Constant_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Constant, Pred> Constant_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Constant, Pred>(impl, meta_Constant_children); }

		static Uml::CompositionChildRole meta_NamedType_children;
		Udm::ChildrenAttr<PICML::NamedType> NamedType_children() const { return Udm::ChildrenAttr<PICML::NamedType>(impl, meta_NamedType_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::NamedType, Pred> NamedType_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::NamedType, Pred>(impl, meta_NamedType_children); }

		static Uml::CompositionChildRole meta_ComponentRef_children;
		Udm::ChildrenAttr<PICML::ComponentRef> ComponentRef_children() const { return Udm::ChildrenAttr<PICML::ComponentRef>(impl, meta_ComponentRef_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentRef, Pred> ComponentRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentRef, Pred>(impl, meta_ComponentRef_children); }

		static Uml::CompositionChildRole meta_ManagesComponent_children;
		Udm::ChildrenAttr<PICML::ManagesComponent> ManagesComponent_children() const { return Udm::ChildrenAttr<PICML::ManagesComponent>(impl, meta_ManagesComponent_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ManagesComponent, Pred> ManagesComponent_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ManagesComponent, Pred>(impl, meta_ManagesComponent_children); }

		Udm::ChildrenAttr<PICML::Package> Package_kind_children() const { return Udm::ChildrenAttr<PICML::Package>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Package, Pred> Package_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Package, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Exception> Exception_kind_children() const { return Udm::ChildrenAttr<PICML::Exception>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Exception, Pred> Exception_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Exception, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::FileRef> FileRef_kind_children() const { return Udm::ChildrenAttr<PICML::FileRef>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::FileRef, Pred> FileRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::FileRef, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Constant> Constant_kind_children() const { return Udm::ChildrenAttr<PICML::Constant>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Constant, Pred> Constant_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Constant, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Enum> Enum_kind_children() const { return Udm::ChildrenAttr<PICML::Enum>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Enum, Pred> Enum_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Enum, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Aggregate> Aggregate_kind_children() const { return Udm::ChildrenAttr<PICML::Aggregate>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Aggregate, Pred> Aggregate_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Aggregate, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::SwitchedAggregate> SwitchedAggregate_kind_children() const { return Udm::ChildrenAttr<PICML::SwitchedAggregate>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::SwitchedAggregate, Pred> SwitchedAggregate_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::SwitchedAggregate, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MemberType> MemberType_kind_children() const { return Udm::ChildrenAttr<PICML::MemberType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MemberType, Pred> MemberType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MemberType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::NamedType> NamedType_kind_children() const { return Udm::ChildrenAttr<PICML::NamedType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::NamedType, Pred> NamedType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::NamedType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Collection> Collection_kind_children() const { return Udm::ChildrenAttr<PICML::Collection>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Collection, Pred> Collection_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Collection, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Alias> Alias_kind_children() const { return Udm::ChildrenAttr<PICML::Alias>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Alias, Pred> Alias_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Alias, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Boxed> Boxed_kind_children() const { return Udm::ChildrenAttr<PICML::Boxed>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Boxed, Pred> Boxed_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Boxed, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::NoInheritable> NoInheritable_kind_children() const { return Udm::ChildrenAttr<PICML::NoInheritable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::NoInheritable, Pred> NoInheritable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::NoInheritable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Object> Object_kind_children() const { return Udm::ChildrenAttr<PICML::Object>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Object, Pred> Object_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Object, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Event> Event_kind_children() const { return Udm::ChildrenAttr<PICML::Event>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Event, Pred> Event_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Event, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ValueObject> ValueObject_kind_children() const { return Udm::ChildrenAttr<PICML::ValueObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ValueObject, Pred> ValueObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ValueObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Inheritable> Inheritable_kind_children() const { return Udm::ChildrenAttr<PICML::Inheritable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Inheritable, Pred> Inheritable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Inheritable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::HasOperations> HasOperations_kind_children() const { return Udm::ChildrenAttr<PICML::HasOperations>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::HasOperations, Pred> HasOperations_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::HasOperations, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::SupportsInterfaces> SupportsInterfaces_kind_children() const { return Udm::ChildrenAttr<PICML::SupportsInterfaces>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::SupportsInterfaces, Pred> SupportsInterfaces_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::SupportsInterfaces, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ObjectByValue> ObjectByValue_kind_children() const { return Udm::ChildrenAttr<PICML::ObjectByValue>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ObjectByValue, Pred> ObjectByValue_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ObjectByValue, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Component> Component_kind_children() const { return Udm::ChildrenAttr<PICML::Component>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Component, Pred> Component_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Component, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentRef> ComponentRef_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentRef>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentRef, Pred> ComponentRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentRef, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Manageable> Manageable_kind_children() const { return Udm::ChildrenAttr<PICML::Manageable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Manageable, Pred> Manageable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Manageable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentFactory> ComponentFactory_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentFactory>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentFactory, Pred> ComponentFactory_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentFactory, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ManagesComponent> ManagesComponent_kind_children() const { return Udm::ChildrenAttr<PICML::ManagesComponent>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ManagesComponent, Pred> ManagesComponent_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ManagesComponent, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Prefixable> Prefixable_kind_children() const { return Udm::ChildrenAttr<PICML::Prefixable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Prefixable, Pred> Prefixable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Prefixable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Taggable> Taggable_kind_children() const { return Udm::ChildrenAttr<PICML::Taggable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Taggable, Pred> Taggable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Taggable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_InterfaceDefinitions_parent;
		Udm::ParentAttr<PICML::InterfaceDefinitions> InterfaceDefinitions_parent() const { return Udm::ParentAttr<PICML::InterfaceDefinitions>(impl, meta_InterfaceDefinitions_parent); }

		Udm::ParentAttr<PICML::InterfaceDefinitions> parent() const { return Udm::ParentAttr<PICML::InterfaceDefinitions>(impl, Udm::NULLPARENTROLE); }
	};

	class  Taggable :  virtual public MgaObject {
	 public:
		static Uml::Class meta;

		Taggable() { }
		Taggable(Udm::ObjectImpl *impl) : MgaObject(impl) { }
		Taggable(const Taggable &master) : MgaObject(master) { }
		static Taggable Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Taggable Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Taggable CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Taggable CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Taggable> Instances() { return Udm::InstantiatedAttr<PICML::Taggable>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Taggable, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Taggable, Pred>(impl);}

		Udm::DerivedAttr<PICML::Taggable> Derived() { return Udm::DerivedAttr<PICML::Taggable>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Taggable, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Taggable, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Taggable> Archetype() { return Udm::ArchetypeAttr<PICML::Taggable>(impl);}

		static Uml::Attribute meta_VersionTag;
		Udm::StringAttr VersionTag() const { return Udm::StringAttr(impl, meta_VersionTag); }

		static Uml::Attribute meta_SpecifyIdTag;
		Udm::StringAttr SpecifyIdTag() const { return Udm::StringAttr(impl, meta_SpecifyIdTag); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class  Package :  public Prefixable, public Taggable {
	 public:
		static Uml::Class meta;

		Package() { }
		Package(Udm::ObjectImpl *impl) : Prefixable(impl),Taggable(impl), MgaObject(impl) { }
		Package(const Package &master) : Prefixable(master),Taggable(master), MgaObject(master) { }
		static Package Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Package Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Package CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Package CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Package> Instances() { return Udm::InstantiatedAttr<PICML::Package>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Package, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Package, Pred>(impl);}

		Udm::DerivedAttr<PICML::Package> Derived() { return Udm::DerivedAttr<PICML::Package>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Package, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Package, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Package> Archetype() { return Udm::ArchetypeAttr<PICML::Package>(impl);}

		static Uml::CompositionChildRole meta_Exception_children;
		Udm::ChildrenAttr<PICML::Exception> Exception_children() const { return Udm::ChildrenAttr<PICML::Exception>(impl, meta_Exception_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Exception, Pred> Exception_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Exception, Pred>(impl, meta_Exception_children); }

		static Uml::CompositionChildRole meta_Package_children;
		Udm::ChildrenAttr<PICML::Package> Package_children() const { return Udm::ChildrenAttr<PICML::Package>(impl, meta_Package_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Package, Pred> Package_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Package, Pred>(impl, meta_Package_children); }

		static Uml::CompositionChildRole meta_Constant_children;
		Udm::ChildrenAttr<PICML::Constant> Constant_children() const { return Udm::ChildrenAttr<PICML::Constant>(impl, meta_Constant_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Constant, Pred> Constant_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Constant, Pred>(impl, meta_Constant_children); }

		static Uml::CompositionChildRole meta_NamedType_children;
		Udm::ChildrenAttr<PICML::NamedType> NamedType_children() const { return Udm::ChildrenAttr<PICML::NamedType>(impl, meta_NamedType_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::NamedType, Pred> NamedType_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::NamedType, Pred>(impl, meta_NamedType_children); }

		static Uml::CompositionChildRole meta_ComponentRef_children;
		Udm::ChildrenAttr<PICML::ComponentRef> ComponentRef_children() const { return Udm::ChildrenAttr<PICML::ComponentRef>(impl, meta_ComponentRef_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ComponentRef, Pred> ComponentRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentRef, Pred>(impl, meta_ComponentRef_children); }

		static Uml::CompositionChildRole meta_ManagesComponent_children;
		Udm::ChildrenAttr<PICML::ManagesComponent> ManagesComponent_children() const { return Udm::ChildrenAttr<PICML::ManagesComponent>(impl, meta_ManagesComponent_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ManagesComponent, Pred> ManagesComponent_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ManagesComponent, Pred>(impl, meta_ManagesComponent_children); }

		Udm::ChildrenAttr<PICML::Package> Package_kind_children() const { return Udm::ChildrenAttr<PICML::Package>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Package, Pred> Package_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Package, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Exception> Exception_kind_children() const { return Udm::ChildrenAttr<PICML::Exception>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Exception, Pred> Exception_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Exception, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Constant> Constant_kind_children() const { return Udm::ChildrenAttr<PICML::Constant>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Constant, Pred> Constant_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Constant, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Enum> Enum_kind_children() const { return Udm::ChildrenAttr<PICML::Enum>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Enum, Pred> Enum_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Enum, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Aggregate> Aggregate_kind_children() const { return Udm::ChildrenAttr<PICML::Aggregate>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Aggregate, Pred> Aggregate_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Aggregate, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::SwitchedAggregate> SwitchedAggregate_kind_children() const { return Udm::ChildrenAttr<PICML::SwitchedAggregate>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::SwitchedAggregate, Pred> SwitchedAggregate_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::SwitchedAggregate, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MemberType> MemberType_kind_children() const { return Udm::ChildrenAttr<PICML::MemberType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MemberType, Pred> MemberType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MemberType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::NamedType> NamedType_kind_children() const { return Udm::ChildrenAttr<PICML::NamedType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::NamedType, Pred> NamedType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::NamedType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Collection> Collection_kind_children() const { return Udm::ChildrenAttr<PICML::Collection>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Collection, Pred> Collection_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Collection, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Alias> Alias_kind_children() const { return Udm::ChildrenAttr<PICML::Alias>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Alias, Pred> Alias_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Alias, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Boxed> Boxed_kind_children() const { return Udm::ChildrenAttr<PICML::Boxed>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Boxed, Pred> Boxed_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Boxed, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::NoInheritable> NoInheritable_kind_children() const { return Udm::ChildrenAttr<PICML::NoInheritable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::NoInheritable, Pred> NoInheritable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::NoInheritable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Object> Object_kind_children() const { return Udm::ChildrenAttr<PICML::Object>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Object, Pred> Object_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Object, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Event> Event_kind_children() const { return Udm::ChildrenAttr<PICML::Event>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Event, Pred> Event_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Event, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ValueObject> ValueObject_kind_children() const { return Udm::ChildrenAttr<PICML::ValueObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ValueObject, Pred> ValueObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ValueObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Inheritable> Inheritable_kind_children() const { return Udm::ChildrenAttr<PICML::Inheritable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Inheritable, Pred> Inheritable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Inheritable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::HasOperations> HasOperations_kind_children() const { return Udm::ChildrenAttr<PICML::HasOperations>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::HasOperations, Pred> HasOperations_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::HasOperations, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::SupportsInterfaces> SupportsInterfaces_kind_children() const { return Udm::ChildrenAttr<PICML::SupportsInterfaces>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::SupportsInterfaces, Pred> SupportsInterfaces_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::SupportsInterfaces, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ObjectByValue> ObjectByValue_kind_children() const { return Udm::ChildrenAttr<PICML::ObjectByValue>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ObjectByValue, Pred> ObjectByValue_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ObjectByValue, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Component> Component_kind_children() const { return Udm::ChildrenAttr<PICML::Component>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Component, Pred> Component_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Component, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentRef> ComponentRef_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentRef>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentRef, Pred> ComponentRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentRef, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Manageable> Manageable_kind_children() const { return Udm::ChildrenAttr<PICML::Manageable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Manageable, Pred> Manageable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Manageable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ComponentFactory> ComponentFactory_kind_children() const { return Udm::ChildrenAttr<PICML::ComponentFactory>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ComponentFactory, Pred> ComponentFactory_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ComponentFactory, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ManagesComponent> ManagesComponent_kind_children() const { return Udm::ChildrenAttr<PICML::ManagesComponent>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ManagesComponent, Pred> ManagesComponent_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ManagesComponent, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Prefixable> Prefixable_kind_children() const { return Udm::ChildrenAttr<PICML::Prefixable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Prefixable, Pred> Prefixable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Prefixable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Taggable> Taggable_kind_children() const { return Udm::ChildrenAttr<PICML::Taggable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Taggable, Pred> Taggable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Taggable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_Package_parent;
		Udm::ParentAttr<PICML::Package> Package_parent() const { return Udm::ParentAttr<PICML::Package>(impl, meta_Package_parent); }

		static Uml::CompositionParentRole meta_File_parent;
		Udm::ParentAttr<PICML::File> File_parent() const { return Udm::ParentAttr<PICML::File>(impl, meta_File_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Exception :  public Taggable {
	 public:
		static Uml::Class meta;

		Exception() { }
		Exception(Udm::ObjectImpl *impl) : Taggable(impl), MgaObject(impl) { }
		Exception(const Exception &master) : Taggable(master), MgaObject(master) { }
		static Exception Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Exception Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Exception CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Exception CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Exception> Instances() { return Udm::InstantiatedAttr<PICML::Exception>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Exception, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Exception, Pred>(impl);}

		Udm::DerivedAttr<PICML::Exception> Derived() { return Udm::DerivedAttr<PICML::Exception>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Exception, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Exception, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Exception> Archetype() { return Udm::ArchetypeAttr<PICML::Exception>(impl);}

		static Uml::AssociationRole meta_referedbySetException;
		Udm::AssocAttr<PICML::SetException> referedbySetException() const { return Udm::AssocAttr<PICML::SetException>(impl, meta_referedbySetException); }
		template <class Pred> Udm::AssocAttr<PICML::SetException, Pred > referedbySetException_sorted(const Pred &) const { return Udm::AssocAttr<PICML::SetException, Pred>(impl, meta_referedbySetException); }

		static Uml::AssociationRole meta_referedbyExceptionRef;
		Udm::AssocAttr<PICML::ExceptionRef> referedbyExceptionRef() const { return Udm::AssocAttr<PICML::ExceptionRef>(impl, meta_referedbyExceptionRef); }
		template <class Pred> Udm::AssocAttr<PICML::ExceptionRef, Pred > referedbyExceptionRef_sorted(const Pred &) const { return Udm::AssocAttr<PICML::ExceptionRef, Pred>(impl, meta_referedbyExceptionRef); }

		static Uml::AssociationRole meta_referedbyGetException;
		Udm::AssocAttr<PICML::GetException> referedbyGetException() const { return Udm::AssocAttr<PICML::GetException>(impl, meta_referedbyGetException); }
		template <class Pred> Udm::AssocAttr<PICML::GetException, Pred > referedbyGetException_sorted(const Pred &) const { return Udm::AssocAttr<PICML::GetException, Pred>(impl, meta_referedbyGetException); }

		static Uml::CompositionChildRole meta_Member_children;
		Udm::ChildrenAttr<PICML::Member> Member_children() const { return Udm::ChildrenAttr<PICML::Member>(impl, meta_Member_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Member, Pred> Member_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Member, Pred>(impl, meta_Member_children); }

		Udm::ChildrenAttr<PICML::Member> Member_kind_children() const { return Udm::ChildrenAttr<PICML::Member>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Member, Pred> Member_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Member, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_Package_parent;
		Udm::ParentAttr<PICML::Package> Package_parent() const { return Udm::ParentAttr<PICML::Package>(impl, meta_Package_parent); }

		static Uml::CompositionParentRole meta_File_parent;
		Udm::ParentAttr<PICML::File> File_parent() const { return Udm::ParentAttr<PICML::File>(impl, meta_File_parent); }

		static Uml::CompositionParentRole meta_HasOperations_parent;
		Udm::ParentAttr<PICML::HasOperations> HasOperations_parent() const { return Udm::ParentAttr<PICML::HasOperations>(impl, meta_HasOperations_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Constant :  public Taggable {
	 public:
		static Uml::Class meta;

		Constant() { }
		Constant(Udm::ObjectImpl *impl) : Taggable(impl), MgaObject(impl) { }
		Constant(const Constant &master) : Taggable(master), MgaObject(master) { }
		static Constant Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Constant Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Constant CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Constant CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Constant> Instances() { return Udm::InstantiatedAttr<PICML::Constant>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Constant, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Constant, Pred>(impl);}

		Udm::DerivedAttr<PICML::Constant> Derived() { return Udm::DerivedAttr<PICML::Constant>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Constant, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Constant, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Constant> Archetype() { return Udm::ArchetypeAttr<PICML::Constant>(impl);}

		static Uml::Attribute meta_value;
		Udm::StringAttr value() const { return Udm::StringAttr(impl, meta_value); }

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::ConstantType> ref() const { return Udm::PointerAttr<PICML::ConstantType>(impl, meta_ref); }

		static Uml::CompositionParentRole meta_Package_parent;
		Udm::ParentAttr<PICML::Package> Package_parent() const { return Udm::ParentAttr<PICML::Package>(impl, meta_Package_parent); }

		static Uml::CompositionParentRole meta_File_parent;
		Udm::ParentAttr<PICML::File> File_parent() const { return Udm::ParentAttr<PICML::File>(impl, meta_File_parent); }

		static Uml::CompositionParentRole meta_HasOperations_parent;
		Udm::ParentAttr<PICML::HasOperations> HasOperations_parent() const { return Udm::ParentAttr<PICML::HasOperations>(impl, meta_HasOperations_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  NamedType :  public MemberType, public Taggable {
	 public:
		static Uml::Class meta;

		NamedType() { }
		NamedType(Udm::ObjectImpl *impl) : MemberType(impl),Taggable(impl), MgaObject(impl) { }
		NamedType(const NamedType &master) : MemberType(master),Taggable(master), MgaObject(master) { }
		static NamedType Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static NamedType Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		NamedType CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		NamedType CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<NamedType> Instances() { return Udm::InstantiatedAttr<PICML::NamedType>(impl);}
		template <class Pred> Udm::InstantiatedAttr<NamedType, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::NamedType, Pred>(impl);}

		Udm::DerivedAttr<PICML::NamedType> Derived() { return Udm::DerivedAttr<PICML::NamedType>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::NamedType, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::NamedType, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::NamedType> Archetype() { return Udm::ArchetypeAttr<PICML::NamedType>(impl);}

		static Uml::CompositionParentRole meta_Package_parent;
		Udm::ParentAttr<PICML::Package> Package_parent() const { return Udm::ParentAttr<PICML::Package>(impl, meta_Package_parent); }

		static Uml::CompositionParentRole meta_File_parent;
		Udm::ParentAttr<PICML::File> File_parent() const { return Udm::ParentAttr<PICML::File>(impl, meta_File_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Boxed :  public NamedType {
	 public:
		static Uml::Class meta;

		Boxed() { }
		Boxed(Udm::ObjectImpl *impl) : NamedType(impl), MgaObject(impl) { }
		Boxed(const Boxed &master) : NamedType(master), MgaObject(master) { }
		static Boxed Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Boxed Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Boxed CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Boxed CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Boxed> Instances() { return Udm::InstantiatedAttr<PICML::Boxed>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Boxed, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Boxed, Pred>(impl);}

		Udm::DerivedAttr<PICML::Boxed> Derived() { return Udm::DerivedAttr<PICML::Boxed>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Boxed, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Boxed, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Boxed> Archetype() { return Udm::ArchetypeAttr<PICML::Boxed>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::MemberType> ref() const { return Udm::PointerAttr<PICML::MemberType>(impl, meta_ref); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  NoInheritable :  public NamedType {
	 public:
		static Uml::Class meta;

		NoInheritable() { }
		NoInheritable(Udm::ObjectImpl *impl) : NamedType(impl), MgaObject(impl) { }
		NoInheritable(const NoInheritable &master) : NamedType(master), MgaObject(master) { }
		static NoInheritable Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static NoInheritable Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		NoInheritable CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		NoInheritable CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<NoInheritable> Instances() { return Udm::InstantiatedAttr<PICML::NoInheritable>(impl);}
		template <class Pred> Udm::InstantiatedAttr<NoInheritable, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::NoInheritable, Pred>(impl);}

		Udm::DerivedAttr<PICML::NoInheritable> Derived() { return Udm::DerivedAttr<PICML::NoInheritable>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::NoInheritable, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::NoInheritable, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::NoInheritable> Archetype() { return Udm::ArchetypeAttr<PICML::NoInheritable>(impl);}

		static Uml::CompositionParentRole meta_HasOperations_parent;
		Udm::ParentAttr<PICML::HasOperations> HasOperations_parent() const { return Udm::ParentAttr<PICML::HasOperations>(impl, meta_HasOperations_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Enum :  public ConstantType, public NoInheritable {
	 public:
		static Uml::Class meta;

		Enum() { }
		Enum(Udm::ObjectImpl *impl) : ConstantType(impl),NoInheritable(impl), MgaObject(impl) { }
		Enum(const Enum &master) : ConstantType(master),NoInheritable(master), MgaObject(master) { }
		static Enum Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Enum Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Enum CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Enum CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Enum> Instances() { return Udm::InstantiatedAttr<PICML::Enum>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Enum, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Enum, Pred>(impl);}

		Udm::DerivedAttr<PICML::Enum> Derived() { return Udm::DerivedAttr<PICML::Enum>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Enum, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Enum, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Enum> Archetype() { return Udm::ArchetypeAttr<PICML::Enum>(impl);}

		static Uml::CompositionChildRole meta_EnumValue_children;
		Udm::ChildrenAttr<PICML::EnumValue> EnumValue_children() const { return Udm::ChildrenAttr<PICML::EnumValue>(impl, meta_EnumValue_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::EnumValue, Pred> EnumValue_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::EnumValue, Pred>(impl, meta_EnumValue_children); }

		Udm::ChildrenAttr<PICML::EnumValue> EnumValue_kind_children() const { return Udm::ChildrenAttr<PICML::EnumValue>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::EnumValue, Pred> EnumValue_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::EnumValue, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Aggregate :  public NoInheritable {
	 public:
		static Uml::Class meta;

		Aggregate() { }
		Aggregate(Udm::ObjectImpl *impl) : NoInheritable(impl), MgaObject(impl) { }
		Aggregate(const Aggregate &master) : NoInheritable(master), MgaObject(master) { }
		static Aggregate Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Aggregate Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Aggregate CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Aggregate CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Aggregate> Instances() { return Udm::InstantiatedAttr<PICML::Aggregate>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Aggregate, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Aggregate, Pred>(impl);}

		Udm::DerivedAttr<PICML::Aggregate> Derived() { return Udm::DerivedAttr<PICML::Aggregate>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Aggregate, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Aggregate, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Aggregate> Archetype() { return Udm::ArchetypeAttr<PICML::Aggregate>(impl);}

		static Uml::CompositionChildRole meta_Member_children;
		Udm::ChildrenAttr<PICML::Member> Member_children() const { return Udm::ChildrenAttr<PICML::Member>(impl, meta_Member_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Member, Pred> Member_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Member, Pred>(impl, meta_Member_children); }

		Udm::ChildrenAttr<PICML::Member> Member_kind_children() const { return Udm::ChildrenAttr<PICML::Member>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Member, Pred> Member_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Member, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  SwitchedAggregate :  public NoInheritable {
	 public:
		static Uml::Class meta;

		SwitchedAggregate() { }
		SwitchedAggregate(Udm::ObjectImpl *impl) : NoInheritable(impl), MgaObject(impl) { }
		SwitchedAggregate(const SwitchedAggregate &master) : NoInheritable(master), MgaObject(master) { }
		static SwitchedAggregate Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static SwitchedAggregate Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		SwitchedAggregate CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		SwitchedAggregate CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<SwitchedAggregate> Instances() { return Udm::InstantiatedAttr<PICML::SwitchedAggregate>(impl);}
		template <class Pred> Udm::InstantiatedAttr<SwitchedAggregate, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::SwitchedAggregate, Pred>(impl);}

		Udm::DerivedAttr<PICML::SwitchedAggregate> Derived() { return Udm::DerivedAttr<PICML::SwitchedAggregate>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::SwitchedAggregate, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::SwitchedAggregate, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::SwitchedAggregate> Archetype() { return Udm::ArchetypeAttr<PICML::SwitchedAggregate>(impl);}

		static Uml::CompositionChildRole meta_Label_children;
		Udm::ChildrenAttr<PICML::Label> Label_children() const { return Udm::ChildrenAttr<PICML::Label>(impl, meta_Label_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Label, Pred> Label_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Label, Pred>(impl, meta_Label_children); }

		static Uml::CompositionChildRole meta_LabelConnection_children;
		Udm::ChildrenAttr<PICML::LabelConnection> LabelConnection_children() const { return Udm::ChildrenAttr<PICML::LabelConnection>(impl, meta_LabelConnection_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::LabelConnection, Pred> LabelConnection_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::LabelConnection, Pred>(impl, meta_LabelConnection_children); }

		static Uml::CompositionChildRole meta_Member_children;
		Udm::ChildrenAttr<PICML::Member> Member_children() const { return Udm::ChildrenAttr<PICML::Member>(impl, meta_Member_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Member, Pred> Member_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Member, Pred>(impl, meta_Member_children); }

		static Uml::CompositionChildRole meta_Discriminator_child;
		Udm::ChildAttr<PICML::Discriminator> Discriminator_child() const { return Udm::ChildAttr<PICML::Discriminator>(impl, meta_Discriminator_child); }

		Udm::ChildrenAttr<PICML::Label> Label_kind_children() const { return Udm::ChildrenAttr<PICML::Label>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Label, Pred> Label_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Label, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::LabelConnection> LabelConnection_kind_children() const { return Udm::ChildrenAttr<PICML::LabelConnection>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::LabelConnection, Pred> LabelConnection_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::LabelConnection, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Discriminator> Discriminator_kind_children() const { return Udm::ChildrenAttr<PICML::Discriminator>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Discriminator, Pred> Discriminator_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Discriminator, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Member> Member_kind_children() const { return Udm::ChildrenAttr<PICML::Member>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Member, Pred> Member_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Member, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Collection :  public NoInheritable {
	 public:
		static Uml::Class meta;

		Collection() { }
		Collection(Udm::ObjectImpl *impl) : NoInheritable(impl), MgaObject(impl) { }
		Collection(const Collection &master) : NoInheritable(master), MgaObject(master) { }
		static Collection Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Collection Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Collection CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Collection CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Collection> Instances() { return Udm::InstantiatedAttr<PICML::Collection>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Collection, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Collection, Pred>(impl);}

		Udm::DerivedAttr<PICML::Collection> Derived() { return Udm::DerivedAttr<PICML::Collection>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Collection, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Collection, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Collection> Archetype() { return Udm::ArchetypeAttr<PICML::Collection>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::MemberType> ref() const { return Udm::PointerAttr<PICML::MemberType>(impl, meta_ref); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Alias :  public NoInheritable {
	 public:
		static Uml::Class meta;

		Alias() { }
		Alias(Udm::ObjectImpl *impl) : NoInheritable(impl), MgaObject(impl) { }
		Alias(const Alias &master) : NoInheritable(master), MgaObject(master) { }
		static Alias Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Alias Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Alias CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Alias CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Alias> Instances() { return Udm::InstantiatedAttr<PICML::Alias>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Alias, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Alias, Pred>(impl);}

		Udm::DerivedAttr<PICML::Alias> Derived() { return Udm::DerivedAttr<PICML::Alias>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Alias, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Alias, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Alias> Archetype() { return Udm::ArchetypeAttr<PICML::Alias>(impl);}

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::MemberType> ref() const { return Udm::PointerAttr<PICML::MemberType>(impl, meta_ref); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Inheritable :  public NamedType {
	 public:
		static Uml::Class meta;

		Inheritable() { }
		Inheritable(Udm::ObjectImpl *impl) : NamedType(impl), MgaObject(impl) { }
		Inheritable(const Inheritable &master) : NamedType(master), MgaObject(master) { }
		static Inheritable Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Inheritable Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Inheritable CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Inheritable CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Inheritable> Instances() { return Udm::InstantiatedAttr<PICML::Inheritable>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Inheritable, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Inheritable, Pred>(impl);}

		Udm::DerivedAttr<PICML::Inheritable> Derived() { return Udm::DerivedAttr<PICML::Inheritable>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Inheritable, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Inheritable, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Inheritable> Archetype() { return Udm::ArchetypeAttr<PICML::Inheritable>(impl);}

		static Uml::AssociationRole meta_referedbyInherits;
		Udm::AssocAttr<PICML::Inherits> referedbyInherits() const { return Udm::AssocAttr<PICML::Inherits>(impl, meta_referedbyInherits); }
		template <class Pred> Udm::AssocAttr<PICML::Inherits, Pred > referedbyInherits_sorted(const Pred &) const { return Udm::AssocAttr<PICML::Inherits, Pred>(impl, meta_referedbyInherits); }

		static Uml::CompositionChildRole meta_ReadonlyAttribute_children;
		Udm::ChildrenAttr<PICML::ReadonlyAttribute> ReadonlyAttribute_children() const { return Udm::ChildrenAttr<PICML::ReadonlyAttribute>(impl, meta_ReadonlyAttribute_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ReadonlyAttribute, Pred> ReadonlyAttribute_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ReadonlyAttribute, Pred>(impl, meta_ReadonlyAttribute_children); }

		static Uml::CompositionChildRole meta_Inherits_children;
		Udm::ChildrenAttr<PICML::Inherits> Inherits_children() const { return Udm::ChildrenAttr<PICML::Inherits>(impl, meta_Inherits_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Inherits, Pred> Inherits_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Inherits, Pred>(impl, meta_Inherits_children); }

		Udm::ChildrenAttr<PICML::Inherits> Inherits_kind_children() const { return Udm::ChildrenAttr<PICML::Inherits>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Inherits, Pred> Inherits_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Inherits, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ReadonlyAttribute> ReadonlyAttribute_kind_children() const { return Udm::ChildrenAttr<PICML::ReadonlyAttribute>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ReadonlyAttribute, Pred> ReadonlyAttribute_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ReadonlyAttribute, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Attribute> Attribute_kind_children() const { return Udm::ChildrenAttr<PICML::Attribute>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Attribute, Pred> Attribute_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Attribute, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Taggable> Taggable_kind_children() const { return Udm::ChildrenAttr<PICML::Taggable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Taggable, Pred> Taggable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Taggable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  HasOperations :  virtual public Inheritable {
	 public:
		static Uml::Class meta;

		HasOperations() { }
		HasOperations(Udm::ObjectImpl *impl) : Inheritable(impl), MgaObject(impl) { }
		HasOperations(const HasOperations &master) : Inheritable(master), MgaObject(master) { }
		static HasOperations Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static HasOperations Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		HasOperations CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		HasOperations CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<HasOperations> Instances() { return Udm::InstantiatedAttr<PICML::HasOperations>(impl);}
		template <class Pred> Udm::InstantiatedAttr<HasOperations, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::HasOperations, Pred>(impl);}

		Udm::DerivedAttr<PICML::HasOperations> Derived() { return Udm::DerivedAttr<PICML::HasOperations>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::HasOperations, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::HasOperations, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::HasOperations> Archetype() { return Udm::ArchetypeAttr<PICML::HasOperations>(impl);}

		static Uml::CompositionChildRole meta_TwowayOperation_children;
		Udm::ChildrenAttr<PICML::TwowayOperation> TwowayOperation_children() const { return Udm::ChildrenAttr<PICML::TwowayOperation>(impl, meta_TwowayOperation_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::TwowayOperation, Pred> TwowayOperation_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::TwowayOperation, Pred>(impl, meta_TwowayOperation_children); }

		static Uml::CompositionChildRole meta_OnewayOperation_children;
		Udm::ChildrenAttr<PICML::OnewayOperation> OnewayOperation_children() const { return Udm::ChildrenAttr<PICML::OnewayOperation>(impl, meta_OnewayOperation_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::OnewayOperation, Pred> OnewayOperation_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::OnewayOperation, Pred>(impl, meta_OnewayOperation_children); }

		static Uml::CompositionChildRole meta_Exception_children;
		Udm::ChildrenAttr<PICML::Exception> Exception_children() const { return Udm::ChildrenAttr<PICML::Exception>(impl, meta_Exception_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Exception, Pred> Exception_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Exception, Pred>(impl, meta_Exception_children); }

		static Uml::CompositionChildRole meta_Constant_children;
		Udm::ChildrenAttr<PICML::Constant> Constant_children() const { return Udm::ChildrenAttr<PICML::Constant>(impl, meta_Constant_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Constant, Pred> Constant_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Constant, Pred>(impl, meta_Constant_children); }

		static Uml::CompositionChildRole meta_NoInheritable_children;
		Udm::ChildrenAttr<PICML::NoInheritable> NoInheritable_children() const { return Udm::ChildrenAttr<PICML::NoInheritable>(impl, meta_NoInheritable_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::NoInheritable, Pred> NoInheritable_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::NoInheritable, Pred>(impl, meta_NoInheritable_children); }

		Udm::ChildrenAttr<PICML::Exception> Exception_kind_children() const { return Udm::ChildrenAttr<PICML::Exception>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Exception, Pred> Exception_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Exception, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Constant> Constant_kind_children() const { return Udm::ChildrenAttr<PICML::Constant>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Constant, Pred> Constant_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Constant, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Enum> Enum_kind_children() const { return Udm::ChildrenAttr<PICML::Enum>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Enum, Pred> Enum_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Enum, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Aggregate> Aggregate_kind_children() const { return Udm::ChildrenAttr<PICML::Aggregate>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Aggregate, Pred> Aggregate_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Aggregate, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::SwitchedAggregate> SwitchedAggregate_kind_children() const { return Udm::ChildrenAttr<PICML::SwitchedAggregate>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::SwitchedAggregate, Pred> SwitchedAggregate_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::SwitchedAggregate, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MemberType> MemberType_kind_children() const { return Udm::ChildrenAttr<PICML::MemberType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MemberType, Pred> MemberType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MemberType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::NamedType> NamedType_kind_children() const { return Udm::ChildrenAttr<PICML::NamedType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::NamedType, Pred> NamedType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::NamedType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Collection> Collection_kind_children() const { return Udm::ChildrenAttr<PICML::Collection>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Collection, Pred> Collection_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Collection, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Alias> Alias_kind_children() const { return Udm::ChildrenAttr<PICML::Alias>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Alias, Pred> Alias_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Alias, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::NoInheritable> NoInheritable_kind_children() const { return Udm::ChildrenAttr<PICML::NoInheritable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::NoInheritable, Pred> NoInheritable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::NoInheritable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::OperationBase> OperationBase_kind_children() const { return Udm::ChildrenAttr<PICML::OperationBase>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::OperationBase, Pred> OperationBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::OperationBase, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::TwowayOperation> TwowayOperation_kind_children() const { return Udm::ChildrenAttr<PICML::TwowayOperation>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::TwowayOperation, Pred> TwowayOperation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::TwowayOperation, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::OnewayOperation> OnewayOperation_kind_children() const { return Udm::ChildrenAttr<PICML::OnewayOperation>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::OnewayOperation, Pred> OnewayOperation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::OnewayOperation, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::HasExceptions> HasExceptions_kind_children() const { return Udm::ChildrenAttr<PICML::HasExceptions>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::HasExceptions, Pred> HasExceptions_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::HasExceptions, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Taggable> Taggable_kind_children() const { return Udm::ChildrenAttr<PICML::Taggable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Taggable, Pred> Taggable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Taggable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Object :  public HasOperations, public Provideable, public Prefixable {
	 public:
		static Uml::Class meta;

		Object() { }
		Object(Udm::ObjectImpl *impl) : HasOperations(impl),Provideable(impl),Prefixable(impl), Inheritable(impl), MgaObject(impl) { }
		Object(const Object &master) : HasOperations(master),Provideable(master),Prefixable(master), Inheritable(master), MgaObject(master) { }
		static Object Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Object Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Object CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Object CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Object> Instances() { return Udm::InstantiatedAttr<PICML::Object>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Object, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Object, Pred>(impl);}

		Udm::DerivedAttr<PICML::Object> Derived() { return Udm::DerivedAttr<PICML::Object>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Object, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Object, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Object> Archetype() { return Udm::ArchetypeAttr<PICML::Object>(impl);}

		static Uml::Attribute meta_abstract;
		Udm::BooleanAttr abstract() const { return Udm::BooleanAttr(impl, meta_abstract); }

		static Uml::Attribute meta_local;
		Udm::BooleanAttr local() const { return Udm::BooleanAttr(impl, meta_local); }

		static Uml::AssociationRole meta_referedbySupports;
		Udm::AssocAttr<PICML::Supports> referedbySupports() const { return Udm::AssocAttr<PICML::Supports>(impl, meta_referedbySupports); }
		template <class Pred> Udm::AssocAttr<PICML::Supports, Pred > referedbySupports_sorted(const Pred &) const { return Udm::AssocAttr<PICML::Supports, Pred>(impl, meta_referedbySupports); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  SupportsInterfaces :  virtual public Inheritable {
	 public:
		static Uml::Class meta;

		SupportsInterfaces() { }
		SupportsInterfaces(Udm::ObjectImpl *impl) : Inheritable(impl), MgaObject(impl) { }
		SupportsInterfaces(const SupportsInterfaces &master) : Inheritable(master), MgaObject(master) { }
		static SupportsInterfaces Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static SupportsInterfaces Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		SupportsInterfaces CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		SupportsInterfaces CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<SupportsInterfaces> Instances() { return Udm::InstantiatedAttr<PICML::SupportsInterfaces>(impl);}
		template <class Pred> Udm::InstantiatedAttr<SupportsInterfaces, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::SupportsInterfaces, Pred>(impl);}

		Udm::DerivedAttr<PICML::SupportsInterfaces> Derived() { return Udm::DerivedAttr<PICML::SupportsInterfaces>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::SupportsInterfaces, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::SupportsInterfaces, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::SupportsInterfaces> Archetype() { return Udm::ArchetypeAttr<PICML::SupportsInterfaces>(impl);}

		static Uml::CompositionChildRole meta_Supports_children;
		Udm::ChildrenAttr<PICML::Supports> Supports_children() const { return Udm::ChildrenAttr<PICML::Supports>(impl, meta_Supports_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Supports, Pred> Supports_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Supports, Pred>(impl, meta_Supports_children); }

		Udm::ChildrenAttr<PICML::Supports> Supports_kind_children() const { return Udm::ChildrenAttr<PICML::Supports>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Supports, Pred> Supports_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Supports, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  ReadonlyAttribute :  public Taggable {
	 public:
		static Uml::Class meta;

		ReadonlyAttribute() { }
		ReadonlyAttribute(Udm::ObjectImpl *impl) : Taggable(impl), MgaObject(impl) { }
		ReadonlyAttribute(const ReadonlyAttribute &master) : Taggable(master), MgaObject(master) { }
		static ReadonlyAttribute Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ReadonlyAttribute Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ReadonlyAttribute CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ReadonlyAttribute CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ReadonlyAttribute> Instances() { return Udm::InstantiatedAttr<PICML::ReadonlyAttribute>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ReadonlyAttribute, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ReadonlyAttribute, Pred>(impl);}

		Udm::DerivedAttr<PICML::ReadonlyAttribute> Derived() { return Udm::DerivedAttr<PICML::ReadonlyAttribute>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ReadonlyAttribute, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ReadonlyAttribute, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ReadonlyAttribute> Archetype() { return Udm::ArchetypeAttr<PICML::ReadonlyAttribute>(impl);}

		static Uml::CompositionChildRole meta_AttributeMember_child;
		Udm::ChildAttr<PICML::AttributeMember> AttributeMember_child() const { return Udm::ChildAttr<PICML::AttributeMember>(impl, meta_AttributeMember_child); }

		static Uml::CompositionChildRole meta_GetException_children;
		Udm::ChildrenAttr<PICML::GetException> GetException_children() const { return Udm::ChildrenAttr<PICML::GetException>(impl, meta_GetException_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::GetException, Pred> GetException_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::GetException, Pred>(impl, meta_GetException_children); }

		Udm::ChildrenAttr<PICML::GetException> GetException_kind_children() const { return Udm::ChildrenAttr<PICML::GetException>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::GetException, Pred> GetException_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::GetException, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::AttributeMember> AttributeMember_kind_children() const { return Udm::ChildrenAttr<PICML::AttributeMember>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::AttributeMember, Pred> AttributeMember_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::AttributeMember, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_Inheritable_parent;
		Udm::ParentAttr<PICML::Inheritable> Inheritable_parent() const { return Udm::ParentAttr<PICML::Inheritable>(impl, meta_Inheritable_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  ObjectByValue :  public HasOperations, public SupportsInterfaces, public Prefixable {
	 public:
		static Uml::Class meta;

		ObjectByValue() { }
		ObjectByValue(Udm::ObjectImpl *impl) : HasOperations(impl),SupportsInterfaces(impl),Prefixable(impl), Inheritable(impl), MgaObject(impl) { }
		ObjectByValue(const ObjectByValue &master) : HasOperations(master),SupportsInterfaces(master),Prefixable(master), Inheritable(master), MgaObject(master) { }
		static ObjectByValue Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ObjectByValue Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ObjectByValue CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ObjectByValue CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ObjectByValue> Instances() { return Udm::InstantiatedAttr<PICML::ObjectByValue>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ObjectByValue, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ObjectByValue, Pred>(impl);}

		Udm::DerivedAttr<PICML::ObjectByValue> Derived() { return Udm::DerivedAttr<PICML::ObjectByValue>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ObjectByValue, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ObjectByValue, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ObjectByValue> Archetype() { return Udm::ArchetypeAttr<PICML::ObjectByValue>(impl);}

		static Uml::Attribute meta_abstract;
		Udm::BooleanAttr abstract() const { return Udm::BooleanAttr(impl, meta_abstract); }

		static Uml::CompositionChildRole meta_PrivateFlag_children;
		Udm::ChildrenAttr<PICML::PrivateFlag> PrivateFlag_children() const { return Udm::ChildrenAttr<PICML::PrivateFlag>(impl, meta_PrivateFlag_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::PrivateFlag, Pred> PrivateFlag_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PrivateFlag, Pred>(impl, meta_PrivateFlag_children); }

		static Uml::CompositionChildRole meta_FactoryOperation_children;
		Udm::ChildrenAttr<PICML::FactoryOperation> FactoryOperation_children() const { return Udm::ChildrenAttr<PICML::FactoryOperation>(impl, meta_FactoryOperation_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::FactoryOperation, Pred> FactoryOperation_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::FactoryOperation, Pred>(impl, meta_FactoryOperation_children); }

		static Uml::CompositionChildRole meta_MakeMemberPrivate_children;
		Udm::ChildrenAttr<PICML::MakeMemberPrivate> MakeMemberPrivate_children() const { return Udm::ChildrenAttr<PICML::MakeMemberPrivate>(impl, meta_MakeMemberPrivate_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::MakeMemberPrivate, Pred> MakeMemberPrivate_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MakeMemberPrivate, Pred>(impl, meta_MakeMemberPrivate_children); }

		static Uml::CompositionChildRole meta_Member_children;
		Udm::ChildrenAttr<PICML::Member> Member_children() const { return Udm::ChildrenAttr<PICML::Member>(impl, meta_Member_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Member, Pred> Member_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Member, Pred>(impl, meta_Member_children); }

		Udm::ChildrenAttr<PICML::Member> Member_kind_children() const { return Udm::ChildrenAttr<PICML::Member>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Member, Pred> Member_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Member, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::PrivateFlag> PrivateFlag_kind_children() const { return Udm::ChildrenAttr<PICML::PrivateFlag>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::PrivateFlag, Pred> PrivateFlag_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::PrivateFlag, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MakeMemberPrivate> MakeMemberPrivate_kind_children() const { return Udm::ChildrenAttr<PICML::MakeMemberPrivate>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MakeMemberPrivate, Pred> MakeMemberPrivate_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MakeMemberPrivate, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::OperationBase> OperationBase_kind_children() const { return Udm::ChildrenAttr<PICML::OperationBase>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::OperationBase, Pred> OperationBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::OperationBase, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::FactoryOperation> FactoryOperation_kind_children() const { return Udm::ChildrenAttr<PICML::FactoryOperation>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::FactoryOperation, Pred> FactoryOperation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::FactoryOperation, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::HasExceptions> HasExceptions_kind_children() const { return Udm::ChildrenAttr<PICML::HasExceptions>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::HasExceptions, Pred> HasExceptions_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::HasExceptions, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Taggable> Taggable_kind_children() const { return Udm::ChildrenAttr<PICML::Taggable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Taggable, Pred> Taggable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Taggable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Event :  public ObjectByValue {
	 public:
		static Uml::Class meta;

		Event() { }
		Event(Udm::ObjectImpl *impl) : ObjectByValue(impl), Inheritable(impl), MgaObject(impl) { }
		Event(const Event &master) : ObjectByValue(master), Inheritable(master), MgaObject(master) { }
		static Event Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Event Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Event CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Event CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Event> Instances() { return Udm::InstantiatedAttr<PICML::Event>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Event, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Event, Pred>(impl);}

		Udm::DerivedAttr<PICML::Event> Derived() { return Udm::DerivedAttr<PICML::Event>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Event, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Event, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Event> Archetype() { return Udm::ArchetypeAttr<PICML::Event>(impl);}

		static Uml::AssociationRole meta_referedbyInEventPort;
		Udm::AssocAttr<PICML::InEventPort> referedbyInEventPort() const { return Udm::AssocAttr<PICML::InEventPort>(impl, meta_referedbyInEventPort); }
		template <class Pred> Udm::AssocAttr<PICML::InEventPort, Pred > referedbyInEventPort_sorted(const Pred &) const { return Udm::AssocAttr<PICML::InEventPort, Pred>(impl, meta_referedbyInEventPort); }

		static Uml::AssociationRole meta_referedbyOutEventPort;
		Udm::AssocAttr<PICML::OutEventPort> referedbyOutEventPort() const { return Udm::AssocAttr<PICML::OutEventPort>(impl, meta_referedbyOutEventPort); }
		template <class Pred> Udm::AssocAttr<PICML::OutEventPort, Pred > referedbyOutEventPort_sorted(const Pred &) const { return Udm::AssocAttr<PICML::OutEventPort, Pred>(impl, meta_referedbyOutEventPort); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  ValueObject :  public ObjectByValue {
	 public:
		static Uml::Class meta;

		ValueObject() { }
		ValueObject(Udm::ObjectImpl *impl) : ObjectByValue(impl), Inheritable(impl), MgaObject(impl) { }
		ValueObject(const ValueObject &master) : ObjectByValue(master), Inheritable(master), MgaObject(master) { }
		static ValueObject Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ValueObject Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ValueObject CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ValueObject CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ValueObject> Instances() { return Udm::InstantiatedAttr<PICML::ValueObject>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ValueObject, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ValueObject, Pred>(impl);}

		Udm::DerivedAttr<PICML::ValueObject> Derived() { return Udm::DerivedAttr<PICML::ValueObject>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ValueObject, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ValueObject, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ValueObject> Archetype() { return Udm::ArchetypeAttr<PICML::ValueObject>(impl);}

		static Uml::AssociationRole meta_referedbyLookupKey;
		Udm::AssocAttr<PICML::LookupKey> referedbyLookupKey() const { return Udm::AssocAttr<PICML::LookupKey>(impl, meta_referedbyLookupKey); }
		template <class Pred> Udm::AssocAttr<PICML::LookupKey, Pred > referedbyLookupKey_sorted(const Pred &) const { return Udm::AssocAttr<PICML::LookupKey, Pred>(impl, meta_referedbyLookupKey); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Attribute :  public ReadonlyAttribute {
	 public:
		static Uml::Class meta;

		Attribute() { }
		Attribute(Udm::ObjectImpl *impl) : ReadonlyAttribute(impl), MgaObject(impl) { }
		Attribute(const Attribute &master) : ReadonlyAttribute(master), MgaObject(master) { }
		static Attribute Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Attribute Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Attribute CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Attribute CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Attribute> Instances() { return Udm::InstantiatedAttr<PICML::Attribute>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Attribute, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Attribute, Pred>(impl);}

		Udm::DerivedAttr<PICML::Attribute> Derived() { return Udm::DerivedAttr<PICML::Attribute>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Attribute, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Attribute, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Attribute> Archetype() { return Udm::ArchetypeAttr<PICML::Attribute>(impl);}

		static Uml::CompositionChildRole meta_SetException_children;
		Udm::ChildrenAttr<PICML::SetException> SetException_children() const { return Udm::ChildrenAttr<PICML::SetException>(impl, meta_SetException_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::SetException, Pred> SetException_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::SetException, Pred>(impl, meta_SetException_children); }

		Udm::ChildrenAttr<PICML::SetException> SetException_kind_children() const { return Udm::ChildrenAttr<PICML::SetException>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::SetException, Pred> SetException_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::SetException, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  OperationBase :  public Taggable {
	 public:
		static Uml::Class meta;

		OperationBase() { }
		OperationBase(Udm::ObjectImpl *impl) : Taggable(impl), MgaObject(impl) { }
		OperationBase(const OperationBase &master) : Taggable(master), MgaObject(master) { }
		static OperationBase Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static OperationBase Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		OperationBase CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		OperationBase CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<OperationBase> Instances() { return Udm::InstantiatedAttr<PICML::OperationBase>(impl);}
		template <class Pred> Udm::InstantiatedAttr<OperationBase, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::OperationBase, Pred>(impl);}

		Udm::DerivedAttr<PICML::OperationBase> Derived() { return Udm::DerivedAttr<PICML::OperationBase>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::OperationBase, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::OperationBase, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::OperationBase> Archetype() { return Udm::ArchetypeAttr<PICML::OperationBase>(impl);}

		static Uml::CompositionChildRole meta_InParameter_children;
		Udm::ChildrenAttr<PICML::InParameter> InParameter_children() const { return Udm::ChildrenAttr<PICML::InParameter>(impl, meta_InParameter_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::InParameter, Pred> InParameter_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::InParameter, Pred>(impl, meta_InParameter_children); }

		Udm::ChildrenAttr<PICML::InParameter> InParameter_kind_children() const { return Udm::ChildrenAttr<PICML::InParameter>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::InParameter, Pred> InParameter_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::InParameter, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class  OnewayOperation :  public OperationBase {
	 public:
		static Uml::Class meta;

		OnewayOperation() { }
		OnewayOperation(Udm::ObjectImpl *impl) : OperationBase(impl), MgaObject(impl) { }
		OnewayOperation(const OnewayOperation &master) : OperationBase(master), MgaObject(master) { }
		static OnewayOperation Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static OnewayOperation Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		OnewayOperation CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		OnewayOperation CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<OnewayOperation> Instances() { return Udm::InstantiatedAttr<PICML::OnewayOperation>(impl);}
		template <class Pred> Udm::InstantiatedAttr<OnewayOperation, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::OnewayOperation, Pred>(impl);}

		Udm::DerivedAttr<PICML::OnewayOperation> Derived() { return Udm::DerivedAttr<PICML::OnewayOperation>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::OnewayOperation, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::OnewayOperation, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::OnewayOperation> Archetype() { return Udm::ArchetypeAttr<PICML::OnewayOperation>(impl);}

		static Uml::CompositionParentRole meta_HasOperations_parent;
		Udm::ParentAttr<PICML::HasOperations> HasOperations_parent() const { return Udm::ParentAttr<PICML::HasOperations>(impl, meta_HasOperations_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  HasExceptions :  public OperationBase {
	 public:
		static Uml::Class meta;

		HasExceptions() { }
		HasExceptions(Udm::ObjectImpl *impl) : OperationBase(impl), MgaObject(impl) { }
		HasExceptions(const HasExceptions &master) : OperationBase(master), MgaObject(master) { }
		static HasExceptions Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static HasExceptions Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		HasExceptions CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		HasExceptions CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<HasExceptions> Instances() { return Udm::InstantiatedAttr<PICML::HasExceptions>(impl);}
		template <class Pred> Udm::InstantiatedAttr<HasExceptions, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::HasExceptions, Pred>(impl);}

		Udm::DerivedAttr<PICML::HasExceptions> Derived() { return Udm::DerivedAttr<PICML::HasExceptions>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::HasExceptions, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::HasExceptions, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::HasExceptions> Archetype() { return Udm::ArchetypeAttr<PICML::HasExceptions>(impl);}

		static Uml::CompositionChildRole meta_ExceptionRef_children;
		Udm::ChildrenAttr<PICML::ExceptionRef> ExceptionRef_children() const { return Udm::ChildrenAttr<PICML::ExceptionRef>(impl, meta_ExceptionRef_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::ExceptionRef, Pred> ExceptionRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ExceptionRef, Pred>(impl, meta_ExceptionRef_children); }

		Udm::ChildrenAttr<PICML::ExceptionRef> ExceptionRef_kind_children() const { return Udm::ChildrenAttr<PICML::ExceptionRef>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ExceptionRef, Pred> ExceptionRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ExceptionRef, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
	};

	class  FactoryOperation :  public HasExceptions {
	 public:
		static Uml::Class meta;

		FactoryOperation() { }
		FactoryOperation(Udm::ObjectImpl *impl) : HasExceptions(impl), MgaObject(impl) { }
		FactoryOperation(const FactoryOperation &master) : HasExceptions(master), MgaObject(master) { }
		static FactoryOperation Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static FactoryOperation Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		FactoryOperation CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		FactoryOperation CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<FactoryOperation> Instances() { return Udm::InstantiatedAttr<PICML::FactoryOperation>(impl);}
		template <class Pred> Udm::InstantiatedAttr<FactoryOperation, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::FactoryOperation, Pred>(impl);}

		Udm::DerivedAttr<PICML::FactoryOperation> Derived() { return Udm::DerivedAttr<PICML::FactoryOperation>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::FactoryOperation, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::FactoryOperation, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::FactoryOperation> Archetype() { return Udm::ArchetypeAttr<PICML::FactoryOperation>(impl);}

		static Uml::CompositionParentRole meta_ObjectByValue_parent;
		Udm::ParentAttr<PICML::ObjectByValue> ObjectByValue_parent() const { return Udm::ParentAttr<PICML::ObjectByValue>(impl, meta_ObjectByValue_parent); }

		static Uml::CompositionParentRole meta_ComponentFactory_parent;
		Udm::ParentAttr<PICML::ComponentFactory> ComponentFactory_parent() const { return Udm::ParentAttr<PICML::ComponentFactory>(impl, meta_ComponentFactory_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  LookupOperation :  public HasExceptions {
	 public:
		static Uml::Class meta;

		LookupOperation() { }
		LookupOperation(Udm::ObjectImpl *impl) : HasExceptions(impl), MgaObject(impl) { }
		LookupOperation(const LookupOperation &master) : HasExceptions(master), MgaObject(master) { }
		static LookupOperation Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static LookupOperation Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		LookupOperation CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		LookupOperation CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<LookupOperation> Instances() { return Udm::InstantiatedAttr<PICML::LookupOperation>(impl);}
		template <class Pred> Udm::InstantiatedAttr<LookupOperation, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::LookupOperation, Pred>(impl);}

		Udm::DerivedAttr<PICML::LookupOperation> Derived() { return Udm::DerivedAttr<PICML::LookupOperation>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::LookupOperation, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::LookupOperation, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::LookupOperation> Archetype() { return Udm::ArchetypeAttr<PICML::LookupOperation>(impl);}

		static Uml::CompositionParentRole meta_ComponentFactory_parent;
		Udm::ParentAttr<PICML::ComponentFactory> ComponentFactory_parent() const { return Udm::ParentAttr<PICML::ComponentFactory>(impl, meta_ComponentFactory_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  TwowayOperation :  public HasExceptions {
	 public:
		static Uml::Class meta;

		TwowayOperation() { }
		TwowayOperation(Udm::ObjectImpl *impl) : HasExceptions(impl), MgaObject(impl) { }
		TwowayOperation(const TwowayOperation &master) : HasExceptions(master), MgaObject(master) { }
		static TwowayOperation Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static TwowayOperation Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		TwowayOperation CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		TwowayOperation CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<TwowayOperation> Instances() { return Udm::InstantiatedAttr<PICML::TwowayOperation>(impl);}
		template <class Pred> Udm::InstantiatedAttr<TwowayOperation, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::TwowayOperation, Pred>(impl);}

		Udm::DerivedAttr<PICML::TwowayOperation> Derived() { return Udm::DerivedAttr<PICML::TwowayOperation>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::TwowayOperation, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::TwowayOperation, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::TwowayOperation> Archetype() { return Udm::ArchetypeAttr<PICML::TwowayOperation>(impl);}

		static Uml::CompositionChildRole meta_OutParameter_children;
		Udm::ChildrenAttr<PICML::OutParameter> OutParameter_children() const { return Udm::ChildrenAttr<PICML::OutParameter>(impl, meta_OutParameter_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::OutParameter, Pred> OutParameter_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::OutParameter, Pred>(impl, meta_OutParameter_children); }

		static Uml::CompositionChildRole meta_InoutParameter_children;
		Udm::ChildrenAttr<PICML::InoutParameter> InoutParameter_children() const { return Udm::ChildrenAttr<PICML::InoutParameter>(impl, meta_InoutParameter_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::InoutParameter, Pred> InoutParameter_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::InoutParameter, Pred>(impl, meta_InoutParameter_children); }

		static Uml::CompositionChildRole meta_ReturnType_child;
		Udm::ChildAttr<PICML::ReturnType> ReturnType_child() const { return Udm::ChildAttr<PICML::ReturnType>(impl, meta_ReturnType_child); }

		Udm::ChildrenAttr<PICML::OutParameter> OutParameter_kind_children() const { return Udm::ChildrenAttr<PICML::OutParameter>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::OutParameter, Pred> OutParameter_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::OutParameter, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ReturnType> ReturnType_kind_children() const { return Udm::ChildrenAttr<PICML::ReturnType>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ReturnType, Pred> ReturnType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ReturnType, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::InoutParameter> InoutParameter_kind_children() const { return Udm::ChildrenAttr<PICML::InoutParameter>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::InoutParameter, Pred> InoutParameter_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::InoutParameter, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_HasOperations_parent;
		Udm::ParentAttr<PICML::HasOperations> HasOperations_parent() const { return Udm::ParentAttr<PICML::HasOperations>(impl, meta_HasOperations_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Component :  public SupportsInterfaces, public Manageable {
	 public:
		static Uml::Class meta;

		Component() { }
		Component(Udm::ObjectImpl *impl) : SupportsInterfaces(impl),Manageable(impl), Inheritable(impl), MgaObject(impl) { }
		Component(const Component &master) : SupportsInterfaces(master),Manageable(master), Inheritable(master), MgaObject(master) { }
		static Component Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Component Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Component CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Component CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Component> Instances() { return Udm::InstantiatedAttr<PICML::Component>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Component, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Component, Pred>(impl);}

		Udm::DerivedAttr<PICML::Component> Derived() { return Udm::DerivedAttr<PICML::Component>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Component, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Component, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Component> Archetype() { return Udm::ArchetypeAttr<PICML::Component>(impl);}

		static Uml::Attribute meta_UUID;
		Udm::StringAttr UUID() const { return Udm::StringAttr(impl, meta_UUID); }

		static Uml::Attribute meta_label;
		Udm::StringAttr label() const { return Udm::StringAttr(impl, meta_label); }

		static Uml::AssociationRole meta_referedbyComponentType;
		Udm::AssocAttr<PICML::ComponentType> referedbyComponentType() const { return Udm::AssocAttr<PICML::ComponentType>(impl, meta_referedbyComponentType); }
		template <class Pred> Udm::AssocAttr<PICML::ComponentType, Pred > referedbyComponentType_sorted(const Pred &) const { return Udm::AssocAttr<PICML::ComponentType, Pred>(impl, meta_referedbyComponentType); }

		static Uml::AssociationRole meta_dstAssemblyselectRequirement, meta_dstAssemblyselectRequirement_rev;
		Udm::AClassAssocAttr<PICML::AssemblyselectRequirement, PICML::Requirement> dstAssemblyselectRequirement() const { return Udm::AClassAssocAttr<PICML::AssemblyselectRequirement, PICML::Requirement>(impl, meta_dstAssemblyselectRequirement, meta_dstAssemblyselectRequirement_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::AssemblyselectRequirement, PICML::Requirement, Pred> dstAssemblyselectRequirement_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::AssemblyselectRequirement, PICML::Requirement, Pred>(impl, meta_dstAssemblyselectRequirement, meta_dstAssemblyselectRequirement_rev); }

		static Uml::AssociationRole meta_dstAssemblyConfigProperty, meta_dstAssemblyConfigProperty_rev;
		Udm::AClassAssocAttr<PICML::AssemblyConfigProperty, PICML::Property> dstAssemblyConfigProperty() const { return Udm::AClassAssocAttr<PICML::AssemblyConfigProperty, PICML::Property>(impl, meta_dstAssemblyConfigProperty, meta_dstAssemblyConfigProperty_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::AssemblyConfigProperty, PICML::Property, Pred> dstAssemblyConfigProperty_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::AssemblyConfigProperty, PICML::Property, Pred>(impl, meta_dstAssemblyConfigProperty, meta_dstAssemblyConfigProperty_rev); }

		static Uml::AssociationRole meta_referedbyComponentRef;
		Udm::AssocAttr<PICML::ComponentRef> referedbyComponentRef() const { return Udm::AssocAttr<PICML::ComponentRef>(impl, meta_referedbyComponentRef); }
		template <class Pred> Udm::AssocAttr<PICML::ComponentRef, Pred > referedbyComponentRef_sorted(const Pred &) const { return Udm::AssocAttr<PICML::ComponentRef, Pred>(impl, meta_referedbyComponentRef); }

		static Uml::CompositionChildRole meta_Port_children;
		Udm::ChildrenAttr<PICML::Port> Port_children() const { return Udm::ChildrenAttr<PICML::Port>(impl, meta_Port_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::Port, Pred> Port_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Port, Pred>(impl, meta_Port_children); }

		Udm::ChildrenAttr<PICML::CommonPortAttrs> CommonPortAttrs_kind_children() const { return Udm::ChildrenAttr<PICML::CommonPortAttrs>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::CommonPortAttrs, Pred> CommonPortAttrs_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::CommonPortAttrs, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::OutEventPort> OutEventPort_kind_children() const { return Udm::ChildrenAttr<PICML::OutEventPort>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::OutEventPort, Pred> OutEventPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::OutEventPort, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::ProvidedRequestPort> ProvidedRequestPort_kind_children() const { return Udm::ChildrenAttr<PICML::ProvidedRequestPort>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::ProvidedRequestPort, Pred> ProvidedRequestPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::ProvidedRequestPort, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::RequiredRequestPort> RequiredRequestPort_kind_children() const { return Udm::ChildrenAttr<PICML::RequiredRequestPort>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::RequiredRequestPort, Pred> RequiredRequestPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::RequiredRequestPort, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::InEventPort> InEventPort_kind_children() const { return Udm::ChildrenAttr<PICML::InEventPort>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::InEventPort, Pred> InEventPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::InEventPort, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Port> Port_kind_children() const { return Udm::ChildrenAttr<PICML::Port>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Port, Pred> Port_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Port, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Taggable> Taggable_kind_children() const { return Udm::ChildrenAttr<PICML::Taggable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Taggable, Pred> Taggable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Taggable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		static Uml::CompositionParentRole meta_ComponentAssembly_parent;
		Udm::ParentAttr<PICML::ComponentAssembly> ComponentAssembly_parent() const { return Udm::ParentAttr<PICML::ComponentAssembly>(impl, meta_ComponentAssembly_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  Port :  public CommonPortAttrs, public Taggable {
	 public:
		static Uml::Class meta;

		Port() { }
		Port(Udm::ObjectImpl *impl) : CommonPortAttrs(impl),Taggable(impl), MgaObject(impl) { }
		Port(const Port &master) : CommonPortAttrs(master),Taggable(master), MgaObject(master) { }
		static Port Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static Port Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		Port CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		Port CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<Port> Instances() { return Udm::InstantiatedAttr<PICML::Port>(impl);}
		template <class Pred> Udm::InstantiatedAttr<Port, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::Port, Pred>(impl);}

		Udm::DerivedAttr<PICML::Port> Derived() { return Udm::DerivedAttr<PICML::Port>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::Port, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::Port, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::Port> Archetype() { return Udm::ArchetypeAttr<PICML::Port>(impl);}

		static Uml::CompositionParentRole meta_Component_parent;
		Udm::ParentAttr<PICML::Component> Component_parent() const { return Udm::ParentAttr<PICML::Component>(impl, meta_Component_parent); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  OutEventPort :  public Port {
	 public:
		static Uml::Class meta;

		OutEventPort() { }
		OutEventPort(Udm::ObjectImpl *impl) : Port(impl), MgaObject(impl) { }
		OutEventPort(const OutEventPort &master) : Port(master), MgaObject(master) { }
		static OutEventPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static OutEventPort Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		OutEventPort CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		OutEventPort CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<OutEventPort> Instances() { return Udm::InstantiatedAttr<PICML::OutEventPort>(impl);}
		template <class Pred> Udm::InstantiatedAttr<OutEventPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::OutEventPort, Pred>(impl);}

		Udm::DerivedAttr<PICML::OutEventPort> Derived() { return Udm::DerivedAttr<PICML::OutEventPort>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::OutEventPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::OutEventPort, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::OutEventPort> Archetype() { return Udm::ArchetypeAttr<PICML::OutEventPort>(impl);}

		static Uml::Attribute meta_single_destination;
		Udm::BooleanAttr single_destination() const { return Udm::BooleanAttr(impl, meta_single_destination); }

		static Uml::AssociationRole meta_dstpublish, meta_dstpublish_rev;
		Udm::AClassAssocAttr<PICML::publish, PICML::PublishConnector> dstpublish() const { return Udm::AClassAssocAttr<PICML::publish, PICML::PublishConnector>(impl, meta_dstpublish, meta_dstpublish_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::publish, PICML::PublishConnector, Pred> dstpublish_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::publish, PICML::PublishConnector, Pred>(impl, meta_dstpublish, meta_dstpublish_rev); }

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::Event> ref() const { return Udm::PointerAttr<PICML::Event>(impl, meta_ref); }

		static Uml::AssociationRole meta_dstemit, meta_dstemit_rev;
		Udm::AClassPointerAttr<PICML::emit, PICML::InEventPort> dstemit() const { return Udm::AClassPointerAttr<PICML::emit, PICML::InEventPort>(impl, meta_dstemit, meta_dstemit_rev); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  ProvidedRequestPort :  public Port {
	 public:
		static Uml::Class meta;

		ProvidedRequestPort() { }
		ProvidedRequestPort(Udm::ObjectImpl *impl) : Port(impl), MgaObject(impl) { }
		ProvidedRequestPort(const ProvidedRequestPort &master) : Port(master), MgaObject(master) { }
		static ProvidedRequestPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ProvidedRequestPort Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ProvidedRequestPort CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ProvidedRequestPort CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ProvidedRequestPort> Instances() { return Udm::InstantiatedAttr<PICML::ProvidedRequestPort>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ProvidedRequestPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ProvidedRequestPort, Pred>(impl);}

		Udm::DerivedAttr<PICML::ProvidedRequestPort> Derived() { return Udm::DerivedAttr<PICML::ProvidedRequestPort>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ProvidedRequestPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ProvidedRequestPort, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ProvidedRequestPort> Archetype() { return Udm::ArchetypeAttr<PICML::ProvidedRequestPort>(impl);}

		static Uml::AssociationRole meta_srcinvoke, meta_srcinvoke_rev;
		Udm::AClassAssocAttr<PICML::invoke, PICML::RequiredRequestPort> srcinvoke() const { return Udm::AClassAssocAttr<PICML::invoke, PICML::RequiredRequestPort>(impl, meta_srcinvoke, meta_srcinvoke_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::invoke, PICML::RequiredRequestPort, Pred> srcinvoke_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::invoke, PICML::RequiredRequestPort, Pred>(impl, meta_srcinvoke, meta_srcinvoke_rev); }

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::Provideable> ref() const { return Udm::PointerAttr<PICML::Provideable>(impl, meta_ref); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  RequiredRequestPort :  public Port {
	 public:
		static Uml::Class meta;

		RequiredRequestPort() { }
		RequiredRequestPort(Udm::ObjectImpl *impl) : Port(impl), MgaObject(impl) { }
		RequiredRequestPort(const RequiredRequestPort &master) : Port(master), MgaObject(master) { }
		static RequiredRequestPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static RequiredRequestPort Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		RequiredRequestPort CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		RequiredRequestPort CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<RequiredRequestPort> Instances() { return Udm::InstantiatedAttr<PICML::RequiredRequestPort>(impl);}
		template <class Pred> Udm::InstantiatedAttr<RequiredRequestPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::RequiredRequestPort, Pred>(impl);}

		Udm::DerivedAttr<PICML::RequiredRequestPort> Derived() { return Udm::DerivedAttr<PICML::RequiredRequestPort>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::RequiredRequestPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::RequiredRequestPort, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::RequiredRequestPort> Archetype() { return Udm::ArchetypeAttr<PICML::RequiredRequestPort>(impl);}

		static Uml::Attribute meta_multiple_connections;
		Udm::BooleanAttr multiple_connections() const { return Udm::BooleanAttr(impl, meta_multiple_connections); }

		static Uml::AssociationRole meta_dstinvoke, meta_dstinvoke_rev;
		Udm::AClassAssocAttr<PICML::invoke, PICML::ProvidedRequestPort> dstinvoke() const { return Udm::AClassAssocAttr<PICML::invoke, PICML::ProvidedRequestPort>(impl, meta_dstinvoke, meta_dstinvoke_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::invoke, PICML::ProvidedRequestPort, Pred> dstinvoke_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::invoke, PICML::ProvidedRequestPort, Pred>(impl, meta_dstinvoke, meta_dstinvoke_rev); }

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::Provideable> ref() const { return Udm::PointerAttr<PICML::Provideable>(impl, meta_ref); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  InEventPort :  public Port {
	 public:
		static Uml::Class meta;

		InEventPort() { }
		InEventPort(Udm::ObjectImpl *impl) : Port(impl), MgaObject(impl) { }
		InEventPort(const InEventPort &master) : Port(master), MgaObject(master) { }
		static InEventPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static InEventPort Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		InEventPort CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		InEventPort CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<InEventPort> Instances() { return Udm::InstantiatedAttr<PICML::InEventPort>(impl);}
		template <class Pred> Udm::InstantiatedAttr<InEventPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::InEventPort, Pred>(impl);}

		Udm::DerivedAttr<PICML::InEventPort> Derived() { return Udm::DerivedAttr<PICML::InEventPort>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::InEventPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::InEventPort, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::InEventPort> Archetype() { return Udm::ArchetypeAttr<PICML::InEventPort>(impl);}

		static Uml::AssociationRole meta_srcdeliverTo, meta_srcdeliverTo_rev;
		Udm::AClassAssocAttr<PICML::deliverTo, PICML::PublishConnector> srcdeliverTo() const { return Udm::AClassAssocAttr<PICML::deliverTo, PICML::PublishConnector>(impl, meta_srcdeliverTo, meta_srcdeliverTo_rev); }
		template<class Pred> Udm::AClassAssocAttr<PICML::deliverTo, PICML::PublishConnector, Pred> srcdeliverTo_sorted(const Pred &) const { return Udm::AClassAssocAttr<PICML::deliverTo, PICML::PublishConnector, Pred>(impl, meta_srcdeliverTo, meta_srcdeliverTo_rev); }

		static Uml::AssociationRole meta_ref;
		Udm::PointerAttr<PICML::Event> ref() const { return Udm::PointerAttr<PICML::Event>(impl, meta_ref); }

		static Uml::AssociationRole meta_srcemit, meta_srcemit_rev;
		Udm::AClassPointerAttr<PICML::emit, PICML::OutEventPort> srcemit() const { return Udm::AClassPointerAttr<PICML::emit, PICML::OutEventPort>(impl, meta_srcemit, meta_srcemit_rev); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

	class  ComponentFactory :  public HasOperations, public SupportsInterfaces {
	 public:
		static Uml::Class meta;

		ComponentFactory() { }
		ComponentFactory(Udm::ObjectImpl *impl) : HasOperations(impl),SupportsInterfaces(impl), Inheritable(impl), MgaObject(impl) { }
		ComponentFactory(const ComponentFactory &master) : HasOperations(master),SupportsInterfaces(master), Inheritable(master), MgaObject(master) { }
		static ComponentFactory Cast(const Udm::Object &a) { return __Cast(a, meta); }

		static ComponentFactory Create(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

		ComponentFactory CreateInstance(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

		ComponentFactory CreateDerived(const Udm::Object &parent, const Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

		Udm::InstantiatedAttr<ComponentFactory> Instances() { return Udm::InstantiatedAttr<PICML::ComponentFactory>(impl);}
		template <class Pred> Udm::InstantiatedAttr<ComponentFactory, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<PICML::ComponentFactory, Pred>(impl);}

		Udm::DerivedAttr<PICML::ComponentFactory> Derived() { return Udm::DerivedAttr<PICML::ComponentFactory>(impl);}
		template <class Pred> Udm::DerivedAttr<PICML::ComponentFactory, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<PICML::ComponentFactory, Pred>(impl);}

		Udm::ArchetypeAttr<PICML::ComponentFactory> Archetype() { return Udm::ArchetypeAttr<PICML::ComponentFactory>(impl);}

		static Uml::AssociationRole meta_dstManagesComponent, meta_dstManagesComponent_rev;
		Udm::AClassPointerAttr<PICML::ManagesComponent, PICML::Manageable> dstManagesComponent() const { return Udm::AClassPointerAttr<PICML::ManagesComponent, PICML::Manageable>(impl, meta_dstManagesComponent, meta_dstManagesComponent_rev); }

		static Uml::CompositionChildRole meta_LookupKey_child;
		Udm::ChildAttr<PICML::LookupKey> LookupKey_child() const { return Udm::ChildAttr<PICML::LookupKey>(impl, meta_LookupKey_child); }

		static Uml::CompositionChildRole meta_FactoryOperation_children;
		Udm::ChildrenAttr<PICML::FactoryOperation> FactoryOperation_children() const { return Udm::ChildrenAttr<PICML::FactoryOperation>(impl, meta_FactoryOperation_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::FactoryOperation, Pred> FactoryOperation_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::FactoryOperation, Pred>(impl, meta_FactoryOperation_children); }

		static Uml::CompositionChildRole meta_LookupOperation_children;
		Udm::ChildrenAttr<PICML::LookupOperation> LookupOperation_children() const { return Udm::ChildrenAttr<PICML::LookupOperation>(impl, meta_LookupOperation_children); }
		 template <class Pred> Udm::ChildrenAttr<PICML::LookupOperation, Pred> LookupOperation_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::LookupOperation, Pred>(impl, meta_LookupOperation_children); }

		Udm::ChildrenAttr<PICML::LookupKey> LookupKey_kind_children() const { return Udm::ChildrenAttr<PICML::LookupKey>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::LookupKey, Pred> LookupKey_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::LookupKey, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::OperationBase> OperationBase_kind_children() const { return Udm::ChildrenAttr<PICML::OperationBase>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::OperationBase, Pred> OperationBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::OperationBase, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::FactoryOperation> FactoryOperation_kind_children() const { return Udm::ChildrenAttr<PICML::FactoryOperation>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::FactoryOperation, Pred> FactoryOperation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::FactoryOperation, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::LookupOperation> LookupOperation_kind_children() const { return Udm::ChildrenAttr<PICML::LookupOperation>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::LookupOperation, Pred> LookupOperation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::LookupOperation, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::HasExceptions> HasExceptions_kind_children() const { return Udm::ChildrenAttr<PICML::HasExceptions>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::HasExceptions, Pred> HasExceptions_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::HasExceptions, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::Taggable> Taggable_kind_children() const { return Udm::ChildrenAttr<PICML::Taggable>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::Taggable, Pred> Taggable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::Taggable, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ChildrenAttr<PICML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr<PICML::MgaObject>(impl, Udm::NULLCHILDROLE); }
		template<class Pred> Udm::ChildrenAttr<PICML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr<PICML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

		Udm::ParentAttr<PICML::MgaObject> parent() const { return Udm::ParentAttr<PICML::MgaObject>(impl, Udm::NULLPARENTROLE); }
	};

}

#endif //MOBIES_PICML_H
