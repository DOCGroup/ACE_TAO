// $Id$

// ============================================================================
//
// = DESCRIPTION
//    Unit test for the fragmentation module of the RMCast library.
//
// = AUTHORS
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#include "../test_config.h"
#include "ace/Message_Block.h"
#include "ace/Task.h"
#include "ace/RMCast/RMCast_Fragment.h"

ACE_RCSID(tests, RMCast_Fragment_Test, "$Id$")

class ACE_RMCast_Fragment_Tester
  : public ACE_Task_Base
  , public ACE_RMCast_Module
{
public:
  ACE_RMCast_Fragment_Tester (void);

  virtual int data (ACE_RMCast::Data &data);
  virtual int svc (void);

private:
  void initialize (ACE_Message_Block *mb);
  // Initialize the message block with zero data

  int compare (ACE_Message_Block *mb);
  // Compare the message block to <received_>

private:
  ACE_RMCast_Fragment fragment_;

  ACE_Message_Block received_;
  ACE_UINT32 received_bytes_;
  ACE_UINT32 message_sequence_number_;
};

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("RMCast_Fragment_Test"));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This is ACE Version %u.%u.%u\n\n"),
              ACE::major_version(),
              ACE::minor_version(),
              ACE::beta_version()));

  ACE_RMCast_Fragment_Tester tester;

  if (tester.svc () == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error running the svc() routine\n")));

  ACE_END_TEST;
  return 0;
}

// ****************************************************************

ACE_RMCast_Fragment_Tester::ACE_RMCast_Fragment_Tester (void)
{
  this->fragment_.next (this);
}

int
ACE_RMCast_Fragment_Tester::svc (void)
{
  {
    this->received_.wr_ptr (this->received_.rd_ptr ());
    this->received_bytes_ = 0;
    this->message_sequence_number_ = 0;

    ACE_UINT32 n = 128 * 1024;
    ACE_Message_Block big_blob (n);
    big_blob.wr_ptr (n);

    this->initialize (&big_blob);

    ACE_RMCast::Data data;
    data.payload = &big_blob;
    if (this->fragment_.data (data) == -1)
      return -1;

    if (this->received_bytes_ != n)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Unexpected number of ")
                         ACE_TEXT ("received bytes (%d/%d)\n"),
                         this->received_bytes_, n),
                        -1);

    if (this->compare (&big_blob) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Mismatched big_blob data\n")),
                          -1);
      }

  }


  {
    this->received_.wr_ptr (this->received_.rd_ptr ());
    this->received_bytes_ = 0;
    this->message_sequence_number_ = 0;

    const int n = 256;
    const int size = 512;
    ACE_Message_Block smallb[n];
    smallb[0].size (size);
    smallb[0].wr_ptr (size);

    for (int i = 1; i != n; ++i)
      {
        smallb[i].size (size);
        smallb[i].wr_ptr (size);
        smallb[i - 1].cont (&smallb[i]);
      }
    this->initialize (smallb);

    ACE_RMCast::Data data;
    data.payload = smallb;
    if (this->fragment_.data (data) == -1)
      return -1;

    ACE_UINT32 total = n * size;
    if (this->received_bytes_ != total)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Unexpected number of ")
                         ACE_TEXT ("received bytes (%d/%d)\n"),
                         this->received_bytes_, total),
                        -1);

    if (this->compare (smallb) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Mismatched smallb chain data\n")),
                          -1);
      }

  }

  {
    this->received_.wr_ptr (this->received_.rd_ptr ());
    this->received_bytes_ = 0;
    this->message_sequence_number_ = 0;

    const int n = 256;
    ACE_UINT32 total = 0;

    ACE_RANDR_TYPE seed =
      ACE_static_cast(ACE_RANDR_TYPE,ACE_OS::time (0));

    int size = 64 + ACE_OS::rand_r(seed) % 128;
    ACE_Message_Block smallb[n];
    smallb[0].size (size);
    smallb[0].wr_ptr (size);
    total += size;

    for (int i = 1; i != n; ++i)
      {
        size = 64 + ACE_OS::rand_r(seed) % 128;
        total += size;
        smallb[i].size (size);
        smallb[i].wr_ptr (size);
        smallb[i - 1].cont (&smallb[i]);
      }
    this->initialize (smallb);

    ACE_RMCast::Data data;
    data.payload = smallb;
    if (this->fragment_.data (data) == -1)
      return -1;

    if (this->received_bytes_ != total)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Unexpected number of ")
                         ACE_TEXT ("received bytes (%d/%d)\n"),
                         this->received_bytes_, total),
                        -1);

    if (this->compare (smallb) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Mismatched smallb chain data\n")),
                          -1);
      }

  }
  return 0;
}

