/* -*- C++ -*- */

/***
 * file RMClient.cpp
 *
 * $Id$
 *
 * A sample client to the RepositoryManager showcasing how to use it
 *
 * author Stoyan Paunov <spaunov@isis.vanderbilt.edu>
 *        Shanshan Jiang <shanshan.jiang@vanderbilt.edu>
 **/

#include "RepositoryManagerDaemonC.h"
#include "Options.h"

#include "ace/OS_NS_fcntl.h"      //for open
#include "ace/OS_NS_unistd.h"        //for close
#include "ace/OS_NS_sys_stat.h"        //for filesize and fstat and mkdir
#include "ace/streams.h"

#include "Config_Handlers/DnC_Dump.h"

#include "RM_Helper.h"          //to be able to externalize/internalize a PackageConfiguration
#include "tao/CDR.h"          //for TAO CDR classes
#include "ace/Message_Block.h"      //for ACE_Message_Block

#include "Package_Handlers/PCD_Handler.h"

#include "orbsvcs/CosNamingC.h"


//IOR file of the RM
static const char *ior = "file://RepositoryManagerDaemon.ior";

// Name service of the RM
static const char *RMname_service;

/// main function that provides a sample interface for RM clients
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);


      Options* options = TheOptions::instance ();
      if (!options->parse_args (argc, argv))
        return -1;

      CORBA::Object_var obj;

      if (options->write_to_ior_)
      {
        obj = orb->string_to_object (ior);
      }

      else if (options->register_with_ns_)
      {
        if (options->repoman_name_ != "")
          RMname_service = const_cast<char*> (options->repoman_name_.c_str ());

        // Naming Service related operations
        CORBA::Object_var naming_context_object =
          orb->resolve_initial_references ("NameService");

        CosNaming::NamingContext_var naming_context =
          CosNaming::NamingContext::_narrow (naming_context_object.in ());

        // Initialize the Naming Sequence
        CosNaming::Name name (1);
        name.length (1);

        // String dup required for MSVC6
        name[0].id = CORBA::string_dup (RMname_service);

        // Resolve object from name
        obj = naming_context->resolve (name);
      }


    CIAO::RepositoryManagerDaemon_var rm =
      CIAO::RepositoryManagerDaemon::_narrow (obj.in ());

      if (CORBA::is_nil (rm.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to acquire RepositoryManagerDaemon's objref\n"),
                            -1);
        }

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
        for (size_t i = 0;
           i < seq->length ();
           ++i)
           cout << seq[i] << endl;
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
           cout << seq[i] << endl;
      }
      catch (CORBA::Exception & ex)
      {
        cout << "\nException caught!" << ex << "\n";
        return 0;
      }
    }
    else if (options->type_ != "" && options->names_by_type_)
    {
      try
      {
        CORBA::StringSeq_var seq = rm->findNamesByType (options->type_.c_str ());
        cout << "Known Component Interface Types:\n";
        for (size_t i = 0;
           i < seq->length ();
           ++i)
           cout << seq[i] << endl;
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
        rm->installPackage (options->name_.c_str (), options->path_.c_str (), false);
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
    else if (options->create_)
    {
      try
      {
        // Change the working dir.
        char cwd [1024];
        ACE_OS::getcwd (cwd, 1024);
        ACE_CString descriptor_dir (cwd);
        descriptor_dir += "/packageDescriptors/RACE/descriptors/";
        ACE_OS::chdir (descriptor_dir.c_str ());

        Deployment::PackageConfiguration *pc = new Deployment::PackageConfiguration ();

        // Parse the PCD to make sure that there are no package errors.
        try
        {
          CIAO::Config_Handlers::Packaging::PCD_Handler::package_config ("default.pcd", *pc);
        }
        catch (...)
        {
          ACE_ERROR ((
            LM_ERROR,
            "(%P|%t) [RM::retrieve_PC_from_descriptors] Error parsing the PCD\n"));

          throw Deployment::PackageError ();
        }

        ACE_OS::chdir (cwd);

        rm->createPackage (options->name_.c_str (), *pc, options->path_.c_str (), false);
      }
      catch (CORBA::Exception & ex)
      {
        cout << "\nException caught!" << ex << "\n";
        return 0;
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
        //Deployment::DnC_Dump::dump (pc);
      }
      else
      {
        Deployment::PackageConfiguration_var pc = rm->findPackageByUUID (options->uuid_.c_str ());
        cout << "The package was found!" << endl;
        //Deployment::DnC_Dump::dump (pc);
      }
    }

      orb->shutdown (1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unknown exception \n");
      return -1;
    }

  return 0;
}
