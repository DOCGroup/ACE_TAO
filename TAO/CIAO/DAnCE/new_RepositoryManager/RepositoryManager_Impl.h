// $Id$

#ifndef REPOSITORYMANAGERI_H_
#define REPOSITORYMANAGERI_H_

///====================================================================
// filename: RepositoryManager_Impl.h
// Author: Stoyan Paunov	spaunov@isis.vanderbilt.edu
//
// Purpose: This class is the implementation class for the spec 
//          complient RepositoryManager
//


#include "RepositoryManagerS.h"
#include "DeploymentS.h"					//added for the deployment stuff

#include "ace/Hash_Map_Manager.h"			//for the ACE_Hash_Map_Manager
#include "ace/Null_Mutex.h"					//for ACE_Null_Mutex
#include "ace/RW_Mutex.h"					//for ACE_RW_Mutex
#include "ace/OS_NS_string.h"				//for ACE_CString
#include "ace/SString.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace
{
///DIRECTORY WHERE THE PACKAGES WILL BE STORED LOCALLY
const static char* RM_STORAGE_PATH = "RepositoryDir";

const static size_t TEMP_LEN = 512;

const static char* PC_EXTENSION = ".epc";
}

class  CIAO_RepositoryManagerDaemon_i : public virtual POA_CIAO::RepositoryManagerDaemon, public virtual PortableServer::RefCountServantBase
{
public:
  //Constructor 
  CIAO_RepositoryManagerDaemon_i (CORBA::ORB_ptr the_orb);
  
  //Destructor 
  virtual ~CIAO_RepositoryManagerDaemon_i (void);
  
  virtual
  void shutdown (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  void installPackage (
      const char * installationName,
      const char * location
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ::Deployment::NameExists,
      ::Deployment::PackageError
    ));
  
  virtual
  void createPackage (
      const char * installationName,
      const ::Deployment::PackageConfiguration & package,
      const char * baseLocation,
      ::CORBA::Boolean replace
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ::Deployment::NameExists,
      ::Deployment::PackageError
    ));
  
  virtual
  ::Deployment::PackageConfiguration * findPackageByName (
      const char * name
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ::Deployment::NoSuchName
    ));
  
  virtual
  ::Deployment::PackageConfiguration * findPackageByUUID (
      const char * UUID
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ::Deployment::NoSuchName
    ));
  
  virtual
  ::CORBA::StringSeq * findNamesByType (
      const char * type
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  ::CORBA::StringSeq * getAllNames (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  ::CORBA::StringSeq * getAllTypes (
      
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual
  void deletePackage (
      const char * installationName
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ::Deployment::NoSuchName
    ));

  protected:

	  ///function to parse and return the PackageConfiguration
	  Deployment::PackageConfiguration* retrieve_PC (char* package);

	  ///function to extract all necessary files for parsing the PackageConfiguration
	  ///descriptor and populating the idl struct.
	  ///return 1 on success
	  ///       0 on error
	  ///
	  ///NOTE: ACE_CString& pcd_name is an out parameter

	  int extract_necessary_files (char* package,  
								   ACE_CString& pcd_name);


	  ///function to remove the files extracted for parsing the PackageConfiguration
	  ///descriptor and populating the idl struct. It reads the names of the files
	  ///from the package. They correspond to the names on disk.
	  ///return 1 on success
	  ///       0 on error

	  int remove_extracted_files (char* package);


  private:
	/// Cached information about the installed PackageConfigurations
	/// A separate map for the installation names and their UUID's
    /// Key:	PackageConfiguration name or its UUID (CString type)
    /// Value:	The location of the local copy of the package

	///Based on the synchronization needed we can parametrize this with either 
	///ACE_Null_Mutex or ACE_RW_Mutex

    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    ACE_CString,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_RW_Mutex> PCMap;


	typedef PCMap::iterator PCMap_Iterator;

	//a hash map that associates the names of PackageConfigurations with their location
    PCMap names_;

	//a hash map that associates the UUIDs of PackageConfigurations with their location
	PCMap uuids_;

	CORBA::ORB_var the_orb_;

	char cwd_ [TEMP_LEN];		//will hold the current working directory

};


#endif /* REPOSITORYMANAGER_H_  */

