// $Id$
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/AV/RTCP_Packet.h"
#include "orbsvcs/AV/RTP.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

RTCP_Packet::RTCP_Packet(void)
{
  this->chd_.ver_ = 2;
  this->chd_.count_ = 0;
  this->chd_.pad_ = 0;
  this->chd_.length_ = 0;
  this->packet_data_ = 0;
}

RTCP_Packet::RTCP_Packet(char* buffer)
{
  // Parse the common part of the control packet header.
  this->chd_.ver_ = (buffer[0] & 0xC0) >> 6;

  if (this->chd_.ver_ != RTP_VERSION)
    ORBSVCS_DEBUG ((LM_DEBUG,
                "RTCP_Packet::RTCP_Packet version incorrect"));

  this->chd_.pad_ = (buffer[0] & 0x20) >> 5;
  this->chd_.count_ = buffer[0] & 0x1F;
  this->chd_.pt_ = buffer[1];
  this->chd_.length_ = ACE_NTOHS(*(ACE_UINT16*)&buffer[2]);
  this->packet_data_ = 0;
}

RTCP_Packet::~RTCP_Packet(void)
{
}

void
RTCP_Packet::get_packet_data(char **buffer, ACE_UINT16 &length)
{
  length = static_cast<ACE_UINT16> (this->packet_size());

  // buiidPacket is defined for each child of RTCP_Packet
  // buildPacket creates a snapshot of the RTCP packet in the buffer pktData
  this->build_packet ();

  *buffer = this->packet_data_;
}

int
RTCP_Packet::is_valid (char is_first)
{
  // make sure the RTP version is correct
  if (this->chd_.ver_ != RTP_VERSION)
    return 0;

  // these checks are only for the first RTCP packet in a compound packet
  if (is_first)
    {
      // the payload type must be RR or SR
      if ((this->chd_.pt_ != RTCP_PT_SR) && (this->chd_.pt_ != RTCP_PT_RR))
        return 0;

      // the padding bit must not be set
      if (this->chd_.pad_ != 0)
        return 0;
    }

  return 1;

}

/*
 * RTCP_BYE_Packet
 */

RTCP_BYE_Packet::RTCP_BYE_Packet(ACE_UINT32 *ssrc_list,
                                 unsigned char length,
                                 const char *text)
{
  this->chd_.ver_ = 2;
  this->chd_.count_ = length;
  this->chd_.pt_ = RTCP_PT_BYE;

  if (length)
    {
      ACE_NEW (this->ssrc_list_,
               ACE_UINT32[length]);

      this->ssrc_list_length_ = length;

      for (int i=0; i<length; i++)
        this->ssrc_list_[i] = ssrc_list[i];
    }

  // Optional - if there is a reason for leaving, store it.
  // The reason is padded with extra zeros because the packet must
  // end on an even 32-bit boundary.
  ACE_OS::memset(this->reason_, 0, sizeof(this->reason_));
  if (text)
    {
      size_t text_length = ACE_OS::strlen(text);
      ACE_OS::memcpy(this->reason_, text, text_length);
      this->reason_length_ = static_cast<unsigned char> (text_length);
    }
  else
    this->reason_length_ = 0;

  // Set the packet length
  this->chd_.length_ = static_cast<ACE_UINT16> (this->chd_.count_ + (this->reason_length_+1)/4);
  if ((this->reason_length_+1)%4)
    this->chd_.length_++;

  this->packet_data_ = 0;
}

//==============================================================================

