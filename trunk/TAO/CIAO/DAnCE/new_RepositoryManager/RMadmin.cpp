// $Id$

/**
 * #file RMadmin.cpp
 *
 * @author Stoyan Paunov <spaunov@isis.vanderbilt.edu>
 **/

#include "new_RepositoryManagerC.h"
#include "Options.h"

#include "ace/OS_NS_fcntl.h"      //for open
#include "ace/OS_NS_unistd.h"     //for close
#include "ace/OS_NS_sys_stat.h"   //for filesize and fstat and mkdir

#include "Config_Handlers/DnC_Dump.h"

#include <iostream>
using namespace std;

//IOR file of the Sender
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

      CIAO::new_RepositoryManagerDaemon_var rm =
        CIAO::new_RepositoryManagerDaemon::_narrow (obj.in ()
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (rm.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR, 
                             "Unable to acquire new_RepositoryManagerDaemon's objref\n"),
                            -1);
        }


      Options* options = Options::instance ();
      options->parse_args (argc, argv);
                
      if (options->shutdown_)
        {
          rm->shutdown ();
        }
      else if (options->install_)
        {
          size_t length = 0;     
          CORBA::Octet* buffer = read_from_disk (options->local_path_.c_str (), length);

          Deployment::Package* the_package = new Deployment::Package (
                                                                      length,                           //max of the sequence
                                                                      length,                           //length of the sequence
                                                                      buffer,                           //data to be stored within the sequence
                                                                      true                              //take ownership of the data
                                                                      );                

          try
            {
              rm->installPackage (options->package_.c_str (), *the_package, false);
            }
          catch (CORBA::Exception &ex)
            {
              cout << "\nPackage is already in the repository!\n";
            }

          cout << "\nReassuring that the package in the repository ..." << endl;
          try
            {
              Deployment::Package_var package_back = rm->findPackageByName (options->package_.c_str ());
              cout << "The package was found!" << endl;
              cout << "Its size is " << package_back->length () << endl;
            }
          catch (CORBA::Exception &ex)
            {
              cout << "\nNo such package!" << endl;
            }
        }
      else if (options->delete_)
        {
          try
            {
              rm->deletePackage (options->package_.c_str ());
              cout << options->package_.c_str () << " deleted" << endl;
            }
          catch (CORBA::Exception &ex)
            {
              cout << "\nNo such package!" << endl;
            }

        }
      else if (options->plan_)
        {

          Deployment::DeploymentPlan_var plan = rm->retrievePlan (options->package_.c_str ());

          Deployment::DnC_Dump::dump (plan);
        }
      else if (options->artifact_ != "")
        {
          try
            {
              Deployment::Implementation_var impl = rm->findImplementationByName (options->artifact_.c_str (), 
                                                                                  options->package_.c_str());
              write_to_disk (impl->name, impl->the_implementation.get_buffer (),impl->the_implementation.length ()); 
            }
          catch (CORBA::Exception &ex)
            {
              cout << "\nNo such implementation artifact in that package!" << endl;
            }

        }


      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
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

  CORBA::Octet* buffer = new CORBA::Octet[file_info.st_size];

  if (buffer == 0)
    return 0;

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

