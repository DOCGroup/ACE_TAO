// $Id$

#include "tao/ZIOP/ZIOP_ORBInitializer.h"
#include "tao/ZIOP/ZIOP_Policy_Validator.h"
#include "tao/ZIOP/ZIOP.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/operation_details.h"
#include "tao/Stub.h"
#include "tao/Transport.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ZIOP_Loader::TAO_ZIOP_Loader (void)
  : initialized_ (false)
{
}

TAO_ZIOP_Loader::~TAO_ZIOP_Loader (void)
{
}

int
TAO_ZIOP_Loader::init (int, ACE_TCHAR* [])
{
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  if (!this->initialized_ && TAO_DEF_GIOP_MINOR >= 2)
    {
      PortableInterceptor::ORBInitializer_ptr tmp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();
      PortableInterceptor::ORBInitializer_var ziop_orb_initializer;

      try
        {
          /// Register the BiDir ORBInitializer.
          ACE_NEW_THROW_EX (tmp_orb_initializer,
                            TAO_ZIOP_ORBInitializer (this),
                            CORBA::NO_MEMORY (
                                CORBA::SystemException::_tao_minor_code (
                                    TAO::VMCID,
                                    ENOMEM),
                                CORBA::COMPLETED_NO));

          ziop_orb_initializer = tmp_orb_initializer;

          PortableInterceptor::register_orb_initializer (
            ziop_orb_initializer.in ());

          this->initialized_ = true;
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
#endif /* TAO_HAS_CORBA_MESSAGING */

  return 0;
}

void
TAO_ZIOP_Loader::load_policy_validators (TAO_Policy_Validator &val)
{
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
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
#else /* TAO_HAS_CORBA_MESSAGING */
  ACE_UNUSED_ARG (val);
#endif /* TAO_HAS_CORBA_MESSAGING */
}

int
TAO_ZIOP_Loader::Initializer (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_ZIOP_Loader);
}

const char *
TAO_ZIOP_Loader::ziop_compressorid_name (::Compression::CompressorId st)
{
  switch (st)
    {
      case ::Compression::COMPRESSORID_NONE: return "NONE";
      case ::Compression::COMPRESSORID_GZIP: return "GZIP";
      case ::Compression::COMPRESSORID_PKZIP: return "PKZIP";
      case ::Compression::COMPRESSORID_BZIP2: return "BZIP2";
      case ::Compression::COMPRESSORID_ZLIB: return "ZLIB";
      case ::Compression::COMPRESSORID_LZMA: return "LZMA";
      case ::Compression::COMPRESSORID_LZO: return "LZO";
      case ::Compression::COMPRESSORID_RZIP: return "RZIP";
      case ::Compression::COMPRESSORID_7X: return "7X";
      case ::Compression::COMPRESSORID_XAR: return "XAR";
      case ::Compression::COMPRESSORID_RLE: return "RLE";
    }

  return "Unknown";
}

void
TAO_ZIOP_Loader::dump_msg (const char *type,  const u_char *ptr,
                                size_t len, size_t original_data_length,
                                ::Compression::CompressorId  compressor_id,
                                ::Compression::CompressionLevel compression_level)
{
  if (TAO_debug_level < 10)
    {
      return;
    }

  static const char digits[] = "0123456789ABCD";
  int const byte_order = ptr[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & 0x01;
  CORBA::Double const
    ratio = 100.0 -
            ( static_cast<CORBA::Double> (len) / static_cast<CORBA::Double> (original_data_length) * 100.0);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TAO (%P|%t) - ZIOP_Loader::dump_msg, ")
              ACE_TEXT ("ZIOP message v%c.%c %C, %d data bytes, %s endian, ")
              ACE_TEXT ("original_data_length = %d, ratio = %4.2f, ")
              ACE_TEXT ("compressor = %C, compression_level = %d\n"),
              digits[ptr[TAO_GIOP_VERSION_MAJOR_OFFSET]],
              digits[ptr[TAO_GIOP_VERSION_MINOR_OFFSET]],
              type,
              len - TAO_GIOP_MESSAGE_HEADER_LEN ,
              (byte_order == TAO_ENCAP_BYTE_ORDER) ? ACE_TEXT("my") : ACE_TEXT("other"),
              original_data_length,
              ratio,
              TAO_ZIOP_Loader::ziop_compressorid_name(compressor_id),
              compression_level));

  ACE_HEX_DUMP ((LM_DEBUG,
                 (const char *) ptr,
                 len,
                 ACE_TEXT ("ZIOP message")));
}

