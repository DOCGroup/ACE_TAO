// $Id$
#ifndef REDIRECTIONSERVICE_H_
#define REDIRECTIONSERVICE_H_

#include "ace/Map_Manager.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"

#include "RedirectionService_Export.h"

#include "CCMObjectLocator.h"
#include "NameServiceRedirection.h"

/*
  This class encapsulates redirection mechanisms for dance offline deployment.
  It contains NameService and ServantLocator classes with correspondent poas
  and hides from NodeApplication all specific steps required for supporting redirection
  If no NameService nor ServantLocator is not required when registration methods do nothing.
*/
namespace DAnCE
  {
  class RedirectionService_Export RedirectionService
    {
    public:
      /// Constructor for redirection service object
      /// In general, redirection service should be singleton in scope of dance agent process
      /// but probably it is not compulsory
      /// parameter hosting_context is compulsory if ns is true
      /// parameter domain_context should be specified if we need to bind local contextes to external context
      /// parameter ns should be passed as true if NameService redirection mechanism should be used
      /// parameter sl should be passed as true if ServantLocator redirection should be used
      /// parameter poa should be root poa (? probably this parameter is unnecessary and this class can obtains from orb)
      RedirectionService (CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa,
                          CosNaming::NamingContext_ptr hosting_context,
                          CosNaming::NamingContext_ptr domain_context,
                          bool ns = false,
                          bool sl = false);

      ~RedirectionService ();
      ///  Creates record in redirection service for correspondent node
      void add_node (const ACE_CString& node);
      ///  Opens transaction for registering node ccm objects.
      ///  Parameters node and plan are the keys
      void registration_start (const ACE_CString& node, const ACE_CString& plan);
      ///  Registration for port object
      void registration (const ACE_CString& node, const ACE_CString& plan, const ACE_CString& component, const ACE_CString& port, CORBA::Object_ptr obj);
      ///  Registration for component object
      void registration (const ACE_CString& node, const ACE_CString& plan, const ACE_CString& component, CORBA::Object_ptr obj);
      ///  Closes transaction for registering node ccm objects
      ///  and insert node to correspondent ServantLocator and/or NameService
      ///  Parameters node and plan are the keys
      void registration_finish (const ACE_CString& node, const ACE_CString& plan);
      // Removes all records for specified plan
      void unregister (const ACE_CString& node, const ACE_CString& plan);
    private:
      /// Orb variable
      CORBA::ORB_var orb_;

      /// Root POA variable
      PortableServer::POA_var   poa_;

      NameServiceRedirection naming_;

      bool ns_;

      bool sl_;

      /// Map of servant locators (node name is a key)
      typedef ACE_Map_Manager<ACE_CString, DAnCE::CCMObjectLocator*,  ACE_Null_Mutex> TLocators;
      TLocators locators_;
    };
}

#endif /*REDIRECTIONSERVICE_H_*/
