//$Id$
// -*- C++ -*-

/// Data Members for Class Attributes
ACE_INLINE ACE_UINT32 
ACE_UUID::timeLow (void)
{
  return this->timeLow_;
}

ACE_INLINE void 
ACE_UUID::timeLow (ACE_UINT32 timelow)
{
  this->timeLow_ = timelow;
}

ACE_INLINE ACE_UINT16 
ACE_UUID::timeMid (void)
{
  return this->timeMid_;
}

ACE_INLINE void  
ACE_UUID::timeMid (ACE_UINT16 time_mid)
{
  this->timeMid_ = time_mid;
}

ACE_INLINE ACE_UINT16 
ACE_UUID::timeHiAndVersion (void)
{
  return this->timeHiAndVersion_;
}

ACE_INLINE void 
ACE_UUID::timeHiAndVersion (ACE_UINT16 timeHiAndVersion)
{
  this->timeHiAndVersion_ = timeHiAndVersion;
}

ACE_INLINE u_char 
ACE_UUID::clockSeqHiAndReserved (void)
{
  return this->clockSeqHiAndReserved_;
}

ACE_INLINE void 
ACE_UUID::clockSeqHiAndReserved (u_char clockSeqHiAndReserved)
{
  this->clockSeqHiAndReserved_ = clockSeqHiAndReserved;
}

ACE_INLINE u_char 
ACE_UUID::clockSeqLow (void)
{
  return this->clockSeqLow_;
}

ACE_INLINE void 
ACE_UUID::clockSeqLow (u_char clockSeqLow)
{
  this->clockSeqLow_ = clockSeqLow;
}

ACE_INLINE UUID_node* 
ACE_UUID::node (void)
{
  return this->node_;
}

ACE_INLINE void 
ACE_UUID::node (UUID_node* node)
{
  this->node_ = node;
}

/*
  ACE_INLINE ACE_UUID& 
  ACE_UUID::operator=(const ACE_UUID &right)
  {
  *this = right;
  return *this;
  }

  ACE_INLINE bool 
  ACE_UUID::operator==(const ACE_UUID &right) const
  {
  if ((timeLow_ != right.timeLow ()) ||
  (this->timeMid () != right.timeMid ()) ||
  (this->timeHiAndVersion () != right.timeHiAndVersion ()) ||
  (this->clockSeqHiAndReserved () != right.clockSeqHiAndReserved ()) ||
  (this->clockSeqLow () != right.clockSeqLow ())) 
  {
  return false;
  }
    
  if (this->node () != right.node ())
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

*/
