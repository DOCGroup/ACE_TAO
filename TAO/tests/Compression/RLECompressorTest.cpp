// $Id$

#include "ace/Log_Msg.h"
#include "ace/High_Res_Timer.h"
#include "ace/Compression/rle/RLECompressor.h"

#include "tao/ORB.h"
#include "tao/objectid.h"
#include "tao/Compression/Compression.h"
#include "tao/Compression/rle/RLECompressor_Factory.h"

#include <memory>

// Older versions of GCC do not support std::unique_ptr!
struct ACE_Byte_Array_ptr : std::auto_ptr<ACE_Byte> {
    explicit ACE_Byte_Array_ptr(ACE_Byte *_Ptr = 0)
        : std::auto_ptr<ACE_Byte>(_Ptr) {}
    ~ACE_Byte_Array_ptr(void) {
        delete [] (this->release());
    }
};

/* This test produces the following output (RLECompressor)

******** TEST RLE Compressor ************

****---> TEST256 (Compress/Expand Data)
Original (test256) Buffer - HEXDUMP 256 bytes
30 00 00 00 00 00 00 00  00 00 00 00 00 00 00 30   0..............0
00 31 00 00 00 00 00 00  00 00 00 00 00 00 31 00   .1............1.
00 00 32 00 00 00 00 00  00 00 00 00 00 32 00 00   ..2..........2..
00 00 00 33 00 00 00 00  00 00 00 00 33 00 00 00   ...3........3...
00 00 00 00 34 00 00 00  00 00 00 34 00 00 00 00   ....4......4....
00 00 00 00 00 35 00 00  00 00 35 00 00 00 00 00   .....5....5.....
00 00 00 00 00 00 36 00  00 36 00 00 00 00 00 00   ......6..6......
00 00 00 00 00 00 00 37  37 00 00 00 00 00 00 00   .......77.......
00 00 00 00 00 00 00 38  38 00 00 00 00 00 00 00   .......88.......
00 00 00 00 00 00 39 00  00 39 00 00 00 00 00 00   ......9..9......
00 00 00 00 00 41 00 00  00 00 41 00 00 00 00 00   .....A....A.....
00 00 00 00 42 00 00 00  00 00 00 42 00 00 00 00   ....B......B....
00 00 00 43 00 00 00 00  00 00 00 00 43 00 00 00   ...C........C...
00 00 44 00 00 00 00 00  00 00 00 00 00 44 00 00   ..D..........D..
00 45 00 00 00 00 00 00  00 00 00 00 00 00 45 00   .E............E.
46 00 00 00 00 00 00 00  00 00 00 00 00 00 00 46   F..............F
Compress Time -  total   0.000071 secs
Compressed Buffer - HEXDUMP 114 bytes
00 30 8d 00 02 30 00 31  8b 00 00 31 82 00 00 32   .0...0.1...1...2
89 00 00 32 84 00 00 33  87 00 00 33 86 00 00 34   ...2...3...3...4
85 00 00 34 88 00 00 35  83 00 00 35 8a 00 00 36   ...4...5...5...6
81 00 00 36 8c 00 81 37  8d 00 81 38 8c 00 00 39   ...6...7...8...9
81 00 00 39 8a 00 00 41  83 00 00 41 88 00 00 42   ...9...A...A...B
85 00 00 42 86 00 00 43  87 00 00 43 84 00 00 44   ...B...C...C...D
89 00 00 44 82 00 00 45  8b 00 02 45 00 46 8d 00   ...D...E...E.F..
00 46                                              .F
DeCompress Time -  total   0.000006 secs
****---> END TEST256 - OK!

****---> TESTArray (Compress/Expand on Progressive Boundaries)
Compression Data - HEXDUMP 16 bytes
41 42 42 43 43 43 44 44  45 46 47 48 49 4a 4a 4a   ABBCCCDDEFGHIJJJ
HEXDUMP 2 bytes
00 41                                              .A
HEXDUMP 3 bytes
01 41 42                                           .AB
HEXDUMP 4 bytes
00 41 81 42                                        .A.B
HEXDUMP 6 bytes
00 41 81 42 00 43                                  .A.B.C
HEXDUMP 6 bytes
00 41 81 42 81 43                                  .A.B.C
HEXDUMP 6 bytes
00 41 81 42 82 43                                  .A.B.C
HEXDUMP 8 bytes
00 41 81 42 82 43 00 44                            .A.B.C.D
HEXDUMP 8 bytes
00 41 81 42 82 43 81 44                            .A.B.C.D
HEXDUMP 10 bytes
00 41 81 42 82 43 81 44  00 45                     .A.B.C.D.E
HEXDUMP 11 bytes
00 41 81 42 82 43 81 44  01 45 46                  .A.B.C.D.EF
HEXDUMP 12 bytes
00 41 81 42 82 43 81 44  02 45 46 47               .A.B.C.D.EFG
HEXDUMP 13 bytes
00 41 81 42 82 43 81 44  03 45 46 47 48            .A.B.C.D.EFGH
HEXDUMP 14 bytes
00 41 81 42 82 43 81 44  04 45 46 47 48 49         .A.B.C.D.EFGHI
HEXDUMP 15 bytes
00 41 81 42 82 43 81 44  05 45 46 47 48 49 4a      .A.B.C.D.EFGHIJ
HEXDUMP 16 bytes
00 41 81 42 82 43 81 44  04 45 46 47 48 49 81 4a   .A.B.C.D.EFGHI.J
HEXDUMP 16 bytes
00 41 81 42 82 43 81 44  04 45 46 47 48 49 82 4a   .A.B.C.D.EFGHI.J
****---> END TESTArray - OK!

****---> TEST128 (Compress Boundary)
Original (test128) Buffer - HEXDUMP 128 bytes
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
Compress Time -  total   0.000019 secs
Compressed (test128) Buffer - HEXDUMP 2 bytes
ff 00                                              ..
DeCompress Time -  total   0.000004 secs
****---> END TEST128 - OK!

****---> TEST129A (Compress Boundary Extended)
Original (test129A) Buffer - HEXDUMP 129 bytes
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00                                                 .
Compress Time -  total   0.000010 secs
Compressed (test129A) Buffer - HEXDUMP 4 bytes
ff 00 00 00                                        ....
DeCompress Time -  total   0.000004 secs
****---> END TEST129A - OK!

****---> TEST129B (Compress Boundary NonExtend)
Original (test129B) Buffer - HEXDUMP 129 bytes
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00   ................
78                                                 x
Compress Time -  total   0.000030 secs
Compressed (test129B) Buffer - HEXDUMP 4 bytes
ff 00 00 78                                        ...x
DeCompress Time -  total   0.000005 secs
****---> END TEST129B - OK!

********* END TEST RLE ************
ACE::RLECompression Ratio = 0.352185 : 1
TAO::RLECompression Ratio = 0.352185 : 1

*/

