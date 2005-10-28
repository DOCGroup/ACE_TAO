// $Id$

// Checks to make sure that wide-char conversion work

#include "test_config.h"
#include "ace/config-all.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

ACE_RCSID(tests, WChar_Conversion, "$Id$")

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("WChar_Conversion"));

// Const Input

  // Pass through where constructed with destination type
  {
    const char* source = "Source";
    const char* result = 0;
    {
      ACE::String_Conversion::Convert_In< char, wchar_t > convert( source );
      result = convert.c_str();
    }
    ACE_ASSERT( result == source );
  }

  // Pass through where constructed with destination type
  {
    const wchar_t* source = L"Source";
    const wchar_t* result = 0;
    {
      ACE::String_Conversion::Convert_In< wchar_t, char > convert( source );
      result = convert.c_str();
    }
    ACE_ASSERT( result == source );
  }

  // Narrow to wide to narrow
  {
    const char* source = "Source";
    const wchar_t* result1 = 0;
    const char* result2 = 0;

    ACE::String_Conversion::Convert_In< wchar_t, char > convert1( source );
    result1 = convert1.c_str();

    ACE::String_Conversion::Convert_In< char, wchar_t > convert2( result1 );
    result2 = convert2.c_str();

    ACE_ASSERT( ACE_OS::strcmp( result2, source ) == 0 );
  }

  // Wide to narrow to wide
  {
    const wchar_t* source = L"Source";
    const char* result1 = 0;
    const wchar_t* result2 = 0;

    ACE::String_Conversion::Convert_In< char, wchar_t > convert1( source );
    result1 = convert1.c_str();
 
    ACE::String_Conversion::Convert_In< wchar_t, char > convert2( result1 );
    result2 = convert2.c_str();

    ACE_ASSERT( ACE_OS::strcmp( result2, source ) == 0 );
  }

// In/Out

  // Pass through where constructed with destination type
  {
    char source[ 255 ] = "Source";
    char* result = 0;
    {
      ACE::String_Conversion::Convert_InOut< char, wchar_t > convert( source, 255 );
      result = convert.c_str();
    }
    ACE_ASSERT( result == source );
  }

  // Pass through where constructed with destination type
  {
    wchar_t source[ 255 ] = L"Source";
    wchar_t* result = 0;
    {
      ACE::String_Conversion::Convert_InOut< wchar_t, char > convert( source, 255 );
      result = convert.c_str();
    }
    ACE_ASSERT( result == source );
  }

  // Narrow to wide
  {
    char source[ 255 ] = "Source";
    {
      ACE::String_Conversion::Convert_InOut< wchar_t, char > convert( source, 255 );
      wchar_t* result = convert.c_str();
      result[0] = L'T';
    }
    ACE_ASSERT( ACE_OS::strcmp( "Tource", source ) == 0 );
  }

  // Wide to narrow
  {
    wchar_t source[ 255 ] = L"Source";
    {
      ACE::String_Conversion::Convert_InOut< char, wchar_t > convert( source, 255 );
      char* result = convert.c_str();
      result[0] = 'T';
    }
    ACE_ASSERT( ACE_OS::strcmp( L"Tource", source ) == 0 );
  }

// Output

  // char to char
  {
    char source[ 255 ] = "Source";
    ACE::String_Conversion::Convert_Out< char > convert( source );
    char* result = convert.c_str();
    ACE_ASSERT( ACE_OS::strcmp( result, source ) == 0 );
    convert.free( result );
  }

  // char to wchar_t
  {
    char source[ 255 ] = "Source";
    ACE::String_Conversion::Convert_Out< wchar_t > convert( source );
    wchar_t* result = convert.c_str();
    ACE_ASSERT( ACE_OS::strcmp( result, ACE_TEXT_TO_WCHAR_IN( source ) ) == 0 );
    convert.free( result );
  }

  // wchar_t to char
  {
    wchar_t source[ 255 ] = L"Source";
    ACE::String_Conversion::Convert_Out< char > convert( source );
    char* result = convert.c_str();
    ACE_ASSERT( ACE_OS::strcmp( result, ACE_TEXT_TO_CHAR_IN( source ) ) == 0 );
    convert.free( result );
  }

  // wchar_t to wchar_t
  {
    wchar_t source[ 255 ] = L"Source";
    ACE::String_Conversion::Convert_Out< wchar_t > convert( source );
    wchar_t* result = convert.c_str();
    ACE_ASSERT( ACE_OS::strcmp( result, source ) == 0 );
    convert.free( result );
  }

