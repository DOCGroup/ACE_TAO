// $Id$

#ifndef RTCP_PACKET_INCLUDE
#define RTCP_PACKET_INCLUDE

#include "ace/pre.h"

#include "ace/OS.h"

/**
 * @struct RTCP_Common_Header
 * @brief This is the header data that is common to all RTCP messages.
 */
typedef struct
{
  /// the RTP version being used
  unsigned int ver_:2;

  /// indicates whether or not the end of the packet is padded with zeros
  unsigned int pad_:1;

  /// this value varies by packet type
  unsigned int count_:5;

  /// RTCP packet type (eg BYE, SR, RR, SDES)
  unsigned int pt_:8;

  /// packet length in words without this word
  ACE_UINT16 length_;
} RTCP_Common_Header;

/**
 * @class RTCP_Packet
 * @brief This is an abstract class from which all RTCP packet types are derived.
 * It contains code used to validate the RTCP packet.
 */

class RTCP_Packet
{
public:
  /// Constructor for outgoing RTCP packets.
  RTCP_Packet(void);

  /// Constructor for incoming RTCP packets.
  RTCP_Packet(char *buffer);

  /// Destructor
  virtual ~RTCP_Packet() = 0;

  /// Returns a pointer to a local buffer containing the packet.
  void get_packet_data(char **buffer, ACE_UINT16 &length);

  /// Returns the size of the packet.
  /// Defined in child class.
  virtual unsigned int packet_size(void) = 0;

  /// Checks the validity of an RTCP packet.  RTCP packets can be sent
  /// together in a compound packet and is_first indicates the first packet
  /// in a compound packet
  int is_valid (char is_first);

protected:
  /// Header data common to all RTCP packets.
  RTCP_Common_Header chd_;

  /// Buffer to hold byte representation of the RTCP packet.
  char *packet_data_;

  /// Used to create the byte representation of the RTCP packet.
  /// Defined in child class.
  virtual void build_packet(void) = 0;
};

/**
 * @class RTCP_BYE_Packet
 * @brief The BYE RTCP packet is sent by a party when leaving an RTP session.
 */

class RTCP_BYE_Packet : public RTCP_Packet
{
public:
  /// Constructor for outgoing BYE RTCP packets.
  /// Takes a synchronization source id list, the list length (1 for non-mixers),
  /// and an optional reason for leaving the session.
  RTCP_BYE_Packet (ACE_UINT32 *srcList,
                   unsigned char length,
                   const char* text=0);

  /// Constructor for incoming BYE RTCP packets.
  RTCP_BYE_Packet (char *buffer,
                   int *len);

  /// Destructor.
  virtual ~RTCP_BYE_Packet (void);

  /// Returns the size of the packet in bytes.
  unsigned int packet_size (void);

  /// Returns a pointer to a local list of synchronization source ids that are
  /// leaving the session.
  void ssrc_list (ACE_UINT32 **ssrc_list,
                  unsigned char &length);

  /// Returns the reason for leaving the session.
  const char *reason (void);

  /// Prints the contents of the packet.
  void dump (void);

private:
  /// Used to create the byte representation of the RTCP packet.
  void build_packet();

  /// List of synchronization source ids that are leaving the session.
  ACE_UINT32 *ssrc_list_;

  /// The number of ssrc's that are leaving the session (1 for non-mixers).
  unsigned char ssrc_list_length_;

  /// An optional reason for leaving the session.
  char reason_[256];

  /// The number of bytes in the reason for leaving the session.
  unsigned char reason_length_;
};

/**
 * @struct RR_Block
 * @brief The receiver report block encapsulates the data that represents the
 * reception statistics for a particular stream.
 */

struct RR_Block
{
  /// The synchronization source id of the source of the data.
  ACE_UINT32 ssrc_;

  /// The fraction of RTP data packets lost since the previous SR or RR was sent.
  unsigned int fraction_:8;

  /// The cumulative number of packets lost.
  int lost_:24;

  /// The highest extended sequence number received in an RTP packet.
  ACE_UINT32 last_seq_;

  /// An estimate of the statistical variance of the RTP data packet interarrival
  /// time measured in timestamp units.
  ACE_UINT32 jitter_;

  /// The middle 32 bits of the NTP timestamp received in the most recent sender
  /// report.
  ACE_UINT32 lsr_;

  /// The delay in 1/65536 seconds since receiving the last sender report.
  ACE_UINT32 dlsr_;

  /// Link to the next receiver report block.
  RR_Block *next_;
};

/**
 * @class RTCP_RR_Packet
 * @brief The Receiver Report packet is sent by all members of a session that
 * are not sending data.  It contains a list of RR_Block to represent each
 * source this party is receiving data from.
 */

class RTCP_RR_Packet : public RTCP_Packet
{
public:
  /// Constructor for incoming receiver reports.
  RTCP_RR_Packet (char *buffer, int* len);

  /// Constructor for outgoing receiver reports.
  RTCP_RR_Packet (ACE_UINT32 ssrc, RR_Block *blocks);

  /// Destructor.
  virtual ~RTCP_RR_Packet (void);

  /// Returns the size of the packet in bytes.
  unsigned int packet_size(void);

  /// Returns the synchronization source id of the source sending this packet.
  ACE_INT32 ssrc (void) { return this->ssrc_; }

  /// Prints the contents of the packet.
  void dump (void);

private:
  /// Used to create the byte representation of the RTCP packet.
  void build_packet(void);

  /// The synchronization source id of the sender of this report.
  ACE_UINT32 ssrc_;

  /// A linked list of the receiver report block(s) being sent.
  RR_Block *rr_;
};

typedef struct sdesItem_s sdesItem_t;

