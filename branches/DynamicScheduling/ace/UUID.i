//$Id$
// -*- C++ -*-

ACE_INLINE ACE_UUID & 
ACE_UUID::operator=(const ACE_UUID &right)
{
    uuid_ = right.uuid_;
    return *this;
}

ACE_INLINE bool 
ACE_UUID::operator==(const ACE_UUID &right) const
{
    if ((uuid_.timeLow != right.uuid_.timeLow) ||
        (uuid_.timeMid != right.uuid_.timeMid) ||
        (uuid_.timeHiAndVersion != right.uuid_.timeHiAndVersion) ||
        (uuid_.clockSeqHiAndReserved != right.uuid_.clockSeqHiAndReserved) ||
        (uuid_.clockSeqLow != right.uuid_.clockSeqLow)) 
      {
        return false;
      }
    
    if (uuid_.node != right.uuid_.node)
      {
        return false;
      }
    
    return true;
}

ACE_INLINE bool 
ACE_UUID::operator!=(const ACE_UUID &right) const
{
  return !(*this == right);
}

ACE_INLINE bool 
ACE_UUID::operator<(const ACE_UUID &right) const
{
  if ((uuid_.timeLow < right.uuid_.timeLow) ||
      (uuid_.timeMid < right.uuid_.timeMid) ||
      (uuid_.timeHiAndVersion < right.uuid_.timeHiAndVersion) ||
      (uuid_.clockSeqHiAndReserved < right.uuid_.clockSeqHiAndReserved) ||
      (uuid_.clockSeqLow < right.uuid_.clockSeqLow))
    {
      return true;
    }
    
  if (uuid_.node < right.uuid_.node)
    {
      return true;
    }

  return false;
}

ACE_INLINE bool 
ACE_UUID::operator>(const ACE_UUID &right) const
{
  return right < *this;
}

ACE_INLINE bool 
ACE_UUID::operator<=(const ACE_UUID &right) const
{
  return !(*this > right);
}

ACE_INLINE bool 
ACE_UUID::operator>=(const ACE_UUID &right) const
{
  return !(*this < right);
}

ACE_INLINE bool 
ACE_UUID::UUID_node_t::operator == (const ACE_UUID::UUID_node_t& right) const
{
  for (size_t i = 0; i < (sizeof(nodeID) / sizeof(nodeID[0])); ++i) 
    {
      if (nodeID[i] != right.nodeID[i]) 
	{
	  return false;
	}
    }
  return true;
}

ACE_INLINE bool 
ACE_UUID::UUID_node_t::operator != (const ACE_UUID::UUID_node_t& right) const
{
  return !(*this == right);
}

ACE_INLINE bool 
ACE_UUID::UUID_node_t::operator < (const ACE_UUID::UUID_node_t& right) const
{
  for (size_t i = 0; i < (sizeof( nodeID) / sizeof( nodeID[0])); ++i)
    if (nodeID[i] < right.nodeID[i])
      return true;
  
  return false;
}

