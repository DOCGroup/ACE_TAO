#include "ace/Log_Msg.h"
#include "tao/ORB.h"
#include "tao/Compression/Compression.h"
#include "tao/Compression/zlib/ZlibCompressor_Factory.h"

bool
test_invalid_compression_factory (Compression::CompressionManager_ptr const cm)
{
  bool succeed {};
  try
    {
      // Get an invalid compression factory
      Compression::CompressorFactory_var factory = cm->get_factory (100);
    }
  catch (Compression::UnknownCompressorId const&)
    {
      succeed = true;
    }
  catch (CORBA::Exception const&)
    {
    }

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%t) ERROR, get invalid compression factory failed\n")));
  }

  return succeed;
}


bool
test_duplicate_compression_factory (
  Compression::CompressionManager_ptr const cm,
  Compression::CompressorFactory_ptr const cf)
{
  bool succeed {};
  try
    {
      // Register duplicate
      cm->register_factory (cf);
    }
  catch (Compression::FactoryAlreadyRegistered const&)
    {
      succeed = true;
    }
  catch (CORBA::Exception const&)
    {
    }

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%t) ERROR, register duplicate factory failed\n")));
  }

  return succeed;
}

bool
test_register_nil_compression_factory (
  Compression::CompressionManager_ptr const cm)
{
  bool succeed {};
  try
    {
      // Register nil factory
      cm->register_factory (Compression::CompressorFactory::_nil ());
    }
  catch (CORBA::BAD_PARAM const& ex)
    {
      if ((ex.minor () & 0xFFFU) == 44)
        {
          succeed = true;
        }
    }
  catch (CORBA::Exception const&)
    {
    }

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%t) ERROR, register nill factory failed\n")));
  }

  return succeed;
}

bool
test_compression (CORBA::ULong const nelements,
                  Compression::CompressionManager_ptr const cm)
{
  bool succeed {};

  CORBA::OctetSeq mytest;
  mytest.length (nelements);
  for (CORBA::ULong j = 0; j != nelements; ++j)
    {
      mytest[j] = 'a';
    }

  Compression::Compressor_var const compressor = cm->get_compressor (
    ::Compression::COMPRESSORID_ZLIB, 6);

  CORBA::OctetSeq myout;
  myout.length (static_cast<CORBA::ULong> (mytest.length () * 1.1));

  compressor->compress (mytest, myout);

  CORBA::OctetSeq decompress;
  decompress.length (nelements);

  compressor->decompress (myout, decompress);

  if (decompress != mytest)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, decompress not working\n")));
    }
  else
    {
      succeed = true;
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Compression worked with zlib, original "
                                      "size %d, compressed size %d\n"),
                  mytest.length (), myout.length ()));
    }
  return succeed;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int retval {};
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const compression_manager =
        orb->resolve_initial_references ("CompressionManager");

      Compression::CompressionManager_var const manager =
        Compression::CompressionManager::_narrow (compression_manager.in ());

      if (CORBA::is_nil (manager.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Panic: nil compression manager\n")),
                          1);

      Compression::CompressorFactory_var const compr_fact =
        new TAO::Zlib_CompressorFactory ();
      manager->register_factory (compr_fact.in ());

      if (!test_duplicate_compression_factory (manager.in (), compr_fact.in ()))
        retval = 1;

      if (!test_register_nil_compression_factory (manager.in ()))
        retval = 1;

      if (!test_compression (1024, manager.in ()))
        retval = 1;

      if (!test_compression (5, manager.in ()))
        retval = 1;


      if (!test_invalid_compression_factory (manager.in ()))
        retval = 1;

      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      retval = 1;
    }

  return retval;
}
