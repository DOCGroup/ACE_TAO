//$Id$
#include "ace/OS.h"
#include "ace/Singleton.h"
#include "ace/UUID.h"
//#include "stdafx.h"
#include <Windows.h>
#include <rpc.h>
#include <rpcdce.h>

UUID_node::UUID_Node (void)
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

  		
  nodeID_ [0] = nodeID [0];
  nodeID_ [1] = nodeID [1];
  nodeID_ [2] = nodeID [2];
  nodeID_ [3] = nodeID [3];
  nodeID_ [4] = nodeID [4];
  nodeID_ [5] = nodeID [5];
}

ACE_UUID ACE_UUID::NIL_UUID;

/*
ACE_UUID::UUID_t nil_uuid_t =
    {0, 0, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}};
*/

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
}

ACE_UUID::ACE_UUID(const ACE_UUID &right)
  : timeLow_ (0),
    timeMid_ (0),
    timeHiAndVersion_ (0),
    clockSeqHiAndReserved_ (0),
    clockSeqLow_ (0),
    as_string_ (0)
{
  *this = right;
}

/// Construct a UUID from a string representation of an UUID.
ACE_UUID::ACE_UUID (const ACE_SString& uuid_string)
  : timeLow_ (0),
    timeMid_ (0),
    timeHiAndVersion_ (0),
    clockSeqHiAndReserved_ (0),
    clockSeqLow_ (0),
    as_string_ (0)
{
  ACE_TRACE ("ACE_UUID::ACE_UUID");
 
  if (uuid_string.length() != NIL_UUID.to_string()->length()) 
    {
      ACE_DEBUG ((LM_DEBUG,
		  "ACE_UUID::ACE_UUID - "
		  "IllegalArgument(incorrect string length)"));
    }
  
  /// Special case for the nil UUID.
  if (uuid_string == *NIL_UUID.to_string()) 
    {
      *this = NIL_UUID;
      return;
    }
  
  long timeLow;
  int  timeMid;
  int  timeHiAndVersion;
  int  clockSeqHiAndReserved;
  int  clockSeqLow;
  int  node [UUID_node::NODE_ID_SIZE];
  
  // This might seem quite strange seeing as this is ACE, but it 
  // seems to be a bit difficult to write a facade for ::sscanf because
  // some compilers dont support vsscanf, including MSVC. It appears
  // that most platforms support sscanf though so we need to use it
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
    }

  this->timeLow_ = ACE_static_cast (ACE_UINT32, timeLow);
  this->timeMid_ = ACE_static_cast (ACE_UINT16, timeMid);
  this->timeHiAndVersion_ = ACE_static_cast (ACE_UINT16, timeHiAndVersion);
  this->clockSeqHiAndReserved_ = ACE_static_cast (u_char , clockSeqHiAndReserved);
  this->clockSeqLow_ = ACE_static_cast (u_char , clockSeqLow);

  UUID_node::NodeID nodeID;
  nodeID [0] = ACE_static_cast (u_char , node[0]);
  nodeID [1] = ACE_static_cast (u_char , node[1]);
  nodeID [2] = ACE_static_cast (u_char , node[2]);
  nodeID [3] = ACE_static_cast (u_char , node[3]);
  nodeID [4] = ACE_static_cast (u_char , node[4]);
  nodeID [5] = ACE_static_cast (u_char , node[5]);

  this->node_->nodeID (nodeID);

  // Support varient 10- only
  if ((this->clockSeqHiAndReserved_ & 0xc0) != 0x80)
    {
      ACE_DEBUG ((LM_DEBUG,
		  "ACE_UUID_Impl::ACE_UUID_Impl - "
		  "IllegalArgument(unsupported variant)"));
    }
  
  /// Support versions 1, 3, and 4 only
  if (((this->timeHiAndVersion & 0xF000) != 0x1000) &&
      ((this->timeHiAndVersion & 0xF000) != 0x3000) &&
      ((this->timeHiAndVersion & 0xF000) != 0x4000)) 
    {
      ACE_DEBUG ((LM_DEBUG,
		  "ACE_UUID::ACE_UUID - "
		  "IllegalArgument(unsupported version)"));
    }
}

ACE_UUID::~ACE_UUID()
{
}

const ACE_SString*
ACE_UUID::to_string (void)
{
  /// Only compute the string representation once.
  if (as_string_ == 0) 
    {     
      // Get a buffer exactly the correct size. Use the nil UUID as a gauge.
      // Don't forget the trailing nul.   
      int UUID_STRING_LENGTH = 36; ///+ uuid_.thr_id->length () + uuid_.pid->length ();

      char *buf;
      ACE_NEW_RETURN (buf, 
		      char[UUID_STRING_LENGTH + 1],
		      0);
      
      ACE_OS::sprintf(buf,
		      //"%8.8x-%4.4x-%4.4x-%2.2x%2.2x-%s-%s-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x",
		      "%8.8x-%4.4x-%4.4x-%2.2x%2.2x-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x",
		      this->timeLow_,
		      this->timeMid_,
		      this->timeHiAndVersion_,
		      this->clockSeqHiAndReserved_,
		      this->clockSeqLow_,
		      // uuid_.thr_id->c_str (),
		      // uuid_.pid->c_str (),
		      (this->node_->nodeID ()) [0],
		      (this->node_->nodeID ()) [1],
		      (this->node_->nodeID ()) [2],
		      (this->node_->nodeID ()) [3],
		      (this->node_->nodeID ()) [4],
		      (this->node_->nodeID ()) [5]);

      this->as_string_ = new ACE_SString (buf, UUID_STRING_LENGTH + 1);
    }

  return as_string_;
}

