#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/AV/RTCP_Packet.h"
#include "orbsvcs/AV/RTP.h"

#include <cerrno>
#include <cstring>
#include <new>
#include <utility>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

RTCP_Packet::RTCP_Packet()
{
  this->chd_.ver_ = 2;
  this->chd_.count_ = 0;
  this->chd_.pad_ = 0;
  this->chd_.length_ = 0;
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
}

RTCP_Packet::~RTCP_Packet() = default;

void
RTCP_Packet::get_packet_data(char **buffer, ACE_UINT16 &length)
{
  length = static_cast<ACE_UINT16> (this->packet_size());

  // buiidPacket is defined for each child of RTCP_Packet
  // buildPacket creates a snapshot of the RTCP packet in the buffer pktData
  this->build_packet ();

  *buffer = this->packet_data_.get ();
}

bool
RTCP_Packet::is_valid (bool is_first)
{
  // make sure the RTP version is correct
  if (this->chd_.ver_ != RTP_VERSION)
    return false;

  // these checks are only for the first RTCP packet in a compound packet
  if (is_first)
    {
      // the payload type must be RR or SR
      if ((this->chd_.pt_ != RTCP_PT_SR) && (this->chd_.pt_ != RTCP_PT_RR))
        return false;

      // the padding bit must not be set
      if (this->chd_.pad_ != 0)
        return false;
    }

  return true;
}

/*
 * RTCP_BYE_Packet
 */

RTCP_BYE_Packet::RTCP_BYE_Packet(ACE_UINT32 *ssrc_list,
                                 unsigned char length,
                                 char const *text)
{
  this->chd_.ver_ = 2;
  this->chd_.count_ = length;
  this->chd_.pt_ = RTCP_PT_BYE;

  if (length)
    {
      this->ssrc_list_.reset (new (std::nothrow) ACE_UINT32[length]);
      if (!this->ssrc_list_)
        {
          errno = ENOMEM;
          return;
        }

      this->ssrc_list_length_ = length;

      for (int i {}; i < length; ++i)
        this->ssrc_list_[i] = ssrc_list[i];
    }

  // Optional - if there is a reason for leaving, store it.
  // The reason is padded with extra zeros because the packet must
  // end on an even 32-bit boundary.
  if (text)
    {
      size_t text_length = std::strlen(text);
      if (text_length >= sizeof (this->reason_))
        text_length = sizeof (this->reason_) - 1;
      std::memcpy(this->reason_, text, text_length);
      this->reason_length_ = static_cast<unsigned char> (text_length);
    }
  else
    this->reason_length_ = 0;

  // Set the packet length
  this->chd_.length_ = static_cast<ACE_UINT16> (this->chd_.count_ + (this->reason_length_+1)/4);
  if ((this->reason_length_+1)%4)
    ++this->chd_.length_;
}

//==============================================================================

RTCP_BYE_Packet::RTCP_BYE_Packet(char* buffer, int *len)
                 : RTCP_Packet(buffer)
{
  unsigned int index = 0;
  unsigned int j;

  // The common part of the header is initialized in the parent.
  index=4;

  this->ssrc_list_.reset (new (std::nothrow) ACE_UINT32[this->chd_.count_]);
  if (!this->ssrc_list_)
    {
      errno = ENOMEM;
      return;
    }
  this->ssrc_list_length_ = this->chd_.count_;

  // Store the source ids of the sources leaving the session
  for (j=0; j<this->chd_.count_; ++j)
    {
      this->ssrc_list_[j] = ACE_NTOHL(*(ACE_UINT32*)&buffer[index]);
      index+=4;
    }

  // Optional - store the reason for leaving
  if (this->chd_.length_ > this->chd_.count_)
    {
      this->reason_length_ = buffer[index];
      ++index;
      std::memcpy(this->reason_, &buffer[index], this->reason_length_);
      index+=this->reason_length_;
    }
  else
    this->reason_length_ = 0;

  // Decrement the length by the size of this message. This is necessary
  // because multiple RTCP packets may be contained in a single UDP packet.
  *len-=(chd_.length_+1)*4;
}

//==============================================================================