bool
TAO_ZIOP_Loader::decompress (Compression::Compressor_ptr compressor,
                             const ::Compression::Buffer &source,
                             ::Compression::Buffer &target)
{
  try
    {
      compressor->decompress (source, target);
    }
  catch (::Compression::CompressionException &e)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - TAO_ZIOP_Loader::decompress, ")
                        ACE_TEXT ("decompression failed, %C\n"),
                        e._info ().c_str ()),
                        false);
    }

  return true;
}

bool
TAO_ZIOP_Loader::decompress (ACE_Data_Block **db, TAO_Queued_Data& qd,
                             TAO_ORB_Core& orb_core)
{
#if defined (TAO_HAS_ZIOP) && TAO_HAS_ZIOP != 0
  CORBA::Object_var compression_manager =
    orb_core.resolve_compression_manager();

  Compression::CompressionManager_var manager =
    Compression::CompressionManager::_narrow (compression_manager.in ());

  if (!CORBA::is_nil(manager.in ()))
    {
      ZIOP::CompressionData data;
      // first set the read pointer after the header
      size_t begin = qd.msg_block ()-> rd_ptr() - qd.msg_block ()->base ();
      char * initial_rd_ptr = qd.msg_block ()-> rd_ptr();
      size_t const wr = qd.msg_block ()->wr_ptr () - qd.msg_block ()->base ();

      TAO_InputCDR cdr ((*db),
                        qd.msg_block ()->self_flags (),
                        begin + TAO_GIOP_MESSAGE_HEADER_LEN,
                        wr,
                        qd.byte_order (),
                        qd.giop_version ().major_version (),
                        qd.giop_version ().minor_version (),
                        &orb_core);

      if (!(cdr >> data))
        {
          return false;
        }

      // Get hold of the required compressor to perform the decompression.
      // NOTE: every compressor can decompress any of its own compression levels,
      // and at this stage we do not know what level of compression was
      // used to compress the data. (Policies are stored inside the compressed
      // datablock so we can't look it up anyway.)
      Compression::Compressor_var compressor =
            manager->get_compressor (data.compressor, 0);
      CORBA::OctetSeq myout;
      myout.length (data.original_length);

      if (decompress(compressor.in(), data.data, myout))
        {
          size_t new_data_length = (size_t)(data.original_length +
                        TAO_GIOP_MESSAGE_HEADER_LEN);

          ACE_Message_Block mb (new_data_length);
          qd.msg_block ()->rd_ptr (initial_rd_ptr);
          mb.copy(qd.msg_block ()->base () + begin,
                        TAO_GIOP_MESSAGE_HEADER_LEN);

          if (mb.copy((char*)myout.get_buffer(false),
                  (size_t)data.original_length) != 0)
            ACE_ERROR_RETURN((LM_ERROR,
                              ACE_TEXT ("TAO - (%P|%t) - ")
                              ACE_TEXT ("TAO_ZIOP_Loader::decompress, ")
                              ACE_TEXT ("failed to copy decompressed data, ")
                              ACE_TEXT ("Buffer too small\n")),
                              false);
          // change it into a GIOP message..
          mb.base ()[0] = 0x47;
          ACE_CDR::mb_align (&mb);

          if (TAO_debug_level > 9)
            {  // we're only logging ZIOP messages. Log datablock before it's
               // replaced by it's decompressed datablock
               this->dump_msg ("before decompression",
                    reinterpret_cast <u_char *>(qd.msg_block ()->rd_ptr ()),
                    qd.msg_block ()->length (), data.original_length,
                    data.compressor, compressor->compression_level ());
            }
          //replace data block
          *db = mb.data_block ()->duplicate ();
          (*db)->size (new_data_length);
          return true;
        }
    }
  else
    {
      return false;
    }
#else /* TAO_HAS_ZIOP */
  ACE_UNUSED_ARG (db);
  ACE_UNUSED_ARG (qd);
  ACE_UNUSED_ARG (orb_core);
#endif /* TAO_HAS_ZIOP */

  return true;
}

CORBA::ULong
TAO_ZIOP_Loader::compression_low_value (CORBA::Policy_ptr policy) const
{
  CORBA::ULong result = 0;
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

  if (!CORBA::is_nil (policy))
    {
      ZIOP::CompressionLowValuePolicy_var srp =
        ZIOP::CompressionLowValuePolicy::_narrow (policy);

      if (!CORBA::is_nil (srp.in ()))
        {
          result = srp->low_value ();
        }
    }
#else /* TAO_HAS_CORBA_MESSAGING */
  ACE_UNUSED_ARG (policy);
#endif /* TAO_HAS_CORBA_MESSAGING */
  return result;
}

