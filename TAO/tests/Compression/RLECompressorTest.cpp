// $Id$

#include "ace/Log_Msg.h"
#include "ace/High_Res_Timer.h"

#include "tao/ORB.h"
#include "tao/objectid.h"
#include "tao/Compression/Compression.h"
#include "tao/Compression/rle/RLECompressor_Factory.h"

/* This test produces the following output (RLECompressor)

    Original Buffer - HEXDUMP 256 bytes
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
    Compressed Buffer - HEXDUMP 114 bytes
    00 30 8d 00 02 30 00 31  8b 00 00 31 82 00 00 32   .0...0.1...1...2
    89 00 00 32 84 00 00 33  87 00 00 33 86 00 00 34   ...2...3...3...4
    85 00 00 34 88 00 00 35  83 00 00 35 8a 00 03 36   ...4...5...5...6
    00 00 36 8c 00 01 37 37  8d 00 01 38 38 8c 00 03   ..6...77...88...
    39 00 00 39 8a 00 00 41  83 00 00 41 88 00 00 42   9..9...A...A...B
    85 00 00 42 86 00 00 43  87 00 00 43 84 00 00 44   ...B...C...C...D
    89 00 00 44 82 00 00 45  8b 00 02 45 00 46 8d 00   ...D...E...E.F..
    00 46                                              .F
    Decompressed Buffer - HEXDUMP 256 bytes
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
    RLE Compression Ratio = 2.245614 : 1 - Blocking for 10 Seconds.

*/

static char test_array[] = {
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

struct HRTimer : ACE_High_Res_Timer
{
    std::string msg_;
    HRTimer(const std::string &msg) : msg_(msg) {
        this->reset(); this->start();
    }
    ~HRTimer(void) {
        this->stop(); this->print_ave(this->msg_.c_str(), 1);
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

        manager->register_factory(::Compression::CompressorFactory_var(new TAO::RLE_CompressorFactory()));

    } catch (...) {
        ACE_ERROR_RETURN((LM_ERROR,
            ACE_TEXT("ERROR: Unable to initialise RLE compressor manager.\n")),-1);
    }

    return 0;
}

static int  test_compressor(const ::Compression::Compressor_var &compressor)
{
    if (::CORBA::is_nil(compressor.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
            ACE_TEXT("ERROR: nil Compressor.\n")),-1);
    }

    ::CORBA::ULong  in_len = static_cast< ::CORBA::ULong>(sizeof(test_array));
    ::CORBA::Octet* in_buf = reinterpret_cast< ::CORBA::Octet*>(test_array);

    ::Compression::Buffer in_data(in_len, in_len, in_buf, false);

    ::Compression::Buffer comp_out;

    comp_out.length(static_cast< ::CORBA::ULong>(in_len * 1.1));

    {
        HRTimer _hrt("Compress Time - "); compressor->compress(in_data, comp_out);
    }

    Compression::CompressionRatio ratio = compressor->compression_ratio();

    ACE_HEX_DUMP((  LM_INFO,
                    reinterpret_cast<const char*>(comp_out.get_buffer()),
                    comp_out.length(),
                    ACE_TEXT("Compressed Buffer")
                    ));

    ::Compression::Buffer decomp_out; decomp_out.length(1024);

    {
        HRTimer _hrt("DeCompress Time - "); compressor->decompress(comp_out, decomp_out);
    }

    // Make sure we got back the original OK.
    if (ACE_OS::memcmp( in_data.get_buffer(), decomp_out.get_buffer(), in_data.length()))
    {
        ACE_ERROR_RETURN((LM_ERROR,
            ACE_TEXT("ERROR: Did not Compress/Decompress correctly.\n")),-1);
    }

    ACE_ERROR_RETURN((LM_INFO, ACE_TEXT("Compression Ratio = %f : 1\n\n"), ratio), 0);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
    try {

        ::CORBA::ORB_var orb = ::CORBA::ORB_init(argc, argv);

        if (init_compressor(orb) != 0) {
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

        ACE_HEX_DUMP((LM_INFO,
                        test_array,
                        sizeof(test_array),
                        ACE_TEXT("Original Buffer")
                        ));


        ACE_DEBUG((LM_INFO, ACE_TEXT("\n******** TEST RLE Compressor ************\n")));
        test_compressor(::Compression::Compressor_var(manager->get_compressor(::Compression::COMPRESSORID_RLE, 0)));

        ACE_DEBUG((LM_INFO, ACE_TEXT("\n********* END TEST - Block for 10 Seconds ************\n")));

        orb->shutdown ();

    } catch (const CORBA::Exception& ex) {
        ex._tao_print_exception ("RLECompressorTest: UNEXPECTED exception - ");
        return -1;
    }

    return 0;
}