RTCP_BYE_Packet::~RTCP_BYE_Packet() = default;

//==============================================================================

unsigned int
RTCP_BYE_Packet::packet_size()
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
  *ssrc_list = this->ssrc_list_.get ();
  length = this->ssrc_list_length_;
}

//==============================================================================

char const *
RTCP_BYE_Packet::reason ()
{
  return this->reason_;
}

//==============================================================================

void
RTCP_BYE_Packet::build_packet()
{
  unsigned int index;
  unsigned int i;

  this->packet_data_.reset (new (std::nothrow) char[this->packet_size ()]);
  if (!this->packet_data_)
    {
      errno = ENOMEM;
      return;
    }

  index = 0;
  this->packet_data_[index] = static_cast<char> ((this->chd_.ver_ << 6) |
                                                 (this->chd_.pad_ << 5) |
                                                 this->chd_.count_);
  ++index;
  this->packet_data_[index] = this->chd_.pt_;
  ++index;
  *((ACE_UINT16*)&this->packet_data_[index]) = ACE_HTONS(this->chd_.length_);
  index+=2;

  for (i=0; i<this->chd_.count_; ++i)
    {
      *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(this->ssrc_list_[i]);
      index+=4;
    }

  if (this->reason_length_)
    {
      this->packet_data_[index] = this->reason_length_;
      ++index;
      std::memcpy(&this->packet_data_[index], this->reason_, this->reason_length_);
      index += this->reason_length_;
      while (index < this->packet_size())
        {
          this->packet_data_[index] = 0;
          ++index;
        }
    }
}

void
RTCP_BYE_Packet::dump ()
{
  ORBSVCS_DEBUG ((LM_DEBUG,
              "\nRTCP_BYE_Packet:: from ssrc(s) "));
  for (int i {}; i < this->ssrc_list_length_; ++i)
    ORBSVCS_DEBUG ((LM_DEBUG,
                "%u ",
                this->ssrc_list_[i]));
  ORBSVCS_DEBUG ((LM_DEBUG,
              "\n    Reason '%s'\n",
              this->reason_));
}

RTCP_RR_Packet::RTCP_RR_Packet (ACE_UINT32 ssrc,
                                std::unique_ptr<RR_Block> blocks)
{
  RR_Block *block_ptr = blocks.get ();

  this->chd_.count_ = 0;
  this->chd_.ver_ = 2;
  this->chd_.pt_ = RTCP_PT_RR;
  this->ssrc_ = ssrc;
  this->rr_ = std::move (blocks);

  while (block_ptr)
    {
      ++this->chd_.count_;

      // Can only have 31 receiver reports
      if (this->chd_.count_ == 31)
        {
          block_ptr->next_.reset ();
          break;
        }

      block_ptr = block_ptr->next_.get ();
    }

  this->chd_.length_ = static_cast<ACE_UINT16> (1+6*(this->chd_.count_)); // + profile specific extensions ??
}

//==============================================================================

RTCP_RR_Packet::RTCP_RR_Packet (char* buffer,
                                int *len)
  :RTCP_Packet (buffer)
{
  unsigned int i = 0;
  std::unique_ptr<RR_Block> *next_block = &this->rr_;

  // The common part of the header is initialized in the parent.
  i=4;
  this->ssrc_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
  i+=4;
  for (unsigned int j {}; j < this->chd_.count_; ++j)
    {
      *next_block = std::unique_ptr<RR_Block> (
        new (std::nothrow) RR_Block {});
      if (*next_block == nullptr)
        {
          errno = ENOMEM;
          return;
        }

      RR_Block * const local_block_ptr = next_block->get ();
      next_block = &local_block_ptr->next_;
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
}

//==============================================================================

RTCP_RR_Packet::~RTCP_RR_Packet() = default;

//==============================================================================

unsigned int
RTCP_RR_Packet::packet_size()
{
    ACE_UINT16 size = static_cast<ACE_UINT16> ((2+this->chd_.count_*6) * 4);
    return size;
}

//==============================================================================

void
RTCP_RR_Packet::build_packet()
{
  int index;
  RR_Block *local_block_ptr;

  this->packet_data_.reset (new (std::nothrow) char[this->packet_size ()]);
  if (!this->packet_data_)
    {
      errno = ENOMEM;
      return;
    }

  index = 0;
  this->packet_data_[index] = static_cast<char> ((this->chd_.ver_ << 6) |
                                                 (this->chd_.pad_ << 5) |
                                                 this->chd_.count_);
  ++index;
  this->packet_data_[index] = chd_.pt_;
  ++index;
  *((ACE_UINT16*)&this->packet_data_[index]) = ACE_HTONS(chd_.length_);
  index+=2;
  *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(this->ssrc_);
  index+=4;

  local_block_ptr = this->rr_.get ();
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
      local_block_ptr = local_block_ptr->next_.get ();
    }
}

