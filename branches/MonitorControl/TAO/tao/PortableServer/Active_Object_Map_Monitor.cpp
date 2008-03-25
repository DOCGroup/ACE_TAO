// $Id$

#include "tao/PortableServer/Active_Object_Map_Monitor.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Guard_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

Active_Object_Map_Monitor::Active_Object_Map_Monitor (void)
  : Size_Monitor ("Active Object Map monnitor")
{
}
    
Active_Object_Map_Monitor::Active_Object_Map_Monitor (const char* name)
  : Size_Monitor (name)
{
}

Active_Object_Map_Monitor::~Active_Object_Map_Monitor (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
