// -*- C++ -*-

//=============================================================================

/**
 * @file AMI_ClientRequestInfo_i.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
//=============================================================================


#ifndef TAO_MESSAGING_AMI_CLIENTREQUESTINFO_I_H
#define TAO_MESSAGING_AMI_CLIENTREQUESTINFO_I_H

#include "ace/pre.h"

#include "messaging_export.h"
#include "tao/ClientRequestInfo_i.h"

#if (TAO_HAS_INTERCEPTORS == 1)

class TAO_GIOP_Twoway_Asynch_Invocation;


class TAO_Messaging_Export TAO_AMI_ClientRequestInfo_i
  : public TAO_ClientRequestInfo_i
{
public:
  TAO_AMI_ClientRequestInfo_i (TAO_GIOP_Twoway_Asynch_Invocation *,
                               CORBA::Object_ptr target);

  virtual Dynamic::ParameterList * arguments (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual Dynamic::ExceptionList * exceptions (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual CORBA::Any * result (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#endif /* TAO_HAS_INTERCEPTORS == 1 */

#include "ace/post.h"

#endif /* TAO_MESSAGING_AMI_CLIENTREQUESTINFO_I_H */
