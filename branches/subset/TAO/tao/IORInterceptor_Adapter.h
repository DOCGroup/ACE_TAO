// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    IORInterceptor_Adapter.h
 *
 *  $Id$
 *
 *  @author  Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_IORINTERCEPTOR_ADAPTER_H
#define TAO_IORINTERCEPTOR_ADAPTER_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Interceptor_List.h"
#include "ace/Service_Object.h"
#include "ace/CORBA_macros.h"

namespace PortableInterceptor
{
  class Interceptor;
  typedef Interceptor *Interceptor_ptr;
  class IORInterceptor;
  typedef IORInterceptor *IORInterceptor_ptr;
};

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

  virtual void tao_iorinterceptor_release (
      PortableInterceptor::IORInterceptor_ptr
    ) = 0;

  virtual PortableInterceptor::Interceptor_ptr tao_iorinterceptor (
      TAO_IORInterceptor_List::TYPE & container,
      size_t index
    ) = 0;

  virtual void tao_add_iorinterceptor (
      TAO_IORInterceptor_List * list,
      TAO_IORInterceptor_List::TYPE & container,
      PortableInterceptor::IORInterceptor_ptr interceptor
      ACE_ENV_ARG_DECL
    ) = 0;

  virtual void tao_iorinterceptor_destroy (
      TAO_IORInterceptor_List::TYPE & container,
      size_t ilen
      ACE_ENV_ARG_DECL
    ) = 0;
};

#include "ace/post.h"
#endif /* TAO_IORINTERCEPTOR_ADAPTER_H */
