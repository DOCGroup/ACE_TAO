// $Id$

#include "AMH_DSI_Response_Handler.h"
#include "Request.h"
#include "ExceptionList.h"
#include "DII_Invocation.h"
#include "Server_Request.h"

#include "tao/ORB_Core.h"
#include "tao/GIOP_Utils.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/Valuetype/ValueFactory.h"
#include "tao/Object.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/AnyTypeCode/Any_Impl.h"

ACE_RCSID (DynamicInterface, AMH_DSI_Response_Handler,
           "$Id$")

#if !defined (__ACE_INLINE__)
# include "AMH_DSI_Response_Handler.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_AMH_DSI_Response_Handler_ptr
TAO::Objref_Traits<TAO_AMH_DSI_Response_Handler>::duplicate (
    TAO_AMH_DSI_Response_Handler_ptr p)
{
  return TAO_AMH_DSI_Response_Handler::_duplicate (p);
}

void
TAO::Objref_Traits<TAO_AMH_DSI_Response_Handler>::release (
    TAO_AMH_DSI_Response_Handler_ptr p)
{
  ::CORBA::release (p);
}

TAO_AMH_DSI_Response_Handler_ptr
TAO::Objref_Traits<TAO_AMH_DSI_Response_Handler>::nil (void)
{
  return TAO_AMH_DSI_Response_Handler::_nil ();
}

::CORBA::Boolean
TAO::Objref_Traits<TAO_AMH_DSI_Response_Handler>::marshal (
    const TAO_AMH_DSI_Response_Handler_ptr p,
    TAO_OutputCDR & cdr)
{
  return ::CORBA::Object::marshal (p, cdr);
}


const char*
TAO_AMH_DSI_Exception_Holder::_tao_obv_static_repository_id ()
{
  return "IDL:TAO_AMH_DSI_Exception_Holder:1.0";
}

TAO_AMH_DSI_Exception_Holder::TAO_AMH_DSI_Exception_Holder (CORBA::Exception *ex)
  : exception_ (ex)
{
}

TAO_AMH_DSI_Exception_Holder::~TAO_AMH_DSI_Exception_Holder ()
{
}

TAO_AMH_DSI_Exception_Holder *
TAO_AMH_DSI_Exception_Holder::_downcast (CORBA::ValueBase * v)
{
  return dynamic_cast < TAO_AMH_DSI_Exception_Holder * >(v);
}

const char *
TAO_AMH_DSI_Exception_Holder::_tao_obv_repository_id (void) const
{
  return this->_tao_obv_static_repository_id ();
}

void
TAO_AMH_DSI_Exception_Holder::_tao_any_destructor (void *_tao_void_pointer)
{
  TAO_AMH_DSI_Exception_Holder *tmp =
    static_cast<TAO_AMH_DSI_Exception_Holder *> (_tao_void_pointer);
  delete tmp;
}

CORBA::Boolean
TAO_AMH_DSI_Exception_Holder::_tao_unmarshal (
    TAO_InputCDR &strm,
    TAO_AMH_DSI_Exception_Holder *&new_object
  )
{
  CORBA::ValueBase *base = 0;
  CORBA::ValueFactory_var factory;
  CORBA::Boolean retval =
    CORBA::ValueBase::_tao_unmarshal_pre (
        strm,
        base,
        TAO_AMH_DSI_Exception_Holder::_tao_obv_static_repository_id ()
      );

  if (retval == 0)
    {
      return false;
    }

  if (factory.in () != 0)
    {
      base = factory->create_for_unmarshal ();

      if (base == 0)
        {
          return false;  // %! except.?
        }

      retval = base->_tao_unmarshal_v (strm);

      if (retval == 0)
        {
          return false;
        }
    }

  // Now base must be null or point to the unmarshaled object.
  // Align the pointer to the right subobject.
  new_object = TAO_AMH_DSI_Exception_Holder::_downcast (base);
  return retval;
}

void
TAO_AMH_DSI_Exception_Holder::raise_invoke ()
{
  this->exception_->_raise ();
}

// The pseudo-object _nil method.
TAO_AMH_DSI_Response_Handler_ptr
TAO_AMH_DSI_Response_Handler::_nil (void)
{
  return (TAO_AMH_DSI_Response_Handler_ptr) 0;
}


TAO_AMH_DSI_Response_Handler::TAO_AMH_DSI_Response_Handler (
    TAO_ServerRequest & sr)
  : TAO_AMH_Response_Handler (),
    orb_core_ (sr.orb_core ())
{
}

TAO_AMH_DSI_Response_Handler::~TAO_AMH_DSI_Response_Handler (void)
{
}

void
TAO_AMH_DSI_Response_Handler::invoke_reply (CORBA::NVList_ptr args,
                                            CORBA::NamedValue_ptr result)
{
  try
   {
     this->_tao_rh_init_reply ();

    // Send the return value, if any.
    if (result != 0 && result->value () != 0)
      {
        result->value ()->impl ()->marshal_value (this->_tao_out);
      }

    // Send the "inout" and "out" parameters.
    if (args != 0)
      {
        args->_tao_encode (this->_tao_out, CORBA::ARG_INOUT | CORBA::ARG_OUT);
      }
    this->_tao_rh_send_reply ();
  }
  catch (const CORBA::Exception &)
    {
      // TODO:
    }
}

