#include "Custom_Network_Priority_Mapping.h"
#include "tao/debug.h"
#include "ace/Log_Msg.h"

#include <array>

constexpr std::array<int, 21> dscp
{{
  0x00,
  0x08,
  0x10,
  0x18,
  0x20,
  0x28,
  0x30,
  0x38,
  0x0A,
  0x0C,
  0x0E,
  0x12,
  0x14,
  0x16,
  0x1A,
  0x1C,
  0x1E,
  0x22,
  0x24,
  0x26,
  0x2E
}};

void
Custom_Network_Priority_Mapping::corba_priority (RTCORBA::Priority corba_priority)
{
  this->corba_priority_ = corba_priority;
}

CORBA::Boolean
Custom_Network_Priority_Mapping::to_network (
  RTCORBA::Priority,
  RTCORBA::NetworkPriority &network_priority)
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "Custom_Network_Priority_Mapping::to_network corba_priority %d\n",
                this->corba_priority_));

  constexpr int total_slots = static_cast<int> (dscp.size ());
  int array_slot = static_cast<int> (
    ((this->corba_priority_ - RTCORBA::minPriority) /
     double (RTCORBA::maxPriority - RTCORBA::minPriority)) * total_slots);

  if (array_slot == total_slots)
    --array_slot;

  network_priority = dscp[array_slot];

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "Custom_Network_Priority_Mapping::to_network = %x\n",
                network_priority));

  return 1;
}

CORBA::Boolean
Custom_Network_Priority_Mapping::to_CORBA (
  RTCORBA::NetworkPriority,
  RTCORBA::Priority &)
{
  // This method should not be called.
  ACE_ASSERT (0);

  return 0;
}
