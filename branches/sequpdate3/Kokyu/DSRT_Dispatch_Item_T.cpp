// $Id$

#ifndef DSRT_DISPATCH_ITEM_T_CPP
#define DSRT_DISPATCH_ITEM_T_CPP

#include "DSRT_Dispatch_Item_T.h"

#if ! defined (__ACE_INLINE__)
#include "DSRT_Dispatch_Item_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Kokyu, DSRT_Dispatch_Item, "$Id$")

namespace Kokyu
{

template <class DSRT_Scheduler_Traits>
DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>::
DSRT_Dispatch_Item_var (DSRT_Dispatch_Item<DSRT_Scheduler_Traits> *p)
  :ACE_Strong_Bound_Ptr<DSRT_Dispatch_Item<DSRT_Scheduler_Traits>,
                        ACE_SYNCH_MUTEX> (p)
{
}

template <class DSRT_Scheduler_Traits>
DSRT_Dispatch_Item_var<DSRT_Scheduler_Traits>::
DSRT_Dispatch_Item_var (const DSRT_Dispatch_Item_var &r)
  :ACE_Strong_Bound_Ptr<DSRT_Dispatch_Item<DSRT_Scheduler_Traits>,
                        ACE_SYNCH_MUTEX> (r)
{
}

}

#endif /* DSRT_DISPATCH_ITEM_T_CPP */
