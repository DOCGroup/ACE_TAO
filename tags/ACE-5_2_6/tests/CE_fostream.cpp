// $Id$

#include "CE_fostream.h"


ACE_CE_fostream* ACE_CE_fostream::instance_ = 0;


ACE_CE_fostream* ACE_CE_fostream::instance (void)
{
    if (instance_ == 0) {
        instance_ = new ACE_CE_fostream();
    }

    return instance_;
}


ACE_CE_fostream::ACE_CE_fostream()
: ostream_(0)
, displayMode_(dec)
{
}


ACE_CE_fostream::~ACE_CE_fostream()
{
    fclose(ostream_);
}


FILE* ACE_CE_fostream::open(const ACE_TCHAR *prog_name)
{
    ostream_ = _wfopen(prog_name, ACE_TEXT("a+"));

    return ostream_;
}


ACE_CE_fostream& ACE_CE_fostream::operator << (NUM_TYPE num_type)
{
    displayMode_ = num_type;
    return *this;
}


ACE_CE_fostream& ACE_CE_fostream::operator << (const ACE_ANTI_TCHAR* c)
{
    fprintf(ostream_, "%s", c);
    return *this;
}


ACE_CE_fostream& ACE_CE_fostream::operator << (ACE_ANTI_TCHAR c)
{
    fprintf(ostream_, "%c", c);
    return *this;
}


ACE_CE_fostream& ACE_CE_fostream::operator << (const ACE_TCHAR* c)
{
    fwprintf(ostream_, ACE_TEXT("%s"), c);
    return *this;
}


ACE_CE_fostream& ACE_CE_fostream::operator << (ACE_TCHAR c)
{
    fwprintf(ostream_, ACE_TEXT("%c"), c);
    return *this;
}


ACE_CE_fostream& ACE_CE_fostream::operator << (short s)
{
    if (displayMode_ == oct) {
        const NUM_BITS = ACE_SIZEOF_SHORT * 8;
        short currentMax = 16384;

        for (int i = 0; i < NUM_BITS; ++i) {
        }
    }
    else if (displayMode_ == hex) {
    }

    fwprintf(ostream_, ACE_TEXT("%d"), s);
    return *this;
}


ACE_CE_fostream& ACE_CE_fostream::operator << (int i)
{
    fwprintf(ostream_, ACE_TEXT("%d"), i);
    return *this;
}


ACE_CE_fostream& ACE_CE_fostream::operator << (unsigned int i)
{
    fwprintf(ostream_, ACE_TEXT("%d"), i);
    return *this;
}


ACE_CE_fostream& ACE_CE_fostream::operator << (long l)
{
    fwprintf(ostream_, ACE_TEXT("%f"), l);
    return *this;
}


ACE_CE_fostream& ACE_CE_fostream::operator << (unsigned long l)
{
    fwprintf(ostream_, ACE_TEXT("%f"), l);
    return *this;
}


ACE_CE_fostream& ACE_CE_fostream::operator << (float f)
{
    fwprintf(ostream_, ACE_TEXT("%f"), f);
    return *this;
}


ACE_CE_fostream& ACE_CE_fostream::operator << (double d)
{
    fwprintf(ostream_, ACE_TEXT("%f"), d);
    return *this;
}


ACE_CE_fostream& ACE_CE_fostream::operator << (long double d)
{
    fwprintf(ostream_, ACE_TEXT("%f"), d);
    return *this;
}


ACE_CE_fostream& ACE_CE_fostream::operator << (const void* v)
{
    fwprintf(ostream_, ACE_TEXT("%d"), v);
    return *this;
}
