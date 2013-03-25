// -*- C++ -*-
// $Id$

#include "AsyncAccessManager.h"
#include "ImR_Locator_i.h"


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#if 0
class AsyncAccessManager
{
 public:
  AsyncAccessManager (UpdateableServerInfo &info, ImR_Locator_i &locator);
  ~AsyncAccessManager (void);

  void add_interest (ImR_ReplyHandler *rh);
  AAM_Status status (void) const;

  void activator_replied (void);
  void server_is_running (void);
  void ping_replied (bool is_alive);
  void

  void add_ref (void);
  void remove_ref (void);

 private:
  UpdateableServerInfo &info_;
  ImR_Locator_i &locator_;

  ACE_Stack<ImR_ReplyHandler *> rh_list_;

  AAM_Status status_;

  int refcount_;
  TAO_SYNCH_MUTEX lock_;

};
#endif

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

ImR_Loc_ReplyHandler::ImR_Loc_ReplyHandler (AMH_ImplementationRepository::LocatorResponseHandler_ptr rh)
  :rh_ (AMH_ImplementationRepository::LocatorResponseHandler::_duplicate(rh))
{
}

ImR_Loc_ReplyHandler::~ImR_Loc_ReplyHandler (void)
{
}

void
ImR_Loc_ReplyHandler::send_ior (const char *)
{
  rh_->activate_server (); // void return
  delete this;

}

void
ImR_Loc_ReplyHandler::send_exception (void)
{
  CORBA::TRANSIENT ex (CORBA::SystemException::_tao_minor_code
                       ( TAO_IMPLREPO_MINOR_CODE, 0),
                       CORBA::COMPLETED_NO);
  TAO_AMH_DSI_Exception_Holder h(&ex);
  resp_->invoke_excep(&h);
  delete this;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

AsyncLiveListener::AsyncLiveListener (AsyncAccessManager &aam, LiveCheck &pinger)
  :aam_ (aam),
   pinger_ (pinger),
   status_ (LS_UNKNOWN),
   retries_ (10)
{
  this->aam_.add_ref ();
  this->pinger_.add_listener (this);
}

AsyncLiveListener::~AsyncLiveListener (void)
{
  this->aam_.remove_ref ();
}

void
AsyncLiveListener::status_changed (LiveStatus status)
{
  this->status_ = status;
  if (status == LS_TRANSIENT && --this->retries_ > 0)
    this->pinger_.add_listener (this);
  else
    this->aam_.ping_replied (status != LS_DEAD);
}
