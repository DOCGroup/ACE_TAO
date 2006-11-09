// $Id$

//====================================================================
/**
 * @file RepositoryManager_Impl.cpp
 *
 * $Id$
 *
 * Description: Actial implementation of the RepoMan
 *
 * @author Stoyan Paunov
 *         Shanshan Jiang <shanshan.jiang@vanderbilt.edu>
 */
//====================================================================

#include "RepositoryManager_Impl.h"

#include "ace/OS_NS_fcntl.h"       //for open
#include "ace/OS_NS_unistd.h"      //for close
#include "ace/OS_NS_sys_stat.h"    //for filesize and fstat and mkdir
#include "ace/OS_NS_string.h"      //for ACE_CString
#include "ace/OS_Memory.h"         //for ACE_NEW* macros


//to remove a file or dir from the local filesystem need remove () from stdio.h
// ---> need to include ace/OS_NS_stdio.h which would include the correct file for any OS!
#include "ace/OS_NS_stdio.h"

#include "ZIP_Wrapper.h"                //Wrapper around zzip
#include "ace/Containers_T.h"           //for ACE_Double_Linked_List
#include "ace/Malloc_Allocator.h"       //for ACE_New_Allocator

//for the PackageConfiguration parsing
#include "ciao/Deployment_DataC.h"
#include "ciao/Packaging_DataC.h"
#include "Package_Handlers/PCD_Handler.h"

#include "RM_Helper.h"            //to be able to externalize/internalize a PackageConfiguration
#include "ace/Message_Block.h"    //for ACE_Message_Block

#include "ace/Thread.h"           //for obtaining the ID of the current thread
#include "ace/OS_NS_stdlib.h"     //for itoa ()

#include "URL_Parser.h"           //for parsing the URL
#include "HTTP_Client.h"          //the HTTP client class to downloading packages

#include "PC_Updater.h"           //A visitor class to walk through the elements of the PC

#include "ace/Configuration_Import_Export.h"

#include <iostream>
using namespace std;

namespace
{
  static const char* PC_EXTENSION = ".epc";

  static const char *RM_RECORD_FILE = "RM_record";
  static const char *RM_RECORD_NAME_SECTION = "Names";
  static const char *RM_RECORD_UUID_SECTION = "UUIDs";
}

//-----------------------------------------------------------------
//Constructor
//
//-----------------------------------------------------------------

CIAO_RepositoryManagerDaemon_i::CIAO_RepositoryManagerDaemon_i
  (CORBA::ORB_ptr the_orb, const char* server, char* install_dir)
  : the_orb_ (CORBA::ORB::_duplicate (the_orb)),
    install_root_ (""),
    HTTP_server_ ("http://"),
    install_path (install_dir)
{
  //form the path
  this->HTTP_server_ += server;
  this->HTTP_server_ += "/";
  this->HTTP_server_ += install_path;
  this->HTTP_server_ += "/";

  //create directory in which the packages will be stored
  ACE_OS::mkdir(install_path.c_str ());
  //if dir already exists a -1 is returned
  //we ignore this, just need to make sure the directory exists

  ACE_OS::getcwd (this->cwd_, TEMP_LEN);

  this->install_root_ = this->cwd_;
  this->install_root_ += "/";
  this->install_root_ += install_path;

  // Install the configuration files to get the names, UUIDs, & types info.
  ACE_Configuration_Heap cfg;
  cfg.open ();
  ACE_Configuration_Section_Key root = cfg.root_section ();

  ACE_Registry_ImpExp config_importer (cfg);
  ACE_OS::chdir (install_path.c_str ());
  config_importer.import_config (RM_RECORD_FILE);
  ACE_OS::chdir (this->cwd_);

  ACE_CString name;
  ACE_Configuration::VALUETYPE type;
  ACE_CString path;

  ACE_Configuration_Section_Key NameSection;
  cfg.open_section (root, RM_RECORD_NAME_SECTION, 1, NameSection);
  u_int index = 0;
  while (!cfg.enumerate_values (NameSection, index, name, type))
  {
    cfg.get_string_value (NameSection, name.c_str (), path);
    this->names_.bind (name, path);

    ++index;
  }

  ACE_Configuration_Section_Key UUIDSection;
  cfg.open_section (root, RM_RECORD_UUID_SECTION, 1, UUIDSection);
  index = 0;
  while (!cfg.enumerate_values (UUIDSection, index, name, type))
  {
    cfg.get_string_value (UUIDSection, name.c_str (), path);
    this->uuids_.bind (name, path);

    ++index;
  }

  // Add types
  index = 0;
  for (PCMap_Iterator iter = this->names_.begin ();
    iter != this->names_.end ();
    ++iter, ++index)
  {
    PCEntry& element = *iter;

    ::Deployment::PackageConfiguration_var pc = this->findPackageByName (element.ext_id_.c_str ());

    if(!this->add_type (pc, element.ext_id_.c_str ()))
      ACE_ERROR ((LM_ERROR, "Failed to add the type\n"));
  }
}

