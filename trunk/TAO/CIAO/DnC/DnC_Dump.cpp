// $Id$

#ifndef DNC_DUMP_C
#define DNC_DUMP_C

#include "ace/Log_Msg.h"

#include "DnC_Dump.h"
#include <iostream>

namespace Deployment
{
  // AssemblyConnectionDescription

  void DnC_Dump::dump (const ::Deployment::AssemblyConnectionDescription &acd)
  {
    ACE_DEBUG ((LM_DEBUG, "AssemblyConnectionDescription: \n"));
    
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
    ACE_DEBUG ((LM_DEBUG, "AssemblyPropertyMapping: \n"));
    
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
    ACE_DEBUG ((LM_DEBUG, "ComponentPackageDescription: \n"));
    
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
    ACE_DEBUG ((LM_DEBUG, "ComponentPortDescription: \n"));
    
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
    ACE_DEBUG ((LM_DEBUG, "ComponentPropertyDescription: \n"));  
    ACE_DEBUG ((LM_DEBUG, "  name: %s\n", comppropdesc.name.in ()));
    ACE_DEBUG ((LM_DEBUG, "  type: \n"));
  }

  // MonolithicImplementationDescription

  void DnC_Dump::dump (const ::Deployment::MonolithicImplementationDescription &mid)
  {
    ACE_DEBUG ((LM_DEBUG, "MonolithicImplementationDescription: \n"));
    
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
    ACE_DEBUG ((LM_DEBUG, "PackageConfiguration: \n"));  
    ACE_DEBUG ((LM_DEBUG, "  label: %s\n", pc.label.in ()));
    ACE_DEBUG ((LM_DEBUG, "  UUID: %s\n", pc.UUID.in ()));

    ACE_DEBUG ((LM_DEBUG, "  specializedConfig: \n"));
    for (CORBA::ULong i = 0;
          i < pc.specializedConfigRef.length ();
          ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "%i\n", pc.specializedConfigRef[i]));
      }

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
    ACE_DEBUG ((LM_DEBUG, "PackagedComponentImplementation: \n"));  
    ACE_DEBUG ((LM_DEBUG, "  name: %s\n", pci.name.in ()));

    ACE_DEBUG ((LM_DEBUG, "  referencedImplementation: \n"));
    DnC_Dump::dump (pci.referencedImplementation); // ComponentImplementationDescription
  }

  // SubcomponentPortEndpoint

  void DnC_Dump::dump (const ::Deployment::SubcomponentPortEndpoint &spe)
  {
    ACE_DEBUG ((LM_DEBUG, "SubcomponentPortEndpoint: \n"));
    
    ACE_DEBUG ((LM_DEBUG, "  portName: %s\n", spe.portName.in ()));
    
    ACE_DEBUG ((LM_DEBUG, "  instance: \n"));
    for (CORBA::ULong i = 0;
          i < spe.instance.length ();
          ++i)
      {
        DnC_Dump::dump (spe.instance[i]); // SubcomponentInstantiationDescription
      }
  }
}



#endif /* DNC_DUMP_C */
