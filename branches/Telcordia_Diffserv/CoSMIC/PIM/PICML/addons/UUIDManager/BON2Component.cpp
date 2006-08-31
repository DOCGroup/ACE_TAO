//###############################################################################################################################################
//
//  Meta and Builder Object Network V2.0 for GME
//  BON2Component.cpp
//
//###############################################################################################################################################

/*
  Copyright (c) Vanderbilt University, 2000-2004
  ALL RIGHTS RESERVED

  Vanderbilt University disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall Vanderbilt University be liable for
  any special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#include "StdAfx.h"
#include "BON2Component.h"
#include "PICML/Utils.h"

namespace BON
{

//###############################################################################################################################################
//
//   C L A S S : BON::Component
//
//###############################################################################################################################################

Component::Component (void)
: m_bIsInteractive (false)
{

}

Component::~Component(void)
{
  if (m_project) 
  {
    m_project->finalizeObjects ();
    finalize( m_project );
    m_project = NULL;
  }
}

// ====================================================
// This method is called after all the generic initialization is done
// This should be empty unless application-specific initialization is needed

void Component::initialize( Project& project )
{
  // Turn off the auto commit for the project while using the
  // add-on. This prevents exceptions from occurring.
  //project->setAutoCommit (false);
}

// ====================================================
// This method is called before the whole BON2 project released and disposed
// This should be empty unless application-specific finalization is needed

void Component::finalize( Project& project )
{

}

// ====================================================
// This is the obsolete component interface
// This present implementation either tries to call InvokeEx, or does nothing except of a notification

void Component::invoke( Project& project, const std::set<FCO>& setModels, long lParam )
{

}

// ====================================================
// This is the main component method for Interpereters and Plugins.
// May also be used in case of invokeable Add-Ons

void Component::invokeEx( Project& project, FCO& currentFCO, const std::set<FCO>& setSelectedFCOs, long lParam )
{

}

// ====================================================
// GME currently does not use this function
// You only need to implement it if other invokation mechanisms are used

void Component::objectInvokeEx( Project& project, Object& currentObject, const std::set<Object>& setSelectedObjects, long lParam )
{

}

// ====================================================
// Implement application specific parameter-mechanism in these functions

Util::Variant Component::getParameter( const std::string& strName )
{
  return Util::Variant();
}

void Component::setParameter( const std::string& strName, const Util::Variant& varValue )
{

}

#ifdef GME_ADDON

// ====================================================
// If the component is an Add-On, then this method is called for every Global Event

void Component::globalEventPerformed( globalevent_enum event )
{

}

// ====================================================
// If the component is an Add-On, then this method is called for every Object Event

void Component::objectEventPerformed( Object& object, unsigned long event, VARIANT v )
{
  //try
  //{
  //  // Get the name of the object type. Only monitor <Component> 
  //  // and <ComponentAssembly> objects.
  //  std::string name = object->getObjectMeta ().name ();

  //  if (name == "Component" || name == "ComponentAssembly")
  //  {
  //    BON::FCO fco = object;
  //    BON::Attribute uuid = fco->getAttribute ("UUID");

  //    if (uuid)
  //    {
  //      // We only handle the creation events at this stage.
  //      if (event == MON::OET_ObjectCreated)
  //      {
  //        // Create an UUID and add the <uuid_manager_> event
  //        // listener to the object.
  //        uuid->setStringValue (PICML::CreateUuid ());     
  //      }
  //      else if (event == MON::OET_AttributeChanged)
  //      {
  //        // Validate the UUID of the object.
  //        if (!PICML::ValidUuid (uuid->getStringValue ()))
  //        {
  //          uuid->setStringValue (PICML::CreateUuid ());
  //        }
  //      }
  //    }
  //  }
  //}
  //catch (...)
  //{
  //  AfxMessageBox ("Gotcha");
  //}
}

#endif // GME_ADDON

}; // namespace BON

