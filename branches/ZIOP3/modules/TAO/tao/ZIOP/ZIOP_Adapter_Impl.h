

class
{
ZlibCompressor::ZlibCompressor (
  ::ZIOP::CompressionLevel compression_level,
  ::ZIOP::CompressorFactory_ptr compressor_factory) :
    BaseCompressor (compression_level, compressor_factory)
{
}


#if !defined (__BORLANDC__)
            Bytef* LargBuffer = new Bytef [1000];
      uLongf length = 100;
            int retval = compress (LargBuffer,   &length,
                                 reinterpret_cast <const Bytef*>(compression_stream.buffer ()), compression_stream.total_length ());
#endif
            // Compress stream, dependent on bigger or smaller we use
            // the compressed stream or the non compressed
            this->details_.compressed (true);
            cdr.compressed (true);

            /// @todo write length of original data in the service context list
      this->details_.uncompressed_size_ = compression_stream.total_length();
            this->write_header (tspec,
                                cdr
                                ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
ACE_Message_Block *newblock = new ACE_Message_Block ((const char*)LargBuffer, (size_t)length);
newblock->wr_ptr ((size_t)length);
            cdr.write_octet_array_mb (newblock);



#if !defined (__BORLANDC__)
            Bytef* LargBuffer = new Bytef [request.original_message_length_ * 2];
      uLongf length = request.original_message_length_ * 2;
            int retval = uncompress (LargBuffer,   &length,
        (const Bytef*)cdr.rd_ptr(), cdr.length ());
                          //       reinterpret_cast <const Bytef*>(compression_stream.buffer ()), compression_stream.total_length ());
      char* buf = (char*)LargBuffer;

TAO_InputCDR* newstream = new TAO_InputCDR (buf, (size_t)length);
request.incoming_ = newstream;


  }
}

};
