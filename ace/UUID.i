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
  if (node_release_ == 1)
    delete node_;

  this->node_ = node;
  node_release_ = 0;
}

ACE_INLINE ACE_CString* 
ACE_UUID::thr_id (void)
{
  return &this->thr_id_;
}

ACE_INLINE void
ACE_UUID::thr_id (char* thr_id)
{
  this->thr_id_ = thr_id;
}
  
ACE_INLINE ACE_CString* 
ACE_UUID::pid (void)
{
  return &this->pid_;
}

ACE_INLINE void 
ACE_UUID::pid (char* pid)
{
  this->pid_ = pid;
}

ACE_INLINE ACE_UUID& 
ACE_UUID::operator = (const ACE_UUID &right)
{
  *this = right;
  return *this;
}

ACE_INLINE bool 
ACE_UUID::operator == (const ACE_UUID &right) const
{
  ACE_UUID rt (right);
  if ((timeLow_ != rt.timeLow ()) ||
      (timeMid_ != rt.timeMid ()) ||
      (timeHiAndVersion_ != rt.timeHiAndVersion ()) ||
      (clockSeqHiAndReserved_ != rt.clockSeqHiAndReserved ()) ||
      (clockSeqLow_ != rt.clockSeqLow ()) ||
      (node_ != rt.node ())) 
    {
      return false;
    }
  
  return true;
}

ACE_INLINE bool 
ACE_UUID::operator != (const ACE_UUID &right) const
{
  return !(*this == right);
}

ACE_INLINE bool 
ACE_UUID::operator < (const ACE_UUID &rt) const
{
  ACE_UUID right (rt);
  if ((timeLow_ < right.timeLow ()) ||
      (timeMid_ < right.timeMid ()) ||
      (timeHiAndVersion_ < right.timeHiAndVersion ()) ||
      (clockSeqHiAndReserved_ < right.clockSeqHiAndReserved ()) ||
      (clockSeqLow_ < right.clockSeqLow ()) ||
      (node_ < right.node ()))
    {
      return true;
    }
  
  return false;
}

ACE_INLINE bool 
ACE_UUID::operator > (const ACE_UUID &right) const
{
  return right < *this;
}

ACE_INLINE bool 
ACE_UUID::operator <= (const ACE_UUID &right) const
{
  return !(*this > right);
}

ACE_INLINE bool 
ACE_UUID::operator >= (const ACE_UUID &right) const
{
  return !(*this < right);
}

ACE_INLINE UUID_node& 
UUID_node::operator = (const UUID_node &right)
{
  *this = right;
  return *this;
}

ACE_INLINE bool 
UUID_node::operator == (const UUID_node& rt) const
{
  UUID_node right = rt;
  for (size_t i = 0; i < NODE_ID_SIZE; ++i) 
    {
      if (nodeID_ [i] != right.nodeID ()[i]) 
	{
	  return false;
	}
    }
  return true;
}

ACE_INLINE bool 
UUID_node::operator != (const UUID_node& right) const
{
  return !(*this == right);
}

ACE_INLINE bool 
UUID_node::operator < (const UUID_node& rt) const
{
  UUID_node right = rt;
  for (size_t i = 0; i < NODE_ID_SIZE; ++i)
    if (nodeID_ [i] < right.nodeID ()[i])
      return true;
  
  return false;
}





