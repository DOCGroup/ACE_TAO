// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/ORB.h"
#include "tao/Compression/Compression.h"
#include "tao/Compression/zlib/ZlibCompressor_Factory.h"

ACE_RCSID (Hello,
           server,
           "$Id$")

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var compression_manager =
        orb->resolve_initial_references("CompressionManager");

      Compression::CompressionManager_var manager =
        Compression::CompressionManager::_narrow (compression_manager.in ());

      if (CORBA::is_nil(manager.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil compression manager\n"),
                          1);

      manager->register_factory(new TAO::Zlib_CompressorFactory ());

      CORBA::ULong const nelements = 1024;
      CORBA::OctetSeq mytest;
      mytest.length (1024);
      for (CORBA::ULong j = 0; j != nelements; ++j)
        {
          mytest[j] = 'a';
        }

      Compression::Compressor_var compressor = manager->get_compressor (4, 6);

      CORBA::OctetSeq myout;
      myout.length (1300);

      compressor->compress (mytest, myout);

      CORBA::OctetSeq decompress;
      decompress.length (1024);

      compressor->decompress (myout, decompress);

      if (decompress != mytest)
        {
          ACE_ERROR ((LM_ERROR, "Error, decompress not working\n"));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Compression worked, original size %d, compressed size %d\n", mytest.length(), myout.length ()));
        }
      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
