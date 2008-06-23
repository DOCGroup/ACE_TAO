#include "tao/ZIOP/ZIOP_ORBInitializer.h"
#include "tao/ZIOP/ZIOP_Policy_Validator.h"
#include "tao/ZIOP/ZIOP.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/Operation_Details.h"
#include "tao/Stub.h"

ACE_RCSID (ZIOP,
           ZIOP,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Set the flag to zero to start with
bool TAO_ZIOP_Loader::is_activated_ = false;

TAO_ZIOP_Loader::TAO_ZIOP_Loader (void)
{
}

TAO_ZIOP_Loader::~TAO_ZIOP_Loader (void)
{
}

int
TAO_ZIOP_Loader::init (int, ACE_TCHAR* [])
{
  if (TAO_ZIOP_Loader::is_activated_ == false && TAO_DEF_GIOP_MINOR >= 2)
    {
      PortableInterceptor::ORBInitializer_ptr tmp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();
      PortableInterceptor::ORBInitializer_var ziop_orb_initializer;

      try
        {
          /// Register the BiDir ORBInitializer.
          ACE_NEW_THROW_EX (tmp_orb_initializer,
                            TAO_ZIOP_ORBInitializer,
                            CORBA::NO_MEMORY (
                                CORBA::SystemException::_tao_minor_code (
                                    TAO::VMCID,
                                    ENOMEM),
                                CORBA::COMPLETED_NO));

          ziop_orb_initializer = tmp_orb_initializer;

          PortableInterceptor::register_orb_initializer (
            ziop_orb_initializer.in ());

          TAO_ZIOP_Loader::is_activated_ = true;
        }
      catch (const ::CORBA::Exception& ex)
        {
          if (TAO_debug_level > 0)
            {
              ex._tao_print_exception ("Caught exception:");
            }
          return -1;
        }
    }

  return 0;
}

void
TAO_ZIOP_Loader::load_policy_validators (TAO_Policy_Validator &val)
{
  // Is this true? Does the GIOP protocol version matter here?
  if (TAO_DEF_GIOP_MINOR < 2)
    return;

  TAO_ZIOPPolicy_Validator *validator = 0;
  ACE_NEW_THROW_EX (validator,
                    TAO_ZIOPPolicy_Validator (val.orb_core ()),
                    CORBA::NO_MEMORY (
                        CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                        CORBA::COMPLETED_NO));

  // We may be adding another TAO_BiDirPolicy_Validator instance for
  // the same ORB (different POA). In cases where huge numbers of
  // bi-directional POA instances are created, having a validator
  // instance per POA may introduce additional delays in policy
  // validation and hence, the overal policy creation time. Since this
  // is out of the critical invocation processing path, I plan to keep
  // the design simple and not try to avoid an ineficiency of such
  // small proportions.
  val.add_validator (validator);
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
TAO_ZIOP_Loader::marshal_data (TAO_Operation_Details &details, TAO_OutputCDR &stream, TAO::Profile_Transport_Resolver &resolver)
{
  CORBA::Boolean use_ziop = false;
  CORBA::Policy_var policy = CORBA::Policy::_nil ();

  if (resolver.stub () == 0)
    {
      policy =
        resolver.stub()->orb_core()->get_cached_policy_including_current (TAO_CACHED_POLICY_ZIOP);
    }
  else
    {
      policy = resolver.stub ()->get_cached_policy (TAO_CACHED_POLICY_ZIOP);
    }

  if (!CORBA::is_nil (policy.in ()))
    {
      ZIOP::CompressionEnablingPolicy_var srp =
        ZIOP::CompressionEnablingPolicy::_narrow (policy.in ());

      if (!CORBA::is_nil (srp.in ()))
        {
          use_ziop = srp->compression_enabled ();
        }
    }

    ACE_Message_Block* current = const_cast <ACE_Message_Block*> (stream.current ());

    if (use_ziop)
      {
         // Set the read pointer to the point where the application data starts
         current->rd_ptr (current->wr_ptr());
      }

    // Marshal application data
    if (details.marshal_args (stream) == false)
      {
        throw ::CORBA::MARSHAL ();
      }

    if (use_ziop) {
         // We can only compress one message block, so when compression is enabled first do
         // a consolidate.
         stream.consolidate ();

  CORBA::Object_var compression_manager =
    resolver.stub()->orb_core()->resolve_compression_manager();

  Compression::CompressionManager_var manager =
    Compression::CompressionManager::_narrow (compression_manager.in ());

  if (!CORBA::is_nil(manager.in ()))
  {
    Compression::CompressorId compressor_id = Compression::COMPRESSORID_ZLIB;
    Compression::Compressor_var compressor = manager->get_compressor (compressor_id, 6);

    CORBA::OctetSeq myout;
    current = const_cast <ACE_Message_Block*> (stream.current());
    CORBA::ULong original_data_length =(CORBA::ULong)(current->wr_ptr() - current->rd_ptr());
    myout.length (original_data_length);

    CORBA::OctetSeq input (original_data_length, current);

    // todo catch exceptions
    compressor->compress (input, myout);
    if (myout.length () < original_data_length)
    {
    stream.compressed (true);
    current->wr_ptr (current->rd_ptr ());
    stream.current_alignment (current->wr_ptr() - current->base ());
    ZIOP::CompressedData data;
    data.compressorid = compressor_id;
    data.original_length = input.length();
    data.data = myout;
    stream << data;
    }
  }
         // Set the read pointer back to the starting point
         current->rd_ptr (current->base ());
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

