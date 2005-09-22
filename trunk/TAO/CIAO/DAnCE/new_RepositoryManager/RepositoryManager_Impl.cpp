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
#include "ciao/tools/Config_Handlers/STD_PC_Intf.h"
#include "ciao/tools/Config_Handlers/Deployment.hpp"
#include "ciao/Deployment_DataC.h"
#include "ciao/tools/Config_Handlers/XML_Helper.h"
#include "xercesc/dom/DOM.hpp"

#include "RM_Helper.h"					//to be able to externalize/internalize a PackageConfiguration
#include "tao/CDR.h"					//for TAO CDR classes
#include "ace/Message_Block.h"			//for ACE_Message_Block

#include "ace/Thread.h"					//for obtaining the ID of the current thread
#include "ace/OS_NS_stdlib.h"			//for itoa ()


#include "URL_Parser.h"
#include "HTTP_Client.h"


#include <iostream>
using namespace std;


// Implementation skeleton constructor
CIAO_RepositoryManagerDaemon_i::CIAO_RepositoryManagerDaemon_i (CORBA::ORB_ptr the_orb)
: the_orb_ (CORBA::ORB::_duplicate (the_orb))
{
	//create directory in which the packages will be stored

	ACE_OS::mkdir(RM_STORAGE_PATH);	
	//if dir already exists a -1 is returned
	//we ignore this, just need to make sure the directory exists

	ACE_OS::getcwd (this->cwd_, TEMP_LEN);
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

	ACE_CString path (this->cwd_);
	path += "/";
	path += RM_STORAGE_PATH;
	path += "/";
	path += installationName;

	ACE_CString pc_path (path);
	ACE_CString package_path (path);

	package_path += ".cpk";				//package extension
	pc_path += PC_EXTENSION;			//external PackageConfiguration extension


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
			cout << "open\n";
			ACE_THROW (CORBA::INTERNAL ());
		}

	    // Read from it
	    if (client.read (mb) <= 0)
	    {
			mb->release ();
			client.close ();
			cout << "read\n";
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


	//Start the parsing

	Deployment::PackageConfiguration_var pc;
	try
	{
		pc = retrieve_PC( const_cast<char*> (package_path.c_str ()));
	}
	catch (...) //TODO: this does not work. Fix it.
	{
		cout << "Some exception caugth\n";
		ACE_THROW (CORBA::INTERNAL ());
	}


	//now lets externalize the PackageConfiguration, so that we can access it later on
	//without having to do the whole parsing again.
	//NOTE: Order here is important. Do not populate maps before the externalization!
	RM_Helper::externalize (pc, pc_path.c_str ());

	//insert the package into the database
	this->names_.bind (ACE_CString (installationName), path);

	//ALSO NEED THE UUID here
	this->uuids_.bind (ACE_CString (pc->UUID), path);

	cout << pc->label << endl;
	cout << pc->UUID << endl;

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
	// I need to figure out how to do that without causing some race condition
	bool purge = false;
	if (purge)
	{
		ACE_CString path (RM_STORAGE_PATH);
		path += "/";
		path += installationName;
		path += ".cpk";

		remove (path.c_str ());
	}
}

//==========================================HELPER METHODS========================================================

Deployment::PackageConfiguration* CIAO_RepositoryManagerDaemon_i::retrieve_PC (char* package)
{
	char temp[128];
	unsigned int thread_id = ACE_Thread::self ();
	char* PID = ACE_OS::itoa (thread_id, temp, 10);

	ACE_OS::mkdir(PID);	
	//if dir already exists a -1 is returned
	//we ignore this, just need to make sure the directory exists

	//change the working dir
	ACE_OS::chdir (PID);

	ACE_CString pcd_name;
	//extract the necessary descriptors
	if (extract_necessary_files (package,
							     pcd_name) < 0)
	{
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
		ACE_THROW (Deployment::PackageError ());
    }
    ACE_ENDTRY;
	//able to parse the PC. So lets install the package in the repo

	//we no longer need the descriptors, so lets erase them!
	remove_extracted_files (package);

	//change back the the old working dir
	ACE_OS::chdir (this->cwd_);

	//now lets erase the directory!
	ACE_OS::rmdir (PID);
	//the ACE_OS::rmdir does not work. Possibly because we need to delete
	//the contents first. I will look into it more closely when I am back.

	return pc._retn ();
}



//We are using Xercesc in the Config_Handlers and unfortunately its API only
//takes a file in the local file system as an argument, thus need to
//write out the contents of the deployent plan to a file
//in the current directory. I use the thread id to guarrantee 
//lack of race conditions if multithreading is enabled

int CIAO_RepositoryManagerDaemon_i::extract_necessary_files (char* package, ACE_CString& pcd_name) 
{
	//create a ZIP wrapper
	ZIP_Wrapper zip;

	//create a doubly link list
	ACE_New_Allocator allocator;
	ACE_Double_Linked_List<ZIP_File_Info> list (&allocator);

	//get the list of files in the package and figure out the names of all necessary files
	if (!(zip.file_list_info (package, list)))
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
			if (!zip.get_file(const_cast<char*> (package), 
							  const_cast<char*> (inf->name_.c_str ()), 
							  *file))
			{
				ACE_ERROR ((LM_ERROR, 
							"[RM::extract_necessary_files] Unable to retrieve file!\n"));
				//release the message block chain
				file->release ();
				return 0;
			}


			//write the file to disk
			if(!RM_Helper::write_to_disk (inf->name_.c_str () + skip_len, *file))
			{
				ACE_ERROR ((LM_ERROR, 
							"[RM::extract_necessary_files] Unable to write out descriptor to disk!\n"));
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

int CIAO_RepositoryManagerDaemon_i::remove_extracted_files (char* package)
{
	//create a ZIP wrapper
	ZIP_Wrapper zip;
	int return_code = 1;

	//create a doubly link list
	ACE_New_Allocator allocator;
	ACE_Double_Linked_List<ZIP_File_Info> list (&allocator);

	//get the list of files in the package and figure out the names of all necessary files
	if (!(zip.file_list_info (package, list)))
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
							"[RM::remove_extracted files] Unable to write out descriptor to disk!\n"));
				return_code = 0;
			}
		}
		//deallocate the head of the filename list
		delete inf;
	}

	return return_code;
}
