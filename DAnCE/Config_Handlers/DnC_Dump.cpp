// $Id$

#include "ace/Log_Msg.h"
#include "tao/SystemException.h"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "DnC_Dump.h"
#include "DnC_Dump_T.h"
#include <iostream>

using namespace ::Deployment::DnC_Dump_T;
namespace Deployment
{
  void
  DnC_Dump::dump (const char* caption, const ::CORBA::StringSeq &str_seq)
  {
    CORBA::ULong size = str_seq.length ();

    if (size != 0)
      {
        Dump_Obj dump_obj (caption);

        for (CORBA::ULong i = 0; i < size; ++i)
          {
            DANCE_DEBUG (1, (LM_DEBUG, "%s%s %d:\n",
                        Dump_Obj::indent(), caption, i));
            DANCE_DEBUG (1, (LM_DEBUG, "%s  %s:\n",
                        Dump_Obj::indent(), str_seq[i].in ()));
          }
      }
  }

  // Dumps a string
  void DnC_Dump::dump (const char* caption, const TAO::String_Manager& str)
  {
    DANCE_DEBUG (1, (LM_DEBUG, "%s%s: %s\n", Dump_Obj::indent(),
                caption, str.in()));
  }

  // Dumps a boolean
  void DnC_Dump::dump (const char* caption, const CORBA::Boolean& val)
  {
    DANCE_DEBUG (1, (LM_DEBUG, "%s%s: %s\n", Dump_Obj::indent(),
                caption, val ? "true" : "false"));
  }

  /*
   *  Object dump methods
   */

  // SatisfierProperty
  void DnC_Dump::dump (const Deployment::SatisfierProperty& sp)
  {
    Dump_Obj dump_obj("SatisfierProperty");
    dump ("name", sp.name);

    DANCE_DEBUG (1, (LM_DEBUG, "%skind: ", Dump_Obj::indent()));
    switch (sp.kind) {
    case Quantity: DANCE_DEBUG (1, (LM_DEBUG, "Quantity\n")); break;
    case Capacity: DANCE_DEBUG (1, (LM_DEBUG, "Capacity\n")); break;
    case Minimum: DANCE_DEBUG (1, (LM_DEBUG, "Minimum\n")); break;
    case Maximum: DANCE_DEBUG (1, (LM_DEBUG, "Maximum\n")); break;
    case Attribute: DANCE_DEBUG (1, (LM_DEBUG, "Attribute\n")); break;
    case Selection: DANCE_DEBUG (1, (LM_DEBUG, "Selection\n")); break;
    }

    DANCE_DEBUG (1, (LM_DEBUG, "%svalue:\n", Dump_Obj::indent()));
    dump (sp.value);
  }

  // SharedResource
  void DnC_Dump::dump (const Deployment::SharedResource& sr)
  {
    Dump_Obj dump_obj("SharedResource");
    dump ("name", sr.name);
    dump ("resourceType", sr.resourceType);
#if 0
    dump_ref_seq<Deployment::Domain> ("nodeRef",
                                      sr.nodeRef,
                                      "Domain",
                                      &Domain::node);
#endif /*if 0*/
    dump_sequence ("property",
                   sr.property);
  }

  // Resource
  void DnC_Dump::dump (const Deployment::Resource& resource)
  {
    Dump_Obj dump_obj("Resource");
    dump ("name", resource.name);
    dump ("resourceType", resource.resourceType);
    dump_sequence ("property",
                   resource.property);
  }

  // Node
  void DnC_Dump::dump (const Deployment::Node& node)
  {
    Dump_Obj dump_obj("Node");
    dump ("name", node.name);
    dump ("label", node.label);

#if (_MSC_VER)
    dump_ref_seq<Deployment::Domain> (
                                      "sharedResourceRef",
                                      node.sharedResourceRef,
                                      "Domain",
                                      &Domain::sharedResource);
#endif

#if (_MSC_VER)
    dump_ref_seq<Deployment::Domain> (
                                      "connectionRef",
                                      node.connectionRef,
                                      "Domain",
                                      &Domain::interconnect);
#endif

    dump_sequence ("resource",
                   node.resource);

  }

  // Interconnect
  void DnC_Dump::dump (const Deployment::Interconnect& conn)
  {
    Dump_Obj dump_obj("Interconnect");
    dump ("name", conn.name);
    dump ("label", conn.label);

#if (_MSC_VER)
    dump_ref_seq<Deployment::Domain> (
                                      "connectionRef",
                                      conn.connectionRef,
                                      "Domain",
                                      &Domain::bridge);
#endif
#if (_MSC_VER)
    dump_ref_seq<Deployment::Domain> (
                                      "connectRef",
                                      conn.connectRef,
                                      "Domain",
                                      &Domain::node);
#endif
    dump_sequence ("resource",
                   conn.resource);
  }

