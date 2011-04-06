
//=============================================================================
/**
 *  @file   alignment.cpp
 *
 *  $Id$
 *
 * Verifies that octet sequence marshaling does not affect
 * marshaling.
 *
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#include "tao/CDR.h"

#include "ace/Log_Msg.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  int status = 0;

  for (CORBA::ULong i = 16; i != 64; ++i)
    {
      ACE_Message_Block mb (i + ACE_CDR::MAX_ALIGNMENT);
      ACE_CDR::mb_align (&mb);
      mb.wr_ptr (i);

      CORBA::Double dbl = i;

      TAO_OutputCDR cdr;
      cdr.write_ulong (i); // length
      cdr.write_octet_array_mb (&mb);
      cdr.write_double (dbl);
      cdr.write_double (dbl);

      TAO_InputCDR input (cdr);

      CORBA::ULong len;

      input.read_ulong (len);

      if (len != i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "ERROR: mismatched lengths,"
                      " got %d, expected %d\n",
                      len, i));
        }

      ACE_Message_Block read_mb (len + ACE_CDR::MAX_ALIGNMENT);
      ACE_CDR::mb_align (&mb);
      mb.wr_ptr (len);
      input.read_char_array (mb.rd_ptr (), len);

      CORBA::Double read_dbl;
      if (input.read_double (read_dbl) == 0)
        ACE_DEBUG ((LM_DEBUG, "Failure reading double...\n"));

      if (!ACE::is_equal (read_dbl, dbl))
        {
          status = 1;
          ACE_DEBUG ((LM_DEBUG,
                      "ERROR: mismatched doubles,"
                      " got %f, expected %f\n",
                      read_dbl, dbl));
          for (const ACE_Message_Block *j = cdr.begin ();
               j != cdr.end ();
               j = j->cont ())
            {
              ACE_HEX_DUMP ((LM_DEBUG,
                             j->rd_ptr (),
                             j->length (),
                             ACE_TEXT("Output CDR stream")));
            }
          TAO_InputCDR debug (cdr);
          ACE_HEX_DUMP ((LM_DEBUG,
                         debug.rd_ptr (),
                         debug.length (),
                         ACE_TEXT("Input CDR stream")));
        }
    }

  return status;
}
