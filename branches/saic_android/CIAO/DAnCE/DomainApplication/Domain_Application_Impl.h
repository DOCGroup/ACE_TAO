// $Id$
/**========================================================
 *
 * @file   Domain_Application_Impl.h
 *
 * @Brief  This file contains the implementation of
 *         the DomainApplication interface.
 *
 * @author Vinzenz Tornow <vt@prismtech.com>
 *========================================================*/

#ifndef DOMAIN_APPLICATION_H
#define DOMAIN_APPLICATION_H
#include /**/ "ace/pre.h"

#include "Domain_Application_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Deployment/Deployment_DomainApplicationS.h"
#include "ace/Map_Manager.h"
#include "Deployment/Deployment_NodeApplicationC.h"
#include "Deployment/Deployment_NodeApplicationManagerC.h"
#include "Deployment/Deployment_NodeManagerC.h"

namespace DAnCE
  {

  /**
   *
   * @class DomainApplication_Impl
   *
   * @brief This class implements the DomainApplication interface.
   *
   **/
  class Domain_Application_Export DomainApplication_Impl
        : public virtual POA_Deployment::DomainApplication
    {
    public:
      typedef ACE_Map_Manager < Deployment::NodeApplicationManager_var,
        Deployment::NodeManager_var,
        ACE_Null_Mutex > TNam2Nm;
      typedef ACE_Map_Manager < Deployment::Application_var,
        Deployment::NodeApplicationManager_var,
        ACE_Null_Mutex > TApp2Mgr;

      DomainApplication_Impl (TNam2Nm& nams,
                              const Deployment::Properties & configProperty,
                              Deployment::Connections& conn);
      ~DomainApplication_Impl();

      /**
      * The second step in launching an application in the
      * domain-level.  If the start parameter is true, the application
      * is started as well.  Raises the StartError exception if
      * launching or starting the application fails.
      */
      virtual void
      finishLaunch (const Deployment::Connections & connections,
                    CORBA::Boolean start);
      /**
      * Starts the application. Raises the StartError exception if
      * starting the application fails.
      */
      virtual void start ();

    private:
      TApp2Mgr node_applications_;
      Deployment::Connections connections_;
      TNam2Nm & nams_;
      Deployment::Application_var na_;
      void startLaunch (const Deployment::Properties & configProperty);
    };
} // DAnCE

#include /**/ "ace/post.h"
#endif // DOMAIN_APPLICATION_H


