// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    IORInterceptor_Adapter.h
 *
 *  $Id$
 *
 *  @author  Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IORINTERCEPTOR_ADAPTER_H
#define TAO_IORINTERCEPTOR_ADAPTER_H
#include /**/ "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Exception.h"
#include "ace/Service_Object.h"
#include "ace/CORBA_macros.h"

namespace PortableInterceptor
{
  class Interceptor;
  typedef Interceptor *Interceptor_ptr;
  class IORInterceptor;
  typedef IORInterceptor *IORInterceptor_ptr;
};

class TAO_IORInterceptor_List;

/**
 * @class TAO_IORInterceptor_Adapter
 *
 * @brief TAO_IORInterceptor_Adapter.
 *
 * Class that adapts various functions involving the PortableInterceptor
 * interfaces IORInfo and IORInterceptor. This is a base class for
 * the actual implementation in the TAO_IORInterceptor library.
 */
class TAO_Export TAO_IORInterceptor_Adapter : public ACE_Service_Object
{
public:
  virtual ~TAO_IORInterceptor_Adapter (void);

  virtual void add_interceptor (
      PortableInterceptor::IORInterceptor_ptr interceptor
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

  virtual void destroy_interceptors (
      ACE_ENV_SINGLE_ARG_DECL
      )
  ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

  virtual TAO_IORInterceptor_List *interceptor_list (void)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

};

#include /**/ "ace/post.h"
#endif /* TAO_IORINTERCEPTOR_ADAPTER_H */