RTCP_BYE_Packet::RTCP_BYE_Packet(char* buffer, int *len)
                 : RTCP_Packet(buffer)
{
  unsigned int index = 0;
  unsigned int j;

  // The common part of the header is initialized in the parent.
  index=4;

  ACE_NEW (this->ssrc_list_,
           ACE_UINT32[this->chd_.count_]);
  this->ssrc_list_length_ = this->chd_.count_;

  // Store the source ids of the sources leaving the session
  for (j=0; j<this->chd_.count_; j++)
    {
      this->ssrc_list_[j] = ACE_NTOHL(*(ACE_UINT32*)&buffer[index]);
      index+=4;
    }

  // Optional - store the reason for leaving
  unsigned int temp = this->chd_.length_; // Borland reports a warning on the
                                          // following line with out this.
  ACE_OS::memset(this->reason_, 0, sizeof(this->reason_));
  if (temp > this->chd_.count_)
    {
      this->reason_length_ = buffer[index];
      index++;
      ACE_OS::memcpy(this->reason_, &buffer[index], this->reason_length_);
      index+=this->reason_length_;

    }
  else
    this->reason_length_ = 0;

  // Decrement the length by the size of this message. This is necessary
  // because multiple RTCP packets may be contained in a single UDP packet.
  *len-=(chd_.length_+1)*4;

  this->packet_data_ = 0;
}

//==============================================================================

RTCP_BYE_Packet::~RTCP_BYE_Packet(void)
{
  if (this->ssrc_list_)
    delete []this->ssrc_list_;
  if (this->packet_data_)
    delete []this->packet_data_;
}

//==============================================================================

unsigned int
RTCP_BYE_Packet::packet_size(void)
{
  ACE_UINT16 size = static_cast<ACE_UINT16> ((1+chd_.count_) * 4);

  if (this->reason_length_ > 0)
    {
      size += this->reason_length_ + 1;
      if (size%4)
        size += 4 - size%4; // pad with zeros to even 32 bit bound
    }

  return size;
}

//==============================================================================

void
RTCP_BYE_Packet::ssrc_list(ACE_UINT32 **ssrc_list, unsigned char &length)
{
  *ssrc_list = this->ssrc_list_;
  length = this->ssrc_list_length_;
}

//==============================================================================

const char *
RTCP_BYE_Packet::reason (void)
{
  ACE_CString reason = (const char *)this->reason_;

  return reason.c_str();
}

//==============================================================================

void
RTCP_BYE_Packet::build_packet(void)
{
  unsigned int index;
  unsigned int i;

  if (this->packet_data_)
    delete []this->packet_data_;

  ACE_NEW (this->packet_data_,
           char[this->packet_size()]);

  index = 0;
  this->packet_data_[index] = static_cast<char> ((this->chd_.ver_ << 6) |
                                                 (this->chd_.pad_ << 5) |
                                                 this->chd_.count_);
  index++;
  this->packet_data_[index] = this->chd_.pt_;
  index++;
  *((ACE_UINT16*)&this->packet_data_[index]) = ACE_HTONS(this->chd_.length_);
  index+=2;

  for (i=0; i<this->chd_.count_; i++)
    {
      *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(this->ssrc_list_[i]);
      index+=4;
    }

  if (this->reason_)
    {
      this->packet_data_[index] = this->reason_length_;
      index++;
      ACE_OS::memcpy(&this->packet_data_[index], this->reason_, this->reason_length_);
      index += this->reason_length_;
      while (index < this->packet_size())
        {
          this->packet_data_[index] = 0;
          index ++;
        }
    }
}

void
RTCP_BYE_Packet::dump (void)
{
  ORBSVCS_DEBUG ((LM_DEBUG,
              "\nRTCP_BYE_Packet:: from ssrc(s) "));
  for (int i=0; i< this->ssrc_list_length_; i++)
    ORBSVCS_DEBUG ((LM_DEBUG,
                "%u ",
                this->ssrc_list_[i]));
  ORBSVCS_DEBUG ((LM_DEBUG,
              "\n    Reason '%s'\n",
              this->reason_));
}

RTCP_RR_Packet::RTCP_RR_Packet(ACE_UINT32 ssrc, RR_Block *blocks)
{
  RR_Block *block_ptr = blocks;

  this->chd_.count_ = 0;
  this->chd_.ver_ = 2;
  this->chd_.pt_ = RTCP_PT_RR;
  this->ssrc_ = ssrc;
  this->rr_ = blocks;

  while (block_ptr)
    {
      this->chd_.count_++;

      // Can only have 31 receiver reports
      if (this->chd_.count_ == 31)
        {
          block_ptr->next_ = 0;
          break;
        }

      block_ptr = block_ptr->next_;
    }

  this->chd_.length_ = static_cast<ACE_UINT16> (1+6*(this->chd_.count_)); // + profile specific extensions ??

  this->packet_data_ = 0;
}