/**
 * @struct sdesItem_s
 * @brief This is a linked list of structures containing source description
 * 'items' such as canonical name, email, location etc.
 */

struct sdesItem_s
{
  /// link to next item.
  sdesItem_t *next_;

  /// Type of item (eg canonical name).
  unsigned char type_;

  union
  {
    struct
    {
      /// Length of an item (in octets).
      unsigned char length_;

      /// Item text, not null-terminated.
      char *data_;
    } standard_;
    struct
    {
      /// Length of the name of an item (in octets).
      unsigned char name_length_;

      /// Length of the item data (in octets).
      unsigned char data_length_;

      /// The name of the item, not null-terminated.
      char *name_;

      /// Item data, not null-terminated.
      char *data_;
    } priv_;
  } info_;
};

typedef struct sdesChunk_s sdesChunk_t;

/**
 * @struct sdesChunk_s
 * @brief This is a linked list of structures containing groups of source
 * description items.  A group of items for a particular synchronization source
 * id is referred to as a 'chunk'.
 */
struct sdesChunk_s
{
  /// Link to next item.
  sdesChunk_t *next_;

  /// The synchronization source id that this chunk describes.
  ACE_UINT32 ssrc_;

  /// A linked list of items to describe this source.
  sdesItem_t *item_;
};

/**
 * @class RTCP_SDES_Packet
 * @brief The Source Description packet is sent by all members of a session.
 * At a minimum, the canonical name (or CNAME) is sent with each RTCP packet.
 * Other items such as name, email, or location are included less frequently.
 */
class RTCP_SDES_Packet : public RTCP_Packet
{
public:
  /// Constructor for incoming SDES packets.
  RTCP_SDES_Packet(char* buffer, int *len);

  /// Constructor for outgoing SDES packets.
  RTCP_SDES_Packet(void);

  /// Destructor.
  virtual ~RTCP_SDES_Packet(void);

  /// This will add a standard item of type and length for the ssrc specified.
  /// When the first item for a ssrc is added, a chunk is created.  Subsequent
  /// items for that ssrc are added to the same chunk.  New chunks are created
  /// for each unique ssrc.
  void add_item(ACE_UINT32 ssrc,
                unsigned char type,
                unsigned char length,
                const char* data);

  /// This will add a private item using the name and data for the ssrc specified.
  /// When the first item for a ssrc is added, a chunk is created.  Subsequent
  /// items for that ssrc are added to the same chunk.  New chunks are created
  /// for each unique ssrc.
  void add_priv_item(ACE_UINT32 ssrc,
                     unsigned char nameLength,
                     const char *name,
                     unsigned char dataLength,
                     const char *data);

  /// Returns the size of the packet in bytes.
  unsigned int packet_size(void);

  /// Prints the contents of the packet.
  void dump (void);

  /// This returns the synchronization source id for this packet.  This assumes
  /// that this source is only receiving messages from end systems (i.e. only
  /// one source id per SDES)
  ACE_UINT32 ssrc (void) { return this->chunk_->ssrc_; }

private:
  /// Used to create the byte representation of the RTCP packet.
  void build_packet(void);

  /// Add a chunk to the packet.
  void add_chunk(ACE_UINT32 ssrc);

  /// The number of chunks contained in this packet.
  /// 1 for end systems, 1+ for mixers
  unsigned long num_chunks_;

//  unsigned long num_items_;

  /// A linked list of chunks for this packet (only 1 for non-mixers).
  sdesChunk_t *chunk_;
};

/**
 * @class RTCP_SR_Packet
 * @brief The Sender Report packet is sent by all members of a session that
 * are sending data.  It contains statistics on the data being sent out. It
 * also contains a list of RR_Block to represent each source this party is
 * receiving data from.
 */

class RTCP_SR_Packet : public RTCP_Packet
{
public:
  /// Constructor for incoming SR packets.
  RTCP_SR_Packet(char *buffer, int *len);

  /// Constructor for outgoing SR packets.
  RTCP_SR_Packet(ACE_UINT32 ssrcVal,
                 ACE_UINT32 ntpMSByte,
                 ACE_UINT32 ntpLSByte,
                 ACE_UINT32 timestamp,
                 ACE_UINT32 pktsSent,
                 ACE_UINT32 octetsSent,
                 RR_Block *rrBlocks);

  /// Destructor
  virtual ~RTCP_SR_Packet(void);

  /// Returns the size of the packet in bytes.
  unsigned int packet_size(void);

  /// Returns the synchronization source id for the sender of this packet.
  ACE_UINT32 ssrc (void) { return this->ssrc_; }

  /// Returns the most significant word of the NTP timestamp.
  ACE_UINT32 ntp_ts_msw (void) { return this->ntp_ts_msw_; }

  /// Returns the least significant word of the NTP timestamp.
  ACE_UINT32 ntp_ts_lsw (void) { return this->ntp_ts_lsw_; }

  /// Prints the contents of the packet.
  void dump (void);

private:
  /// Used to create the byte representation of the RTCP packet.
  void build_packet(void);

  /// The synchronization source id of the sender generating this report.
  ACE_UINT32 ssrc_;

  /// The most significant word of the NTP timestamp.
  ACE_UINT32 ntp_ts_msw_;

  /// The least significant word of the NTP timestamp.
  ACE_UINT32 ntp_ts_lsw_;

  /// The RTP timestamp
  ACE_UINT32 rtp_ts_;

  /// The total number of packets sent.
  ACE_UINT32 psent_;

  /// The total number of octets sent.
  ACE_UINT32 osent_;

  /// A linked list of receiver report blocks.
  RR_Block *rr_;
};

#include "ace/post.h"
#endif  /*  RTCP_PACKET_INCLUDE  */
