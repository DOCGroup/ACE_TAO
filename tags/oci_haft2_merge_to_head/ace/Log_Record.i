/* -*- C++ -*- */
// $Id$

// Log_Record.i
#include "ace/Global_Macros.h"
#include "ace/os_include/arpa/os_inet.h"
#include "ace/Time_Value.h"
#include "ace/OS.h"

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
  this->secs_ = htonl (this->secs_);
  this->usecs_ = htonl (this->usecs_);
  this->pid_ = htonl (this->pid_);
}

ASYS_INLINE void
ACE_Log_Record::decode (void)
{
  ACE_TRACE ("ACE_Log_Record::decode");
  ACE_Time_Value tv (ntohl (this->secs_),
                     ntohl (this->usecs_));

  this->secs_ = tv.sec ();
  this->usecs_ = tv.usec ();
  this->type_ = ntohl (this->type_);
  this->pid_ = ntohl (this->pid_);
  this->length_ = ntohl (this->length_);
}

ASYS_INLINE long
ACE_Log_Record::type (void) const
{
  ACE_TRACE ("ACE_Log_Record::type");
  return (long) this->type_;
}

ASYS_INLINE void
ACE_Log_Record::type (long t)
{
  ACE_TRACE ("ACE_Log_Record::type");
  this->type_ = (ACE_UINT32) t;
}

ASYS_INLINE long
ACE_Log_Record::length (void) const
{
  ACE_TRACE ("ACE_Log_Record::length");
  return (long) this->length_;
}

ASYS_INLINE void
ACE_Log_Record::length (long l)
{
  ACE_TRACE ("ACE_Log_Record::length");
  this->length_ = ACE_static_cast (ACE_UINT32, l);
}

ASYS_INLINE ACE_Time_Value 
ACE_Log_Record::time_stamp (void) const
{
  ACE_TRACE ("ACE_Log_Record::time_stamp");
  return ACE_Time_Value ((long) this->secs_, (long) this->usecs_);
}

ASYS_INLINE void
ACE_Log_Record::time_stamp (const ACE_Time_Value &ts)
{
  ACE_TRACE ("ACE_Log_Record::time_stamp");
  this->secs_ = (ACE_UINT32) ts.sec ();
  this->usecs_ = (ACE_UINT32) ts.usec ();
}

ASYS_INLINE long
ACE_Log_Record::pid (void) const
{
  ACE_TRACE ("ACE_Log_Record::pid");
  return (long) this->pid_;
}

ASYS_INLINE void
ACE_Log_Record::pid (long p)
{
  ACE_TRACE ("ACE_Log_Record::pid");
  this->pid_ = (ACE_UINT32) p;
}

ASYS_INLINE const ACE_TCHAR *
ACE_Log_Record::msg_data (void) const
{
  ACE_TRACE ("ACE_Log_Record::msg_data");
  return this->msg_data_;
}

ASYS_INLINE size_t
ACE_Log_Record::msg_data_len (void) const
{
  ACE_TRACE ("ACE_Log_Record::msg_data_len");
  return ACE_OS::strlen (this->msg_data_) + 1;
}
