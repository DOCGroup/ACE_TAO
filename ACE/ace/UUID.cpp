#include "ace/UUID.h"
#include "ace/Guard_T.h"

#if !defined (__ACE_INLINE__)
#include "ace/UUID.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Log_Category.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_netdb.h"
#include "ace/OS_NS_unistd.h"
#include "ace/ACE.h"
#include <memory>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE_Utils
{
  // NIL version of the UUID
  const UUID UUID::NIL_UUID;

#ifndef ACE_LACKS_SSCANF
  UUID::UUID (const ACE_CString& uuid_string)
  {
    this->init ();
    this->from_string_i (uuid_string);
  }
#endif /* ACE_LACKS_SSCANF */

  ACE_ALLOC_HOOK_DEFINE(UUID);

  const UUID &
  UUID::operator = (const UUID & rhs)
  {
    if (this != &rhs)
      {
        // Reset the string version of the UUID a string version
        // exist, and the UUID is not equal to the old UUID.
        if (0 != this->as_string_.get ())
          {
            if (0 == rhs.as_string_.get () || *this != rhs)
              this->as_string_.reset ();
          }

        // Copy the contents of the UUID.
        ACE_OS::memcpy (&this->uuid_, &rhs.uuid_, BINARY_SIZE);

        /// @todo We should create an UUID_Ex class for UUIDs that
        ///       contain the thread id and process id.
        this->thr_id_ = rhs.thr_id_;
        this->pid_ = rhs.pid_;
      }

    return *this;
  }

  const ACE_CString * UUID::to_string () const
  {
    // Compute the string representation only once.
    if (0 != this->as_string_.get ())
      return this->as_string_.get ();

    // Get a buffer exactly the correct size. Use the nil UUID as a
    // gauge.  Don't forget the trailing nul.
    std::unique_ptr <char[]> auto_clean;
    size_t UUID_STRING_LENGTH = 36 + thr_id_.length () + pid_.length ();
    char *buf = 0;

    if (36 == UUID_STRING_LENGTH)
      {
#if defined (ACE_HAS_ALLOC_HOOKS)
        ACE_ALLOCATOR_RETURN (buf,
                              static_cast<char*> (ACE_Allocator::instance()->malloc(sizeof (char) * (UUID_STRING_LENGTH + 1))),
                              0);
#else
        ACE_NEW_RETURN (buf,
                        char[UUID_STRING_LENGTH + 1],
                        0);
#endif /* ACE_HAS_ALLOC_HOOKS */

        // Let the auto array pointer manage the buffer.
        auto_clean.reset (buf);

        ACE_OS::snprintf (buf, UUID_STRING_LENGTH + 1,
                          "%8.8x-%4.4x-%4.4x-%2.2x%2.2x-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x",
                          this->uuid_.time_low_,
                          this->uuid_.time_mid_,
                          this->uuid_.time_hi_and_version_,
                          this->uuid_.clock_seq_hi_and_reserved_,
                          this->uuid_.clock_seq_low_,
                          this->uuid_.node_.node_ID ()[0],
                          this->uuid_.node_.node_ID ()[1],
                          this->uuid_.node_.node_ID ()[2],
                          this->uuid_.node_.node_ID ()[3],
                          this->uuid_.node_.node_ID ()[4],
                          this->uuid_.node_.node_ID ()[5]);
      }
    else
      {
        UUID_STRING_LENGTH += 2; //for '-'

#if defined (ACE_HAS_ALLOC_HOOKS)
        ACE_ALLOCATOR_RETURN (buf,
                              static_cast<char*> (ACE_Allocator::instance()->malloc(sizeof (char) * (UUID_STRING_LENGTH + 1))),
                              0);
#else
        ACE_NEW_RETURN (buf,
                        char[UUID_STRING_LENGTH + 1],
                        0);
#endif /* ACE_HAS_ALLOC_HOOKS */

        // Let the auto array pointer manage the buffer.
        auto_clean.reset (buf);

        ACE_OS::snprintf (buf, UUID_STRING_LENGTH + 1,
                          "%8.8x-%4.4x-%4.4x-%2.2x%2.2x-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x-%s-%s",
                          this->uuid_.time_low_,
                          this->uuid_.time_mid_,
                          this->uuid_.time_hi_and_version_,
                          this->uuid_.clock_seq_hi_and_reserved_,
                          this->uuid_.clock_seq_low_,
                          this->uuid_.node_.node_ID ()[0],
                          this->uuid_.node_.node_ID ()[1],
                          this->uuid_.node_.node_ID ()[2],
                          this->uuid_.node_.node_ID ()[3],
                          this->uuid_.node_.node_ID ()[4],
                          this->uuid_.node_.node_ID ()[5],
                          thr_id_.c_str (),
                          pid_.c_str ());
      }

    // Save the string.
    ACE_CString * as_string = 0;

    ACE_NEW_RETURN (as_string,
                    ACE_CString (buf, UUID_STRING_LENGTH),
                    0);

    this->as_string_.reset (as_string);
    return this->as_string_.get ();
  }

#ifndef ACE_LACKS_SSCANF
  void
  UUID::from_string_i (const ACE_CString& uuid_string)
  {
    if (uuid_string.length () < NIL_UUID.to_string ()->length ())
      {
        ACELIB_ERROR ((LM_ERROR,
                    "%N ACE_UUID::from_string_i - "
                    "IllegalArgument (incorrect string length)\n"));
        return;
      }

    /// Special case for the nil UUID.
    if (uuid_string == *NIL_UUID.to_string ())
      {
        *this = NIL_UUID;
        return;
      }

    unsigned int time_low;
    unsigned int time_mid;
    unsigned int time_hi_and_version;
    unsigned int clock_seq_hi_and_reserved;
    unsigned int clock_seq_low;
    unsigned int node [UUID_Node::NODE_ID_SIZE];
    char thr_pid_buf [BUFSIZ];

    if (uuid_string.length () == NIL_UUID.to_string ()->length ())
      {
        // This might seem quite strange this being in ACE, but it
        // seems to be a bit difficult to write a facade for ::sscanf
        // because some compilers dont support vsscanf, including
        // MSVC. It appears that most platforms support sscanf though
        // so we need to use it directly.
        const int nScanned =
#if defined (ACE_HAS_TR24731_2005_CRT)
          sscanf_s (
                   uuid_string.c_str (),
                   "%8x-%4x-%4x-%2x%2x-%2x%2x%2x%2x%2x%2x",
                   &time_low,
                   &time_mid,
                   &time_hi_and_version,
                   &clock_seq_hi_and_reserved,
                   &clock_seq_low,
                   &node[0],
                   &node[1],
                   &node[2],
                   &node[3],
                   &node[4],
                   &node[5]
                   );
#else
          ::sscanf (
                   uuid_string.c_str (),
                   "%8x-%4x-%4x-%2x%2x-%2x%2x%2x%2x%2x%2x",
                   &time_low,
                   &time_mid,
                   &time_hi_and_version,
                   &clock_seq_hi_and_reserved,
                   &clock_seq_low,
                   &node[0],
                   &node[1],
                   &node[2],
                   &node[3],
                   &node[4],
                   &node[5]
                   );
#endif /* ACE_HAS_TR24731_2005_CRT */

        if (nScanned != 11)
          {
            ACELIB_DEBUG ((LM_DEBUG,
                        "UUID::from_string_i - "
                        "IllegalArgument (invalid string representation)\n"));
            return;
          }
      }
    else
      {
        const int nScanned =
#if defined (ACE_HAS_TR24731_2005_CRT)
          sscanf_s (uuid_string.c_str (),
                    "%8x-%4x-%4x-%2x%2x-%2x%2x%2x%2x%2x%2x-%s",
                    &time_low,
                    &time_mid,
                    &time_hi_and_version,
                    &clock_seq_hi_and_reserved,
                    &clock_seq_low,
                    &node[0],
                    &node[1],
                    &node[2],
                    &node[3],
                    &node[4],
                    &node[5],
                    thr_pid_buf,
                    BUFSIZ
                    );
#else
          ::sscanf (uuid_string.c_str (),
                    "%8x-%4x-%4x-%2x%2x-%2x%2x%2x%2x%2x%2x-%s",
                    &time_low,
                    &time_mid,
                    &time_hi_and_version,
                    &clock_seq_hi_and_reserved,
                    &clock_seq_low,
                    &node[0],
                    &node[1],
                    &node[2],
                    &node[3],
                    &node[4],
                    &node[5],
                    thr_pid_buf
                    );
#endif /* ACE_HAS_TR24731_2005_CRT */

        if (nScanned != 12)
          {
            ACELIB_DEBUG ((LM_DEBUG,
                        "ACE_UUID::from_string_i - "
                        "IllegalArgument (invalid string representation)\n"));
            return;
          }
      }

    this->uuid_.time_low_ = static_cast<ACE_UINT32> (time_low);
    this->uuid_.time_mid_ = static_cast<ACE_UINT16> (time_mid);
    this->uuid_.time_hi_and_version_ = static_cast<ACE_UINT16> (time_hi_and_version);
    this->uuid_.clock_seq_hi_and_reserved_ = static_cast<u_char> (clock_seq_hi_and_reserved);
    this->uuid_.clock_seq_low_ = static_cast<u_char> (clock_seq_low);

    for (size_t i = 0; i < UUID_Node::NODE_ID_SIZE; ++ i)
      this->uuid_.node_.node_ID ()[i] = static_cast <u_char> (node[i]);

    // Support varient 10- only
    if ((this->uuid_.clock_seq_hi_and_reserved_ & 0xc0) != 0x80 &&
        (this->uuid_.clock_seq_hi_and_reserved_ & 0xc0) != 0xc0)
      {
        ACELIB_DEBUG ((LM_DEBUG,
                    "ACE_UUID::from_string_i - "
                    "IllegalArgument (unsupported variant)\n"));
        return;
      }

    /// Support versions 1, 3, and 4 only
    ACE_UINT16 V1 = this->uuid_.time_hi_and_version_;

    if ((V1 & 0xF000) != 0x1000 &&
        (V1 & 0xF000) != 0x3000 &&
        (V1 & 0xF000) != 0x4000)
      {
        ACELIB_DEBUG ((LM_DEBUG,
                    "ACE_UUID::from_string_i - "
                    "IllegalArgument (unsupported version)\n"));
        return;
      }

    if ((this->uuid_.clock_seq_hi_and_reserved_ & 0xc0) == 0xc0)
      {
        if (uuid_string.length () == NIL_UUID.to_string ()->length ())
          {
            ACELIB_DEBUG ((LM_DEBUG,
                      "ACE_UUID::from_string_i - "
                        "IllegalArgument (Missing Thread and Process Id)\n"));
            return;
          }
        ACE_CString thr_pid_str (thr_pid_buf);
        ssize_t pos = static_cast<ssize_t> (thr_pid_str.find ('-'));
        if (pos == -1)
          ACELIB_DEBUG ((LM_DEBUG,
                      "ACE_UUID::from_string_i - "
                      "IllegalArgument (Thread and Process Id format incorrect)\n"));

        this->thr_id_ = thr_pid_str.substr (0, pos);
        this->pid_ = thr_pid_str.substr (pos+1, thr_pid_str.length ()-pos-1);
      }
  }
#endif // ACE_LACKS_SSCANF

  UUID_Generator::UUID_Generator ()
    : time_last_ (0),
      destroy_lock_ (true),
      is_init_ (false)
  {
    ACE_NEW (lock_, ACE_SYNCH_MUTEX);
    this->init ();
  }

  UUID_Generator::~UUID_Generator ()
  {
    if (destroy_lock_)
      delete lock_;
  }

  void
  UUID_Generator::init ()
  {
    if (this->is_init_)
      return;

    ACE_OS::macaddr_node_t macaddress;
    int const result = ACE_OS::getmacaddress (&macaddress);

    UUID_Node::Node_ID node_id;

    if (-1 != result)
      {
        ACE_OS::memcpy (node_id,
                        macaddress.node,
                        UUID_Node::NODE_ID_SIZE);
      }
    else
      {
        node_id [0] = static_cast<u_char> (ACE_OS::rand ());
        node_id [1] = static_cast<u_char> (ACE_OS::rand ());
        node_id [2] = static_cast<u_char> (ACE_OS::rand ());
        node_id [3] = static_cast<u_char> (ACE_OS::rand ());
        node_id [4] = static_cast<u_char> (ACE_OS::rand ());
        node_id [5] = static_cast<u_char> (ACE_OS::rand ());
      }

    this->get_timestamp (time_last_);

    {
      ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, *lock_);
      uuid_state_.timestamp = time_last_;

      ACE_OS::memcpy (uuid_state_.node.node_ID (),
                      node_id,
                      UUID_Node::NODE_ID_SIZE);
    }

    this->is_init_ = true;
  }

  void
  UUID_Generator::
  generate_UUID (UUID& uuid, ACE_UINT16 version, u_char variant)
  {
    UUID_Time timestamp = 0;
    ACE_UINT16 clock_sequence = 0;

    this->get_timestamp_and_clocksequence (timestamp,
                                           clock_sequence);

    // Construct a Version 1 UUID with the information in the arguements.
    uuid.time_low (static_cast<ACE_UINT32> (timestamp & 0xFFFFFFFF));
    uuid.time_mid (static_cast<ACE_UINT16> ((timestamp >> 32) & 0xFFFF));

    ACE_UINT16 tHAV = static_cast<ACE_UINT16> ((timestamp >> 48) & 0xFFFF);
    tHAV = static_cast<ACE_UINT16> (tHAV | (version << 12));
    uuid.time_hi_and_version (tHAV);

    u_char cseqHAV;
    uuid.clock_seq_low (static_cast<u_char> (clock_sequence & 0xFF));
    cseqHAV = static_cast<u_char> ((clock_sequence & 0x3f00) >> 8);
    uuid_state_.timestamp = timestamp;

    cseqHAV = static_cast<u_char> (cseqHAV | variant);
    uuid.clock_seq_hi_and_reserved (cseqHAV);
    uuid.node (uuid_state_.node);

    if (variant == 0xc0)
      {
        ACE_Thread_ID thread_id;
        char buf [BUFSIZ];
        thread_id.to_string (buf, BUFSIZ);
        uuid.thr_id (buf);

        ACE_OS::snprintf (buf, BUFSIZ, "%d",
                          static_cast<int> (ACE_OS::getpid ()));
        uuid.pid (buf);
      }
  }

  UUID*
  UUID_Generator::generate_UUID (ACE_UINT16 version, u_char variant)
  {
    UUID* uuid = 0;
    ACE_NEW_RETURN (uuid,
                    UUID,
                    0);

    this->generate_UUID (*uuid, version, variant);
    return uuid;
  }

  /// Obtain a new timestamp. If UUID's are being generated too quickly
  /// the clock sequence will be incremented
  void
  UUID_Generator::get_timestamp (UUID_Time& timestamp)
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, mon, *lock_);

    this->get_systemtime (timestamp);

    // Account for the clock being set back. Increment the clock /
    // sequence.
    if (timestamp <= time_last_)
      {
        uuid_state_.clock_sequence = static_cast<ACE_UINT16>
          ((uuid_state_.clock_sequence + 1) & ACE_UUID_CLOCK_SEQ_MASK);
      }
    // If the system time ticked since the last UUID was
    // generated. Set / the clock sequence back.
    else if (timestamp > time_last_)
      {
        uuid_state_.clock_sequence = 0;
      }

    time_last_ = timestamp;
  }

  void
  UUID_Generator::get_timestamp_and_clocksequence (UUID_Time& timestamp,
                                                   ACE_UINT16& clock_sequence)
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, mon, *lock_);

    this->get_systemtime (timestamp);

    // Account for the clock being set back. Increment the clock /
    // sequence.
    if (timestamp <= time_last_)
      uuid_state_.clock_sequence = static_cast<ACE_UINT16> ((uuid_state_.clock_sequence + 1) & ACE_UUID_CLOCK_SEQ_MASK);

    // If the system time ticked since the last UUID was
    // generated. Set / the clock sequence back.
    else if (timestamp > time_last_)
      uuid_state_.clock_sequence = 0;

    time_last_ = timestamp;
    clock_sequence = uuid_state_.clock_sequence;
  }

  /**
   * ACE_Time_Value is in POSIX time, seconds since Jan 1, 1970. UUIDs use
   * time in 100ns ticks since 15 October 1582. The difference is:
   *   15 Oct 1582 - 1 Jan 1600: 17 days in Oct, 30 in Nov,  31 in Dec +
   *       17 years and 4 leap days (1584, 88, 92 and 96)
   *   1 Jan 1600 - 1 Jan 1900: 3 centuries + 73 leap days ( 25 in 17th cent.
   *       and 24 each in 18th and 19th centuries)
   *   1 Jan 1900 - 1 Jan 1970: 70 years + 17 leap days.
   * This adds up, in days: (17+30+31+365*17+4)+ (365*300+73)+ (365*70+17) or
   * 122192928000000000U (0x1B21DD213814000) 100 ns ticks.
   */
  void
  UUID_Generator::get_systemtime (UUID_Time & timestamp)
  {
    const UUID_Time timeOffset =
      ACE_UINT64_LITERAL (0x1B21DD213814000);

    /// Get the time of day, convert to 100ns ticks then add the offset.
    ACE_Time_Value now = ACE_OS::gettimeofday ();
    ACE_UINT64 time;
    now.to_usec (time);
    time = time * 10;
    timestamp = time + timeOffset;
  }

  ACE_SYNCH_MUTEX*
  UUID_Generator::lock ()
  {
    return this->lock_;
  }

  void
  UUID_Generator::lock (ACE_SYNCH_MUTEX* lock, bool release_lock)
  {
    if (this->destroy_lock_)
      delete this->lock_;

    this->lock_ = lock;
    this->destroy_lock_ = release_lock;
  }
}

ACE_SINGLETON_TEMPLATE_INSTANTIATE(ACE_Singleton, ACE_Utils::UUID_Generator, ACE_SYNCH_MUTEX);

ACE_END_VERSIONED_NAMESPACE_DECL