// constants for Test256
const ACE_Byte test256_array[] = {
    '0',0,0,0,0,0,0,0,0,0,0,0,0,0,0,'0',
    0,'1',0,0,0,0,0,0,0,0,0,0,0,0,'1',0,
    0,0,'2',0,0,0,0,0,0,0,0,0,0,'2',0,0,
    0,0,0,'3',0,0,0,0,0,0,0,0,'3',0,0,0,
    0,0,0,0,'4',0,0,0,0,0,0,'4',0,0,0,0,
    0,0,0,0,0,'5',0,0,0,0,'5',0,0,0,0,0,
    0,0,0,0,0,0,'6',0,0,'6',0,0,0,0,0,0,
    0,0,0,0,0,0,0,'7','7',0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,'8','8',0,0,0,0,0,0,0,
    0,0,0,0,0,0,'9',0,0,'9',0,0,0,0,0,0,
    0,0,0,0,0,'A',0,0,0,0,'A',0,0,0,0,0,
    0,0,0,0,'B',0,0,0,0,0,0,'B',0,0,0,0,
    0,0,0,'C',0,0,0,0,0,0,0,0,'C',0,0,0,
    0,0,'D',0,0,0,0,0,0,0,0,0,0,'D',0,0,
    0,'E',0,0,0,0,0,0,0,0,0,0,0,0,'E',0,
    'F',0,0,0,0,0,0,0,0,0,0,0,0,0,0,'F'
};
const size_t BUFF_256_SIZE      = sizeof(test256_array);

