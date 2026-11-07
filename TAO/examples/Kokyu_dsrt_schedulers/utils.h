#include "tao/RTCORBA/RTCORBA.h"
#include "Kokyu_qosC.h"
#include "tao/RTScheduling/RTScheduler.h"

void guid_copy (Kokyu::GuidType &lhs,
                RTScheduling::Current::IdType const &rhs);
void guid_copy (RTScheduling::Current::IdType &lhs,
                Kokyu::GuidType const &rhs);