//-----------------------------------------------------------------
//Destructor
//
//-----------------------------------------------------------------

CIAO_RepositoryManagerDaemon_i::~CIAO_RepositoryManagerDaemon_i (void)
{
  this->names_.unbind_all ();
  this->uuids_.unbind_all ();
  this->types_.unbind_all ();
}

//-----------------------------------------------------------------
//shutdown
//
//-----------------------------------------------------------------

void CIAO_RepositoryManagerDaemon_i::shutdown ()

                ACE_THROW_SPEC ((
                                CORBA::SystemException
                                ))
{
  // Save the names, UUIDs, & types info to the configuration files.
  ACE_Configuration_Heap cfg;
  cfg.open ();
  ACE_Configuration_Section_Key root = cfg.root_section ();

  ACE_Configuration_Section_Key NameSection;
  cfg.open_section (root, RM_RECORD_NAME_SECTION, 1, NameSection);
  for (PCMap_Iterator iter = this->names_.begin ();
    iter != this->names_.end ();
    ++iter)
  {
    PCEntry& element = *iter;
    cfg.set_string_value (NameSection, element.ext_id_.c_str (), element.int_id_.c_str ());
  }

  ACE_Configuration_Section_Key UUIDSection;
  cfg.open_section (root, RM_RECORD_UUID_SECTION, 1, UUIDSection);
  for (PCMap_Iterator iter = this->uuids_.begin ();
    iter != this->uuids_.end ();
    ++iter)
  {
    PCEntry& element = *iter;
    cfg.set_string_value (UUIDSection, element.ext_id_.c_str (), element.int_id_.c_str ());
  }

  ACE_Registry_ImpExp exporter (cfg);
  ACE_OS::chdir (install_path.c_str ());
  exporter.export_config (RM_RECORD_FILE);
  ACE_OS::chdir (this->cwd_);

  // Release resource.
  this->names_.unbind_all ();
  this->uuids_.unbind_all ();
  this->types_.unbind_all ();

  this->the_orb_->shutdown (0);
}


//-----------------------------------------------------------------
//installPackage
//
//-----------------------------------------------------------------

