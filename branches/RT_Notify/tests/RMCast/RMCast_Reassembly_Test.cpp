// $Id$

// ============================================================================
//
// = DESCRIPTION
//    Unit test for the reassembly module of the RMCast library.
//
// = AUTHORS
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#include "../test_config.h"
#include "ace/Task.h"
#include "ace/RMCast/RMCast_Reassembly.h"

ACE_RCSID(tests, RMCast_Reassembly_Test, "$Id$")

class ACE_RMCast_Reassembly_Tester
  : public ACE_Task_Base
  , public ACE_RMCast_Module
{
public:
  ACE_RMCast_Reassembly_Tester (void);

  virtual int data (ACE_RMCast::Data &data);
  virtual int svc (void);

private:
  void initialize (ACE_Message_Block *mb);
  // Initialize the message block with zero data

  int compare (ACE_Message_Block *received,
               ACE_Message_Block *original);
  // Compare the message block to <received_>

  int put_fragment (ACE_UINT32 sequence_number,
                    ACE_UINT32 offset,
                    ACE_UINT32 fragment_size,
                    ACE_Message_Block *mb);
  // Put one fragment out

  ACE_UINT32 next_sequence_number (void);
  // Return the next sequence number..

private:
  ACE_RMCast_Reassembly reassembly_;

  ACE_SYNCH_MUTEX mutex_;
  ACE_UINT32 message_sequence_number_;
};

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("RMCast_Reassembly_Test"));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This is ACE Version %u.%u.%u\n\n"),
              ACE::major_version(),
              ACE::minor_version(),
              ACE::beta_version()));

  ACE_RMCast_Reassembly_Tester tester;

  if (tester.activate (THR_NEW_LWP|THR_JOINABLE, 4) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot activate the threads\n")),
                      1);

  ACE_Thread_Manager::instance ()->wait ();

  ACE_END_TEST;
  return 0;
}

// ****************************************************************

ACE_RMCast_Reassembly_Tester::ACE_RMCast_Reassembly_Tester (void)
  : message_sequence_number_ (0)
{
  this->reassembly_.next (this);
}

int
ACE_RMCast_Reassembly_Tester::svc (void)
{
  for (int iteration = 0; iteration != 50; ++iteration)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) iteration %d\n"), iteration));
      ACE_UINT32 sequence_number = this->next_sequence_number ();
      {
        ACE_Message_Block received;

        const size_t fragment_size = 128;
        ACE_UINT32 n = 32 * fragment_size;
        ACE_Message_Block big_blob (n);
        big_blob.wr_ptr (n);

        this->initialize (&big_blob);

        // Use an ACT to store the results in <received>
        ACE_Message_Block *received_pointer = &received;
        ACE_OS::memcpy (big_blob.rd_ptr (),
                        &received_pointer,
                        sizeof(received_pointer));

        for (size_t offset = 0; offset < n; offset += fragment_size)
          {
            if (this->put_fragment (sequence_number,
                                    offset,
                                    fragment_size,
                                    &big_blob) == -1)
              {
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("Error in put_fragment\n")));
                return -1;
              }
          }

        if (this->compare (&received, &big_blob) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Mismatched big_blob data\n")),
                              -1);
          }
      }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) iteration %d, first test passed\n"),
                  iteration));
      sequence_number = this->next_sequence_number ();
      {
        ACE_Message_Block received;

        const size_t fragment_size = 128;
        ACE_UINT32 n = 32 * fragment_size;
        ACE_Message_Block big_blob (n);
        big_blob.wr_ptr (n);

        this->initialize (&big_blob);

        // Use an ACT to store the results in <received>
        ACE_Message_Block *received_pointer = &received;
        ACE_OS::memcpy (big_blob.rd_ptr (),
                        &received_pointer,
                        sizeof(received_pointer));

        ACE_RANDR_TYPE seed =
          ACE_static_cast(ACE_RANDR_TYPE, ACE_OS::time (0));
        for (int i = 0; i != 100; ++i)
          {
            size_t offset = ACE_OS::rand_r (seed) % n;
            if (offset >= n)
              {
                offset = n/2;
              }
            if (this->put_fragment (sequence_number,
                                    offset,
                                    fragment_size,
                                    &big_blob) == -1)
              {
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("Error in put_fragment\n")));
                return -1;
              }
          }

        for (size_t offset = 0; offset < n; offset += fragment_size)
          {
            if (this->put_fragment (sequence_number,
                                    offset,
                                    fragment_size,
                                    &big_blob) == -1)
              {
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("Error in put_fragment\n")));
                return -1;
              }
          }

        if (this->compare (&received, &big_blob) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Mismatched random big_blob data\n")),
                              -1);
          }
      }
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) iteration %d, random test passed\n"),
                  iteration));
    }

  return 0;
}

void
ACE_RMCast_Reassembly_Tester::initialize (ACE_Message_Block *mb)
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
ACE_RMCast_Reassembly_Tester::compare (ACE_Message_Block *received,
                                       ACE_Message_Block *original)
{
  size_t n = original->total_size ();
  ACE_Message_Block blob (n);

  for (const ACE_Message_Block *i = original; i != 0; i = i->cont ())
    {
      blob.copy (i->rd_ptr (), i->length ());
    }

  if (received->rd_ptr () == 0)
    ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT ("INCOMPLETE MESSAGE\n")), -1);

  if (ACE_OS::memcmp (blob.rd_ptr (),
                      received->rd_ptr (),
                      n) != 0)
    {
      for (size_t i = 0; i < n; i += 256)
        {
          size_t z = 256;
          if (n - i < 256)
            z = n - i;
          ACE_HEX_DUMP ((LM_DEBUG,
                         blob.rd_ptr () + i,
                         z,
                         ACE_TEXT ("BIG BLOB")));
        }
      for (size_t j = 0; j < n; j += 256)
        {
          size_t z = 256;
          if (n - j < 256)
            z = n - j;
          ACE_HEX_DUMP ((LM_DEBUG,
                         received->rd_ptr () + j,
                         z,
                         ACE_TEXT ("RECEIVED")));
        }
      return -1;
    }
  return 0;
}

int
ACE_RMCast_Reassembly_Tester::data (ACE_RMCast::Data &data)
{
  ACE_Message_Block *mb = data.payload;

  ACE_Message_Block *pointer;
  ACE_OS::memcpy (&pointer, mb->rd_ptr (), sizeof(pointer));

  size_t l = mb->length ();
  pointer->size (l);
  pointer->wr_ptr (pointer->rd_ptr () + l);
  ACE_OS::memcpy (pointer->rd_ptr (), mb->rd_ptr (), l);
  return 0;
}

int
ACE_RMCast_Reassembly_Tester::put_fragment (ACE_UINT32 sequence_number,
                                            ACE_UINT32 offset,
                                            ACE_UINT32 fragment_size,
                                            ACE_Message_Block *mb)
{
  size_t total_length = mb->total_length ();

  size_t size = fragment_size;
  if (offset + size >= total_length)
    {
      size = total_length - offset;
    }
  ACE_Message_Block p (mb->rd_ptr () + offset, size);

  p.wr_ptr (size);

  ACE_RMCast::Data data;
  data.sequence_number = sequence_number;
  data.total_size = total_length;
  data.fragment_offset = offset;
  data.payload = &p;
  return this->reassembly_.data (data);
}

ACE_UINT32
ACE_RMCast_Reassembly_Tester::next_sequence_number ()
{
  ACE_UINT32 r;
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
    r = this->message_sequence_number_++;
  }
  return r;
}
