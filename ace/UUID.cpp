$Id$
#include <cstring>
#include <cstdio>
#include <new>
#include <vector>
#include "ace/DLL.h"
#include "ace/OS.h"
#include "ace/PI_Malloc.h"
//#include "JCAFCore/src/Common/Exception.h"
#include "ace/Singleton.h"

// UUID
#include "ace/UUID.h"


ACE_UUID_Admin::ACE_UUID_Admin()
  : pimpl( new ACE_UUID_Admin_Impl())
{
}

ACE_UUID_Admin* 
ACE_UUID_Admin::instance ()
{
  return ACE_Singleton< ACE_UUID_Admin, ACE_Null_Mutex>::instance();
}

void 
ACE_UUID_Admin::setPersistenceFilename (const ACE_SString& filename)
{
  pimpl->setPersistenceFilename( filename);
}

const ACE_SString& 
ACE_UUID_Admin::getPersistenceFilename () const
{
  return pimpl->getPersistenceFilename();
}

const ACE_SString&
ACE_UUID_Admin::getDefaultPersistenceFilename ()
{
  return ACE_UUID_Admin_Impl::getDefaultPersistenceFilename();
}

//UUID::NewUUID UUID::newUUID;

ACE_UUID::ACE_UUID()
  : pimpl( ACE_UUID_Impl::nilACE_UUID_Impl)
{
}


ACE_UUID::ACE_UUID(const ACE_UUID &right)
{
  pimpl = right.pimpl;
}

ACE_UUID::ACE_UUID (ACE_UUID_Impl::UUID_t &uuid)//const NewUUID& newUUID)
  : pimpl( new ACE_UUID_Impl(uuid))
{
}

ACE_UUID::ACE_UUID (const ACE_SString& uuid)
  : pimpl( new ACE_UUID_Impl( uuid))
{
}

ACE_UUID::~ACE_UUID()
{
}

ACE_UUID & 
ACE_UUID::operator=(const ACE_UUID &right)
{
  pimpl = right.pimpl;
  return *this;
}


bool 
ACE_UUID::operator==(const ACE_UUID &right) const
{
  return *pimpl == *right.pimpl;
}

bool 
ACE_UUID::operator!=(const ACE_UUID &right) const
{
  return !(*this == right);
}


bool 
ACE_UUID::operator<(const ACE_UUID &right) const
{
  return *pimpl < *right.pimpl;
}

bool 
ACE_UUID::operator>(const ACE_UUID &right) const
{
  return *right.pimpl < *pimpl;
}

bool 
ACE_UUID::operator<=(const ACE_UUID &right) const
{
  return !(*this > right);
}


bool 
ACE_UUID::operator>=(const ACE_UUID &right) const
{
  return !(*this < right);
}

ACE_SString 
ACE_UUID::toString () const
{
  return pimpl->toString();
}

bool 
ACE_UUID_Impl::UUID_node_t::operator == (const UUID_node_t& right) const
{
  for (size_t i = 0; i < (sizeof( nodeID) / sizeof( nodeID[0])); ++i)
    if (nodeID[i] != right.nodeID[i])
      return false;
  
  return true;
}

bool 
ACE_UUID_Impl::UUID_node_t::operator != (const UUID_node_t& right) const
{
  return !(*this == right);
}

bool 
ACE_UUID_Impl::UUID_node_t::operator < (const UUID_node_t& right) const
{
  for (size_t i = 0; i < (sizeof( nodeID) / sizeof( nodeID[0])); ++i)
    if (nodeID[i] < right.nodeID[i])
      return true;

  return false;

}

ACE_Refcounted_Auto_Ptr<class ACE_UUID_Impl,ACE_Thread_Mutex> ACE_UUID_Impl::nilACE_UUID_Impl = new ACE_UUID_Impl();

ACE_UUID_Impl::ACE_UUID_Impl()
{
  //begin ACE_UUID_Impl::ACE_UUID_Impl
  // Construct a nil UUID. Such a UUID has every one of it's data elements set
  // to zero. Also, since the nil UUID representation is shared by all instances
  // of nil UUID (there is just one) we can just go ahead and pre-compute the
  // string representation.
  uuid.timeLow = 0;
  uuid.timeMid = 0;
  uuid.timeHiAndVersion = 0;
  uuid.clockSeqHiAndReserved = 0;
  uuid.clockSeqLow = 0;
  ACE_OS::memset( &uuid.node, 0, sizeof( uuid.node));
  asString = "00000000-0000-0000-0000-000000000000";
}

