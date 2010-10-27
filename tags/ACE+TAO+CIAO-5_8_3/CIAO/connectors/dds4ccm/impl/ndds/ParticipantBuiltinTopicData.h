/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 */

#ifndef PARTICIPANTBUILTINTOPICDATA_H_
#define PARTICIPANTBUILTINTOPICDATA_H_

#include "BuiltinTopicKey_t.h"
#include "UserDataQosPolicy.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::ParticipantBuiltinTopicData &ddstopicdata, const ::DDS_ParticipantBuiltinTopicData & topicdata)
{
  ddstopicdata.key <<= topicdata.key;
  ddstopicdata.user_data <<= topicdata.user_data;
}

inline void
operator<<= (::DDS_ParticipantBuiltinTopicData & ddstopicdata, const ::DDS::ParticipantBuiltinTopicData & topicdata)
{
  ddstopicdata.key <<= topicdata.key;
  ddstopicdata.user_data <<= topicdata.user_data;
}

#endif

#endif /* PARTICIPANTBUILTINTOPICDATA_H_ */
