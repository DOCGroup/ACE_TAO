//$Id$

#include "tao/RTCORBA/RTCORBA.h"
#include "Kokyu_qosC.h"
#include "tao/RTScheduling/RTScheduler.h"

void guid_copy( Kokyu::GuidType& lhs, const RTScheduling::Current::IdType& rhs);
void guid_copy( RTScheduling::Current::IdType& lhs, const Kokyu::GuidType& rhs);