ACE_UUID_Impl::ACE_UUID_Impl (UUID_t& uuid)//const UUID::NewUUID& )
{
  ACE_UUID_Generator::instance()->formatV1( uuid);
  this->uuid = uuid;
}

ACE_UUID_Impl::ACE_UUID_Impl (const ACE_SString& asString)
{
  if (asString.length() != nilACE_UUID_Impl->asString.length())
    //throw 
   ACE_DEBUG ((LM_DEBUG,
	       "IllegalArgument( ACE_UUID_Impl::ACE_UUID_Impl: incorrect string length)"));
    
  
  // Special case for the nil UUID.
  if (asString == nilACE_UUID_Impl->asString) 
    {
      uuid = nilACE_UUID_Impl->uuid;
      return;
    }
  
  int timeLow;
  int timeMid;
  int timeHiAndVersion;
  int clockSeqHiAndReserved;
  int clockSeqLow;
  int nodeID [UUID_node_t::nodeIDSize];
  int nScanned = ::sscanf(
			     asString.c_str(),
			     "%8x-%4x-%4x-%2x%2x-%2x%2x%2x%2x%2x%2x",
			     &timeLow,
			     &timeMid,
			     &timeHiAndVersion,
			     &clockSeqHiAndReserved,
			     &clockSeqLow,
			     nodeID + 0,
			     nodeID + 1,
			     nodeID + 2,
			     nodeID + 3,
			     nodeID + 4,
			     nodeID + 5);
  if (nScanned != 11)
    //throw 
    ACE_DEBUG ((LM_DEBUG,
		"IllegalArgument( ACE_UUID_Impl::ACE_UUID_Impl: invalid string representation);"));

  UUID_t uu;
  uu.timeLow = timeLow;
  uu.timeMid = timeMid;
  uu.timeHiAndVersion = timeHiAndVersion;
  uu.clockSeqHiAndReserved = clockSeqHiAndReserved;
  uu.clockSeqLow = clockSeqLow;
  uu.node.nodeID [0] = nodeID [0];
  uu.node.nodeID [1] = nodeID [1];
  uu.node.nodeID [2] = nodeID [2];
  uu.node.nodeID [3] = nodeID [3];
  uu.node.nodeID [4] = nodeID [4];
  uu.node.nodeID [5] = nodeID [5];

  // Support varient 10- only
  if ((uu.clockSeqHiAndReserved & 0xc0) != 0x80)
    // throw 
        ACE_DEBUG ((LM_DEBUG,
		    "IllegalArgument( ACE_UUID_Impl::ACE_UUID_Impl: unsupported variant)"));
  
  // Support versions 1, 3, and 4 only
  if (((uu.timeHiAndVersion & 0xF000) != 0x1000) &&
      ((uu.timeHiAndVersion & 0xF000) != 0x3000) &&
      ((uu.timeHiAndVersion & 0xF000) != 0x4000))
    //    throw
        ACE_DEBUG ((LM_DEBUG,
		    "IllegalArgument( ACE_UUID_Impl::ACE_UUID_Impl: unsupported version)"));
  
  uuid = uu;
  
}


ACE_UUID_Impl::~ACE_UUID_Impl()
{
}

bool 
ACE_UUID_Impl::operator == (const ACE_UUID_Impl& right) const
{
  if ((uuid.timeLow != right.uuid.timeLow) ||
      (uuid.timeMid != right.uuid.timeMid) ||
      (uuid.timeHiAndVersion != right.uuid.timeHiAndVersion) ||
      (uuid.clockSeqHiAndReserved != right.uuid.clockSeqHiAndReserved) ||
      (uuid.clockSeqLow != right.uuid.clockSeqLow))
    return false;

  if (uuid.node != right.uuid.node)
    return false;

  return true;
}

bool 
ACE_UUID_Impl::operator < (const ACE_UUID_Impl& right) const
{
  if ((uuid.timeLow < right.uuid.timeLow) ||
      (uuid.timeMid < right.uuid.timeMid) ||
      (uuid.timeHiAndVersion < right.uuid.timeHiAndVersion) ||
      (uuid.clockSeqHiAndReserved < right.uuid.clockSeqHiAndReserved) ||
      (uuid.clockSeqLow < right.uuid.clockSeqLow))
    return true;
  if (uuid.node < right.uuid.node)
    return true;

  return false;
}



