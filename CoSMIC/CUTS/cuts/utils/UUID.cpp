// $Id$

#include "cuts/utils/UUID.h"

#if !defined (__CUTS_INLINE__)
#include "cuts/utils/UUID.inl"
#endif

#include "ace/OS_NS_string.h"
#include <sstream>

//
// CUTS_UUID
//
CUTS_UUID::CUTS_UUID (void)
{
  this->init ();
}

//
// CUTS_UUID
//
CUTS_UUID::CUTS_UUID (const char * uuid)
{
  unsigned int low;
  unsigned int mid;
  unsigned int hi_and_version;
  unsigned int seq_high_and_res;
  unsigned int seq_low;
  unsigned int node [6];

  const int scanned =
    ::sscanf (uuid,
              "%8x-%4x-%4x-%2x%2x-%2x%2x%2x%2x%2x%2x",
              &low,
              &mid,
              &hi_and_version,
              &seq_high_and_res,
              &seq_low,
              &node[0],
              &node[1],
              &node[2],
              &node[3],
              &node[4],
              &node[5]);

  if (scanned == 11)
  {
    this->uuid_.fields_.time_low_ = static_cast <ACE_UINT32> (low);
    this->uuid_.fields_.time_mid_ = static_cast <ACE_UINT16> (mid);
    this->uuid_.fields_.time_hi_and_version_ =
      static_cast <ACE_UINT16> (hi_and_version);

    this->uuid_.fields_.clk_seq_hi_res_ =
      static_cast <ACE_UINT8> (seq_high_and_res);
    this->uuid_.fields_.clk_seq_low_ =
      static_cast <ACE_UINT8> (seq_low);

    for (size_t i = 0; i < 6; i ++)
    {
      this->uuid_.fields_.node_[i] =
        static_cast <ACE_UINT8> (node[i]);
    }
  }
  else
  {
    this->init ();
  }
}

//
// CUTS_UUID
//
CUTS_UUID::CUTS_UUID (const CUTS_UUID & uuid)
{
  this->uuid_.words_.msw0_ = uuid.uuid_.words_.msw0_;
  this->uuid_.words_.msw1_ = uuid.uuid_.words_.msw1_;
  this->uuid_.words_.msw2_ = uuid.uuid_.words_.msw2_;
  this->uuid_.words_.msw3_ = uuid.uuid_.words_.msw3_;
}

//
// ~CUTS_UUID
//
CUTS_UUID::~CUTS_UUID (void)
{

}

//
// operator =
//
const CUTS_UUID & CUTS_UUID::operator = (const CUTS_UUID & uuid)
{
  this->uuid_.words_.msw0_ = uuid.uuid_.words_.msw0_;
  this->uuid_.words_.msw1_ = uuid.uuid_.words_.msw1_;
  this->uuid_.words_.msw2_ = uuid.uuid_.words_.msw2_;
  this->uuid_.words_.msw3_ = uuid.uuid_.words_.msw3_;

  return *this;
}

//
// init
//
void CUTS_UUID::init (void)
{
  this->uuid_.words_.msw0_ = 0;
  this->uuid_.words_.msw1_ = 0;
  this->uuid_.words_.msw2_ = 0;
  this->uuid_.words_.msw3_ = 0;
}

//
// operator =
//
bool CUTS_UUID::operator == (const CUTS_UUID & uuid) const
{
  return
    (this->uuid_.words_.msw0_ == uuid.uuid_.words_.msw0_) &&
    (this->uuid_.words_.msw1_ == uuid.uuid_.words_.msw1_) &&
    (this->uuid_.words_.msw2_ == uuid.uuid_.words_.msw2_) &&
    (this->uuid_.words_.msw3_ == uuid.uuid_.words_.msw3_);
}

//
// to_string
//
bool CUTS_UUID::to_string (char * str, size_t n) const
{
  //std::ostringstream ostr (str, n);
  //ostr.flags (std::ios::hex | std::ios::uppercase);

  //ostr
  //  << this->uuid_.fields_.time_low_
  //  << "-"
  //  << this->uuid_.fields_.time_mid_
  //  << "-"
  //  << this->uuid_.fields_.time_hi_and_version_
  //  << "-"
  //  << (int)this->uuid_.fields_.clk_seq_hi_res_
  //  << (int)this->uuid_.fields_.clk_seq_low_
  //  << "-"
  //  << (int)this->uuid_.fields_.node_[0]
  //  << (int)this->uuid_.fields_.node_[1]
  //  << (int)this->uuid_.fields_.node_[2]
  //  << (int)this->uuid_.fields_.node_[3]
  //  << (int)this->uuid_.fields_.node_[4]
  //  << (int)this->uuid_.fields_.node_[5]
  //  << std::ends;

  return true;

  ACE_UNUSED_ARG (str);
  ACE_UNUSED_ARG (n);
  //return ostr.good ();
}

//
// to_binary
//
bool CUTS_UUID::to_binary (char * buffer, size_t n) const
{
  if (n < 16)
  {
    return false;
  }

  std::copy (
    this->uuid_.binary_.data_,
    this->uuid_.binary_.data_ + 16,
    buffer);
  return true;
}
