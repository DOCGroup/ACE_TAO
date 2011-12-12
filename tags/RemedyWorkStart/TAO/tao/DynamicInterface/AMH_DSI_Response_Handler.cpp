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

#if !defined (__ACE_INLINE__)
# include "AMH_DSI_Response_Handler.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

void
tao_TAO_AMH_DSI_Exception_Holder_add_ref (TAO_AMH_DSI_Exception_Holder * p)
{
  CORBA::add_ref (p);
}

void
tao_TAO_AMH_DSI_Exception_Holder_remove_ref (TAO_AMH_DSI_Exception_Holder * p)
{
  CORBA::remove_ref (p);
}


TAO_AMH_DSI_Exception_Holder_var::TAO_AMH_DSI_Exception_Holder_var (void)
  : ptr_ (0)
{
}

TAO_AMH_DSI_Exception_Holder_var::TAO_AMH_DSI_Exception_Holder_var (
    TAO_AMH_DSI_Exception_Holder * p)
  : ptr_ (p)
{
}

TAO_AMH_DSI_Exception_Holder_var::TAO_AMH_DSI_Exception_Holder_var (
    const TAO_AMH_DSI_Exception_Holder * p)
  : ptr_ (const_cast<TAO_AMH_DSI_Exception_Holder *> (p))
{
}

TAO_AMH_DSI_Exception_Holder *
TAO_AMH_DSI_Exception_Holder_var::ptr (void) const
{
  return this->ptr_;
}

TAO_AMH_DSI_Exception_Holder_var::TAO_AMH_DSI_Exception_Holder_var (
    const TAO_AMH_DSI_Exception_Holder_var & p)
{
  CORBA::add_ref (p.ptr ());
  this->ptr_ = p.ptr ();
}

TAO_AMH_DSI_Exception_Holder_var::~TAO_AMH_DSI_Exception_Holder_var (void)
{
  CORBA::remove_ref (this->ptr_);
}

TAO_AMH_DSI_Exception_Holder_var &
TAO_AMH_DSI_Exception_Holder_var::operator= (TAO_AMH_DSI_Exception_Holder *p)
{
  CORBA::remove_ref (this->ptr_);
  this->ptr_ = p;
  return *this;
}

TAO_AMH_DSI_Exception_Holder_var &
TAO_AMH_DSI_Exception_Holder_var::operator= (
    const TAO_AMH_DSI_Exception_Holder_var & p)
{
  if (this != &p)
    {
      CORBA::remove_ref (this->ptr_);
      TAO_AMH_DSI_Exception_Holder *tmp = p.ptr ();
      CORBA::add_ref (tmp);
      this->ptr_ = tmp;
    }

  return *this;
}

TAO_AMH_DSI_Exception_Holder_var::operator const TAO_AMH_DSI_Exception_Holder* (
    ) const // cast
{
  return this->ptr_;
}

TAO_AMH_DSI_Exception_Holder_var::operator TAO_AMH_DSI_Exception_Holder*& ()
{
  return this->ptr_;
}

TAO_AMH_DSI_Exception_Holder *
TAO_AMH_DSI_Exception_Holder_var::operator-> (void) const
{
  return this->ptr_;
}

TAO_AMH_DSI_Exception_Holder *
TAO_AMH_DSI_Exception_Holder_var::in (void) const
{
  return this->ptr_;
}

TAO_AMH_DSI_Exception_Holder *&
TAO_AMH_DSI_Exception_Holder_var::inout (void)
{
  return this->ptr_;
}

