/* -*- C++ -*- */

//========================================================================
/*
 *  file  Plan_Generator.cpp
 *
 * $Id$
 *
 *  This file contains the implementation of the PackageConfiguration
 *  Visitor class Plan_Generator which derives from PCVisitorBase. Each
 *  Visit function focuses on the functionality necessary to process
 *  the PackageConfiguration element which is passed to it as an argument
 *  and on dispatching the next sequence of calls in the correct order!
 *
 *  This implementation takes a PackageConfiguration and tries to grow
 *  a DeploymentPlan out of it by expanding the latter in width and depth
 *  simultaneously. At each level of the PackageConfiguration the
 *  Plan_Generator first expands the DeploymentPlan vertically at the
 *  corrsponding level and then dispatches the children of the current
 *  PackageConfiguration element. This in turn might and most probably
 *  will cause another vertical expansion of the DeploymentPlan, however
 *  for a different element. This effect is produced due to the flattened
 *  structure of the DeploymentPlan.
 *
 *  author Stoyan Paunov <spaunov@isis.vanderbilt.edu
 */
//========================================================================

#include "PCVisitorBase.h"
#include "PlanGenerator.h"

#include "ace/OS_Memory.h"         //for ACE_NEW* macros
#include "ace/SString.h"           //for ACE_CString

#include "ciao/DeploymentC.h"
#include "ciao/Deployment_DataC.h"
#include "ciao/Packaging_DataC.h"

#include "Config_Handlers/DnC_Dump.h"

