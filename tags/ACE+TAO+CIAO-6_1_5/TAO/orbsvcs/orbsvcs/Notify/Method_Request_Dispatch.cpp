// $Id$

#include "orbsvcs/Notify/Method_Request_Dispatch.h"
#include "orbsvcs/Notify/ProxySupplier.h"
#include "orbsvcs/Notify/Consumer.h"
#include "orbsvcs/Notify/Admin.h"
#include "orbsvcs/Notify/ConsumerAdmin.h"
#include "orbsvcs/Notify/EventChannelFactory.h"

#include "tao/debug.h"
#include "tao/CDR.h"

#include "ace/OS_NS_stdio.h"

#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Constuct from event
TAO_Notify_Method_Request_Dispatch::TAO_Notify_Method_Request_Dispatch (
      const TAO_Notify_Event * event,
      TAO_Notify_ProxySupplier* proxy_supplier,
      bool filtering)
  : TAO_Notify_Method_Request_Event (event)
  , proxy_supplier_ (proxy_supplier)
  , filtering_ (filtering)
{
}

// Construct from a delivery rquest
TAO_Notify_Method_Request_Dispatch::TAO_Notify_Method_Request_Dispatch (
      const TAO_Notify::Delivery_Request_Ptr & delivery,
      TAO_Notify_ProxySupplier* proxy_supplier,
      bool filtering)
  : TAO_Notify_Method_Request_Event (delivery)
  , proxy_supplier_ (proxy_supplier)
  , filtering_ (filtering)
{
}

// Constuct construct from another method request+event
// event is passed separately because we may be using a copy
// of the one in the previous method request
TAO_Notify_Method_Request_Dispatch::TAO_Notify_Method_Request_Dispatch (
      const TAO_Notify_Method_Request_Event & request,
      const TAO_Notify_Event * event,
      TAO_Notify_ProxySupplier* proxy_supplier,
      bool filtering)
  : TAO_Notify_Method_Request_Event (request, event)
  , proxy_supplier_ (proxy_supplier)
  , filtering_ (filtering)
{
}

TAO_Notify_Method_Request_Dispatch::~TAO_Notify_Method_Request_Dispatch ()
{
}

int TAO_Notify_Method_Request_Dispatch::execute_i (void)
{
  if (this->proxy_supplier_->has_shutdown ())
    return 0; // If we were shutdown while waiting in the queue, return with no action.

  if (this->filtering_ == 1)
    {
      TAO_Notify_Admin& parent = this->proxy_supplier_->consumer_admin ();
      CORBA::Boolean const val = this->proxy_supplier_->check_filters (this->event_,
                                                                  parent.filter_admin (),
                                                                  parent.filter_operator ());

      if (TAO_debug_level > 1)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Notify (%P|%t) Proxysupplier %x filter ")
                    ACE_TEXT ("eval result = %d\n"),
          &this->proxy_supplier_ , val));

      // Filter failed - do nothing.
      if (!val)
        return 0;
    }

  try
    {
      TAO_Notify_Consumer* consumer = this->proxy_supplier_->consumer ();

      if (consumer != 0)
        {
          consumer->deliver (this);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      if (TAO_debug_level > 0)
        ex._tao_print_exception (
          ACE_TEXT (
            "TAO_Notify_Method_Request_Dispatch::: error sending event.\n"));
    }

  return 0;
}