void
TAO_AMH_DSI_Response_Handler::invoke_excep (TAO_AMH_DSI_Exception_Holder * h)
{
  try
    {
      h->raise_invoke ();
    }
  catch (const CORBA::Exception & ex)
    {
      this->_tao_rh_send_exception (ex);
    }
}

void
TAO_AMH_DSI_Response_Handler::gateway_exception_reply (
    CORBA::ULong reply_status,
    TAO_OutputCDR & encap)
{
  // for this to be effective, ACE & TAO must be built with
  // ACE_ENABLE_SWAP_ON_WRITE defined in ace/config.h
  this->_tao_out.reset_byte_order (encap.byte_order ());
  // This reply path handles only user exceptions.
  switch (reply_status)
    {
    case TAO_AMI_REPLY_USER_EXCEPTION:
      this->reply_status_ = GIOP::USER_EXCEPTION;
      break;
    case TAO_AMI_REPLY_SYSTEM_EXCEPTION:
      this->reply_status_ = GIOP::SYSTEM_EXCEPTION;
      break;

      // TODO: we don't handle location forward at this moment.
      // need to be addressed later.
      //
      //case TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD:
      //  this->exception_type_ = TAO_GIOP_LOCATION_FORWARD;
      //  break;
    }
  try
    {
      this->_tao_rh_init_reply ();

      // We know nothing about this exception, so we marshal it as a block
      // of bytes. The outgoing stream's byte order has already been matched
      // to the original source of the reply.
      this->_tao_out.write_char_array (encap.buffer (), encap.length ());
      // This will prevent the marshaling of any parameters into this reply.
      //  this->sent_gateway_exception_ = 1;
      this->_tao_rh_send_reply ();
    }
  catch (const CORBA::Exception &)
    {
      // TODO:
    }

}

void
TAO_AMH_DSI_Response_Handler::gateway_exception_reply (
    CORBA::ULong reply_status,
    TAO_InputCDR &encap)
{
  // for this to be effective, ACE & TAO must be built with
  // ACE_ENABLE_SWAP_ON_WRITE defined in ace/config.h
  this->_tao_out.reset_byte_order(encap.byte_order());
  // This reply path handles only user exceptions.
  switch (reply_status)
    {
    //case TAO_PLUGGABLE_MESSAGE_USER_EXCEPTION:
    case TAO_AMI_REPLY_USER_EXCEPTION:
      this->reply_status_ = GIOP::USER_EXCEPTION;
      break;
    case TAO_AMI_REPLY_SYSTEM_EXCEPTION:
      this->reply_status_ = GIOP::SYSTEM_EXCEPTION;
      break;

    // we don't handle location forward at this moment.
    // need to be addressed later.
    //
    //case TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD:
    //  this->exception_type_ = TAO_GIOP_LOCATION_FORWARD;
    //  break;
    }
  try
  {
    this->_tao_rh_init_reply ();

  // We know nothing about this exception, so we marshal it as a block
  // of bytes. The outgoing stream's byte order has already been matched
  // to the original source of the reply.
    this->_tao_out.write_octet_array_mb (encap.start());
  // This will prevent the marshaling of any parameters into this reply.
  //  this->sent_gateway_exception_ = 1;
    this->_tao_rh_send_reply ();
  }
  catch (const CORBA::Exception &)
  {
    // TODO:
  }

}

void *
TAO_AMH_DSI_Response_Handler::_downcast (const char *)
{
  return this;
}

TAO_AMH_DSI_Response_Handler_ptr
TAO_AMH_DSI_Response_Handler::_narrow (CORBA::Object_ptr obj)
{
  return TAO_AMH_DSI_Response_Handler::_unchecked_narrow (obj);
}

TAO_AMH_DSI_Response_Handler_ptr
TAO_AMH_DSI_Response_Handler::_unchecked_narrow (CORBA::Object_ptr obj)
{
  if (CORBA::is_nil (obj))
    return TAO_AMH_DSI_Response_Handler::_nil ();

  return dynamic_cast < TAO_AMH_DSI_Response_Handler_ptr > (obj);
}

const char*
TAO_AMH_DSI_Response_Handler::_interface_repository_id (void) const
{
  return 0;
}

CORBA::TypeCode_ptr
TAO_AMH_DSI_Exception_Holder::_tao_type (void) const
{
  return 0;
}

void
TAO_AMH_DSI_Exception_Holder::_tao_obv_truncatable_repo_ids (
    Repository_Id_List &) const
{
}

CORBA::Boolean
TAO_AMH_DSI_Exception_Holder::_tao_match_formal_type (ptrdiff_t ) const
{
  return false;
}

CORBA::Boolean
TAO_AMH_DSI_Exception_Holder::_tao_marshal_v (TAO_OutputCDR &) const
{
  return true;
}

CORBA::Boolean
TAO_AMH_DSI_Exception_Holder::_tao_unmarshal_v (TAO_InputCDR &)
{
  return true;
}

CORBA::Boolean
TAO_AMH_DSI_Exception_Holder::_tao_marshal__Test_TAO_AMH_DSI_Exception_Holder (
    TAO_OutputCDR &)
{
  return true;
}

CORBA::Boolean
TAO_AMH_DSI_Exception_Holder::_tao_unmarshal__Test_TAO_AMH_DSI_Exception_Holder (
    TAO_InputCDR &)
{
  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
