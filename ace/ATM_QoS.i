// $Id$

// ATM_QoS.i

ACE_INLINE
ACE_ATM_QoS::ACE_ATM_QoS ()
{
  ACE_TRACE ("ACE_ATM_QoS::ACE_ATM_QoS");
}

ACE_INLINE
ACE_ATM_QoS::~ACE_ATM_QoS ()
{
  ACE_TRACE ("ACE_ATM_QoS::~ACE_ATM_QoS");
}

ACE_INLINE
ATM_QoS
ACE_ATM_QoS::get_qos (void)
{
  ACE_TRACE ("ACE_ATM_QoS::get_qos");
  return qos_;
}

ACE_INLINE
ACE_QoS_Params
ACE_ATM_QoS::get_option_params(void)
{
  ACE_TRACE( "ACE_ATM_QoS::get_option_params" );
  return ACE_QoS_Params( 0, 0, &qos_, 0, 0 );
}

