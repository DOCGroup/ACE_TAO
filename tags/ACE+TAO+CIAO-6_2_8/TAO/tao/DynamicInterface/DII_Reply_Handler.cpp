// $Id$

#include "Request.h"
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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// The pseudo-object _nil method.
TAO_DII_Reply_Handler_ptr
TAO_DII_Reply_Handler::_nil (void)
{
  return static_cast<TAO_DII_Reply_Handler_ptr> (0);
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
TAO_DII_Reply_Handler::_narrow (CORBA::Object_ptr obj)
{
  return TAO_DII_Reply_Handler::_unchecked_narrow (obj);
}

TAO_DII_Reply_Handler_ptr
TAO_DII_Reply_Handler::_unchecked_narrow (CORBA::Object_ptr obj)
{
  if (CORBA::is_nil (obj))
    return TAO_DII_Reply_Handler::_nil ();

  return
    reinterpret_cast<TAO_DII_Reply_Handler_ptr> (
        &TAO_DII_Reply_Handler::_tao_class_id);
}

void
TAO_DII_Reply_Handler::handle_location_forward (TAO_InputCDR &incoming,
                                                CORBA::ULong reply_status)
{
  if (TAO_debug_level > 3)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) Base DII_Reply_Handler::")
                  ACE_TEXT("handle_location_forward called, ")
                  ACE_TEXT("reply_status = %d\n"),
                  reply_status));
    }
  this->handle_response (incoming);
}

TAO_END_VERSIONED_NAMESPACE_DECL
