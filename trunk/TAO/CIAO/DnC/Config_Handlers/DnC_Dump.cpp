// $Id$

#ifndef DNC_DUMP_C
#define DNC_DUMP_C

#include "ace/Log_Msg.h"

#include "DnC_Dump.h"
#include <iostream>
#include <string>

#include "tckind_names.h"

namespace Deployment
{
  /*
   *  Generic dump functions
   */

  class Dump_Obj {
  public:
    Dump_Obj(const char* caption) {
      ACE_DEBUG ((LM_DEBUG, "%s%s:\n", indent_.c_str(), caption));
      indent_.append("  ");
    }

    ~Dump_Obj() {
      indent_.erase(indent_.size() - 2, 2);
    }

    static const char* indent() {
      return indent_.c_str();
    }
   
  private:
    static std::string indent_;
  };

  std::string Dump_Obj::indent_ = "-";

  // Dumps a string sequence
  void DnC_Dump::dump (const char* caption, const ::CORBA::StringSeq &str_seq)
  {
    CORBA::ULong size = str_seq.length ();

    if (size != 0)
      {
        Dump_Obj dump_obj(caption);
        
        for (CORBA::ULong i = 0; i < size; ++i)
          {
            ACE_DEBUG ((LM_DEBUG, "%s%s%s ",
                        Dump_Obj::indent(), str_seq[i].in (), (i < size) ? ", " : "\n"));
          }
      }
  }

  // Dumps a sequence
  template <typename SEQUENCE>
  void DnC_Dump::dump_sequence (const char* caption, const SEQUENCE &seq)
  {
    CORBA::ULong size = seq.length ();

    if (size != 0)
      {
        Dump_Obj dump_obj(caption);
        
        for (CORBA::ULong i = 0; i < size; ++i)
          {
            ACE_DEBUG ((LM_DEBUG, "%s%s %d: \n", Dump_Obj::indent(), caption, i));
            DnC_Dump::dump (seq[i]);
          }
      }
  }

  // Dumps a string
  void DnC_Dump::dump (const char* caption, const TAO_String_Manager& str)
  {
    ACE_DEBUG ((LM_DEBUG, "%s%s: %s\n", Dump_Obj::indent(), caption, str.in()));
  }

  // Dumps a boolean
  void DnC_Dump::dump (const char* caption, const CORBA::Boolean& val)
  {
    ACE_DEBUG ((LM_DEBUG, "%s%s: %s\n", Dump_Obj::indent(), caption, val ? "true" : "false"));
  }

  /*
   *  Object dump methods
   */

  // SatisfierProperty
  void DnC_Dump::dump (const Deployment::SatisfierProperty& sp)
  {
    Dump_Obj dump_obj("SatisfierProperty");
    dump ("name", sp.name);

    ACE_DEBUG ((LM_DEBUG, "%skind: ", Dump_Obj::indent()));
    switch (sp.kind) {
    case Quantity: ACE_DEBUG ((LM_DEBUG, "Quantity\n")); break;
    case Capacity: ACE_DEBUG ((LM_DEBUG, "Capacity\n")); break;
    case Minimum: ACE_DEBUG ((LM_DEBUG, "Minimum\n")); break;
    case Maximum: ACE_DEBUG ((LM_DEBUG, "Maximum\n")); break;
    case Attribute: ACE_DEBUG ((LM_DEBUG, "Attribute\n")); break;
    case Selection: ACE_DEBUG ((LM_DEBUG, "Selection\n")); break;
    }

    ACE_DEBUG ((LM_DEBUG, "%svalue:\n", Dump_Obj::indent()));
    dump (sp.value);
  }

  // SharedResource
  void DnC_Dump::dump (const Deployment::SharedResource& sr)
  {
    Dump_Obj dump_obj("SharedResource");
    dump ("name", sr.name);
    dump ("resourceType", sr.resourceType);
    ACE_DEBUG ((LM_DEBUG, "%snodeRef: ???\n", Dump_Obj::indent()));
    dump_sequence ("property", sr.property);
  }

  // Resource
  void DnC_Dump::dump (const Deployment::Resource& resource)
  {
    Dump_Obj dump_obj("Resource");
    dump ("name", resource.name);
    dump ("resourceType", resource.resourceType);
    dump_sequence ("property", resource.property);
  }

  // Node
  void DnC_Dump::dump (const Deployment::Node& node)
  {
    Dump_Obj dump_obj("Node");
    dump ("name", node.name);
    dump ("label", node.label);
    ACE_DEBUG ((LM_DEBUG, "%ssharedResourceRef: ???\n", Dump_Obj::indent()));
    ACE_DEBUG ((LM_DEBUG, "%sconnectionRef: ???\n", Dump_Obj::indent()));
    dump_sequence ("resource", node.resource);
  }

  // Interconnect
  void DnC_Dump::dump (const Deployment::Interconnect& conn)
  {
    Dump_Obj dump_obj("Interconnect");
    dump ("name", conn.name);
    dump ("label", conn.label);
    ACE_DEBUG ((LM_DEBUG, "%sconnectionRef: ???\n", Dump_Obj::indent()));
    ACE_DEBUG ((LM_DEBUG, "%sconnectRef: ???\n", Dump_Obj::indent()));
    dump_sequence ("resource", conn.resource);
  }

  // Bridge
  void DnC_Dump::dump (const Deployment::Bridge& bridge)
  {
    Dump_Obj dump_obj("Bridge");
    dump ("name", bridge.name);
    dump ("label", bridge.label);
    ACE_DEBUG ((LM_DEBUG, "%sconnectionRef: ???\n", Dump_Obj::indent()));
    dump_sequence ("resource", bridge.resource);
  }

  // Property
  void DnC_Dump::dump (const Deployment::Property& property)
  {
    Dump_Obj dump_obj("Property");
    dump ("name", property.name);
    ACE_DEBUG ((LM_DEBUG, "%svalue:\n", Dump_Obj::indent()));
    dump (property.value);
  }

