/* -*- C++ -*- */
//=============================================================================
/**
 *  @file  UUID.h
 *
 *  $Id$
 *
 *  @author Andrew T. Finnel <andrew@activesol.net>
 *  @author Yamuna Krishnmaurthy <yamuna@oomworks.com>
 */
//=============================================================================

#ifndef ACE_UUID_H
#define ACE_UUID_H

#include "ace/pre.h"
#include "ace/OS.h"
#include "ace/SString.h"
#include "ace/Singleton.h"

/// Class to hold a MAC address
class ACE_Export UUID_node 
{
 public:

  /// Constructor
  UUID_Node (void);

  typedef enum {NODE_ID_SIZE = 6};
  typedef u_char NodeID[NODE_ID_SIZE];
  
  NodeID& nodeID (void);
  void nodeID (NodeID&);
  
  /*
  bool operator == (const UUID_node_t& right) const;
  bool operator != (const UUID_node_t& right) const;
  bool operator <  (const UUID_node_t& right) const;
  */

 private:
  NodeID nodeID_;
};


/**
 *  @class ACE_UUID
 *
 * ACE_UUID represents a Universally Unique IDentifier (UUID) as
 * described in (the expired) INTERNET-DRAFT specification entitled
 * UUIDs and GUIDs. All instances of UUID are of the time-based
 * variety. That is, the version number part of the timeHiAndVersion
 * field is 1.
 *
 * The default constructor creates a nil UUID.
 *
 * UUIDs have value semantics. In addition, they may be compared for
 * ordering and equality.
 */
class ACE_Export ACE_UUID 
{
 public:

  /// Constructor
  ACE_UUID(void);
 
  /// Copy constructor
  ACE_UUID(const ACE_UUID &right);
  
  /// Constructs a UUID from a string representation.
  ACE_UUID (const ACE_CString& uuidString);
  
  // Destructor
  ~ACE_UUID (void);

  ACE_UINT32 timeLow (void);
  void timeLow (ACE_UINT32);

  ACE_UINT16 timeMid (void);    
  void timeMid (ACE_UINT16);    

  ACE_UINT16 timeHiAndVersion (void);
  void timeHiAndVersion (ACE_UINT16);

  u_char clockSeqHiAndReserved (void);    
  void clockSeqHiAndReserved (u_char);    

  u_char clockSeqLow (void);
  void clockSeqLow (u_char);
  
  UUID_node* node (void); 
  void node (UUID_node*); 

  /// Returns a string representation of the UUID
  const ACE_CString* to_string (void);
    
  static ACE_UUID NIL_UUID;

  /*
  /// Assignment Operation
  ACE_UUID & operator=(const ACE_UUID &right);

  /// Equality Operations 
  bool operator==(const ACE_UUID &right) const;
  bool operator!=(const ACE_UUID &right) const;

  /// Relational Operations 
  bool operator< (const ACE_UUID &right) const;
  bool operator> (const ACE_UUID &right) const;
  bool operator<=(const ACE_UUID &right) const;
  bool operator>=(const ACE_UUID &right) const;
  */
 private:

  /// Data Members for Class Attributes
  ACE_UINT32 timeLow_;
  ACE_UINT16 timeMid_;    
  ACE_UINT16 timeHiAndVersion_;    
  u_char clockSeqHiAndReserved_;    
  u_char clockSeqLow_;    
  UUID_node* node_; 
  int node_release_;
  
  /// The string representation of the UUID. This is created and
  /// updated only on demand.
  ACE_CString *as_string_; 
};


/**
 * @class ACE_UUID_Generator 
 *
 * Singleton class that generates UUIDs.
 *
 */
class ACE_Export ACE_UUID_Generator
{
 public:

  enum {ACE_UUID_CLOCK_SEQ_MASK = 0x3FFF};

  ACE_UUID_Generator(void);
  ~ACE_UUID_Generator();

  void init (void);

  /// Format timestamp, clockseq, and nodeID into a VI UUID.
  void generateUUID(ACE_UUID&);
  
  /// Format timestamp, clockseq, and nodeID into a VI UUID.
  ACE_UUID* generateUUID (void);

  /// Type to represent UTC as a count of 100 nanosecond intervals
  /// since 00:00:00.00, 15 October 1582.
  typedef ACE_UINT64 UUID_time;
  
  /// The locking strategy prevents multiple generators from accessing
  /// the UUID_state at the same time. Get the locking strategy.
  ACE_SYNCH_MUTEX* lock (void);

  /// Set a new locking strategy and return the old one.
  ACE_SYNCH_MUTEX* lock (ACE_SYNCH_MUTEX*lock,
			 int release_lock);
  

 private:

  /// The maximum number of uuids that can be generated per tick of the
  /// system clock. This number should be the number of 100ns ticks of the
  /// actual resolution of the system's clock.
  enum {UUIDS_PER_TICK = 1000};

  /// The number of uuids generated in this process since the last clock
  /// tick. Value never exceeds uuidsPerTick - 1.
  ACE_UINT32 uuidsThisTick_;

  /// The system time when that last uuid was generated.
  UUID_time timeLast_;

  /// Type to contain the UUID generator persistent state. This will
  /// be kept in memory mapped shared memory
  struct UUID_State
  {
    UUID_time timestamp;
    UUID_node node;
    ACE_UINT16 clockSequence;
  };

  /// Obtain a UUID timestamp. Compensate for the fact that the time
  /// obtained from getSystem time has a resolution less than 100ns.
  void get_timestamp (UUID_time& timestamp);

  /// Obtain the system time in UTC as a count of 100 nanosecond intervals
  /// since 00:00:00.00, 15 October 1582 (the date of Gregorian reform to
  /// the Christian calendar).
  void get_systemtime( UUID_time& timeNow);
  
  /// The UUID generator persistent state.
  UUID_State uuid_state_;

  ACE_SYNCH_MUTEX* lock_;
  int destroy_lock_;
};

typedef ACE_Singleton <ACE_UUID_Generator, ACE_SYNCH_MUTEX> ACE_UUID_GENERATOR;

/* class ACE_UUID_Ext: public ACE_UUID */
/* { */
/*  public: */
/*   ACE_UUID_Ext (void); */
/*   ~ACE_UUID_Ext (void); */

/*   ACE_CString* thr_id (void); */
  
/*   ACE_CString* pid (void); */

/*  private: */
/*   ACE_CString* thr_id_; */
/*   ACE_CString* pid_; */
//};


#if defined (__ACE_INLINE__)
#include "ace/UUID.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif // ACE_UUID_H

