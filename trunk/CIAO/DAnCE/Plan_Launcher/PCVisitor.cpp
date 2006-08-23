/* -*- C++ -*- */

//========================================================================
/*
 * @file  PCVisitor.cpp
 *
 * $Id$
 *
 * This file contains the implementation of the PackageConfiguration
 * Visitor class PCVisitor which derives from PCVisitorBase. Each
 * Visit function focuses on the functionality necessary to process
 * the PackageConfiguration element which is passed to it as an argument
 * and on dispatching the next sequence of calls in the correct order!
 *
 * This implementation takes a PackageConfiguration and tries to modify
 * a DeploymentPlan bases on it by expanding the latter in width and depth
 * simultaneously. At each level of the PackageConfiguration the
 * PCVisitor first expands the DeploymentPlan vertically at the
 * corrsponding level and then dispatches the children of the current
 * PackageConfiguration element. This in turn might and most probably
 * will cause another vertical expansion of the DeploymentPlan, however
 * for a different element. This effect is produced due to the flattened
 * structure of the DeploymentPlan.
 *
 * @author Stoyan Paunov <spaunov@isis.vanderbilt.edu>
 *         Shanshan Jiang <shanshan.jiang@vanderbilt.edu> 
 */
//========================================================================

#include "PCVisitorBase.h"
#include "PCVisitor.h"

#include "ace/OS_Memory.h"         //for ACE_NEW* macros
#include "ace/SString.h"           //for ACE_CString

#include "ciao/DeploymentC.h"
#include "ciao/Deployment_DataC.h"
#include "ciao/Packaging_DataC.h"

#include "Config_Handlers/DnC_Dump.h"

#include <iostream>
using namespace std;

//Constructor
PCVisitor::PCVisitor (Deployment::DeploymentPlan &plan,
                      Deployment::PackageConfiguration &pc)
                      : PCVisitorBase (),
                      plan_ (plan),
                      pc_ (pc)
{
}

//entry point for the protected visitor to get it do start
//the visitation process
void PCVisitor::Visit ()
{
  Accept (*this, this->pc_);
}

// A whole slew of overloaded routines for different IDL
// data types part of the PackageConfiguration.

void PCVisitor::Visit (Deployment::PackageConfiguration &pc)
{
  cout << "Flattening:  " << pc.label << endl;

  //visit the ComponentPackageDescription
  if (pc.basePackage.length ())
  {
    //currently no support for that anywhere
    //for (size_t r = 0; r = pc.selectRequirement.length (); ++r);

    Accept (*this, pc.basePackage);
  }
  else
    ACE_DEBUG ((LM_WARNING,
    "[PCVisitor - PackageConfiguration] We currently "
    "do NOT support package references, specializedConfigs",
    "or imports!\n"));
}

//ComponentPackageDescription descendents

void PCVisitor::Visit (Deployment::ComponentPackageDescription &cpd)
{
  Accept (*this, cpd.realizes);
  //for (size_t impl = 0; impl < cpd.implementation.length (); ++impl)
  Accept (*this, cpd.implementation[0]);
}


void PCVisitor::Visit (Deployment::ComponentInterfaceDescription &)
{
  //Might want to populate this too once PICML starts supporting it
}


void PCVisitor::Visit (Deployment::PackagedComponentImplementation &pci)
{
  Accept (*this, pci.referencedImplementation);
}


void PCVisitor::Visit (Deployment::ComponentImplementationDescription &cid)
{
  if (cid.assemblyImpl.length ())
    Accept (*this, cid.assemblyImpl);
  else
    //;//Do nothing - monolithic component deployment not supported
    Accept (*this, cid.monolithicImpl);
}


void PCVisitor::Visit (Deployment::ComponentAssemblyDescription &cad)
{
  //visit the SubcomponentInstantiationDescription
  Accept (*this, cad.instance);
  //visit the connections
  Accept (*this, cad.connection);
}


void PCVisitor::Visit (Deployment::SubcomponentInstantiationDescription &sid)
{
  //visit the ComponentPackageDescription (again)
  if (sid.basePackage.length ())
  {
    Accept (*this, sid.basePackage);
  }
  else
    ACE_DEBUG ((LM_WARNING,
    "[PCVisitor - SubcomponentInstantiationDescription] ",
    "We currently do NOT support package references, ",
    "specializedConfigs or imports!\n"));
}


void PCVisitor::Visit (Deployment::MonolithicImplementationDescription &mid)
{
  //NOTE: There are usually 3 NamedImplementationArtifacts per
  //MonolithicImplementationDescription *_stub, *_svnt & *_exec

  //visit the NamedImplementationArtifacts
  Accept (*this, mid.primaryArtifact);
}


void PCVisitor::Visit (Deployment::NamedImplementationArtifact &nia)
{
  //visit the actual ImplementationArtifactDescriptor
  Accept (*this, nia.referencedArtifact);
}


void PCVisitor::Visit (Deployment::ImplementationArtifactDescription &iad)
{
  size_t num_arti = plan_.artifact.length ();
  for (size_t i = 0; i < num_arti; ++i)
  {
    if (ACE_OS::strstr (iad.location[0], plan_.artifact[i].location[0]))
      plan_.artifact[i].location[0] = iad.location[0];
  }
}


//ComponentPackageReference descendents

void PCVisitor::Visit (Deployment::ComponentPackageReference &)
{
  //not implemented
}


//properties

void PCVisitor::Visit (Deployment::AssemblyPropertyMapping &)
{
}


void PCVisitor::Visit (Deployment::Property &)
{
}


//requirements & capabilities

void PCVisitor::Visit (Deployment::Requirement &)
{
}


void PCVisitor::Visit (Deployment::Capability &)
{
}


void PCVisitor::Visit (Deployment::ImplementationRequirement &)
{
}


void PCVisitor::Visit (Deployment::ImplementationDependency &)
{
}

//ports and connections

void PCVisitor::Visit (Deployment::AssemblyConnectionDescription &)
{
}


void PCVisitor::Visit (Deployment::SubcomponentPortEndpoint &)
{
}


void PCVisitor::Visit (Deployment::ComponentExternalPortEndpoint &)
{
}