//==============================================================================

RTCP_RR_Packet::RTCP_RR_Packet (char* buffer,
                                int *len)
  :RTCP_Packet (buffer)
{
  unsigned int i = 0;
  RR_Block *local_block_ptr = 0;

  this->rr_ = 0;

  // The common part of the header is initialized in the parent.
  i=4;
  this->ssrc_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
  i+=4;
  for (unsigned int j=0; j<this->chd_.count_; j++)
    {
      if (j==0)
        {
          ACE_NEW (this->rr_,
                   RR_Block);
          local_block_ptr = this->rr_;
        }
      else
        {
          ACE_NEW (local_block_ptr->next_,
                   RR_Block);
          local_block_ptr = local_block_ptr->next_;
        }

      local_block_ptr->next_ = 0;
      local_block_ptr->ssrc_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
      i+=4;
      ACE_UINT32 temp = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
      local_block_ptr->fraction_ = (temp&0xff000000) >> 24;
      local_block_ptr->lost_ = temp & 0x00ffffff;
      i+=4;
      local_block_ptr->last_seq_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
      i+=4;
      local_block_ptr->jitter_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
      i+=4;
      local_block_ptr->lsr_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
      i+=4;
      local_block_ptr->dlsr_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
      i+=4;
    }

  *len-=(this->chd_.length_+1)*4;

  this->packet_data_ = 0;
}

//==============================================================================

RTCP_RR_Packet::~RTCP_RR_Packet(void)
{
  RR_Block *prev;

  if (this->rr_)
    {
      while (this->rr_)
        {
          prev = this->rr_;
          this->rr_ = this->rr_->next_;
          delete prev;
        }
    }

  if (this->packet_data_)
    delete []this->packet_data_;
}

//==============================================================================

unsigned int
RTCP_RR_Packet::packet_size(void)
{
    ACE_UINT16 size = static_cast<ACE_UINT16> ((2+this->chd_.count_*6) * 4);
    return size;
}

//==============================================================================

void
RTCP_RR_Packet::build_packet(void)
{
  int index;
  RR_Block *local_block_ptr;

  if (this->packet_data_)
    delete []this->packet_data_;

  ACE_NEW (this->packet_data_,
           char [this->packet_size ()]);

  index = 0;
  this->packet_data_[index] = static_cast<char> ((this->chd_.ver_ << 6) |
                                                 (this->chd_.pad_ << 5) |
                                                 this->chd_.count_);
  index++;
  this->packet_data_[index] = chd_.pt_;
  index++;
  *((ACE_UINT16*)&this->packet_data_[index]) = ACE_HTONS(chd_.length_);
  index+=2;
  *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(this->ssrc_);
  index+=4;

  local_block_ptr = this->rr_;
  while (local_block_ptr)
    {
      *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(local_block_ptr->ssrc_);
      index+=4;
      ACE_UINT32 temp = ACE_HTONL((local_block_ptr->fraction_&0xff) << 24) &
                              local_block_ptr->lost_;
      *((ACE_UINT32*)&this->packet_data_[index]) = temp;
      index+=4;
      *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(local_block_ptr->last_seq_);
      index+=4;
      *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(local_block_ptr->jitter_);
      index+=4;
      *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(local_block_ptr->lsr_);
      index+=4;
      *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(local_block_ptr->dlsr_);
      index+=4;
      local_block_ptr = local_block_ptr->next_;
    }
}

void
RTCP_RR_Packet::dump (void)
{
  RR_Block *b = this->rr_;
  int count = 1;

  ORBSVCS_DEBUG ((LM_DEBUG,
              "\nRTCP_RR_Packet:: from %u - %d rr blocks follow.\n",
              this->ssrc_,
              this->chd_.count_));

  while (b)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  "  Block %d: ssrc %u; frac %u; lost %u; last seq %u\n",
                  count,
                  b->ssrc_,
                  b->fraction_,
                  b->lost_,
                  b->last_seq_));
      ORBSVCS_DEBUG ((LM_DEBUG,
                  "           jitter %u; lsr %u; dlsr %u;\n",
                  b->jitter_,
                  b->lsr_,
                  b->dlsr_));

      b = b->next_;
      ++count;
    }
}


