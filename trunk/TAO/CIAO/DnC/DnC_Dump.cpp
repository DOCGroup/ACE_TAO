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
