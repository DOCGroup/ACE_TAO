/**
 * @file Async_IOR_Table_Impl.cpp
 *
 * $Id$
 *
 * @author Phil Mesnier <mesnier_p@ociweb.com>
 *
 */

#include "tao/Async_IORTable/Async_IOR_Table_Impl.h"
#include "tao/PortableServer/ForwardRequestC.h"
#include "tao/DynamicInterface/AMH_DSI_Response_Handler.h"
#include "ace/Guard_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Async_IOR_Table_Impl::TAO_Async_IOR_Table_Impl (void)
{
}

TAO_Async_IOR_Table_Impl::~TAO_Async_IOR_Table_Impl (void)
{
}

void
TAO_Async_IOR_Table_Impl::async_find (::IORTable::Locate_ResponseHandler handler,
                                      const char *object_key)
{
  // We don't want the lock held during locate, so make it go out
  // of scope before then.
  {
    ACE_CString key (object_key);
    ACE_CString ior;
    TAO_AMH_Locate_ResponseHandler_var rh = handler;
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
    if (this->map_.find (key, ior) == 0)
      {
        rh->forward_ior (ior.c_str(), false);
        return;
      }
    if (CORBA::is_nil (this->async_locator_.in ()))
      {
        if (CORBA::is_nil (this->locator_.in ()))
          {
            rh->raise_excep (IORTable::NotFound ());
          }
        else
          {
            try
              {
                ior = this->locator_->locate (object_key);
                rh->forward_ior (ior.c_str(), false);
              }
            catch (CORBA::Exception &ex)
              {
                rh->raise_excep (ex);
              }
          }
        return;
      }
  }
  this->async_locator_->async_locate (handler, object_key);
}

void
TAO_Async_IOR_Table_Impl::set_locator (IORTable::Locator_ptr locator)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->locator_ = IORTable::Locator::_duplicate (locator);
  this->async_locator_ = IORTable::AsyncLocator::_narrow (locator);
}

TAO_END_VERSIONED_NAMESPACE_DECL
