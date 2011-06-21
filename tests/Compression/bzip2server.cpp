// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/ORB.h"
#include "tao/Compression/Compression.h"
#include "tao/Compression/bzip2/Bzip2Compressor_Factory.h"

bool
test_invalid_compression_factory (Compression::CompressionManager_ptr cm)
{
  bool succeed = false;
  try
    {
      // Get an invalid compression factory
      Compression::CompressorFactory_var factory =
        cm->get_factory (100);
    }
  catch (const Compression::UnknownCompressorId& ex)
    {
      ACE_UNUSED_ARG (ex);
      succeed = true;
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, get invalid compression factory failed\n"));
  }

  return succeed;
}

bool
test_duplicate_compression_factory (
  Compression::CompressionManager_ptr cm,
  Compression::CompressorFactory_ptr cf)
{
  bool succeed = false;
  try
    {
      // Register duplicate
      cm->register_factory (cf);
    }
  catch (const Compression::FactoryAlreadyRegistered&)
    {
      succeed = true;
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, register duplicate factory failed\n"));
  }

  return succeed;
}

bool
test_register_nil_compression_factory (
  Compression::CompressionManager_ptr cm)
{
  bool succeed = false;
  try
    {
      // Register nil factory
      cm->register_factory (Compression::CompressorFactory::_nil());
    }
  catch (const CORBA::BAD_PARAM& ex)
    {
      if ((ex.minor() & 0xFFFU) == 44)
        {
          succeed = true;
        }
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, register nill factory failed\n"));
  }

  return succeed;
}

bool
test_compression (CORBA::ULong nelements,
              Compression::CompressionManager_ptr cm)
{
  bool succeed = false;

  CORBA::OctetSeq mytest;
  mytest.length (nelements);
  for (CORBA::ULong j = 0; j != nelements; ++j)
    {
      mytest[j] = 'a';
    }

  Compression::Compressor_var compressor = cm->get_compressor (
    ::Compression::COMPRESSORID_BZIP2, 6);

  CORBA::OctetSeq myout;
  myout.length ((CORBA::ULong)(mytest.length() * 1.1));

  compressor->compress (mytest, myout);

  CORBA::OctetSeq decompress;
  decompress.length (nelements);

  compressor->decompress (myout, decompress);

  if (decompress != mytest)
    {
      ACE_ERROR ((LM_ERROR, "Error, decompress not working\n"));
    }
  else
    {
      succeed = true;
      ACE_DEBUG ((LM_DEBUG, "Compression worked with bzip2, original "
                            "size %d, compressed size %d\n",
                            mytest.length(), myout.length ()));
    }
  return succeed;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int retval = 0;
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var compression_manager =
        orb->resolve_initial_references("CompressionManager");

      Compression::CompressionManager_var manager =
        Compression::CompressionManager::_narrow (compression_manager.in ());

      if (CORBA::is_nil(manager.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil compression manager\n"),
                          1);

      Compression::CompressorFactory_ptr compressor_factory;

      ACE_NEW_RETURN (compressor_factory, TAO::Bzip2_CompressorFactory (), 1);

      Compression::CompressorFactory_var compr_fact = compressor_factory;
      manager->register_factory(compr_fact.in ());

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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      retval = 1;
    }

  return retval;
}