void
RTCP_RR_Packet::dump ()
{
  RR_Block const *b = this->rr_.get ();
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

      b = b->next_.get ();
      ++count;
    }
}

RTCP_SDES_Packet::RTCP_SDES_Packet() :
                   RTCP_Packet ()
{
  this->chd_.pt_ = RTCP_PT_SDES;
//  this->num_items_ = 0;
}

//==============================================================================

RTCP_SDES_Packet::RTCP_SDES_Packet(char* buffer, int *len):
                   RTCP_Packet (buffer)
{
  unsigned int i;
  std::unique_ptr<sdesChunk_t> *next_chunk = &this->chunk_;

  // The common part of the control packet header is processed
  // in the parent. It is 4 bytes long.

  i=4;
  for (unsigned int j {}; j < this->chd_.count_; ++j)
    {
      *next_chunk = std::unique_ptr<sdesChunk_t> (
        new (std::nothrow) sdesChunk_t {});
      if (*next_chunk == nullptr)
        {
          errno = ENOMEM;
          return;
        }

      sdesChunk_t * const cp = next_chunk->get ();
      next_chunk = &cp->next_;
      ++this->num_chunks_;
      cp->ssrc_ = ACE_NTOHL(*(ACE_UINT32*)&buffer[i]);
      i+=4;

      std::unique_ptr<sdesItem_t> *next_item = &cp->item_;
      while (buffer[i]!=RTCP_SDES_END)
        {
          *next_item = std::unique_ptr<sdesItem_t> (
            new (std::nothrow) sdesItem_t {});
          if (*next_item == nullptr)
            {
              errno = ENOMEM;
              return;
            }

          sdesItem_t * const ip = next_item->get ();
          next_item = &ip->next_;
          ip->type_ = buffer[i];
          ++i;
          if (ip->type_ != RTCP_SDES_PRIV)
            {
              ip->length_ = buffer[i];
              ++i;
              ip->data_.reset (new (std::nothrow) char[ip->length_ + 1] {});
              if (ip->data_ == nullptr)
                {
                  errno = ENOMEM;
                  return;
                }
              std::memcpy (ip->data_.get (), &buffer[i], ip->length_);
              i += ip->length_;
            }
          else
            {
              ip->length_ = buffer[i];
              ++i;
              ip->data_length_ = buffer[i];
              ++i;
              ip->data_.reset (new (std::nothrow) char[ip->length_ + 1] {});
              if (ip->data_ == nullptr)
                {
                  errno = ENOMEM;
                  return;
                }
              std::memcpy (ip->data_.get (), &buffer[i], ip->length_);
              i += ip->length_;
              ip->priv_data_.reset (
                new (std::nothrow) char[ip->data_length_ + 1] {});
              if (ip->priv_data_ == nullptr)
                {
                  errno = ENOMEM;
                  return;
                }
              std::memcpy (ip->priv_data_.get (), &buffer[i], ip->data_length_);
              i += ip->data_length_;
            }
        }
        ++i; // each chunk ends with a zero (END) item
        // each chunk must end on an even 32 bit boundary
        while (i%4) i++;
    }

  *len-=(this->chd_.length_+1)*4;

}

//==============================================================================

RTCP_SDES_Packet::~RTCP_SDES_Packet() = default;

//==============================================================================

