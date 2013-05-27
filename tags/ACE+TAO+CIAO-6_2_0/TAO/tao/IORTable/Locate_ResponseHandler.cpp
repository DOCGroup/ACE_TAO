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

#include "tao/Exception.h"
#include "tao/ORB_Core.h"
#include "tao/GIOP_Utils.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/Object.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/TAO_Server_Request.h"

#if !defined (__ACE_INLINE__)
# include "Locate_ResponseHandler.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// The pseudo-object _nil method.
TAO_AMH_Locate_ResponseHandler_ptr
TAO_AMH_Locate_ResponseHandler::_nil (void)
{
  return (TAO_AMH_Locate_ResponseHandler_ptr) 0;
}


TAO_AMH_Locate_ResponseHandler::TAO_AMH_Locate_ResponseHandler (
    TAO_ServerRequest & sr)
  : TAO_AMH_Response_Handler (),
    orb_ (CORBA::ORB::_duplicate (sr.orb()))
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
TAO_AMH_Locate_ResponseHandler::forward_ior (const char *ior,
                                             CORBA::Boolean is_perm)
{
  CORBA::Object_var fwd = this->orb_->string_to_object (ior);
  this->_tao_rh_send_location_forward (fwd.in(), is_perm);
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


TAO_END_VERSIONED_NAMESPACE_DECL
