#include "RLECompressor.h"

#include <cstring>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_RLECompressor::ACE_RLECompressor ()
  : ACE_Compressor (ACE_COMPRESSORID_RLE)
{
}

// Compress using Run Length Encoding (RLE)
ACE_UINT64
ACE_RLECompressor::compress (void const *in_ptr,
                             ACE_UINT64 in_len,
                             void *out_ptr,
                             ACE_UINT64 max_out_len)
{
  ACE_Byte const *in_p {static_cast<ACE_Byte const *> (in_ptr)};
  ACE_Byte *out_p {static_cast<ACE_Byte *> (out_ptr)};

  ACE_UINT64 const src_len {in_len};
  ACE_UINT64 out_index {};
  ACE_UINT64 out_base {};
  size_t run_count {};
  bool run_code {};

  if (in_p != nullptr && out_p != nullptr && in_len != 0)
    {
      while (in_len > 0)
        {
          --in_len;
          ACE_Byte const cur_byte {*in_p};
          ++in_p;

          switch (out_index ? run_count : 128U)
            {
            case 128:
              out_base = out_index;
              if (out_base >= max_out_len)
                return static_cast<ACE_UINT64> (-1);
              ++out_index;
              run_code = false;
              run_count = 0;
              // Fall through.

            default:
              if (in_len ? cur_byte == *in_p : run_code)
                {
                  if (run_code)
                    {
                      out_p[out_base] = static_cast<ACE_Byte> (run_count | 0x80);
                      ++run_count;
                      continue;
                    }
                  else if (run_count)
                    {
                      out_base = out_index;
                      if (out_base >= max_out_len)
                        return static_cast<ACE_UINT64> (-1);
                      ++out_index;
                      run_count = 0;
                    }
                  run_code = true;
                }
              else if (run_code)
                {
                  out_p[out_base] = static_cast<ACE_Byte> (run_count | 0x80);
                  if (in_len)
                    {
                      out_base = out_index;
                      if (out_base >= max_out_len)
                        return static_cast<ACE_UINT64> (-1);
                      ++out_index;
                    }
                  run_code = false;
                  run_count = 0;
                  continue;
                }

              break;
            }

          out_p[out_base] = static_cast<ACE_Byte> (run_count | (run_code ? 0x80 : 0));
          ++run_count;

          if (out_index >= max_out_len)
            return static_cast<ACE_UINT64> (-1);
          out_p[out_index] = cur_byte;
          ++out_index;
        }
      this->update_stats (src_len, out_index);
    }

  return out_index;
}

// Decompress using Run Length Encoding (RLE)
ACE_UINT64
ACE_RLECompressor::decompress (void const *in_ptr,
                               ACE_UINT64 in_len,
                               void *out_ptr,
                               ACE_UINT64 max_out_len)
{
  ACE_UINT64 out_len {};

  ACE_Byte const *in_p {static_cast<ACE_Byte const *> (in_ptr)};
  ACE_Byte *out_p {static_cast<ACE_Byte *> (out_ptr)};

  if (in_p != nullptr && out_p != nullptr)
    {
      while (in_len > 0)
        {
          --in_len;
          ACE_Byte const cur_byte {*in_p};
          ++in_p;
          ACE_UINT32 const cpy_len {static_cast<ACE_UINT32> ((cur_byte & ACE_CHAR_MAX) + 1)};

          if (cpy_len > max_out_len)
            return static_cast<ACE_UINT64> (-1);
          else if ((cur_byte & 0x80) != 0)
            {
              if (in_len == 0)
                return static_cast<ACE_UINT64> (-1);
              --in_len;
              std::memset (out_p, *in_p, cpy_len);
              ++in_p;
            }
          else if (in_len >= cpy_len)
            {
              std::memcpy (out_p, in_p, cpy_len);
              in_p += cpy_len;
              in_len -= cpy_len;
            }
          else
            return static_cast<ACE_UINT64> (-1);

          out_p += cpy_len;
          max_out_len -= cpy_len;
          out_len += cpy_len;
        }
      }

  return out_len;
}

ACE_SINGLETON_TEMPLATE_INSTANTIATE(ACE_Singleton, ACE_RLECompressor, ACE_SYNCH_MUTEX);

// Close versioned namespace, if enabled by the user.
ACE_END_VERSIONED_NAMESPACE_DECL

