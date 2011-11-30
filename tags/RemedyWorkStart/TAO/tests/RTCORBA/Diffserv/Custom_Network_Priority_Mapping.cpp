// $Id$

#include "Custom_Network_Priority_Mapping.h"
#include "tao/debug.h"
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

static int dscp [] =
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
static const char *dscp_char[]=
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

Custom_Network_Priority_Mapping::Custom_Network_Priority_Mapping (void)
  : corba_priority_ (RTCORBA::minPriority)
{
}

void
Custom_Network_Priority_Mapping::corba_priority (RTCORBA::Priority corba_priority)
{
  this->corba_priority_ = corba_priority;
}

CORBA::Boolean
Custom_Network_Priority_Mapping::to_network (RTCORBA::Priority,
                                             RTCORBA::NetworkPriority &network_priority)
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "Custom_Network_Priority_Mapping::to_network corba_priority %d\n",
                this->corba_priority_));

  const int total_slots = sizeof (dscp) / sizeof (int);

  int array_slot =
    static_cast<int> (((this->corba_priority_ - RTCORBA::minPriority) / double (RTCORBA::maxPriority - RTCORBA::minPriority)) * total_slots);

  if (array_slot == total_slots)
    array_slot -= 1;

  network_priority = dscp[array_slot];

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "Custom_Network_Priority_Mapping::to_network = %x\n",
                network_priority));

  return 1;
}

CORBA::Boolean
Custom_Network_Priority_Mapping::to_CORBA (RTCORBA::NetworkPriority,
                                           RTCORBA::Priority &)
{
  // This method should not be called.
  ACE_ASSERT (0);

  return 0;
}
