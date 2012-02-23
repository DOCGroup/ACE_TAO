// $Id$

/*
* Run-length encoding (RLE) is a very simple form of data compression
* in which runs of data (that is, sequences in which the same data value
* occurs in many consecutive data elements) are stored as a single data
* value and count, rather than as the original run. This is most useful
* on data that contains many such runs: for example, simple graphic
* images such as icons, line drawings, and animations. It is not useful
* with files that don't have many runs as it could slightly increase the
* output size.
* ALGORITHM: This algorithm is an optimised version of the traditional
* RLE algorithm in that it behaves better with very few runs.
*
* With a run of a character where that run is >= 3 this is
* replaced with the repeat indicator 0X80 and then the repeat count OR'd
* over this ident.  This repeat count is therefore has a maximum value
* of 127 (0x7F) which is to be interpreted as the next charater repeated
* another 'repeat count' times (i.e. a maximum of 128 characters can be
* represented in any single dupal). if the repeat ident is not present
* then the count is to be interpreted as a copy of the next repeat count
* charaters + 1.
*
* NOTE: This algorithm seems to do very well on standard GIOP messages
* when sending them across low bandwith (i.e. HF) bearers.
*
* EXAMPLE: the following arbitary string of 67 bytes:-
* WWWWWWWWWWWWBWWWWWWWWWWWWBBBWWWWWWWWWWWWWWWWWWWWWWWWBWWWWWWWWWWWWWW
* will produce (as a HEXDUMP) of 14 bytes
* 8B 57 00 42 8B 57 82 42  97 57 00 42 8D 57        .W.B.W.B.W.B.W
*
*/

#include "RLECompressor.h"
#include "ace/Compression/rle/RLECompressor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

RLECompressor::RLECompressor(::Compression::CompressorFactory_ptr compressor_factory)
    : BaseCompressor (compressor_factory, 0)
{
}

void
RLECompressor::compress(const ::Compression::Buffer &source, ::Compression::Buffer &target)
{
    ACE_UINT64  in_len  = source.length();
    ACE_UINT64  max_len = static_cast<ACE_UINT64>((in_len * 1.1) + 32U); // Slightly bigger than input

    target.length(static_cast< CORBA::ULong>(max_len)); // Ensure we set a length.

    ACE_UINT64 out_len = ACE_RLECompression::instance()->compress( source.get_buffer(),
                                                                   in_len,
                                                                   target.get_buffer(),
                                                                   max_len );
    if (ACE_UINT64(-1) == out_len) { // Overrun
        throw ::Compression::CompressionException();
    }

    target.length(static_cast< CORBA::ULong>(out_len)); // Set Output Buffer to the right size now.

    // Update statistics for this compressor
    this->update_stats(source.length(), target.length());
}

void
RLECompressor::decompress(const ::Compression::Buffer &source, ::Compression::Buffer &target)
{
    ACE_UINT64 out_len = ACE_RLECompression::instance()->decompress(source.get_buffer(),
                                                                    source.length(),
                                                                    target.get_buffer(),
                                                                    target.length() );
    if (ACE_UINT64(-1) == out_len) { // Overrun
        throw ::Compression::CompressionException();
    }

    target.length(static_cast<CORBA::ULong>(out_len));
}

}

TAO_END_VERSIONED_NAMESPACE_DECL