ACE_SString 
ACE_UUID_Impl::toString () const
{
  // Only compute the string representation once.
  if (asString.length() == 0) 
    {
      
      // Note. It would be nice to use ACE_SStringstream for this. But, for
      // compatability with gcc we must used std::sprintf. Fix this when all
      // supported platforms implement ACE_SStringstream.
      
      // Get a buffer exactly the correct size. Use the nil UUID as a gauge.
      // Don't forget the trailing nul.
	  int length = nilACE_UUID_Impl->asString.length();
      char buf[ 36 + 1];			     
	//std::vector<char> buf( ACE_UUID_Impl::nilACE_UUID_Impl->asString.size() + 1);      
      int base = ::sprintf(
			      buf,
			      "%8.8x-%4.4x-%4.4x-%2.2x%2.2x-",
			      uuid.timeLow,
			      uuid.timeMid,
			      uuid.timeHiAndVersion,
			      uuid.clockSeqHiAndReserved,
			      uuid.clockSeqLow);
      for (size_t i = 0; i < sizeof(uuid.node); ++i) 
	{
	  
	  base += ::sprintf(
			       &buf[base],
			       "%2.2x",
			       uuid.node.nodeID [i]);
	  
	}
	 this->asString.rep(buf);
      
    }
  return asString;
}

ACE_UUID_Generator* 
ACE_UUID_Generator::instance()
{
  return ACE_Singleton< ACE_UUID_Generator, ACE_Null_Mutex >::instance();
}

ACE_UUID_Generator::ACE_UUID_Generator() :
  timeLast( 0),
  uuidState( 0),
  lock( "ACE_UUID_Impl")
{
  // TODO: should throw exception if can't lock
  ACE_Guard<ACE_Process_Mutex> mon( lock);
  if (mon.locked() == 0)
    return;

  ACE_UUID_Impl::UUID_node_t currentNode;

  const char* const bindName = "UUIDState";

  getTimestamp( timeLast);
  getIEEENodeIdentifier( currentNode);

  //shmemAllocator.reset( new ShmemAllocator( ACE_UUID_Admin::instance()->getPersistenceFilename().c_str()));
  shmemAllocator = new ShmemAllocator( ACE_UUID_Admin::instance()->getPersistenceFilename().c_str());
  void* temp;
  if (shmemAllocator->find( bindName, temp) == 0) 
    {
      
      // persistence already exists, just map to it.
      uuidState = new( temp) UUID_state();
      
      // If the clock went backward or the node ID changed increment the clockseq
      // Increment clock sequence if time has "run backwards" or the node id
      // has changed since the last uuid was generated.
      if ((timeLast < uuidState->timestamp) || (currentNode != uuidState->node)) {
	
	++uuidState->clockSequence;
	uuidState->timestamp = timeLast;
	uuidState->node = currentNode;
      }
      
    } else {
      
      void* temp = shmemAllocator->malloc( sizeof( UUID_state));
      if (temp == 0)
	throw std::bad_alloc();
      uuidState = new( temp) UUID_state();
      shmemAllocator->bind( bindName, uuidState);
      
      // Provide an initial value to clockSequence
      uuidState->timestamp = 0;
      uuidState->clockSequence = static_cast<Unsigned16>( ACE_OS::rand());
      uuidState->node = currentNode;
      
    }
  ACE_ASSERT( uuidState);
}

ACE_UUID_Generator::~ACE_UUID_Generator()
{
  ACE_ASSERT( uuidState);
  uuidState->~UUID_state();
}

void 
ACE_UUID_Generator::formatV1( ACE_UUID_Impl::UUID_t& uuid)
{
  ACE_ASSERT( uuidState);
  // TODO: should throw exception if can't lock?
  ACE_Guard<ACE_Process_Mutex> mon( lock);
  if (mon.locked() == 0)
    return;

  UUID_time_t timestamp;

  getTimestamp( timestamp);

  // Construct a Version 1 UUID with the information in the arguements.
  uuid.timeLow = static_cast<Unsigned32>( timestamp & 0xFFFFFFFF);
  uuid.timeMid = static_cast<Unsigned16>( (timestamp >> 32) & 0xFFFF);
  uuid.timeHiAndVersion = static_cast<Unsigned16>( (timestamp >> 48) & 0xFFFF);
  uuid.timeHiAndVersion |= (1 << 12);
  uuid.clockSeqLow = uuidState->clockSequence & 0xFF;
  uuid.clockSeqHiAndReserved = (uuidState->clockSequence & 0x3f00) >> 8;
  uuid.clockSeqHiAndReserved |= 0x80;
  uuid.node = uuidState->node;

  uuidState->timestamp = timestamp;
}