  // Bridge
  void
  DnC_Dump::dump (const Deployment::Bridge& bridge)
  {
    Dump_Obj dump_obj("Bridge");
    dump ("name", bridge.name);
    dump ("label", bridge.label);

#if (_MSC_VER)
    dump_ref_seq<Deployment::Domain> (
                                      "connectRef",
                                      bridge.connectRef,
                                      "Domain",
                                      &Domain::interconnect);
#endif

    dump_sequence ("resource", bridge.resource);
  }

  void DnC_Dump::dump (const ::Deployment::Domain &domain)
  {
    CORBA::Any val;
    val <<= domain;

    Dump_Obj dump_obj ("Domain",
                       val);

    dump ("UUID",
          domain.UUID);
    dump ("label",
          domain.label);

    dump_sequence ("sharedResource",
                   domain.sharedResource);
    dump_sequence ("node",
                   domain.node);
    dump_sequence ("interconnect",
                   domain.interconnect);
    dump_sequence ("bridge",
                   domain.bridge);

    dump_sequence ("infoProperty",
                   domain.infoProperty);
  }

  // ComponentPortDescription

  void DnC_Dump::dump (const ::Deployment::ComponentPortDescription
                       &compportdesc)
  {
    Dump_Obj dump_obj("ComponentPortDescription");

    dump ("name", compportdesc.name);
    dump ("specificType", compportdesc.specificType);
    dump ("supportedType", compportdesc.supportedType);
    dump ("provider", compportdesc.provider);
    dump ("exclusiveProvider", compportdesc.exclusiveProvider);
    dump ("exclusiveUser", compportdesc.exclusiveUser);
    dump ("optional", compportdesc.optional);

    DANCE_DEBUG (1, (LM_DEBUG, "%skind: ", Dump_Obj::indent()));
    switch (compportdesc.kind) {
    case Facet: DANCE_DEBUG (1, (LM_DEBUG, "Facet\n")); break;
    case SimplexReceptacle: DANCE_DEBUG (1, (LM_DEBUG, "SimplexReceptacle\n")); break;
    case MultiplexReceptacle: DANCE_DEBUG (1, (LM_DEBUG, "MultiplexReceptacle\n")); break;
    case EventEmitter: DANCE_DEBUG (1, (LM_DEBUG, "EventEmitter\n")); break;
    case EventPublisher: DANCE_DEBUG (1, (LM_DEBUG, "EventPublisher\n")); break;
    case EventConsumer: DANCE_DEBUG (1, (LM_DEBUG, "EventConsumer\n")); break;
    default: DANCE_DEBUG (1, (LM_DEBUG, "Unknown port kind\n")); break;
    }
  }

  // ComponentPropertyDescription

