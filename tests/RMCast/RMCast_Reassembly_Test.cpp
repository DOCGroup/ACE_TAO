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

#include "test_config.h"
#include "ace/RMCast/RMCast_Reassembly.h"

ACE_RCSID(tests, RMCast_Reassembly_Test, "$Id$")

class ACE_RMCast_Reassembly_Tester : public ACE_Task<ACE_MT_SYNCH>
{
public:
  ACE_RMCast_Reassembly_Tester (void);

  virtual int put (ACE_Message_Block *mb,
                   ACE_Time_Value *tv = 0);
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
                    ACE_Message_Block *mb,
                    ACE_Time_Value *tv);
  // Put one fragment out

  ACE_UINT32 next_sequence_number (void);
  // Return the next sequence number..

private:
  ACE_RMCast_Reassembly<ACE_MT_SYNCH> reassembly_;

  ACE_SYNCH_MUTEX mutex_;
  ACE_UINT32 message_sequence_number_;
};

int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("RMCast_Reassembly_Test"));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This is ACE Version %u.%u.%u\n\n"),
              ACE::major_version(),
              ACE::minor_version(),
              ACE::beta_version()));

  ACE_RMCast_Reassembly_Tester tester;

  if (tester.activate (THR_NEW_LWP|THR_JOINABLE, 4) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot activate the threads\n"), 1);

  ACE_Thread_Manager::instance ()->wait ();

  ACE_END_TEST;
  return 0;
}

// ****************************************************************

ACE_RMCast_Reassembly_Tester::ACE_RMCast_Reassembly_Tester (void)
{
  this->reassembly_.next (this);
  this->next (&this->reassembly_);
}

int
ACE_RMCast_Reassembly_Tester::svc (void)
{
  for (int iteration = 0; iteration != 10; ++iteration)
    {
      ACE_UINT32 sequence_number = this->next_sequence_number ();
      {
        ACE_Message_Block received;

        const size_t fragment_size = 128;
        ACE_UINT32 n = 256 * fragment_size;
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
                                    &big_blob,
                                    0) == -1)
              return -1;
          }

        if (this->compare (&received, &big_blob) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Mismatched big_blob data\n"),
                              -1);
            return -1;
          }
      }

      sequence_number = this->next_sequence_number ();
      {
        ACE_Message_Block received;

        const size_t fragment_size = 128;
        ACE_UINT32 n = 256 * fragment_size;
        ACE_Message_Block big_blob (n);
        big_blob.wr_ptr (n);

        this->initialize (&big_blob);

        // Use an ACT to store the results in <received>
        ACE_Message_Block *received_pointer = &received;
        ACE_OS::memcpy (big_blob.rd_ptr (),
                        &received_pointer,
                        sizeof(received_pointer));

        ACE_RANDR_TYPE seed = ACE_static_cast(ACE_RANDR_TYPE,
                                              ACE_OS::gethrtime ());
        for (int i = 0; i != 100; ++i)
          {
            size_t offset = ACE_OS::rand_r (seed) % (n - 16);
            if (this->put_fragment (sequence_number,
                                    offset,
                                    fragment_size,
                                    &big_blob,
                                    0) == -1)
              return -1;
          }

        for (size_t offset = 0; offset < n; offset += fragment_size)
          {
            if (this->put_fragment (sequence_number,
                                    offset,
                                    fragment_size,
                                    &big_blob,
                                    0) == -1)
              return -1;
          }

        if (this->compare (&received, &big_blob) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Mismatched random big_blob data\n"),
                              -1);
            return -1;
          }
      }
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

  if (ACE_OS::memcmp (blob.rd_ptr (),
                      received->rd_ptr (),
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
                         "BIG BLOB"));
          ACE_HEX_DUMP ((LM_DEBUG,
                         received->rd_ptr () + offset,
                         z,
                         "RECEIVED"));
        }
      return -1;
    }
  return 0;
}

int
ACE_RMCast_Reassembly_Tester::put (ACE_Message_Block *mb,
                                   ACE_Time_Value *)
{
  ACE_Message_Block *pointer;
  ACE_OS::memcpy (&pointer, mb->rd_ptr (), sizeof(pointer));

  size_t l = mb->length ();
  pointer->size (l);
  pointer->wr_ptr (pointer->rd_ptr () + l);
  ACE_OS::memcpy (pointer->rd_ptr (), mb->rd_ptr (), mb->length ());
  return 0;
}

int
ACE_RMCast_Reassembly_Tester::put_fragment (ACE_UINT32 sequence_number,
                                            ACE_UINT32 offset,
                                            ACE_UINT32 fragment_size,
                                            ACE_Message_Block *mb,
                                            ACE_Time_Value *tv)
{
  size_t total_length = mb->total_length ();

  ACE_UINT32 header[3];
  header[0] = ACE_HTONL(sequence_number);
  header[1] = ACE_HTONL(offset);
  header[2] = ACE_HTONL(total_length);

  ACE_Message_Block h (ACE_reinterpret_cast(char*,header),
                       sizeof(header));
  h.wr_ptr (sizeof(header));
  size_t size = fragment_size;
  if (total_length - offset < size)
    {
      size = total_length - offset;
    }
  ACE_Message_Block p (mb->rd_ptr () + offset,
                       size);
  p.wr_ptr (size);
  h.cont (&p);

  return this->reassembly_.put (&h);
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_RMCast_Reassembly<ACE_MT_SYNCH>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_RMCast_Reassembly<ACE_MT_SYNCH>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
