
/* -*- C++ -*- */

//======================================================================
/**
 * @file RepositoryManager_Impl.h
 *
 * $Id$
 *
 * Description:
 *  This file is the main implementation file for the RepositoryManager
 *  in CIAO. We have used a number of techniques in order to increase
 *  scalability of the RepoMan while still maintaining complience with
 *  the D&C spec
 *
 * @author Stoyan Paunov
 */
//======================================================================

#ifndef REPOSITORYMANAGERI_H_
#define REPOSITORYMANAGERI_H_


//-----------------------------NOTE---------------------------------
//I need to disable all the code which has to do with interface
//type information because we currently do not support assembly
//interfaces which causes undesired behavior with respect to the
//hash tables because the specificType field in assembly interfaces
//is empty, so two unrelated intefaces appear to be related.

//uncomment this line to turn on the code that relates to interface types
//#define ASSEMBLY_INTERFACE_SUPPORT 1


#include "RepositoryManagerDaemonS.h"

#include "ace/Hash_Map_Manager.h"      //for the ACE_Hash_Map_Manager
#include "ace/Null_Mutex.h"          //for ACE_Null_Mutex
#include "ace/RW_Mutex.h"          //for ACE_RW_Mutex
#include "ace/OS_NS_string.h"        //for ACE_CString
#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class  CIAO_RepositoryManagerDaemon_i :
  public virtual POA_CIAO::RepositoryManagerDaemon
{
public:
  /// Constructor
  CIAO_RepositoryManagerDaemon_i (CORBA::ORB_ptr the_orb,
                  const char* server = "localhost:5432",
                  char* install_dir = "RepositoryManager");

  /// Destructor
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

    /// Function to parse and return the PackageConfiguration from a specified
    /// package
    Deployment::PackageConfiguration* retrieve_PC_from_package (char* package);

    /// Find out what the name of the PackageConfiguration file is
    void find_PC_name (char* package, ACE_CString& pcd_name);

    /// Function to parse and return the PackageConfiguration from the already
    /// extracted descriptor files
    Deployment::PackageConfiguration* retrieve_PC_from_descriptors (const char* pc_name,
                                    const char* descriptor_dir);


    /// Function to retrieve a file via HTTP
    /// stores the file in the passed preallocated ACE_Message_Block
    /// @retval 1 success
    /// @retval 0 error

    int HTTP_Get (const char* URL, ACE_Message_Block &mb);

    /// Function to extract all necessary files for parsing the
    /// PackageConfiguration descriptor and populating the idl struct.
    /// @retval 1 success
    /// @retval 0 error
    ///
    /// @note ACE_CString& pcd_name is an out parameter

    int extract_descriptor_files (char* package,
                                  ACE_CString& pcd_name);


    ///function to remove the files extracted for parsing the PackageConfiguration
    ///descriptor and populating the idl struct. It reads the names of the files
    ///from the package. They correspond to the names on disk.
    ///return 1 on success
    ///       0 on error

    int remove_descriptor_files (char* package);


    ///function to remove the files extracted from the package upon istallation
    ///It reads the names of the files from the package. They correspond to the
    ///names on disk. It deletes each file, then it deletes the directories that
    ///contain them.
    ///NOTE: extraction location is path/*archive_name*/
    ///returns 1 on success
    ///        0 on error

    int remove_extracted_package (const char* package_path, const char* extraction_location);

#if defined ASSEMBLY_INTERFACE_SUPPORT
    ///function to extract the type of the component from
    ///the PackageConfiguration and update the interface map
    ///returns 1 on success
    ///        0 on error

    int add_type (::Deployment::PackageConfiguration& pc,
          const char* name);

      ///function to remove the interface type of the component
    ///being removed from the interface map
    ///returns 1 on success
    ///        0 on error

    int remove_type (::Deployment::PackageConfiguration& pc,
             const char* name);

#endif

    ///function to dump the state of the RepositoryManager
    void dump (void);

  private:
  /// Cached information about the installed PackageConfigurations
  /// A separate map for the installation names and their UUID's
    /// Key:  PackageConfiguration name or its UUID (CString type)
    /// Value:  The location of the local copy of the package

  ///Based on the synchronization needed we can parametrize this with either
  ///ACE_Null_Mutex or ACE_RW_Mutex

    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    ACE_CString,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_RW_Mutex> PCMap;


  typedef PCMap::iterator PCMap_Iterator;
  typedef ACE_Hash_Map_Entry <ACE_CString,ACE_CString> PCEntry;


  /// Cached information about the installed Component Interfaces
  /// A map which associates Component Interface UUIDs with the
  /// names of packages which implement this component type
  /// Key:  Component Interface UUID
  /// Value:  linked list of the names of installed packages which
  ///      implement this component type

  ///Based on the synchronization needed we can parametrize this with either
  ///ACE_Null_Mutex or ACE_RW_Mutex

    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    ACE_CString,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_RW_Mutex> CIMap;


  typedef CIMap::iterator CIMap_Iterator;
  typedef ACE_Hash_Map_Entry <ACE_CString,ACE_CString> CIEntry;
  typedef ACE_Hash_Map_Bucket_Iterator<ACE_CString,
                                       ACE_CString,
                                       ACE_Hash<ACE_CString>,
                                       ACE_Equal_To<ACE_CString>,
                                       ACE_RW_Mutex> CIBucket_Iterator;

  //a hash map that associates the names of
  //PackageConfigurations with their location
  PCMap names_;

  /// a hash map that associates the UUIDs of
  /// PackageConfigurations with their location
  PCMap uuids_;
  
  static  const  size_t TEMP_LEN = 1024;
  static  const  char* PC_EXTENSION;
  static  const  char *RM_RECORD_FILE;
  static  const  char *RM_RECORD_NAME_SECTION;
  static  const  char *RM_RECORD_UUID_SECTION;
 
#if defined ASSEMBLY_INTERFACE_SUPPORT
  static const char *RM_RECORD_TYPE_SECTION;
  //a hash map which associates Component Interface
  //UUIDs with their implementations
  CIMap types_;
#endif

  //the ORB
  CORBA::ORB_var the_orb_;

  char cwd_ [TEMP_LEN];      //will hold the current working directory
  ACE_CString install_root_;    //full path for the install directory
  ACE_CString HTTP_server_;    //location of the server
  ACE_CString install_path; //directory where the packages will be stored locally
};

#endif /* REPOSITORYMANAGER_H_  */
