// $Id$

#include "CoWorkEr_Generator.h"
#include "CUTS_Project.h"
#include "UDM_Utility_T.h"
#include "Uml.h"

// Static decl.
static const std::string COWORKER_POSTFIX ("_CoWorkEr");

// Static decl.
static const std::string COWORKER_RECEPTACLE ("cuts_testing_service");

// Static decl.
static const std::string CUTS_PREFIX ("CUTS_");

// Static decl.
static const std::string CUTS_FILE ("cuts/");

// Static decl.
static const std::string
COWORKER_INTERFACE_FOLDER ("CoWorkEr_InterfaceDefinitions");

//
// CUTS_UDM_CoWorkEr_Generator
//
CUTS_UDM_CoWorkEr_Generator::CUTS_UDM_CoWorkEr_Generator (void)
: parent_ (0),
  coworkers_ (0)
{

}

//
// ~CUTS_UDM_CoWorkEr_Generator
//
CUTS_UDM_CoWorkEr_Generator::~CUTS_UDM_CoWorkEr_Generator (void)
{

}

void CUTS_UDM_CoWorkEr_Generator::Visit_RootFolder (
  const PICML::RootFolder & root)
{
  // Get all the <PICML::InterfaceDefinitions> elements.
  typedef std::vector <PICML::InterfaceDefinitions> IDefs_Set;
  IDefs_Set idefs = root.InterfaceDefinitions_children ();

  // We need to locate the <CoWorkEr_InterfaceDefinitions> folder
  // or create one since this is where all the <CoWorkErs> will be
  // placed.
  if (create_element_if_not_exist (idefs,
                                   Find_Element_By_Name <
                                      PICML::InterfaceDefinitions> (
                                      COWORKER_INTERFACE_FOLDER),
                                   root,
                                   Udm::NULLCHILDROLE,
                                   this->coworker_idefs_))
  {
    this->coworker_idefs_.name () = COWORKER_INTERFACE_FOLDER;
  }

  // We need to visit all the <PICML::InterfaceDefinitions> and generate
  // the <CoWorkEr> elements for all <PICML::Component> elements.
  for (IDefs_Set::iterator iter = idefs.begin ();
       iter != idefs.end ();
       iter ++)
  {
    // We need to ingore the "CoWorkEr_InterfaceDefinitions" folder and
    // any folder that begins w/ "CUTS_".
    if (!((std::string)iter->name () == COWORKER_INTERFACE_FOLDER ||
        std::string(iter->name ()).find (CUTS_FILE) == 0))
    {
      iter->Accept (*this);
    }
  }
}

//
// Visit_InterfaceDefinitions
//
void CUTS_UDM_CoWorkEr_Generator::Visit_InterfaceDefinitions (
  const PICML::InterfaceDefinitions & idefs)
{
  // Get all the <PICML::File> elements in this folder.
  typedef std::vector <PICML::File> File_Set;
  File_Set files = idefs.File_children ();

  // Get the current <PICML::File> elements in target.
  File_Set current_files = this->coworker_idefs_.File_children ();

  for (File_Set::iterator iter = files.begin ();
       iter != files.end ();
       iter ++)
  {
    // We ingore any string that begins w/ 'cuts/'
    if (std::string (iter->name ()).find (CUTS_FILE) == 0)
      continue;

    // Get the <CoWorkEr> equivalent <PICML::File>. If we do not
    // find it we need to create a new one.
    if (create_element_if_not_exist (current_files,
                                     Find_Element_By_Name <PICML::File> (
                                        (std::string)iter->name () +
                                        COWORKER_POSTFIX),
                                     this->coworker_idefs_,
                                     Udm::NULLCHILDROLE,
                                     this->parent_))
    {
      this->parent_.name () =
        (std::string)iter->name () + COWORKER_POSTFIX;
    }

    // Set the <parent_> object to the newly created object
    // then visit the <iter> element.
    this->coworkers_ = 0;
    iter->Accept (*this);

    if (this->coworkers_ == 0)
      this->parent_.parent () = PICML::InterfaceDefinitions::Cast (Udm::null);
  }
}

