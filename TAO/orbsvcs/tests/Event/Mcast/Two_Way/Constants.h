#include "orbsvcs/Event_Service_Constants.h"

inline constexpr long HEARTBEAT {ACE_ES_EVENT_UNDEFINED + 1};
inline constexpr long SOURCE_ID {ACE_ES_EVENT_SOURCE_ANY + 1};
inline constexpr int HEARTBEATS_TO_SEND {50};
