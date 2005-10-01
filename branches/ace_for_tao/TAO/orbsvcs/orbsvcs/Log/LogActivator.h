/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   LogActivator.h
 *
 *  $Id$
 *
 */
//=============================================================================

#ifndef TAO_TLS_BASICLOGACTIVATOR_H
#define TAO_TLS_BASICLOGACTIVATOR_H

#include /**/ "ace/pre.h"

#include "orbsvcs/DsLogAdminC.h"

#if (TAO_HAS_MINIMUM_POA == 0)

#include "log_serv_export.h"

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantActivatorC.h"

// forward declarations
class TAO_LogMgr_i;

/// @class TAO_LogActivator
/// @brief Log Servant Activator
///
/// A servant activator to create Log servants.  Permits servants to 
/// be "lazily" created on demand.  This allows persistent logging
/// strategies to avoid creating servants for each log when the
/// service is started.
///
class TAO_Log_Serv_Export TAO_LogActivator
  : public virtual PortableServer::ServantActivator
{
public:
  /// @brief Constructor
  ///
  TAO_LogActivator(TAO_LogMgr_i& logmgr_i);

  /// @brief Destructor
  ///
  virtual ~TAO_LogActivator();

  virtual PortableServer::Servant incarnate (const PortableServer::ObjectId& oid,
					     PortableServer::POA_ptr poa
					     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
		     CORBA::SystemException,
		     PortableServer::ForwardRequest
		     ));

  virtual void etherealize (const PortableServer::ObjectId& oid,
			    PortableServer::POA_ptr poa,
			    PortableServer::Servant servant,
			    CORBA::Boolean cleanup_in_progress,
			    CORBA::Boolean remaining_activations
			    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
		     CORBA::SystemException
		     ));

private:
  TAO_LogMgr_i&			logmgr_i_;
};

#endif /* TAO_HAS_MINIMUM_POA */

#include /**/ "ace/post.h"
#endif