//
// Visit_File
//
void CUTS_UDM_CoWorkEr_Generator::Visit_File (const PICML::File & file)
{
  // Get all the <PICML::Package> elements in this <file>.
  typedef std::vector <PICML::Package> Package_Set;
  Package_Set packages = file.Package_children ();

  // Get the current <PICML::Package> elements in <parent_>.
  Package_Set current_packages = this->parent_.Package_children ();
  PICML::Package target_pkg;

  for (Package_Set::iterator iter = packages.begin ();
       iter != packages.end ();
       iter ++)
  {
    if (create_element_if_not_exist (current_packages,
                                     Find_Element_By_Name <PICML::Package> (
                                        iter->name ()),
                                     this->parent_,
                                     Udm::NULLCHILDROLE,
                                     target_pkg))
    {
      target_pkg.name () = iter->name ();
    }

    // Set the <parent_> object to the newly created object
    // then visit the <iter> element.
    Package_Visitor pkg_visitor (target_pkg);
    iter->Accept (pkg_visitor);

    if (pkg_visitor.coworker_count () > 0)
      this->coworkers_ += pkg_visitor.coworker_count ();
    else
      target_pkg.parent () = PICML::File::Cast (Udm::null);
  }

  // Get all the <Component> elements at this level and create a
  // duplicate copy of them in the <parent_> package.
  typedef std::vector <PICML::Component> Component_Set;
  Component_Set components = file.Component_kind_children ();
  Component_Set curr_components = file.Component_kind_children ();

  for (Component_Set::iterator iter = components.begin ();
       iter != components.end ();
       iter ++)
  {
    std::string name = (std::string)iter->name () + COWORKER_POSTFIX;

    Component_Set::iterator citer =
      std::find_if (curr_components.begin (),
                    curr_components.end (),
                    Find_Element_By_Name <PICML::Component> (name));

    if (citer == curr_components.end ())
    {
      this->coworker_ = iter->CreateDerived (this->parent_);
      this->coworker_.name () = name;
    }
    else
      this->coworker_ = *iter;

    // Visit the <iter> to create the <coworker_>.
    iter->Accept (*this);
    ++ this->coworkers_;
  }

  if (this->coworkers_ > 0)
  {
    typedef std::vector <PICML::FileRef> FileRef_Set;
    FileRef_Set filerefs = this->parent_.FileRef_children ();
    FileRef_Set::iterator iter;

    // Let's create an explicit file reference to the <cuts/CUTS> file. We
    // need to make sure one does not already exist in the <parent_> model
    // which is really a <PICML::File> element.
    for (iter = filerefs.begin ();
        iter != filerefs.end ();
        iter ++)
    {
      if (PICML::File::Cast (iter->ref ()) ==
          CUTS_Project::instance ()->get_cuts_file ())
        break;
    }

    if (iter == filerefs.end ())
    {
      PICML::FileRef fileref = PICML::FileRef::Create (this->parent_);
      fileref.ref () = CUTS_Project::instance ()->get_cuts_file ();
    }

    // Let's create an explicit file reference to the <file>. We
    // need to make sure one does not already exist in the <parent_> model
    // which is really a <PICML::File> element.
    for (iter = filerefs.begin ();
        iter != filerefs.end ();
        iter ++)
    {
      if (PICML::File::Cast (iter->ref ()) == file)
        break;
    }

    if (iter == filerefs.end ())
    {
      PICML::FileRef fileref = PICML::FileRef::Create (this->parent_);
      fileref.ref () = file;
    }
  }
}

//
// Visit_Component
//
void CUTS_UDM_CoWorkEr_Generator::Visit_Component (
  const PICML::Component & component)
{
  // Get all the <PICML::ProvidedRequestPort> for this component.
  typedef std::vector <PICML::RequiredRequestPort> Receptacle_Set;

  Receptacle_Set receptacles =
    this->coworker_.RequiredRequestPort_kind_children ();

  // We need to locate the <cuts_testing_service> receptacles.
  PICML::RequiredRequestPort testing_service;

  if (create_element_if_not_exist (receptacles,
                                   Find_Element_By_Name <
                                      PICML::RequiredRequestPort> (
                                      COWORKER_RECEPTACLE),
                                   this->coworker_,
                                   Udm::NULLCHILDROLE,
                                   testing_service))
  {
    testing_service.name () = COWORKER_RECEPTACLE;
  }

  // Verify <COWORKER_FACET> is connected to <Testing_Service> object.
  if (PICML::Object::Cast (testing_service.ref ()) !=
      CUTS_Project::instance ()->get_testing_service ())
  {
    testing_service.ref () = CUTS_Project::instance ()->get_testing_service ();
  }

  // Verify that the <coworker_> do not already have an <ComponentFactory>
  // attached to it.
  typedef std::set <PICML::ManagesComponent> Manages_Set;
  Manages_Set manages = component.srcManagesComponent ();

  // There is no need to continue if there isn't a <ComponentFactory>
  // attached to this <component>.
  if (manages.empty ())
    return;

  PICML::ComponentFactory factory =
    manages.begin ()->srcManagesComponent_end ();

  Manages_Set curr_manages = this->coworker_.srcManagesComponent ();

  // There is no need to continue if we already have a factory attached
  // to the <coworker_>.

  // @@todo We need make sure the factory of the coworker component is
  // an instance of the real component's factory.
  if (!curr_manages.empty ())
    return;

  PICML::ComponentFactory cfactory = factory.CreateDerived (this->parent_);
  std::string name = (std::string)cfactory.name () + COWORKER_POSTFIX;
  cfactory.name () = name;

  // Create the connection between <cfactory> and <coworker_>
  PICML::ManagesComponent manage =
    PICML::ManagesComponent::Create (this->parent_);
  manage.dstManagesComponent_end () = this->coworker_;
  manage.srcManagesComponent_end () = cfactory;
}

//=============================================================================
///////////////////////////////////////////////////////////////////////////////
//=============================================================================

