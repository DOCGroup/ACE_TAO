//$id$
#include "tao/GIOP_Message_1_1.h"
#include "tao/GIOPC.h"
#include "tao/Profile.h"
#include "tao/Stub.h"
#include "tao/Object_KeyC.h"
#include "tao/Principal.h"

//$Id$

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_1_1.i"
#endif /* __ACE_INLINE__ */

TAO_GIOP_Message_1_1::TAO_GIOP_Message_1_1 (void)
{
  //no-op
}


TAO_GIOP_Message_1_1::~TAO_GIOP_Message_1_1 (void)
{
  //no-op
}

CORBA::Boolean
TAO_GIOP_Message_1_1::write_request_header (const IOP::ServiceContextList& svc_ctx,
                                            CORBA::ULong request_id,
                                            CORBA::Octet response_flags,
                                            TAO_Stub *stub,
                                            const CORBA::Short address_disposition,
                                            const char *opname,
                                            TAO_OutputCDR &msg)
{
  // This i sepecific to GIOP 1.1. So put them here
  msg << svc_ctx;
  
  // Let us  call our parent class to check what he can do for
  // us. 
  TAO_GIOP_Message_Factory::write_request_header (svc_ctx,
                                                  request_id,
                                                  response_flags,
                                                  stub,
                                                  address_disposition,
                                                  opname,
                                                  msg);
  
  // In this case we cannot recognise anything other than the Object
  // key as the address disposition variable. But we do a sanity check
  // anyway. 
  if (address_disposition == GIOP::KeyAddr)
    {
      // Put in the object key
      TAO_Profile *profile = 
        stub->profile_in_use ();
      
      TAO_ObjectKey *key = profile->_key ();
      
      msg << *key;
    }
  else
    {
      if (TAO_orbdebug)
        ACE_DEBUG ((LM_DEBUG,
                    "(%N | %l) Unable to handle this request \n"));
      return 0;
    }
  
  msg << opname;
  
  // The principal is not used. So send a null pointer
  static CORBA::Principal_ptr principal = 0;
  msg << principal;
  
  
  return 1;

}


CORBA::Boolean
TAO_GIOP_Message_1_1::
write_locate_request_header (CORBA::ULong request_id,
                             TAO_Stub *stub,
                             const CORBA::Short address_disposition,
                             TAO_OutputCDR &msg)
{
  msg << request_id;

    // In this case we cannot recognise anything other than the Object
  // key as the address disposition variable. But we do a sanity check
  // anyway. 
  if (address_disposition == GIOP::KeyAddr)
    {
      // Put in the object key
      TAO_Profile *profile = 
        stub->profile_in_use ();
      
      TAO_ObjectKey *key = profile->_key ();

      // Everything is fine
      msg << *key;
    }
  else
    {
      if (TAO_orbdebug)
        ACE_DEBUG ((LM_DEBUG,
                    "(%N | %l) Unable to handle this request \n"));
      return 0;
    }
  
  return 1;
}


const size_t
TAO_GIOP_Message_1_1::get_header_len (void)
{
  return TAO_GIOP_1_1_HEADER_LEN;
}

const size_t
TAO_GIOP_Message_1_1::get_message_size_offset (void)
{
  return TAO_GIOP_1_1_MESSAGE_SIZE_OFFSET;
}
