/* -*- C++ -*- */
// $Id$

// Log_Record.i

inline void
ACE_Log_Record::encode (void)
{
  ACE_TRACE ("ACE_Log_Record::encode");
  this->type_ = htonl (this->type_);
  this->length_ = htonl (this->length_);
  this->time_stamp_ = ACE_Time_Value (htonl (this->time_stamp_.sec ()),
				      htonl (this->time_stamp_.usec ()));
  this->pid_ = htonl (this->pid_);
}

inline void
ACE_Log_Record::decode (void)
{
  ACE_TRACE ("ACE_Log_Record::decode");
  this->type_ = ntohl (this->type_);
  this->time_stamp_ = ACE_Time_Value (ntohl (this->time_stamp_.sec()),
                                      ntohl (this->time_stamp_.usec()));
  this->pid_ = ntohl (this->pid_);
  this->length_ = ntohl (this->length_);
}

inline long 
ACE_Log_Record::type (void) const
{
  ACE_TRACE ("ACE_Log_Record::type");
  return this->type_;
}

inline void 
ACE_Log_Record::type (long t)
{
  ACE_TRACE ("ACE_Log_Record::type");
  this->type_ = t;
}

inline long 
ACE_Log_Record::length (void) const
{
  ACE_TRACE ("ACE_Log_Record::length");
  return this->length_;
}

inline void 
ACE_Log_Record::length (long l)
{
  ACE_TRACE ("ACE_Log_Record::length");
  this->length_ = l;
}

inline const ACE_Time_Value &
ACE_Log_Record::time_stamp (void) const
{
  ACE_TRACE ("ACE_Log_Record::time_stamp");
  return this->time_stamp_;
}

inline void 
ACE_Log_Record::time_stamp (const ACE_Time_Value &ts)
{
  ACE_TRACE ("ACE_Log_Record::time_stamp");
  this->time_stamp_ = ts;
}

inline long 
ACE_Log_Record::pid (void) const
{
  ACE_TRACE ("ACE_Log_Record::pid");
  return this->pid_;
}

inline void 
ACE_Log_Record::pid (long p)
{
  ACE_TRACE ("ACE_Log_Record::pid");
  this->pid_ = p;
}

inline char *
ACE_Log_Record::msg_data (void)
{
  ACE_TRACE ("ACE_Log_Record::msg_data");
  return this->msg_data_;
}

