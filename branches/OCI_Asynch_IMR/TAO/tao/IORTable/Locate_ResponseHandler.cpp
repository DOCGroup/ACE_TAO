// -*- C++ -*-
//=============================================================================
/**
 *  @file    Locate_ResponseHandler.cpp
 *
 *  $Id$
 *
 *  @author Phil Mesnier <mesnier_p@ociweb>
 */
//=============================================================================

#include "Locate_ResponseHandler.h"
#if 0
#include "tao/DynamicInterface/Request.h"
#include "tao/DynamicInterface/ExceptionList.h"
#include "tao/DynamicInterface/DII_Invocation.h"
#include "tao/DynamicInterface/Server_Request.h"
#endif

#include "tao/Exception.h"
#include "tao/ORB_Core.h"
#include "tao/GIOP_Utils.h"
#include "tao/PortableServer/Root_POA.h"
//#include "tao/Valuetype/ValueFactory.h"
#include "tao/Object.h"
#include "tao/Pluggable_Messaging_Utils.h"
//#include "tao/AnyTypeCode/Any_Impl.h"

#if !defined (__ACE_INLINE__)
# include "Locate_ResponseHandler.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#if 0
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
#endif

// The pseudo-object _nil method.
TAO_AMH_Locate_ResponseHandler_ptr
TAO_AMH_Locate_ResponseHandler::_nil (void)
{
  return (TAO_AMH_Locate_ResponseHandler_ptr) 0;
}


TAO_AMH_Locate_ResponseHandler::TAO_AMH_Locate_ResponseHandler (
    TAO_ServerRequest & sr)
  : TAO_AMH_Response_Handler ()
{
  this->init (sr,0);
}

TAO_AMH_Locate_ResponseHandler::~TAO_AMH_Locate_ResponseHandler (void)
{
}

void
TAO_AMH_Locate_ResponseHandler::raise_excep (const CORBA::Exception &ex)
{
  this->_tao_rh_send_exception (ex);
}

void
TAO_AMH_Locate_ResponseHandler::forward_ior (const char *ior, CORBA::Boolean is_perm)
{
  CORBA::ORB_var orb = this->orb_core_->orb();
  CORBA::Object_var fwd = orb->string_to_object (ior);
  this->_tao_rh_send_location_forward (fwd, is_perm);
}


void *
TAO_AMH_Locate_ResponseHandler::_downcast (const char *)
{
  return this;
}

TAO_AMH_Locate_ResponseHandler_ptr
TAO_AMH_Locate_ResponseHandler::_narrow (CORBA::Object_ptr obj)
{
  return TAO_AMH_Locate_ResponseHandler::_unchecked_narrow (obj);
}

TAO_AMH_Locate_ResponseHandler_ptr
TAO_AMH_Locate_ResponseHandler::_unchecked_narrow (CORBA::Object_ptr obj)
{
  if (CORBA::is_nil (obj))
    return TAO_AMH_Locate_ResponseHandler::_nil ();

  return dynamic_cast < TAO_AMH_Locate_ResponseHandler_ptr > (obj);
}

const char*
TAO_AMH_Locate_ResponseHandler::_interface_repository_id (void) const
{
  return 0;
}

#if 0

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

#endif

TAO_END_VERSIONED_NAMESPACE_DECL