void CIAO_RepositoryManagerDaemon_i::installPackage (
                                                     const char * installationName,
                                                     const char * location,
                                                     ::CORBA::Boolean replace
                                                     )
                     ACE_THROW_SPEC ((
                                      CORBA::SystemException,
                                      ::Deployment::NameExists,
                                      ::Deployment::PackageError
                                     ))
{

  PCEntry *entry = 0;
  if (this->names_.find (ACE_CString (installationName), entry) == 0)
    {
      if (!replace)
        ACE_THROW (Deployment::NameExists ());
      else
        deletePackage (installationName);
    }

  //Now lets form the path for the local file
  //NOTE: I need the absolute path because I will change to a subdirectory
  //when I am parsing the descriptors

  ACE_CString path (this->install_root_);
  path += "/";
  path += installationName;

  ACE_CString package_path (path);
  package_path += ".cpk";        //package extension

  ACE_CString pc_path (path);
  pc_path += PC_EXTENSION;      //external PackageConfiguration extension


  ACE_CString descriptor_dir (path);
  descriptor_dir += "/descriptors/"; //location of the descriptor directory


  //check if URL or local file
  //download or load into memory

  if (ACE_OS::strstr (location, "http://"))
    {

      //TODO: how can I incorporate a Auto_Ptr is explicit release is needed
      ACE_Message_Block* mb;
      ACE_NEW_THROW_EX (mb, ACE_Message_Block (), CORBA::INTERNAL ());
      ACE_CHECK_RETURN (0);

      //get the remote file
      if (!HTTP_Get (location, *mb))
        {
          mb->release ();
          ACE_THROW (CORBA::INTERNAL ());
        }

      // Write file to designated location on disk
      if (!RM_Helper::write_to_disk (package_path.c_str (), *mb))
        {
          mb->release ();
          ACE_THROW (CORBA::INTERNAL ());
        }

      mb->release ();
    }
  else
    {
      if (!RM_Helper::copy_from_disk_to_disk (location, package_path.c_str ()))
        ACE_THROW (CORBA::INTERNAL ());
    }


  ZIP_Wrapper::uncompress (const_cast<char*> (package_path.c_str ()),
                           const_cast<char*> (this->install_root_.c_str ()),
                           false //not verbose
                           );

  //Start the parsing

  ACE_CString pc_name;

  this->find_PC_name (const_cast<char*> (package_path.c_str ()), pc_name);

  //if the PackageConfiguration name cannot be found, then there is nothing to install
  if (pc_name == "")
  {
    //clean the extracted files
    remove_extracted_package (package_path.c_str (), path.c_str ());
    //remove the package
    remove (package_path.c_str ());

    ACE_THROW (Deployment::PackageError ());
  }

  //TODO: move exception throwing out of this func. User boolean error handling!!!
  //TODO: check for errors!
  Deployment::PackageConfiguration_var pc;
  pc = this->retrieve_PC_from_descriptors (const_cast<char*> (pc_name.c_str ()),
                                           descriptor_dir.c_str ());


  if (this->uuids_.find (ACE_CString (pc->UUID), entry) == 0)
  {
    //clean the extracted files
    remove_extracted_package (package_path.c_str (), path.c_str ());
    //remove the package
    remove (package_path.c_str ());

    ACE_THROW (Deployment::NameExists ());
  }

  //forming the server path info
  ACE_CString server_path (this->HTTP_server_);
  server_path += installationName;

  //NOTE: ComponentPackageReferences are currently NOT supported
  if (!(pc->basePackage.length () > 0))
  {
    //clean the extracted files
    remove_extracted_package (package_path.c_str (), path.c_str ());
    //remove the package
    remove (package_path.c_str ());

    ACE_THROW (CORBA::NO_IMPLEMENT ());
  }

  PC_Updater updater (server_path, package_path);

  if (!updater.update (pc))
  {
    ACE_DEBUG ((LM_ERROR, "[RM] problem updating the PackageConfiguration!\n"));

    //clean the extracted files
    remove_extracted_package (package_path.c_str (), path.c_str ());
    //remove the package
    remove (package_path.c_str ());
    ACE_THROW (Deployment::PackageError ());
  }


  //now lets externalize the PackageConfiguration, so that we can access it later on
  //without having to do the whole parsing again.
  //NOTE: Order here is important. Do not populate maps before the externalization!
  RM_Helper::externalize (pc, pc_path.c_str ());

  //insert the package into the database
  if (this->names_.bind (ACE_CString (installationName), path) == -1)
  {
    ACE_ERROR ((LM_ERROR,
                 "[RM] could not bind %s.\n",
                 installationName));

    //clean the extracted files
    remove_extracted_package (package_path.c_str (), path.c_str ());
    //remove the package
    remove (package_path.c_str ());
    //remove the PackageConfiguration externalization
    remove (pc_path.c_str ());

    //throw exception
    ACE_THROW (CORBA::INTERNAL ());
  }

  //ALSO NEED THE UUID here
  if (this->uuids_.bind (ACE_CString (pc->UUID), path) == -1)
  {
     ACE_ERROR ((LM_ERROR,
                 "[RM] could not bind %s.\n",
                 ACE_CString (pc->UUID).c_str ()));

     //unbind the name
     this->names_.unbind (installationName);

     //clean the extracted files
     remove_extracted_package (package_path.c_str (), path.c_str ());
     //remove the package
     remove (package_path.c_str ());
     //remove the PackageConfiguration externalization
     remove (pc_path.c_str ());

     //throw exception
     ACE_THROW (CORBA::INTERNAL ());
  }

  //now add the type interface
  if(!this->add_type (pc, installationName))
    ACE_ERROR ((LM_ERROR, "Failed to add the type\n"));

  this->dump ();

  ACE_DEBUG ((LM_INFO,
              "Installed PackageConfiguration \n\tname: %s \n\tuuid: %s\n",
              installationName, ACE_CString (pc->UUID).c_str ()));
}


//-----------------------------------------------------------------
//createPackage
//
//-----------------------------------------------------------------