sdesChunk_t *
RTCP_SDES_Packet::add_chunk (ACE_UINT32 ssrc)
{
  std::unique_ptr<sdesChunk_t> chunk (
    new (std::nothrow) sdesChunk_t {});
  if (chunk == nullptr)
    {
      errno = ENOMEM;
      return nullptr;
    }

  sdesChunk_t * const cp = chunk.get ();
  cp->ssrc_ = ssrc;
  if (this->chunk_ == nullptr)
    this->chunk_ = std::move (chunk);
  else
    {
      sdesChunk_t *tail = this->chunk_.get ();
      while (tail->next_)
        tail = tail->next_.get ();
      tail->next_ = std::move (chunk);
    }

  ++this->num_chunks_;
  ++this->chd_.count_;
  return cp;
}

sdesItem_t *
RTCP_SDES_Packet::add_item_node (sdesChunk_t *chunk)
{
  std::unique_ptr<sdesItem_t> item (
    new (std::nothrow) sdesItem_t {});
  if (item == nullptr)
    {
      errno = ENOMEM;
      return nullptr;
    }

  sdesItem_t * const item_ptr = item.get ();
  std::unique_ptr<sdesItem_t> *next_item = &chunk->item_;
  while (*next_item != nullptr)
    next_item = &(*next_item)->next_;
  *next_item = std::move (item);
  return item_ptr;
}

//==============================================================================

void
RTCP_SDES_Packet::add_item (ACE_UINT32 ssrc,
                            unsigned char type,
                            unsigned char length,
                            char const *data)
{
  sdesChunk_t *cp = this->chunk_.get ();

  if (cp == nullptr)
    {
      cp = this->add_chunk (ssrc);
      if (cp == nullptr)
        return;
    }
  else
    {
      while (cp->ssrc_ != ssrc && cp->next_ != nullptr)
        cp = cp->next_.get ();
      if (cp->ssrc_ != ssrc)
        {
          cp = this->add_chunk (ssrc);
          if (cp == nullptr)
            return;
        }
    }

  std::unique_ptr<char[]> item_data (
    new (std::nothrow) char[length + 1] {});
  if (item_data == nullptr)
    {
      errno = ENOMEM;
      return;
    }
  std::memcpy (item_data.get (), data, length);

  sdesItem_t * const ip = this->add_item_node (cp);
  if (ip == nullptr)
    return;

  ip->type_ = type;
  ip->length_ = length;
  ip->data_ = std::move (item_data);
}

//==============================================================================

void
RTCP_SDES_Packet::add_priv_item (ACE_UINT32 ssrc,
                                 unsigned char nameLength,
                                 char const *name,
                                 unsigned char dataLength,
                                 char const *data)
{
  sdesChunk_t *cp = this->chunk_.get ();

  if (cp == nullptr)
    {
      cp = this->add_chunk (ssrc);
      if (cp == nullptr)
        return;
    }
  else
    {
      while (cp->ssrc_ != ssrc && cp->next_ != nullptr)
        cp = cp->next_.get ();
      if (cp->ssrc_ != ssrc)
        {
          cp = this->add_chunk (ssrc);
          if (cp == nullptr)
            return;
        }
    }

  std::unique_ptr<char[]> item_name (
    new (std::nothrow) char[nameLength + 1] {});
  std::unique_ptr<char[]> item_data (
    new (std::nothrow) char[dataLength + 1] {});
  if (item_name == nullptr || item_data == nullptr)
    {
      errno = ENOMEM;
      return;
    }
  std::memcpy (item_name.get (), name, nameLength);
  std::memcpy (item_data.get (), data, dataLength);

  sdesItem_t * const ip = this->add_item_node (cp);
  if (ip == nullptr)
    return;

  ip->type_ = RTCP_SDES_PRIV;
  ip->length_ = nameLength;
  ip->data_length_ = dataLength;
  ip->data_ = std::move (item_name);
  ip->priv_data_ = std::move (item_data);
}

//==============================================================================

