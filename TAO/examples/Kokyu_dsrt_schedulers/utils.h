#include "tao/RTCORBA/RTCORBA.h"
#include "Kokyu_qosC.h"
#include "tao/RTScheduling/RTScheduler.h"

void guid_copy (Kokyu::GuidType &lhs,
                RTScheduling::Current::IdType const &rhs);
void guid_copy (RTScheduling::Current::IdType &lhs,
                Kokyu::GuidType const &rhs);

size_t guid_to_size_t (Kokyu::GuidType const &guid);
size_t guid_to_size_t (RTScheduling::Current::IdType const &guid);