// constants for Test128
const size_t BUFF_128_SIZE      = 128U;
const ACE_Byte comp_128_out[]   = {0xFF, 0x00};
// constants for Test129
const size_t BUFF_129_SIZE      = 129U;
const ACE_Byte comp_129B_char   = 'x';
const ACE_Byte comp_129A_out[]  = {0xFF, 0x00, 0x00, 0x00};
const ACE_Byte comp_129B_out[]  = {0xFF, 0x00, 0x00, comp_129B_char};

struct HRTimer : ACE_High_Res_Timer
{
    const ACE_TCHAR* msg_;
    HRTimer(const ACE_TCHAR* msg) : msg_(msg) {
        this->reset(); this->start();
    }
    ~HRTimer(void) {
        this->stop(); this->print_ave(this->msg_, 1);
    }
};

static int init_compressor(::CORBA::ORB_ptr orb)
{
    try {

        ::CORBA::Object_var compression_manager =
            orb->resolve_initial_references(TAO_OBJID_COMPRESSIONMANAGER);

        ::Compression::CompressionManager_var manager =
            ::Compression::CompressionManager::_narrow(compression_manager.in());

        if (::CORBA::is_nil(manager.in())) {
            ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("ERROR: nil compression manager.\n")),-1);
        }

        Compression::CompressorFactory_ptr compressor_factory;
        ACE_NEW_RETURN (compressor_factory, TAO::RLE_CompressorFactory (), 1);
        Compression::CompressorFactory_var compr_fact = compressor_factory;

        manager->register_factory (compr_fact.in ());

    } catch (...) {
        ACE_ERROR_RETURN((LM_ERROR,
            ACE_TEXT("ERROR: Unable to initialise RLE compressor manager.\n")),-1);
    }

    return 0;
}

static int  test256_compressor(const ::Compression::Compressor_var &compressor)
{
    ACE_DEBUG((LM_INFO, ACE_TEXT("\n****---> TEST256 (Compress/Expand Data)\n")));

    if (::CORBA::is_nil(compressor.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
            ACE_TEXT("ERROR: nil Compressor.\n")),-1);
    }

    ACE_HEX_DUMP((  LM_INFO,
                    reinterpret_cast<const char*>(test256_array),
                    BUFF_256_SIZE,
                    ACE_TEXT("Original (test256) Buffer")
                    ));

    ::CORBA::ULong  in_len = static_cast< CORBA::ULong>(sizeof(test256_array));
    const ::CORBA::Octet* in_buf = reinterpret_cast<const CORBA::Octet*>(test256_array);

    ::Compression::Buffer in_data(in_len, in_len, const_cast<CORBA::Octet*>(in_buf), false);

    ::Compression::Buffer comp_out;

    comp_out.length(static_cast< ::CORBA::ULong>(in_len * 1.1));

    {
      HRTimer _hrt(ACE_TEXT("Compress Time - "));
      compressor->compress(in_data, comp_out);
    }

    ACE_HEX_DUMP((  LM_INFO,
                    reinterpret_cast<const char*>(comp_out.get_buffer()),
                    comp_out.length(),
                    ACE_TEXT("Compressed Buffer")
                    ));

    ::Compression::Buffer decomp_out; decomp_out.length(in_len);

    {
        HRTimer _hrt(ACE_TEXT("DeCompress Time - "));
        compressor->decompress(comp_out, decomp_out);
    }

    // Make sure we got back the original OK.
    if (ACE_OS::memcmp( in_data.get_buffer(), decomp_out.get_buffer(), in_data.length()))
    {
        ACE_ERROR_RETURN((LM_ERROR,
            ACE_TEXT("ERROR: Did not Compress/Decompress (test256) correctly.\n")),-1);
    }

    ACE_ERROR_RETURN((LM_INFO, ACE_TEXT("****---> END TEST256 - OK!\n")), 0);
}

