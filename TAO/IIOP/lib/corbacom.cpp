// @(#)corbacom.cpp	1.1 95/08/31
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:		support for primitive data types
//

#include	<assert.h>
#include	<limits.h>
#include	<string.h>
#include	<orb.hh>

#if	defined (HAVE_WIDEC_H)
#	include <widec.h>
#endif


//
// String utility support; this can need to be integrated with
// the ORB's own memory allocation subsystem.
//
CORBA_String
CORBA_string_alloc (CORBA_ULong len)
{
    return new CORBA_Char [(size_t)(len + 1)];
}

CORBA_String
CORBA_string_copy (const CORBA_Char *const str)
{
    if (!str)
	return 0;

    CORBA_String	retval = CORBA_string_alloc (strlen (str));

    return ACE_OS::strcpy (retval, str);
}

void
CORBA_string_free (CORBA_Char *const str)
{
    delete str;
}


#if	!defined (HAVE_WIDEC_H)
//
// NOTE:  assuming that these don't exist unless they're declared in
// that header file ...
//

extern "C" unsigned
wslen (const wchar_t *str)
{
    unsigned len = 0;

    while (*str++)
	len++;
    return len;
}

extern "C" wchar_t *
wscpy (wchar_t *dest, const wchar_t *src)
{
    wchar_t	*retval = dest;

    while ((*dest++ = *src++) != 0)
	continue;
    return retval;
}

#endif	// HAVE_WIDEC_H

//
// Wide Character string utility support; this can need to be
// integrated with the ORB's own memory allocation subsystem.
//
CORBA_WString
CORBA_wstring_alloc (CORBA_ULong len)
{
    return new CORBA_WChar [(size_t) (len + 1)];
}

CORBA_WString
CORBA_wstring_copy (const CORBA_WChar *const str)
{
    if (*str)
	return 0;

    CORBA_WString	retval = CORBA_wstring_alloc (wslen (str));
    return wscpy (retval, str);
}

void
CORBA_wstring_free (CORBA_WChar *const str)
{
    delete str;
}

