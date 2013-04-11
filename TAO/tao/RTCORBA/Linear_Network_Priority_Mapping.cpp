// $Id$

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/Linear_Network_Priority_Mapping.h"
#include "tao/debug.h"

#include "ace/Sched_Params.h"
#include "ace/Log_Msg.h"

#define IPDSFIELD_DSCP_DEFAULT  0x00
#define IPDSFIELD_DSCP_CS1      0x08
#define IPDSFIELD_DSCP_CS2      0x10
#define IPDSFIELD_DSCP_CS3      0x18
#define IPDSFIELD_DSCP_CS4      0x20
#define IPDSFIELD_DSCP_CS5      0x28
#define IPDSFIELD_DSCP_CS6      0x30
#define IPDSFIELD_DSCP_CS7      0x38
#define IPDSFIELD_DSCP_AF11     0x0A
#define IPDSFIELD_DSCP_AF12     0x0C
#define IPDSFIELD_DSCP_AF13     0x0E
#define IPDSFIELD_DSCP_AF21     0x12
#define IPDSFIELD_DSCP_AF22     0x14
#define IPDSFIELD_DSCP_AF23     0x16
#define IPDSFIELD_DSCP_AF31     0x1A
#define IPDSFIELD_DSCP_AF32     0x1C
#define IPDSFIELD_DSCP_AF33     0x1E
#define IPDSFIELD_DSCP_AF41     0x22
#define IPDSFIELD_DSCP_AF42     0x24
#define IPDSFIELD_DSCP_AF43     0x26
#define IPDSFIELD_ECT_MASK      0x02
#define IPDSFIELD_CE_MASK       0x01
#define IPDSFIELD_DSCP_EF       0x2E

static int const dscp[] =
{
  IPDSFIELD_DSCP_DEFAULT ,
  IPDSFIELD_DSCP_CS1     ,
  IPDSFIELD_DSCP_CS2     ,
  IPDSFIELD_DSCP_CS3     ,
  IPDSFIELD_DSCP_CS4     ,
  IPDSFIELD_DSCP_CS5     ,
  IPDSFIELD_DSCP_CS6     ,
  IPDSFIELD_DSCP_CS7     ,
  IPDSFIELD_DSCP_AF11    ,
  IPDSFIELD_DSCP_AF12    ,
  IPDSFIELD_DSCP_AF13    ,
  IPDSFIELD_DSCP_AF21    ,
  IPDSFIELD_DSCP_AF22    ,
  IPDSFIELD_DSCP_AF23    ,
  IPDSFIELD_DSCP_AF31    ,
  IPDSFIELD_DSCP_AF32    ,
  IPDSFIELD_DSCP_AF33    ,
  IPDSFIELD_DSCP_AF41    ,
  IPDSFIELD_DSCP_AF42    ,
  IPDSFIELD_DSCP_AF43    ,
  IPDSFIELD_DSCP_EF
};

/*
static const char * const dscp_char[]=
{
  "Normal",
  "CS1",
  "CS2",
  "CS3",
  "CS4",
  "CS5",
  "CS6",
  "CS7",
  "Assured Forwarding 11",
  "Assured Forwarding 12",
  "Assured Forwarding 13",
  "Assured Forwarding 21",
  "Assured Forwarding 22",
  "Assured Forwarding 23",
  "Assured Forwarding 31",
  "Assured Forwarding 32",
  "Assured Forwarding 33",
  "Assured Forwarding 41",
  "Assured Forwarding 42",
  "Assured Forwarding 43",
  "Expedited Forwarding"
};
*/

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Linear_Network_Priority_Mapping::TAO_Linear_Network_Priority_Mapping (long)
{
}

CORBA::Boolean
TAO_Linear_Network_Priority_Mapping::to_network (
  RTCORBA::Priority corba_priority,
  RTCORBA::NetworkPriority &network_priority)
{
  if (TAO_debug_level)
    TAOLIB_DEBUG ((LM_DEBUG,
                "TAO_Linear_Network_Priority_Mapping::to_network corba_priority %d\n",
                corba_priority));

  int const total_slots = sizeof (dscp) / sizeof (int);

  int array_slot =
    static_cast<int> (((corba_priority - RTCORBA::minPriority) / double (RTCORBA::maxPriority - RTCORBA::minPriority)) * total_slots);

  if (array_slot == total_slots)
    array_slot -= 1;

  network_priority = dscp[array_slot];

  if (TAO_debug_level)
    TAOLIB_DEBUG ((LM_DEBUG,
                "TAO_Linear_Network_Priority_Mapping::to_network = %x\n",
                network_priority));

  return 1;
}

CORBA::Boolean
TAO_Linear_Network_Priority_Mapping::to_CORBA (RTCORBA::NetworkPriority network_priority,
                                               RTCORBA::Priority &/*corba_priority*/)
{
  if (TAO_debug_level)
    TAOLIB_DEBUG ((LM_DEBUG,
                "TAO_Linear_Network_Priority_Mapping::to_CORBA network_priority %d\n",
                network_priority));

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