#include <iostream>
using namespace std;

  //Constructor
  Plan_Generator::Plan_Generator (Deployment::DeploymentPlan &plan,
                        Deployment::PackageConfiguration &pc)
    : PCVisitorBase (),
      plan_ (plan),
      pc_ (pc),
      last_cad_ (0)
  {
  }

  //entry point for the protected visitor to get it do start
  //the visitation process
  void Plan_Generator::Visit ()
  {
    Accept (*this, this->pc_);
  }

  // A whole slew of overloaded routines for different IDL
  // data types part of the PackageConfiguration.


  void Plan_Generator::Visit (Deployment::PackageConfiguration &pc)
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
                 "[Plan_Generator - PackageConfiguration] We currently "
                 "do NOT support package references, specializedConfigs",
                 "or imports!\n"));
  }

  //!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!

  //ComponentPackageDescription descendents

  void Plan_Generator::Visit (Deployment::ComponentPackageDescription &cpd)
  {
    Accept (*this, cpd.realizes);
    //for (size_t impl = 0; impl < cpd.implementation.length (); ++impl)
    Accept (*this, cpd.implementation[0]);
  }


  void Plan_Generator::Visit (Deployment::ComponentInterfaceDescription &cid)
  {
    //Might want to populate this too once PICML starts supporting it
  }


  void Plan_Generator::Visit (Deployment::PackagedComponentImplementation &pci)
  {
    Accept (*this, pci.referencedImplementation);

    size_t plan_impl_len = plan_.implementation.length ();
    size_t plan_inst_len = plan_.instance.length ();

    //NOTE: order here matters. Need to populate the
    //MonolithicDeploymentDescription and the
    //InstanceDeploymentDescription first,
    //then call the update_configProperty
    if (plan_impl_len > 0 && plan_inst_len > 0)
       update_configProperty (pci,
                    plan_.implementation[plan_impl_len - 1],
                    plan_.instance[plan_inst_len - 1]);
  }


  void Plan_Generator::Visit (Deployment::ComponentImplementationDescription &cid)
  {
    if (cid.assemblyImpl.length ())
       Accept (*this, cid.assemblyImpl);
    else
       //;//Do nothing - monolithic component deployment not supported
       Accept (*this, cid.monolithicImpl);
  }


  void Plan_Generator::Visit (Deployment::ComponentAssemblyDescription &cad)
  {
    //NOTE: order matters for these calls. We need to populate
    //the instances before we try to polulate the connections

    //set the last ComponentAssemblyDescription
    this->last_cad_ = &cad;

    //visit the SubcomponentInstantiationDescription
    Accept (*this, cad.instance);
    //visit the connections
    Accept (*this, cad.connection);

    //reset the last ComponentAssemblyDescription
    this->last_cad_ = NULL;
  }


  void Plan_Generator::Visit (Deployment::SubcomponentInstantiationDescription &sid)
  {
    //visit the ComponentPackageDescription (again)
    if (sid.basePackage.length ())
    {
      size_t inst_len = plan_.instance.length ();
      plan_.instance.length (inst_len + 1);

      update_configProperty (sid, plan_.instance[inst_len]);

      plan_.instance[inst_len].name = sid.name;
      size_t impl_len = plan_.implementation.length ();
      plan_.implementation.length (impl_len + 1);

      ACE_CString mdname (plan_.instance[inst_len].name);
      mdname += "-mdd";
      plan_.implementation[impl_len].name = mdname.c_str ();

      plan_.instance[inst_len].implementationRef = impl_len;

      Accept (*this, sid.basePackage);
    }
    else
      ACE_DEBUG ((LM_WARNING,
                  "[Plan_Generator - SubcomponentInstantiationDescription] ",
                  "We currently do NOT support package references, ",
                  "specializedConfigs or imports!\n"));
  }


  void Plan_Generator::Visit (Deployment::MonolithicImplementationDescription &mid)
  {
    //NOTE: There are usually 3 NamedImplementationArtifacts per
    //MonolithicImplementationDescription *_stub, *_svnt & *_exec

    //visit the NamedImplementationArtifacts
    Accept (*this, mid.primaryArtifact);
  }


  void Plan_Generator::Visit (Deployment::NamedImplementationArtifact &nia)
  {
    //increase the artifact length by one
    size_t arti_len = plan_.artifact.length ();
    plan_.artifact.length (arti_len + 1);
    plan_.artifact[arti_len].name = nia.name;

    //increase the artifactRef length by one and
    //update the reference

    //get the index of the last MonolithicDeploymentDescription
    size_t last_mdd = plan_.implementation.length () - 1;
    Deployment::MonolithicDeploymentDescription& mdd = plan_.implementation[last_mdd];
    size_t ref_len = mdd.artifactRef.length ();
    mdd.artifactRef.length (ref_len + 1);
    mdd.artifactRef[ref_len] = arti_len;

    //visit the actual ImplementationArtifactDescriptor
    Accept (*this, nia.referencedArtifact);
  }


  void Plan_Generator::Visit (Deployment::ImplementationArtifactDescription &iad)
  {

    size_t last_arti = plan_.artifact.length ();
    size_t plan_loc_len = plan_.artifact[last_arti - 1].location.length ();
    size_t num_loc = iad.location.length ();
    for (size_t i = 0; i < num_loc; ++i)
    {
      Deployment::ArtifactDeploymentDescription& add = plan_.artifact[last_arti - 1];
      add.location.length (plan_loc_len + 1);
      add.location[plan_loc_len] = iad.location[i];
      ++plan_loc_len;

    //update execParameter
      update_execParameter (iad, add);
    }
  }

  //!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!

  //ComponentPackageReference descendents

  void Plan_Generator::Visit (Deployment::ComponentPackageReference &cpr)
  {
    //not implemented
  }

  //!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!

  //properties

  void Plan_Generator::Visit (Deployment::AssemblyPropertyMapping &apm)
  {

  }


  void Plan_Generator::Visit (Deployment::Property &property)
  {

  }


  //requirements & capabilities

  void Plan_Generator::Visit (Deployment::Requirement &requirement)
  {

  }


  void Plan_Generator::Visit (Deployment::Capability &capability)
  {

  }


  void Plan_Generator::Visit (Deployment::ImplementationRequirement &ir)
  {

  }


  void Plan_Generator::Visit (Deployment::ImplementationDependency &id)
  {

  }

  //ports and connections

  void Plan_Generator::Visit (Deployment::AssemblyConnectionDescription &acd)
  {
    if (!this->last_cad_)
      return;

    Deployment::ComponentAssemblyDescription& cad = *(this->last_cad_);

    size_t plan_conn_len = plan_.connection.length ();
    plan_.connection.length (plan_conn_len + 1);
    plan_.connection[plan_conn_len].name = acd.name;

    size_t iep_len = acd.internalEndpoint.length ();
    size_t plan_iep_len =
           plan_.connection[plan_conn_len].internalEndpoint.length ();

    //cout << "==> " << acd.name << " has " << iep_len << " and "
    //     << acd.externalEndpoint.length () << endl << endl ;

    plan_.connection[plan_conn_len].internalEndpoint.length (plan_iep_len + iep_len);

    for (size_t i = 0; i < iep_len; ++i)
    {
      plan_.connection[plan_conn_len]
           .internalEndpoint[plan_iep_len + i].portName =
                       acd.internalEndpoint[i].portName;

      size_t inst_ref = acd.internalEndpoint[i].instanceRef;
      const char* inst_name = cad.instance[inst_ref].name;
      size_t plan_inst_len = plan_.instance.length ();

      for (size_t j = 0; j < plan_inst_len; ++j)
      {
        const char* plan_inst_name = plan_.instance[j].name;
        if (ACE_OS::strcmp (plan_inst_name, inst_name) == 0)
        {
          plan_.connection[plan_conn_len]
               .internalEndpoint[plan_iep_len + i].instanceRef = j;
          break;
        }
      }

      //Now traverse the interface

      //NOTE: move these up and refactor!
      Deployment::SubcomponentInstantiationDescription& scid =
          cad.instance[inst_ref];
      Deployment::PlanSubcomponentPortEndpoint& pspe =
                  plan_.connection[plan_conn_len]
                       .internalEndpoint[plan_iep_len + i];

      size_t pkg_len = scid.basePackage.length ();
      for (size_t k = 0; k < pkg_len; ++k)
      {
        Deployment::ComponentPackageDescription& package = scid.basePackage[k];

        //empty unfortunately
        Deployment::ComponentInterfaceDescription cid = package.realizes;
        //cid = package.implementation[0].referencedImplementation.implements;

        size_t port_len = cid.port.length ();
        for (size_t p = 0; p < port_len; ++p)
        {
          const char* cid_pname = cid.port[p].name;
          const char* pspe_pname = pspe.portName;
          //cout << "CMP: " << pspe_pname << " <=> " << cid_pname << endl;
          if (ACE_OS::strcmp (cid_pname, pspe_pname) == 0)
          {
            pspe.kind = cid.port[p].kind;

            //cout << "-->" << pspe_pname << " Port kind is "
            //     << pspe.kind << endl;

            goto interface_end;
            break; // ???
          }
        }
      }

interface_end:
    //cout << endl;
    ;

    }

  }


  void Plan_Generator::Visit (Deployment::SubcomponentPortEndpoint &spe)
  {

  }


  void Plan_Generator::Visit (Deployment::ComponentExternalPortEndpoint &cepe)
  {

  }