ACE_UUID_Generator::ACE_UUID_Generator(void) 
  : timeLast_ (0),
    lock_ (0)
{   
}

ACE_UUID_Generator::~ACE_UUID_Generator()
{
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
      // Generate random node. This is pretty bad, but until
      // ACE has a strong prng, this is what we can do.
      nodeID [0] = static_cast<u_char>(ACE_OS::rand());
      nodeID [1] = static_cast<u_char>(ACE_OS::rand());
      nodeID [2] = static_cast<u_char>(ACE_OS::rand());
      nodeID [3] = static_cast<u_char>(ACE_OS::rand());
      nodeID [4] = static_cast<u_char>(ACE_OS::rand());
      nodeID [5] = static_cast<u_char>(ACE_OS::rand());
    }

  uuid_state_.node.nodeID (nodeID);
  
  
  ACE_DEBUG ((LM_DEBUG,
	      "%02X-%02X-%02X-%02X-%02X-%02X\n",
	      (uuid_state_.node.nodeID ()) [0],
	      (uuid_state_.node.nodeID ()) [1],
	      (uuid_state_.node.nodeID ()) [2],
	      (uuid_state_.node.nodeID ()) [3],
	      (uuid_state_.node.nodeID ()) [4],
	      (uuid_state_.node.nodeID ()) [5]));
  
  if (lock_ != 0)
    {
      ACE_GUARD (ACE_Lock, mon, *lock_);
      this->get_timestamp (timeLast_);
      uuid_state_.timestamp = timeLast_;
    }
  else
    {
      this->get_timestamp (timeLast_);
      uuid_state_.timestamp = timeLast_;
    }

}


void
ACE_UUID_Generator::generateUUID (ACE_UUID& uuid)
{
  UUID_time timestamp;
  this->get_timestamp (timestamp);

  // Construct a Version 1 UUID with the information in the arguements.
  uuid.timeLow (ACE_static_cast (ACE_UINT32, timestamp & 0xFFFFFFFF));
  uuid.timeMid (ACE_static_cast(ACE_UINT16, (timestamp >> 32) & 0xFFFF));

  ACE_UINT16 tHAV = ACE_static_cast (ACE_UINT16, (timestamp >> 48) & 0xFFFF);
  tHAV |= (1 << 12);
  uuid.timeHiAndVersion (tHAV);

  u_char cseqHAV = (uuid_state_.clockSequence & 0x3f00) >> 8;
  cseqHAV |= 0x80;

  if (lock_ != 0)
    {
      ACE_GUARD (ACE_Lock, mon, *lock_);
      uuid.clockSeqLow (uuid_state_.clockSequence & 0xFF);
      uuid.clockSeqHiAndReserved (cseqHAV);
    }
  else 
    {
      uuid.clockSeqLow (uuid_state_.clockSequence & 0xFF);
      uuid.clockSeqHiAndReserved (cseqHAV);
    }

  uuid.node (&(uuid_state_.node));

  /*
  ACE_thread_t thr_id = ACE_OS::thr_self ();
  ACE_hthread_t thr_handle;
  ACE_OS::thr_self (thr_handle);
  ACE_Thread_ID thread_id (thr_id, thr_handle);
  char buf [BUFSIZ];
  thread_id.to_string (buf);

  uuid.thr_id = new ACE_CString (buf);
  
  ACE_OS::sprintf (buf, 
		   "%d",
		   ACE_static_cast (int, ACE_OS::getpid ()));
  
  uuid.pid = new ACE_CString (buf);
  */

  if (lock_ != 0)
    {
      ACE_GUARD (ACE_Lock, mon, *lock_);
      uuid_state_.timestamp = timestamp;
    }
  else 
    uuid_state_.timestamp = timestamp;
}

ACE_UUID* 
ACE_UUID_Generator::generateUUID (void)
{
    ACE_UUID* uuid;
    ACE_NEW_RETURN (uuid,
		    ACE_UUID,
		    0);

    this->generateUUID (*uuid);
    return uuid;
}

/// Obtain a new timestamp. If UUID's are being generated to quickly
/// this method will 'spin' until enough time has elapsed.
void 
ACE_UUID_Generator::get_timestamp (UUID_time& timestamp)
{   
  this->get_systemtime(timestamp);
  
  /// Account for the clock being set back. Increment the clock sequence.
  if (timestamp <= timeLast_) 
    {
      if (lock_ != 0)
	{
	  ACE_GUARD (ACE_Lock, mon, *lock_);
	  uuid_state_.clockSequence = (uuid_state_.clockSequence + 1) & ACE_UUID_CLOCK_SEQ_MASK;
	}
      else
	{
	  uuid_state_.clockSequence = (uuid_state_.clockSequence + 1) & ACE_UUID_CLOCK_SEQ_MASK;
	}
    }

  /// If the system time ticked since the last UUID was generated. Set the clock sequence back.
  else if (timestamp > timeLast_)
    {
      if (lock_ != 0)
	{
	  ACE_GUARD (ACE_Lock, mon, *lock_);
	  uuid_state_.clockSequence = 0;
	}
      else
	{
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
  const UUID_time timeOffset = 0x1B21DD213814000;
  
  /// Get the time of day, convert to 100ns ticks then add the offset.
  ACE_Time_Value now = ACE_OS::gettimeofday();
  UUID_time time = now.sec() * 10000000 + now.usec() * 10;
  timestamp = time + timeOffset;
}

#if !defined (__ACE_INLINE__)
#include "ace/UUID.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<ACE_UUID_Generator, ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<ACE_UUID_Generator, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