TAO_AMH_DSI_Exception_Holder *&
TAO_AMH_DSI_Exception_Holder_var::out (void)
{
  CORBA::remove_ref (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

TAO_AMH_DSI_Exception_Holder *
TAO_AMH_DSI_Exception_Holder_var::_retn (void)
{
  // yield ownership of managed obj reference
  TAO_AMH_DSI_Exception_Holder* tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

void
TAO_AMH_DSI_Exception_Holder_var::tao_add_ref (TAO_AMH_DSI_Exception_Holder *p)
{
  CORBA::add_ref (p);
}

void
TAO_AMH_DSI_Exception_Holder_var::tao_remove_ref (
    TAO_AMH_DSI_Exception_Holder *p)
{
  CORBA::remove_ref (p);
}

TAO_AMH_DSI_Exception_Holder_out::TAO_AMH_DSI_Exception_Holder_out (
    TAO_AMH_DSI_Exception_Holder * &p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

TAO_AMH_DSI_Exception_Holder_out::TAO_AMH_DSI_Exception_Holder_out (
     TAO_AMH_DSI_Exception_Holder_var & p)
  : ptr_ (p.out ())
{
  CORBA::remove_ref (this->ptr_);
  this->ptr_ = 0;
}

TAO_AMH_DSI_Exception_Holder_out::TAO_AMH_DSI_Exception_Holder_out (
    const TAO_AMH_DSI_Exception_Holder_out &p)
  : ptr_ (const_cast<TAO_AMH_DSI_Exception_Holder_out&> (p).ptr_)
{}

TAO_AMH_DSI_Exception_Holder_out &
TAO_AMH_DSI_Exception_Holder_out::operator= (
    const TAO_AMH_DSI_Exception_Holder_out &p)
{
  this->ptr_ = const_cast<TAO_AMH_DSI_Exception_Holder_out &> (p).ptr_;
  return *this;
}

TAO_AMH_DSI_Exception_Holder_out &
TAO_AMH_DSI_Exception_Holder_out::operator= (
    const TAO_AMH_DSI_Exception_Holder_var &p)
{
  TAO_AMH_DSI_Exception_Holder *tmp = p.ptr ();
  CORBA::add_ref (tmp);
  this->ptr_ = tmp;
  return *this;
}

TAO_AMH_DSI_Exception_Holder_out &
TAO_AMH_DSI_Exception_Holder_out::operator= (TAO_AMH_DSI_Exception_Holder *p)
{
  this->ptr_ = p;
  return *this;
}

TAO_AMH_DSI_Exception_Holder_out::operator TAO_AMH_DSI_Exception_Holder*& ()
{
  return this->ptr_;
}

TAO_AMH_DSI_Exception_Holder *&
TAO_AMH_DSI_Exception_Holder_out::ptr (void) // ptr
{
  return this->ptr_;
}

TAO_AMH_DSI_Exception_Holder *
TAO_AMH_DSI_Exception_Holder_out::operator-> (void)
{
  return this->ptr_;
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
    TAO_AMH_DSI_Exception_Holder *&new_object)
{
  ::CORBA::ValueBase *base = 0;
  ::CORBA::Boolean is_indirected = 0;
  ::CORBA::Boolean is_null_object = 0;
  ::CORBA::Boolean const retval =
    ::CORBA::ValueBase::_tao_unmarshal_pre (
        strm,
        base,
        TAO_AMH_DSI_Exception_Holder::_tao_obv_static_repository_id (),
        is_null_object,
        is_indirected
      );

  ::CORBA::ValueBase_var owner (base);

  if (!retval)
    return 0;

  if (is_null_object)
    return 1;

  if (!is_indirected && base != 0 && ! base->_tao_unmarshal_v (strm))
    return 0;


  // Now base must be null or point to the unmarshaled object.
  // Align the pointer to the right subobject.
  new_object = TAO_AMH_DSI_Exception_Holder::_downcast (base);
  if (is_indirected)
    new_object->_add_ref ();

  owner._retn ();
  return 1;
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
TAO_AMH_DSI_Response_Handler::invoke_location_forward (CORBA::Object_ptr fwd,
                                                       CORBA::Boolean is_perm)
{
  this->_tao_rh_send_location_forward (fwd,is_perm);
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
