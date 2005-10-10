// $Id$

///====================================================================
// filename: RepositoryManager_Impl.cpp
// Author: Stoyan Paunov	spaunov@isis.vanderbilt.edu
//

#include "RepositoryManager_Impl.h"

#include "ace/OS_NS_fcntl.h"			//for open
#include "ace/OS_NS_unistd.h"			//for close
#include "ace/OS_NS_sys_stat.h"			//for filesize and fstat and mkdir
#include "ace/OS_NS_string.h"			//for ACE_CString


//to remove a file or dir from the local filesystem need remove () from stdio.h
// ---> need to include ace/OS_NS_stdio.h which would include the correct file for any OS!
#include "ace/OS_NS_stdio.h"

#include "ZIP_Wrapper.h"					//Wrapper around zzip
#include "ace/Containers_T.h"				//for ACE_Double_Linked_List
#include "ace/Malloc_Allocator.h"			//for ACE_New_Allocator needed by the doubly link list

//for the PackageConfiguration parsing
#include "Config_Handlers/STD_PC_Intf.h"
//#include "Config_Handlers/Deployment.hpp"
#include "ciao/Deployment_DataC.h"
#include "ciao/Packaging_DataC.h"
#include "Config_Handlers/Utils/XML_Helper.h"
#include "xercesc/dom/DOM.hpp"

#include "RM_Helper.h"					//to be able to externalize/internalize a PackageConfiguration
#include "ace/Message_Block.h"			//for ACE_Message_Block

#include "ace/Thread.h"					//for obtaining the ID of the current thread
#include "ace/OS_NS_stdlib.h"			//for itoa ()

#include "URL_Parser.h"					//for parsing the URL
#include "HTTP_Client.h"				//the HTTP client class to downloading packages

#include "PC_Updater.h"					//A visitor class to walk through the elements of the PC

#include <iostream>
using namespace std;


// Implementation skeleton constructor
CIAO_RepositoryManagerDaemon_i::CIAO_RepositoryManagerDaemon_i (CORBA::ORB_ptr the_orb, const char* server)
  : the_orb_ (CORBA::ORB::_duplicate (the_orb)),
    install_root_ (""),
    HTTP_server_ (server)
{
  //create directory in which the packages will be stored

  ACE_OS::mkdir(INSTALL_PATH);	
  //if dir already exists a -1 is returned
  //we ignore this, just need to make sure the directory exists

  ACE_OS::getcwd (this->cwd_, TEMP_LEN);

  this->install_root_ = this->cwd_;
  this->install_root_ += "/";
  this->install_root_ += INSTALL_PATH;
}

// Implementation skeleton destructor
CIAO_RepositoryManagerDaemon_i::~CIAO_RepositoryManagerDaemon_i (void)
{
  this->names_.unbind_all ();
  this->uuids_.unbind_all ();
}

void CIAO_RepositoryManagerDaemon_i::shutdown (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // Add your implementation here
  this->names_.unbind_all ();
  this->uuids_.unbind_all ();

  this->the_orb_->shutdown (0);
}


