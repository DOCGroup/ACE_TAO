#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/Linear_Network_Priority_Mapping.h"
#include "tao/debug.h"

#include "ace/Sched_Params.h"
#include "ace/Log_Msg.h"
#include <array>

constexpr int dscp_default {0x00};
constexpr int dscp_cs1 {0x08};
constexpr int dscp_cs2 {0x10};
constexpr int dscp_cs3 {0x18};
constexpr int dscp_cs4 {0x20};
constexpr int dscp_cs5 {0x28};
constexpr int dscp_cs6 {0x30};
constexpr int dscp_cs7 {0x38};
constexpr int dscp_af11 {0x0A};
constexpr int dscp_af12 {0x0C};
constexpr int dscp_af13 {0x0E};
constexpr int dscp_af21 {0x12};
constexpr int dscp_af22 {0x14};
constexpr int dscp_af23 {0x16};
constexpr int dscp_af31 {0x1A};
constexpr int dscp_af32 {0x1C};
constexpr int dscp_af33 {0x1E};
constexpr int dscp_af41 {0x22};
constexpr int dscp_af42 {0x24};
constexpr int dscp_af43 {0x26};
constexpr int dscp_ef {0x2E};

constexpr std::array<int, 21> dscp {
  dscp_default, dscp_cs1, dscp_cs2, dscp_cs3, dscp_cs4, dscp_cs5, dscp_cs6,
  dscp_cs7, dscp_af11, dscp_af12, dscp_af13, dscp_af21, dscp_af22, dscp_af23,
  dscp_af31, dscp_af32, dscp_af33, dscp_af41, dscp_af42, dscp_af43, dscp_ef
};

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

  int const total_slots = static_cast<int> (dscp.size ());

  int array_slot {
    static_cast<int> (((corba_priority - RTCORBA::minPriority) /
                       double (RTCORBA::maxPriority - RTCORBA::minPriority)) * total_slots)};

  if (array_slot == total_slots)
    --array_slot;

  network_priority = dscp[array_slot];

  if (TAO_debug_level)
    TAOLIB_DEBUG ((LM_DEBUG,
                "TAO_Linear_Network_Priority_Mapping::to_network = %x\n",
                network_priority));

  return true;
}

CORBA::Boolean
TAO_Linear_Network_Priority_Mapping::to_CORBA (RTCORBA::NetworkPriority network_priority,
                                               RTCORBA::Priority &/*corba_priority*/)
{
  if (TAO_debug_level)
    TAOLIB_DEBUG ((LM_DEBUG,
                "TAO_Linear_Network_Priority_Mapping::to_CORBA network_priority %d\n",
                network_priority));

  return false;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
