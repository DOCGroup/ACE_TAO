// -*- C++ -*-

//=============================================================================
/**
 *  @file    DII_ClientRequestInfo.h
 *
 *  $Id$
 *
 *  @author  Aaron Nielsen <nielsa@clcsnet.ksc.nasa.gov>
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_DII_CLIENT_REQUEST_INFO_H
#define TAO_DII_CLIENT_REQUEST_INFO_H

#include "ace/pre.h"

#include "tao/ClientRequestInfo_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/DynamicInterface/Request.h"


/**
 * @class TAO_DII_ClientRequestInfo
 *
 * @brief DII-specific PortableInterceptor::ClientRequestInfo
 *        implementation.
 *
 * This class is a DII-specific concrete implementation of the
 * PortableInterceptor::ClientRequestInfo interface.
 */
class TAO_DII_ClientRequestInfo : public TAO_ClientRequestInfo_i
{
public:

  /// Constructor
  TAO_DII_ClientRequestInfo (TAO_GIOP_Invocation *_tao_invocation,
                             CORBA::Object_ptr _tao_target,
                             CORBA::Request_ptr request);

  virtual Dynamic::ParameterList * arguments (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::ExceptionList * exceptions (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Any * result (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char * operation (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));


  void result (CORBA::Long result);

private:

  // Prevent copying.
  TAO_DII_ClientRequestInfo (const TAO_DII_ClientRequestInfo &);
  void operator= (const TAO_DII_ClientRequestInfo &);

private:

  CORBA::Request_ptr request_;
  CORBA::Long result_;

};

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include "ace/post.h"

#endif  /* TAO_DII_CLIENT_REQUEST_INFO_H */