RTCP_SDES_Packet::RTCP_SDES_Packet(void) :
                   RTCP_Packet ()
{
  this->chd_.pt_ = RTCP_PT_SDES;
  this->chunk_ = 0;
  this->packet_data_ = 0;
  this->num_chunks_ = 0;
//  this->num_items_ = 0;
}

//==============================================================================

RTCP_SDES_Packet::RTCP_SDES_Packet(char* buffer, int *len):
                   RTCP_Packet (buffer)
{
  unsigned int i;
  sdesChunk_t *cp = 0; // pointer to chunk
  sdesItem_t *ip = 0; // pointer to item

  // The common part of the control packet header is processed
  // in the parent. It is 4 bytes long.

  i=4;
  for (unsigned int j=0; j<this->chd_.count_; j++)
    {
      if (j==0)
        {
          ACE_NEW (this->chunk_,
                   sdesChunk_t);
          cp = this->chunk_;

          this->num_chunks_ = 1;
        }
      else
        {
          ACE_NEW (cp->next_,
                   sdesChunk_t);
          cp = cp->next_;

          this->num_chunks_++;
        }
      cp->next_ = 0;
      cp->item_ = 0;
      cp->ssrc_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
      i+=4;

      while (buffer[i]!=RTCP_SDES_END)
        {
          if (!cp->item_)
            {
              ACE_NEW (cp->item_,
                       sdesItem_t);
              ip = cp->item_;

//              this->num_items_ = 1;
            }
          else
            {
              ACE_NEW (ip->next_,
                       sdesItem_t);
              ip = ip->next_;

//              this->num_items_++;
            }
          ip->next_ = 0;
          ip->type_ = buffer[i];
          i++;
          if (ip->type_ != RTCP_SDES_PRIV)
            {
              ip->info_.standard_.length_ = buffer[i];
              i++;
              ACE_NEW (ip->info_.standard_.data_,
                       char[ip->info_.standard_.length_+1]);
              ACE_OS::memcpy(ip->info_.standard_.data_,
                             &buffer[i],
                             ip->info_.standard_.length_);
              ip->info_.standard_.data_[ip->info_.standard_.length_] = 0;
              i+=ip->info_.standard_.length_;
            }
          else
            {
              ip->info_.priv_.name_length_ = buffer[i];
              i++;
              ip->info_.priv_.data_length_ = buffer[i];
              i++;
              ACE_NEW (ip->info_.priv_.name_,
                       char[ip->info_.priv_.name_length_+1]);
              ACE_OS::memcpy(ip->info_.priv_.name_,
                             &buffer[i],
                             ip->info_.priv_.name_length_);
              ip->info_.priv_.name_[ip->info_.priv_.name_length_] = 0;
              i+=ip->info_.priv_.name_length_;
              ACE_NEW (ip->info_.priv_.data_,
                       char[ip->info_.priv_.data_length_+1]);
              ACE_OS::memcpy(ip->info_.priv_.data_,
                             &buffer[i],
                             ip->info_.priv_.data_length_);
              ip->info_.priv_.data_[ip->info_.priv_.data_length_] = 0;
              i+=ip->info_.priv_.data_length_;
            }
        }
        i++; // each chunk ends with a zero (END) item
        // each chunk must end on an even 32 bit boundary
        while (i%4) i++;
    }

  *len-=(this->chd_.length_+1)*4;

  this->packet_data_ = 0;
}

//==============================================================================

