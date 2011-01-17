//$Id$

#include "orbsvcs/FtRtEvent/Utils/UUID.h"

#if !defined(__ACE_INLINE__)
#include "orbsvcs/FtRtEvent/Utils/UUID.inl"
#endif /* __ACE_INLINE__ */

#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_netdb.h"
#include "ace/OS_NS_sys_time.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace TAO_FtRt
{

static union
{
  struct
  {
    ACE_UINT32  rand1;
    ACE_UINT16  rand2;
  } rand_node;
  ACE_OS::macaddr_node_t  mac_address;
} node;

int
hex_to_byte (char  h)
{
  if (h >= '0' && h <= '9') return h - '0';
  if (h >= 'A' && h <= 'F') return h - 'A' + 10;
  if (h >= 'a' && h <= 'f') return h - 'a' + 10;
  return -1;
}

int
hexbyte_to_byte (const char  *hexbyte)
{
  int hi = hex_to_byte(*hexbyte);
  if (hi == -1) return -1;
  ++hexbyte;

  int low = hex_to_byte(*hexbyte);
  if (low == -1) return -1;
  return (hi << 4) | low;
}

static const int  counts[] = { 4, 2, 2, 2, 6 };
static const char *seperators = "----";

/**
 * construct an UUID from the string representation
 */
UUID::UUID (const ACE_TCHAR *string_rep)
{
  if (this->from_string(ACE_TEXT_ALWAYS_CHAR(string_rep)) == false)
    rep_.timestamp.hi = 0;
}

bool
UUID::from_string (const char *string_rep)
{
  int offset = 0;

  for (int i = 0; i < 5; ++i)
  {
    for (int j = 0; j < counts[i]; ++j)
    {
      int r = hexbyte_to_byte(string_rep);
      if (r == -1)
      {
        return false;
      }

      rep_.uuid[offset++] = static_cast<unsigned char> (r);
      string_rep += 2;
    }

    if (*string_rep++ != seperators[i])
    {
      return false;
    }
  }

  return true;
}

static const char *table = "0123456789abcdef";

inline char *
bytes_to_hex (const unsigned char *bytes,
              char  *dest,
              int   len)
{
  for (int i = 0; i < len; ++i)
  {
    *dest++ = table[bytes[i] >> 4];
    *dest++ = table[bytes[i] & 0x0f];
  }

  return dest;
}

/**
 * convert to a string representation
 */
void
UUID::to_string (char  *string_rep) const
{
  for (int i = 0; i < 5; ++i)
  {
    string_rep = bytes_to_hex(rep_.uuid,
                              string_rep,
                              counts[i]);
    *string_rep++ = seperators[i];
  }
}

void
UUID::create (unsigned char *buffer)
{
  static ACE_RANDR_TYPE seed;

  if (seed == 0) seed = ACE_OS::getpid();

  // test if node is properly initialized
  if (node.rand_node.rand1 == 0)
  {
    // initialize the node
    if (ACE_OS::getmacaddress(&node.mac_address) == -1)
    {
      node.rand_node.rand1 = ACE_OS::rand_r(seed);
      node.rand_node.rand2 = (unsigned short) ACE_OS::rand_r(seed);
    }
  }

  // Days in years
  static ACE_UINT64 SecondsToJan1970 =
    (static_cast<ACE_UINT64> (365)*(1970-1583) // Days in years
    + (1970-1583)/4 // Leap days
    - 3  // Allow for 1700, 1800, 1900 not leap years
    + 31  // Days in December 1583
    + 30  // Days in November 1583
    + 16)*60*60*24; // Days from 15th October


  ACE_Time_Value    now = ACE_OS::gettimeofday();
  ACE_UINT64        timestamp = (SecondsToJan1970 + now.sec()) * 10000000 + now.usec() * 10;

  buffer[0] = (unsigned char) (timestamp & 0xff);
  buffer[1] = (unsigned char) ((timestamp >> 8) & 0xff);
  buffer[2] = (unsigned char) ((timestamp >> 16) & 0xff);
  buffer[3] = (unsigned char) ((timestamp >> 24) & 0xff);
  buffer[4] = (unsigned char) ((timestamp >> 32) & 0xff);
  buffer[5] = (unsigned char) ((timestamp >> 40) & 0xff);

  // multiplex timestamp with thread id to ensure the uniqueness between thread
  buffer[6] = (unsigned char) ((timestamp >> 48) & 0xff);
  // Version number is 1
  buffer[7] = (unsigned char) (((timestamp >> 56) & 0x0f) + 0x10);

  ACE_UINT16  clockSequence = static_cast<
    ACE_UINT16> (ACE_OS::rand_r(seed) & 0x2ff);

  buffer[8] = (unsigned char) ((clockSequence >> 8) & 0x1f);
  buffer[9] = (unsigned char) (clockSequence & 0x1f);

  ACE_OS::memcpy(buffer + 10, &node, 6);
}
}
TAO_END_VERSIONED_NAMESPACE_DECL