  // AssemblyConnectionDescription

  void DnC_Dump::dump (const ::Deployment::AssemblyConnectionDescription &acd)
  {
    Dump_Obj dump_obj("AssemblyConnectionDescription");

    ACE_DEBUG ((LM_DEBUG, "  name: %s\n", acd.name.in ()));

    ACE_DEBUG ((LM_DEBUG, "  deployRequirement: \n"));
    for (CORBA::ULong i = 0;
         i < acd.deployRequirement.length ();
         ++i)
      {
        DnC_Dump::dump (acd.deployRequirement[i]);
      }

    ACE_DEBUG ((LM_DEBUG, "  externalEndpoint: \n"));
    for (CORBA::ULong i = 0;
         i < acd.externalEndpoint.length ();
         ++i)
      {
        DnC_Dump::dump (acd.externalEndpoint[i]);
      }

    ACE_DEBUG ((LM_DEBUG, "  internalEndpoint: \n"));
    for (CORBA::ULong i = 0;
         i < acd.internalEndpoint.length ();
         ++i)
      {
        DnC_Dump::dump (acd.internalEndpoint[i]);
      }

    ACE_DEBUG ((LM_DEBUG, "  externalReference: \n"));
    for (CORBA::ULong i = 0;
         i < acd.externalReference.length ();
         ++i)
      {
        DnC_Dump::dump (acd.externalReference[i]);
      }
  }

  // AssemblyPropertyMapping

  void DnC_Dump::dump (const ::Deployment::AssemblyPropertyMapping &apm)
  {
    Dump_Obj dump_obj("AssemblyPropertyMapping");

    ACE_DEBUG ((LM_DEBUG, "  name: %s\n", apm.name.in ()));
    ACE_DEBUG ((LM_DEBUG, "  externalName: %s\n", apm.externalName.in ()));

    ACE_DEBUG ((LM_DEBUG, "  delegatesTo: \n"));
    for (CORBA::ULong i = 0;
         i < apm.delegatesTo.length ();
         ++i)
      {
        DnC_Dump::dump (apm.delegatesTo[i]); // SubcomponentPropertyReference
      }
  }

  // ComponentPackageDescription

  void DnC_Dump::dump (const ::Deployment::ComponentPackageDescription &comppkgdesc)
  {
    Dump_Obj dump_obj("ComponentPackageDescription");

    ACE_DEBUG ((LM_DEBUG, "  label: %s\n", comppkgdesc.label.in ()));
    ACE_DEBUG ((LM_DEBUG, "  UUID: %s\n", comppkgdesc.UUID.in ()));

    ACE_DEBUG ((LM_DEBUG, "  configProperty: \n"));
    for (CORBA::ULong i = 0;
         i < comppkgdesc.configProperty.length ();
         ++i)
      {
        DnC_Dump::dump (comppkgdesc.configProperty[i]); // Property
      }

    ACE_DEBUG ((LM_DEBUG, "  infoProperty: \n"));
    for (CORBA::ULong i = 0;
         i < comppkgdesc.infoProperty.length ();
         ++i)
      {
        DnC_Dump::dump (comppkgdesc.infoProperty[i]); // Property
      }

    ACE_DEBUG ((LM_DEBUG, "  realizes: \n"));
    DnC_Dump::dump (comppkgdesc.realizes); // ComponentInterfaceDescription

    ACE_DEBUG ((LM_DEBUG, "  implementation: \n"));
    for (CORBA::ULong i = 0;
         i < comppkgdesc.implementation.length ();
         ++i)
      {
        DnC_Dump::dump (comppkgdesc.implementation[i]); // PackagedComponentImplementation
      }
  }

  // ComponentPortDescription

  void DnC_Dump::dump (const ::Deployment::ComponentPortDescription &compportdesc)
  {
    Dump_Obj dump_obj("ComponentPortDescription");

    dump ("name", compportdesc.name);
    dump ("specificType", compportdesc.specificType);
    dump ("supportedType", compportdesc.supportedType);
    dump ("provider", compportdesc.provider);
    dump ("exclusiveProvider", compportdesc.exclusiveProvider);
    dump ("exclusiveUser", compportdesc.exclusiveUser);
    dump ("optional", compportdesc.optional);

    ACE_DEBUG ((LM_DEBUG, "%skind: ", Dump_Obj::indent()));
    switch (compportdesc.kind) {
    case Facet: ACE_DEBUG ((LM_DEBUG, "Facet\n")); break;
    case SimplexReceptacle: ACE_DEBUG ((LM_DEBUG, "SimplexReceptacle\n")); break;
    case MultiplexReceptacle: ACE_DEBUG ((LM_DEBUG, "MultiplexReceptacle\n")); break;
    case EventEmitter: ACE_DEBUG ((LM_DEBUG, "EventEmitter\n")); break;
    case EventPublisher: ACE_DEBUG ((LM_DEBUG, "EventPublisher\n")); break;
    case EventConsumer: ACE_DEBUG ((LM_DEBUG, "EventConsumer\n")); break;
    }
  }

  // ComponentPropertyDescription

