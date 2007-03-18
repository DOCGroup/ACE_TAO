// -*- C++ -*-
//
// $Id$

#ifndef IMR_AsyncStartupWaiterI_H_
#define IMR_AsyncStartupWaiterI_H_

#include "locator_export.h"
#include "AsyncStartupWaiterS.h"

#include "ace/Vector_T.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Bound_Ptr.h"
#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class ImR_Locator_i;

class Locator_Export AsyncStartupWaiter_i : public virtual POA_ImplementationRepository::AMH_AsyncStartupWaiter
{
  struct PendingData {
    PendingData();
    PendingData(const char* partial_ior, const char* ior);
    ACE_CString partial_ior;
    ACE_CString ior;
  };
  typedef ACE_Vector<PendingData> PendingList;
  typedef ACE_Strong_Bound_Ptr<PendingList, ACE_Null_Mutex> PendingListPtr;
  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
    PendingListPtr,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> PendingMap;

  typedef ACE_Vector<ImplementationRepository::AMH_AsyncStartupWaiterResponseHandler_var> RHList;
  typedef ACE_Strong_Bound_Ptr<RHList, ACE_Null_Mutex> RHListPtr;
  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
    RHListPtr,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> WaitingMap;

public:

  void wait_for_startup (
   ImplementationRepository::AMH_AsyncStartupWaiterResponseHandler_ptr rh,
   const char* name);

  void unblock_one(const char* name, const char* partial_ior, const char* ior, bool queue);
  void unblock_all(const char* name);

  void debug(bool dbg);

private:

ImplementationRepository::AMH_AsyncStartupWaiterResponseHandler_ptr
  get_one_waiter(const char* name);

void get_all_waiters(const char* name, RHList& ret);

void send_response(ImplementationRepository::AMH_AsyncStartupWaiterResponseHandler& rh,
                   const char* name, const char* partial_ior, const char* ior);
private:
  WaitingMap waiting_;
  PendingMap pending_;
  bool debug_;
};

#endif /* AsyncStartupWaiterI_H_  */