//
// Package_Visitor
//
CUTS_UDM_CoWorkEr_Generator::
Package_Visitor::Package_Visitor (const PICML::Package & parent)
: parent_ (parent),
  coworkers_ (0)
{

}

//
// ~Package_Visitor
//
CUTS_UDM_CoWorkEr_Generator::
Package_Visitor::~Package_Visitor (void)
{

}

//
// Visit_Package
//
void CUTS_UDM_CoWorkEr_Generator::
Package_Visitor::Visit_Package (const PICML::Package & package)
{
  // Get all the <Package> elements at this level and create a
  // duplicate copy of them in the <parent_> package.
  typedef std::vector <PICML::Package> Package_Set;
  Package_Set packages = package.Package_children ();
  Package_Set curr_packages = this->parent_.Package_children ();
  PICML::Package target_pkg;

  for (Package_Set::iterator iter = packages.begin ();
       iter != packages.end ();
       iter ++)
  {
    if (create_element_if_not_exist (
          curr_packages,
          Find_Element_By_Name <PICML::Package> (iter->name ()),
          this->parent_,
          Udm::NULLCHILDROLE,
          target_pkg))
    {
      target_pkg.name () = iter->name ();
    }

    Package_Visitor pkg_visitor (target_pkg);
    iter->Accept (pkg_visitor);

    if (pkg_visitor.coworker_count () > 0)
      this->coworkers_ += pkg_visitor.coworker_count ();
    else
      target_pkg.parent () = PICML::Package::Cast (Udm::null);
  }

  // Get all the <Component> elements at this level and create a
  // duplicate copy of them in the <parent_> package.
  typedef std::vector <PICML::Component> Component_Set;
  Component_Set components = package.Component_kind_children ();
  Component_Set curr_components = this->parent_.Component_kind_children ();

  for (Component_Set::iterator iter = components.begin ();
       iter != components.end ();
       iter ++)
  {
    std::string name = (std::string)iter->name () + COWORKER_POSTFIX;

    Component_Set::iterator citer =
      std::find_if (curr_components.begin (),
                    curr_components.end (),
                    Find_Element_By_Name <PICML::Component> (name));

    if (citer == curr_components.end ())
    {
      this->coworker_ = iter->CreateDerived (this->parent_);
      this->coworker_.name () = name;
    }
    else
      this->coworker_ = *iter;

    // Visit the <iter> to create the <coworker_>.
    iter->Accept (*this);
    ++ this->coworkers_;
  }
}

//
// Visit_Component
//
void CUTS_UDM_CoWorkEr_Generator::
Package_Visitor::Visit_Component (const PICML::Component & component)
{
  // Get all the <PICML::ProvidedRequestPort> for this component.
  typedef std::vector <PICML::RequiredRequestPort> Receptacle_Set;

  Receptacle_Set receptacles =
    this->coworker_.RequiredRequestPort_kind_children ();

  // We need to locate the <cuts_testing_service> receptacles.
  PICML::RequiredRequestPort testing_service;

  if (create_element_if_not_exist (receptacles,
                                   Find_Element_By_Name <
                                      PICML::RequiredRequestPort> (
                                      COWORKER_RECEPTACLE),
                                   this->coworker_,
                                   Udm::NULLCHILDROLE,
                                   testing_service))
  {
    testing_service.name () = COWORKER_RECEPTACLE;
  }

  // Verify <COWORKER_FACET> is connected to <Testing_Service> object.
  if (PICML::Object::Cast (testing_service.ref ()) !=
      CUTS_Project::instance ()->get_testing_service ())
  {
    testing_service.ref () = CUTS_Project::instance ()->get_testing_service ();
  }

  // Verify that the <coworker_> do not already have an <ComponentFactory>
  // attached to it.
  typedef std::set <PICML::ManagesComponent> Manages_Set;
  Manages_Set manages = component.srcManagesComponent ();

  // There is no need to continue if there isn't a <ComponentFactory>
  // attached to this <component>.
  if (manages.empty ())
    return;

  PICML::ComponentFactory factory =
    manages.begin ()->srcManagesComponent_end ();

  Manages_Set curr_manages = this->coworker_.srcManagesComponent ();

  // There is no need to continue if we already have a factory attached
  // to the <coworker_>.

  // @@todo We need make sure the factory of the coworker component is
  // an instance of the real component's factory.
  if (!curr_manages.empty ())
    return;

  PICML::ComponentFactory cfactory = factory.CreateDerived (this->parent_);
  std::string name = (std::string)cfactory.name () + COWORKER_POSTFIX;
  cfactory.name () = name;

  // Create the connection between <cfactory> and <coworker_>
  PICML::ManagesComponent manage =
    PICML::ManagesComponent::Create (this->parent_);
  manage.dstManagesComponent_end () = this->coworker_;
  manage.srcManagesComponent_end () = cfactory;
}

size_t CUTS_UDM_CoWorkEr_Generator::
Package_Visitor::coworker_count (void) const
{
  return this->coworkers_;
}