void CIAO_RepositoryManagerDaemon_i::createPackage (
                                                    const char * installationName,
                                                    const ::Deployment::PackageConfiguration & package,
                                                    const char * baseLocation,
                                                    ::CORBA::Boolean replace
                                                    )
                   ACE_THROW_SPEC ((
                                    CORBA::SystemException,
                                    ::Deployment::NameExists,
                                    ::Deployment::PackageError
                                   ))
{
  ::Deployment::PackageConfiguration pc = package;

  // Find if there is a PackageConfiguration with the same name.
  PCEntry *entry = 0;
  if (this->names_.find (ACE_CString (installationName), entry) == 0)
    {
      if (!replace)
        ACE_THROW (Deployment::NameExists ());
      else
        deletePackage (installationName);
    }

  // Find if there is a PackageConfiguration with the same uuid.
  if (this->uuids_.find (ACE_CString (pc.UUID), entry) == 0)
    ACE_THROW (Deployment::NameExists ());

  // Find if the PackageConfiguration has a basePackage.
  // NOTE: ComponentPackageReferences are currently NOT supported.
  if (!(pc.basePackage.length () > 0))
    ACE_THROW (CORBA::NO_IMPLEMENT ());

  // Form the path for the local file
  ACE_CString path (this->install_root_);
  path += "/";
  path += installationName;

  ACE_CString package_path (path);
  package_path += ".cpk";        //package extension

  ACE_CString pc_path (path);
  pc_path += PC_EXTENSION;      //external PackageConfiguration extension

  // Check if URL or local file, download or load into memory
  if (ACE_OS::strstr (baseLocation, "http://"))
  {
    //TODO: how can I incorporate a Auto_Ptr is explicit release is needed
    ACE_Message_Block* mb;
    ACE_NEW_THROW_EX (mb, ACE_Message_Block (), CORBA::INTERNAL ());
    ACE_CHECK_RETURN (0);

    //get the remote file
    if (!HTTP_Get (baseLocation, *mb))
    {
      mb->release ();
      ACE_THROW (CORBA::INTERNAL ());
    }

    // Write file to designated location on disk
    if (!RM_Helper::write_to_disk (package_path.c_str (), *mb))
    {
      mb->release ();
      ACE_THROW (CORBA::INTERNAL ());
    }

    mb->release ();
  }
  else
  {
    if (!RM_Helper::copy_from_disk_to_disk (baseLocation, package_path.c_str ()))
      ACE_THROW (CORBA::INTERNAL ());
  }


  ZIP_Wrapper::uncompress (const_cast<char*> (package_path.c_str ()),
    const_cast<char*> (this->install_root_.c_str ()),
    false //not verbose
    );

  // Form the server path info
  ACE_CString server_path (this->HTTP_server_);
  server_path += installationName;

  // Update the newly installed package configration informantion.
  PC_Updater updater (server_path, package_path);

  if (!updater.update (pc))
  {
    ACE_ERROR ((LM_ERROR, "[RM] problem updating the PackageConfiguration!\n"));
    //clean the extracted files
    remove_extracted_package (package_path.c_str (), path.c_str ());
    //remove the package
    remove (package_path.c_str ());
    ACE_THROW (Deployment::PackageError ());
  }

  // Externalize the PackageConfiguration, so that we can access it later on
  // without having to do the whole parsing again.
  // NOTE: Order here is important. Do not populate maps before the externalization!
  RM_Helper::externalize (pc, pc_path.c_str ());

  // Insert the name of the package.
  if (this->names_.bind (ACE_CString (installationName), path) == -1)
  {
    ACE_ERROR ((LM_ERROR,
                 "[RM] could not bind %s.\n",
                 installationName));

    //clean the extracted files
    remove_extracted_package (package_path.c_str (), path.c_str ());
    //remove the package
    remove (package_path.c_str ());
    //remove the PackageConfiguration externalization
    remove (pc_path.c_str ());

    //throw exception
    ACE_THROW (CORBA::INTERNAL ());
  }

  // Insert the UUID of the package.
  if (this->uuids_.bind (ACE_CString (pc.UUID), path) == -1)
  {
     ACE_ERROR ((LM_ERROR,
                 "[RM] could not bind %s.\n",
                 ACE_CString (pc.UUID).c_str ()));

     //unbind the name
     this->names_.unbind (installationName);

     //clean the extracted files
     remove_extracted_package (package_path.c_str (), path.c_str ());
     //remove the package
     remove (package_path.c_str ());
     //remove the PackageConfiguration externalization
     remove (pc_path.c_str ());

     //throw exception
     ACE_THROW (CORBA::INTERNAL ());
  }

  //now add the type interface
  //TODO: CHECK if successful
  if(!this->add_type (pc, installationName))
    ACE_ERROR ((LM_ERROR, "Failed to add the type\n"));

  this->dump ();

  ACE_DEBUG ((LM_INFO,
              "Created PackageConfiguration \n  directory: %s \n  name: %s \n  uuid: %s\n",
              path.c_str (), installationName, ACE_CString (pc.UUID).c_str ()));
}


//-----------------------------------------------------------------
//findPackageByName
//
//-----------------------------------------------------------------

::Deployment::PackageConfiguration*
CIAO_RepositoryManagerDaemon_i::findPackageByName (const char * name)

                               ACE_THROW_SPEC ((
                                                CORBA::SystemException,
                                                ::Deployment::NoSuchName
                                               ))
{
  // Find out if the PackageConfiguration was installed in the repository,
  // return it if found or throw and exception otherwise

  PCEntry *entry = 0;

  if (this->names_.find (ACE_CString (name), entry) != 0)
    ACE_THROW (Deployment::NoSuchName ());
  //PackageConfiguration was not found

  ACE_CString pc_path (entry->int_id_.c_str ());
  pc_path += PC_EXTENSION;

  Deployment::PackageConfiguration_var pc;
  ACE_NEW_THROW_EX (pc,
                    Deployment::PackageConfiguration (),
                    CORBA::INTERNAL ());

  ACE_CHECK_RETURN (0);

  if(!RM_Helper::reincarnate (pc, pc_path.c_str ()))
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  ACE_DEBUG ((LM_INFO, "Successfully looked up \'%s\'.\n", name));

  return pc._retn ();
}


//-----------------------------------------------------------------
//findPackageByUUID
//
//-----------------------------------------------------------------

