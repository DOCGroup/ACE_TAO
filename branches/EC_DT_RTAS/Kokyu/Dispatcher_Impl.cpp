// $Id$

#include "Dispatcher_Impl.h"

#if ! defined (__ACE_INLINE__)
#include "Dispatcher_Impl.i"
#endif /* __ACE_INLINE__ */

#if ! defined (ACE_WIN32) && defined (ACE_HAS_DSUI)
#include "kokyu_config.h"
#include "kokyu_dsui_families.h"
#include <dsui.h>
#endif

ACE_RCSID(Kokyu, Dispatcher_Impl, "$Id$")

namespace Kokyu
{

//virtual - so don't inline
Dispatcher_Impl::~Dispatcher_Impl()
{
}

}