  void DnC_Dump::dump (const ::Deployment::ComponentPropertyDescription &comppropdesc)
  {
    Dump_Obj dump_obj("ComponentPropertyDescription");

    dump ("name", comppropdesc.name);

    DANCE_DEBUG (1, (LM_DEBUG, "%stype: ", Dump_Obj::indent()));
    switch (comppropdesc.type.in()->kind()) {
    case ::CORBA::tk_null: DANCE_DEBUG (1, (LM_DEBUG, "tk_null\n")); break;
    case ::CORBA::tk_void: DANCE_DEBUG (1, (LM_DEBUG, "tk_void\n")); break;
    case ::CORBA::tk_short: DANCE_DEBUG (1, (LM_DEBUG, "tk_short\n")); break;
    case ::CORBA::tk_long: DANCE_DEBUG (1, (LM_DEBUG, "tk_long\n")); break;
    case ::CORBA::tk_ushort: DANCE_DEBUG (1, (LM_DEBUG, "tk_ushort\n")); break;
    case ::CORBA::tk_ulong: DANCE_DEBUG (1, (LM_DEBUG, "tk_ulong\n")); break;
    case ::CORBA::tk_float: DANCE_DEBUG (1, (LM_DEBUG, "tk_float\n")); break;
    case ::CORBA::tk_double: DANCE_DEBUG (1, (LM_DEBUG, "tk_double\n")); break;
    case ::CORBA::tk_boolean: DANCE_DEBUG (1, (LM_DEBUG, "tk_boolean\n")); break;
    case ::CORBA::tk_char: DANCE_DEBUG (1, (LM_DEBUG, "tk_char\n")); break;
    case ::CORBA::tk_octet: DANCE_DEBUG (1, (LM_DEBUG, "tk_octet\n")); break;
    case ::CORBA::tk_any: DANCE_DEBUG (1, (LM_DEBUG, "tk_any\n")); break;
    case ::CORBA::tk_TypeCode: DANCE_DEBUG (1, (LM_DEBUG, "tk_TypeCode\n")); break;
    case ::CORBA::tk_Principal: DANCE_DEBUG (1, (LM_DEBUG, "tk_Principal\n")); break;
    case ::CORBA::tk_objref: DANCE_DEBUG (1, (LM_DEBUG, "tk_objref\n")); break;
    case ::CORBA::tk_struct: DANCE_DEBUG (1, (LM_DEBUG, "tk_struct\n")); break;
    case ::CORBA::tk_union: DANCE_DEBUG (1, (LM_DEBUG, "tk_union\n")); break;
    case ::CORBA::tk_enum: DANCE_DEBUG (1, (LM_DEBUG, "tk_enum\n")); break;
    case ::CORBA::tk_string: DANCE_DEBUG (1, (LM_DEBUG, "tk_string\n")); break;
    case ::CORBA::tk_sequence: DANCE_DEBUG (1, (LM_DEBUG, "tk_sequence\n")); break;
    case ::CORBA::tk_array: DANCE_DEBUG (1, (LM_DEBUG, "tk_array\n")); break;
    case ::CORBA::tk_alias: DANCE_DEBUG (1, (LM_DEBUG, "tk_alias\n")); break;
    case ::CORBA::tk_except: DANCE_DEBUG (1, (LM_DEBUG, "tk_except\n")); break;
    case ::CORBA::tk_longlong: DANCE_DEBUG (1, (LM_DEBUG, "tk_longlong\n")); break;
    case ::CORBA::tk_ulonglong: DANCE_DEBUG (1, (LM_DEBUG, "tk_ulonglong\n")); break;
    case ::CORBA::tk_longdouble: DANCE_DEBUG (1, (LM_DEBUG, "tk_longdouble\n")); break;
    case ::CORBA::tk_wchar: DANCE_DEBUG (1, (LM_DEBUG, "tk_wchar\n")); break;
    case ::CORBA::tk_wstring: DANCE_DEBUG (1, (LM_DEBUG, "tk_wstring\n")); break;
    case ::CORBA::tk_fixed: DANCE_DEBUG (1, (LM_DEBUG, "tk_fixed\n")); break;
    case ::CORBA::tk_value: DANCE_DEBUG (1, (LM_DEBUG, "tk_value\n")); break;
    case ::CORBA::tk_value_box: DANCE_DEBUG (1, (LM_DEBUG, "tk_value_box\n")); break;
    case ::CORBA::tk_native: DANCE_DEBUG (1, (LM_DEBUG, "tk_native\n")); break;
    case ::CORBA::tk_abstract_interface: DANCE_DEBUG (1, (LM_DEBUG, "tk_abstract_interface\n")); break;
    case ::CORBA::tk_local_interface: DANCE_DEBUG (1, (LM_DEBUG, "tk_local_interface\n")); break;
    case ::CORBA::tk_component: DANCE_DEBUG (1, (LM_DEBUG, "tk_component\n")); break;
    case ::CORBA::tk_home: DANCE_DEBUG (1, (LM_DEBUG, "tk_home\n")); break;
    case ::CORBA::tk_event: DANCE_DEBUG (1, (LM_DEBUG, "tk_event\n")); break;
    default:
      break;
    };
  }

  // ComponentInterfaceDescription

  void DnC_Dump::dump (const ::Deployment::ComponentInterfaceDescription &cid)
  {
    Dump_Obj dump_obj("ComponentInterfaceDescription");
    dump ("label", cid.label);
    dump ("UUID", cid.UUID);
    dump ("specificType", cid.specificType);
    dump ("supportedType", cid.supportedType);
    dump ("idlFile", cid.idlFile);
    dump_sequence ("configProperty", cid.configProperty);
    dump_sequence ("port", cid.port);
    dump_sequence ("property", cid.property);
    dump_sequence ("infoProperty", cid.infoProperty);
  }

  // Requirement

  void DnC_Dump::dump (const ::Deployment::Requirement &req)
  {
    Dump_Obj dump_obj("Requirement");

    dump ("resourceType", req.resourceType);
    dump ("name", req.name);
    dump_sequence ("property", req.property);
  }

  // MonolithicDeploymentDescription

  void DnC_Dump::dump(const Deployment::MonolithicDeploymentDescription &mdd)
  {
    Dump_Obj dump_obj("MonolithicDeploymentDescription");

    dump ("name", mdd.name);
    dump ("source", mdd.source);

#if (_MSC_VER)
    dump_ref_seq<Deployment::DeploymentPlan> ("artifactRef", mdd.artifactRef,
                                              "DeploymentPlan",
                                              &DeploymentPlan::artifact);
#endif
    dump_sequence ("execParameter", mdd.execParameter);
    dump_sequence ("deployRequirement", mdd.deployRequirement);
  }

  // ResourceUsageKind