::Deployment::PackageConfiguration*
CIAO_RepositoryManagerDaemon_i::findPackageByUUID (const char * UUID)

                              ACE_THROW_SPEC ((
                                              CORBA::SystemException,
                                              ::Deployment::NoSuchName
                                              ))
{
  // Find out if the PackageConfiguration was installed in the repository,
  // return it if found or throw and exception otherwise

  PCEntry *entry = 0;

  if (this->uuids_.find (ACE_CString (UUID), entry) != 0)
    ACE_THROW (Deployment::NoSuchName ());
  //PackageConfiguration was not found

  ACE_CString pc_path (entry->int_id_.c_str ());
  pc_path += PC_EXTENSION;

  Deployment::PackageConfiguration_var pc;
  ACE_NEW_THROW_EX (pc,
                    Deployment::PackageConfiguration (),
                    CORBA::INTERNAL ());

  ACE_CHECK_RETURN (0);

  if(!RM_Helper::reincarnate (pc, pc_path.c_str ()))
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  ACE_DEBUG ((LM_INFO, "Successfully looked up %s.\n", UUID));

  return pc._retn ();
}

//-----------------------------------------------------------------
//findPackageByType
//
//-----------------------------------------------------------------

::CORBA::StringSeq * CIAO_RepositoryManagerDaemon_i::findNamesByType (
                                                                      const char * type
                                                                      )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CIEntry *entry = 0;

    //find the type in the interface map
  if (this->types_.find (ACE_CString (type), entry) != 0)
  {
    //return an empty sequence
    CORBA::StringSeq_var seq;
    ACE_NEW_THROW_EX (seq, CORBA::StringSeq (0), CORBA::INTERNAL ());
    ACE_CHECK_RETURN (0);

    return seq._retn ();
  }
  else
  {
    CISet ci_set = (*entry).int_id_set_;

    CORBA::ULong len = ci_set.size ();

    //allocate a sequence of the right length
    CORBA::StringSeq_var seq;
    ACE_NEW_THROW_EX (seq,
                      CORBA::StringSeq (len),
                      CORBA::INTERNAL ());
    seq->length (len);

    //store the elements in the string sequence
    CISet_Iterator ci_set_iter (ci_set);
    CORBA::ULong index = 0;
    for (ci_set_iter = ci_set.begin ();
         ci_set_iter != ci_set.end () && index < len;
         ++ci_set_iter, ++index)
    {
      seq[index] = CORBA::string_dup ((*ci_set_iter).c_str ());
    }

    return seq._retn ();
  }
}


//-----------------------------------------------------------------
//getAllNames
//
//-----------------------------------------------------------------

::CORBA::StringSeq*
CIAO_RepositoryManagerDaemon_i::getAllNames ()

                   ACE_THROW_SPEC ((
                                   CORBA::SystemException
                                   ))
{
  //Map.current_size () gives you the current number with the duplicates
  //Map.total_size () gives you the allocated space + the empty slots
  //Apparently the only way to figure out the number of keys is to
  //count them with an iterator.

  CORBA::ULong num_entries = 0;

  for (PCMap_Iterator i = this->names_.begin ();
       i != this->names_.end ();
       ++i)
     ++num_entries;

  CORBA::StringSeq_var seq;
  ACE_NEW_THROW_EX (seq, CORBA::StringSeq (num_entries), CORBA::INTERNAL ());

  ACE_CHECK_RETURN (0);

  seq->length (num_entries);

  CORBA::ULong index = 0;
  for (PCMap_Iterator iter = this->names_.begin ();
    iter != this->names_.end () && index < num_entries;
    ++iter, ++index)
  {
    PCEntry& element = *iter;
    seq[index] = CORBA::string_dup (element.ext_id_.c_str ());
  }

  ACE_DEBUG ((LM_INFO, "The number of packages %d\n", seq->length ()));

  return seq._retn ();    //release the underlying CORBA::StringSeq
}


//-----------------------------------------------------------------
//getAllTypes
//
//-----------------------------------------------------------------

::CORBA::StringSeq * CIAO_RepositoryManagerDaemon_i::getAllTypes (

  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  //Map.current_size () gives you the current number with the duplicates
  //Map.total_size () gives you the allocated space + the empty slots
  //Apparently the only way to figure out the number of keys is to
  //count them with an iterator.

  CORBA::ULong num_entries = 0;

  for (CIMap_Iterator i = this->types_.begin ();
       i != this->types_.end ();
       ++i)
     ++num_entries;

  CORBA::StringSeq_var seq;
  ACE_NEW_THROW_EX (seq,
                    CORBA::StringSeq (num_entries),
                    CORBA::INTERNAL ());

  ACE_CHECK_RETURN (0);

  seq->length (num_entries);

  CORBA::ULong index = 0;
  for (CIMap_Iterator iter = this->types_.begin ();
    iter != this->types_.end () && index < num_entries;
    ++iter, ++index)

  {
    CIEntry& element = *iter;
    seq[index] = CORBA::string_dup (element.ext_id_.c_str ());
  }

  ACE_DEBUG ((LM_DEBUG, "The number of types: %d\n", num_entries));

  return seq._retn ();    //release the underlying CORBA::StringSeq
}


