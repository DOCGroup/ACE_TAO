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
#include /**/ "ace/pre.h"

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "ace/Singleton.h"
#include "ace/Synch.h"

namespace ACE_Utils
{
  /// Class to hold a MAC address
  class ACE_Export UUID_node
  {
  public:

    /// Constructor
    UUID_node (void);

    enum {NODE_ID_SIZE = 6};
    typedef u_char NodeID[NODE_ID_SIZE];

    NodeID& nodeID (void);
    void nodeID (NodeID&);

    /// Assignment Operation
    UUID_node &operator=(const UUID_node &right);

    /// Equality Operations
    bool operator == (const UUID_node& right) const;
    bool operator != (const UUID_node& right) const;

    /// Relational Operations
    bool operator <  (const UUID_node& right) const;

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
   *
   * Additionally in this implementation provisions have been made to include
   * process and thread ids to make the UUIDs more unique. The variant 0xc0
   * has been added to facilitate this.
   */
  class ACE_Export UUID
  {
  public:

    /// Constructor
    UUID (void);

    /// Copy constructor
    UUID (const UUID &right);

    /// Constructs a UUID from a string representation.
    UUID (const ACE_CString& uuidString);

    // Destructor
    ~UUID (void);

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

    ACE_CString* thr_id (void);
    void thr_id (char*);

    ACE_CString* pid (void);
    void pid (char*);

    /// Returns a string representation of the UUID
    const ACE_CString* to_string (void);

    static UUID NIL_UUID;

    /// Assignment Operation
    UUID & operator= (const UUID &right);

    /// Equality Operations
    bool operator== (const UUID &right) const;
    bool operator!= (const UUID &right) const;

    /// Relational Operations
    bool operator<  (const UUID &right) const;
    bool operator>  (const UUID &right) const;
    bool operator<= (const UUID &right) const;
    bool operator>= (const UUID &right) const;

  private:

    /// Data Members for Class Attributes
    ACE_UINT32 timeLow_;
    ACE_UINT16 timeMid_;
    ACE_UINT16 timeHiAndVersion_;
    u_char clockSeqHiAndReserved_;
    u_char clockSeqLow_;
    UUID_node* node_;
    int node_release_;
    ACE_CString thr_id_;
    ACE_CString pid_;

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
  class ACE_Export UUID_Generator
  {
  public:

    enum {ACE_UUID_CLOCK_SEQ_MASK = 0x3FFF};

    UUID_Generator();
    ~UUID_Generator();

    void init (void);

    /// Format timestamp, clockseq, and nodeID into an UUID of the
    /// specified version and variant. For generating UUID's with
    /// thread and process ids use variant=0xc0
    void generateUUID (UUID&, ACE_UINT16 version=0x0001, u_char variant=0x80);

    /// Format timestamp, clockseq, and nodeID into a VI UUID. For
    /// generating UUID's with thread and process ids use variant=0xc0
    UUID* generateUUID (ACE_UINT16 version=0x0001, u_char variant=0x80);

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

  typedef ACE_Singleton <UUID_Generator, ACE_SYNCH_MUTEX> UUID_GENERATOR;

};

#if defined (__ACE_INLINE__)
#include "ace/UUID.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif // ACE_UUID_H
