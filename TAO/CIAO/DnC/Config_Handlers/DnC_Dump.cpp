// $Id$

#ifndef DNC_DUMP_C
#define DNC_DUMP_C

#include "ace/Log_Msg.h"

#include "DnC_Dump.h"
#include <iostream>
#include <string>
#include <map>

#include "tckind_names.h"

namespace Deployment
{
  /*
   *  Generic dump functions
   */

  class Dump_Obj {
  public:
    template <typename DESC>
    Dump_Obj(const char* caption, DESC &desc): desc_(caption)
    {
      ACE_DEBUG ((LM_DEBUG, "%s%s:\n", indent_.c_str(), caption));
      indent_.append("   ");

      CORBA::Any val;
      val <<= desc;
      desc_map[caption] <<= desc;
    }

    ~Dump_Obj() {
      indent_.erase(indent_.size() - 3, 3);
      desc_map.erase(desc_map.find(desc_.c_str()));
    }

    static const char* indent() {
      return indent_.c_str();
    }

    // TODO: function returning the description
    template <typename DESC>
    static const DESC& desc(const char* caption)
    {
      static DESC* val;
      desc_map[caption] >>= val;
      return *val;
    }

  private:
    static std::string indent_;
    static std::map<const char*, CORBA::Any> desc_map;
    std::string desc_;
  };

  std::string Dump_Obj::indent_ = "-";