//-----------------------------------------------------------------
//DeletePackage
//
//-----------------------------------------------------------------

void CIAO_RepositoryManagerDaemon_i::deletePackage (
                                                    const char * installationName
                                                    )
                     ACE_THROW_SPEC ((
                                      CORBA::SystemException,
                                      ::Deployment::NoSuchName
                                     ))
{
  bool internal_err = false;

  PCEntry *entry = 0;

  if (this->names_.find (ACE_CString (installationName), entry) != 0)
    ACE_THROW (Deployment::NoSuchName ());

  //cache the package path
  ACE_CString path (entry->int_id_.c_str ());

  //remove the name association
  if (this->names_.unbind (installationName) == -1)
  {
    ACE_ERROR ((LM_ERROR,
                "Unable to unbind %s.\n",
                installationName));
    internal_err = true;
  }

  //the package location
  ACE_CString package_path (path);
  package_path += ".cpk";        //package extension

  //the PackageConfiguration externalization location
  ACE_CString pc_path (path);
  pc_path += PC_EXTENSION;        //external PackageConfiguration extension

  Deployment::PackageConfiguration_var pc;
  ACE_NEW_THROW_EX (pc,
                    Deployment::PackageConfiguration (),
                    CORBA::INTERNAL ());

  ACE_CHECK_RETURN (0);

  if(!RM_Helper::reincarnate (pc, pc_path.c_str ()))
  {
    ACE_ERROR ((LM_ERROR, "Could not reincarnate PC\n"));
      internal_err = true;
  }

  if (this->uuids_.unbind (ACE_CString (pc->UUID)) == -1)
  {
    ACE_ERROR ((LM_ERROR, "Could not remove UUID\n"));
    internal_err = true;
  }

  //remove the type from the interface map
  if (!this->remove_type (pc, installationName))
  {
    ACE_ERROR ((LM_ERROR, "Could not remove type\n"));
    internal_err = true;
  }

  //actually delete the package here!

  //clean the extracted files
  remove_extracted_package (package_path.c_str (), path.c_str ());
  //remove the package
  remove (package_path.c_str ());
  //remove the PackageConfiguration externalization
  remove (pc_path.c_str ());

  this->dump ();

  if (internal_err)
    ACE_THROW (CORBA::INTERNAL ());
  else
    ACE_DEBUG ((LM_INFO, "Successfully deleted \'%s\'\n", installationName));

}


//==========================================HELPER METHODS==================================================

Deployment::PackageConfiguration*
CIAO_RepositoryManagerDaemon_i::retrieve_PC_from_package (char* package)
{
  char temp[128];
  //  ACE_thread_t thread_id = ACE_Thread::self ();
  char* PID = ACE_OS::itoa (ACE_OS::getpid (), temp, 10);

  ACE_OS::mkdir(PID);
  //if dir already exists a -1 is returned
  //we ignore this, just need to make sure the directory exists

  //change the working dir
  ACE_OS::chdir (PID);

  ACE_CString pcd_name;
  //extract the necessary descriptors
  if (extract_descriptor_files (package,
                                pcd_name) < 0)
    {
      ACE_OS::chdir (this->cwd_);
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) RepositoryManager: error extracting necessary files\n"));
      ACE_THROW (CORBA::INTERNAL ());
    }

  Deployment::PackageConfiguration_var pc;
  //parse the PCD to make sure that there are no package errors
  ACE_TRY
    {
      //CIAO::Config_Handlers::STD_PC_Intf intf (pcd_name.c_str ());
      //pc = intf.get_PC ();
    }
  ACE_CATCHALL
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) RepositoryManager: Error parsing the PCD\n"));

      //change back the the old working dir
      ACE_OS::chdir (this->cwd_);
      ACE_THROW (Deployment::PackageError ());
    }
  ACE_ENDTRY;
  //able to parse the PC. So lets install the package in the repo

  //we no longer need the descriptors, so lets erase them!
  remove_descriptor_files (package);

  //change back the the old working dir
  ACE_OS::chdir (this->cwd_);

  //now lets erase the directory!
  ACE_OS::rmdir (PID);
  //the ACE_OS::rmdir does not work. Possibly because we need to delete
  //the contents first. I will look into it more closely when I am back.

  return pc._retn ();
}


//function to retvieve a file via HTTP
//stores the file in the passed preallocated ACE_Message_Block
//returns 1 on success
//      0 on error

