/* This may look like C, but it's really -*- C++ -*- */
#ifndef TAO_COMPAT_INITGUID_H
#  define TAO_COMPAT_INITGUID_H

/* Insure proper inclusion order */
#ifndef DEFINE_GUID
#error initguid: must include objbase.h first.
#endif

/* assert DEFINE_GUID is defined */
#undef	DEFINE_GUID

#define DEFINE_GUID(name,b,c,d,e,f,g,h,i,j,k,l) EXTERN_C const IID name = { 0 }

#endif /* TAO_COMPAT_INITGUID_H */
