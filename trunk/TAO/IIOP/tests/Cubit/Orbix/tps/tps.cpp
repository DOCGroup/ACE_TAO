#include "tps.h"

int
TPS_Filter::inRequestPreMarshal(CORBA::Request &r,  CORBA::Environment& env)
{
  // Get target
  CORBA::Object_ptr obj = r.target ();
  Cubit * cb = Cubit::_narrow (obj, env);

  if (env) // Not for our object!!
    return 1; // continue in the same thread
  
  // Enqueue the request
  Cubit_Impl_Task *ct;
  if (CORBA::is_nil (cb))
      return 1;
  ct = (Cubit_Impl_Task *) DEREF(cb);
  if (ct) {
    ACE_Message_Block *mb = new ACE_Message_Block ( (char *) (&r));
    ct->msg_queue ()->enqueue (mb);
  }
  // Done
  return -1;
}

TPS_Filter tps_filter;
