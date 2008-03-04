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

#include "ace/OS.h"

//Constructor
PCVisitor::PCVisitor (Deployment::DeploymentPlan &plan,
                      Deployment::PackageConfiguration &pc,
                      bool modify)
                      : PCVisitorBase (),
                      plan_ (plan),
                      pc_ (pc),
                      modify_ (modify),
                      last_impl_index_ (-1)
{
}

//entry point for the protected visitor to get it do start
//the visitation process
int PCVisitor::Visit ()
{
  Accept (*this, this->pc_);
  return last_impl_index_;
}

// A whole slew of overloaded routines for different IDL
// data types part of the PackageConfiguration.

void PCVisitor::Visit (Deployment::PackageConfiguration &pc)
{
  //visit the ComponentPackageDescription
  if (pc.basePackage.length ())
  {
    //currently no support for that anywhere
    //for (size_t r = 0; r = pc.selectRequirement.length (); ++r);

    Accept (*this, pc.basePackage);
  }
  else
    DANCE_WARN(("[PCVisitor - PackageConfiguration] We currently "
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
    DANCE_WARN(("[PCVisitor - SubcomponentInstantiationDescription] ",
    "We currently do NOT support package references, ",
    "specializedConfigs or imports!\n"));
}


void PCVisitor::Visit (Deployment::MonolithicImplementationDescription &mid)
{
  if (!modify_)
    {
      //increase the implementation length by one
      size_t const impl_len = plan_.implementation.length ();
      last_impl_index_ = impl_len;
      plan_.implementation.length (impl_len + 1);
    }

  //visit the NamedImplementationArtifacts
  Accept (*this, mid.primaryArtifact);
}


void PCVisitor::Visit (Deployment::NamedImplementationArtifact &nia)
{
  if (!modify_)
    {
      //increase the artifact length by one
      size_t const arti_len = plan_.artifact.length ();
      plan_.artifact.length (arti_len + 1);

      //set the name
      plan_.artifact[arti_len].name = nia.name;

      // Set the artifactRef of implementation
      size_t const last_mdd = plan_.implementation.length () - 1;
      Deployment::MonolithicDeploymentDescription& mdd = plan_.implementation[last_mdd];
      size_t const ref_len = mdd.artifactRef.length ();
      mdd.artifactRef.length (ref_len + 1);
      mdd.artifactRef[ref_len] = arti_len;
    }

  //visit the actual ImplementationArtifactDescriptor
  Accept (*this, nia.referencedArtifact);
}


void PCVisitor::Visit (Deployment::ImplementationArtifactDescription &iad)
{
  if (!modify_)
    {
      size_t last_arti = plan_.artifact.length ();
      Deployment::ArtifactDeploymentDescription& add = plan_.artifact[last_arti - 1];

      //set the location
      size_t plan_loc_len = add.location.length ();
      size_t const num_loc = iad.location.length ();
      for (size_t i = 0; i < num_loc; ++i)
      {
        add.location.length (plan_loc_len + 1);
        add.location[plan_loc_len] = iad.location[i];
        ++plan_loc_len;
      }

      //set the execParameter
      update_execParameter (iad, add);
    }

  else
    {
      size_t const num_arti = plan_.artifact.length ();
      for (size_t i = 0; i < num_arti; ++i)
        {
          if (ACE_OS::strstr (iad.location[0], plan_.artifact[i].location[0]))
            plan_.artifact[i].location[0] = iad.location[0];
        }
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

void PCVisitor::
update_execParameter (Deployment::ImplementationArtifactDescription& iad,
                      Deployment::ArtifactDeploymentDescription& add)
{
  size_t const num_execP = iad.execParameter.length ();
  size_t execP_len = add.execParameter.length ();
  for (size_t j = 0; j < num_execP; ++j)
  {
    add.execParameter.length (execP_len + 1);
    add.execParameter[execP_len] = iad.execParameter[j];
    ++execP_len;
  }
}
