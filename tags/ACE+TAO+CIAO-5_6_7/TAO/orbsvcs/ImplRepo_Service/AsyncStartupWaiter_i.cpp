// -*- C++ -*-
//
// $Id$

#include "AsyncStartupWaiter_i.h"
#include "ImR_Locator_i.h"

using namespace ImplementationRepository;

AsyncStartupWaiter_i::PendingData::PendingData (const char* p, const char* i)
: partial_ior (p)
, ior( i)
{
}

AsyncStartupWaiter_i::PendingData::PendingData ()
{
}


void AsyncStartupWaiter_i::debug (bool dbg)
{
  debug_ = dbg;
}

void AsyncStartupWaiter_i::wait_for_startup (AMH_AsyncStartupWaiterResponseHandler_ptr rh,
   const char* name)
{
  PendingListPtr plst;
  pending_.find(name, plst);
  if (! plst.null () && plst->size () > 0)
    {
      PendingList& tmp = *plst;
      PendingData& pd = tmp[tmp.size () - 1];
      tmp.pop_back ();

      if (debug_)
        ACE_DEBUG ((LM_DEBUG, "ImR: Skipping wait due to queued startup info for <%s>.\n", name));

      send_response (*rh, name, pd.partial_ior.c_str(), pd.ior.c_str());

    }
  else
    {
      RHListPtr lst;
      waiting_.find (name, lst);
      if (lst.null ())
      {
        lst = RHListPtr (new RHList);
        int err = waiting_.bind (name, lst);
        ACE_ASSERT (err == 0);
        ACE_UNUSED_ARG (err);
      }
      lst->push_back (AMH_AsyncStartupWaiterResponseHandler::_duplicate (rh));
    }
}

void
AsyncStartupWaiter_i::send_response (ImplementationRepository::AMH_AsyncStartupWaiterResponseHandler& rh,
                                    const char* name, const char* partial_ior, const char* ior)
{
  StartupInfo_var si = new StartupInfo();
  si->name = name;
  si->partial_ior = partial_ior;
  si->ior = ior;

  try
    {
      rh.wait_for_startup (si.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      if (debug_)
        ex._tao_print_exception (
          "AsyncStartupWaiter_i::send_response ()");
    }
}

void
AsyncStartupWaiter_i::unblock_one (const char* name, const char* partial_ior, const char* ior, bool queue) {
  ImplementationRepository::AMH_AsyncStartupWaiterResponseHandler_var rh = get_one_waiter(name);
  if (! CORBA::is_nil(rh.in ()))
    {
      send_response (*rh.in (), name, partial_ior, ior);
    }
  else if (queue)
    {
      if (debug_)
        ACE_DEBUG((LM_DEBUG, "ImR: Queuing startup info.\n"));

      PendingListPtr lst;
      pending_.find (name, lst);
      if (lst.null ())
        {
          lst = PendingListPtr (new PendingList);
          int err = pending_.bind (name, lst);
          ACE_ASSERT (err == 0);
          ACE_UNUSED_ARG (err);
        }
      lst->push_back (PendingData (partial_ior, ior));
    }
}

void
AsyncStartupWaiter_i::unblock_all (const char* name) {
  RHList tmp;

  get_all_waiters (name, tmp);

  // This startup info should be ignored when unblocking all, because we
  // don't know the ior or partial_ior at this point.
  StartupInfo_var si = new StartupInfo ();
  si->name = name;

  // Note : This method may be called when there are no waiters.

  for (size_t i = 0; i < tmp.size(); ++i)
  {
    try
      {
        ImplementationRepository::AMH_AsyncStartupWaiterResponseHandler_var& rh = tmp[i];

        rh->wait_for_startup (si.in ());
      }
    catch (const CORBA::Exception& ex)
      {
        if (debug_)
          ex._tao_print_exception (
            "AsyncStartupWaiter_i::unblock_all ()");
      }
  }
}

ImplementationRepository::AMH_AsyncStartupWaiterResponseHandler_ptr
AsyncStartupWaiter_i::get_one_waiter (const char* name)
{
  RHListPtr lst;
  waiting_.find (name, lst);
  if (! lst.null() && lst->size () > 0)
    {
      RHList& rhlst = *lst;
      ImplementationRepository::AMH_AsyncStartupWaiterResponseHandler_var& tmp = rhlst[rhlst.size () - 1];
      ImplementationRepository::AMH_AsyncStartupWaiterResponseHandler_ptr ret = tmp._retn ();
      rhlst.pop_back ();
      return ret;
    }
  return ImplementationRepository::AMH_AsyncStartupWaiterResponseHandler::_nil ();
}

void
AsyncStartupWaiter_i::get_all_waiters (const char* name, RHList& ret)
{
  RHListPtr lst;
  waiting_.find (name, lst);
  if (! lst.null ()) {
    for (size_t i = 0; i < lst->size (); ++i)
      {
        RHList& tmp = *lst;
        ret.push_back (tmp[i]);
        // The ACE_Vector will not destruct the elements when cleared, so we must
        // make sure to do so here.
        tmp[i] = ImplementationRepository::AMH_AsyncStartupWaiterResponseHandler::_nil ();
      }
    lst->clear ();
  }
}

