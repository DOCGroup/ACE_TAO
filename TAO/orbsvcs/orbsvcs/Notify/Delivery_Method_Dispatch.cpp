// $Id$

#include "Delivery_Method_Dispatch.h"

#if ! defined (__ACE_INLINE__)
#include "Delivery_Method_Dispatch.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, Delivery_Method_Dispatch, "$Id$")

#include "Delivery_Request.h"
#include "ProxySupplier.h"
#include "Consumer.h"
#include "tao/debug.h"
//#define DEBUG_LEVEL 9
#define DEBUG_LEVEL TAO_debug_level

//////////////////////////////////////////////////////////

namespace TAO_NOTIFY
{

Delivery_Method_Dispatch::Delivery_Method_Dispatch (
  const TAO_NOTIFY::Delivery_Request_Ptr& delivery_request,
  TAO_Notify_ProxySupplier* proxy_supplier,
  bool filtering)
  : Delivery_Method (delivery_request)
  , proxy_supplier_ (proxy_supplier)
  , refcountable_guard_ (*proxy_supplier)
  , filtering_ (filtering)
{
  delivery_request->set_delivery_type (1);
  IdVec ids(3);
  proxy_supplier->get_id_path (ids);
  delivery_request->set_destination_id (ids);
}

Delivery_Method_Dispatch::Delivery_Method_Dispatch (
  const Delivery_Method_Dispatch & rhs)
  : Delivery_Method (rhs.delivery_request_)
  , proxy_supplier_ (rhs.proxy_supplier_)
  , refcountable_guard_ (*rhs.proxy_supplier_)
  , filtering_ (rhs.filtering_)
{
}

Delivery_Method_Dispatch::~Delivery_Method_Dispatch ()
{
}

//static
Delivery_Method_Dispatch *
Delivery_Method_Dispatch::create (
  const Delivery_Request_Ptr& delivery_request,
  TAO_Notify_EventChannelFactory &ecf,
  TAO_InputCDR & cdr
  ACE_ENV_ARG_DECL)
{
  bool ok = true;
  Delivery_Method_Dispatch * result = 0;
  ACE_CString textpath;
  CORBA::ULong count;
  if (cdr.read_ulong (count))
  {
    IdVec id_path (count);
    for (size_t nid = 0; ok && nid < count; ++nid)
    {
      TAO_Notify_Object_Id id = 0;
      if ( cdr.read_long (id))
      {
        id_path.push_back (id);
        char idbuf[20];
        ACE_OS::snprintf (idbuf, sizeof(idbuf)-1, "/%d", ACE_static_cast (int, id));
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
        0 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN(0);
      if (proxy_supplier != 0)
      {
        if (DEBUG_LEVEL > 6) ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Delivery_Method reload event for %s\n")
          , textpath.c_str()
          ));
        ACE_NEW_NORETURN (result,
          Delivery_Method_Dispatch (delivery_request, proxy_supplier, true));
      }
      else
      {
        TAO_Notify_ProxyConsumer * proxy_consumer = ecf.find_proxy_consumer (id_path, 0 ACE_ENV_ARG_PARAMETER); //@@todo
        if (proxy_consumer == 0)
        {
          ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("(%P|%t) Delivery_Method_Dispatch::unmarshal: unknown proxy id %s\n")
            , textpath.c_str()
            ));
        }
        else
        {
          ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("(%P|%t) Delivery_Method_Dispatch::unmarshal: wrong type of proxy id %s\n")
            , textpath.c_str()
            ));
        }
      }
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Delivery_Method_Dispatch::unmarshal: Cant read proxy id path\n")
        ));
    }
  }
  return result;
}


TAO_Notify_Method_Request*
Delivery_Method_Dispatch::copy (void)
{
  return new Delivery_Method_Dispatch (*this);
}

int
Delivery_Method_Dispatch::execute (ExecuteOption eo ACE_ENV_ARG_DECL)
{
  ACE_ASSERT(this->proxy_supplier_ != 0);
  ACE_ASSERT(this->delivery_request_ != 0);
  ACE_ASSERT(! this->delivery_request_->event().null());

  if (eo == TAO_Notify_Method_Request::DISCARD)
  {
    this->delivery_request_->complete();
    return 0;
  }
  // Note : Unlike suppliers, if the consumer has shutdown() then we can safely discard the
  // event.
  if (this->proxy_supplier_->has_shutdown())
  {
    if (DEBUG_LEVEL > 0)ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Delivery_Method_Dispatch: proxy_supplier is shut down.  Discard delivery.\n")
      ));
    this->delivery_request_->complete();
    return 0;
  }

  // Same logic as above
  TAO_Notify_Consumer* consumer = this->proxy_supplier_->consumer ();
  if (consumer == 0)
  {
    if (DEBUG_LEVEL > 0)ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Delivery_Method_Dispatch: consumer is null.  Discard delivery.\n")
      ));
    this->delivery_request_->complete();
    return 0;
  }

  if (this->filtering_)
  {
    CORBA::Boolean val = this->proxy_supplier_->check_filters (* this->delivery_request_->event() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);
    if (! val)
    {
      if (DEBUG_LEVEL > 0)ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Delivery_Method_Dispatch: Proxysupplier %d filter discards delivery.\n"),
        ACE_static_cast (int, this->proxy_supplier_->id ())
        ));

      this->delivery_request_->complete();
      return 0;
    }
  }

  if (DEBUG_LEVEL > 0)ACE_DEBUG ((LM_DEBUG,
     ACE_TEXT ("(%P|%t) Delivery_Method_Dispatch: Push event to Proxysupplier %d.\n"),
    ACE_static_cast (int, this->proxy_supplier_->id ())
    ));

  consumer->push (this->delivery_request_); // calls delivery_request_->complete()

  return 0;
}

} //namespace TAO_NOTIFY
