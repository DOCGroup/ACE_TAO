// -*- C++ -*-
/*
 * @file AsyncListManager.h
 *
 * @author Phil Mesnier <mesnier_p@ociweb.com>
 */

#ifndef IMR_ASYNCLISTMANAGER_H_
#define IMR_ASYNCLISTMANAGER_H_

#include "locator_export.h"

#include "ImR_LocatorC.h"
#include "ace/Vector_T.h"
#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Intrusive_Ref_Count_Handle_T.h"
#include <atomic>
#include "LiveCheck.h"

class Locator_Repository;

//----------------------------------------------------------------------------
/*
 * @class AsyncListManager
 *
 * @brief manages active detection of multiple servers to satisfy "list -a"
 *
 * A client of the locator may issue a list command to be filtered to
 * currently active servers only. To satisfy that request each server must
 * be pinged to determine liveness. The AsyncListManager handles the
 * callbacks for all of the ping requests as efficiently as possible.
 */

class AsyncListManager
{
 public:
  AsyncListManager (const Locator_Repository *repo,
                    PortableServer::POA_ptr poa,
                    LiveCheck *pinger);

  ~AsyncListManager ();

  PortableServer::POA_ptr poa ();
  void list (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
             CORBA::ULong count);

  CORBA::ULong list (ImplementationRepository::AMH_ServerInformationIteratorResponseHandler_ptr _tao_rh,
                     CORBA::ULong start, CORBA::ULong count);

  bool evaluate_status (CORBA::ULong index, LiveStatus status, int pid);
  void ping_replied (CORBA::ULong index, LiveStatus status, int pid);

  AsyncListManager *_add_ref ();
  void _remove_ref ();

 private:
  void init_list ();
  void list_i (CORBA::ULong start, CORBA::ULong count);
  bool make_iterator (ImplementationRepository::ServerInformationIterator_out si,
                      CORBA::ULong start);
  void final_state ();

  const Locator_Repository *repo_;
  PortableServer::POA_var poa_;
  ImplementationRepository::AMH_AdministrationResponseHandler_var primary_;
  ImplementationRepository::AMH_ServerInformationIteratorResponseHandler_var secondary_;
  LiveCheck *pinger_;
  ImplementationRepository::ServerInformationList server_list_;
  CORBA::ULong first_;
  CORBA::ULong how_many_;
  CORBA::ULong waiters_;
  std::atomic<int> refcount_;
};

typedef TAO_Intrusive_Ref_Count_Handle<AsyncListManager> AsyncListManager_ptr;

//----------------------------------------------------------------------------

class ListLiveListener : public LiveListener
{
 public:
  ListLiveListener (const char * server,
                    int pid,
                    CORBA::ULong index,
                    AsyncListManager *owner,
                    LiveCheck &pinger);

  virtual ~ListLiveListener ();
  bool start ();
  void cancel ();
  LiveStatus status ();
  bool status_changed (LiveStatus status);

 private:
  AsyncListManager_ptr owner_;
  LiveCheck &pinger_;
  LiveStatus status_;
  CORBA::ULong index_;
  bool started_;
  int pid_;
};

#endif /* IMR_ASYNCACCESSMANGER_H_  */