  void DnC_Dump::dump(const Deployment::ResourceUsageKind &ruk)
  {
    DANCE_DEBUG (1, (LM_DEBUG, "%sresourceUsage: ", Dump_Obj::indent()));
    switch (ruk) {
    case None: DANCE_DEBUG (1, (LM_DEBUG, "None\n")); break;
    case InstanceUsesResource: DANCE_DEBUG (1, (LM_DEBUG, "InstanceUsesResource\n")); break;
    case ResourceUsesInstance: DANCE_DEBUG (1, (LM_DEBUG, "ResourceUsesInstance\n")); break;
    case PortUsesResource: DANCE_DEBUG (1, (LM_DEBUG, "PortUsesResource\n")); break;
    case ResourceUsesPort: DANCE_DEBUG (1, (LM_DEBUG, "ResourceUsesPort\n")); break;
    }
  }

  // InstanceResourceDeploymentDescription

  void DnC_Dump::dump(const Deployment::InstanceResourceDeploymentDescription &irdd)
  {
    Dump_Obj dump_obj("InstanceResourceDeploymentDescription");

    dump (irdd.resourceUsage);
    dump ("requirementName", irdd.requirementName);
    dump ("resourceName", irdd.resourceName);
    DANCE_DEBUG (1, (LM_DEBUG, "%sresourceValue:\n", Dump_Obj::indent()));
    dump_sequence ("property",  irdd.property);
  }

  // InstanceDeploymentDescription

  void DnC_Dump::dump(const Deployment::InstanceDeploymentDescription &idd)
  {
    Dump_Obj dump_obj("InstanceDeploymentDescription");

    dump ("name", idd.name);
    dump ("node", idd.node);
    dump ("source", idd.source);

#if (_MSC_VER)
    dump_ref<Deployment::DeploymentPlan> ("implementationRef",
                                          idd.implementationRef,
                                          "DeploymentPlan",
                                          &DeploymentPlan::implementation);
#endif
    dump_sequence ("configProperty", idd.configProperty);
    dump_sequence ("deployedResource", idd.deployedResource);
    dump_sequence ("deployedSharedResource", idd.deployedSharedResource);
  }

  // ComponentExternalPortEndpoint

  void DnC_Dump::dump (const ::Deployment::ComponentExternalPortEndpoint &cepe)
  {
    Dump_Obj dump_obj("ComponentExternalPortEndpoint");

    dump ("portName", cepe.portName);
  }

  // PlanSubcomponentPortEndpoint

  void DnC_Dump::dump(const Deployment::PlanSubcomponentPortEndpoint &pspe)
  {
    Dump_Obj dump_obj("PlanSubcomponentPortEndpoint");

    dump ("portName", pspe.portName);
    dump ("provider", pspe.provider);
    DANCE_DEBUG (1, (LM_DEBUG, "%skind: ", Dump_Obj::indent()));
    switch (pspe.kind) {
    case Facet: DANCE_DEBUG (1, (LM_DEBUG, "Facet\n")); break;
    case SimplexReceptacle: DANCE_DEBUG (1, (LM_DEBUG, "SimplexReceptacle\n")); break;
    case MultiplexReceptacle: DANCE_DEBUG (1, (LM_DEBUG, "MultiplexReceptacle\n")); break;
    case EventEmitter: DANCE_DEBUG (1, (LM_DEBUG, "EventEmitter\n")); break;
    case EventPublisher: DANCE_DEBUG (1, (LM_DEBUG, "EventPublisher\n")); break;
    case EventConsumer: DANCE_DEBUG (1, (LM_DEBUG, "EventConsumer\n")); break;
    default: DANCE_DEBUG (1, (LM_DEBUG, "Unknown port kind\n")); break;
    }
#if (_MSC_VER)
    dump_ref<Deployment::DeploymentPlan> ("instanceRef", pspe.instanceRef,
                                          "DeploymentPlan",
                                          &DeploymentPlan::instance);
#endif
  }

  // ExternalReferenceEndpoint

  void DnC_Dump::dump (const ::Deployment::ExternalReferenceEndpoint &ere)
  {
    Dump_Obj dump_obj("ExternalReferenceEndpoint");

    dump ("location", ere.location);
  }

  // ConnectionResourceDeploymentDescription

  void DnC_Dump::dump(const Deployment::ConnectionResourceDeploymentDescription &crdd)
  {
    Dump_Obj dump_obj("ConnectionResourceDeploymentDescription");

    dump ("targetName", crdd.targetName);
    dump ("requirementName", crdd.requirementName);
    dump ("resourceName", crdd.resourceName);
    DANCE_DEBUG (1, (LM_DEBUG, "%sresourceValue:\n", Dump_Obj::indent()));
    dump_sequence ("properties", crdd.property);
  }

  // PlanConnectionDescription

