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
      ACE_DEBUG ((LM_DEBUG, "%s%s: ", indent_.c_str(), caption));
      indent_.append("  ");
    }

    ~Dump_Obj() {
      indent_.erase(indent_.size() - 2, 2);
    }

    const char* indent() {
      return indent_.c_str();
    }
   
  private:
    static std::string indent_;
  };

  std::string Dump_Obj::indent_ = "";

  // Dumps a string sequence
  void DnC_Dump::dump (const char* caption, const ::CORBA::StringSeq &str_seq)
  {
    ACE_DEBUG ((LM_DEBUG, "%s: ", caption));

    CORBA::ULong size = str_seq.length ();
    for (CORBA::ULong i = 0; i < size; ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "%s%s ",
                    str_seq[i].in (), (i < size) ? ", " : "\n"));
      }
  }

  // Dumps a sequence
  template <typename SEQUENCE>
  void DnC_Dump::dump_sequence (const char* caption, const SEQUENCE &seq)
  {
    ACE_DEBUG ((LM_DEBUG, "%s: ", caption));

    for (CORBA::ULong i = 0; i < seq.length (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "  %s %d: \n", caption, i));
        DnC_Dump::dump (seq[i]);
      }
  }

  // AssemblyConnectionDescription

  void DnC_Dump::dump (const ::Deployment::AssemblyConnectionDescription &acd)
  {
    Dump_Obj ("AssemblyConnectionDescription");

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
    Dump_Obj ("AssemblyPropertyMapping");

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
    Dump_Obj ("ComponentPackageDescription");

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
    Dump_Obj ("ComponentPortDescription");

    ACE_DEBUG ((LM_DEBUG, "  name: %s\n", compportdesc.name.in ()));
    ACE_DEBUG ((LM_DEBUG, "  specificType: %s\n", compportdesc.specificType.in ()));

    ACE_DEBUG ((LM_DEBUG, "  supportedType: \n"));
    for (CORBA::ULong i = 0;
         i < compportdesc.supportedType.length ();
         ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "%s\n", compportdesc.supportedType[i].in ()));
      }

    ACE_DEBUG ((LM_DEBUG, "  provider: %i\n", compportdesc.provider));
    ACE_DEBUG ((LM_DEBUG, "  exclusiveProvider: %i\n", compportdesc.exclusiveProvider));
    ACE_DEBUG ((LM_DEBUG, "  exclusiveUser: %i\n", compportdesc.exclusiveUser));
    ACE_DEBUG ((LM_DEBUG, "  optional: %i\n", compportdesc.optional));
  }

  // ComponentPropertyDescription

  void DnC_Dump::dump (const ::Deployment::ComponentPropertyDescription &comppropdesc)
  {
    Dump_Obj ("ComponentPropertyDescription");
    ACE_DEBUG ((LM_DEBUG, "  name: %s\n", comppropdesc.name.in ()));
    //ACE_DEBUG ((LM_DEBUG, "  type: %s\n", TCKind_Names::get_name(comppropdesc.type.in()->kind())));
  }

  // MonolithicImplementationDescription

  void DnC_Dump::dump (const ::Deployment::MonolithicImplementationDescription &mid)
  {
    Dump_Obj ("MonolithicImplementationDescription");

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
    Dump_Obj ("PackageConfiguration");
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
    Dump_Obj ("PackagedComponentImplementation");
    ACE_DEBUG ((LM_DEBUG, "  name: %s\n", pci.name.in ()));

    ACE_DEBUG ((LM_DEBUG, "  referencedImplementation: \n"));
    DnC_Dump::dump (pci.referencedImplementation); // ComponentImplementationDescription
  }

  // SubcomponentPortEndpoint

  void DnC_Dump::dump (const ::Deployment::SubcomponentPortEndpoint&)
  {
    Dump_Obj ("SubcomponentPortEndpoint");
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
    Dump_Obj ("Requirement");
    ACE_DEBUG ((LM_DEBUG, "resourceType: %s \n", req.resourceType.in ()));
    for (::CORBA::ULong i = 0; i < req.property.length (); i ++)
      DnC_Dump::dump (req.property [i]);
    ACE_DEBUG ((LM_DEBUG, "name: %s \n", req.name.in ()));
  }

  void DnC_Dump::dump (const ::Deployment::ComponentExternalPortEndpoint &)
  {
    Dump_Obj ("ComponentExternalPortEndpoint");
  }

  void DnC_Dump::dump (const ::Deployment::ComponentPackageReference &)
  {
    Dump_Obj ("ComponentPackageReference");
  }

  void DnC_Dump::dump (const ::Deployment::ComponentImplementationDescription &)
  {
    Dump_Obj ("ComponentImplementationDescription");
  }

  void DnC_Dump::dump (const ::Deployment::SubcomponentInstantiationDescription &)
  {
    Dump_Obj ("SubcomponentInstantiationDescription");
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
          ACE_DEBUG ((LM_DEBUG, "Any value: %l \n", temp));
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

  void DnC_Dump::dump (const ::Deployment::Property &property)
  {
    Dump_Obj ("Property");
    ACE_DEBUG ((LM_DEBUG, "name: %s \n", property.name.in ()));

    // Print the Any value stored
    DnC_Dump::dump (property.value);
  }

  void DnC_Dump::dump (const ::Deployment::NamedImplementationArtifact &nia)
  {
    Dump_Obj ("NamedImplementationArtifact");
    ACE_DEBUG ((LM_DEBUG, "name: %s \n", nia.name.in ()));
    ACE_DEBUG ((LM_DEBUG, "referencedArtifact: \n"));
    DnC_Dump::dump (nia.referencedArtifact);  // ImplementationArtifactDescription
  }

  void DnC_Dump::dump (const ::Deployment::ComponentInterfaceDescription &cid)
  {
    Dump_Obj ("ComponentInterfaceDescription");
    ACE_DEBUG ((LM_DEBUG, "label: %s \n", cid.label.in ()));
    ACE_DEBUG ((LM_DEBUG, "UUID: %s \n", cid.UUID.in ()));
    ACE_DEBUG ((LM_DEBUG, "specificType: %s \n", cid.specificType.in ()));
    DnC_Dump::dump ("supportedType", cid.supportedType); // string sequence
    //    ACE_DEBUG ((LM_DEBUG, "idlFile: %s \n", cid.idlFile.in ()));
    DnC_Dump::dump_sequence ("configProperty", cid.configProperty); // Property seq.
    DnC_Dump::dump_sequence ("infoProperty", cid.infoProperty); // Property seq.
    DnC_Dump::dump_sequence ("port", cid.port); // ComponentPortDescription seq.
    DnC_Dump::dump_sequence ("property", cid.property); // ComponentPropertyDescription seq.
  }

  void DnC_Dump::dump (const ::Deployment::SubcomponentPropertyReference &)
  {
    Dump_Obj ("SubcomponentPropertyReference");
  }

  void DnC_Dump::dump (const ::Deployment::Domain &domain)
  {
    Dump_Obj ("Domain");
    ACE_DEBUG ((LM_DEBUG, "UUID: %s \n", domain.UUID.in ()));
    ACE_DEBUG ((LM_DEBUG, "label: %s \n", domain.label.in ()));

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
  }

  void DnC_Dump::dump (const ::Deployment::ExternalReferenceEndpoint &ere)
  {
    Dump_Obj ("ExternalReferenceEndpoint");
    ACE_DEBUG ((LM_DEBUG, "location: %s \n", ere.location.in ()));
  }

  void DnC_Dump::dump (const ::Deployment::Capability &capability)
  {
    Dump_Obj ("Capability");
    ACE_DEBUG ((LM_DEBUG, "name: %s", capability.name.in ()));
    DnC_Dump::dump ("resourceType", capability.resourceType); // string sequence
    DnC_Dump::dump_sequence ("property", capability.property); // SatisfierProperty sequence
  }

  void DnC_Dump::dump (const ::Deployment::ImplementationArtifactDescription &iad)
  {
    Dump_Obj ("ImplementationArtifactDescription");
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
    Dump_Obj ("ImplementationRequirement");
    //    DnC_Dump::dump ("resourcePort", ir.resourcePort); // string sequence
    //    DnC_Dump::dump ("componentPort", ir.componentPort); // string sequence
    DnC_Dump::dump_sequence ("resourceUsage", ir.resourceUsage); // ResourceUsageKind seq.
    ACE_DEBUG ((LM_DEBUG, "resourceType: %s", ir.resourceType.in ()));
    ACE_DEBUG ((LM_DEBUG, "name: %s", ir.name.in ()));
  }

  void DnC_Dump::dump(const Deployment::SatisfierProperty&)
  {
    Dump_Obj ("SatisfierProperty");
  }

  void DnC_Dump::dump(const Deployment::ResourceUsageKind&)
  {
    Dump_Obj ("ResourceUsageKind");
  }

  void DnC_Dump::dump(const Deployment::DeploymentPlan &)
  {
    Dump_Obj ("DeploymentPlan");
  }
}

#endif /* DNC_DUMP_C */
