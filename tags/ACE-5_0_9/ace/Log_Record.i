/* -*- C++ -*- */
// $Id$

// Log_Record.i

ASYS_INLINE
ACE_Log_Record::~ACE_Log_Record (void)
{
}

ASYS_INLINE void
ACE_Log_Record::encode (void)
{
  ACE_TRACE ("ACE_Log_Record::encode");
  this->length_ = htonl (this->length_);
  this->type_ = htonl (this->type_);
  // Make sure we don't enclose the sec() and usec() fields until
  // they've been normalized.
  this->time_stamp_.sec (htonl (this->time_stamp_.sec ()));
  this->time_stamp_.usec (htonl (this->time_stamp_.usec ()));
  this->pid_ = htonl (this->pid_);
}

ASYS_INLINE void
ACE_Log_Record::decode (void)
{
  ACE_TRACE ("ACE_Log_Record::decode");
  this->time_stamp_ = ACE_Time_Value (ntohl (this->time_stamp_.sec()),
                                      ntohl (this->time_stamp_.usec()));
  this->type_ = ntohl (this->type_);
  this->pid_ = ntohl (this->pid_);
  this->length_ = ntohl (this->length_);
}

ASYS_INLINE long
ACE_Log_Record::type (void) const
{
  ACE_TRACE ("ACE_Log_Record::type");
  return this->type_;
}

ASYS_INLINE void
ACE_Log_Record::type (long t)
{
  ACE_TRACE ("ACE_Log_Record::type");
  this->type_ = t;
}

ASYS_INLINE long
ACE_Log_Record::length (void) const
{
  ACE_TRACE ("ACE_Log_Record::length");
  return this->length_;
}

ASYS_INLINE void
ACE_Log_Record::length (long l)
{
  ACE_TRACE ("ACE_Log_Record::length");
  this->length_ = ACE_static_cast (ACE_UINT32, l);
}

ASYS_INLINE const ACE_Time_Value &
ACE_Log_Record::time_stamp (void) const
{
  ACE_TRACE ("ACE_Log_Record::time_stamp");
  return this->time_stamp_;
}

ASYS_INLINE void
ACE_Log_Record::time_stamp (const ACE_Time_Value &ts)
{
  ACE_TRACE ("ACE_Log_Record::time_stamp");
  this->time_stamp_ = ts;
}

ASYS_INLINE long
ACE_Log_Record::pid (void) const
{
  ACE_TRACE ("ACE_Log_Record::pid");
  return this->pid_;
}

ASYS_INLINE void
ACE_Log_Record::pid (long p)
{
  ACE_TRACE ("ACE_Log_Record::pid");
  this->pid_ = p;
}

ASYS_INLINE ASYS_TCHAR *
ACE_Log_Record::msg_data (void)
{
  ACE_TRACE ("ACE_Log_Record::msg_data");
  return this->msg_data_;
}
