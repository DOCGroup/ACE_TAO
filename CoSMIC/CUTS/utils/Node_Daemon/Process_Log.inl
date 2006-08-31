// $Id$

//=============================================================================
/*
 * Process_Log_Entry
 */
//=============================================================================

//
// is_active
//
CUTS_INLINE
bool Process_Log_Entry::is_active (void) const
{
  return ACE_BIT_ENABLED (this->flags_, PLE_ACTIVE);
}

//
// is_localhost
//
CUTS_INLINE
bool Process_Log_Entry::is_localhost (void) const
{
  return ACE_BIT_ENABLED (this->flags_, PLE_ACTIVE);
}

//
// port
//
CUTS_INLINE
u_short Process_Log_Entry::port (void) const
{
  return this->port_;
}

//
// pid
//
CUTS_INLINE
pid_t Process_Log_Entry::pid (void) const
{
  return this->pid_;
}

//
// operator ==
//
CUTS_INLINE
bool Process_Log_Entry::operator == (const Process_Log_Entry & entry)
{
  return this->flags_ == entry.flags_ &&
         this->pid_ == entry.pid_ &&
         this->port_ == entry.port_;
}

//=============================================================================
/*
 * Process_Log
 */
//=============================================================================

//
// log_file
//
CUTS_INLINE
const ACE_CString & Process_Log::log_file (void) const
{
  return this->log_file_;
}