static int  testArray_compressor(const ::Compression::Compressor_var &compressor)
{
    ACE_DEBUG((LM_INFO, ACE_TEXT("\n****---> TESTArray (Compress/Expand on Progressive Boundaries)\n")));

    if (::CORBA::is_nil(compressor.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
            ACE_TEXT("ERROR: nil Compressor.\n")),-1);
    }

    struct COMP_RESULT {
        Compression::Buffer out; // We Compress to this buffer
        const int len;           // We Expect this compressed Length
        const ACE_Byte hex[16];  // We expect this compressed Data
    } comp_result[] = {
        /*00*/  { 0, 0, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
        /*01*/  { 2, 2, {0x00,0x41,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}, //".A"
        /*02*/  { 3, 3, {0x01,0x41,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}, //".AB"
        /*03*/  { 4, 4, {0x00,0x41,0x81,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}, //".A.B"
        /*04*/  { 6, 6, {0x00,0x41,0x81,0x42,0x00,0x43,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}, //".A.B.C"
        /*05*/  { 6, 6, {0x00,0x41,0x81,0x42,0x81,0x43,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}, //".A.B.C"
        /*06*/  { 6, 6, {0x00,0x41,0x81,0x42,0x82,0x43,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}, //".A.B.C"
        /*07*/  { 8, 8, {0x00,0x41,0x81,0x42,0x82,0x43,0x00,0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}, //".A.B.C.D"
        /*08*/  { 8, 8, {0x00,0x41,0x81,0x42,0x82,0x43,0x81,0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}, //".A.B.C.D"
        /*09*/  {10,10, {0x00,0x41,0x81,0x42,0x82,0x43,0x81,0x44,0x00,0x45,0x00,0x00,0x00,0x00,0x00,0x00}}, //".A.B.C.D.E"
        /*10*/  {11,11, {0x00,0x41,0x81,0x42,0x82,0x43,0x81,0x44,0x01,0x45,0x46,0x00,0x00,0x00,0x00,0x00}}, //".A.B.C.D.EF"
        /*11*/  {12,12, {0x00,0x41,0x81,0x42,0x82,0x43,0x81,0x44,0x02,0x45,0x46,0x47,0x00,0x00,0x00,0x00}}, //".A.B.C.D.EFG"
        /*12*/  {13,13, {0x00,0x41,0x81,0x42,0x82,0x43,0x81,0x44,0x03,0x45,0x46,0x47,0x48,0x00,0x00,0x00}}, //".A.B.C.D.EFGH"
        /*13*/  {14,14, {0x00,0x41,0x81,0x42,0x82,0x43,0x81,0x44,0x04,0x45,0x46,0x47,0x48,0x49,0x00,0x00}}, //".A.B.C.D.EFGHI"
        /*14*/  {15,15, {0x00,0x41,0x81,0x42,0x82,0x43,0x81,0x44,0x05,0x45,0x46,0x47,0x48,0x49,0x4A,0x00}}, //".A.B.C.D.EFGHIJ"
        /*15*/  {16,16, {0x00,0x41,0x81,0x42,0x82,0x43,0x81,0x44,0x04,0x45,0x46,0x47,0x48,0x49,0x81,0x4A}}, //".A.B.C.D.EFGHI.J"
        /*16*/  {16,16, {0x00,0x41,0x81,0x42,0x82,0x43,0x81,0x44,0x04,0x45,0x46,0x47,0x48,0x49,0x82,0x4A}}, //".A.B.C.D.EFGHI.J"
    };

    char comp_data[] = "ABBCCCDDEFGHIJJJ";  // Test String with compressor run switching

    ACE_HEX_DUMP((LM_INFO, comp_data, (sizeof(comp_data) - 1), ACE_TEXT("Compression Data")));

    for (unsigned i = 0; i < sizeof(comp_data); i++) {

        COMP_RESULT &result = comp_result[i]; // Get A reference to our result for testing output 

        Compression::Buffer in_data(i, i, reinterpret_cast<CORBA::Octet*>(comp_data), false), &out_data = result.out;

        compressor->compress(in_data, out_data);

        const char *out_ptr = reinterpret_cast<const char*>(out_data.get_buffer());
        int out_len         = int(out_data.length());

        if (out_len > 0) { // We expect out_len == 0 when i == 0.
            ACE_HEX_DUMP((LM_INFO, out_ptr, out_len, 0)); // Show the hex output
        } else if (out_len == result.len) { // Hopefully We expected out_len == 0
            continue;      // If So then just continue (nothing to check)
        }

        // Now Make sure all is good and we can also expand back to what we had
        if (out_len == result.len) {    // Expected result length?
            if (ACE_OS::memcmp(out_ptr, result.hex, out_len) == 0) { // Make sure its what we expected
                Compression::Buffer out_buf(i);     // Set maximum out space (exactly -> Ignored by TAO)
                compressor->decompress(out_data, out_buf);
                if (out_buf.length() == i) {        // Make sure we got our length back
                    if (ACE_OS::memcmp(comp_data, out_buf.get_buffer(), i) == 0) { 
                        continue; // It is what we expected.
                    }
                }
            }
        }

        ACE_ERROR_RETURN((LM_INFO, ACE_TEXT("****---> END TESTArray - TestLoop %d ERROR!\n"),i), -1);
    }

    ACE_ERROR_RETURN((LM_INFO, ACE_TEXT("****---> END TESTArray - OK!\n")), 0);
}

static int  test128_compressor(const ::Compression::Compressor_var &compressor)
{
    ACE_DEBUG((LM_INFO, ACE_TEXT("\n****---> TEST128 (Compress Boundary)\n")));

    if (::CORBA::is_nil(compressor.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
            ACE_TEXT("ERROR: nil Compressor.\n")),-1);
    }

    ACE_Byte_Array_ptr buff_128(new ACE_Byte[BUFF_128_SIZE]);

    ACE_OS::memset(buff_128.get(), 0, BUFF_128_SIZE);  // Set Buffer to zero.

    ACE_HEX_DUMP((  LM_INFO,
                    reinterpret_cast<const char*>(buff_128.get()),
                    BUFF_128_SIZE,
                    ACE_TEXT("Original (test128) Buffer")
                    ));

    ::CORBA::ULong  in_len = static_cast< CORBA::ULong>(BUFF_128_SIZE);
    ::CORBA::Octet* in_buf = reinterpret_cast<CORBA::Octet*>(buff_128.get());

    ::Compression::Buffer in_data(in_len, in_len, in_buf, false);

    ::Compression::Buffer comp_out;

    comp_out.length(static_cast< ::CORBA::ULong>(sizeof(comp_128_out)));  // SHould Compress to 2

    {
      HRTimer _hrt(ACE_TEXT("Compress Time - "));
      compressor->compress(in_data, comp_out);
    }

    ACE_HEX_DUMP((  LM_INFO,
                    reinterpret_cast<const char*>(comp_out.get_buffer()),
                    comp_out.length(),
                    ACE_TEXT("Compressed (test128) Buffer")
                    ));

    if (comp_out.length() != sizeof(comp_128_out)  // Size must equal
        || ACE_OS::memcmp(comp_out.get_buffer(), comp_128_out, comp_out.length())) 
    {
        ACE_ERROR_RETURN((LM_ERROR,
            ACE_TEXT("ERROR: Did not Compress/Decompress (test128) correctly.\n")),-1);
    }

    ::Compression::Buffer decomp_out; decomp_out.length(in_len);

    {
        HRTimer _hrt(ACE_TEXT("DeCompress Time - "));
        compressor->decompress(comp_out, decomp_out);
    }

    // Make sure we got back the original OK.
    if (ACE_OS::memcmp( in_data.get_buffer(), decomp_out.get_buffer(), in_data.length()))
    {
        ACE_ERROR_RETURN((LM_ERROR,
            ACE_TEXT("ERROR: Did not Compress/Decompress (test128) correctly.\n")),-1);
    }

    ACE_ERROR_RETURN((LM_INFO, ACE_TEXT("****---> END TEST128 - OK!\n")), 0);
}

static int  test129A_compressor(const ::Compression::Compressor_var &compressor)
{
    ACE_DEBUG((LM_INFO, ACE_TEXT("\n****---> TEST129A (Compress Boundary Extended)\n")));

    if (::CORBA::is_nil(compressor.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
            ACE_TEXT("ERROR: nil Compressor.\n")),-1);
    }

    const ACE_Byte_Array_ptr buff_129(new ACE_Byte[BUFF_129_SIZE]); 

    ACE_OS::memset(buff_129.get(), 0, BUFF_129_SIZE);  // Set Buffer to zero.

    ACE_HEX_DUMP((  LM_INFO,
                    reinterpret_cast<const char*>(buff_129.get()),
                    BUFF_129_SIZE,
                    ACE_TEXT("Original (test129A) Buffer")
                    ));

    ::CORBA::ULong  in_len = static_cast< CORBA::ULong>(BUFF_129_SIZE);
    ::CORBA::Octet* in_buf = reinterpret_cast< CORBA::Octet*>(buff_129.get());

    ::Compression::Buffer in_data(in_len, in_len, in_buf, false);

    ::Compression::Buffer comp_out;

    comp_out.length(static_cast< ::CORBA::ULong>(sizeof(comp_129A_out)));

    {
      HRTimer _hrt(ACE_TEXT("Compress Time - "));
      compressor->compress(in_data, comp_out);
    }

    ACE_HEX_DUMP((  LM_INFO,
                reinterpret_cast<const char*>(comp_out.get_buffer()),
                comp_out.length(),
                ACE_TEXT("Compressed (test129A) Buffer")
                ));

    if (comp_out.length() != sizeof(comp_129A_out)  // Size must equal
        || ACE_OS::memcmp(comp_out.get_buffer(), comp_129A_out, comp_out.length())) 
    {
        ACE_ERROR_RETURN((LM_ERROR,
            ACE_TEXT("ERROR: Did not Compress/Decompress (test129A) correctly.\n")),-1);
    }

    ::Compression::Buffer decomp_out; decomp_out.length(BUFF_129_SIZE);

    {
        HRTimer _hrt(ACE_TEXT("DeCompress Time - "));
        compressor->decompress(comp_out, decomp_out);
    }

    // Make sure we got back the original OK.
    if (ACE_OS::memcmp( in_data.get_buffer(), decomp_out.get_buffer(), in_data.length()))
    {
        ACE_ERROR_RETURN((LM_ERROR,
            ACE_TEXT("ERROR: Did not Compress/Decompress (test129A) correctly.\n")),-1);
    }

    ACE_ERROR_RETURN((LM_INFO, ACE_TEXT("****---> END TEST129A - OK!\n")), 0);
}

static int  test129B_compressor(const ::Compression::Compressor_var &compressor)
{
    ACE_DEBUG((LM_INFO, ACE_TEXT("\n****---> TEST129B (Compress Boundary NonExtend)\n")));

    if (::CORBA::is_nil(compressor.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
            ACE_TEXT("ERROR: nil Compressor.\n")),-1);
    }

    const ACE_Byte_Array_ptr buff_129(new ACE_Byte[BUFF_129_SIZE]); 

    ACE_OS::memset(buff_129.get(), 0, BUFF_129_SIZE);  // Set Buffer to zero.

    buff_129.get()[128] = comp_129B_char; // Add a non Repeat Character

    ACE_HEX_DUMP((  LM_INFO,
                    reinterpret_cast<const char*>(buff_129.get()),
                    BUFF_129_SIZE,
                    ACE_TEXT("Original (test129B) Buffer")
                    ));

    ::CORBA::ULong  in_len = static_cast< CORBA::ULong>(BUFF_129_SIZE);
    ::CORBA::Octet* in_buf = reinterpret_cast< CORBA::Octet*>(buff_129.get());

    ::Compression::Buffer in_data(in_len, in_len, in_buf, false);

    ::Compression::Buffer comp_out;

    comp_out.length(static_cast< ::CORBA::ULong>(sizeof(comp_129B_out)));

    {
      HRTimer _hrt(ACE_TEXT("Compress Time - "));
      compressor->compress(in_data, comp_out);
    }

    ACE_HEX_DUMP((  LM_INFO,
                reinterpret_cast<const char*>(comp_out.get_buffer()),
                comp_out.length(),
                ACE_TEXT("Compressed (test129B) Buffer")
                ));

    if (comp_out.length() != sizeof(comp_129B_out)  // Size must equal
        || ACE_OS::memcmp(comp_out.get_buffer(), comp_129B_out, comp_out.length())) 
    {
        ACE_ERROR_RETURN((LM_ERROR,
            ACE_TEXT("ERROR: Did not Compress/Decompress (test129B) correctly.\n")),-1);
    }

    ::Compression::Buffer decomp_out; decomp_out.length(BUFF_129_SIZE);

    {
        HRTimer _hrt(ACE_TEXT("DeCompress Time - "));
        compressor->decompress(comp_out, decomp_out);
    }

    // Make sure we got back the original OK.
    if (ACE_OS::memcmp( in_data.get_buffer(), decomp_out.get_buffer(), in_data.length()))
    {
        ACE_ERROR_RETURN((LM_ERROR,
            ACE_TEXT("ERROR: Did not Compress/Decompress (test129B) correctly.\n")),-1);
    }

    ACE_ERROR_RETURN((LM_INFO, ACE_TEXT("****---> END TEST129B - OK!\n")), 0);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
    try {

        ::CORBA::ORB_var orb = ::CORBA::ORB_init(argc, argv);

        if (init_compressor(orb.in ()) != 0) {
            ACE_ERROR_RETURN((LM_ERROR,
                ACE_TEXT("ERROR: Unable to initialise RLE compressor.\n")),-1);
        }

        ::CORBA::Object_var compression_manager =
                orb->resolve_initial_references(TAO_OBJID_COMPRESSIONMANAGER);

        ::Compression::CompressionManager_var manager =
            ::Compression::CompressionManager::_narrow(compression_manager.in());

        if (::CORBA::is_nil(manager.in())) {
            ACE_ERROR_RETURN((LM_ERROR,
                ACE_TEXT("ERROR: nil compression manager.\n")),-1);
        }

        ACE_DEBUG((LM_INFO, ACE_TEXT("\n******** TEST RLE Compressor ************\n")));

        ::Compression::Compressor_var rleCompressor = manager->get_compressor(::Compression::COMPRESSORID_RLE, 0);

        if (::CORBA::is_nil(rleCompressor.in())) {
            ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("ERROR: Unable to locate TAO::RLECompressor - exiting.\n")),-1);
        } else if (test256_compressor(rleCompressor) == -1) {
            ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("ERROR: TAO::RLECompressor reported an error (test256) - exiting.\n")),-1);
        } else if (testArray_compressor(rleCompressor) == -1) {
            ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("ERROR: TAO::RLECompressor reported an error (testArray) - exiting.\n")),-1);
        } else if (test128_compressor(rleCompressor) == -1) {
            ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("ERROR: TAO::RLECompressor reported an error (test128) - exiting.\n")),-1);
        } else if (test129A_compressor(rleCompressor) == -1) {
            ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("ERROR: TAO::RLECompressor reported an error (test129A) - exiting.\n")),-1);
        } else if (test129B_compressor(rleCompressor) == -1) {
            ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("ERROR: TAO::RLECompressor reported an error (test129B) - exiting.\n")),-1);
        }

        ACE_DEBUG((LM_INFO, ACE_TEXT("\n********* END TEST RLE ************\n")));

        float ace_ratio = ACE_RLECompression::instance()->compression_ratio();
        ACE_DEBUG((LM_INFO, ACE_TEXT("ACE::RLECompression Ratio = %f : 1\n"), ace_ratio));
        float tao_ratio = rleCompressor->compression_ratio();
        ACE_DEBUG((LM_INFO, ACE_TEXT("TAO::RLECompression Ratio = %f : 1\n"), tao_ratio));

        orb->shutdown ();

    } catch (const CORBA::Exception& ex) {
        ex._tao_print_exception ("RLECompressorTest: UNEXPECTED exception - ");
        return -1;
    }

    return 0;
}