RTCP_SDES_Packet::~RTCP_SDES_Packet(void)
{
  sdesChunk_t *cp; // pointer to chunk
  sdesChunk_t *cpprev;
  sdesItem_t *ip; // pointer to item
  sdesItem_t *ipprev;

  cp = this->chunk_;
  while (cp)
    {
      ip = cp->item_;
      while (ip)
        {
          ipprev = ip;
          ip = ip->next_;
          if (ipprev->type_ != RTCP_SDES_PRIV)
            {
              delete []ipprev->info_.standard_.data_;
            }
          else
            {
              delete []ipprev->info_.priv_.name_;
              delete []ipprev->info_.priv_.data_;
            }

          delete ipprev;
        }
      cpprev = cp;
      cp = cp->next_;
      delete cpprev;
    }

  if (this->packet_data_)
    delete []this->packet_data_;
}

//==============================================================================

void
RTCP_SDES_Packet::add_chunk(ACE_UINT32 ssrc)
{
  sdesChunk_t *cp = 0; // pointer to chunk

  // If this is the first chunk.
  if (chd_.count_ == 0)
    {
      ACE_NEW (this->chunk_,
               sdesChunk_t);
      this->chunk_->next_ = 0;
      this->chunk_->item_ = 0;
      cp = this->chunk_;
    }
  else
    {
      cp = this->chunk_;
      while (cp->next_)
        cp = cp->next_;

      ACE_NEW (cp->next_,
               sdesChunk_t);
      cp = cp->next_;
      cp->next_ = 0;
      cp->item_ = 0;
    }
  cp->ssrc_ = ssrc;    // store the source
  chd_.count_++;       // increment the source count
}

//==============================================================================

void
RTCP_SDES_Packet::add_item (ACE_UINT32 ssrc,
                            unsigned char type,
                            unsigned char length,
                            const char *data)
{
  sdesChunk_t *cp; // pointer to chunk
  sdesItem_t *ip; // pointer to item

  if (this->chunk_ == 0)
    {
      this->add_chunk(ssrc);
    }

  cp = this->chunk_;

  while (cp != 0)
    {
      if (cp->ssrc_ == ssrc)
        {
          break;
        }

      if (!cp->next_)
        {
          this->add_chunk(ssrc);
          cp = cp->next_;
          break;
        }

      cp = cp->next_;
    }

  ip = cp->item_;

  if (ip == 0)
    {
      ACE_NEW (cp->item_,
               sdesItem_t);

      ip = cp->item_;
      ip->next_= 0;
    }
  else
    {
      while (ip->next_)
        {
          ip = ip->next_;
        }

      ACE_NEW (ip->next_,
               sdesItem_t);

      ip = ip->next_;
      ip->next_ = 0;
    }

  ip->type_ = type;

  ip->info_.standard_.length_ = length;

  ACE_NEW (ip->info_.standard_.data_,
           char[length]);

  ACE_OS::memcpy(ip->info_.standard_.data_, data, length);
}

//==============================================================================

void
RTCP_SDES_Packet::add_priv_item (ACE_UINT32 ssrc,
                                 unsigned char nameLength,
                                 const char* name,
                                 unsigned char dataLength,
                                 const char* data)
{
  sdesChunk_t *cp; // pointer to chunk
  sdesItem_t *ip; // pointer to item

  if (this->chunk_ == 0)
    {
      this->add_chunk(ssrc);
    }

  cp = this->chunk_;

  while (cp != 0)
    {
      if (cp->ssrc_ == ssrc)
        {
          break;
        }

      if (!cp->next_)
        {
          this->add_chunk(ssrc);
          cp = cp->next_;
          break;
        }

      cp = cp->next_;
    }

  ip = cp->item_;

  if (ip == 0)
    {
      ACE_NEW (cp->item_,
               sdesItem_t);

      ip = cp->item_;
      ip->next_ = 0;
    }
  else
    {
      while (ip->next_)
        {
          ip = ip->next_;
        }

      ACE_NEW (ip->next_,
               sdesItem_t);

      ip = ip->next_;
      ip->next_ = 0;
    }

  ip->type_ = RTCP_SDES_PRIV;

  ip->info_.priv_.name_length_ = nameLength;
  ip->info_.priv_.data_length_ = dataLength;

  ACE_NEW (ip->info_.priv_.name_,
           char[nameLength]);

  ACE_NEW (ip->info_.priv_.data_,
           char[dataLength]);

  ACE_OS::memcpy(ip->info_.priv_.name_, name, nameLength);
  ACE_OS::memcpy(ip->info_.priv_.data_, data, dataLength);
}