  void DnC_Dump::dump (const ::Deployment::ComponentPropertyDescription &comppropdesc)
  {
    Dump_Obj dump_obj("ComponentPropertyDescription");
    
    dump ("name", comppropdesc.name);
    /*
    ACE_DEBUG ((LM_DEBUG, "%stype: ", Dump_Obj::indent()));
    switch (comppropdesc.type) {
    case tk_null: ACE_DEBUG ((LM_DEBUG, "tk_null\n")); break;
    case tk_void: ACE_DEBUG ((LM_DEBUG, "tk_void\n")); break;
    case tk_short: ACE_DEBUG ((LM_DEBUG, "tk_short\n")); break;
    case tk_long: ACE_DEBUG ((LM_DEBUG, "tk_long\n")); break;
    case tk_ushort: ACE_DEBUG ((LM_DEBUG, "tk_ushort\n")); break;
    case tk_ulong: ACE_DEBUG ((LM_DEBUG, "tk_ulong\n")); break;
    case tk_float: ACE_DEBUG ((LM_DEBUG, "tk_float\n")); break;
    case tk_double: ACE_DEBUG ((LM_DEBUG, "tk_double\n")); break;
    case tk_boolean: ACE_DEBUG ((LM_DEBUG, "tk_boolean\n")); break;
    case tk_char: ACE_DEBUG ((LM_DEBUG, "tk_char\n")); break;
    case tk_octet: ACE_DEBUG ((LM_DEBUG, "tk_octet\n")); break;
    case tk_any: ACE_DEBUG ((LM_DEBUG, "tk_any\n")); break;
    case tk_TypeCode: ACE_DEBUG ((LM_DEBUG, "tk_TypeCode\n")); break;
    case tk_Principal: ACE_DEBUG ((LM_DEBUG, "tk_Principal\n")); break;
    case tk_objref: ACE_DEBUG ((LM_DEBUG, "tk_objref\n")); break;
    case tk_struct: ACE_DEBUG ((LM_DEBUG, "tk_struct\n")); break;
    case tk_union: ACE_DEBUG ((LM_DEBUG, "tk_union\n")); break;
    case tk_enum: ACE_DEBUG ((LM_DEBUG, "tk_enum\n")); break;
    case tk_string: ACE_DEBUG ((LM_DEBUG, "tk_string\n")); break;
    case tk_sequence: ACE_DEBUG ((LM_DEBUG, "tk_sequence\n")); break;
    case tk_array: ACE_DEBUG ((LM_DEBUG, "tk_array\n")); break;
    case tk_alias: ACE_DEBUG ((LM_DEBUG, "tk_alias\n")); break;
    case tk_except: ACE_DEBUG ((LM_DEBUG, "tk_except\n")); break;
    case tk_longlong: ACE_DEBUG ((LM_DEBUG, "tk_longlong\n")); break;
    case tk_ulonglong: ACE_DEBUG ((LM_DEBUG, "tk_ulonglong\n")); break;
    case tk_longdouble: ACE_DEBUG ((LM_DEBUG, "tk_longdouble\n")); break;
    case tk_wchar: ACE_DEBUG ((LM_DEBUG, "tk_wchar\n")); break;
    case tk_wstring: ACE_DEBUG ((LM_DEBUG, "tk_wstring\n")); break;
    case tk_fixed: ACE_DEBUG ((LM_DEBUG, "tk_fixed\n")); break;
    case tk_value: ACE_DEBUG ((LM_DEBUG, "tk_value\n")); break;
    case tk_value_box: ACE_DEBUG ((LM_DEBUG, "tk_value_box\n")); break;
    case tk_native: ACE_DEBUG ((LM_DEBUG, "tk_native\n")); break;
    case tk_abstract_interface: ACE_DEBUG ((LM_DEBUG, "tk_abstract_interface\n")); break;
    case tk_local_interface: ACE_DEBUG ((LM_DEBUG, "tk_local_interface\n")); break;
    case tk_component: ACE_DEBUG ((LM_DEBUG, "tk_component\n")); break;
    case tk_home: ACE_DEBUG ((LM_DEBUG, "tk_home\n")); break;
    }
    */
  }

  // MonolithicImplementationDescription

  void DnC_Dump::dump (const ::Deployment::MonolithicImplementationDescription &mid)
  {
    Dump_Obj dump_obj("MonolithicImplementationDescription");

    ACE_DEBUG ((LM_DEBUG, "  execParameter: \n"));
    for (CORBA::ULong i = 0;
         i < mid.execParameter.length ();
         ++i)
      {
        DnC_Dump::dump (mid.execParameter[i]);
      }

    ACE_DEBUG ((LM_DEBUG, "  deployRequirement: \n"));
    for (CORBA::ULong i = 0;
         i < mid.deployRequirement.length ();
         ++i)
      {
        DnC_Dump::dump (mid.deployRequirement[i]);
      }

    ACE_DEBUG ((LM_DEBUG, "  primaryArtifact: \n"));
    for (CORBA::ULong i = 0;
         i < mid.primaryArtifact.length ();
         ++i)
      {
        DnC_Dump::dump (mid.primaryArtifact[i]); // NamedImplementationArtifact
      }
  }

  // PackageConfiguration

  void DnC_Dump::dump (const ::Deployment::PackageConfiguration &pc)
  {
    Dump_Obj dump_obj("PackageConfiguration");
    ACE_DEBUG ((LM_DEBUG, "  label: %s\n", pc.label.in ()));
    ACE_DEBUG ((LM_DEBUG, "  UUID: %s\n", pc.UUID.in ()));

    ACE_DEBUG ((LM_DEBUG, "  specializedConfig: \n"));
    /*    for (CORBA::ULong i = 0;
         i < pc.specializedConfigRef.length ();
         ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "%i\n", pc.specializedConfigRef[i]));
      }
    */
    ACE_DEBUG ((LM_DEBUG, "  configProperty: \n"));
    for (CORBA::ULong i = 0;
         i < pc.configProperty.length ();
         ++i)
      {
        DnC_Dump::dump (pc.configProperty[i]); // Property
      }

    ACE_DEBUG ((LM_DEBUG, "  selectRequirement: \n"));
    for (CORBA::ULong i = 0;
         i < pc.selectRequirement.length ();
         ++i)
      {
        DnC_Dump::dump (pc.selectRequirement[i]); // Requirement
      }

    ACE_DEBUG ((LM_DEBUG, "  reference: \n"));
    for (CORBA::ULong i = 0;
         i < pc.reference.length ();
         ++i)
      {
        DnC_Dump::dump (pc.reference[i]); // ComponentPackageReference
      }

    ACE_DEBUG ((LM_DEBUG, "  basePackage: \n"));
    for (CORBA::ULong i = 0;
         i < pc.basePackage.length ();
         ++i)
      {
        DnC_Dump::dump (pc.basePackage[i]); // ComponentPackageDescription
      }
  }

