//$Id$
#include "ace/OS.h"
#include "ace/UUID.h"

UUID_node::UUID_node (void)
{
  for (int i = 0; i < 6; i++)
    {
      nodeID_ [i] = 0;
    }
}

UUID_node::NodeID&
UUID_node::nodeID (void)
{
  return nodeID_;
}

void
UUID_node::nodeID (NodeID& nodeID)
{
  for (int i = 0; i < 6; i++)
    {
      nodeID_ [i] = nodeID [i];
    }
}

ACE_UUID ACE_UUID::NIL_UUID;

/// Construct a nil UUID. Such a UUID has every one of it's data
/// elements set to zero.
ACE_UUID::ACE_UUID()
  : timeLow_ (0),
    timeMid_ (0),
    timeHiAndVersion_ (0),
    clockSeqHiAndReserved_ (0),
    clockSeqLow_ (0),
    as_string_ (0)
{
  ACE_NEW (node_,
           UUID_node);

  node_release_ = 1;
}

ACE_UUID::ACE_UUID(const ACE_UUID &right)
  : timeLow_ (0),
    timeMid_ (0),
    timeHiAndVersion_ (0),
    clockSeqHiAndReserved_ (0),
    clockSeqLow_ (0),
    as_string_ (0)
{
  node_release_ = 0;
  *this = right;
}

