#include "tao/ZIOP/ZIOP_ORBInitializer.h"
#include "tao/ZIOP/ZIOP.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/Operation_Details.h"

ACE_RCSID (ZIOP,
           ZIOP,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Set the flag to zero to start with
int TAO_ZIOP_Loader::is_activated_ = 0;

TAO_ZIOP_Loader::TAO_ZIOP_Loader (void)
{
}

TAO_ZIOP_Loader::~TAO_ZIOP_Loader (void)
{
}

int
TAO_ZIOP_Loader::init (int,
                            ACE_TCHAR* [])
{
  if (TAO_ZIOP_Loader::is_activated_ == 0 && TAO_DEF_GIOP_MINOR >= 2)
    {
      PortableInterceptor::ORBInitializer_ptr tmp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();
      PortableInterceptor::ORBInitializer_var ziop_orb_initializer;

      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          /// Register the BiDir ORBInitializer.
          ACE_NEW_THROW_EX (tmp_orb_initializer,
                            TAO_ZIOP_ORBInitializer,
                            CORBA::NO_MEMORY (
                                CORBA::SystemException::_tao_minor_code (
                                    TAO::VMCID,
                                    ENOMEM),
                                CORBA::COMPLETED_NO));
          ACE_TRY_CHECK;

          ziop_orb_initializer = tmp_orb_initializer;

          PortableInterceptor::register_orb_initializer (
            ziop_orb_initializer.in ()
            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          TAO_ZIOP_Loader::is_activated_ = 1;
        }
      ACE_CATCHANY
        {
          if (TAO_debug_level > 0)
            {
              ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                   "Caught exception:");
            }
          return -1;
        }
      ACE_ENDTRY;
    }

  return 0;
}

void
TAO_ZIOP_Loader::load_policy_validators (TAO_Policy_Validator &val)
{
}

int
TAO_ZIOP_Loader::Initializer (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_ZIOP_Loader);
}

bool
TAO_ZIOP_Loader::decompress (TAO_ServerRequest& server_request)
{
  CORBA::Object_var compression_manager =
    server_request.orb_core()->resolve_compression_manager();

  Compression::CompressionManager_var manager =
    Compression::CompressionManager::_narrow (compression_manager.in ());

  if (!CORBA::is_nil(manager.in ()))
    {
      ZIOP::CompressedData data;
      if ((*(server_request.incoming()) >> data) == 0)
        return false;
//      server_request.compressed_ = true;

      Compression::Compressor_var compressor = manager->get_compressor (data.compressorid, 6);
      CORBA::OctetSeq myout;
      myout.length (data.original_length);

      compressor->decompress (data.data, myout);
      TAO_InputCDR* newstream = new TAO_InputCDR ((char*)myout.get_buffer(true), (size_t)data.original_length);
      server_request.incoming()->steal_from (*newstream);
    }
  return true;
}

bool
TAO_ZIOP_Loader::compress (
  TAO_ORB_Core& core,
  TAO_Operation_Details &details,
  TAO_OutputCDR &out_stream)
{
  CORBA::Object_var compression_manager =
    core.resolve_compression_manager();

  Compression::CompressionManager_var manager =
    Compression::CompressionManager::_narrow (compression_manager.in ());

  if (!CORBA::is_nil(manager.in ()))
  {
    Compression::CompressorId compressor_id = Compression::COMPRESSORID_ZLIB;
    Compression::Compressor_var compressor = manager->get_compressor (compressor_id, 6);

    CORBA::OctetSeq myout;
    myout.length ((CORBA::ULong)(out_stream.length()));

    CORBA::OctetSeq input ((CORBA::ULong)(out_stream.length()), out_stream.begin ());
    
    // todo catch exceptions
    compressor->compress (input, myout);
    if (myout.length () < input.length())
    {
    out_stream.compressed (true);
    ACE_Message_Block *newblock = new ACE_Message_Block ((const char*)myout.get_buffer(), (size_t)myout.length());
    newblock->wr_ptr ((size_t)myout.length());
    ZIOP::CompressedData data;
    data.compressorid = compressor_id;
    data.original_length = input.length();
    data.data = myout;
    out_stream.reset ();
    out_stream << data;
    return true;
    }
  }
  return false;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (TAO_ZIOP_Loader,
                       ACE_TEXT ("ZIOP_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_ZIOP_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_ZIOP, TAO_ZIOP_Loader)

