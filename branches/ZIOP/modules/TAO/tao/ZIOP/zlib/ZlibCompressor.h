#include "tao/ZIOP/ZIOP.h"
#include "tao/ZIOP/zlib/ZlibCompressor_Export.h"

namespace TAO
{
  namespace ZIOP
  {
    class TAO_ZLIBCOMPRESSOR_Export ZlibCompressor : public BaseCompressor
    {
      public:
      ZlibCompression (::ZIOP::CompressionLevel compression_level,
                      ::ZIOP::CompressorFactory_ptr compressor_factory);

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
    };
  }
}