  // PackagedComponentImplementation

  void DnC_Dump::dump (const ::Deployment::PackagedComponentImplementation &pci)
  {
    Dump_Obj dump_obj("PackagedComponentImplementation");
    ACE_DEBUG ((LM_DEBUG, "  name: %s\n", pci.name.in ()));

    ACE_DEBUG ((LM_DEBUG, "  referencedImplementation: \n"));
    DnC_Dump::dump (pci.referencedImplementation); // ComponentImplementationDescription
  }

  // SubcomponentPortEndpoint

  void DnC_Dump::dump (const ::Deployment::SubcomponentPortEndpoint&)
  {
    Dump_Obj dump_obj("SubcomponentPortEndpoint");
    /*

    ACE_DEBUG ((LM_DEBUG, "  portName: %s\n", spe.portName.in ()));

    ACE_DEBUG ((LM_DEBUG, "  instance: \n"));
    for (CORBA::ULong i = 0;
         i < spe.instance.length ();
         ++i)
      {
        DnC_Dump::dump (spe.instance[i]); // SubcomponentInstantiationDescription
      }

    */
  }

  void DnC_Dump::dump (const ::Deployment::Requirement &req)
  {
    Dump_Obj dump_obj("Requirement");
    ACE_DEBUG ((LM_DEBUG, "resourceType: %s \n", req.resourceType.in ()));
    for (::CORBA::ULong i = 0; i < req.property.length (); i ++)
      DnC_Dump::dump (req.property [i]);
    ACE_DEBUG ((LM_DEBUG, "name: %s \n", req.name.in ()));
  }

  void DnC_Dump::dump (const ::Deployment::ComponentExternalPortEndpoint &)
  {
    Dump_Obj dump_obj("ComponentExternalPortEndpoint");
  }

  void DnC_Dump::dump (const ::Deployment::ComponentPackageReference &)
  {
    Dump_Obj dump_obj("ComponentPackageReference");
  }

  void DnC_Dump::dump (const ::Deployment::ComponentImplementationDescription &)
  {
    Dump_Obj dump_obj("ComponentImplementationDescription");
  }