Compression::CompressionRatio
TAO_ZIOP_Loader::compression_minratio_value (CORBA::Policy_ptr policy) const
{
  Compression::CompressionRatio result = 0;
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

  if (!CORBA::is_nil (policy))
    {
      ZIOP::CompressionMinRatioPolicy_var srp =
        ZIOP::CompressionMinRatioPolicy::_narrow (policy);

      if (!CORBA::is_nil (srp.in ()))
        {
          result = srp->ratio ();
        }
    }
#else /* TAO_HAS_CORBA_MESSAGING */
  ACE_UNUSED_ARG (policy);
#endif /* TAO_HAS_CORBA_MESSAGING */
  return result;
}

bool
TAO_ZIOP_Loader::compress (Compression::Compressor_ptr compressor,
                           const ::Compression::Buffer &source,
                           ::Compression::Buffer &target)
{
  try
    {
      compressor->compress (source, target);
    }
  catch (::Compression::CompressionException &e)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - TAO_ZIOP_Loader::compress, ")
                        ACE_TEXT ("compression failed, %C\n"),
                        e._info ().c_str ()),
                        false);
    }

  return true;
}

::Compression::CompressionRatio
TAO_ZIOP_Loader::get_ratio (CORBA::OctetSeq& uncompressed, CORBA::OctetSeq& compressed)
{
  return static_cast< ::Compression::CompressionRatio> (uncompressed.length ()) /
         static_cast< ::Compression::CompressionRatio> (compressed.length ());
}

bool
TAO_ZIOP_Loader::check_min_ratio (const ::Compression::CompressionRatio& this_ratio,
                                  ::Compression::CompressionRatio overall_ratio,
                                  ::Compression::CompressionRatio min_ratio) const
{
  bool accepted = ACE::is_equal (min_ratio, 0.0f) || (this_ratio > min_ratio);
  if (TAO_debug_level > 8)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - TAO_ZIOP_Loader::check_min_ratio, ")
                  ACE_TEXT ("overall_ratio = %4.2f, this_ratio = %4.2f, accepted = %d\n"),
                  overall_ratio, this_ratio, accepted));
    }
  return accepted;
}


bool
TAO_ZIOP_Loader::get_compressor_details (
                        ::Compression::CompressorIdLevelList *list,
                        Compression::CompressorId &compressor_id,
                        Compression::CompressionLevel &compression_level)

{
  if (list)
    {
      compressor_id = (*list)[0].compressor_id;
      compression_level = (*list)[0].compression_level;

      if (TAO_debug_level > 6)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - ")
                      ACE_TEXT ("TAO_ZIOP_Loader::get_compressor_details,")
                      ACE_TEXT ("compressor policy found, compressor = %C, ")
                      ACE_TEXT ("compression_level = %d\n"),
                      TAO_ZIOP_Loader::ziop_compressorid_name (compressor_id),
                          compression_level));
        }

    }
  else
    {
      if (TAO_debug_level > 6)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - ")
                      ACE_TEXT ("TAO_ZIOP_Loader::get_compressor_details, ")
                      ACE_TEXT ("no appropriate compressor found\n")));
        }
      return false;
    }
  return true;
}

