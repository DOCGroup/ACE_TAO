//     $Id$

#if 0
#include "ace/OS.h"    // WARNING! This MUST come before objbase.h on WIN32!
#include <objbase.h>
#include <initguid.h>

#include "tao/orb.h"
#include "tao/roa.h"
#include "tao/boa.h"
#include "tao/cdr.h"
#include "tao/connect.h"
#include "tao/giop.h"
#include "tao/params.h"
#include "tao/orbobj.h"
#include "tao/nvlist.h"
#include "tao/debug.h"
#include "tao/Orb_Core.h"
#endif

#include "tao/corba.h"



// For COM -- IUnknown operations, we provide the vtable entry

// {A201E4C4-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_ROA,
0xa201e4c4, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);


#if !defined (__ACE_INLINE__)
#  include "roa.i"
#endif /* __ACE_INLINE__ */