void 
ACE_UUID_Generator::getTimestamp( UUID_time_t& timestamp)
{
  UUID_time_t timeNow;

  while (1) 
    {
      getSystemTime( timeNow);
      
      // If the system time ticked since the last UUID was generated
      if (timeLast != timeNow) {
	
	// reset the count of uuids generated with this clock reading
	uuidsThisTick = 0;
	timeLast = timeNow;
	break;
	
      }

    if (uuidsThisTick < uuidsPerTick) 
      {
	++uuidsThisTick;
	break;
      }
    // Otherwise, we've generated our quota of uuids this tick; spin.

  }
  timestamp = timeNow + uuidsThisTick;
}

void 
ACE_UUID_Generator::getSystemTime( UUID_time_t& timestamp)
{
  // ACE_Time_Value is in POSIX time, seconds since Jan 1, 1970. UUIDs use
  // time in 100ns ticks since 15 October 1582. The difference is:
  //   15 Oct 1582 - 1 Jan 1600: 17 days in Oct, 30 in Nov,  31 in Dec +
  //       17 years and 4 leap days (1584, 88, 92 and 96)
  //   1 Jan 1600 - 1 Jan 1900: 3 centuries + 73 leap days ( 25 in 17th cent.
  //       and 24 each in 18th and 19th centuries)
  //   1 Jan 1900 - 1 Jan 1970: 70 years + 17 leap days.
  // This adds up, in days: (17+30+31+365*17+4)+(365*300+73)+(365*70+17) or
  // 122192928000000000U (0x1B21DD213814000) 100 ns ticks.
  const UUID_time_t timeOffset = 0x1B21DD213814000;

    // Get the time of day, convert to 100ns ticks then add the offset.
  ACE_Time_Value now = ACE_OS::gettimeofday();
  UUID_time_t time = now.sec() * 10000000 + now.usec() * 10;
  timestamp = time + timeOffset;
}

#if defined(WIN32)
#  include <iphlpapi.h> 
#endif

