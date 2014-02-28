
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
 * @author Shanshan Jiang <shanshan.jiang@vanderbilt.edu>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
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

#include "RepositoryManagerDaemonS.h"

#include "ace/Hash_Map_Manager_T.h"      //for the ACE_Hash_Map_Manager
#include "ace/Hash_Multi_Map_Manager_T.h"      //for the ACE_Hash_MultiMap_Manager
#include "ace/Null_Mutex.h"          //for ACE_Null_Mutex
#include "ace/RW_Mutex.h"          //for ACE_RW_Mutex
#include "ace/OS_NS_string.h"        //for ACE_CString
#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace
{
  static const size_t TEMP_LEN = 1024;
}

namespace DAnCE
{
  class RepositoryManagerDaemon_i :
    public virtual POA_CIAO::RepositoryManagerDaemon
  {
  public:
    /// Constructor
    RepositoryManagerDaemon_i (CORBA::ORB_ptr the_orb,
                               const ACE_TCHAR* server = ACE_TEXT("localhost:5432"),
                               const ACE_TCHAR* install_dir = ACE_TEXT("RepositoryManager"));

    /// Destructor
    virtual ~RepositoryManagerDaemon_i (void);

    virtual void shutdown ();

    virtual
    void installPackage (const ACE_TCHAR * installationName,
                         const ACE_TCHAR * location,
                         ::CORBA::Boolean replace);

    virtual
    void createPackage (const ACE_TCHAR * installationName,
                        const ::Deployment::PackageConfiguration & package,
                        const ACE_TCHAR * baseLocation,
                        ::CORBA::Boolean replace);

    virtual
    ::Deployment::PackageConfiguration * findPackageByName (const ACE_TCHAR * name);

    virtual
    ::Deployment::PackageConfiguration * findPackageByUUID (const ACE_TCHAR * UUID);

    virtual
    ::CORBA::StringSeq * findNamesByType (const ACE_TCHAR * type);

    virtual
    ::CORBA::StringSeq * getAllNames ();

    virtual
    ::CORBA::StringSeq * getAllTypes ();

    virtual
    void deletePackage (const ACE_TCHAR * installationName);

  protected:

    /// Function to parse and return the PackageConfiguration from a specified
    /// package
    Deployment::PackageConfiguration* retrieve_PC_from_package (ACE_TCHAR* package);

    /// Find out what the name of the PackageConfiguration file is
    void find_PC_name (ACE_TCHAR* package, ACE_CString& pcd_name);

    /// Function to parse and return the PackageConfiguration from the already
    /// extracted descriptor files
    Deployment::PackageConfiguration* retrieve_PC_from_descriptors (const ACE_TCHAR* pc_name,
                                                                    const ACE_TCHAR* descriptor_dir);


    /// Function to retrieve a file via HTTP
    /// stores the file in the passed preallocated ACE_Message_Block
    /// @retval 1 success
    /// @retval 0 error
    int HTTP_Get (const ACE_TCHAR* URL, ACE_Message_Block &mb);

    /// Function to extract all necessary files for parsing the
    /// PackageConfiguration descriptor and populating the idl struct.
    /// @retval 1 success
    /// @retval 0 error
    ///
    /// @note ACE_CString& pcd_name is an out parameter
    int extract_descriptor_files (ACE_TCHAR* package,
                                  ACE_CString& pcd_name);


    /// Function to remove the files extracted for parsing the PackageConfiguration
    /// descriptor and populating the idl struct. It reads the names of the files
    /// from the package. They correspond to the names on disk.
    /// @retval 1 on success
    /// @retval 0 on error
    int remove_descriptor_files (ACE_TCHAR* package);


    /// Function to remove the files extracted from the package upon istallation
    /// It reads the names of the files from the package. They correspond to the
    /// names on disk. It deletes each file, then it deletes the directories that
    /// contain them.
    /// @note extraction location is path/*archive_name*/
    /// @retval 1 on success
    /// @retval 0 on error
    int remove_extracted_package (const ACE_TCHAR* package_path);

    /// Function to extract the type of the component from
    /// the PackageConfiguration and update the interface map
    /// @retval 1 on success
    /// @retval 0 on error
    int add_type (::Deployment::PackageConfiguration& pc,
                  const ACE_TCHAR* name);

    /// Function to remove the interface type of the component
    /// being removed from the interface map
    /// @retval 1 on success
    /// @retval 0 on error
    int remove_type (::Deployment::PackageConfiguration& pc,
                     const ACE_TCHAR* name);

    /// Function to dump the state of the RepositoryManager
    void dump (void);

    /// Function to save the package info of the RepositoryManager
    void save (void);

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

    typedef PCMap::ITERATOR PCMap_Iterator;
    typedef PCMap::ENTRY PCEntry;


    /// Cached information about the installed Component Interfaces
    /// A map which associates Component supportedType with the
    /// names of packages which implement this component type
    /// Key:  Component supportedType
    /// Value:  Unbounded set of the names of installed packages which
    ///      implement this component type

    ///Based on the synchronization needed we can parametrize this with either
    ///ACE_Null_Mutex or ACE_RW_Mutex

    typedef ACE_Hash_Multi_Map_Manager<ACE_CString,
                                       ACE_CString,
                                       ACE_Hash<ACE_CString>,
                                       ACE_Equal_To<ACE_CString>,
                                       ACE_RW_Mutex> CIMap;

  typedef CIMap::ITERATOR CIMap_Iterator;
  typedef CIMap::ENTRY CIEntry;
  typedef CIEntry::VALUE_SET CISet;
  typedef CIEntry::VALUE_SET_ITERATOR CISet_Iterator;

  /// A hash map that associates the names of
  /// PackageConfigurations with their location
  PCMap names_;

  /// a hash map that associates the UUIDs of
  /// PackageConfigurations with their location
  PCMap uuids_;

  /// a hash map which associates Component Interface
  /// UUIDs with their implementations
  CIMap types_;

  /// The ORB
  CORBA::ORB_var the_orb_;

  /// Will hold the current working directory
  ACE_TCHAR cwd_ [TEMP_LEN];

  /// Full path for the install directory
  ACE_CString install_root_;

  /// Location of the server
  ACE_CString HTTP_server_;

  /// Directory where the packages will be stored locally
  ACE_CString install_path;
};

}

#endif /* REPOSITORYMANAGER_H_  */