/// Construct a UUID from a string representation of an UUID.
ACE_UUID::ACE_UUID (const ACE_CString& uuid_string)
  : timeLow_ (0),
    timeMid_ (0),
    timeHiAndVersion_ (0),
    clockSeqHiAndReserved_ (0),
    clockSeqLow_ (0),
    as_string_ (0)
{
  ACE_NEW (node_,
           UUID_node);

  node_release_ = 1;

  ACE_TRACE ("ACE_UUID::ACE_UUID");

  if (uuid_string.length() < NIL_UUID.to_string()->length())
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%N ACE_UUID::ACE_UUID - "
                  "IllegalArgument(incorrect string length)"));
      return;
    }

  /// Special case for the nil UUID.
  if (uuid_string == *NIL_UUID.to_string())
    {
      *this = NIL_UUID;
      return;
    }

  int timeLow;
  int timeMid;
  int timeHiAndVersion;
  int clockSeqHiAndReserved;
  int clockSeqLow;
  int node [UUID_node::NODE_ID_SIZE];
  char thr_pid_buf [BUFSIZ];

  if (uuid_string.length() == NIL_UUID.to_string()->length())
    {
      // This might seem quite strange this being in ACE, but it seems to
      // be a bit difficult to write a facade for ::sscanf because some
      // compilers dont support vsscanf, including MSVC. It appears that
      // most platforms support sscanf though so we need to use it
      // directly.
#if defined (ACE_HAS_PACE)
      int nScanned = ::pace_sscanf(uuid_string.c_str(),
                                   "%8x-%4x-%4x-%2x%2x-%2x%2x%2x%2x%2x%2x",
                                   &timeLow,
                                   &timeMid,
                                   &timeHiAndVersion,
                                   &clockSeqHiAndReserved,
                                   &clockSeqLow,
                                   &node[0],
                                   &node[1],
                                   &node[2],
                                   &node[3],
                                   &node[4],
                                   &node[5]
                                   );

#else

      int nScanned = ::sscanf(uuid_string.c_str(),
                              "%8x-%4x-%4x-%2x%2x-%2x%2x%2x%2x%2x%2x",
                              &timeLow,
                              &timeMid,
                              &timeHiAndVersion,
                              &clockSeqHiAndReserved,
                              &clockSeqLow,
                              &node[0],
                              &node[1],
                              &node[2],
                              &node[3],
                              &node[4],
                              &node[5]
                              );
#endif
      if (nScanned != 11)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "ACE_UUID::ACE_UUID - "
                      "IllegalArgument(invalid string representation)"));
          return;
        }
    }
  else
    {
#if defined (ACE_HAS_PACE)
      int nScanned = ::pace_sscanf(uuid_string.c_str(),
                                   "%8x-%4x-%4x-%2x%2x-%2x%2x%2x%2x%2x%2x-%s",
                                   &timeLow,
                                   &timeMid,
                                   &timeHiAndVersion,
                                   &clockSeqHiAndReserved,
                                   &clockSeqLow,
                                   &node[0],
                                   &node[1],
                                   &node[2],
                                   &node[3],
                                   &node[4],
                                   &node[5],
                                   thr_pid_buf
                                   );

#else

      int nScanned = ::sscanf(uuid_string.c_str(),
                              "%8x-%4x-%4x-%2x%2x-%2x%2x%2x%2x%2x%2x-%s",
                              &timeLow,
                              &timeMid,
                              &timeHiAndVersion,
                              &clockSeqHiAndReserved,
                              &clockSeqLow,
                              &node[0],
                              &node[1],
                              &node[2],
                              &node[3],
                              &node[4],
                              &node[5],
                              thr_pid_buf
                              );
#endif
      if (nScanned != 12)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "ACE_UUID::ACE_UUID - "
                      "IllegalArgument(invalid string representation)"));
          return;
        }
    }

  this->timeLow_ = ACE_static_cast (ACE_UINT64, timeLow);
  this->timeMid_ = ACE_static_cast (ACE_UINT16, timeMid);
  this->timeHiAndVersion_ = ACE_static_cast (ACE_UINT16, timeHiAndVersion);
  this->clockSeqHiAndReserved_ = ACE_static_cast (u_char, clockSeqHiAndReserved);
  this->clockSeqLow_ = ACE_static_cast (u_char, clockSeqLow);

  UUID_node::NodeID nodeID;
  for (int i = 0; i < UUID_node::NODE_ID_SIZE; i++)
    nodeID [i] = ACE_static_cast (u_char, node[i]);

  this->node_->nodeID (nodeID);

  // Support varient 10- only
  if ((this->clockSeqHiAndReserved_ & 0xc0) != 0x80 && (this->clockSeqHiAndReserved_ & 0xc0) != 0xc0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ACE_UUID_Impl::ACE_UUID_Impl - "
                  "IllegalArgument(unsupported variant)"));
      return;
    }

  /// Support versions 1, 3, and 4 only
  ACE_UINT16 V1 = this->timeHiAndVersion_;

  if ((V1 & 0xF000) != 0x1000 &&
      (V1 & 0xF000) != 0x3000 &&
      (V1 & 0xF000) != 0x4000)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ACE_UUID::ACE_UUID - "
                  "IllegalArgument(unsupported version)"));
      return;
    }
  if ((this->clockSeqHiAndReserved_ & 0xc0) == 0xc0)
    {
      if (uuid_string.length() == NIL_UUID.to_string()->length())
        {
          ACE_DEBUG ((LM_DEBUG,
                      "ACE_UUID::ACE_UUID - "
                      "IllegalArgument (Missing Thread and Process Id)"));
          return;
        }
      ACE_CString thr_pid_str (thr_pid_buf);
      int pos = thr_pid_str.find ('-');
      if (pos == -1)
        ACE_DEBUG ((LM_DEBUG,
                    "ACE_UUID::ACE_UUID - "
                    "IllegalArgument (Thread and Process Id format incorrect)"));

      this->thr_id_ = thr_pid_str.substr (0, pos);
      this->pid_ = thr_pid_str.substr (pos+1, thr_pid_str.length ()-pos-1);
    }
}

ACE_UUID::~ACE_UUID (void)
{
  if (node_release_)
    delete node_;

  if (as_string_ != 0)
    delete as_string_;
}

