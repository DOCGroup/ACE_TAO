
/* -*- C++ -*- */

//========================================================================
/**
 *  file  PC_Updater.h
 *
 * $Id$
 *
 *  This class is used to update the location field of the implementation
 *  artifacts in the PackageConfiguration, so that they point to the
 *  physical libraries on the collocated HTTP server
 *
 *  author Stoyan Paunov <spaunov@isis.vanderbilt.edu>
 */
//========================================================================

#ifndef PC_UPDATER_H
#define PC_UPDATER_H
#include /**/ "ace/pre.h"

#include "DAnCE/Deployment/DeploymentC.h"
#include "DAnCE/Deployment/Deployment_Packaging_DataC.h"
#include "ace/SString.h"             //for the ACE_CString

#include "ZIP_Wrapper.h"             //Wrapper around zzip
#include "ace/Containers_T.h"        //for ACE_Double_Linked_List
#include "ace/Malloc_Allocator.h"    //for ACE_New_Allocator needed by the doubly link list

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
  * @class PC_Updater
  *
  * This class defines a set of overloaded methods used to update
  * the contents of a PackageConfiguration. More specifically the class
  * goes through the PackageConfiguration and updates the locations of the
  * artifacts, wrt to their location on the HTTP server.
  */
class PC_Updater
{
public:

  /// Constructors
  PC_Updater (const char* server_path, const char* package);
  PC_Updater (ACE_CString& server_path, ACE_CString& package);

  ~PC_Updater ();

  /// A whole slew of overloaded routines for different IDL
  /// data types part of the PackageConfiguration.

  bool update (::Deployment::PackageConfiguration &pc);

  void update (::Deployment::Property &property);

  void update (::Deployment::AssemblyConnectionDescription &acd);

  void update (::Deployment::AssemblyPropertyMapping &apm);

  void update (::Deployment::ComponentPackageDescription &comppkgdesc);

  void update (::Deployment::MonolithicImplementationDescription &mid);

  void update (::Deployment::PackagedComponentImplementation &pci);

  void update (::Deployment::SubcomponentPortEndpoint &spe);

  void update (::Deployment::Requirement &requirement);

  void update (::Deployment::ComponentExternalPortEndpoint &cepe);

  void update (::Deployment::ComponentPackageReference &cpr);

  void update (::Deployment::ComponentImplementationDescription &cid);

  void update (::Deployment::SubcomponentInstantiationDescription &sid);

  void update (::Deployment::NamedImplementationArtifact &named_implementation);

  void update (::Deployment::ComponentInterfaceDescription &cid);

  void update (::Deployment::Capability &capability);

  void update (::Deployment::ImplementationArtifactDescription &iad);

  void update (::Deployment::ImplementationRequirement &ir);

  void update (::Deployment::ImplementationDependency &id);

  void update (::Deployment::ComponentAssemblyDescription& cad);

protected:

  void clear_list ();

private:

  ACE_CString server_path_;

  /// Create a doubly link list
  ACE_Double_Linked_List<ZIP_File_Info> file_list_;

  ACE_CString package_;
  bool success_;
};

#include /**/ "ace/post.h"

#endif /* PC_UPDATER_H */