//==============================================================================

unsigned int
RTCP_SDES_Packet::packet_size(void)
{
  int size;
  sdesChunk_t *cp; // pointer to chunk
  sdesItem_t *ip; // pointer to item

  // Determine the size of the packet.
  size = 4;  // size of common header data in octets

  cp = this->chunk_;
  while (cp)
    {
      size += 4; // size of ssrc
      ip = cp->item_;

      while (ip && (ip->type_ != 0))
        {
          if (ip->type_ != RTCP_SDES_PRIV)
            {
              size += 2 + ip->info_.standard_.length_;  // size of item
            }
          else
            {
              size += 3 + ip->info_.priv_.name_length_ + ip->info_.priv_.data_length_;
            }
          ip = ip->next_;
        }
      size += 4 - size%4;           // pad with zeros to even 32 bit bound
      cp = cp->next_;
    }

  chd_.length_ = static_cast<ACE_UINT16> (size/4 - 1);

  return size;
}

//==============================================================================

void
RTCP_SDES_Packet::build_packet(void)
{
  sdesChunk_t *cp; // pointer to chunk
  sdesItem_t *ip; // pointer to item
  int index, i;

  if (this->packet_data_)
    delete this->packet_data_;

  ACE_NEW (this->packet_data_,
           char[this->packet_size()]);

  index = 0;
  this->packet_data_[index] = static_cast<char> ((chd_.ver_ << 6) |
                                                 (chd_.pad_ << 5) |
                                                  chd_.count_);
  index++;
  this->packet_data_[index] = chd_.pt_;
  index++;
  *((ACE_UINT16*)&this->packet_data_[index]) = ACE_HTONS(chd_.length_);
  index+=2;

  cp = this->chunk_;
  while (cp)
    {
      *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(cp->ssrc_);
      index+=4;

      ip = cp->item_;
      while (ip && (ip->type_ != 0))
        {
          this->packet_data_[index] = ip->type_;
          index++;
          if (ip->type_ != RTCP_SDES_PRIV)
            {
              this->packet_data_[index] = ip->info_.standard_.length_;
              index++;
              for (i=0; i<ip->info_.standard_.length_; i++)
                {
                  this->packet_data_[index] = ip->info_.standard_.data_[i];
                  index++;
                }
            }
          else
            {
              this->packet_data_[index] = ip->info_.priv_.name_length_;
              index++;
              this->packet_data_[index] = ip->info_.priv_.data_length_;
              index++;
              for (i=0; i<ip->info_.priv_.name_length_; i++)
                {
                  this->packet_data_[index] = ip->info_.priv_.name_[i];
                  index++;
                }
              for (i=0; i<ip->info_.priv_.data_length_; i++)
                {
                  this->packet_data_[index] = ip->info_.priv_.data_[i];
                  index++;
                }
            }

          ip = ip->next_;
        }

      this->packet_data_[index] = 0;
      index++;

      i=1;
      while ((index)%4)
        { // pad chunk with zeros to 32 bit bound
          this->packet_data_[index] = 0;
          index++;
          i++;
        }

      // Store the number of bytes added. TODO: do we need this
      // this->packet_data_[index - 1] = i;

      cp = cp->next_;
    }
}