void
ACE_RMCast_Fragment_Tester::initialize (ACE_Message_Block *mb)
{
  for (ACE_Message_Block *i = mb; i != 0; i = i->cont ())
    {
      char z = 0;
      for (char *j = i->rd_ptr (); j != i->wr_ptr (); ++j)
        {
          *j = ++z;
        }
    }
}

int
ACE_RMCast_Fragment_Tester::compare (ACE_Message_Block *mb)
{
  size_t n = mb->total_size ();
  ACE_Message_Block blob (n);

  for (const ACE_Message_Block *i = mb; i != 0; i = i->cont ())
    {
      blob.copy (i->rd_ptr (), i->length ());
    }

  if (ACE_OS::memcmp (blob.rd_ptr (),
                      this->received_.rd_ptr (),
                      n) != 0)
    {
      for (size_t offset = 0; offset < n; offset += 256)
        {
          size_t z = 256;
          if (n - offset < 256)
            z = n - offset;
          ACE_HEX_DUMP ((LM_DEBUG,
                         blob.rd_ptr () + offset,
                         z,
                         ACE_TEXT ("BIG BLOB")));
          ACE_HEX_DUMP ((LM_DEBUG,
                         this->received_.rd_ptr () + offset,
                         z,
                         ACE_TEXT ("RECEIVED")));
        }
      return -1;
    }
  return 0;
}

int
ACE_RMCast_Fragment_Tester::data (ACE_RMCast::Data &data)
{
  ACE_UINT32 sequence_number = data.sequence_number;
  ACE_UINT32 message_size = data.total_size;
  size_t offset = data.fragment_offset;
  ACE_Message_Block *mb = data.payload;

  if (this->received_bytes_ == 0)
    {
      this->received_.size (message_size);
      this->received_.wr_ptr (message_size);
      this->message_sequence_number_ =  sequence_number;
    }
  else
    {
      if (this->message_sequence_number_ != sequence_number)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Mismatched sequence number\n")),
                          -1);
      if (this->received_.length () != message_size)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Mismatched sequence size\n")),
                          -1);
    }

  size_t payload_size = mb->length ();
  size_t fragment_size = payload_size;
  if (payload_size > 0)
    {
      ACE_OS::memcpy (this->received_.rd_ptr () + offset,
                      mb->rd_ptr (),
                      payload_size);
      this->received_bytes_ += ACE_static_cast (ACE_UINT32, payload_size);
      offset += payload_size;
    }

  for (const ACE_Message_Block *i = mb->cont (); i != 0; i = i->cont ())
    {
      payload_size = i->length ();
      // ACE_DEBUG ((LM_DEBUG,
      //         "offset = %d , payload = %d\n", offset, payload_size));
      fragment_size += payload_size;
      ACE_OS::memcpy (this->received_.rd_ptr () + offset,
                      i->rd_ptr (), payload_size);
      this->received_bytes_ += ACE_static_cast (ACE_UINT32, payload_size);
      offset += payload_size;
    }

  if (fragment_size > this->fragment_.max_fragment_size ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Invalid fragment size\n")),
                      -1);

  return 0;
}
