// $Id$

#include "dance/Logger/Log_Macros.h"
#include "dance/Deployment/Deployment_DataC.h"
#include "PC_Updater.h"
#include "PC_Updater_T.h"
#include "ace/Containers_T.h"        //for ACE_Double_Linked_List

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


/// PackageConfiguration
bool PC_Updater::update (::Deployment::PackageConfiguration &pc)
{
  //get the list of files in the package and figure out the names of all necessary files
  if (!ZIP_Wrapper::file_list_info (const_cast <char*> (this->package_.c_str ()), this->file_list_))
    return false;

  update_sequence (pc.basePackage, this);

  return this->success_;
}


/// ComponentInterfaceDescription
void PC_Updater::update (::Deployment::ComponentInterfaceDescription &)
{
}

/// Requirement
void PC_Updater::update (::Deployment::Requirement &)
{
}


/// ComponentExternalPortEndpoint
void PC_Updater::update (::Deployment::ComponentExternalPortEndpoint &)
{
}

/// ImplementationDependency
void PC_Updater::update (Deployment::ImplementationDependency &)
{
}

// ComponentPackageReference

void PC_Updater::update (::Deployment::ComponentPackageReference &)
{
}

// SubcomponentInstantiationDescription

void PC_Updater::update (::Deployment::SubcomponentInstantiationDescription &sid)
{
  update_sequence (sid.basePackage, this);
}

// SubcomponentPortEndpoint

void PC_Updater::update (::Deployment::SubcomponentPortEndpoint& )
{
}

// AssemblyConnectionDescription

void PC_Updater::update (::Deployment::AssemblyConnectionDescription &)
{
}


// AssemblyPropertyMapping

void
PC_Updater::update (::Deployment::AssemblyPropertyMapping &)
{
}

// ComponentAssemblyDescription

void PC_Updater::update (::Deployment::ComponentAssemblyDescription& cad)
{
  update_sequence (cad.instance, this);
}

// ImplementationArtifactDescription

void PC_Updater::update (::Deployment::ImplementationArtifactDescription &iad)
{
  const char* location = CORBA::string_dup (iad.location[0]);

  //create an iterator
  ACE_Double_Linked_List_Iterator<ZIP_File_Info> iter (this->file_list_);

  //find the correct path and return
  while (!iter.done ())
  {
    const char* full_path = iter.next ()->name_.c_str ();
    //weird. Need to call next to get current ?!?!

    //is it an implementation artifact?
    const char* name = ACE_OS::strstr (full_path, "implementations/");
    if (name)
    {
      //now check if the name matches
      name = ACE_OS::strstr (full_path, iad.location[0]);

      if (name)
      {
        ACE_CString loc (this->server_path_);
        loc += "/implementations/";
        loc += location;

        iad.location[0] = CORBA::string_dup (loc.c_str ());

        //cout << "Location after update: " << iad.location[0] << endl << endl;
        return;
      }
    }
    iter++;
  }

  DANCE_ERROR (DANCE_LOG_ERROR, (LM_ERROR,
             "[PC_Updater::update] Unable to update: %s!\n",
             location));

  this->success_ = false;
}

// NamedImplementationArtifact

void PC_Updater::update (::Deployment::NamedImplementationArtifact &nia)
{
  update (nia.referencedArtifact);
}

// ImplementationRequirement
void PC_Updater::update (::Deployment::ImplementationRequirement &)
{
}

// MonolithicImplementationDescription
void PC_Updater::update (::Deployment::MonolithicImplementationDescription &mid)
{
  update_sequence (mid.primaryArtifact, this);
}

// Capability
void PC_Updater::update (::Deployment::Capability &)
{
}

// ComponentImplementationDescription
void PC_Updater::update (::Deployment::ComponentImplementationDescription &cid)
{
  update_sequence (cid.assemblyImpl, this);
  update_sequence (cid.monolithicImpl, this);
}

// PackagedComponentImplementation
void PC_Updater::update (::Deployment::PackagedComponentImplementation &pci)
{
  PC_Updater::update (pci.referencedImplementation);
}

// ComponentPackageDescription
void PC_Updater::update (::Deployment::ComponentPackageDescription &comppkgdesc)
{
  update_sequence (comppkgdesc.implementation, this);
}


// Property
void PC_Updater::update (Deployment::Property& )
{
}
