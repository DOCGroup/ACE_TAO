// $Id$

#ifndef DSRT_DISPATCHER_IMPL_T_CPP
#define DSRT_DISPATCHER_IMPL_T_CPP

#include "DSRT_Dispatcher_Impl_T.h"

#if ! defined (__ACE_INLINE__)
#include "DSRT_Dispatcher_Impl_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Kokyu, DSRT_Dispatcher_Impl, "$Id$")

namespace Kokyu
{

//virtual - so don't inline
template <class DSRT_Scheduler_Traits>
DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits>::~DSRT_Dispatcher_Impl ()
{
}

}

#endif /* DSRT_DISPATCHER_IMPL_T_CPP */
