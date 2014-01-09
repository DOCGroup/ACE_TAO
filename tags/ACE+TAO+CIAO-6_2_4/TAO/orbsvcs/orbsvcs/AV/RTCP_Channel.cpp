// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/AV/RTCP_Channel.h"
#include "orbsvcs/AV/RTP.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

RTCP_Channel_In::RTCP_Channel_In (ACE_UINT32 ssrc,
                                  const ACE_Addr *peer_addr)
 : remote_ssrc_ (ssrc),
   cname_ (""),
   transit_ (0),
   jitter_ (0.0),
   first_data_packet_ (1),
   ntp_ts_msw_ (0),
   ntp_ts_lsw_ (0),
   last_sr_time_ (0),
   active_ (0),
   no_data_counter_ (0),
   data_since_last_report_ (0)
{
  const ACE_INET_Addr *const_inet_addr = dynamic_cast<const ACE_INET_Addr*> (peer_addr);

  ACE_INET_Addr *inet_addr;
  ACE_NEW (inet_addr,
           ACE_INET_Addr (*const_inet_addr));

  this->peer_address_ = inet_addr;
}

RTCP_Channel_In::~RTCP_Channel_In(void)
{
  delete this->peer_address_;
}

void
RTCP_Channel_In::updateStatistics(RTP_Packet* dataPkt)
{
  ACE_Time_Value current_time;
  ACE_UINT32 arrival;
  int transit, d;

  this->active_ = 0;

  // determine if the source has been declared valid
  if (update_seq(dataPkt->sn ()) == 0)
    return;

  // Set initial values if this is the first data packet.
  if (this->first_data_packet_)
    {
      // store the initial timestamp
      this->init_time_stamp_ = dataPkt->ts ();
      this->init_local_time_ = ACE_OS::gettimeofday ();
      this->first_data_packet_ = 0;
    }

  // Get the current time.
  current_time = ACE_OS::gettimeofday ();

  unsigned int samples_per_sec;
  double samples_per_usec;


  switch (dataPkt->pt())
  {
    case RTP_PT_PCMU:
    case RTP_PT_CELP:
    case RTP_PT_G721:
    case RTP_PT_GSM:
    case RTP_PT_DVI:
    case RTP_PT_LPC:
    case RTP_PT_PCMA:
    case RTP_PT_G722:
      samples_per_sec = 8000;
      break;
    case RTP_PT_L16_STEREO:
    case RTP_PT_L16_MONO:
      samples_per_sec = 44100;
      break;
    default:
      samples_per_sec = 1000000;
  };

  samples_per_usec = samples_per_sec/1000000.0;

  // Calculate the current timestamp.
  arrival = (ACE_UINT32)((current_time.sec () -
                this->init_local_time_.sec ()) * samples_per_sec +
            ((double)(current_time.usec () -
                (double)this->init_local_time_.usec ()) * samples_per_usec)  +
            this->init_time_stamp_);


  // jitter calc from RFC 1889 app a.8
  transit = arrival - dataPkt->ts ();
  d = transit - this->transit_;
  this->transit_ = transit;
  if (d < 0)
    d = -d;

  // Calculate the inter-arrival jitter.
  this->jitter_ += (1./16.)*((double)d - this->jitter_);

  // Indicate that data has been received since the last report.
  this->data_since_last_report_ = 1;

  // Store the payload type.
  this->payload_type_ = dataPkt->pt ();
}

int
RTCP_Channel_In::updateStatistics(RTCP_SR_Packet *sr)
{
  // calculate the last SR time in 1/65536 sec.
  ACE_Time_Value now = ACE_OS::gettimeofday ();
  this->last_sr_time_ = (ACE_UINT32)
                        (now.sec () * 65536 +
                         now.usec () * 0.065536);

  this->ntp_ts_msw_ = sr->ntp_ts_msw ();
  this->ntp_ts_lsw_ = sr->ntp_ts_lsw ();

  return 0;
}

int
RTCP_Channel_In::updateStatistics(RTCP_RR_Packet * /*rr*/)
{
  return 0;
}

void
RTCP_Channel_In::init_seq(ACE_UINT16 seq)
{
  this->base_seq_ = seq - 1;
  this->max_seq_ = seq;
  this->bad_seq_ = RTP_SEQ_MOD + 1;
  this->cycles_ = 0;
  this->received_ = 0;
  this->received_prior_ = 0;
  this->expected_prior_ = 0;
}

