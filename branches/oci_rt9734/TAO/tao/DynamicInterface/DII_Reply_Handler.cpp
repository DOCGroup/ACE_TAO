#include "DynamicInterface/dynamicinterface_pch.h"
#include "Request.h"
// $Id$

ACE_RCSID(DynamicInterface, Request, "$Id$")

#include "ExceptionList.h"
#include "DII_Invocation.h"

#if defined (TAO_HAS_AMI)
#include "DII_Reply_Handler.h"
#endif /* TAO_HAS_AMI */

#include "tao/Object.h"
#include "tao/Pluggable_Messaging_Utils.h"

#if !defined (__ACE_INLINE__)
# include "DII_Reply_Handler.inl"
#endif /* ! __ACE_INLINE__ */


// The pseudo-object _nil method.
TAO_DII_Reply_Handler_ptr
TAO_DII_Reply_Handler::_nil (void)
{
  return (TAO_DII_Reply_Handler_ptr)0;
}

// DII Request class implementation

int TAO_DII_Reply_Handler::_tao_class_id = 0;
TAO_DII_Reply_Handler::TAO_DII_Reply_Handler ()
{
}

TAO_DII_Reply_Handler::~TAO_DII_Reply_Handler (void)
{
}

TAO_DII_Reply_Handler_ptr
TAO_DII_Reply_Handler::_narrow (CORBA::Object_ptr obj
                                ACE_ENV_ARG_DECL)
{
  return TAO_DII_Reply_Handler::_unchecked_narrow (obj
                                                   ACE_ENV_ARG_PARAMETER);
}

TAO_DII_Reply_Handler_ptr
TAO_DII_Reply_Handler::_unchecked_narrow (CORBA::Object_ptr obj
                                ACE_ENV_ARG_DECL)
{
  if (CORBA::is_nil (obj))
    return TAO_DII_Reply_Handler::_nil ();

  return
    ACE_reinterpret_cast (TAO_DII_Reply_Handler_ptr,
                            &TAO_DII_Reply_Handler::_tao_class_id);
}

