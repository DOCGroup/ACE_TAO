// $Id$
#include "Request.h"

ACE_RCSID(DynamicInterface, Request, "$Id$")


#include "ExceptionList.h"
#include "DII_Invocation.h"
#include "Server_Request.h"
#include "tao/ORB_Core.h"
#include "tao/GIOP_Utils.h"
#include "tao/PortableServer/Root_POA.h"
#include "AMH_DSI_Response_Handler.h"

#include "tao/Valuetype/ValueFactory.h"
#include "tao/Object.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/AnyTypeCode/Any_Impl.h"

#if !defined (__ACE_INLINE__)
# include "AMH_DSI_Response_Handler.inl"
#endif /* ! __ACE_INLINE__ */


const char*
TAO_AMH_DSI_Exception_Holder::_tao_obv_static_repository_id ()
{
  return "IDL:TAO_AMH_DSI_Exception_Holder:1.0";
}

TAO_AMH_DSI_Exception_Holder::~TAO_AMH_DSI_Exception_Holder()
{
}

void
tao_TAO_AMH_DSI_Exception_Holder_add_ref (
    TAO_AMH_DSI_Exception_Holder *p
  )
{
  CORBA::add_ref (p);
}

void
tao_TAO_AMH_DSI_Exception_Holder_remove_ref (
    TAO_AMH_DSI_Exception_Holder *p
  )
{
  CORBA::remove_ref (p);
}

// TAO_IDL - Generated from
// be/be_valuetype.cpp:300

// *************************************************************
// Operations for class Test::TAO_AMH_DSI_Exception_Holder_var
// *************************************************************

TAO_AMH_DSI_Exception_Holder_var::TAO_AMH_DSI_Exception_Holder_var (void)
  : ptr_ (0)
{}

TAO_AMH_DSI_Exception_Holder_var::TAO_AMH_DSI_Exception_Holder_var (TAO_AMH_DSI_Exception_Holder* p)
  : ptr_ (p)
{}

TAO_AMH_DSI_Exception_Holder_var::TAO_AMH_DSI_Exception_Holder_var (const TAO_AMH_DSI_Exception_Holder* p)
  : ptr_ (ACE_const_cast (TAO_AMH_DSI_Exception_Holder *, p))
{}

TAO_AMH_DSI_Exception_Holder *
TAO_AMH_DSI_Exception_Holder_var::ptr (void) const
{
  return this->ptr_;
}

TAO_AMH_DSI_Exception_Holder_var::TAO_AMH_DSI_Exception_Holder_var (const TAO_AMH_DSI_Exception_Holder_var &p)
{
  CORBA::add_ref (p.ptr ());
  this->ptr_ = p.ptr ();
}

TAO_AMH_DSI_Exception_Holder_var::~TAO_AMH_DSI_Exception_Holder_var (void) // destructor
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
TAO_AMH_DSI_Exception_Holder_var::operator= (const TAO_AMH_DSI_Exception_Holder_var &p)
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

TAO_AMH_DSI_Exception_Holder_var::operator const TAO_AMH_DSI_Exception_Holder* () const // cast
{
  return this->ptr_;
}

TAO_AMH_DSI_Exception_Holder_var::operator TAO_AMH_DSI_Exception_Holder*& () // cast
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
TAO_AMH_DSI_Exception_Holder_var::tao_add_ref (
    TAO_AMH_DSI_Exception_Holder *p
  )
{
  CORBA::add_ref (p);
}

void
TAO_AMH_DSI_Exception_Holder_var::tao_remove_ref (
    TAO_AMH_DSI_Exception_Holder *p
  )
{
  CORBA::remove_ref (p);
}

// TAO_IDL - Generated from
// be/be_valuetype.cpp:570

// *************************************************************
// Operations for class TAO_AMH_DSI_Exception_Holder_out
// *************************************************************

