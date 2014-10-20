// $Id$

#ifndef RTCP_CHANNEL_INCLUDE
#define RTCP_CHANNEL_INCLUDE

#include /**/ "ace/pre.h"

#include "orbsvcs/AV/RTP.h"
#include "orbsvcs/AV/RTCP_Packet.h"

#include "ace/Message_Block.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class RTCP_Channel_In
 * @brief The RTCP_Channel_In class represents a single incoming data channel,
 * or stream.  The class has several responsibilities.  When the class is
 * instantiated, the incoming RTP traffic must be declared a valid source based
 * on the RTP packets received.  Once declared valie, this class is responsible
 * for keeping up with reception statistics and other information.  When an SR
 * or RR is created (outside of this class), this class is used to get the
 * Receiver Report block for this particular stream.
 */

class RTCP_Channel_In
{
public:
  /// Constructor for an incoming channel.  Requires the synchronization source
  /// id and address of the sender.
  RTCP_Channel_In(ACE_UINT32 ssrc,
                  const ACE_Addr *addr);

  /// Destructor
  ~RTCP_Channel_In(void);

  /// This operation takes the incoming ACE_Message_Block, converts it to an
  /// RTP_Packet and calls updateStatistics.  It also uses compares the peer_addr
  /// to the stored peer_address_ to check for loops.
  void recv_rtp_packet (ACE_Message_Block *mb,
                        const ACE_Addr *peer_addr);

  /// Takes statistics of incoming RTP traffic and creates a receiver report block.
  RR_Block *getRRBlock(void);

  /// Returns the delay since last sender report.
  ACE_UINT32 dlsr (void);

  /// Returns the last sender report timestamp.
  ACE_UINT32 lsr (void);

  /// Updates channel information with incoming sender report.
  int updateStatistics(RTCP_SR_Packet *sr);

  /// Updates channel information with incoming receiver report.
  int updateStatistics(RTCP_RR_Packet *rr);

  /// Returns 1 if data has been sent since the last report, 0 if not.
  int sender (void) { return this->data_since_last_report_; }

  /// Returns 1 if this is an active source, 0 if not.
  int active (void) { return this->active_; }

private:
  /// Don't want default constructor.
  RTCP_Channel_In(void);

  /// Don't want copy constructor.
  RTCP_Channel_In(const RTCP_Channel_In &ch);

  /// The synchronization source id of the source this channel represents.
  ACE_UINT32 remote_ssrc_;

  /// The canonical name of the source this channel represents.
  ACE_CString cname_;

  // Used to declare a source valid
  /// The highest sequence number seen.
  ACE_UINT16 max_seq_;

  /// The shifted count of sequence number cycles (ie when sequence number wraps)
  ACE_UINT32 cycles_;

  /// The first sequence number received.
  ACE_UINT32 base_seq_;

  /// last 'bad' sequence number + 1
  ACE_UINT32 bad_seq_;

  /// The number of in sequence packets until a source is declared valid.
  ACE_UINT32 probation_;

  /// The number of packets received.
  ACE_UINT32 received_;

  /// The packet expected at last interval.
  ACE_UINT32 expected_prior_;

  /// The packet received at last interval.
  ACE_UINT32 received_prior_;

  /// The last transit time.
  ACE_UINT32 transit_;

  /// The inter-arrival jitter measured in timestamp units.
  double jitter_;

  // Used for jitter calculations
  /// Flag to indicate the first data packet received.
  char first_data_packet_;

  /// The first timestamp received.
  ACE_UINT32 init_time_stamp_;

  /// The local time that the initial packet was received.
  ACE_Time_Value init_local_time_;

  /// The address that the first RTP packet was received from.
  ACE_Addr *peer_address_;

  /// The most significant word of the last sender report NTP timestamp.
  ACE_UINT32 ntp_ts_msw_;

  /// The least significant word of the last sender report NTP timestamp.
  ACE_UINT32 ntp_ts_lsw_;

  /// The last sender report RTP timestamp.
  ACE_UINT32 rtp_ts_;

  /// The last time a sender report was received in 1/65536 seconds.
  /// Used to calculate DLSR.
  ACE_UINT32 last_sr_time_;

  /// Flag to indicate whether or not the source is active.
  int active_;

  /// This is a counter to indicate the number of reporting intervals that have
  /// passed since data has been received.  After 32, declare the source inactive.
  int no_data_counter_;

  /// This flag indicates that data has been received since the last report was
  /// sent.
  char data_since_last_report_;

  /// The RTP payload type.
  int payload_type_;

  /// This operation is used update statistics for the incoming RTP packet.
  void updateStatistics(RTP_Packet *pkt);

  /// This is called when the first RTP packet is received.
  void init_seq(ACE_UINT16 seq);

  /// This is called when each RTP packet is received.  It is used to declare
  /// a source as valid.
  int update_seq(ACE_UINT16 seq);
};

/**
 * @class RTCP_Channel_Out
 * @brief The RTCP_Channel_Out class represents a single outgoing data channel,
 * or stream.  It keeps track of statistics such as number of packets sent and
 * number of bytes sent.
 */

class RTCP_Channel_Out
{
public:
  /// Constructor.
  RTCP_Channel_Out(void);

  /// Destructor.
  ~RTCP_Channel_Out(void);

  /// Update the channel statistics each time an RTP packet is sent.
  void updateStatistics (RTP_Packet *pkt);

  /// Returns the timestamp of the last RTP packet sent.
  ACE_UINT32 timestamp (void);

  /// Returns the number of packets sent.
  ACE_UINT32 packets_sent (void);

  /// Returns the number of octets sent.
  ACE_UINT32 octets_sent (void);

  /// Sets the canonical name of the source.
  void cname (const char *cname) { this->cname_ = cname; }

  /// Returns the canonical name of the source.
  const char *cname (void) { return this->cname_.c_str(); }

  /// Returns whether or not this source is active.
  char active (void);

private:
  /// Holds the canonical name for this channel.
  ACE_CString cname_;

  /// Flag to indicate whether or not this channel is active.
  char active_;

  /// The sequence number of the last RTP packet sent.
  unsigned int seq_num_;

  /// The timestamp of the last RTP packet sent.
  unsigned int timestamp_;

  /// The total number of packets sent.
  unsigned int packets_sent_;

  /// The total numbef of octets sent.
  unsigned int octets_sent_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* RTCP_CHANNEL_INCLUDE */
