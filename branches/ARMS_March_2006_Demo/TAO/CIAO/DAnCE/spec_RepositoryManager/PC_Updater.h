//==================================================================
/**
 *  file  PC_Updater.h
 *
 * $Id$
 *
 *  author Stoyan Paunov <spaunov@isis.vanderbilt.edu>
 */
//=====================================================================

#ifndef PC_UPDATER_H
#define PC_UPDATER_H
#include /**/ "ace/pre.h"

#include "ciao/DeploymentC.h"
#include "ace/SString.h"					//for the ACE_CString

#include "ZIP_Wrapper.h"					//Wrapper around zzip
#include "ace/Containers_T.h"				//for ACE_Double_Linked_List
#include "ace/Malloc_Allocator.h"			//for ACE_New_Allocator needed by the doubly link list


#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


  /**
   * class PC_Updater
   *
   * This class defines a set of overloaded methods used to update
   * the contents of a PackageConfiguration. More specifically the class
   * goes through the PackageConfiguration and updates the locations of the
   * artifacts, wrt to their location on the HTTP server.
   */

class  PC_Updater
{
public:

	///constructors 

	PC_Updater (const char* server_path, const char* package);
	PC_Updater (ACE_CString& server_path, ACE_CString& package);

	~PC_Updater ();

    /// A whole slew of overloaded routines for different IDL
    /// data types part of the PackageConfiguration.

	bool update (const ::Deployment::PackageConfiguration &pc);

    void update (const ::Deployment::Property &property);

    void update (const ::Deployment::AssemblyConnectionDescription &acd);

    void update (const ::Deployment::AssemblyPropertyMapping &apm);

    void update (const ::Deployment::ComponentPackageDescription &comppkgdesc);

    void update (const ::Deployment::MonolithicImplementationDescription &mid);

    void update (const ::Deployment::PackagedComponentImplementation &pci);

    void update (const ::Deployment::SubcomponentPortEndpoint &spe);

    void update (const ::Deployment::Requirement &requirement);

    void update (const ::Deployment::ComponentExternalPortEndpoint &cepe);

    void update (const ::Deployment::ComponentPackageReference &cpr);

    void update (const ::Deployment::ComponentImplementationDescription &cid);

    void update (const ::Deployment::SubcomponentInstantiationDescription &sid);

    void update (const ::Deployment::NamedImplementationArtifact &named_implementation);

    void update (const ::Deployment::ComponentInterfaceDescription &cid);

    void update (const ::Deployment::Capability &capability);

    void update (const ::Deployment::ImplementationArtifactDescription &iad);

    void update (const ::Deployment::ImplementationRequirement &ir);

    void update(const Deployment::ImplementationDependency &id);

    void update (const ::Deployment::ComponentAssemblyDescription& cad);

protected:

	void clear_list ();

private:

	ACE_CString server_path_;

	//create a doubly link list
	ACE_New_Allocator allocator_;
	ACE_Double_Linked_List<ZIP_File_Info> file_list_;

	ACE_CString package_;
	bool success_;
};

#include /**/ "ace/post.h"

#endif /* PC_UPDATER_H */