void
RTCP_SDES_Packet::dump (void)
{
  sdesItem_t *ip;

  ORBSVCS_DEBUG ((LM_DEBUG,
              "\nRTCP_SDES_Packet:: "));

  if (this->num_chunks_ != 1)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  "Mixers not currently supported.\n"));
      return;
    }

  ORBSVCS_DEBUG ((LM_DEBUG,
              "from ssrc %u\n",
              this->chunk_->ssrc_));

  // Loop through all of the items.
  ip = this->chunk_->item_;

  while (ip)
    {
      // If there is no data to store, continue.
      if (ip->info_.standard_.length_ == 0)
        {
          ip = ip->next_;
          continue;
        }

      switch (ip->type_)
      {
        case RTCP_SDES_END:
             break;
        case RTCP_SDES_CNAME:
             ORBSVCS_DEBUG ((LM_DEBUG,
                         "    CNAME '%s'\n",
                         ip->info_.standard_.data_));
             break;
        case RTCP_SDES_NAME:
             ORBSVCS_DEBUG ((LM_DEBUG,
                         "    NAME '%s'\n",
                         ip->info_.standard_.data_));
             break;
        case RTCP_SDES_EMAIL:
             ORBSVCS_DEBUG ((LM_DEBUG,
                         "    EMAIL '%s'\n",
                         ip->info_.standard_.data_));
             break;
        case RTCP_SDES_PHONE:
             ORBSVCS_DEBUG ((LM_DEBUG,
                         "    PHONE '%s'\n",
                         ip->info_.standard_.data_));
             break;
        case RTCP_SDES_LOC:
             ORBSVCS_DEBUG ((LM_DEBUG,
                         "    LOC '%s'\n",
                         ip->info_.standard_.data_));
             break;
        case RTCP_SDES_TOOL:
             ORBSVCS_DEBUG ((LM_DEBUG,
                         "    TOOL '%s'\n",
                         ip->info_.standard_.data_));
             break;
        case RTCP_SDES_NOTE:
             ORBSVCS_DEBUG ((LM_DEBUG,
                         "    NOTE '%s'\n",
                         ip->info_.standard_.data_));
             break;
        case RTCP_SDES_PRIV:
             ORBSVCS_DEBUG ((LM_DEBUG,
                         "    '%s' '%s'\n",
                         ip->info_.priv_.name_,
                         ip->info_.priv_.data_));
             break;
      }
      ip = ip->next_;
    }

}

RTCP_SR_Packet::RTCP_SR_Packet(ACE_UINT32 ssrc,
                               ACE_UINT32 ntp_ts_msw,
                               ACE_UINT32 ntp_ts_lsw,
                               ACE_UINT32 timestamp,
                               ACE_UINT32 packets_sent,
                               ACE_UINT32 octets_sent,
                               RR_Block *blocks)
{
  RR_Block *block_ptr= 0;
  chd_.count_ = 0;
  chd_.ver_ = 2;
  chd_.pt_ = RTCP_PT_SR;

  this->ssrc_ = ssrc;
  this->ntp_ts_msw_ = ntp_ts_msw;
  this->ntp_ts_lsw_ = ntp_ts_lsw;
  this->rtp_ts_ = timestamp;
  this->psent_ = packets_sent;
  this->osent_ = octets_sent;
  this->rr_ = blocks;

  block_ptr = blocks;

  while (block_ptr)
    {
      chd_.count_++;

      // Can only have 31 receiver reports
      if (this->chd_.count_ == 31)
        {
          block_ptr->next_ = 0;
          break;
        }

      block_ptr = block_ptr->next_;
    }

  this->chd_.length_ = static_cast<ACE_UINT16> (6 + 6*(chd_.count_)); //+profile specific extensions ??

  this->packet_data_ = 0;
}

//==============================================================================

RTCP_SR_Packet::RTCP_SR_Packet (char* buffer,
                                int *len)
                : RTCP_Packet (buffer)
{
  unsigned int i = 0;
  RR_Block *local_block_ptr = 0;

  this->rr_ = 0;

  // The common part of the header is initialized in the parent.
  i=4;
  this->ssrc_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
  i+=4;
  this->ntp_ts_msw_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
  i+=4;
  this->ntp_ts_lsw_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
  i+=4;
  this->rtp_ts_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
  i+=4;
  this->psent_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
  i+=4;
  this->osent_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
  i+=4;
  for (unsigned int j=0; j<this->chd_.count_; j++)
    {
      if (j==0)
        {
          ACE_NEW (local_block_ptr,
                   RR_Block);
          this->rr_ = local_block_ptr;
        }
      else
        {
          ACE_NEW (local_block_ptr->next_,
                   RR_Block);
          local_block_ptr = local_block_ptr->next_;
        }

      local_block_ptr->next_ = 0;
      local_block_ptr->ssrc_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
      i+=4;
      ACE_UINT32 temp = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
      local_block_ptr->fraction_ = (temp&0xff000000) >> 24;
      local_block_ptr->lost_ = temp & 0x00ffffff;
      i+=4;
      local_block_ptr->last_seq_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
      i+=4;
      local_block_ptr->jitter_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
      i+=4;
      local_block_ptr->lsr_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
      i+=4;
      local_block_ptr->dlsr_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
      i+=4;
    }

  *len-=(this->chd_.length_+1)*4;

  this->packet_data_ = 0;
}