void CIAO_RepositoryManagerDaemon_i::installPackage (
                                                     const char * installationName,
                                                     const char * location
                                                     )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   ::Deployment::NameExists,
                   ::Deployment::PackageError
                   ))
{
  // Add your implementation here
	
  ACE_Hash_Map_Entry <ACE_CString, ACE_CString> *entry;

  if (this->names_.find (ACE_CString (installationName), entry) == 0)
    ACE_THROW (Deployment::NameExists ());

  //Now lets form the path for the local file
  //NOTE: I need the absolute path because I will change to a subdirectory 
  //when I am parsing the descriptors

  ACE_CString path (this->install_root_);
  path += "/";
  path += installationName;

  ACE_CString package_path (path);
  package_path += ".cpk";				//package extension

  ACE_CString pc_path (path);
  pc_path += PC_EXTENSION;			//external PackageConfiguration extension


  ACE_CString descriptor_dir (path);
  descriptor_dir += "/descriptors/"; //location of the descriptor directory


  //check if URL or local file
  //download or load into memory

  size_t length = 0;	 

  if (ACE_OS::strstr (location, "http://"))
    {
      URL_Parser *parser = URL_Parser::instance ();
      if (!parser->parseURL (const_cast <char*> (location)))
        ACE_THROW (CORBA::INTERNAL ());

      // Create a client
      HTTP_Client client;

      //TODO: how can I incorporate a Auto_Ptr is explicit release is needed
      ACE_Message_Block* mb = new ACE_Message_Block (0,0);

      // Open the client
      if (client.open (parser->filename_,
                       parser->hostname_,
                       parser->port_) == -1)
        {
          mb->release ();
          client.close ();
          ACE_THROW (CORBA::INTERNAL ());
        }

      // Read from it
      if (client.read (mb) <= 0)
        {
          mb->release ();
          client.close ();
          ACE_THROW (CORBA::INTERNAL ());
        }

      // Write file to designated location on disk
      if (!RM_Helper::write_to_disk (package_path.c_str (), *mb))
        {
          mb->release ();
          client.close ();
          ACE_THROW (CORBA::INTERNAL ());
        }

      mb->release ();
      client.close ();
    }
  else
    {
      CORBA::Octet* file = 0;

      //read the package from disk and store in the RM directory
      //see if you can substiture this with a memory mapped file
      //for better perofrmance (mimic zero copy here)
      file = RM_Helper::read_from_disk (location, length);

      if (!file)
        ACE_THROW (CORBA::INTERNAL ());

      //Store the package in the local RM dir for future retrieval
      if (!RM_Helper::write_to_disk (package_path.c_str (), file, length))
        ACE_THROW (CORBA::INTERNAL ());

      //NOTE: MEMORY LEAK UNLESS delete file; change to Auto_Ptr
      delete file;
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
    ACE_THROW (Deployment::PackageError ());


  //TODO: move exception throwing out of this func. User boolean error handling!!!
  //TODO: check for errors!
  Deployment::PackageConfiguration_var pc;
  pc = this->retrieve_PC_from_descriptors (const_cast<char*> (pc_name.c_str ()), 
                                           descriptor_dir.c_str ());


  //forming the server path info
  ACE_CString server_path (this->HTTP_server_);
  server_path += installationName;
  server_path += "/implementations/";

  PC_Updater updater (server_path, package_path);
	
  if (!updater.update (pc))
    {
      cout << "[RM] problem updating the PackageConfiguration!\n";
      remove_extracted_package (package_path.c_str (), path.c_str ());
      remove (package_path.c_str ());
      ACE_THROW (Deployment::PackageError ());
    }


  //now lets externalize the PackageConfiguration, so that we can access it later on
  //without having to do the whole parsing again.
  //NOTE: Order here is important. Do not populate maps before the externalization!
  RM_Helper::externalize (pc, pc_path.c_str ());

  //insert the package into the database
  this->names_.bind (ACE_CString (installationName), path);

  //ALSO NEED THE UUID here
  this->uuids_.bind (ACE_CString (pc->UUID), path);

  ACE_TRACE (("Installed PackageConfiguration with: \nlabel %s \nuuid %s\n", pc->label, pc->UUID));
}

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
  // Add your implementation here
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

::Deployment::PackageConfiguration * CIAO_RepositoryManagerDaemon_i::findPackageByName (
                                                                                        const char * name
                                                                                        )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   ::Deployment::NoSuchName
                   ))
{
  // Find out if the PackageConfiguration was installed in the repository,
  // return it if found or throw and exception otherwise

  ACE_Hash_Map_Entry <ACE_CString, ACE_CString> *entry = 0;

  if (this->names_.find (ACE_CString (name), entry) != 0)
    ACE_THROW (Deployment::NoSuchName ());							
  //PackageConfiguration was not found

  ACE_CString pc_path (entry->int_id_.c_str ());
  pc_path += PC_EXTENSION;

  Deployment::PackageConfiguration_var pc = new Deployment::PackageConfiguration (); 
	
  if(!RM_Helper::reincarnate (pc, pc_path.c_str ()))
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  //maybe I will still retrieve it in the case of failure: TODO
  //Deployment::PackageConfiguration_var pc = retrieve_PC(const_cast<char*> ());

  return pc._retn ();
}