bool
TAO_ZIOP_Loader::get_compression_details(
                        CORBA::Policy_ptr compression_enabling_policy,
                        CORBA::Policy_ptr compression_level_list_policy,
                        Compression::CompressorId &compressor_id,
                        Compression::CompressionLevel &compression_level)
{
  bool use_ziop = false;
  if (!CORBA::is_nil (compression_enabling_policy))
    {
      ZIOP::CompressionEnablingPolicy_var srp =
        ZIOP::CompressionEnablingPolicy::_narrow (compression_enabling_policy);

      if (!CORBA::is_nil (srp.in ()))
        {
          use_ziop = srp->compression_enabled ();
          if (!use_ziop && TAO_debug_level > 8)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - ")
                          ACE_TEXT ("TAO_ZIOP_Loader::get_compression_details, ")
                          ACE_TEXT ("ZIOP enabled policy not set\n")));
            }
        }
    }
  else
    {
      if (TAO_debug_level > 6)
        {
          ACE_DEBUG ((LM_DEBUG,
                     ACE_TEXT ("TAO (%P|%t) - ")
                     ACE_TEXT ("TAO_ZIOP_Loader::get_compression_details, ")
                     ACE_TEXT ("compression_enabling_policy is NIL, no ZIOP\n")));
        }
    }

  if (use_ziop)
    {
      if (!CORBA::is_nil (compression_level_list_policy))
        {
          ZIOP::CompressorIdLevelListPolicy_var srp =
            ZIOP::CompressorIdLevelListPolicy::_narrow (compression_level_list_policy);

          if (!CORBA::is_nil (srp.in ()))
            {
              use_ziop = get_compressor_details (srp->compressor_ids (),
                                    compressor_id, compression_level);
            }
        }
      else
        {
          if (TAO_debug_level > 6)
            {
              ACE_DEBUG ((LM_DEBUG,
                         ACE_TEXT("TAO (%P|%t) - ")
                         ACE_TEXT("TAO_ZIOP_Loader::get_compression_details, ")
                         ACE_TEXT("compressor ID/Level list policy not found\n")));
            }
          use_ziop = false;
        }
    }
  return use_ziop;
}

bool
TAO_ZIOP_Loader::complete_compression (Compression::Compressor_ptr compressor,
                                       TAO_OutputCDR &cdr,
                                       ACE_Message_Block& mb,
                                       char *initial_rd_ptr,
                                       CORBA::ULong low_value,
                                       Compression::CompressionRatio min_ratio,
                                       CORBA::ULong original_data_length,
                                       Compression::CompressorId compressor_id)
{
  if (low_value > 0 && original_data_length > low_value)
    {
      CORBA::OctetSeq output;
      CORBA::OctetSeq input (original_data_length, &mb);
      output.length (original_data_length);

      if (!this->compress (compressor, input, output))
        return false;
      else if ((output.length () < original_data_length) &&
              (this->check_min_ratio (this->get_ratio (input, output),
                      compressor->compression_ratio(),
                      min_ratio)))
        {
          mb.wr_ptr (mb.rd_ptr ());
          cdr.current_alignment (mb.wr_ptr() - mb.base ());
          ZIOP::CompressionData data;
          data.compressor = compressor_id;
          data.original_length = input.length();
          data.data = output;
          cdr << data;
          mb.rd_ptr(initial_rd_ptr);
          int begin = (mb.rd_ptr() - mb.base ());
          mb.data_block ()->base ()[0 + begin] = 0x5A;
          mb.data_block ()->base ()[TAO_GIOP_MESSAGE_SIZE_OFFSET + begin] =
            cdr.length() - TAO_GIOP_MESSAGE_HEADER_LEN;

          if (TAO_debug_level > 9)
            {
               this->dump_msg ("after compression", reinterpret_cast <u_char *>(mb.rd_ptr ()),
                    mb.length (), data.original_length,
                    data.compressor, compressor->compression_level ());
            }

        }
      else
        return false;
    }
    else if (TAO_debug_level > 8)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - ")
                    ACE_TEXT ("TAO_ZIOP_Loader::compress_data, ")
                    ACE_TEXT ("no compression used, ")
                    ACE_TEXT ("low value policy applied\n")));
      }
  return true;
}

bool
TAO_ZIOP_Loader::compress_data (TAO_OutputCDR &cdr,
               CORBA::Object_ptr compression_manager,
               CORBA::ULong low_value,
               ::Compression::CompressionRatio min_ratio,
               ::Compression::CompressorId compressor_id,
               ::Compression::CompressionLevel compression_level)
{
  bool compressed = true;

  cdr.consolidate ();

  ACE_Message_Block* current = const_cast <ACE_Message_Block*> (cdr.current ());

  char* initial_rd_ptr = current->rd_ptr();

  // Set the read pointer to the point where the data starts
  current->rd_ptr (TAO_GIOP_MESSAGE_HEADER_LEN);

  current = const_cast <ACE_Message_Block*> (cdr.current());
  CORBA::ULong const original_data_length =
    (CORBA::ULong)(current->wr_ptr() - current->rd_ptr());

  if (original_data_length > 0)
    {
      Compression::CompressionManager_var manager =
        Compression::CompressionManager::_narrow (compression_manager);

      if (!CORBA::is_nil(manager.in ()))
        {
          Compression::Compressor_var compressor =
            manager->get_compressor (compressor_id, compression_level);

          compressed = complete_compression (compressor.in (), cdr, *current,
                initial_rd_ptr, low_value, min_ratio,
                original_data_length, compressor_id);
        }
    }
  // set back read pointer in case no compression was done...
  current->rd_ptr(initial_rd_ptr);
  return compressed;
}

