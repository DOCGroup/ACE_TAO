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
    size_t  in_len  = static_cast<size_t>(source.length());
    size_t  out_len = static_cast<size_t>((in_len * 1.1) + 32); // Slightly bigger than input

    target.length(static_cast< ::CORBA::ULong>(out_len)); // Set a length

    const unsigned char *in_p   = static_cast<const unsigned char*>(source.get_buffer());
    unsigned char *out_p        = static_cast<unsigned char*>(target.get_buffer());

    size_t  out_index   = 0;
    size_t  out_base    = 0;
    size_t  run_count   = 0;
    size_t  dup_count   = 0;

    bool    run_code    = false;
    unsigned char nxt_byte, cur_byte;

    if (in_p && out_p) while (in_len-- > 0) {

        if (run_code) switch (run_count) {

        default:

            out_p[out_index = out_base] = ::CORBA::Octet(run_count++ | 0x80);
            out_p[++out_index] = cur_byte = *in_p++;

            if (in_len ? cur_byte == (nxt_byte = *in_p) : true) {
                continue;
            }

            // Fall Through

        case 128:

            if (++out_index >= out_len) {
                throw ::Compression::CompressionException();
            } else if (in_len == 0) {
                continue;
            }

            run_code  = false;
            out_p[out_base = out_index] = 0;
            dup_count = run_count = 0;
            continue;
        }

        switch (run_count) {

        case 128:

            if (++out_index >= out_len) {
                throw ::Compression::CompressionException();
            }
            out_p[out_base = out_index] = 0;
            dup_count = run_count = 0;

            // Fall Through

        default :

            cur_byte = *in_p++;

            if (in_len > 0) {
                if (cur_byte == (nxt_byte = *in_p)) {
                    if (dup_count++ == 1) {
                        if (run_count >= dup_count) {
                            out_p[out_base] = static_cast<unsigned char>(run_count - dup_count);
                            out_base += run_count;
                        }
                        run_code  = true;
                        run_count = dup_count - 1;
                        dup_count = 0;
                        out_p[out_index = out_base] = static_cast<unsigned char>(run_count++ | 0x80);
                        break;
                    }
                } else dup_count = 0;
            }
            out_p[out_base] = char(run_count++);
            break;
        }

        if (++out_index >= out_len) {
            throw ::Compression::CompressionException();
        }

        out_p[out_index] = cur_byte;
    }

    target.length(++out_index);

    // Update statistics for this compressor
    this->update_stats(source.length(), target.length());
}

void
RLECompressor::decompress(const ::Compression::Buffer &source, ::Compression::Buffer &target)
{
    size_t  in_len  = static_cast<size_t>(source.length());
    size_t  max_len = static_cast<size_t>(target.length());

    size_t  out_len = 0;

    const unsigned char *in_p   = static_cast<const unsigned char*>(source.get_buffer());
    unsigned char *out_p        = static_cast<unsigned char*>(target.get_buffer());

    if (in_p && out_p) while(in_len--) {
        unsigned char cur_byte = *in_p++;
        size_t cpy_len = size_t((cur_byte & 0x7F) + 1U);

        if (cpy_len > max_len) {
            throw ::Compression::CompressionException();
        } else if (cur_byte & 0x80) {  // compressed
            if (in_len--) {
                ACE_OS::memset(out_p, *in_p++, cpy_len);
            } else {
                throw ::Compression::CompressionException();
            }
        } else if (in_len >= cpy_len) {
            ACE_OS::memcpy(out_p, in_p, cpy_len); in_p += cpy_len; in_len -= cpy_len;
        } else {
            throw ::Compression::CompressionException();
        }

        out_p   += cpy_len;
        max_len -= cpy_len;
        out_len += cpy_len;
    }

    target.length(static_cast<CORBA::ULong>(out_len));
}

}

TAO_END_VERSIONED_NAMESPACE_DECL