void 
ACE_UUID_Generator::getIEEENodeIdentifier( ACE_UUID_Impl::UUID_node_t& node)
{
  // Prepare dummy nodeID to return in case of failure.
  node.nodeID[0] = static_cast<Octet>( ACE_OS::rand());
  node.nodeID[1] = static_cast<Octet>( ACE_OS::rand());
  node.nodeID[2] = static_cast<Octet>( ACE_OS::rand());
  node.nodeID[3] = static_cast<Octet>( ACE_OS::rand());
  node.nodeID[4] = static_cast<Octet>( ACE_OS::rand());
  node.nodeID[5] = static_cast<Octet>( ACE_OS::rand());

#if defined(WIN32)
  typedef std::vector <char> MacAddress;
  typedef std::vector <char> IFTab;

  // Use run time linking to eliminate need to always deliver the dll.
  typedef DWORD (WINAPI *PGetNumberOfInterfaces) ( OUT PDWORD  pdwNumIf);
  typedef DWORD (WINAPI *PGetIfTable) (
				       OUT    PMIB_IFTABLE pIfTable,
				       IN OUT PULONG       pdwSize,
				       IN     BOOL         bOrder);
  PGetNumberOfInterfaces pGetNumberOfInterfaces = 0;
  PGetIfTable pGetIfTable = 0;
  ACE_DLL dll;
  if (dll.open( "iphlpapi.dll") != 0) {

    ACE_DEBUG((
	       LM_WARNING,
	       ACE_TEXT( "(%P|%t) ACE_UUID_Impl::getIEEENodeIdentifier: ")
	       ACE_TEXT( "unable to load iphlpapi.dll, using dummy node id/n")
	       ));
    return;

  }
  pGetNumberOfInterfaces = reinterpret_cast<PGetNumberOfInterfaces>(
								    dll.symbol( "GetNumberOfInterfaces"));
  pGetIfTable = reinterpret_cast<PGetIfTable>( dll.symbol( "GetIfTable"));
  if ((pGetNumberOfInterfaces == 0) ||
      (pGetIfTable == 0)) {

    ACE_DEBUG((
	       LM_WARNING,
	       ACE_TEXT( "(%P|%t) ACE_UUID_Impl::getIEEENodeIdentifier: ")
	       ACE_TEXT( "unable to load a needed symbol, using dummy node id/n")
	       ));
    return;

  }

  // This is really ugly stuff; but then again it's Win32
  DWORD numberOfInterfaces = 0;
  DWORD retValue = 0;

    // The number of interfaces includes the loopback interface.
  retValue = (*pGetNumberOfInterfaces)( &numberOfInterfaces);
  ACE_ASSERT( retValue == NO_ERROR);
  if (retValue != NO_ERROR)
    return;
  if (numberOfInterfaces < 2) {

    ACE_DEBUG((
	       LM_WARNING,
	       ACE_TEXT( "(%P|%t) ACE_UUID_Impl::getIEEENodeIdentifier: ")
	       ACE_TEXT( "too few interfaces, using dummy node id/n")
	       ));
    return;

  }

  // Apparently the table must actually be larger than needed to hold
  // numberOfInterfaces entries. An error is returned f the exact size is used.
  // This calculation should give the size of an MIB_IFTABLE with numberOfInterfaces
  // + 1 entries.
  ULONG tableSize = sizeof( DWORD) + (numberOfInterfaces + 1) * sizeof( MIB_IFROW);
  IFTab ifTab( tableSize);

  retValue = (*pGetIfTable)(
			    (PMIB_IFTABLE)&ifTab[0],
			    &tableSize,
			    TRUE);
  ACE_ASSERT( retValue == NO_ERROR);
  if (retValue != NO_ERROR) {

    ACE_DEBUG((
	       LM_WARNING,
	       ACE_TEXT( "(%P|%t) ACE_UUID_Impl::getIEEENodeIdentifier: ")
	       ACE_TEXT( "unable to get interface table/n")
	       ));
    return;

  }
  MIB_IFTABLE& ifTable = *reinterpret_cast<PMIB_IFTABLE>( &ifTab[0]);
  ACE_ASSERT( ifTable.dwNumEntries > 1);

  // Search of the first ethernet interface.
  bool ethernetFound;
  for (DWORD i = 0; i < numberOfInterfaces; ++i) {

    if (ifTable.table[i].dwType == MIB_IF_TYPE_ETHERNET) {

		ACE_ASSERT( ifTable.table[i].dwPhysAddrLen == ACE_UUID_Impl::UUID_node_t::nodeIDSize);
      for (DWORD j = 0; j < ifTable.table[i].dwPhysAddrLen; ++j)
	node.nodeID[j] = ifTable.table[i].bPhysAddr[j];
      ethernetFound = true;
      break;

    }

  }
  if (!ethernetFound) {

    ACE_DEBUG((
	       LM_WARNING,
	       ACE_TEXT( "(%P|%t) ACE_UUID_Impl::getIEEENodeIdentifier: ")
	       ACE_TEXT( "could not find an either interface, using dummy node id/n")
	       ));
    return;

  }
#elif defined(ACE_CONFIG_LINUX_H)
  ACE_HANDLE fd = ACE_OS::socket( AF_INET, SOCK_DGRAM, 0);
  if (fd == ACE_INVALID_HANDLE) {

    ACE_DEBUG((
	       LM_WARNING,
	       ACE_TEXT( "(%P|%t) ACE_UUID_Impl::getIEEENodeIdentifier: ")
	       ACE_TEXT( "error opening kernel socket: %s/n"),
	       strerror( errno)
	       ));
    return;

  }
  struct ifreq ifr;
  ACE_OS::strcpy( ifr.ifr_name, "eth0");
  if (ACE_OS::ioctl( fd, SIOCGIFHWADDR, & ifr) < 0) {

    ACE_DEBUG((
	       LM_WARNING,
	       ACE_TEXT( "(%P|%t) ACE_UUID_Impl::getIEEENodeIdentifier: ")
	       ACE_TEXT( "error ioctl( SIOCGIFHWADDR): %s/n"),
	       strerror( errno)
	       ));
    return;

  }
  ACE_OS::memcpy(
		 &node.nodeID,
		 ifr.ifr_hwaddr.sa_data,
		 UUID_node_t::nodeIDSize);
#else
#error Need to implement ACE_UUID_Impl::getIEEENodeIdentifier for platform
#endif

  return;
}
const ACE_SString 
ACE_UUID_Admin_Impl::defaultPersistenceFilename = "./UUIDState";

ACE_UUID_Admin_Impl::ACE_UUID_Admin_Impl()
  : persistenceFilename(defaultPersistenceFilename)
{
}

void 
ACE_UUID_Admin_Impl::setPersistenceFilename (const ACE_SString& filename)
{
  persistenceFilename = filename;
}

const ACE_SString& 
ACE_UUID_Admin_Impl::getPersistenceFilename () const
{
  return persistenceFilename;
}

const ACE_SString& 
ACE_UUID_Admin_Impl::getDefaultPersistenceFilename ()
{
  return defaultPersistenceFilename;
}