int CIAO_RepositoryManagerDaemon_i::HTTP_Get (const char* URL, ACE_Message_Block &mb)
{
    URL_Parser *parser = TheURL_Parser::instance ();
    if (!parser->parseURL (const_cast<char*> (URL)))
    return 0;

    // Create a client
    HTTP_Client client;

    // Open the client
    if (client.open (parser->filename_,
                     parser->hostname_,
                     parser->port_) == -1)
    {
        client.close ();
        return 0;
    }

    // Read from it
    if (client.read (&mb) <= 0)
    {
        client.close ();
        return 0;
    }

  return 1;
}



//function to parse and return the PackageConfiguration from the already
//extracted descriptor files
Deployment::PackageConfiguration*
CIAO_RepositoryManagerDaemon_i::retrieve_PC_from_descriptors (const char* pc_name,
                                                              const char* descriptor_dir)
{
  //change the working dir
  ACE_OS::chdir (descriptor_dir);

  Deployment::PackageConfiguration_var pc = new Deployment::PackageConfiguration ();
  //parse the PCD to make sure that there are no package errors
  ACE_TRY
    {
       CIAO::Config_Handlers::Packaging::PCD_Handler::package_config (pc_name, *pc);
    }
  ACE_CATCHALL
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) [RM::retrieve_PC_from_descriptors] Error parsing the PCD\n"));

      //change back the the old working dir
      ACE_OS::chdir (this->cwd_);
      ACE_THROW (Deployment::PackageError ());
    }
  ACE_ENDTRY;
  //able to parse the PC. So lets install the package in the repo

  //change back the the old working dir
  ACE_OS::chdir (this->cwd_);

  return pc._retn ();
}


//find out what the name of the PackageConfiguration file is
void CIAO_RepositoryManagerDaemon_i::find_PC_name (char* package, ACE_CString& pcd_name)
{
  pcd_name = "";  //empty the contents of the ACE_CString

  //create a doubly link list
  ACE_New_Allocator allocator;
  ACE_Double_Linked_List<ZIP_File_Info> list (&allocator);

  //get the list of files in the package and figure out the names of all necessary files
  if (!(ZIP_Wrapper::file_list_info (package, list)))
    return;

  size_t skip_len = ACE_OS::strlen ("descriptors") + 1;

  while (!list.is_empty ())
    {
      ZIP_File_Info* inf = list.delete_head ();

      if (ACE_OS::strstr (inf->name_.c_str (), "descriptors"))
        if (ACE_OS::strstr (inf->name_.c_str (), ".pcd"))
          pcd_name = inf->name_.c_str () + skip_len;

      //deallocate the head of the filename list
      delete inf;
    }
}


//We are using Xercesc in the Config_Handlers and unfortunately its API only
//takes a file in the local file system as an argument, thus need to
//write out the contents of the deployent plan to a file
//in the current directory. I use the thread id to guarrantee
//lack of race conditions if multithreading is enabled

int CIAO_RepositoryManagerDaemon_i::extract_descriptor_files (char* package, ACE_CString& pcd_name)
{
  //create a doubly link list
  ACE_New_Allocator allocator;
  ACE_Double_Linked_List<ZIP_File_Info> list (&allocator);

  //get the list of files in the package and figure out the names of all necessary files
  if (!(ZIP_Wrapper::file_list_info (package, list)))
    return 0;

  size_t skip_len = ACE_OS::strlen ("descriptors") + 1;

  while (!list.is_empty ())
    {
      ZIP_File_Info* inf = list.delete_head ();
      ACE_Message_Block* file = 0;
      if (ACE_OS::strstr (inf->name_.c_str (), "descriptors"))
        {
          if (ACE_OS::strstr (inf->name_.c_str (), ".pcd"))
            pcd_name = inf->name_.c_str () + skip_len;

          //extract the descriptor from the package
          ACE_NEW_RETURN (file, ACE_Message_Block (0,0), 0);
          if (!ZIP_Wrapper::get_file(const_cast<char*> (package),
                                     const_cast<char*> (inf->name_.c_str ()),
                                     *file))
            {
              ACE_ERROR ((LM_ERROR,
                          "[RM::extract_descriptor_files] Unable to retrieve file!\n"));
              //release the message block chain
              file->release ();
              return 0;
            }


          //write the file to disk
          if(!RM_Helper::write_to_disk (inf->name_.c_str () + skip_len, *file))
            {
              ACE_ERROR ((LM_ERROR,
                          "[RM::extract_descriptor_files] Unable to write out descriptor to disk!\n"));
              //release the message block chain
              file->release ();
              return 0;
            }

          //release the message block chain
          file->release ();
        }

      //deallocate the head of the filename list
      delete inf;
    }

  return 1;
}