bool
TAO_ZIOP_Loader::marshal_data (TAO_OutputCDR &cdr, TAO_Stub &stub)
{
#if defined (TAO_HAS_ZIOP) && TAO_HAS_ZIOP != 0
  Compression::CompressorId compressor_id = Compression::COMPRESSORID_NONE;
  Compression::CompressionLevel compression_level = 0;

  CORBA::Policy_var compression_enabling_policy =
    stub.get_cached_policy (TAO_CACHED_COMPRESSION_ENABLING_POLICY);
  CORBA::Policy_var compression_level_list_policy =
    stub.get_cached_policy (TAO_CACHED_COMPRESSION_ID_LEVEL_LIST_POLICY);

  if (get_compression_details (
        compression_enabling_policy.in (),
        compression_level_list_policy.in (),
        compressor_id,
        compression_level))
    {
      CORBA::Object_var compression_manager =
        stub.orb_core ()->resolve_compression_manager();

      CORBA::Policy_var policy_low_value =
        stub.get_cached_policy (TAO_CACHED_COMPRESSION_LOW_VALUE_POLICY);
      CORBA::Policy_var policy_min_ratio =
        stub.get_cached_policy (TAO_CACHED_MIN_COMPRESSION_RATIO_POLICY);

      CORBA::ULong low_value =
        this->compression_low_value (policy_low_value.in ());
      Compression::CompressionRatio min_ratio =
        this->compression_minratio_value (policy_min_ratio.in ());

      return this->compress_data (cdr, compression_manager.in (),
                                  low_value, min_ratio,
                                  compressor_id, compression_level);
    }
#else /* TAO_HAS_ZIOP */
  ACE_UNUSED_ARG (cdr);
  ACE_UNUSED_ARG (stub);
#endif /* TAO_HAS_ZIOP */

  return false; // Did not compress
}

bool
TAO_ZIOP_Loader::marshal_data (TAO_OutputCDR &cdr, TAO_ORB_Core &orb_core, TAO_ServerRequest *request)
{
  // If there is no TAO_ServerRequest supplied, then there are no client side ZIOP policies to check.
  if (!request)
    {
      if (6 < TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - ")
                      ACE_TEXT("TAO_ZIOP_Loader::marshal_data (server_reply), ")
                      ACE_TEXT("client policies not available (did not compress).\n")));
        }
      return false; // Did not compress
    }

