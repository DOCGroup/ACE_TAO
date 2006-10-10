/* -*- C++ -*- */

//========================================================================
/*
 *  file  PlanUpdater.cpp
 *
 *  $Id$
 *
 *  This file contains the implementation of the PackageConfiguration
 *  Visitor class PlanUpdater which derives from PCVisitorBase and attempts
 *  to update the location fields in the received DeploymentPlan to reflect
 *  the locations of the implementation artifacts as outlined in the
 *  PackageConfiguration which are downloadable via HTTP. Each Visit
 *  function focuses on the functionality necessary to process the
 *  PackageConfiguration element which is passed to it as an argument
 *  and on dispatching the next sequence of calls in the correct order!
 *
 *  author Stoyan Paunov <spaunov@isis.vanderbilt.edu
 */
//========================================================================

#include "PCVisitorBase.h"
#include "PlanUpdater.h"

#include "ace/OS_Memory.h"         //for ACE_NEW* macros
#include "ace/SString.h"           //for ACE_CString

#include "ciao/DeploymentC.h"
#include "ciao/Deployment_DataC.h"
#include "ciao/Packaging_DataC.h"

#include "Config_Handlers/DnC_Dump.h"

//#include <iostream>
//using namespace std;

  //Constructor
  PlanUpdater::PlanUpdater (Deployment::DeploymentPlan &plan,
                        Deployment::PackageConfiguration &pc)
    : PCVisitorBase (),
      plan_ (plan),
      pc_ (pc),
      status_ (true)
  {
  }

  //entry point for the protected visitor to get it do start
  //the visitation process
  bool PlanUpdater::Visit ()
  {
    Accept (*this, this->pc_);

    return this->status_;

  }

  // A whole slew of overloaded routines for different IDL
  // data types part of the PackageConfiguration.


  void PlanUpdater::Visit (Deployment::PackageConfiguration &pc)
  {
    //visit the ComponentPackageDescription
    if (pc.basePackage.length ())
    {
      //currently no support for that anywhere
      //for (size_t r = 0; r = pc.selectRequirement.length (); ++r);

      Accept (*this, pc.basePackage);
    }
    else
      ACE_DEBUG ((LM_WARNING,
                 "[PlanUpdater - PackageConfiguration] We currently "
                 "do NOT support package references, specializedConfigs",
                 "or imports!\n"));
  }

  //!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!

  //ComponentPackageDescription descendents

  void PlanUpdater::Visit (Deployment::ComponentPackageDescription &cpd)
  {
    //do not need to visit the interface
    //Accept (*this, cpd.realizes);

    //visit the implementations
    Accept (*this, cpd.implementation);
  }


  void PlanUpdater::Visit (Deployment::ComponentInterfaceDescription &cid)
  {
    //Might want to populate this too once PICML starts supporting it
  }


  void PlanUpdater::Visit (Deployment::PackagedComponentImplementation &pci)
  {
    //visit the referencedImplementationArtifact
    Accept (*this, pci.referencedImplementation);
  }


  void PlanUpdater::Visit (Deployment::ComponentImplementationDescription &cid)
  {
    if (cid.assemblyImpl.length ())
       //visit the component assembly
       Accept (*this, cid.assemblyImpl);
    else
       //visit the monolithic component
       Accept (*this, cid.monolithicImpl);
  }


  void PlanUpdater::Visit (Deployment::ComponentAssemblyDescription &cad)
  {
    //visit the SubcomponentInstantiationDescription
    Accept (*this, cad.instance);

    //do not need to visit the connections
    //Accept (*this, cad.connection);
  }


  void PlanUpdater::Visit (Deployment::SubcomponentInstantiationDescription &sid)
  {
    //visit the ComponentPackageDescription (again)
    if (sid.basePackage.length ())
      //visit the base package in the subcomponent
      Accept (*this, sid.basePackage);
    else
      ACE_DEBUG ((LM_WARNING,
                  "[PlanUpdater - SubcomponentInstantiationDescription] ",
                  "We currently do NOT support package references, ",
                  "specializedConfigs or imports!\n"));
  }


  void PlanUpdater::Visit (Deployment::MonolithicImplementationDescription &mid)
  {
    //NOTE: There are usually 3 NamedImplementationArtifacts per
    //MonolithicImplementationDescription *_stub, *_svnt & *_exec

    //visit the NamedImplementationArtifacts
    Accept (*this, mid.primaryArtifact);
  }


  void PlanUpdater::Visit (Deployment::NamedImplementationArtifact &nia)
  {
    //visit the actual ImplementationArtifactDescriptor
    Accept (*this, nia.referencedArtifact);
  }


  //This function attempts to update the location of the artifact deployment
  //descriptions to reflect the once in the RepositoryManager.
  //
  //Not sure what the input is here! I am assuming that the
  //location came from the descriptor files, so it just holds
  //the name of the library. If this code evolves, we will need to
  //update the string matching mechanism with a more complicated one.


  void PlanUpdater::Visit (Deployment::ImplementationArtifactDescription &iad)
  {
    //some heavy lifting here!
    static size_t iters = 0;
    ++iters;
    ACE_CString iad_loc (iad.location[0]);

    size_t add_len = this->plan_.artifact.length ();

    for (size_t i = 0; i < add_len; ++i)
    {
      Deployment::ArtifactDeploymentDescription& add = this->plan_.artifact[i];

      //NOTE: Right now we only populate location[0]
      //When this evolves, check needs to evolve as well.
      ACE_CString add_loc (add.location[0]);

      //check if the add location has already been updated
      if (ACE_OS::strstr (add_loc.c_str (), "http://"))//, add_loc.length ()))
        continue;

      //check for a match and update the location
      if (ACE_OS::strstr (iad_loc.c_str (), add_loc.c_str ()))//, iad_loc.length ()))
      {
        //if there is a match substitute one for the other
        add.location[0] = CORBA::string_dup (iad_loc.c_str ());

        //ACE_OS::printf ("Matching %s with %s\n",
        //                 iad_loc.c_str (),
        //                 add_loc.c_str ());

      }

    }
  }

  //!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!

  //ComponentPackageReference descendents

  void PlanUpdater::Visit (Deployment::ComponentPackageReference &cpr)
  {
    //not implemented
  }

  //!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!

  //properties

  void PlanUpdater::Visit (Deployment::AssemblyPropertyMapping &apm)
  {
    //not needed
  }


  void PlanUpdater::Visit (Deployment::Property &property)
  {
     //not needed
  }


  //requirements & capabilities

  void PlanUpdater::Visit (Deployment::Requirement &requirement)
  {
    //not needed
  }


  void PlanUpdater::Visit (Deployment::Capability &capability)
  {
    //not needed
  }


  void PlanUpdater::Visit (Deployment::ImplementationRequirement &ir)
  {
    //not needed
  }


  void PlanUpdater::Visit (Deployment::ImplementationDependency &id)
  {
    //not needed
  }

  //ports and connections

  void PlanUpdater::Visit (Deployment::AssemblyConnectionDescription &acd)
  {
    //not needed
  }


  void PlanUpdater::Visit (Deployment::SubcomponentPortEndpoint &spe)
  {
    //not needed
  }


  void PlanUpdater::Visit (Deployment::ComponentExternalPortEndpoint &cepe)
  {
    //not needed
  }