unsigned int
RTCP_SDES_Packet::packet_size()
{
  int size;
  sdesChunk_t *cp; // pointer to chunk
  sdesItem_t *ip; // pointer to item

  // Determine the size of the packet.
  size = 4;  // size of common header data in octets

  cp = this->chunk_.get ();
  while (cp)
    {
      size += 4; // size of ssrc
      ip = cp->item_.get ();

      while (ip && (ip->type_ != 0))
        {
          if (ip->type_ != RTCP_SDES_PRIV)
            {
              size += 2 + ip->length_;  // size of item
            }
          else
            {
              size += 3 + ip->length_ + ip->data_length_;
            }
          ip = ip->next_.get ();
        }
      size += 4 - size%4;           // pad with zeros to even 32 bit bound
      cp = cp->next_.get ();
    }

  chd_.length_ = static_cast<ACE_UINT16> (size/4 - 1);

  return size;
}

//==============================================================================

void
RTCP_SDES_Packet::build_packet()
{
  sdesChunk_t *cp; // pointer to chunk
  sdesItem_t *ip; // pointer to item
  int index, i;

  this->packet_data_.reset (new (std::nothrow) char[this->packet_size ()]);
  if (!this->packet_data_)
    {
      errno = ENOMEM;
      return;
    }

  index = 0;
  this->packet_data_[index] = static_cast<char> ((chd_.ver_ << 6) |
                                                 (chd_.pad_ << 5) |
                                                  chd_.count_);
  ++index;
  this->packet_data_[index] = chd_.pt_;
  ++index;
  *((ACE_UINT16*)&this->packet_data_[index]) = ACE_HTONS(chd_.length_);
  index+=2;

  cp = this->chunk_.get ();
  while (cp)
    {
      *((ACE_UINT32*)&this->packet_data_[index]) = ACE_HTONL(cp->ssrc_);
      index+=4;

      ip = cp->item_.get ();
      while (ip && (ip->type_ != 0))
        {
          this->packet_data_[index] = ip->type_;
          ++index;
          if (ip->type_ != RTCP_SDES_PRIV)
            {
              this->packet_data_[index] = ip->length_;
              ++index;
              for (i=0; i<ip->length_; ++i)
                {
                  this->packet_data_[index] = ip->data_[i];
                  ++index;
                }
            }
          else
            {
              this->packet_data_[index] = ip->length_;
              ++index;
              this->packet_data_[index] = ip->data_length_;
              ++index;
              for (i=0; i<ip->length_; ++i)
                {
                  this->packet_data_[index] = ip->data_[i];
                  ++index;
                }
              for (i=0; i<ip->data_length_; ++i)
                {
                  this->packet_data_[index] = ip->priv_data_[i];
                  ++index;
                }
            }

          ip = ip->next_.get ();
        }

      this->packet_data_[index] = 0;
      ++index;

      i=1;
      while ((index)%4)
        { // pad chunk with zeros to 32 bit bound
          this->packet_data_[index] = 0;
          ++index;
          ++i;
        }

      // Store the number of bytes added. TODO: do we need this
      // this->packet_data_[index - 1] = i;

      cp = cp->next_.get ();
    }
}

void
RTCP_SDES_Packet::dump ()
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
  ip = this->chunk_->item_.get ();

  while (ip)
    {
      // If there is no data to store, continue.
      if (ip->type_ != RTCP_SDES_PRIV && ip->length_ == 0)
        {
          ip = ip->next_.get ();
          continue;
        }

      switch (ip->type_)
      {
        case RTCP_SDES_END:
             break;
        case RTCP_SDES_CNAME:
             ORBSVCS_DEBUG ((LM_DEBUG,
                         "    CNAME '%s'\n",
                         ip->data_.get ()));
             break;
        case RTCP_SDES_NAME:
             ORBSVCS_DEBUG ((LM_DEBUG,
                         "    NAME '%s'\n",
                         ip->data_.get ()));
             break;
        case RTCP_SDES_EMAIL:
             ORBSVCS_DEBUG ((LM_DEBUG,
                         "    EMAIL '%s'\n",
                         ip->data_.get ()));
             break;
        case RTCP_SDES_PHONE:
             ORBSVCS_DEBUG ((LM_DEBUG,
                         "    PHONE '%s'\n",
                         ip->data_.get ()));
             break;
        case RTCP_SDES_LOC:
             ORBSVCS_DEBUG ((LM_DEBUG,
                         "    LOC '%s'\n",
                         ip->data_.get ()));
             break;
        case RTCP_SDES_TOOL:
             ORBSVCS_DEBUG ((LM_DEBUG,
                         "    TOOL '%s'\n",
                         ip->data_.get ()));
             break;
        case RTCP_SDES_NOTE:
             ORBSVCS_DEBUG ((LM_DEBUG,
                         "    NOTE '%s'\n",
                         ip->data_.get ()));
             break;
        case RTCP_SDES_PRIV:
             ORBSVCS_DEBUG ((LM_DEBUG,
                         "    '%s' '%s'\n",
                         ip->data_.get (),
                         ip->priv_data_.get ()));
             break;
      }
      ip = ip->next_.get ();
    }

}

