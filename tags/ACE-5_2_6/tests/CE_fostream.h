// -*- C++ -*-

//=============================================================================
/**
 *  @file    CE_fostream.h
 *
 *  $Id$
 *
 *  @author Si Mong Park  <spark@ociweb.com>
 *
 *  This is a helper class to simulate basic functions of fostream for
 *  Windows CE since WinCE does not have IO stream.
 *
 *  Note that the numeric base conversion does not work and will be displayed
 *  as received.
 */
// ============================================================================

#ifndef ACE_CE_fostream_h
#define ACE_CE_fostream_h

#include "ace/OS.h"

#ifdef ACE_HAS_WINCE

#include "ace/Singleton.h"

#ifdef ostream
#undef ostream
#endif

#ifdef OFSTREAM
#undef OFSTREAM
#endif  // OFSTREAM

#define ostream  ACE_CE_fostream
#define OFSTREAM ACE_CE_fostream
#define dec  ACE_CE_fostream::dec
#define oct  ACE_CE_fostream::oct
#define hex  ACE_CE_fostream::hex
#define endl ACE_TEXT("\n")

class ACE_CE_fostream
{
public:
    friend class ACE_Singleton<ACE_CE_fostream, ACE_Null_Mutex>;

    enum NUM_TYPE {
        // These numbers are not really meaningful for this class; set same as defined in ios.
        // Also, base formatting works only for the integer types.
        dec = 0x0010,
        oct = 0x0020,
        hex = 0x0040
    };

    static ACE_CE_fostream* instance (void);

    FILE* open(const ACE_TCHAR *prog_name);

    ACE_CE_fostream& operator << (NUM_TYPE);

    ACE_CE_fostream& operator << (unsigned char);

    ACE_CE_fostream& operator << (const ACE_ANTI_TCHAR *);
    ACE_CE_fostream& operator << (ACE_ANTI_TCHAR);

    ACE_CE_fostream& operator << (const ACE_TCHAR*);
    ACE_CE_fostream& operator << (ACE_TCHAR);

    ACE_CE_fostream& operator << (short);

    ACE_CE_fostream& operator << (int);
    ACE_CE_fostream& operator << (unsigned int);

    ACE_CE_fostream& operator << (long);
    ACE_CE_fostream& operator << (unsigned long);
    ACE_CE_fostream& operator << (float);
    ACE_CE_fostream& operator << (double);
    ACE_CE_fostream& operator << (long double);

    ACE_CE_fostream& operator << (const void *);

private:
    ACE_CE_fostream();

    ~ACE_CE_fostream();

    /**
     * The ostream where logging messages can be written.
     */
    ACE_OSTREAM_TYPE *ostream_;

    /**
     * Currently set numeric base.
     */
    NUM_TYPE displayMode_;

    static ACE_CE_fostream* instance_;
};

typedef ACE_Singleton<ACE_CE_fostream, ACE_Null_Mutex> ACE_CE_OSTREAM;

#endif  // ACE_HAS_WINCE
#endif  // ACE_CE_fostream_h