/// Static method used to reconstruct a Method Request Dispatch
TAO_Notify_Method_Request_Dispatch_Queueable *
TAO_Notify_Method_Request_Dispatch::unmarshal (
    TAO_Notify::Delivery_Request_Ptr & delivery_request,
    TAO_Notify_EventChannelFactory &ecf,
    TAO_InputCDR & cdr)
{
  bool ok = true;
  TAO_Notify_Method_Request_Dispatch_Queueable * result = 0;
  ACE_CString textpath;
  CORBA::ULong count;
  if (cdr.read_ulong (count))
  {
    TAO_Notify::IdVec id_path (count);
    for (size_t nid = 0; ok && nid < count; ++nid)
    {
      TAO_Notify_Object::ID id = 0;
      if ( cdr.read_long (id))
      {
        id_path.push_back (id);
        char idbuf[20];
        ACE_OS::snprintf (idbuf, sizeof(idbuf), "/%d", static_cast<int> (id));
        textpath += idbuf;
      }
      else
      {
        ok = false;
      }
    }

    if (ok)
    {
      TAO_Notify_ProxySupplier* proxy_supplier = ecf.find_proxy_supplier (id_path,
        0);
      if (proxy_supplier != 0)
      {
        if (DEBUG_LEVEL > 6)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) TAO_Notify_Method_Request_Dispatch")
                      ACE_TEXT (" reload event for %C\n"),
                      textpath.c_str()));
        ACE_NEW_NORETURN (result,
          TAO_Notify_Method_Request_Dispatch_Queueable (delivery_request, proxy_supplier, true));
      }
      else
      {
        TAO_Notify_ProxyConsumer * proxy_consumer = ecf.find_proxy_consumer (id_path, 0); //@@todo
        if (proxy_consumer == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) TAO_Notify_Method_Request_Dispatch")
                      ACE_TEXT ("::unmarshal: unknown proxy id %C\n"),
                      textpath.c_str()));
        }
        else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) TAO_Notify_Method_Request_Dispatch")
                      ACE_TEXT ("::unmarshal: wrong type of proxy id %C\n"),
                      textpath.c_str()));
        }
      }
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) TAO_Notify_Method_Request_Dispatch")
                  ACE_TEXT ("::unmarshal: Cant read proxy id path\n")));
    }
  }
  return result;
}

///////////////////////////////////////////////////////////////////////////////


/*******************************************************************/

// Constuct construct from another method request+event
// event is passed separately because we may be using a copy
// of the one in the previous method request
TAO_Notify_Method_Request_Dispatch_Queueable::TAO_Notify_Method_Request_Dispatch_Queueable (
      const TAO_Notify_Method_Request_Event & request,
      TAO_Notify_Event::Ptr & event,
      TAO_Notify_ProxySupplier* proxy_supplier,
      bool filtering)
  : TAO_Notify_Method_Request_Dispatch (request, event.get (), proxy_supplier, filtering)
  , TAO_Notify_Method_Request_Queueable (event.get ())
  , event_var_( event )
{
}

  /// Constuct construct from Delivery Request
  /// should ONLY be used by unmarshall
TAO_Notify_Method_Request_Dispatch_Queueable::TAO_Notify_Method_Request_Dispatch_Queueable (
        const TAO_Notify::Delivery_Request_Ptr & request,
        TAO_Notify_ProxySupplier* proxy_supplier,
        bool filtering)
  : TAO_Notify_Method_Request_Dispatch (request, request->event ().get (), proxy_supplier, filtering)
  , TAO_Notify_Method_Request_Queueable (request->event ().get ())
  , event_var_( request->event () )

{
}

TAO_Notify_Method_Request_Dispatch_Queueable::~TAO_Notify_Method_Request_Dispatch_Queueable ()
{
}

int
TAO_Notify_Method_Request_Dispatch_Queueable::execute (void)
{
  return this->execute_i ();
}

/*********************************************************************************************************/

  /// Constuct construct from another method request
TAO_Notify_Method_Request_Dispatch_No_Copy::TAO_Notify_Method_Request_Dispatch_No_Copy (
      const TAO_Notify_Method_Request_Event & request,
      TAO_Notify_ProxySupplier* proxy_supplier,
      bool filtering)
  : TAO_Notify_Method_Request_Dispatch (request, request.event (), proxy_supplier, filtering)
{
}

TAO_Notify_Method_Request_Dispatch_No_Copy:: ~TAO_Notify_Method_Request_Dispatch_No_Copy ()
{
}

int
TAO_Notify_Method_Request_Dispatch_No_Copy::execute (void)
{
  return this->execute_i ();
}

TAO_Notify_Method_Request_Queueable*
TAO_Notify_Method_Request_Dispatch_No_Copy::copy (void)
{
  TAO_Notify_Method_Request_Queueable* request = 0;

  TAO_Notify_Event::Ptr event_var (
    this->event_->queueable_copy () );

  ACE_NEW_THROW_EX (request,
                    TAO_Notify_Method_Request_Dispatch_Queueable (*this, event_var, this->proxy_supplier_.get(), this->filtering_),
                    //TAO_Notify_Method_Request_Dispatch_Queueable (*this, event_var, this->proxy_supplier_, this->filtering_),
                    CORBA::INTERNAL ());

  return request;
}

TAO_END_VERSIONED_NAMESPACE_DECL