int
RTCP_Channel_In::update_seq(ACE_UINT16 seq)
{
  // The following is taken from RFC 1889 Appendix A.1
  ACE_UINT16 udelta = seq - this->max_seq_;
  const int MAX_DROPOUT = 3000;
  const int MAX_MISORDER = 100;
  const int MIN_SEQUENTIAL = 2;

  // Source is not valid until MIN_SEQUENTIAL packets with
  // sequential sequence numbers have been received.
  if (this->probation_)
    {
      // packet is in sequence
      if (seq == this->max_seq_ + (ACE_UINT16)1)
        {
          this->probation_ --;
          this->max_seq_ = seq;
          if (this->probation_ == 0)
            {
              this->init_seq(seq);
              this->received_++;
              return 1;
            }
        }
      else
        {
          this->probation_ = MIN_SEQUENTIAL - 1;
          this->max_seq_ = seq;
        }
      return 0;
    }
  else if (udelta < MAX_DROPOUT)
    {
      // in order, with permissible gap
      if (seq < this->max_seq_)
        {
          // seq number wrapped - count another 64k cycle
          this->cycles_+=RTP_SEQ_MOD;
        }
      this->max_seq_ = seq;
    }
  else if (udelta <= RTP_SEQ_MOD - MAX_MISORDER)
    {
      // the sequence number made a large jump
      ACE_UINT32 temp = seq; // Borland reports a warning on the next line
                             // without this line.
      if (temp == this->bad_seq_)
        {
          // two sequential packets, assume the other side restarted without
          // telling us so just re-sync
          // (i.e., pretend this was the first packet).
          this->init_seq (seq);

          if (TAO_debug_level > 0)
          ORBSVCS_DEBUG ((LM_DEBUG,
                      "RTCP_Channel_In: large jump in sequence number",
                      "; init seq\n"));
        }
      else
        {
          this->bad_seq_ = (seq+1)&(RTP_SEQ_MOD-1);
          return 0;
        }
    }
  else
    {
      // dup or reordered packet
    }
  this->received_++;

  return 1;
}

void
RTCP_Channel_In::recv_rtp_packet(ACE_Message_Block *mb,
                                const ACE_Addr *peer_address)
{
  if (*peer_address != *this->peer_address_)
    ORBSVCS_DEBUG ((LM_DEBUG,
                "RTCP_Channel_In::recv_rtp_packet - possible loop/collision detected"));

  RTP_Packet data_packet(mb->rd_ptr (), static_cast<int> (mb->length ()));

  // make sure the packet is valid
  if (data_packet.is_valid ())
    this->updateStatistics(&data_packet);
  else
    ORBSVCS_DEBUG ((LM_DEBUG,
                "RTCP_Channel_In::recvDataPacket - invalid RTP packet\n"));
}


RR_Block *
RTCP_Channel_In::getRRBlock(void)
{
  // If no data has been received since the last report, don't create a block.
  if (!this->data_since_last_report_)
    {
      this->no_data_counter_++;

      // make the source inactive if significant time has passed since last report
      if (this->no_data_counter_ == 32)
        this->active_ = 0;

      return 0;
    }

  this->no_data_counter_ = 0;

  RR_Block *local_block_ptr = 0;

  ACE_NEW_RETURN (local_block_ptr,
                  RR_Block,
                  0);

  ACE_OS::memset(local_block_ptr, 0, sizeof(RR_Block));

  // Set the ssrc of the source this report is for.
  local_block_ptr->ssrc_ = this->remote_ssrc_;

  // Calculate packets expected/lost (from RFC 1889 Appendix A.3)
  ACE_UINT32 extended_max;
  ACE_UINT32 expected;
  ACE_UINT32 expected_interval;
  ACE_UINT32 received_interval;
  int lost_interval;

  extended_max = this->cycles_ + this->max_seq_;
  expected = extended_max - this->base_seq_ + 1;

  local_block_ptr->lost_ = expected - this->received_;
  expected_interval = expected - this->expected_prior_;
  this->expected_prior_ = expected;
  received_interval = this->received_ - this->received_prior_;
  this->received_prior_ = this->received_;
  lost_interval = expected_interval - received_interval;

  if ((expected_interval == 0) || (lost_interval <= 0))
    local_block_ptr->fraction_ = 0;
  else
    local_block_ptr->fraction_ = (lost_interval << 8) / expected_interval;


  local_block_ptr->last_seq_ = extended_max;

  // taken from RFC 1889 App A.8
  local_block_ptr->jitter_ = (ACE_UINT32)this->jitter_;

  // calculate the last SR timestamp (lsr)
  local_block_ptr->lsr_ = ((this->ntp_ts_msw_ & 0xFFFF) << 16) |
                          ((this->ntp_ts_lsw_ & 0xFFFF0000) >> 16);

  // calculate the delay since last SR (dlsr)
  ACE_Time_Value now = ACE_OS::gettimeofday ();
  ACE_UINT32 now32 = (ACE_UINT32)
                     (now.sec () * 65536 +
                      now.usec () * 0.065536);

  local_block_ptr->dlsr_ = now32 - this->last_sr_time_;

  // indicate that no data has been received since the last report
  this->data_since_last_report_ = 0;

  return local_block_ptr;
}

RTCP_Channel_Out::RTCP_Channel_Out(void)
  :cname_ ("cname"),
   active_ (0),
   timestamp_ (0),
   timestamp_offset_ (0),
   packets_sent_ (0),
   octets_sent_ (0)
{
}

RTCP_Channel_Out::~RTCP_Channel_Out(void)
{
}

void
RTCP_Channel_Out::updateStatistics (RTP_Packet *data_packet)
{
  // indicate that this source is active
  this->active_ = 1;

  // Update various counters.
  this->octets_sent_ += data_packet->payload_size();
  this->packets_sent_ ++;
  this->seq_num_ = data_packet->sn ();
  this->timestamp_ = data_packet->ts ();
}

ACE_UINT32
RTCP_Channel_Out::timestamp (void)
{
  return this->timestamp_;
}

ACE_UINT32
RTCP_Channel_Out::packets_sent (void)
{
  return this->packets_sent_;
}

ACE_UINT32
RTCP_Channel_Out::octets_sent (void)
{
  return this->octets_sent_;
}

char
RTCP_Channel_Out::active (void)
{
  return this->active_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