  void DnC_Dump::dump (const ::Deployment::SubcomponentInstantiationDescription &)
  {
    Dump_Obj dump_obj("SubcomponentInstantiationDescription");
  }

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
              ACE_DEBUG ((LM_DEBUG, "DnC_Dump::dump (CORBA::Any), expected short\
                                  encoded different type"));
              ACE_THROW (CORBA::INTERNAL ());
            }
          ACE_DEBUG ((LM_DEBUG, "Any value: %d \n", temp));
        }
        break;

      case CORBA::tk_null:
        ACE_DEBUG ((LM_DEBUG, "Any value: null value encoded\n"));
        break;

      case CORBA::tk_void:
        ACE_DEBUG ((LM_DEBUG, "Any value: void type encoded \n"));
        break;
      case CORBA::tk_long:
        {
          CORBA::Long temp;
          if (! (any >>= temp))
            {
              ACE_DEBUG ((LM_DEBUG, "DnC::dump (CORBA::Any) expected long\
                                    encoded with different type"));
              ACE_THROW (CORBA::INTERNAL ());
            }
          ACE_DEBUG ((LM_DEBUG, "Any value: %d \n", temp));
        }
        break;

      case CORBA::tk_ushort:
        {
          CORBA::UShort temp;
          if (! (any >>= temp))
            {
              ACE_DEBUG ((LM_DEBUG, "DnC::dump (CORBA::Any) expected u short\
                                   encoded with different type"));
              ACE_THROW (CORBA::INTERNAL ());
            }
          ACE_DEBUG ((LM_DEBUG, "Any value: %u \n", temp));
        }
        break;

      case CORBA::tk_ulong:
        {
          CORBA::ULong temp;
          if (! (any >>= temp))
            {
              ACE_DEBUG ((LM_DEBUG, "DnC::dump (CORBA::Any) expected ulong\
                                    encoded with different type"));
              ACE_THROW (CORBA::INTERNAL ());
            }
          ACE_DEBUG ((LM_DEBUG, "Any value: %u \n", temp));
        }
        break;

      case CORBA::tk_float:
        {
          CORBA::Float temp;
          if (! (any >>= temp))
            {
              ACE_DEBUG ((LM_DEBUG, "DnC::dump (CORBA::Any) expected float\
                                    encoded with different type"));
              ACE_THROW (CORBA::INTERNAL ());
            }
          ACE_DEBUG ((LM_DEBUG, "Any value: %f \n", temp));
        }
        break;
      case CORBA::tk_double:
        {
          CORBA::Double temp;
          if (! (any >>= temp))
            {
              ACE_DEBUG ((LM_DEBUG, "DnC::dump (CORBA::Any) expected double\
                                    encoded with different type"));
              ACE_THROW (CORBA::INTERNAL ());
            }
          ACE_DEBUG ((LM_DEBUG, "Any value: %f \n", temp));
        }
        break;
      case CORBA::tk_boolean:
        {
          CORBA::Boolean temp;
          if (! (any >>= CORBA::Any::to_boolean (temp)))
            {
              ACE_DEBUG ((LM_DEBUG, "DnC::dump (CORBA::Any) expected bool\
                                    encoded with different type"));
              ACE_THROW (CORBA::INTERNAL ());
            }

          if (temp)
            ACE_DEBUG ((LM_DEBUG, "Any value: True \n"));
          else
            ACE_DEBUG ((LM_DEBUG, "Any value: False \n"));
        }
        break;

      case CORBA::tk_char:
        {
          CORBA::Char temp;
          if (! (any >>= CORBA::Any::to_char (temp)))
            {
              ACE_DEBUG ((LM_DEBUG, "DnC::dump (CORBA::Any) expected char\
                                    encoded with different type"));
              ACE_THROW (CORBA::INTERNAL ());
            }
          ACE_DEBUG ((LM_DEBUG, "Any value: %c \n", temp));
        }
        break;

      case CORBA::tk_octet:
        {
          CORBA::Octet temp;
          if (! (any >>= CORBA::Any::to_octet (temp)))
            {
              ACE_DEBUG ((LM_DEBUG, "DnC::dump (CORBA::Any) expected octet\
                                    encoded with different type"));
              ACE_THROW (CORBA::INTERNAL ());
            }
          ACE_DEBUG ((LM_DEBUG, "Any value: %d \n", temp));
        }
        break;

      case CORBA::tk_string:
        {
          const char * temp = 0;
          if (! (any >>= temp))
            {
              ACE_DEBUG ((LM_DEBUG, "DnC::dump (CORBA::Any) expected string\
                                    encoded with different type"));
              ACE_THROW (CORBA::INTERNAL ());
            }
          ACE_DEBUG ((LM_DEBUG, "Any value: %s \n", temp));
        }
        break;
      case CORBA::tk_longlong:
        {
          CORBA::LongLong temp;
          if (! (any >>= temp))
            {
              ACE_DEBUG ((LM_DEBUG, "DnC::dump (CORBA::Any) expected longlong\
                                    encoded with different type"));
              ACE_THROW (CORBA::INTERNAL ());
            }
          ACE_DEBUG ((LM_DEBUG, "Any value: %l \n", temp));
        }
        break;

      case CORBA::tk_longdouble:
        {
          CORBA::LongDouble temp;
          if (! (any >>= temp))
            {
              ACE_DEBUG ((LM_DEBUG, "DnC::dump (CORBA::Any) expected longdouble\
                                    encoded with different type"));
              ACE_THROW (CORBA::INTERNAL ());
            }
          ACE_DEBUG ((LM_DEBUG, "Any value: %d \n", temp));
        }

        break;
      case CORBA::tk_wchar:
        {
          CORBA::WChar temp;
          if (! (any >>= CORBA::Any::to_wchar (temp)))
            {
              ACE_DEBUG ((LM_DEBUG, "DnC::dump (CORBA::Any) expected wchar\
                                    encoded with different type"));
              ACE_THROW (CORBA::INTERNAL ());
            }
          ACE_DEBUG ((LM_DEBUG, "Any value: %c \n", temp));
        }
        break;

      case CORBA::tk_wstring:
        {
          const CORBA::WChar * temp;
          if (! (any >>= temp))
            {
              ACE_DEBUG ((LM_DEBUG, "DnC::dump (CORBA::Any) expected wstring\
                                    encoded with different type"));
              ACE_THROW (CORBA::INTERNAL ());
            }
          ACE_DEBUG ((LM_DEBUG, "Any value: %s \n", temp));
        }
        break;

      default:
        ACE_DEBUG ((LM_DEBUG, "Unknown type encoded in Any\n"));
        ACE_THROW (CORBA::INTERNAL ());
      }
  }

  void DnC_Dump::dump (const ::Deployment::NamedImplementationArtifact &nia)
  {
    Dump_Obj dump_obj("NamedImplementationArtifact");
    ACE_DEBUG ((LM_DEBUG, "name: %s \n", nia.name.in ()));
    ACE_DEBUG ((LM_DEBUG, "referencedArtifact: \n"));
    DnC_Dump::dump (nia.referencedArtifact);  // ImplementationArtifactDescription
  }

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

  void DnC_Dump::dump (const ::Deployment::SubcomponentPropertyReference &)
  {
    Dump_Obj dump_obj("SubcomponentPropertyReference");
  }

  void DnC_Dump::dump (const ::Deployment::Domain &domain)
  {
    Dump_Obj dump_obj("Domain");
    dump ("UUID", domain.UUID);
    dump ("label", domain.label);

    dump_sequence ("sharedResource", domain.sharedResource);
    dump_sequence ("node", domain.node);
    dump_sequence ("interconnect", domain.interconnect);
    dump_sequence ("bridge", domain.bridge);
    /*
    for (CORBA::ULong i = 0; i < domain.node.length (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "\nNode %d: \n", i + 1));
        ACE_DEBUG ((LM_DEBUG, "     Name: %s \n",
                    domain.node[i].name.in ()));
        for (CORBA::ULong j = 0; j < domain.node[i].resource.length (); ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Resource %d: \n", j + 1));
            ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                        domain.node[i].resource[j].name.in ()));
            for (CORBA::ULong k = 0;
                 k < domain.node[i].resource[j].resourceType.length (); ++k)
              {
                ACE_DEBUG ((LM_DEBUG, "         ResourceType: %s \n",
                            domain.node[i].resource[j].resourceType[k].in ()));
              }
          }

        for (CORBA::ULong j = 0;
             j < domain.node[i].connectionRef.length (); ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Connection %d: \n", j + 1));
            int value = domain.node[i].connectionRef[j];
            ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                        domain.interconnect[value].name.in ()));
          }

        for (CORBA::ULong j = 0;
             j < domain.node[i].sharedResourceRef.length (); ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     SharedResource %d: \n", j + 1));
            int value = domain.node[i].sharedResourceRef[j];
            ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                        domain.sharedResource[value].name.in ()));
          }

      }

    for (CORBA::ULong i = 0; i < domain.interconnect.length (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "\nInterconnect %d: \n", i + 1));
        ACE_DEBUG ((LM_DEBUG, "     Name: %s \n",
                    domain.interconnect[i].name.in ()));
        ACE_DEBUG ((LM_DEBUG, "     Label: %s \n",
                    domain.interconnect[i].label.in ()));
        for (CORBA::ULong j = 0;
             j < domain.interconnect[i].resource.length (); ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Resource %d: \n", j + 1));
            ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                        domain.interconnect[i].resource[j].name.in ()));
            for (CORBA::ULong k = 0;
                 k < domain.interconnect[i].resource[j].resourceType.length ();
                 ++k)
              {
                ACE_DEBUG ((LM_DEBUG, "         ResourceType: %s \n",
                            domain.interconnect[i].resource[j].resourceType[k].in ()));
              }
          }

        for (CORBA::ULong j = 0;
             j < domain.interconnect[i].connectionRef.length (); ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Connection %d: \n", j + 1));
            int value = domain.interconnect[i].connectionRef[j];
            ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                        domain.bridge[value].name.in ()));
          }

        for (CORBA::ULong j = 0;
             j < domain.interconnect[i].connectRef.length (); ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     SharedResource %d: \n", j + 1));
            int value = domain.interconnect[i].connectRef[j];
            ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                        domain.node[value].name.in ()));
          }

      }

    for (CORBA::ULong i = 0; i < domain.bridge.length (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "\nBridge %d: \n", i + 1));
        ACE_DEBUG ((LM_DEBUG, "     Name: %s \n",
                    domain.bridge[i].name.in ()));
        ACE_DEBUG ((LM_DEBUG, "     Label: %s \n",
                    domain.bridge[i].label.in ()));
        for (CORBA::ULong j = 0;
             j < domain.bridge[i].resource.length (); ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Resource %d: \n", j + 1));
            ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                        domain.bridge[i].resource[j].name.in ()));
            for (CORBA::ULong k = 0;
                 k < domain.bridge[i].resource[j].resourceType.length ();
                 ++k)
              {
                ACE_DEBUG ((LM_DEBUG, "         ResourceType: %s \n",
                            domain.bridge[i].resource[j].resourceType[k].in ()));
              }
          }

        for (CORBA::ULong j = 0; j < domain.bridge[i].connectRef.length ();
             ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Connection %d: \n", j + 1));
            int value = domain.bridge[i].connectRef[j];
            ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                        domain.interconnect[value].name.in ()));
          }
      }

    for (CORBA::ULong i = 0; i < domain.sharedResource.length (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "\nSharedResource %d: \n", i + 1));
        ACE_DEBUG ((LM_DEBUG, "     Name: %s \n",
                    domain.sharedResource[i].name.in ()));
        for (CORBA::ULong k = 0;
             k < domain.sharedResource[i].resourceType.length (); ++k)
          {
            ACE_DEBUG ((LM_DEBUG, "         ResourceType: %s \n",
                        domain.sharedResource[i].resourceType[k].in ()));
          }

        for (CORBA::ULong j = 0;
             j < domain.sharedResource[i].nodeRef.length (); ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Connection %d: \n", j + 1));
            int value = domain.sharedResource[i].nodeRef[j];
            ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                        domain.node[value].name.in ()));
          }
      }
    */
  }

  void DnC_Dump::dump (const ::Deployment::ExternalReferenceEndpoint &ere)
  {
    Dump_Obj dump_obj("ExternalReferenceEndpoint");
    ACE_DEBUG ((LM_DEBUG, "location: %s \n", ere.location.in ()));
  }

  void DnC_Dump::dump (const ::Deployment::Capability &capability)
  {
    Dump_Obj dump_obj("Capability");
    ACE_DEBUG ((LM_DEBUG, "name: %s", capability.name.in ()));
    DnC_Dump::dump ("resourceType", capability.resourceType); // string sequence
    DnC_Dump::dump_sequence ("property", capability.property); // SatisfierProperty sequence
  }

  void DnC_Dump::dump (const ::Deployment::ImplementationArtifactDescription &iad)
  {
    Dump_Obj dump_obj("ImplementationArtifactDescription");
    ACE_DEBUG ((LM_DEBUG, "label: %s", iad.label.in ()));
    ACE_DEBUG ((LM_DEBUG, "UUID: %s", iad.UUID.in ()));
    //    ACE_DEBUG ((LM_DEBUG, "location: %s", iad.location.in ()));
    DnC_Dump::dump_sequence ("execParameter", iad.execParameter); // Property seq.
    DnC_Dump::dump_sequence ("infoProperty", iad.infoProperty); // Property seq.
    DnC_Dump::dump_sequence ("deployRequirement", iad.deployRequirement); // Requirement seq.
    DnC_Dump::dump_sequence ("dependsOn", iad.dependsOn); // NamedImplementationArtifact seq.
  }

  void DnC_Dump::dump (const ::Deployment::ImplementationRequirement &ir)
  {
    Dump_Obj dump_obj("ImplementationRequirement");
    //    DnC_Dump::dump ("resourcePort", ir.resourcePort); // string sequence
    //    DnC_Dump::dump ("componentPort", ir.componentPort); // string sequence
    DnC_Dump::dump_sequence ("resourceUsage", ir.resourceUsage); // ResourceUsageKind seq.
    ACE_DEBUG ((LM_DEBUG, "resourceType: %s", ir.resourceType.in ()));
    ACE_DEBUG ((LM_DEBUG, "name: %s", ir.name.in ()));
  }

  void DnC_Dump::dump(const Deployment::ResourceUsageKind&)
  {
    Dump_Obj dump_obj("ResourceUsageKind");
  }

  void DnC_Dump::dump(const Deployment::DeploymentPlan &plan)
  {
    ACE_DEBUG ((LM_DEBUG,
                "========================================================\n"));
    Dump_Obj dump_obj("              DEPLOYMENT PLAN");
    ACE_DEBUG ((LM_DEBUG,
                "===================================-====================\n"));
    ACE_DEBUG ((LM_DEBUG, "\nUUID: %s \n", plan.UUID.in ()));
    ACE_DEBUG ((LM_DEBUG, "Label: %s \n", plan.label.in ()));
    //dump ("label", plan.label);
    //dump ("UUID", plan.UUID);
    ACE_DEBUG ((LM_DEBUG, "%srealizes:\n", Dump_Obj::indent()));
    dump (plan.realizes);
    for (CORBA::ULong i = 0; i < plan.implementation.length (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "\nMonolithicImplementation %d: \n", i + 1));
        ACE_DEBUG ((LM_DEBUG, "     Name: %s \n",
                    plan.implementation[i].name.in ()));
        for (CORBA::ULong j = 0; j < plan.implementation[i].source.length ();
             ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Source: %d: \n", j + 1));
            ACE_DEBUG ((LM_DEBUG, "       Address: %s \n",
                        plan.implementation[i].source[j].in ()));
          }
        for (CORBA::ULong j = 0; 
             j < plan.implementation[i].artifactRef.length (); ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Artifact: %d: \n", j + 1));
            int value = plan.implementation[i].artifactRef[j];
            ACE_DEBUG ((LM_DEBUG, "       Name: %s \n",
                        plan.artifact[value].name.in ()));
          }
        for (CORBA::ULong j = 0; 
             j < plan.implementation[i].execParameter.length (); ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     ExecParameter: %d: \n", j + 1));
            ACE_DEBUG ((LM_DEBUG, "       Name: %s \n",
                        plan.implementation[i].execParameter[j].name.in ()));
            dump (plan.implementation[i].execParameter[j].value);
          }
      }
    for (CORBA::ULong i = 0; i < plan.instance.length (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "\nInstanceDescription %d: \n", i + 1));
        ACE_DEBUG ((LM_DEBUG, "     Name: %s \n",
                    plan.instance[i].name.in ()));
        ACE_DEBUG ((LM_DEBUG, "     Node: %s \n",
                    plan.instance[i].node.in ()));
        for (CORBA::ULong j = 0; j < plan.instance[i].source.length ();
             ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Source: %d: \n", j + 1));
            ACE_DEBUG ((LM_DEBUG, "       Address: %s \n",
                        plan.instance[i].source[j].in ()));
          }
        int value = plan.instance[i].implementationRef;
        ACE_DEBUG ((LM_DEBUG, "     Implementation: %s \n",
                    plan.implementation[value].name.in ()));
        for (CORBA::ULong j = 0; 
             j < plan.instance[i].configProperty.length (); ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     ConfigProperty: %d: \n", j + 1));
            ACE_DEBUG ((LM_DEBUG, "       Name: %s \n",
                        plan.instance[i].configProperty[j].name.in ()));
            dump (plan.instance[i].configProperty[j].value);
          }
      }
    for (CORBA::ULong i = 0; i < plan.connection.length (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "\nPlanConnection %d: \n", i + 1));
        ACE_DEBUG ((LM_DEBUG, "     Name: %s \n",
                    plan.connection[i].name.in ()));
        for (CORBA::ULong j = 0; j < plan.connection[i].source.length ();
             ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Source: %d: \n", j + 1));
            ACE_DEBUG ((LM_DEBUG, "       Address: %s \n",
                        plan.connection[i].source[j].in ()));
          }
        for (CORBA::ULong j = 0; 
             j < plan.connection[i].internalEndpoint.length ();
             ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Connection: %d: \n", j + 1));
            ACE_DEBUG ((LM_DEBUG, "       Portname: %s \n",
                        plan.connection[i].
                        internalEndpoint[j].portName.in ()));
            int value = plan.connection[i].internalEndpoint[j].instanceRef;
            ACE_DEBUG ((LM_DEBUG, "       Instance: %s \n",
                    plan.instance[value].name.in ()));
            ACE_DEBUG ((LM_DEBUG, "       PortKind: "));
            switch (plan.connection[i].internalEndpoint[j].kind)
              {
                case Facet: 
                  ACE_DEBUG ((LM_DEBUG, "Facet\n"));
                  break;

                case SimplexReceptacle: 
                  ACE_DEBUG ((LM_DEBUG, "SimplexReceptacle\n"));
                  break;

                case MultiplexReceptacle:
                  ACE_DEBUG ((LM_DEBUG, "MultiplexReceptacle\n"));
                  break;

                case EventEmitter:
                  ACE_DEBUG ((LM_DEBUG, "EventEmitter\n"));
                  break;

                case EventPublisher:
                  ACE_DEBUG ((LM_DEBUG, "EventPublisher\n"));
                  break;

                case EventConsumer:
                  ACE_DEBUG ((LM_DEBUG, "EventConsumer\n"));
                  break;

              }
          }
      }
    for (CORBA::ULong i = 0; i < plan.artifact.length (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "\nArtifactDescription %d: \n", i + 1));
        ACE_DEBUG ((LM_DEBUG, "     Name: %s \n",
                    plan.artifact[i].name.in ()));
        ACE_DEBUG ((LM_DEBUG, "     Node: %s \n",
                    plan.artifact[i].node.in ()));
        for (CORBA::ULong j = 0; j < plan.artifact[i].source.length ();
             ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Source: %d: \n", j + 1));
            ACE_DEBUG ((LM_DEBUG, "       Address: %s \n",
                        plan.artifact[i].source[j].in ()));
          }
        for (CORBA::ULong j = 0; j < plan.artifact[i].location.length ();
             ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Location: %d: \n", j + 1));
            ACE_DEBUG ((LM_DEBUG, "       Address: %s \n",
                        plan.artifact[i].location[j].in ()));
          }
        for (CORBA::ULong j = 0; 
             j < plan.artifact[i].execParameter.length (); ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     ExecParameter: %d: \n", j + 1));
            ACE_DEBUG ((LM_DEBUG, "       Name: %s \n",
                        plan.artifact[i].execParameter[j].name.in ()));
            dump (plan.artifact[i].execParameter[j].value);
          }
      }
    for (CORBA::ULong i = 0; i < plan.externalProperty.length (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "\nPlanProperty %d: \n", i + 1));
        ACE_DEBUG ((LM_DEBUG, "     Name: %s \n",
                    plan.externalProperty[i].name.in ()));
      }
    for (CORBA::ULong i = 0; i < plan.dependsOn.length (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "\nImplementationDependency %d: \n", i + 1));
      }
    for (CORBA::ULong i = 0; i < plan.infoProperty.length (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "\nExternalProperty %d: \n", i + 1));
        ACE_DEBUG ((LM_DEBUG, "     Name: %s \n",
                    plan.infoProperty[i].name.in ()));
      }
    /*
    dump_sequence ("implementation", plan.implementation);
    dump_sequence ("instance", plan.instance);
    dump_sequence ("connection", plan.connection);
    dump_sequence ("externalProperty", plan.externalProperty);
    dump_sequence ("dependsOn", plan.dependsOn);
    dump_sequence ("artifact", plan.artifact);
    dump_sequence ("infoProperty", plan.infoProperty);
    */
  }

  void DnC_Dump::dump(const Deployment::MonolithicDeploymentDescription &mdd)
  {
    Dump_Obj dump_obj("MonolithicDeploymentDescription");
    dump ("name", mdd.name);
    dump ("source", mdd.source);
    ACE_DEBUG ((LM_DEBUG, "%sartifactRef: ???\n", Dump_Obj::indent()));
    dump_sequence ("execParameter", mdd.execParameter);
    dump_sequence ("deployRequirement", mdd.deployRequirement);
  }

  void DnC_Dump::dump(const Deployment::InstanceDeploymentDescription &idd)
  {
    Dump_Obj dump_obj("InstanceDeploymentDescription");
    dump ("name", idd.name);
    dump ("node", idd.node);
    dump ("source", idd.source);
    ACE_DEBUG ((LM_DEBUG, "%simplementationRef: ???\n", Dump_Obj::indent()));
    dump_sequence ("configProperty", idd.configProperty);
    dump_sequence ("deployedResource", idd.deployedResource);
    dump_sequence ("deployedSharedResource", idd.deployedSharedResource);
  }

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

  void DnC_Dump::dump(const Deployment::PlanPropertyMapping &ppm)
  {
    Dump_Obj dump_obj("PlanPropertyMapping");
    dump ("name", ppm.name);
    dump ("source", ppm.source);
    dump ("externalName", ppm.externalName);
    dump_sequence ("delegatesTo", ppm.delegatesTo);
  }

  void DnC_Dump::dump(const Deployment::ImplementationDependency &id)
  {
    Dump_Obj dump_obj("ImplementationDependency");
    dump ("requiredType", id.requiredType);
  }

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

  void DnC_Dump::dump(const Deployment::InstanceResourceDeploymentDescription &irdd)
  {
    Dump_Obj dump_obj("InstanceResourceDeploymentDescription");
    ACE_DEBUG ((LM_DEBUG, "%sresourceUsage: ", Dump_Obj::indent()));
    switch (irdd.resourceUsage) {
    case None: ACE_DEBUG ((LM_DEBUG, "None\n")); break;
    case InstanceUsesResource: ACE_DEBUG ((LM_DEBUG, "InstanceUsesResource\n")); break;
    case ResourceUsesInstance: ACE_DEBUG ((LM_DEBUG, "ResourceUsesInstance\n")); break;
    case PortUsesResource: ACE_DEBUG ((LM_DEBUG, "PortUsesResource\n")); break;
    case ResourceUsesPort: ACE_DEBUG ((LM_DEBUG, "ResourceUsesPort\n")); break;
    }
    dump ("requirementName", irdd.requirementName);
    dump ("resourceName", irdd.resourceName);
    ACE_DEBUG ((LM_DEBUG, "%sresourceValue:\n", Dump_Obj::indent()));
    dump (irdd.resourceValue);
  }

  void DnC_Dump::dump(const PlanSubcomponentPortEndpoint &pspe)
  {
    Dump_Obj dump_obj("PlanSubcomponentPortEndpoint");
    dump ("portName", pspe.portName);
    dump ("provider", pspe.provider);
    ACE_DEBUG ((LM_DEBUG, "%skind: ", Dump_Obj::indent()));
    switch (pspe.kind) {
    case Facet: ACE_DEBUG ((LM_DEBUG, "Facet\n")); break;
    case SimplexReceptacle: ACE_DEBUG ((LM_DEBUG, "SimplexReceptacle\n")); break;
    case MultiplexReceptacle: ACE_DEBUG ((LM_DEBUG, "MultiplexReceptacle\n")); break;
    case EventEmitter: ACE_DEBUG ((LM_DEBUG, "EventEmitter\n")); break;
    case EventPublisher: ACE_DEBUG ((LM_DEBUG, "EventPublisher\n")); break;
    case EventConsumer: ACE_DEBUG ((LM_DEBUG, "EventConsumer\n")); break;
    }
    ACE_DEBUG ((LM_DEBUG, "%sinstanceRef: ???\n", Dump_Obj::indent()));
  }

  void DnC_Dump::dump(const ConnectionResourceDeploymentDescription &crdd)
  {
    Dump_Obj dump_obj("ConnectionResourceDeploymentDescription");
    dump ("targetName", crdd.targetName);
    dump ("requirementName", crdd.requirementName);
    dump ("resourceName", crdd.resourceName);
    ACE_DEBUG ((LM_DEBUG, "%sresourceValue:\n", Dump_Obj::indent()));
    dump (crdd.resourceValue);
  }

  void DnC_Dump::dump(const PlanSubcomponentPropertyReference &pspr)
  {
    Dump_Obj dump_obj("PlanSubcomponentPropertyReference");
    dump ("propertyName", pspr.propertyName);
    ACE_DEBUG ((LM_DEBUG, "%sinstanceRef: ???\n", Dump_Obj::indent()));
  }

  void DnC_Dump::dump(const ResourceDeploymentDescription &rdd)
  {
    Dump_Obj dump_obj("ResourceDeploymentDescription");
    dump ("requirementName", rdd.requirementName);
    dump ("resourceName", rdd.resourceName);
    ACE_DEBUG ((LM_DEBUG, "%sresourceValue:\n", Dump_Obj::indent()));
    dump (rdd.resourceValue);
  }
}

#endif /* DNC_DUMP_C */