TAO_AMH_DSI_Exception_Holder_out::TAO_AMH_DSI_Exception_Holder_out (TAO_AMH_DSI_Exception_Holder *&p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

TAO_AMH_DSI_Exception_Holder_out::TAO_AMH_DSI_Exception_Holder_out (TAO_AMH_DSI_Exception_Holder_var &p)
  : ptr_ (p.out ())
{
  CORBA::remove_ref (this->ptr_);
  this->ptr_ = 0;
}

TAO_AMH_DSI_Exception_Holder_out::TAO_AMH_DSI_Exception_Holder_out (const TAO_AMH_DSI_Exception_Holder_out &p)
  : ptr_ (ACE_const_cast (TAO_AMH_DSI_Exception_Holder_out&, p).ptr_)
{}

TAO_AMH_DSI_Exception_Holder_out &
TAO_AMH_DSI_Exception_Holder_out::operator= (const TAO_AMH_DSI_Exception_Holder_out &p)
{
  this->ptr_ = ACE_const_cast (TAO_AMH_DSI_Exception_Holder_out&, p).ptr_;
  return *this;
}

TAO_AMH_DSI_Exception_Holder_out &
TAO_AMH_DSI_Exception_Holder_out::operator= (const TAO_AMH_DSI_Exception_Holder_var &p)
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

TAO_AMH_DSI_Exception_Holder_out::operator TAO_AMH_DSI_Exception_Holder*& () // cast
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

// *************************************************************

// TAO_IDL - Generated from
// be/be_visitor_valuetype/valuetype_cs.cpp:138

TAO_AMH_DSI_Exception_Holder *
TAO_AMH_DSI_Exception_Holder::_downcast (CORBA::ValueBase *v)
{
  if (v == 0)
    {
      return 0;
    }

  return dynamic_cast<TAO_AMH_DSI_Exception_Holder *>(v);
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
    ACE_static_cast (
        TAO_AMH_DSI_Exception_Holder *,
        _tao_void_pointer
      );
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
      return 0;
    }

  if (factory.in () != 0)
    {
      base = factory->create_for_unmarshal ();

      if (base == 0)
        {
          return 0;  // %! except.?
        }

      retval = base->_tao_unmarshal_v (strm);

      if (retval == 0)
        {
          return 0;
        }
    }

  // Now base must be null or point to the unmarshaled object.
  // Align the pointer to the right subobject.
  new_object = TAO_AMH_DSI_Exception_Holder::_downcast (base);
  return retval;
}

void
TAO_AMH_DSI_Exception_Holder::raise_invoke ( ACE_ENV_SINGLE_ARG_DECL )
    ACE_THROW_SPEC (( CORBA::SystemException ))
{
#if defined (TAO_HAS_EXCEPTIONS)
  auto_ptr<CORBA::Exception> safety (this->exception);
  // Direct throw because we don't have the ACE_TRY_ENV.
  this->exception->_raise ();
#else
  // We can not use ACE_THROW here.
  ACE_TRY_ENV.exception (this->exception);
#endif
}

// The pseudo-object _nil method.
TAO_AMH_DSI_Response_Handler_ptr
TAO_AMH_DSI_Response_Handler::_nil (void)
{
  return (TAO_AMH_DSI_Response_Handler_ptr)0;
}

// DII Request class implementation

TAO_AMH_DSI_Response_Handler::TAO_AMH_DSI_Response_Handler (TAO_ServerRequest &sr)
  :TAO_AMH_Response_Handler (),
   exception_type_ (TAO_GIOP_NO_EXCEPTION),
   orb_core_ (sr.orb_core())
{
}

TAO_AMH_DSI_Response_Handler::~TAO_AMH_DSI_Response_Handler (void)
{
}