  // Dumps a string sequence
  void DnC_Dump::dump (const char* caption, const ::CORBA::StringSeq &str_seq)
  {
    CORBA::ULong size = str_seq.length ();

    if (size != 0)
      {
        Dump_Obj dump_obj(caption, str_seq);
        
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
        Dump_Obj dump_obj(caption, seq);
        
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
    Dump_Obj dump_obj("SatisfierProperty", sp);
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
    Dump_Obj dump_obj("SharedResource", sr);
    dump ("name", sr.name);
    dump ("resourceType", sr.resourceType);
    ACE_DEBUG ((LM_DEBUG, "%snodeRef: ???\n", Dump_Obj::indent()));
    dump_sequence ("property", sr.property);
  }

  // Resource
  void DnC_Dump::dump (const Deployment::Resource& resource)
  {
    Dump_Obj dump_obj("Resource", resource);
    dump ("name", resource.name);
    dump ("resourceType", resource.resourceType);
    dump_sequence ("property", resource.property);
  }

  // Node
  void DnC_Dump::dump (const Deployment::Node& node)
  {
    Dump_Obj dump_obj("Node", node);
    dump ("name", node.name);
    dump ("label", node.label);
    ACE_DEBUG ((LM_DEBUG, "%ssharedResourceRef: ???\n", Dump_Obj::indent()));
    ACE_DEBUG ((LM_DEBUG, "%sconnectionRef: ???\n", Dump_Obj::indent()));
    dump_sequence ("resource", node.resource);
  }

  // Interconnect
  void DnC_Dump::dump (const Deployment::Interconnect& conn)
  {
    Dump_Obj dump_obj("Interconnect", conn);
    dump ("name", conn.name);
    dump ("label", conn.label);
    ACE_DEBUG ((LM_DEBUG, "%sconnectionRef: ???\n", Dump_Obj::indent()));
    ACE_DEBUG ((LM_DEBUG, "%sconnectRef: ???\n", Dump_Obj::indent()));
    dump_sequence ("resource", conn.resource);
  }

  // Bridge
  void DnC_Dump::dump (const Deployment::Bridge& bridge)
  {
    Dump_Obj dump_obj("Bridge", bridge);
    dump ("name", bridge.name);
    dump ("label", bridge.label);
    ACE_DEBUG ((LM_DEBUG, "%sconnectionRef: ???\n", Dump_Obj::indent()));
    dump_sequence ("resource", bridge.resource);
  }

  // Property
  void DnC_Dump::dump (const Deployment::Property& property)
  {
    Dump_Obj dump_obj("Property", property);
    dump ("name", property.name);
    ACE_DEBUG ((LM_DEBUG, "%svalue:\n", Dump_Obj::indent()));
    dump (property.value);
  }

  // AssemblyConnectionDescription

  void DnC_Dump::dump (const ::Deployment::AssemblyConnectionDescription &acd)
  {
    Dump_Obj dump_obj("AssemblyConnectionDescription", acd);

    dump ("name", acd.name);
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
    Dump_Obj dump_obj("AssemblyPropertyMapping", apm);

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
    Dump_Obj dump_obj("ComponentPackageDescription", comppkgdesc);
    dump ("label", comppkgdesc.label);
    dump ("UUID", comppkgdesc.UUID);
    dump_sequence ("configProperty", comppkgdesc.configProperty);
    dump_sequence ("infoProperty", comppkgdesc.infoProperty);
    dump_sequence ("implementation", comppkgdesc.implementation);
    ACE_DEBUG ((LM_DEBUG, "  realizes: \n"));
    DnC_Dump::dump (comppkgdesc.realizes); // ComponentInterfaceDescription
  }

  // ComponentPortDescription

  void DnC_Dump::dump (const ::Deployment::ComponentPortDescription &compportdesc)
  {
    Dump_Obj dump_obj("ComponentPortDescription", compportdesc);

    dump ("name", compportdesc.name);
    dump ("specificType", compportdesc.specificType);
    //dump ("supportedType", compportdesc.supportedType);
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
    Dump_Obj dump_obj("ComponentPropertyDescription", comppropdesc);
    
    dump ("name", comppropdesc.name);
  }

  // MonolithicImplementationDescription

  void DnC_Dump::dump (const ::Deployment::MonolithicImplementationDescription &mid)
  {
    Dump_Obj dump_obj("MonolithicImplementationDescription", mid);

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
    Dump_Obj dump_obj("PackageConfiguration", pc);
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
    Dump_Obj dump_obj("PackagedComponentImplementation", pci);
    dump ("Name", pci.name);
    ACE_DEBUG ((LM_DEBUG, 
                "%sReferencedIMplementation: \n", Dump_Obj::indent()));
    ACE_DEBUG ((LM_DEBUG, "  referencedImplementation: \n"));
    DnC_Dump::dump (pci.referencedImplementation); // ComponentImplementationDescription
  }

  // SubcomponentPortEndpoint

  void DnC_Dump::dump (const ::Deployment::SubcomponentPortEndpoint& spe)
  {
    Dump_Obj dump_obj("SubcomponentPortEndpoint", spe);
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
    Dump_Obj dump_obj("Requirement", req);
    ACE_DEBUG ((LM_DEBUG, "resourceType: %s \n", req.resourceType.in ()));
    for (::CORBA::ULong i = 0; i < req.property.length (); i ++)
      DnC_Dump::dump (req.property [i]);
    ACE_DEBUG ((LM_DEBUG, "name: %s \n", req.name.in ()));
  }

  void DnC_Dump::dump (const ::Deployment::ComponentExternalPortEndpoint &cepe)
  {
    Dump_Obj dump_obj("ComponentExternalPortEndpoint", cepe);
  }

  void DnC_Dump::dump (const ::Deployment::ComponentPackageReference &cpr)
  {
    Dump_Obj dump_obj("ComponentPackageReference", cpr);
  }

  void DnC_Dump::dump (const ::Deployment::SubcomponentInstantiationDescription &sid)
  {
    Dump_Obj dump_obj("SubcomponentInstantiationDescription", sid);
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
    Dump_Obj dump_obj("NamedImplementationArtifact", nia);
    ACE_DEBUG ((LM_DEBUG, "%sName: %s \n", Dump_Obj::indent(), nia.name.in ()));
    ACE_DEBUG ((LM_DEBUG, "%sReferencedArtifact: \n", Dump_Obj::indent()));
    DnC_Dump::dump (nia.referencedArtifact);  
  }

  void DnC_Dump::dump (const ::Deployment::ComponentInterfaceDescription &cid)
  {
    Dump_Obj dump_obj("ComponentInterfaceDescription", cid);

    dump ("label", cid.label);
    dump ("UUID", cid.UUID);
    dump ("specificType", cid.specificType);
    //dump_sequence ("supportedType", cid.supportedType);
    dump ("idlFile", cid.idlFile);
    dump_sequence ("configProperty", cid.configProperty);
    dump_sequence ("port", cid.port);
    dump_sequence ("property", cid.property);
    dump_sequence ("infoProperty", cid.infoProperty);
  }

  void DnC_Dump::dump (const ::Deployment::SubcomponentPropertyReference &spr)
  {
    Dump_Obj dump_obj("SubcomponentPropertyReference", spr);
  }

  void DnC_Dump::dump (const ::Deployment::Domain &domain)
  {
    Dump_Obj dump_obj("Domain", domain);
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
    Dump_Obj dump_obj("ExternalReferenceEndpoint", ere);
    ACE_DEBUG ((LM_DEBUG, "location: %s \n", ere.location.in ()));
  }

  void DnC_Dump::dump (const ::Deployment::Capability &capability)
  {
    Dump_Obj dump_obj("Capability", capability);
    ACE_DEBUG ((LM_DEBUG, "name: %s", capability.name.in ()));
    DnC_Dump::dump ("resourceType", capability.resourceType); 
    DnC_Dump::dump_sequence ("property", capability.property);
  }

  void DnC_Dump::dump (const ::Deployment::ImplementationArtifactDescription &iad)
  {
    Dump_Obj dump_obj("ImplementationArtifactDescription", iad);
    ACE_DEBUG ((LM_DEBUG, "%sLabel: %s \n", Dump_Obj::indent(), 
                iad.label.in ()));
    ACE_DEBUG ((LM_DEBUG, "%sUUID: %s \n", Dump_Obj::indent(), 
                iad.UUID.in ()));
    DnC_Dump::dump_sequence ("execParameter", iad.execParameter);
    DnC_Dump::dump_sequence ("infoProperty", iad.infoProperty); 
    DnC_Dump::dump_sequence ("deployRequirement", iad.deployRequirement); 
    DnC_Dump::dump_sequence ("dependsOn", iad.dependsOn); 
  }

  void DnC_Dump::dump (const ::Deployment::ImplementationRequirement &ir)
  {
    Dump_Obj dump_obj("ImplementationRequirement", ir);
    DnC_Dump::dump_sequence ("resourceUsage", ir.resourceUsage); 
    ACE_DEBUG ((LM_DEBUG, "resourceType: %s", ir.resourceType.in ()));
    ACE_DEBUG ((LM_DEBUG, "name: %s", ir.name.in ()));
  }

  void DnC_Dump::dump(const Deployment::ResourceUsageKind &ruk)
  {
    Dump_Obj dump_obj("ResourceUsageKind", ruk);
  }

  void DnC_Dump::dump (
        const ::Deployment::ComponentImplementationDescription &cid)
  {
    ACE_DEBUG ((LM_DEBUG,
                "========================================================\n"));
    Dump_Obj dump_obj("      COMPONENT IMPLEMENTATION DESCRIPTION ", cid);
    ACE_DEBUG ((LM_DEBUG,
                "===================================-====================\n"));
    dump ("label", cid.label);
    dump ("UUID", cid.UUID);
    ACE_DEBUG ((LM_DEBUG, "%sImplements:\n", Dump_Obj::indent()));
    dump (cid.implements);
    for (CORBA::ULong i = 0; i < cid.assemblyImpl.length (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "\nAssemblyDescription %d: \n", i + 1));
        for (CORBA::ULong j = 0; j < cid.assemblyImpl[i].instance.length ();
             ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Instance: %d: \n", j + 1));
            ACE_DEBUG ((LM_DEBUG, "       Name: %s \n",
                        cid.assemblyImpl[i].instance[j].name.in ()));
            for (CORBA::ULong k = 0; 
                 k < cid.assemblyImpl[i].instance[j].
                     package.length ();
                 ++k)
              {
                ACE_DEBUG ((LM_DEBUG, "     Package: %d: \n", k + 1));
                ACE_DEBUG ((LM_DEBUG, "       UUID: %s \n", 
                  cid.assemblyImpl[i].instance[j].package[k].UUID.in ()));
                ACE_DEBUG ((LM_DEBUG, "       Label: %s \n", 
                  cid.assemblyImpl[i].instance[j].package[k].label.in ()));
                ACE_DEBUG ((LM_DEBUG, "       Realizes:\n"));
                dump (cid.assemblyImpl[i].instance[j].package[k].realizes);
                dump (cid.assemblyImpl[i].instance[j].package[k]);
              }
          }
        for (CORBA::ULong j = 0; j < cid.assemblyImpl[i].connection.length ();
             ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Connection: %d: \n", j + 1));
            ACE_DEBUG ((LM_DEBUG, "       Name: %s \n",
                        cid.assemblyImpl[i].connection[j].name.in ()));
            for (CORBA::ULong k = 0; 
                 k < cid.assemblyImpl[i].connection[j].
                     internalEndpoint.length ();
                 ++k)
              {
                ACE_DEBUG ((LM_DEBUG, "     Endpoint: %d: \n", k + 1));
                ACE_DEBUG ((LM_DEBUG, "       Portname: %s \n",
                            cid.assemblyImpl[i].connection[j].
                            internalEndpoint[k].portName.in ()));
                int value = cid.assemblyImpl[i].connection[j].
                            internalEndpoint[k].instanceRef;
                ACE_DEBUG ((LM_DEBUG, "       Instance: %s \n",
                        cid.assemblyImpl[i].instance[value].name.in ()));
              }
          }
        for (CORBA::ULong j = 0; 
             j < cid.assemblyImpl[i].externalProperty.length ();
             ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     ExternalProperty: %d: \n", j + 1));
          }
      }
    for (CORBA::ULong i = 0; i < cid.monolithicImpl.length (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "\nMonolithicImplDescription %d: \n", i + 1));
        for (CORBA::ULong j = 0; 
             j < cid.monolithicImpl[i].primaryArtifact.length ();
             ++j)
          {
            ACE_DEBUG ((LM_DEBUG, "     Artifact: %d: \n", j + 1));
            ACE_DEBUG ((LM_DEBUG, "       Name: %s \n",
                        cid.monolithicImpl[i].primaryArtifact[j].name.in ()));
            dump (cid.monolithicImpl[i].primaryArtifact[j].
                  referencedArtifact);
          }
      }
  }

  void DnC_Dump::dump(const Deployment::DeploymentPlan &plan)
  {
    ACE_DEBUG ((LM_DEBUG,
                "========================================================\n"));
    Dump_Obj dump_obj("              DEPLOYMENT PLAN", plan);
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
            ACE_DEBUG ((LM_DEBUG, "       "));
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
            ACE_DEBUG ((LM_DEBUG, "       "));
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
            ACE_DEBUG ((LM_DEBUG, "       "));
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
    Dump_Obj dump_obj("MonolithicDeploymentDescription", mdd);
    dump ("name", mdd.name);
    dump ("source", mdd.source);
    ACE_DEBUG ((LM_DEBUG, "%sartifactRef: ???\n", Dump_Obj::indent()));
    dump_sequence ("execParameter", mdd.execParameter);
    dump_sequence ("deployRequirement", mdd.deployRequirement);
  }

  void DnC_Dump::dump(const Deployment::InstanceDeploymentDescription &idd)
  {
    Dump_Obj dump_obj("InstanceDeploymentDescription", idd);
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
    Dump_Obj dump_obj("PlanConnectionDescription", pcd);
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
    Dump_Obj dump_obj("PlanPropertyMapping", ppm);
    dump ("name", ppm.name);
    dump ("source", ppm.source);
    dump ("externalName", ppm.externalName);
    dump_sequence ("delegatesTo", ppm.delegatesTo);
  }

  void DnC_Dump::dump(const Deployment::ImplementationDependency &id)
  {
    Dump_Obj dump_obj("ImplementationDependency", id);
    dump ("requiredType", id.requiredType);
  }

  void DnC_Dump::dump(const Deployment::ArtifactDeploymentDescription &add)
  {
    Dump_Obj dump_obj("ArtifactDeploymentDescription", add);
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
    Dump_Obj dump_obj("InstanceResourceDeploymentDescription", irdd);
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
    Dump_Obj dump_obj("PlanSubcomponentPortEndpoint", pspe);
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
    Dump_Obj dump_obj("ConnectionResourceDeploymentDescription", crdd);
    dump ("targetName", crdd.targetName);
    dump ("requirementName", crdd.requirementName);
    dump ("resourceName", crdd.resourceName);
    ACE_DEBUG ((LM_DEBUG, "%sresourceValue:\n", Dump_Obj::indent()));
    dump (crdd.resourceValue);
  }

  void DnC_Dump::dump(const PlanSubcomponentPropertyReference &pspr)
  {
    Dump_Obj dump_obj("PlanSubcomponentPropertyReference", pspr);
    dump ("propertyName", pspr.propertyName);
    ACE_DEBUG ((LM_DEBUG, "%sinstanceRef: ???\n", Dump_Obj::indent()));
  }

  void DnC_Dump::dump(const ResourceDeploymentDescription &rdd)
  {
    Dump_Obj dump_obj("ResourceDeploymentDescription", rdd);
    dump ("requirementName", rdd.requirementName);
    dump ("resourceName", rdd.resourceName);
    ACE_DEBUG ((LM_DEBUG, "%sresourceValue:\n", Dump_Obj::indent()));
    dump (rdd.resourceValue);
  }
}

#endif /* DNC_DUMP_C */
