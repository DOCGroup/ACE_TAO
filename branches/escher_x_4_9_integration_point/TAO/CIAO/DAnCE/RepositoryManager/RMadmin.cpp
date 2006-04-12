
/* -*- C++ -*- */

/***
 * file RMClient.cpp
 *
 * $Id$
 *
 * A sample client to the RepositoryManager showcasing how to use it
 *
 * author Stoyan Paunov <spaunov@isis.vanderbilt.edu>
 **/

#include "RepositoryManagerC.h"
#include "Options.h"

//#include "Config_Handlers/pcd.hpp"  //for the PackageConfiguration data struct
#include "ace/OS_NS_fcntl.h"      //for open
#include "ace/OS_NS_unistd.h"        //for close
#include "ace/OS_NS_sys_stat.h"        //for filesize and fstat and mkdir

#include "Config_Handlers/DnC_Dump.h"

#include <iostream>
using namespace std;

#include "RM_Helper.h"          //to be able to externalize/internalize a PackageConfiguration
#include "tao/CDR.h"          //for TAO CDR classes
#include "ace/Message_Block.h"      //for ACE_Message_Block



//IOR file of the RM
const char * ior = "file://RepositoryManagerDeamon.ior";


///=============================COUPLE OF HELPER METHORS==================================
CORBA::Octet* read_from_disk (
  const char* full_path,
  size_t &length
  );

int write_to_disk (
  const char* full_path,
  const CORBA::Octet* buffer,
  size_t length
  );
///========================================================================================


///main function that provides a sample interface for RM clients

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv,
                      ""ACE_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;


      CORBA::Object_var obj =
        orb->string_to_object (ior
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

    CIAO::RepositoryManagerDaemon_var rm =
      CIAO::RepositoryManagerDaemon::_narrow (obj.in ()
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (rm.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to acquire RepositoryManagerDaemon's objref\n"),
                            -1);
        }


    Options* options = TheOptions::instance ();
    if (!options->parse_args (argc, argv))
       return -1;

    if (options->shutdown_)
    {
      rm->shutdown ();
    }
    else if (options->all_names_)
    {
      try
      {
        CORBA::StringSeq_var seq = rm->getAllNames ();
        cout << "Known Names:\n";
        cout << "Seq length () : " << seq->length () << endl;
        for (size_t i = 0;
           i < seq->length ();
           ++i)
           cout << seq[i].in () << endl;
      }
      catch (CORBA::Exception & ex)
      {
        cout << "\nException caught!" << ex << "\n";
        return 0;
      }
    }
    else if (options->all_types_)
    {
      try
      {
        CORBA::StringSeq_var seq = rm->getAllTypes ();
        cout << "Known Component Interface Types:\n";
        for (size_t i = 0;
           i < seq->length ();
           ++i)
           cout << seq[i].in () << endl;
      }
      catch (CORBA::Exception & ex)
      {
        cout << "\nException caught!" << ex << "\n";
        return 0;
      }
    }
    else if (options->uuid_ != "" && options->names_by_type_)
    {
      try
      {
        CORBA::StringSeq_var seq = rm->findNamesByType (options->uuid_.c_str ());
        cout << "Known Component Interface Types:\n";
        for (size_t i = 0;
           i < seq->length ();
           ++i)
           cout << seq[i].in () << endl;
      }
      catch (CORBA::Exception & ex)
      {
        cout << "\nException caught!" << ex << "\n";
        return 0;
      }

    }
    else if (options->install_)
    {
      try
      {
        rm->installPackage (options->name_.c_str (), options->path_.c_str ());
      }
      catch (CORBA::Exception & ex)
      {
        cout << "\nException caught!" << ex << "\n";
        return 0;
      }

      cout << "\nReassuring that the package in the repository ..." << endl;
      try
      {
        Deployment::PackageConfiguration_var pc = rm->findPackageByName (options->name_.c_str ());
        cout << "The package was found!" << endl;
        cout << "Label: " << pc->label << endl;
        cout << "UUID: " << pc->UUID << endl;
      }
      catch (CORBA::Exception &)
      {
        cout << "\nError! Package not found!" << endl;
      }
    }
    else if (options->delete_)
    {
      try
      {
        rm->deletePackage (options->name_.c_str ());
        cout << options->name_.c_str () << " deleted" << endl;
      }
      catch (CORBA::Exception & ex)
      {
        cout << "\nException: " << ex << endl;
      }

    }
    else if (options->find_)
    {
      if (options->name_ != "")
      {
        Deployment::PackageConfiguration_var pc = rm->findPackageByName (options->name_.c_str ());
        cout << "The package was found!" << endl;
        Deployment::DnC_Dump::dump (pc);
      }
      else
      {
        Deployment::PackageConfiguration_var pc = rm->findPackageByUUID (options->uuid_.c_str ());
        cout << "The package was found!" << endl;
        Deployment::DnC_Dump::dump (pc);
      }
    }

      orb->shutdown (1);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unknown exception \n");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}



CORBA::Octet* read_from_disk (
  const char* full_path,
  size_t &length
  )
{
  //open the file

  ACE_HANDLE handle = ACE_OS::open (full_path, O_RDONLY);
    if (handle == ACE_INVALID_HANDLE)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("[RM::read_from_disk] file open error")),
                           0);

  ACE_stat file_info;

  ACE_OS::fstat (handle, &file_info);

  CORBA::Octet* buffer;
  ACE_NEW_RETURN (buffer, CORBA::Octet[file_info.st_size], 0);

  //read the contents of the file into the buffer
  if (ACE_OS::read_n (handle, buffer, file_info.st_size) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
               ACE_TEXT ("[RM::write_to_disk] file write error")),
                           0);

  // Close the file handle
    ACE_OS::close (handle);

  length = file_info.st_size;
  return buffer;
}


int write_to_disk (
  const char* full_path,
  const CORBA::Octet* buffer,
  size_t length
  )
{

  // Open a file handle to the local filesystem
    ACE_HANDLE handle = ACE_OS::open (full_path, O_CREAT | O_TRUNC | O_WRONLY);
    if (handle == ACE_INVALID_HANDLE)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("[RM::write_to_disk] file creation error")),
                           -1);

  //write the data to the file
  if (ACE_OS::write (handle, buffer, length) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
               ACE_TEXT ("[RM::write_to_disk] file write error")),
                           -1);

  // Close the file handle
    ACE_OS::close (handle);

  return 1;
}