void
TAO_AMH_DSI_Response_Handler::invoke_reply(CORBA::NVList_ptr args,
                                           CORBA::NamedValue_ptr result
                                           ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      this->_tao_rh_init_reply(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Send the return value, if any.
      if (result != 0 && result->value() != 0)
        {
          result->value()->impl()->marshal_value(this->_tao_out);
          ACE_TRY_CHECK;
        }

      // Send the "inout" and "out" parameters.
      if (args != 0)
        {
          args->_tao_encode (this->_tao_out,
                             CORBA::ARG_INOUT | CORBA::ARG_OUT
                             ACE_ENV_ARG_PARAMETER
                             );
          ACE_TRY_CHECK;
        }
      this->_tao_rh_send_reply (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
      //      request->_tao_server_request().tao_send_reply_exception (ex);
    }
  ACE_ENDTRY;
}

void
TAO_AMH_DSI_Response_Handler::invoke_excep ( TAO_AMH_DSI_Exception_Holder * h
                                             ACE_ENV_ARG_DECL )
  ACE_THROW_SPEC (( CORBA::SystemException ))
{
  ACE_TRY
  {
    h->raise_invoke (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCH (CORBA::Exception, ex)
  {
    this->_tao_rh_send_exception (ex ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }
  ACE_ENDTRY;
}

void
TAO_AMH_DSI_Response_Handler::gateway_exception_reply (CORBA::ULong reply_status,
                                                       TAO_OutputCDR &encap)
{
  // for this to be effective, ACE & TAO must be built with
  // ACE_ENABLE_SWAP_ON_WRITE defined in ace/config.h
  this->_tao_out.reset_byte_order(encap.byte_order());
  // This reply path handles only user exceptions.
  switch (reply_status)
    {
    //case TAO_PLUGGABLE_MESSAGE_USER_EXCEPTION:
    case TAO_AMI_REPLY_USER_EXCEPTION:
      this->exception_type_ = TAO_GIOP_USER_EXCEPTION;
      break;
    case TAO_AMI_REPLY_SYSTEM_EXCEPTION:
      this->exception_type_ = TAO_GIOP_SYSTEM_EXCEPTION;
      break;

    // we don't handle location forward at this moment.
    // need to be addressed later.
    //
    //case TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD:
    //  this->exception_type_ = TAO_GIOP_LOCATION_FORWARD;
    //  break;
    }
  ACE_TRY_NEW_ENV
  {
    this->_tao_rh_init_reply (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

  // We know nothing about this exception, so we marshal it as a block
  // of bytes. The outgoing stream's byte order has already been matched
  // to the original source of the reply.
    this->_tao_out.write_char_array (encap.buffer(), encap.length());
  // This will prevent the marshaling of any parameters into this reply.
  //  this->sent_gateway_exception_ = 1;
    this->_tao_rh_send_reply (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;

}



#if 0
void
TAO_AMH_DSI_Response_Handler::_primary_interface (CORBA::RepositoryId *result
                                   ACE_ENV_ARG_DECL)
{
}

void
TAO_AMH_DSI_Response_Handler::_is_a (CORBA::Boolean result
                                     ACE_ENV_ARG_DECL)
{
}
#endif

void *
TAO_AMH_DSI_Response_Handler::_downcast (const char *)
{
  return this;
}

TAO_AMH_DSI_Response_Handler_ptr
TAO_AMH_DSI_Response_Handler::_narrow (CORBA::Object_ptr obj
                                ACE_ENV_ARG_DECL)
{
  return TAO_AMH_DSI_Response_Handler::_unchecked_narrow (obj
                                                   ACE_ENV_ARG_PARAMETER);
}

TAO_AMH_DSI_Response_Handler_ptr
TAO_AMH_DSI_Response_Handler::_unchecked_narrow (CORBA::Object_ptr obj
                                ACE_ENV_ARG_DECL)
{
  if (CORBA::is_nil (obj))
    return TAO_AMH_DSI_Response_Handler::_nil ();

  return dynamic_cast<TAO_AMH_DSI_Response_Handler_ptr>(obj);
}

const char*
TAO_AMH_DSI_Response_Handler::_interface_repository_id (void) const
{
  return 0;
}
