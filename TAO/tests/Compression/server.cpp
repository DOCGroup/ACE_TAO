// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/ORB.h"
#include "tao/Compression/Compression.h"
#include "tao/Compression/zlib/ZlibCompressor_Factory.h"

ACE_RCSID (Hello,
           server,
           "$Id$")

bool
test_invalid_compression_factory (Compression::CompressionManager_ptr cm)
{
  bool succeed = false;
  ACE_TRY_NEW_ENV
    {
      // Get an invalid compression factory
      Compression::CompressorFactory_var factory =
        cm->get_factory (100);
    }
  ACE_CATCH (Compression::UnknownCompressorId, ex)
    {
      ACE_UNUSED_ARG (ex);
      succeed = true;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

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
  ACE_TRY_NEW_ENV
    {
      // Register duplicate
      cm->register_factory (cf);
    }
  ACE_CATCH (Compression::FactoryAlreadyRegistered, ex)
    {
      ACE_UNUSED_ARG (ex);
      succeed = true;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

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
  ACE_TRY_NEW_ENV
    {
      // Register nil factory
      cm->register_factory (Compression::CompressorFactory::_nil());
    }
  ACE_CATCH (CORBA::BAD_PARAM, ex)
    {
      if ((ex.minor() & 0xFFFU) == 44)
        {
          succeed = true;
        }
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, register nill factory failed\n"));
  }

  return succeed;
}


int
main (int argc, char *argv[])
{
  int retval = 0;
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var compression_manager =
        orb->resolve_initial_references("CompressionManager");

      Compression::CompressionManager_var manager =
        Compression::CompressionManager::_narrow (compression_manager.in ());

      if (CORBA::is_nil(manager.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil compression manager\n"),
                          1);

      Compression::CompressorFactory_ptr compressor_factory;

      ACE_NEW_RETURN (compressor_factory, TAO::Zlib_CompressorFactory (), 1);

      Compression::CompressorFactory_var compr_fact = compressor_factory;
      manager->register_factory(compr_fact.in ());

      if (!test_duplicate_compression_factory (manager.in (), compr_fact.in ()))
        retval = 1;

      if (!test_register_nil_compression_factory (manager.in ()))
        retval = 1;

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

      if (!test_invalid_compression_factory (manager.in ()))
        retval = 1;

      orb->destroy ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      retval = 1;
    }
  ACE_ENDTRY;

  return retval;
}