  void DnC_Dump::dump(const Deployment::PlanConnectionDescription &pcd)
  {
    Dump_Obj dump_obj("PlanConnectionDescription");

    dump ("name", pcd.name);
    dump ("source", pcd.source);
    dump_sequence ("deployRequirement", pcd.deployRequirement);
    dump_sequence ("externalEndpoint", pcd.externalEndpoint);
    dump_sequence ("internalEndpoint", pcd.internalEndpoint);
    dump_sequence ("externalReference", pcd.externalReference);
    dump_sequence ("deployedResource", pcd.deployedResource);
  }

  // PlanSubcomponentPropertyReference

  void DnC_Dump::dump(const Deployment::PlanSubcomponentPropertyReference &pspr)
  {
    Dump_Obj dump_obj("PlanSubcomponentPropertyReference");

    dump ("propertyName", pspr.propertyName);

#if (_MSC_VER)
    dump_ref<Deployment::DeploymentPlan> ("instanceRef", pspr.instanceRef,
                                          "DeploymentPlan",
                                          &DeploymentPlan::instance);
#endif
  }

  // PlanPropertyMapping

  void DnC_Dump::dump(const Deployment::PlanPropertyMapping &ppm)
  {
    Dump_Obj dump_obj("PlanPropertyMapping");

    dump ("name", ppm.name);
    dump ("source", ppm.source);
    dump ("externalName", ppm.externalName);
    dump_sequence ("delegatesTo", ppm.delegatesTo);
  }

  // ImplementationDependency

  void DnC_Dump::dump(const Deployment::ImplementationDependency &id)
  {
    Dump_Obj dump_obj("ImplementationDependency");

    dump ("requiredType", id.requiredType);
  }

  // ResourceDeploymentDescription

  void DnC_Dump::dump(const Deployment::ResourceDeploymentDescription &rdd)
  {
    Dump_Obj dump_obj("ResourceDeploymentDescription");

    dump ("requirementName", rdd.requirementName);
    dump ("resourceName", rdd.resourceName);
    DANCE_DEBUG (1, (LM_DEBUG, "%sresourceValue:\n", Dump_Obj::indent()));
    dump_sequence ("properties", rdd.property);
  }

  // ArtifactDeploymentDescription

  void DnC_Dump::dump(const Deployment::ArtifactDeploymentDescription &add)
  {
    Dump_Obj dump_obj("ArtifactDeploymentDescription");

    dump ("name", add.name);
    dump ("location", add.location);
    dump ("node", add.node);
    dump ("source", add.source);
    dump_sequence ("execParameter", add.execParameter);
    dump_sequence ("deployRequirement", add.deployRequirement);
    dump_sequence ("deployedResource", add.deployedResource);
  }

  void DnC_Dump::dump(const Deployment::DeploymentPlan &plan)
  {
    CORBA::Any val;

    val <<= plan;

    Dump_Obj dump_obj("DeploymentPlan", val);

    dump ("label", plan.label);
    dump ("UUID", plan.UUID);
    dump (plan.realizes);
    dump_sequence ("implementation", plan.implementation);
    dump_sequence ("instance", plan.instance);
    //dump_sequence ("connection", plan.connection);
    dump_sequence ("externalProperty", plan.externalProperty);
    dump_sequence ("dependsOn", plan.dependsOn);
    dump_sequence ("artifact", plan.artifact);
    dump_sequence ("infoProperty", plan.infoProperty);
  }

  // ComponentPackageReference

  void DnC_Dump::dump (const ::Deployment::ComponentPackageReference &cpr)
  {
    Dump_Obj dump_obj("ComponentPackageReference");

    dump ("requiredUUID", cpr.requiredUUID);
    dump ("requiredName", cpr.requiredName);
    dump ("requiredType", cpr.requiredType.specificType);
  }

  // SubcomponentInstantiationDescription

  void DnC_Dump::dump (const ::Deployment::SubcomponentInstantiationDescription &sid)
  {
    Dump_Obj dump_obj("SubcomponentInstantiationDescription");

    dump ("name", sid.name);
    //    dump_sequence ("basePackage", sid.package);
    dump_sequence ("configProperty", sid.configProperty);
    dump_sequence ("selectRequirement", sid.selectRequirement);
    //    dump_sequence ("reference", sid.reference);
  }

  // SubcomponentPortEndpoint

  void DnC_Dump::dump (const ::Deployment::SubcomponentPortEndpoint& spe)
  {
    Dump_Obj dump_obj("SubcomponentPortEndpoint");

    dump ("portName", spe.portName);
    int value = spe.instanceRef;
    ::Deployment::ComponentAssemblyDescription *v;
    const CORBA::Any &a =
      Dump_Obj::desc ("ComponentAssemblyDescription");

    a >>= v;

    DANCE_DEBUG (1, (LM_DEBUG, "%sinstance: %s\n", Dump_Obj::indent (),
                v->instance[value].name.in ()));
  }

  // AssemblyConnectionDescription