int CIAO_RepositoryManagerDaemon_i::remove_descriptor_files (char* package)
{
  int return_code = 1;

  //create a doubly link list
  ACE_New_Allocator allocator;
  ACE_Double_Linked_List<ZIP_File_Info> list (&allocator);

  //get the list of files in the package and figure out the names of all necessary files
  if (!(ZIP_Wrapper::file_list_info (package, list)))
    return 0;

  size_t skip_len = ACE_OS::strlen ("descriptors") + 1;

  while (!list.is_empty ())
    {
      ZIP_File_Info* inf = list.delete_head ();
      if (ACE_OS::strstr (inf->name_.c_str (), "descriptors"))
        {
          //delete disk
          if(remove (inf->name_.c_str () + skip_len))
            {
              ACE_ERROR ((LM_ERROR,
                          "[RM::remove_descriptor_files] Unable to remove file from disk!\n"));
              return_code = 0;
            }
        }
      //deallocate the head of the filename list
      delete inf;
    }

  return return_code;
}

//function to remove the files extracted from the package upon istallation
//It reads the names of the files from the package. They correspond to the
//names on disk. It deletes each file, then it deletes the directories that
//contain them.
//return 1 on success
//       0 on error

int CIAO_RepositoryManagerDaemon_i::remove_extracted_package
                                       (const char* package_path,
                                        const char* extraction_location)
{
  //change the working dir
  if (ACE_OS::chdir (extraction_location) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "[RM::remove_extracted_package] Unable to chdir to doomed directory!\n"),
                      0);

  int return_code = 1;

  //create a doubly link list
  ACE_New_Allocator allocator;
  ACE_Double_Linked_List<ZIP_File_Info> list (&allocator);

  //get the list of files in the package and figure out the names of all necessary files
  if (!(ZIP_Wrapper::file_list_info (const_cast <char*> (package_path), list)))
    {
      //change back the the old working dir
      ACE_OS::chdir (this->cwd_);
      return 0;
    }

  while (!list.is_empty ())
    {
      ZIP_File_Info* inf = list.delete_head ();

      //delete file from disk
      if(remove (inf->name_.c_str ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "[RM::remove_extracted files] Unable to delete %s!\n", inf->name_.c_str ()));
          return_code = 0;
        }

      //deallocate the head of the filename list
      delete inf;
    }

  //now remove the descriptors and implementations directories.
  ACE_OS::rmdir ("descriptors");
  ACE_OS::rmdir ("implementations");

  //now go one directory up and delete the extraction directory
  ACE_OS::chdir (this->install_root_.c_str ());
  ACE_OS::rmdir (extraction_location);

  //change back the the old working dir
  ACE_OS::chdir (this->cwd_);

  return return_code;
}

//function to extract the type of the component from
//the PackageConfiguration and update the interface map
//returns 1 on success
//        0 on error

int CIAO_RepositoryManagerDaemon_i::add_type (Deployment::PackageConfiguration& pc,
                                              const char* name)
{
  if (pc.basePackage.length () > 0)
  {
    ::CORBA::StringSeq supportedTypes = pc.basePackage[0]
                                        .implementation[0]
                                        .referencedImplementation
                                        .implements
                                        .supportedType;

    if (supportedTypes.length () != 0)
    {
      CORBA::ULong len = supportedTypes.length ();
      for (CORBA::ULong i = 0; i < len; ++i)
      {
        this->types_.bind (ACE_CString (supportedTypes[i]), name);
      }
    }
  }
  else //ComponentPackageReference
  {
    //not implemented yet
    return 0;
  }

  return 1;
}

//function to remove the interface type of the component
//being removed from the interface map
//returns 1 on success
//        0 on error

int CIAO_RepositoryManagerDaemon_i::remove_type (Deployment::PackageConfiguration& pc,
                                                 const char* name)
{
  if (pc.basePackage.length () > 0)
  {
    ::CORBA::StringSeq supportedTypes = pc.basePackage[0]
                                        .implementation[0]
                                        .referencedImplementation
                                        .implements
                                        .supportedType;

    if (supportedTypes.length () != 0)
    {
      CORBA::ULong len = supportedTypes.length ();
      for (CORBA::ULong i = 0; i < len; ++i)
      {
        if (this->types_.unbind (ACE_CString (supportedTypes[i]), ACE_CString (name)) != 0)
          ACE_DEBUG ((LM_DEBUG, "Could not find type %s with package name %s!\n",
                      ACE_CString (supportedTypes[i]).c_str (),
                      name));
      }
    }
  }
  else //ComponentPackageReference
  {
    //not implemented yet
    return 0;
  }

  return 1;
}

//function to dump the state of the RepositoryManager
void CIAO_RepositoryManagerDaemon_i::dump (void)
{
#if defined (ACE_HAS_DUMP)

  ACE_DEBUG(LM_DEBUG, "NAMES:\n");
  this->names_.dump ();
  ACE_DEBUG(LM_DEBUG, "UUIDs:\n");
  this->uuids_.dump ();
  ACE_DEBUG (LM_DEBUG, "Component Interface Types:\n");
  this->types_.dump ();

#endif /* ACE_HAS_DUMP */
}