::Deployment::PackageConfiguration * CIAO_RepositoryManagerDaemon_i::findPackageByUUID (
                                                                                        const char * UUID
                                                                                        )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   ::Deployment::NoSuchName
                   ))
{
  // Add your implementation here
 
  //NOTE: The following code actually works but I have not gotten 
  //the DELETE to remove the UUIDs and I am not allowing this code 
  //to be used for right now because the RM stays in an inconsistent 
  //state. A package can be deleted but it is still retrievable by is UUID.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);


  // Find out if the PackageConfiguration was installed in the repository,
  // return it if found or throw and exception otherwise

  ACE_Hash_Map_Entry <ACE_CString, ACE_CString> *entry = 0;

  if (this->uuids_.find (ACE_CString (UUID), entry) != 0)
    ACE_THROW (Deployment::NoSuchName ());							
  //PackageConfiguration was not found

  ACE_CString pc_path (entry->int_id_.c_str ());
  pc_path += PC_EXTENSION;

  Deployment::PackageConfiguration_var pc = new Deployment::PackageConfiguration (); 
	
  if(!RM_Helper::reincarnate (pc, pc_path.c_str ()))
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  //maybe I will still retrieve it in the case of failure: TODO
  //Deployment::PackageConfiguration_var pc = retrieve_PC(const_cast<char*> ());

  return pc._retn ();
}

::CORBA::StringSeq * CIAO_RepositoryManagerDaemon_i::findNamesByType (
                                                                      const char * type
                                                                      )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // Add your implementation here
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

::CORBA::StringSeq * CIAO_RepositoryManagerDaemon_i::getAllNames (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // Add your implementation here

  CORBA::ULong num_entries = this->names_.current_size (); 
  CORBA::StringSeq_var seq = new CORBA::StringSeq (num_entries);

  CORBA::ULong index = 0;
  for (PCMap_Iterator iter = this->names_.begin ();
       iter != this->names_.end () && index < num_entries;
       iter++)

    seq[index] = const_cast<char*> (((*iter).int_id_).c_str ());	//this looks hideous, but as long as it works!


  return seq._retn ();		//release the underlying CORBA::StringSeq
}

::CORBA::StringSeq * CIAO_RepositoryManagerDaemon_i::getAllTypes (
    
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // Add your implementation here
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void CIAO_RepositoryManagerDaemon_i::deletePackage (
                                                    const char * installationName
                                                    )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   ::Deployment::NoSuchName
                   ))
{
  // Add your implementation here

  ACE_Hash_Map_Entry <ACE_CString, ACE_CString> *entry;

  if (this->names_.find (ACE_CString (installationName), entry) != 0)
    ACE_THROW (Deployment::NoSuchName ());
  else
    this->names_.unbind (installationName);

  //do the same for the UUID!!!!
  //TODO: NEED TO DO THIS LATER


  //actually delete the package here!
  //TODO: Check if files are ref-counted by the OS, so if I delete it here while
  //some other process is still using it from a previous request, would that cause an error,
  //or would it decrement the recount and delete the file when the other process is done.
  //
  //on that note is a file handle is not closed the file does not get deleted! , so see how
  //temp files work!
		
  ACE_CString path (this->install_root_.c_str ());
  path += "/";
  path += installationName;

  ACE_CString package_path (path);
  package_path += ".cpk";				//package extension

  ACE_CString pc_path (path);
  pc_path += PC_EXTENSION;			//external PackageConfiguration extension

  remove_extracted_package (package_path.c_str (), path.c_str ());

  remove (package_path.c_str ());
  remove (pc_path.c_str ());
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
      CIAO::Config_Handlers::STD_PC_Intf intf (pcd_name.c_str ());
    
      pc = intf.get_PC ();
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


//function to parse and return the PackageConfiguration from the already
//extracted descriptor files
Deployment::PackageConfiguration* 
CIAO_RepositoryManagerDaemon_i::retrieve_PC_from_descriptors (const char* pc_name, 
                                                              const char* descriptor_dir)
{
  //change the working dir
  ACE_OS::chdir (descriptor_dir);

  Deployment::PackageConfiguration_var pc;
  //parse the PCD to make sure that there are no package errors
  ACE_TRY
    {
      CIAO::Config_Handlers::STD_PC_Intf intf (pc_name);
    
      pc = intf.get_PC ();
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
  pcd_name = "";	//empty the contents of the ACE_CString

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
          file = new ACE_Message_Block (0,0);
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
                          "[RM::remove_descriptor_files] Unable to write out descriptor to disk!\n"));
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


