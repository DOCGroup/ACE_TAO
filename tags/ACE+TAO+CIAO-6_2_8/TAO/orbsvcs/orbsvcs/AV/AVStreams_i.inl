// -*- C++ -*-
//
// $Id$

#include "orbsvcs/Log_Macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_AV_QoS &
TAO_Base_StreamEndPoint::qos (void)
{
  return this->qos_;
}

ACE_INLINE
int
TAO_AV_QoS::set (AVStreams::streamQoS &stream_qos)
{
  this->stream_qos_ = stream_qos;

  for (u_int j=0;j<this->stream_qos_.length ();j++)
    {
      ACE_CString qos_key (CORBA::string_dup (this->stream_qos_[j].QoSType));
      int result = this->qos_map_.bind (qos_key,this->stream_qos_[j]);
      if (result < 0)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           "(%N,%l) TAO_AV_QoS::set qos_map::bind failed\n"),
                          -1);
    }
  return 0;
}


ACE_INLINE
int
TAO_AV_QoS::get_flow_qos (const char *flowname,
                          AVStreams::QoS &flow_qos)
{
  int result = this->qos_map_.find (flowname, flow_qos);

  if (result < 0)
    {
      if (TAO_debug_level > 0 ) {
        ORBSVCS_DEBUG((LM_DEBUG, "(%N,%l) qos_map contains the flows:\n"));

        ACE_Hash_Map_Manager<ACE_CString,AVStreams::QoS,ACE_Null_Mutex>::ITERATOR iter
          = qos_map_.begin();

        while( iter != qos_map_.end() )
        {
          ORBSVCS_DEBUG((LM_DEBUG, "  %s\n", (*iter).ext_id_.c_str() ));
          ++iter;
        }

        ORBSVCS_DEBUG ((LM_DEBUG,
                    "(%N,%l) TAO_AV_QOS::get_flow_qos qos_map::find failed for %s\n", flowname));
      }
      return -1;
    }
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
