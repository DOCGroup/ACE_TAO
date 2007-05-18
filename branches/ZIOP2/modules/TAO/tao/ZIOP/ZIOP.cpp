#include "tao/ZIOP/ZIOP_ORBInitializer.h"
#include "tao/ZIOP/ZIOP.h"
//#include "tao/BiDir_GIOP/BiDirPolicy_Validator.h"
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
TAO_ZIOP_Loader::load_policy_validators (TAO_Policy_Validator &val
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
/*  // Is this true? Does the GIOP protocol version matter here?
  if (TAO_DEF_GIOP_MINOR < 2)
    return;

  TAO_BiDirPolicy_Validator *validator = 0;
  ACE_NEW_THROW_EX (validator,
                    TAO_BiDirPolicy_Validator (val.orb_core ()),
                    CORBA::NO_MEMORY (
                        CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                        CORBA::COMPLETED_NO));
  ACE_CHECK;

  // We may be adding another TAO_BiDirPolicy_Validator instance for
  // the same ORB (different POA). In cases where huge numbers of
  // bi-directional POA instances are created, having a validator
  // instance per POA may introduce additional delays in policy
  // validation and hence, the overal policy creation time. Since this
  // is out of the critical invocation processing path, I plan to keep
  // the design simple and not try to avoid an ineficiency of such
  // small proportions.
  val.add_validator (validator);*/
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

  Compression::Compressor_var compressor = manager->get_compressor (::Compression::COMPRESSORID_ZLIB, 6);

      CORBA::OctetSeq myout;
      myout.length ((CORBA::ULong)server_request.original_message_length_);

      CORBA::OctetSeq input ((CORBA::ULong)(server_request.incoming()->length(),server_request.incoming()->rd_ptr()));
      compressor->decompress (input, myout);
      TAO_InputCDR* newstream = new TAO_InputCDR ((char*)myout.get_buffer(), (size_t)server_request.original_message_length_);
      server_request.incoming()->steal_from (*newstream);
  }
//  TAO_InputCDR cdr (reinterpret_cast<const char*> (
//                      context.context_data.get_buffer ()),
//                      context.context_data.length ());

//  CORBA::Boolean byte_order;
//  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
//    return false;

//  cdr.reset_byte_order (static_cast<int> (byte_order));

//  CORBA::ULong message_length = 0;
//  if (!(cdr >> message_length))
//    return false;
//+#if !defined (__BORLANDC__)
//+            Bytef* LargBuffer = new Bytef [request.original_message_length_ * 2];
//+     uLongf length = request.original_message_length_ * 2;
//+            int retval = uncompress (LargBuffer,   &length,
//+       (const Bytef*)cdr.rd_ptr(), cdr.length ());
//+                          //       reinterpret_cast <const Bytef*>(compression_stream.buffer ()), compression_stream.total_length ());
//+     char* buf = (char*)LargBuffer;

//+TAO_InputCDR* newstream = new TAO_InputCDR (buf, (size_t)length);
//+request.incoming_ = newstream;
//+#endif
//+
//+ // do decompression
return true;
}

bool
TAO_ZIOP_Loader::compress (TAO_ORB_Core& core, TAO_Operation_Details &details, TAO_OutputCDR &out_stream)
{
  TAO_OutputCDR compression_stream;
  if (details.marshal_args (compression_stream) == false)
    {
      throw ::CORBA::MARSHAL ();
    }

  CORBA::Object_var compression_manager =
    core.resolve_compression_manager();

  Compression::CompressionManager_var manager =
    Compression::CompressionManager::_narrow (compression_manager.in ());

  if (!CORBA::is_nil(manager.in ()))
  {

  Compression::Compressor_var compressor = manager->get_compressor (::Compression::COMPRESSORID_ZLIB, 6);

      CORBA::OctetSeq myout;
      myout.length ((CORBA::ULong)(compression_stream.length() * 1.1));

      CORBA::OctetSeq input ((CORBA::ULong)(compression_stream.length()), compression_stream.begin ());
      compressor->compress (input, myout);
  details.compressed (true);
  ACE_Message_Block *newblock = new ACE_Message_Block ((const char*)myout.get_buffer(), (size_t)myout.length());
  newblock->wr_ptr ((size_t)myout.length());
//  out_stream.begin ()->cont (newblock);
out_stream.write_octet_array(myout.get_buffer (), myout.length());

      TAO_OutputCDR cdr;

      // Add the original message length to the service contenxt
      CORBA::ULong length = compression_stream.total_length();
      if ((cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER) == 0)
          || (cdr << length) == 0)
        return false;

      // Add this info in to the svc_list
      details.request_service_context ().set_context (IOP::TAG_ZIOP_COMPONENT,
                                                        cdr);

  }

//#if !defined (__BORLANDC__)
//            Bytef* LargBuffer = new Bytef [1000];
//     uLongf length = 100;
//            int retval = compress (LargBuffer,   &length,
//                                 reinterpret_cast <const Bytef*>(compression_stream.buffer ()), compression_stream.total_length ());
//#endif
            // Compress stream, dependent on bigger or smaller we use
            // the compressed stream or the non compressed

}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (TAO_ZIOP_Loader,
                       ACE_TEXT ("ZIOP_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_ZIOP_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_ZIOP, TAO_ZIOP_Loader)
