// $Id$

#include "ace/Log_Msg.h"
#include "ciao/Deployment_DataC.h"
#include "PC_Updater.h"
#include "PC_Updater_T.h"
#include "ace/Containers_T.h"        //for ACE_Double_Linked_List


#include <iostream>
using namespace std;

namespace
{
  const size_t TEMP_LEN = 1024;
}

using namespace PC_Updater_T;


  //PATH of glory/gory to update the locations of the IADs
  //
  //PackageConfiguration something;
  //ComponentPackageDescriptions basePackage;
  //PackagedComponentImplementations implementation;
  //ComponentImplementationDescription referencedImplementation;
  //
  //MONOLITHIC Component:
  //MonolithicImplementationDescriptions monolithicImpl;
  //NamedImplementationArtifacts primaryArtifact;
  //ImplementationArtifactDescription referencedArtifact;
  //::CORBA::StringSeq location;
  //
  //ASSEMBLY-BASED Component
  //ComponentAssemblyDescriptions assemblyImpl;
  //SubcomponentInstantiationDescriptions instance;
  //ComponentPackageDescriptions package;
  //...


  /*
   *  PC_Updater Constructors
   */

PC_Updater::PC_Updater (const char* server_path, const char* package)
: server_path_ (server_path),
  file_list_ (),
  package_ (package),
  success_ (true)
{
}


PC_Updater::PC_Updater (ACE_CString& server_path, ACE_CString& package)
: server_path_ (server_path),
  file_list_ (),
  package_ (package),
  success_ (true)
{
}

  /*
   *  PC_Updater - Destructor
   */

PC_Updater::~PC_Updater ()
{
  this->clear_list ();
}


void PC_Updater::clear_list ()
{
  while (!this->file_list_.is_empty ())
  {
    ZIP_File_Info* inf = this->file_list_.delete_head ();

    //deallocate the head of the filename list
    delete inf;
  }
}


  /*
   *  PC_Updater - Object update methods
   */


  // PackageConfiguration

  bool PC_Updater::update (const ::Deployment::PackageConfiguration &pc)
  {
    //get the list of files in the package and figure out the names of all necessary files
    if (!ZIP_Wrapper::file_list_info (const_cast <char*> (this->package_.c_str ()), this->file_list_))
      return false;

    update_sequence (pc.basePackage, this);

    return this->success_;
  }


  // ComponentInterfaceDescription

  void PC_Updater::update (const ::Deployment::ComponentInterfaceDescription &cid)
  {
  }

  // Requirement

  void PC_Updater::update (const ::Deployment::Requirement &req)
  {
  }


  // ComponentExternalPortEndpoint

  void PC_Updater::update (const ::Deployment::ComponentExternalPortEndpoint &cepe)
  {
  }



  // ImplementationDependency

  void PC_Updater::update(const Deployment::ImplementationDependency &id)
  {
  }

  // ComponentPackageReference

  void PC_Updater::update (const ::Deployment::ComponentPackageReference &cpr)
  {
  }

  // SubcomponentInstantiationDescription

  void PC_Updater::update (const ::Deployment::SubcomponentInstantiationDescription &sid)
  {
    update_sequence (sid.basePackage, this);
  }

  // SubcomponentPortEndpoint

  void PC_Updater::update (const ::Deployment::SubcomponentPortEndpoint& spe)
  {
  }

  // AssemblyConnectionDescription

  void PC_Updater::update (const ::Deployment::AssemblyConnectionDescription &acd)
  {
  }


  // AssemblyPropertyMapping

  void
  PC_Updater::update (const ::Deployment::AssemblyPropertyMapping &apm)
  {
  }

  // ComponentAssemblyDescription

  void PC_Updater::update (const ::Deployment::ComponentAssemblyDescription& cad)
  {
    update_sequence (cad.instance, this);
  }

  // ImplementationArtifactDescription

  void PC_Updater::update (const ::Deployment::ImplementationArtifactDescription &iad)
  {
    bool found = false;

    //cout << "label: " << iad.label << endl;
    //cout << "location: " << CORBA::string_dup (iad.location[0].in ()) << endl;

    ACE_Double_Linked_List_Iterator<ZIP_File_Info> iter (this->file_list_);
    char str [TEMP_LEN];

    while (!iter.done ())
    {
      ACE_OS::strncpy ( str, iter.next ()->name_.c_str (), TEMP_LEN);
      //weird. Need to call next to get current ?!?!

      const char* name;

      name = ACE_OS::strstr (str, iad.location[0]);

      if (name)
      {
        ACE_CString loc (this->server_path_);
        loc += name;
        loc += iad.location[0];

        iad.location[0] = CORBA::string_dup (loc.c_str ());

        //cout << "new location: " << iad.location[0].in () << endl << endl;

        found = true;
        break;
      }
      iter++;
    }

    if (!found)
      this->success_ = false;
  }

  // NamedImplementationArtifact

  void PC_Updater::update (const ::Deployment::NamedImplementationArtifact &nia)
  {
    update (nia.referencedArtifact);
  }

  // ImplementationRequirement

  void PC_Updater::update (const ::Deployment::ImplementationRequirement &ir)
  {
  }

  // MonolithicImplementationDescription

  void PC_Updater::update (const ::Deployment::MonolithicImplementationDescription &mid)
  {
    update_sequence (mid.primaryArtifact, this);
  }

  // Capability

  void PC_Updater::update (const ::Deployment::Capability &capability)
  {
  }



  // ComponentImplementationDescription

  void PC_Updater::update (
        const ::Deployment::ComponentImplementationDescription &cid)
  {
    update_sequence (cid.assemblyImpl, this);
    update_sequence (cid.monolithicImpl, this);
  }

  // PackagedComponentImplementation

  void PC_Updater::update (const ::Deployment::PackagedComponentImplementation &pci)
  {
    PC_Updater::update (pci.referencedImplementation);
  }

  // ComponentPackageDescription

  void PC_Updater::update (const ::Deployment::ComponentPackageDescription &comppkgdesc)
  {
    update_sequence (comppkgdesc.implementation, this);
  }


  // Property
  void PC_Updater::update (const Deployment::Property& property)
  {
  }