#if defined (TAO_HAS_ZIOP) && TAO_HAS_ZIOP != 0
  // Check the client supplied the compression enabling policy for this request.
  ::ZIOP::CompressionEnablingPolicy_var
    check (::ZIOP::CompressionEnablingPolicy::_narrow (
             request->clientCompressionEnablingPolicy ()));
  if (CORBA::is_nil (check.in ()) || !check->compression_enabled ())
    {
      if (6 < TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - ")
                      ACE_TEXT("TAO_ZIOP_Loader::marshal_data (server_reply), ")
                      ACE_TEXT("clientCompressionEnablingPolicy (did not compress).\n")));
        }
      return false; // Did not compress
    }

  // Check the server supplied the compression enabling policy.
  CORBA::Policy_var serverPolicy (
    orb_core.get_cached_policy_including_current (
      TAO_CACHED_COMPRESSION_ENABLING_POLICY));
  check = ::ZIOP::CompressionEnablingPolicy::_narrow (serverPolicy.in ());
  if (CORBA::is_nil (check.in ()) || !check->compression_enabled ())
    {
      if (6 < TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - ")
                      ACE_TEXT("TAO_ZIOP_Loader::marshal_data (server_reply), ")
                      ACE_TEXT("serverCompressionEnablingPolicy (did not compress).\n")));
        }
      return false; // Did not compress
    }

  // Check the client has supplied the available compressor ID list for this request.
  ZIOP::CompressorIdLevelListPolicy_var clientCompressors (
    ZIOP::CompressorIdLevelListPolicy::_narrow (
      request->clientCompressorIdLevelListPolicy ()));
  if (is_nil (clientCompressors.in ()))
    {
      if (6 < TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - ")
                      ACE_TEXT("TAO_ZIOP_Loader::marshal_data (server_reply), ")
                      ACE_TEXT("no clientCompressorIdLevelListPolicy (did not compress).\n")));
        }
      return false; // Did not compress
    }
  ::Compression::CompressorIdLevelList &clientList =
    *clientCompressors->compressor_ids ();

  // Check the server has supplied a compressor ID list.
  serverPolicy= orb_core.get_cached_policy_including_current (
    TAO_CACHED_COMPRESSION_ID_LEVEL_LIST_POLICY);
  ZIOP::CompressorIdLevelListPolicy_var serverCompressors (
    ZIOP::CompressorIdLevelListPolicy::_narrow (serverPolicy.in ()));
  if (is_nil (serverCompressors.in ()))
    {
      if (6 < TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - ")
                      ACE_TEXT("TAO_ZIOP_Loader::marshal_data (server_reply), ")
                      ACE_TEXT("no serverCompressorIdLevelListPolicy (did not compress).\n")));
        }
      return false; // Did not compress
    }
  ::Compression::CompressorIdLevelList &serverList =
    *serverCompressors->compressor_ids ();

  // Check the whole server list (in priority order)
  for (CORBA::ULong server = 0u; server < serverList.length (); ++server)
    {
      ::Compression::CompressorIdLevel_var serverEntry (serverList[server]);

      // ...for the first matching client compressor that is available
      for (CORBA::ULong client = 0u; client < clientList.length (); ++client)
        {
          ::Compression::CompressorIdLevel_var clientEntry (clientList[client]);
          if (serverEntry->compressor_id == clientEntry->compressor_id)
            {
              // Found the first matching server in the client's available list.
              // The correct compression level to use is the smaller of the two
              // listed compression levels.
              Compression::CompressionLevel
                compression_level = ACE_MIN (serverEntry->compression_level,
                                             clientEntry->compression_level);
              if (6 < TAO_debug_level)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT("TAO (%P|%t) - ")
                              ACE_TEXT("TAO_ZIOP_Loader::marshal_data (server_reply), ")
                              ACE_TEXT("Found (Server %d: %s@%d == Client %d: %s@%d) using @%d.\n"),
                              server,
                              this->ziop_compressorid_name (serverEntry->compressor_id),
                              serverEntry->compression_level,
                              client,
                              this->ziop_compressorid_name (clientEntry->compressor_id),
                              clientEntry->compression_level,
                              compression_level));
                }

              // Obtain the other server supplied policy settings
              serverPolicy= orb_core.get_cached_policy_including_current (
                TAO_CACHED_COMPRESSION_LOW_VALUE_POLICY);
              CORBA::ULong low_value=
                this->compression_low_value (serverPolicy.in ());

              serverPolicy= orb_core.get_cached_policy_including_current (
                TAO_CACHED_MIN_COMPRESSION_RATIO_POLICY);
              Compression::CompressionRatio min_ratio=
                this->compression_minratio_value (serverPolicy.in ());

              // Attempt to compress the data.
              CORBA::Object_var compression_manager (
                orb_core.resolve_compression_manager ());
              return this->compress_data (cdr, compression_manager.in (),
                                          low_value, min_ratio,
                                          serverEntry->compressor_id,
                                          compression_level);
            }

          if (7 < TAO_debug_level)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT("TAO (%P|%t) - ")
                          ACE_TEXT("TAO_ZIOP_Loader::marshal_data (server_reply), ")
                          ACE_TEXT("checking (Server %d: %s@%d != Client %d: %s@%d).\n"),
                          server,
                          this->ziop_compressorid_name (serverEntry->compressor_id),
                          serverEntry->compression_level,
                          client,
                          this->ziop_compressorid_name (clientEntry->compressor_id),
                          clientEntry->compression_level));
            }
        } // next clientEntry
    } // next serverEntry

  if (6 < TAO_debug_level)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - ")
                  ACE_TEXT("TAO_ZIOP_Loader::marshal_data (server_reply), ")
                  ACE_TEXT("no matching CompressorIdLevelListPolicy (did not compress).\n")));
    }
#else /* TAO_HAS_ZIOP */
  ACE_UNUSED_ARG (cdr);
  ACE_UNUSED_ARG (orb_core);
#endif /* TAO_HAS_ZIOP */

  return false; // Did not compress
}

ACE_STATIC_SVC_DEFINE (TAO_ZIOP_Loader,
                       ACE_TEXT ("ZIOP_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_ZIOP_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_ZIOP, TAO_ZIOP_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL
