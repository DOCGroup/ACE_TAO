//$Id$:
/**
 *
 * @file NodeApplication_Impl.h
 *
 * @auther Tao Lu <lu@dre.vanderbilt.edu>
 *
 **/

#ifndef NODEAPPLICATION_IMPL_H
#define NODEAPPLICATION_IMPL_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "DnC_DeploymentS.h"

namespace Deployment
{

  /**
   *
   * @class NodeApplication_Impl
   *
   * @brief This class implements the NodeApplication interface.
   * This interface is simillar to the old DnC CIAO_Daemon.
   *
   **/
  class NodeApplication_Impl
    : public virtual POA_Deployment::NodeApplication
    {
    public:

      // constructor.
      NodeApplication (NodeApplicationManager_ptr nam_ptr,
		       ACE_ENV_ARG_DECL_WITH_DEFAULTS);

      // Default destructor.
      virtual ~NodeApplication (void);

      /**
       * @method finishLaunch
       *
       * @brief
       *
       */
      virtual void finishLaunch (const Deployment::Connections & providedReference,
				 CORBA::Boolean start
				 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
	ACE_THROW_SPEC ((CORBA::SystemException,
			 Deployment::StartError,
			 Deployment::InvalidConnection
			 ));

      /**
       * @method start
       *
       * @brief
       *
       */
      virtual void start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
	ACE_THROW_SPEC ((CORBA::SystemException,
			 Deployment::StartError
			 ));

    protected:
      // Cached Manager
      NodeApplicationManager_var node_application_manager_;

      // Cached ComponentServer
      // Note: Need to deal with the namespace refactoring.
      ComponentServer_ptr component_server_;

      // Cached ComponentInstallation
      ComponentInstallation_ptr component_installation_;

      // Cached ServerActivator
      ServerActivator_ptr server_activator_;

    }; /* class NodeApplication_Impl */


}; /* namespace Deployment */

#endif /* NODEAPPLICATION_IMPL_H */