RTCP_SR_Packet::RTCP_SR_Packet(ACE_UINT32 ssrc,
                               ACE_UINT32 ntp_ts_msw,
                               ACE_UINT32 ntp_ts_lsw,
                               ACE_UINT32 timestamp,
                               ACE_UINT32 packets_sent,
                               ACE_UINT32 octets_sent,
                               std::unique_ptr<RR_Block> rr_blocks)
{
  RR_Block *block_ptr = rr_blocks.get ();
  chd_.count_ = 0;
  chd_.ver_ = 2;
  chd_.pt_ = RTCP_PT_SR;

  this->ssrc_ = ssrc;
  this->ntp_ts_msw_ = ntp_ts_msw;
  this->ntp_ts_lsw_ = ntp_ts_lsw;
  this->rtp_ts_ = timestamp;
  this->psent_ = packets_sent;
  this->osent_ = octets_sent;
  this->rr_ = std::move (rr_blocks);

  while (block_ptr)
    {
      ++chd_.count_;

      // Can only have 31 receiver reports
      if (this->chd_.count_ == 31)
        {
          block_ptr->next_.reset ();
          break;
        }

      block_ptr = block_ptr->next_.get ();
    }

  this->chd_.length_ = static_cast<ACE_UINT16> (6 + 6*(chd_.count_)); //+profile specific extensions ??
}

//==============================================================================

RTCP_SR_Packet::RTCP_SR_Packet (char* buffer,
                                int *len)
                : RTCP_Packet (buffer)
{
  unsigned int i = 0;
  std::unique_ptr<RR_Block> *next_block = &this->rr_;

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
  for (unsigned int j {}; j < this->chd_.count_; ++j)
    {
      *next_block = std::unique_ptr<RR_Block> (
        new (std::nothrow) RR_Block {});
      if (*next_block == nullptr)
        {
          errno = ENOMEM;
          return;
        }

      RR_Block * const local_block_ptr = next_block->get ();
      next_block = &local_block_ptr->next_;
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
}

//==============================================================================

RTCP_SR_Packet::~RTCP_SR_Packet() = default;

//==============================================================================

unsigned int RTCP_SR_Packet::packet_size ()
{
  ACE_UINT16 size  = static_cast<ACE_UINT16> ((2+chd_.count_*6) * 4); // + profile specific extensions ?
  size += 20; // the first line is the same as RR; 20 more bytes for SR

  return size;
}

//==============================================================================

void RTCP_SR_Packet::build_packet()
{
  int index = 0;
  RR_Block *local_block_ptr;

  this->packet_data_.reset (new (std::nothrow) char[this->packet_size ()]);
  if (!this->packet_data_)
    {
      errno = ENOMEM;
      return;
    }

  this->packet_data_[index] = static_cast<char> ((this->chd_.ver_ << 6) |
                                                 (this->chd_.pad_ << 5) |
                                                  this->chd_.count_);
  ++index;
  this->packet_data_[index] = this->chd_.pt_;
  ++index;
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

  local_block_ptr = this->rr_.get ();
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
      local_block_ptr = local_block_ptr->next_.get ();
    }
}

void
RTCP_SR_Packet::dump ()
{
  RR_Block const *b = this->rr_.get ();
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

      b = b->next_.get ();
      ++count;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