//==============================================================================

RTCP_SR_Packet::~RTCP_SR_Packet(void)
{
  RR_Block *prev;

  if (this->rr_)
    {
      while (this->rr_)
        {
          prev = this->rr_;
          this->rr_ = this->rr_->next_;
          delete prev;
        }
    }

  if (this->packet_data_)
    delete []this->packet_data_;
}

//==============================================================================

unsigned int RTCP_SR_Packet::packet_size (void)
{
  ACE_UINT16 size  = static_cast<ACE_UINT16> ((2+chd_.count_*6) * 4); // + profile specific extensions ?
  size += 20; // the first line is the same as RR; 20 more bytes for SR

  return size;
}

//==============================================================================

void RTCP_SR_Packet::build_packet(void)
{
  int index = 0;
  RR_Block *local_block_ptr;

  if (this->packet_data_)
    delete []this->packet_data_;

  ACE_NEW (this->packet_data_,
           char[this->packet_size()]);

  this->packet_data_[index] = static_cast<char> ((this->chd_.ver_ << 6) |
                                                 (this->chd_.pad_ << 5) |
                                                  this->chd_.count_);
  index++;
  this->packet_data_[index] = this->chd_.pt_;
  index++;
  *((ACE_UINT16*)&this->packet_data_[index]) = ACE_HTONS(this->chd_.length_);
  index+=2;
  *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(this->ssrc_);
  index+=4;
  *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(this->ntp_ts_msw_);
  index+=4;
  *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(this->ntp_ts_lsw_);
  index+=4;
  *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(this->rtp_ts_);
  index+=4;
  *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(this->psent_);
  index+=4;
  *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(this->osent_);
  index+=4;

  local_block_ptr = this->rr_;
  while (local_block_ptr)
    {
      *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(local_block_ptr->ssrc_);
      index+=4;
      ACE_UINT32 temp = ACE_HTONL((local_block_ptr->fraction_&0xff) << 24) &
                              local_block_ptr->lost_;
      *((ACE_UINT32*)&this->packet_data_[index]) = temp;
      index+=4;
      *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(local_block_ptr->last_seq_);
      index+=4;
      *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(local_block_ptr->jitter_);
      index+=4;
      *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(local_block_ptr->lsr_);
      index+=4;
      *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(local_block_ptr->dlsr_);
      index+=4;
      local_block_ptr = local_block_ptr->next_;
    }
}

void
RTCP_SR_Packet::dump (void)
{
  RR_Block *b = this->rr_;
  int count = 1;

  ORBSVCS_DEBUG ((LM_DEBUG,
              "\nRTCP_SR_Packet:: from %u - %d rr blocks follow.\n",
              this->ssrc_,
              this->chd_.count_));
  ORBSVCS_DEBUG ((LM_DEBUG,
              "    NTP(sec) %u.%u; RTP ts %u\n",
              this->ntp_ts_msw_,
              this->ntp_ts_lsw_,
              this->rtp_ts_));
  ORBSVCS_DEBUG ((LM_DEBUG,
              "    packets sent %u; octets sent %u\n",
              this->psent_,
              this->osent_));

  while (b)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  "  Block %d: ssrc %u; frac %u; lost %u; last seq %u\n",
                  count,
                  b->ssrc_,
                  b->fraction_,
                  b->lost_,
                  b->last_seq_));
      ORBSVCS_DEBUG ((LM_DEBUG,
                  "           jitter %u; lsr %u; dlsr %u;\n",
                  b->jitter_,
                  b->lsr_,
                  b->dlsr_));

      b = b->next_;
      ++count;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