  void DnC_Dump::dump (const ::Deployment::AssemblyConnectionDescription &acd)
  {
    Dump_Obj dump_obj("AssemblyConnectionDescription");

    dump ("name", acd.name);
    dump_sequence ("deployRequirement", acd.deployRequirement);
    dump_sequence ("externalEndpoint", acd.externalEndpoint);
    dump_sequence ("internalEndpoint", acd.internalEndpoint);
    dump_sequence ("externalReference", acd.externalReference);
  }

  // SubcomponentPropertyReference

  void DnC_Dump::dump (const ::Deployment::SubcomponentPropertyReference &spr)
  {
    Dump_Obj dump_obj("SubcomponentPropertyReference");

    dump ("propertyName", spr.propertyName);
    int value = spr.instanceRef;

    ::Deployment::ComponentAssemblyDescription *v;

    const CORBA::Any &a =
      Dump_Obj::desc ("ComponentAssemblyDescription");

    a >>= v;

    DANCE_DEBUG (1, (LM_DEBUG, "%sinstance: %s\n", Dump_Obj::indent (),
                v->instance[value].name.in ()));
  }

  void
  DnC_Dump::dump (const ::Deployment::AssemblyPropertyMapping &apm)
  {
    Dump_Obj dump_obj("AssemblyPropertyMapping");

    dump ("name", apm.name);
    dump ("externalName", apm.externalName);
    dump_sequence ("delegatesTo", apm.delegatesTo);
  }

  // ComponentAssemblyDescription

  void DnC_Dump::dump (const ::Deployment::ComponentAssemblyDescription& cad)
  {
    CORBA::Any a;
    a <<= cad;

    Dump_Obj dump_obj ("ComponentAssemblyDescription",
                       a);

    dump_sequence ("instance", cad.instance);
    dump_sequence ("connection", cad.connection);
    dump_sequence ("externalProperty", cad.externalProperty);
  }

  // ImplementationArtifactDescription

  void DnC_Dump::dump (const ::Deployment::ImplementationArtifactDescription &iad)
  {
    Dump_Obj dump_obj("ImplementationArtifactDescription");

    dump ("label", iad.label);
    dump ("UUID", iad.UUID);
    dump ("location", iad.location);
    dump_sequence ("execParameter", iad.execParameter);
    dump_sequence ("deployRequirement", iad.deployRequirement);
    dump_sequence ("dependsOn", iad.dependsOn);
    dump_sequence ("infoProperty", iad.infoProperty);
  }

  // NamedImplementationArtifact

  void DnC_Dump::dump (const ::Deployment::NamedImplementationArtifact &nia)
  {
    Dump_Obj dump_obj("NamedImplementationArtifact");

    dump ("name", nia.name);
    dump (nia.referencedArtifact);
  }

  // ImplementationRequirement

  void DnC_Dump::dump (const ::Deployment::ImplementationRequirement &ir)
  {
    Dump_Obj dump_obj("ImplementationRequirement");

    dump_sequence ("resourceUsage", ir.resourceUsage);
    dump ("resourcePort", ir.resourcePort);
    dump ("componentPort", ir.componentPort);
    dump ("resourceType", ir.resourceType);
    dump ("name", ir.name);
    dump_sequence ("property", ir.property);
  }

  // MonolithicImplementationDescription

  void DnC_Dump::dump (const ::Deployment::MonolithicImplementationDescription
                       &mid)
  {
    Dump_Obj dump_obj("MonolithicImplementationDescription");

    //    dump_sequence ("execParameter", mid.execParameter);
    dump_sequence ("primaryArtifact", mid.primaryArtifact);
    dump_sequence ("deployRequirement", mid.deployRequirement);
  }

  void
  DnC_Dump::dump (const ::Deployment::Capability &capability)
  {
    Dump_Obj dump_obj("Capability");

    dump ("name", capability.name);
    DnC_Dump::dump ("resourceType", capability.resourceType);
    dump_sequence ("property",
                   capability.property);
  }

  // ComponentImplementationDescription

  void DnC_Dump::dump (
                       const ::Deployment::ComponentImplementationDescription &cid)
  {
    Dump_Obj dump_obj("ComponentImplementationDescription");

    dump ("label", cid.label);
    dump ("UUID", cid.UUID);
    DANCE_DEBUG (1, (LM_DEBUG, "%sImplements:\n", Dump_Obj::indent()));
    dump (cid.implements);
    dump_sequence ("assemblyImpl", cid.assemblyImpl);
    dump_sequence ("monolithicImpl", cid.monolithicImpl);
    dump_sequence ("configProperty", cid.configProperty);
    dump_sequence ("capability", cid.capability);
    dump_sequence ("dependsOn", cid.dependsOn);
    dump_sequence ("infoProperty", cid.infoProperty);
  }

  // PackagedComponentImplementation

