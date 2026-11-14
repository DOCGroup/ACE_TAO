/**
 * @file Async_IOR_Table_Impl.cpp
 *
 * @author Phil Mesnier <mesnier_p@ociweb.com>
 */

#include "tao/IORTable/Async_IOR_Table_Impl.h"
#include "tao/PortableServer/ForwardRequestC.h"
#include "tao/DynamicInterface/AMH_DSI_Response_Handler.h"
#include "ace/Guard_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_Async_IOR_Table_Impl::async_find (::IORTable::Locate_ResponseHandler handler,
                                      char const *object_key)
{
  std::string ior;
  IORTable::AsyncLocator_var async_locator;
  IORTable::Locator_var locator;
  bool found {};
  TAO_AMH_Locate_ResponseHandler_var const rh = handler;
  {
    std::string const key (object_key);
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
    Map::const_iterator const iter = this->map_.find (key);
    if (iter != this->map_.end ())
      {
        ior = iter->second;
        found = true;
      }
    else
      {
        async_locator = IORTable::AsyncLocator::_duplicate (this->async_locator_.in ());
        if (CORBA::is_nil (async_locator.in ()))
          {
            locator = IORTable::Locator::_duplicate (this->locator_.in ());
          }
      }
  }

  if (found)
    {
      rh->forward_ior (ior.c_str (), false);
    }
  else if (!CORBA::is_nil (async_locator.in ()))
    {
      async_locator->async_locate (handler, object_key);
    }
  else if (!CORBA::is_nil (locator.in ()))
    {
      try
        {
          ior = locator->locate (object_key);
          rh->forward_ior (ior.c_str (), false);
        }
      catch (CORBA::Exception const &ex)
        {
          rh->raise_excep (ex);
        }
    }
  else
    {
      rh->raise_excep (IORTable::NotFound ());
    }
}

void
TAO_Async_IOR_Table_Impl::set_locator (IORTable::Locator_ptr locator)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->locator_ = IORTable::Locator::_duplicate (locator);
  this->async_locator_ = IORTable::AsyncLocator::_narrow (locator);
}

TAO_END_VERSIONED_NAMESPACE_DECL
