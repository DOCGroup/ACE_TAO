/* -*- C++ -*- */

//========================================================================
/**
 *  file  PlanUpdater.h
 *
 * $Id$
 *
 *  This file contains a Visitor classes which are used to traverse a
 *  PackageConfiguration and update the location fields in a corresponding
 *  DeploymentPlan.
 *
 *  author Stoyan Paunov <spaunov@isis.vanderbilt.edu>
 */
//========================================================================

#ifndef PLAN_UPDATER_H
#define PLAN_UPDATER_H

#include /**/ "ace/pre.h"
#include "ace/SString.h"             //for the ACE_CString

#include "PCVisitorBase.h"           //for the base visitor
//#include "Node_T.h"                  //Node
#include "ace/Containers_T.h"        //for ACE_Double_Linked_List



#include "ciao/DeploymentC.h"
#include "ciao/Deployment_BaseC.h"
#include "ciao/Deployment_DataC.h"
#include "ciao/Packaging_DataC.h"

//namespace {
////Definition of various Node elements parametrized by
////Deployment specific types
//  typedef Node<Deployment::Node> NNode;
//  typedef Node<Deployment::MonolithicImplementationDescription> MNode;
//
////Definition of various stacks used to store information
////about the elements being parsed at various levels
//  typedef ACE_Bounded_Stack<int> INDEX_STACK;
//
////definition of a Node which takes an ACE_CString
//  typedef Node<ACE_CString> SNode;
//}

//===========================================================================
/**
 *  class PlanUpdater
 *
 *  This class takes in a Deployment Plan and a PackageConfiguration and
 *  attempts to update the location fields in the deployment plan to reflect
 *  the locations available via HTTP as outlined in the passed Package-
 *  Configuration. It derives from PCVisitorBase and overloads the functions
 *  which deal with single elements. Sequences are handled in the base class.
 */
//===========================================================================


class  PlanUpdater : public PCVisitorBase
{
public:

  //constructor
  PlanUpdater (Deployment::DeploymentPlan &plan,
               Deployment::PackageConfiguration &pc);

  ///Entry point to protected Visitor functions
  bool Visit ();

protected:
  /// A whole slew of overloaded routines for different IDL
  /// data types part of the PackageConfiguration.

  virtual
  void Visit (Deployment::PackageConfiguration &pc);

  //!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!

  //ComponentPackageDescription descendents
  virtual
  void Visit (Deployment::ComponentPackageDescription &cpd);

  virtual
  void Visit (Deployment::ComponentInterfaceDescription &cid);

  virtual
  void Visit (Deployment::PackagedComponentImplementation &pci);

  virtual
  void Visit (Deployment::ComponentImplementationDescription &cid);

  virtual
  void Visit (Deployment::ComponentAssemblyDescription &cad);

  virtual
  void Visit (Deployment::SubcomponentInstantiationDescription &sid);

  virtual
  void Visit (Deployment::MonolithicImplementationDescription &mid);

  virtual
  void Visit (Deployment::NamedImplementationArtifact &nia);

  virtual
  void Visit (Deployment::ImplementationArtifactDescription &iad);

  //!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!

  //ComponentPackageReference descendents
  virtual
  void Visit (Deployment::ComponentPackageReference &cpr);

  //!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!-!

  //properties
  virtual
  void Visit (Deployment::AssemblyPropertyMapping &apm);

  virtual
  void Visit (Deployment::Property &property);


  //requirements & capabilities
  virtual
  void Visit (Deployment::Requirement &requirement);

  virtual
  void Visit (Deployment::Capability &capability);

  virtual
  void Visit (Deployment::ImplementationRequirement &ir);

  virtual
  void Visit (Deployment::ImplementationDependency &id);

  //ports and connections
  virtual
  void Visit (Deployment::AssemblyConnectionDescription &acd);

  virtual
  void Visit (Deployment::SubcomponentPortEndpoint &spe);

  virtual
  void Visit (Deployment::ComponentExternalPortEndpoint &cepe);


private:

  Deployment::DeploymentPlan& plan_;
  Deployment::PackageConfiguration& pc_;
  bool status_;
};

#include /**/ "ace/post.h"

#endif /* PLAN_UPDATER_H */
