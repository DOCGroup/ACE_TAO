#include "tao/ZIOP/ZIOP.h"
#include "ace/Synch_Traits.h"

namespace TAO
{
  namespace ZIOP
  {
    class BaseCompressor : public ::ZIOP::Compressor
    {
      virtual void compress (
          const ::CORBA::OctetSeq & source,
          ::CORBA::OctetSeq & target
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException,
          ::ZIOP::CompressionException
        )) = 0;

      virtual void decompress (
          const ::CORBA::OctetSeq & source,
          ::CORBA::OctetSeq & target
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException,
          ::ZIOP::CompressionException
        )) = 0;

      virtual ::ZIOP::CompressorFactory_ptr compressor_factory (
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException
        ))
      {
        return ::ZIOP::CompressorFactory::_duplicate (compressor_factory_.in ());
      }

      virtual ::ZIOP::CompressionLevel compression_level (
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException
        ))
      {
        return compression_level_;
      }

      private:
        ::ZIOP::CompressionLevel compression_level_;
        ::ZIOP::CompressorFactory_var compressor_factory_;

    };

    class ZlibCompressor : public BaseCompressor
    {
      virtual void compress (
          const ::CORBA::OctetSeq & source,
          ::CORBA::OctetSeq & target
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException,
          ::ZIOP::CompressionException
        ))
      {
        // http://www.zlib.net/manual.html
      }

      virtual void decompress (
          const ::CORBA::OctetSeq & source,
          ::CORBA::OctetSeq & target
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException,
          ::ZIOP::CompressionException
        ))
      {
      }
    };

    class PkzipCompressor : public BaseCompressor
    {
      virtual void compress (
          const ::CORBA::OctetSeq & source,
          ::CORBA::OctetSeq & target
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException,
          ::ZIOP::CompressionException
        ))
      {
      }

      virtual void decompress (
          const ::CORBA::OctetSeq & source,
          ::CORBA::OctetSeq & target
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException,
          ::ZIOP::CompressionException
        ))
      {
      }
    };

    class Bzip2Compressor : public BaseCompressor
    {
      virtual void compress (
          const ::CORBA::OctetSeq & source,
          ::CORBA::OctetSeq & target
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException,
          ::ZIOP::CompressionException
        ))
      {
        // See http://www.bzip.org/1.0.3/html/util-fns.html
      }

      virtual void decompress (
          const ::CORBA::OctetSeq & source,
          ::CORBA::OctetSeq & target
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException,
          ::ZIOP::CompressionException
        ))
      {
      }
    };
  }
}