  void DnC_Dump::dump (const ::Deployment::PackagedComponentImplementation &pci)
  {
    Dump_Obj dump_obj("PackagedComponentImplementation");

    dump ("Name", pci.name);
    DANCE_DEBUG (1, (LM_DEBUG,
                "%sreferencedImplementation:\n", Dump_Obj::indent()));
    DnC_Dump::dump (pci.referencedImplementation);
  }

  // ComponentPackageDescription

  void DnC_Dump::dump (const ::Deployment::ComponentPackageDescription
                       &comppkgdesc)
  {
    Dump_Obj dump_obj("ComponentPackageDescription");

    dump ("label", comppkgdesc.label);
    dump ("UUID", comppkgdesc.UUID);
    DANCE_DEBUG (1, (LM_DEBUG, "%srealizes:\n", Dump_Obj::indent ()));
    DnC_Dump::dump (comppkgdesc.realizes); // ComponentInterfaceDescription
    dump_sequence ("configProperty", comppkgdesc.configProperty);
    dump_sequence ("implementation", comppkgdesc.implementation);
    dump_sequence ("infoProperty", comppkgdesc.infoProperty);
  }

  // PackageConfiguration

  void DnC_Dump::dump (const ::Deployment::PackageConfiguration &pc)
  {
    Dump_Obj dump_obj("PackageConfiguration");

    dump ("label", pc.label);
    dump ("UUID", pc.UUID);
    dump_sequence ("specializedConfig", pc.specializedConfig);
    dump_sequence ("basePackage", pc.basePackage);
    dump_sequence ("reference", pc.referencedPackage);
    dump_sequence ("selectRequirement", pc.selectRequirement);
    dump_sequence ("configProperty", pc.configProperty);
  }

  // Property
  void DnC_Dump::dump (const Deployment::Property& property)
  {
    Dump_Obj dump_obj("Property");
    dump ("name", property.name);
    DANCE_DEBUG (1, (LM_DEBUG, "%svalue:\n", Dump_Obj::indent()));
    dump (property.value);
  }

  // CORBA::Any

  void DnC_Dump::dump (const ::CORBA::Any &any)
  {
    ::CORBA::TypeCode * type = any.type ();
    switch (type->kind ())
      {
      case CORBA::tk_short:
        {
          CORBA::Short temp;
          if (! (any >>= temp))
            {
              DANCE_DEBUG (1, (LM_DEBUG, "DnC_Dump::dump (CORBA::Any), expected short\
encoded different type"));
              throw CORBA::INTERNAL ();
            }
          DANCE_DEBUG (1, (LM_DEBUG, "%sAny value: %d\n", Dump_Obj::indent (),
                      temp));
        }
        break;

      case CORBA::tk_null:
        DANCE_DEBUG (1, (LM_DEBUG, "%sAny value: null value encoded\n",
                    Dump_Obj::indent ()));
        break;

      case CORBA::tk_void:
        DANCE_DEBUG (1, (LM_DEBUG, "Any value: void type encoded\n"));
        break;
      case CORBA::tk_long:
        {
          CORBA::Long temp;
          if (! (any >>= temp))
            {
              DANCE_DEBUG (1, (LM_DEBUG, "DnC::dump (CORBA::Any) expected long\
encoded with different type"));
              throw CORBA::INTERNAL ();
            }
          DANCE_DEBUG (1, (LM_DEBUG, "%sAny value: %d\n", Dump_Obj::indent (),
                      temp));
        }
        break;

      case CORBA::tk_ushort:
        {
          CORBA::UShort temp;
          if (! (any >>= temp))
            {
              DANCE_DEBUG (1, (LM_DEBUG, "DnC::dump (CORBA::Any) expected u short\
encoded with different type"));
              throw CORBA::INTERNAL ();
            }
          DANCE_DEBUG (1, (LM_DEBUG, "%sAny value: %u\n", Dump_Obj::indent (),
                      temp));
        }
        break;

      case CORBA::tk_ulong:
        {
          CORBA::ULong temp;
          if (! (any >>= temp))
            {
              DANCE_DEBUG (1, (LM_DEBUG, "DnC::dump (CORBA::Any) expected ulong\
encoded with different type"));
              throw CORBA::INTERNAL ();
            }
          DANCE_DEBUG (1, (LM_DEBUG, "%sAny value: %u\n", Dump_Obj::indent (),
                      temp));
        }
        break;

      case CORBA::tk_float:
        {
          CORBA::Float temp;
          if (! (any >>= temp))
            {
              DANCE_DEBUG (1, (LM_DEBUG, "DnC::dump (CORBA::Any) expected float\
encoded with different type"));
              throw CORBA::INTERNAL ();
            }
          DANCE_DEBUG (1, (LM_DEBUG, "%sAny value: %f\n", Dump_Obj::indent (),
                      temp));
        }
        break;
      case CORBA::tk_double:
        {
          CORBA::Double temp;
          if (! (any >>= temp))
            {
              DANCE_DEBUG (1, (LM_DEBUG, "DnC::dump (CORBA::Any) expected double\
encoded with different type"));
              throw CORBA::INTERNAL ();
            }
          DANCE_DEBUG (1, (LM_DEBUG, "%sAny value: %f\n", Dump_Obj::indent (),
                      temp));
        }
        break;
      case CORBA::tk_boolean:
        {
          CORBA::Boolean temp;
          if (! (any >>= CORBA::Any::to_boolean (temp)))
            {
              DANCE_DEBUG (1, (LM_DEBUG, "DnC::dump (CORBA::Any) expected bool\
encoded with different type"));
              throw CORBA::INTERNAL ();
            }

          if (temp)
            DANCE_DEBUG (1, (LM_DEBUG, "Any value: True\n"));
          else
            DANCE_DEBUG (1, (LM_DEBUG, "Any value: False\n"));
        }
        break;

      case CORBA::tk_char:
        {
          CORBA::Char temp;
          if (! (any >>= CORBA::Any::to_char (temp)))
            {
              DANCE_DEBUG (1, (LM_DEBUG, "DnC::dump (CORBA::Any) expected char\
encoded with different type"));
              throw CORBA::INTERNAL ();
            }
          DANCE_DEBUG (1, (LM_DEBUG, "%sAny value: %c\n", Dump_Obj::indent (),
                      temp));
        }
        break;

      case CORBA::tk_octet:
        {
          CORBA::Octet temp;
          if (! (any >>= CORBA::Any::to_octet (temp)))
            {
              DANCE_DEBUG (1, (LM_DEBUG, "DnC::dump (CORBA::Any) expected octet\
encoded with different type"));
              throw CORBA::INTERNAL ();
            }
          DANCE_DEBUG (1, (LM_DEBUG, "%sAny value: %d\n", Dump_Obj::indent (),
                      temp));
        }
        break;

      case CORBA::tk_string:
        {
          const char * temp = 0;
          if (! (any >>= temp))
            {
              DANCE_DEBUG (1, (LM_DEBUG, "DnC::dump (CORBA::Any) expected string\
encoded with different type"));
              throw CORBA::INTERNAL ();
            }
          DANCE_DEBUG (1, (LM_DEBUG, "%sAny value: %s\n", Dump_Obj::indent (),
                      temp));
        }
        break;
      case CORBA::tk_longlong:
        {
          CORBA::LongLong temp;
          if (! (any >>= temp))
            {
              DANCE_DEBUG (1, (LM_DEBUG, "DnC::dump (CORBA::Any) expected longlong\
encoded with different type"));
              throw CORBA::INTERNAL ();
            }
          DANCE_DEBUG (1, (LM_DEBUG, "%sAny value: %l\n", Dump_Obj::indent (),
                      temp));
        }
        break;

      case CORBA::tk_longdouble:
        {
          CORBA::LongDouble temp;
          if (! (any >>= temp))
            {
              DANCE_DEBUG (1, (LM_DEBUG, "DnC::dump (CORBA::Any) expected longdouble\
encoded with different type"));
              throw CORBA::INTERNAL ();
            }
          DANCE_DEBUG (1, (LM_DEBUG, "%sAny value: %d\n", Dump_Obj::indent (),
                      temp));
        }

        break;
      case CORBA::tk_wchar:
        {
          CORBA::WChar temp;
          if (! (any >>= CORBA::Any::to_wchar (temp)))
            {
              DANCE_DEBUG (1, (LM_DEBUG, "DnC::dump (CORBA::Any) expected wchar\
encoded with different type"));
              throw CORBA::INTERNAL ();
            }
          DANCE_DEBUG (1, (LM_DEBUG, "%sAny value: %c\n", Dump_Obj::indent (),
                      temp));
        }
        break;

      case CORBA::tk_wstring:
        {
          const CORBA::WChar * temp;
          if (! (any >>= temp))
            {
              DANCE_DEBUG (1, (LM_DEBUG, "DnC::dump (CORBA::Any) expected wstring\
encoded with different type"));
              throw CORBA::INTERNAL ();
            }
          DANCE_DEBUG (1, (LM_DEBUG, "%sAny value: %s\n", Dump_Obj::indent (),
                      temp));
        }
        break;

        case CORBA::tk_enum:
        {
/*          CORBA::Enum temp;
          if (! (any >>= temp))
            {
              DANCE_DEBUG (1, (LM_DEBUG, "DnC::dump (CORBA::Any) expected enum \
encoded with different type"));
              throw CORBA::INTERNAL ();
            }
*/          DANCE_DEBUG (1, (LM_DEBUG, "%sAny value: some enum\n", Dump_Obj::indent ()));
        }
        break;

      default:
        DANCE_DEBUG (1, (LM_DEBUG, "Unknown type encoded in Any\n"));
        throw CORBA::INTERNAL ();
      }
  }
}