// In place

  // char to char
  {
    const char source[ 255 ] = "Source";
    char dest[ 255 ] = "";
    ACE::String_Conversion::copy( dest, source, sizeof(source)/sizeof(source[0])-1 );
    ACE_ASSERT( ACE_OS::strcmp( "Source", dest ) == 0 );
  }

  // char to wchar_t
  {
    const char source[ 255 ] = "Source";
    wchar_t dest[ 255 ] = L"";
    ACE::String_Conversion::copy( dest, source, sizeof(source)/sizeof(source[0])-1 );
    ACE_ASSERT( ACE_OS::strcmp( L"Source", dest ) == 0 );
  }

  // wchar_t to char
  {
    const wchar_t source[ 255 ] = L"Source";
    char dest[ 255 ] = "";
    ACE::String_Conversion::copy( dest, source, sizeof(source)/sizeof(source[0])-1 );
    ACE_ASSERT( ACE_OS::strcmp( "Source", dest ) == 0 );
  }

  // wchar_t to wchar_t
  {
    const wchar_t source[ 255 ] = L"Source";
    wchar_t dest[ 255 ] = L"";
    ACE::String_Conversion::copy( dest, source, sizeof(source)/sizeof(source[0])-1 );
    ACE_ASSERT( ACE_OS::strcmp( L"Source", dest ) == 0 );
  }

// Boundary

  // NULL strings
  {
    const wchar_t* nullW = 0;
    ACE_ASSERT( ACE_TEXT_TO_CHAR_IN( nullW ) == 0 );
    ACE_ASSERT( ACE_TEXT_TO_WCHAR_IN( nullW ) == 0 );
    const char* nullN = 0;
    ACE_ASSERT( ACE_TEXT_TO_WCHAR_IN( nullN ) == 0 );
    ACE_ASSERT( ACE_TEXT_TO_CHAR_IN( nullN ) == 0 );
  }

  // Empty strings
  {
    const wchar_t* nullW = L"";
    ACE_ASSERT( ACE_OS::strcmp( ACE_TEXT_TO_CHAR_IN( nullW ), "" ) == 0 );
    ACE_ASSERT( ACE_OS::strcmp( ACE_TEXT_TO_WCHAR_IN( nullW ), L"" ) == 0 );
    const char* nullN = "";
    ACE_ASSERT( ACE_OS::strcmp( ACE_TEXT_TO_WCHAR_IN( nullN ), L"" ) == 0 );
    ACE_ASSERT( ACE_OS::strcmp( ACE_TEXT_TO_CHAR_IN( nullN ), "" ) == 0 );
  }

// Other In/Out

  // Limited Narrow to wide
  {
    char source[ 255 ] = "Source";
    {
      ACE::String_Conversion::Convert_InOut< wchar_t, char > convert( source, 3 );
      wchar_t* result = convert.c_str();
      result[0] = L'T';
    }
    ACE_ASSERT( ACE_OS::strcmp( "To", source ) == 0 );
  }

  // Limited Wide to narrow
  {
    wchar_t source[ 255 ] = L"Source";
    {
      ACE::String_Conversion::Convert_InOut< char, wchar_t > convert( source, 3 );
      char* result = convert.c_str();
      result[0] = 'T';
    }
    ACE_ASSERT( ACE_OS::strcmp( L"To", source ) == 0 );
  }

  // Narrow to wide
  {
    char source[ 255 ] = "Source";
    {
      ACE::String_Conversion::Convert_InOut< wchar_t, char > convert( source ); // calc len
      wchar_t* result = convert.c_str();
      result[0] = L'T';
    }
    ACE_ASSERT( ACE_OS::strcmp( "Tource", source ) == 0 );
  }

  // Wide to narrow
  {
    wchar_t source[ 255 ] = L"Source";
    {
      ACE::String_Conversion::Convert_InOut< char, wchar_t > convert( source ); // calc len
      char* result = convert.c_str();
      result[0] = 'T';
    }
    ACE_ASSERT( ACE_OS::strcmp( L"Tource", source ) == 0 );
  }

  ACE_END_TEST;
  return 0;
}
