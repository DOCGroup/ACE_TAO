// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Wchar_IBM1047_ISO8859_Translator.cpp
//
// = DESCRIPTION
//    Defines the arrays required to convert between ISO8859 (aka
//    Latin/1) and IBM1047 (aka EBCDIC).
//
// = AUTHOR
//    Mahesh Vedantam (mahesh@ociweb.com)
//
// ============================================================================

#include "Wchar_IBM1047_ISO8859_Translator.h"

// ****************************************************************

WIBM1047_ISO8859::WIBM1047_ISO8859 (void)
{

}

WIBM1047_ISO8859::~WIBM1047_ISO8859 (void)
{

}

// = Documented in $ACE_ROOT/ace/CDR_Stream.h
ACE_CDR::Boolean WIBM1047_ISO8859::read_wchar (ACE_InputCDR &,
                                               ACE_CDR::WChar &)
{
  return 0;
}

ACE_CDR::Boolean WIBM1047_ISO8859::read_wstring (ACE_InputCDR &,
                                                 ACE_CDR::WChar *&)
{
  return 0;
}

ACE_CDR::Boolean WIBM1047_ISO8859::read_wchar_array (ACE_InputCDR &,
                                                     ACE_CDR::WChar *,
                                                     ACE_CDR::ULong)
{
  return 0;
}

ACE_CDR::Boolean WIBM1047_ISO8859::write_wchar (ACE_OutputCDR &,
                                       ACE_CDR::WChar)
{
  return 0;
}
ACE_CDR::Boolean WIBM1047_ISO8859::write_wstring (ACE_OutputCDR &,
                                         ACE_CDR::ULong,
                                         const ACE_CDR::WChar *)
{
  return 0;
}
ACE_CDR::Boolean WIBM1047_ISO8859::write_wchar_array (ACE_OutputCDR &,
                                             const ACE_CDR::WChar *,
                                             ACE_CDR::ULong)
{
  return 0;
}