const ACE_CString*
ACE_UUID::to_string (void)
{
  /// Only compute the string representation once.
  if (as_string_ == 0)
    {
      // Get a buffer exactly the correct size. Use the nil UUID as a
      // gauge.  Don't forget the trailing nul.
      int UUID_STRING_LENGTH = 36 + thr_id_.length () + pid_.length ();
      char *buf;

      if ((thr_id_.length () != 0) && (pid_.length () != 0))
        {
          UUID_STRING_LENGTH += 2; //for '-'
          ACE_NEW_RETURN (buf,
                          char[UUID_STRING_LENGTH + 1],
                          0);

          ACE_OS::sprintf(buf,
                          "%8.8x-%4.4x-%4.4x-%2.2x%2.2x-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x-%s-%s",
                          this->timeLow_,
                          this->timeMid_,
                          this->timeHiAndVersion_,
                          this->clockSeqHiAndReserved_,
                          this->clockSeqLow_,
                          (this->node_->nodeID ()) [0],
                          (this->node_->nodeID ()) [1],
                          (this->node_->nodeID ()) [2],
                          (this->node_->nodeID ()) [3],
                          (this->node_->nodeID ()) [4],
                          (this->node_->nodeID ()) [5],
                          thr_id_.c_str (),
                          pid_.c_str ()
                          );
        }
      else
        {
          ACE_NEW_RETURN (buf,
                          char[UUID_STRING_LENGTH + 1],
                          0);

          ACE_OS::sprintf(buf,
                          "%8.8x-%4.4x-%4.4x-%2.2x%2.2x-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x",
                          this->timeLow_,
                          this->timeMid_,
                          this->timeHiAndVersion_,
                          this->clockSeqHiAndReserved_,
                          this->clockSeqLow_,
                          (this->node_->nodeID ()) [0],
                          (this->node_->nodeID ()) [1],
                          (this->node_->nodeID ()) [2],
                          (this->node_->nodeID ()) [3],
                          (this->node_->nodeID ()) [4],
                          (this->node_->nodeID ()) [5]
                          );
        }
      ACE_NEW_RETURN (this->as_string_,
                      ACE_CString (buf, UUID_STRING_LENGTH),
                      0);
      delete buf;
    }

  return as_string_;
}

ACE_UUID_Generator::ACE_UUID_Generator ()
  : timeLast_ (0)
{
  ACE_NEW (lock_,
           ACE_SYNCH_MUTEX);
  destroy_lock_ = 1;
}

ACE_UUID_Generator::~ACE_UUID_Generator()
{
  if (destroy_lock_)
    delete lock_;
}

void
ACE_UUID_Generator::init (void)
{
  ACE_OS::macaddr_node_t macaddress;
  int result =
    ACE_OS::getmacaddress (&macaddress);

  UUID_node::NodeID nodeID;
  if (result != -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%02X-%02X-%02X-%02X-%02X-%02X\n",
                  macaddress.node [0],
                  macaddress.node [1],
                  macaddress.node [2],
                  macaddress.node [3],
                  macaddress.node [4],
                  macaddress.node [5]));

      ACE_OS::memcpy (&nodeID,
                      macaddress.node,
                      sizeof (nodeID));
    }
  else
    {
      nodeID [0] = ACE_static_cast (u_char, ACE_OS::rand());
      nodeID [1] = ACE_static_cast (u_char, ACE_OS::rand());
      nodeID [2] = ACE_static_cast (u_char, ACE_OS::rand());
      nodeID [3] = ACE_static_cast (u_char, ACE_OS::rand());
      nodeID [4] = ACE_static_cast (u_char, ACE_OS::rand());
      nodeID [5] = ACE_static_cast (u_char, ACE_OS::rand());
    }

  this->get_timestamp (timeLast_);

  {
    ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, *lock_);
    uuid_state_.timestamp = timeLast_;
    uuid_state_.node.nodeID (nodeID);
  }
}