//several helper functions

  void Plan_Generator::
  update_execParameter (Deployment::ImplementationArtifactDescription& iad,
                        Deployment::ArtifactDeploymentDescription& add)
  {
    //update execParameters
    size_t num_execP = iad.execParameter.length ();
    for (size_t j = 0; j < num_execP; ++j)
    {
      size_t execP_len = add.execParameter.length ();
      add.execParameter.length (execP_len + 1);
      add.execParameter[execP_len] = iad.execParameter[j];
      ++execP_len;
    }
  }

  void Plan_Generator::
  update_configProperty (Deployment::SubcomponentInstantiationDescription &scid,
                         Deployment::InstanceDeploymentDescription &idd)
  {
    size_t scid_plen =
    scid.configProperty.length ();

    size_t idd_plen = idd.configProperty.length ();
    idd.configProperty.length (idd_plen + scid_plen);

    for (size_t i = 0; i < scid_plen; ++i)
         idd.configProperty[idd_plen + i] =
                   scid.configProperty[i];

  }

  void Plan_Generator::
  update_configProperty (Deployment::PackagedComponentImplementation& pcid,
                         Deployment::MonolithicDeploymentDescription& mid,
                         Deployment::InstanceDeploymentDescription& idd)
  {
    bool update;
    size_t pcid_plen = pcid.referencedImplementation
                           .configProperty.length ();

    size_t mid_eplen = mid.execParameter.length ();
    mid.execParameter.length (mid_eplen + pcid_plen);

    for (size_t i = 0; i < pcid_plen; ++i)
    {
      update = true;
      mid.execParameter[mid_eplen + i] = pcid.referencedImplementation
                                             .configProperty[i];

      const char* pname = pcid.referencedImplementation
                              .configProperty[i].name;

      size_t idd_plen = idd.configProperty.length ();

      for (size_t j = 0; j < idd_plen; ++j)
      {
        const char* idd_pname = idd.configProperty[j].name;
        if (ACE_OS::strcmp (idd_pname, pname) == 0)
        {
          update = false;
          break;
        }
      }

      if (update)
      {
        idd.configProperty.length (idd_plen + 1);
        idd.configProperty[idd_plen] =
            pcid.referencedImplementation.configProperty[i];
      }
    }
  }

