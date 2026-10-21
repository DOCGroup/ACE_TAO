#include "testC.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      test::event0_init *ev0_fac = nullptr;
      ACE_NEW_RETURN (ev0_fac,
                      test::event0_init,
                      -1);
      CORBA::ValueFactoryBase_var ev0_fac_owner (ev0_fac);

      test::event1_init *ev1_fac = nullptr;
      ACE_NEW_RETURN (ev1_fac,
                      test::event1_init,
                      -1);
      CORBA::ValueFactoryBase_var ev1_fac_owner (ev1_fac);

      orb->register_value_factory (ev0_fac->tao_repository_id (), ev0_fac);
      orb->register_value_factory (ev1_fac->tao_repository_id (), ev1_fac);

      test::event0_var ev0;
      ACE_NEW_RETURN (ev0,
                      OBV_test::event0,
                      -1);
      ev0->message ("one");

      // write ev0 to CDR
      TAO_OutputCDR cdr (static_cast<size_t> (0), // size
                         static_cast<int> (ACE_CDR_BYTE_ORDER),
                         nullptr,                 // buffer_allocator
                         nullptr,                 // data_block_allocator
                         nullptr,                 // message_block_allocator
                         0,                     // memcpy_tradeoff
                         1,
                         1);
      cdr.reset ();

      if (!(cdr << ev0.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "ERROR: cdr << ev0\n"),
                            -1);
        }

      // convert cdr into safe_octet_seq
      CORBA::OctetSeq safe_octet_seq (cdr.total_length ());
      safe_octet_seq.length (cdr.total_length ());

      CORBA::Octet *buffer = safe_octet_seq.get_buffer ();
      for (ACE_Message_Block const *i = cdr.begin ();
           i != nullptr;
           i = i->cont ())
        {
          CORBA::ULong len = i->length ();
          ACE_OS::memcpy (buffer, i->rd_ptr (), static_cast<size_t> (len));
          buffer += len;
        }

      // Use TAO_InputCDR for unmarshalling
      buffer = safe_octet_seq.get_buffer ();
      size_t char_len = safe_octet_seq.length () * sizeof (CORBA::Octet);
      TAO_InputCDR input (reinterpret_cast<char const *> (buffer),
                          char_len,
                          ACE_CDR_BYTE_ORDER,
                          TAO_DEF_GIOP_MAJOR,
                          TAO_DEF_GIOP_MINOR,
                          orb->orb_core ());

      // Read ev1 from CDR, this should fail (we actually encoded a ev0),
      // but *NOT* leak memory.
      test::event1_var ev1;
      if (input >> ev1.out ())
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "ERROR: unmarshal shouldn't work\n"),
                            -1);
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Test passed.\n"));

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
                           "Caught exception in server:");
      return 1;
    }

  return 0;
}