void
ACE_UUID_Generator::generateUUID (ACE_UUID& uuid,ACE_UINT16 version, u_char variant)
{
  UUID_time timestamp;
  this->get_timestamp (timestamp);


  // Construct a Version 1 UUID with the information in the arguements.
  uuid.timeLow (ACE_static_cast (ACE_UINT32, timestamp & 0xFFFFFFFF));
  uuid.timeMid (ACE_static_cast(ACE_UINT16, (timestamp >> 32) & 0xFFFF));


  ACE_UINT16 tHAV = ACE_static_cast (ACE_UINT16, (timestamp >> 48) & 0xFFFF);
  tHAV |= (version << 12);
  uuid.timeHiAndVersion (tHAV);

  u_char cseqHAV;
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, mon, *lock_);
    uuid.clockSeqLow (uuid_state_.clockSequence & 0xFF);
    cseqHAV = (uuid_state_.clockSequence & 0x3f00) >> 8;
    uuid_state_.timestamp = timestamp;
  }

  cseqHAV |= variant;
  uuid.clockSeqHiAndReserved (cseqHAV);
  uuid.node (&(uuid_state_.node));

  if (variant == 0xc0)
    {
      ACE_thread_t thr_id = ACE_OS::thr_self ();
      ACE_hthread_t thr_handle;
      ACE_OS::thr_self (thr_handle);
      ACE_Thread_ID thread_id (thr_id, thr_handle);
      char buf [BUFSIZ];
      thread_id.to_string (buf);
      uuid.thr_id (buf);

      ACE_OS::sprintf (buf,
                       "%d",
                       ACE_static_cast (int, ACE_OS::getpid ()));
      uuid.pid (buf);
    }
}

ACE_UUID*
ACE_UUID_Generator::generateUUID (ACE_UINT16 version, u_char variant)
{
  ACE_UUID* uuid;
  ACE_NEW_RETURN (uuid,
                  ACE_UUID,
                  0);

  this->generateUUID (*uuid, version, variant);
  return uuid;
}

/// Obtain a new timestamp. If UUID's are being generated too quickly
/// the clock sequence will be incremented
void
ACE_UUID_Generator::get_timestamp (UUID_time& timestamp)
{
  this->get_systemtime(timestamp);

  /// Account for the clock being set back. Increment the clock
  /// sequence.
  if (timestamp <= timeLast_)
    {
      {
        ACE_GUARD (ACE_SYNCH_MUTEX, mon, *lock_);
        uuid_state_.clockSequence = (uuid_state_.clockSequence + 1) & ACE_UUID_CLOCK_SEQ_MASK;
      }
    }

  /// If the system time ticked since the last UUID was generated. Set
  /// the clock sequence back.
  else if (timestamp > timeLast_)
    {
      {
        ACE_GUARD (ACE_SYNCH_MUTEX, mon, *lock_);
        uuid_state_.clockSequence = 0;
      }
    }

  timeLast_ = timestamp;
}

/**
 * ACE_Time_Value is in POSIX time, seconds since Jan 1, 1970. UUIDs use
 * time in 100ns ticks since 15 October 1582. The difference is:
 *   15 Oct 1582 - 1 Jan 1600: 17 days in Oct, 30 in Nov,  31 in Dec +
 *       17 years and 4 leap days (1584, 88, 92 and 96)
 *   1 Jan 1600 - 1 Jan 1900: 3 centuries + 73 leap days ( 25 in 17th cent.
 *       and 24 each in 18th and 19th centuries)
 *   1 Jan 1900 - 1 Jan 1970: 70 years + 17 leap days.
 * This adds up, in days: (17+30+31+365*17+4)+(365*300+73)+(365*70+17) or
 * 122192928000000000U (0x1B21DD213814000) 100 ns ticks.
 */
void
ACE_UUID_Generator::get_systemtime(UUID_time& timestamp)
{
  const UUID_time timeOffset = 0;
  //const UUID_time timeOffset = 0x1B21DD213814000;

  /// Get the time of day, convert to 100ns ticks then add the offset.
  ACE_Time_Value now = ACE_OS::gettimeofday();
  UUID_time time = now.sec() * 10000000 + now.usec() * 10;
  timestamp = time + timeOffset;
}

ACE_SYNCH_MUTEX*
ACE_UUID_Generator::lock (void)
{
  return this->lock_;
}

ACE_SYNCH_MUTEX*
ACE_UUID_Generator::lock (ACE_SYNCH_MUTEX* lock,
                          int release_lock_)
{
  if (destroy_lock_)
    delete lock_;

  ACE_SYNCH_MUTEX* prev_lock = this->lock_;
  this->lock_ = lock;
  this->destroy_lock_ = release_lock_;
  return prev_lock;
}

#if !defined (__ACE_INLINE__)
#include "ace/UUID.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton <ACE_UUID_Generator, ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <ACE_UUID_Generator, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
