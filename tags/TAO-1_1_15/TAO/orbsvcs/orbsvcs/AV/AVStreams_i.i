/* -*- C++ -*- */
// $Id$

// AVStreams_i.i

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
      TAO_String_Hash_Key qos_key (CORBA::string_dup (this->stream_qos_[j].QoSType));
      int result = this->qos_map_.bind (qos_key,this->stream_qos_[j]);
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,"qos_map::bind failed\n"),-1);
    }
  return 0;
}


ACE_INLINE
int
TAO_AV_QoS::get_flow_qos (const char *flowname,
			  AVStreams::QoS &flow_qos)
{
  int result = this->qos_map_.find (flowname, 
				    flow_qos);

  if (result < 0)
    ACE_ERROR_RETURN ((LM_DEBUG,"qos_map::find failed\n"),-1);
  return 0;
}


