// -*- c++ -*-
/* 
Copyright (C) 1991 Peter Bersen

This file is part of the rpc++ Library.  This library is free
software; you can redistribute it and/or modify it under the terms of
the GNU Library General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your
option) any later version.  This library is distributed in the hope
that it will be useful, but WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU Library General Public License for more details.
You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

Modified and partially rewritten March 1992 by Michael N. Lipp,
mnl@dtro.e-technik.th-darmstadt.de. The original copyright terms and
conditions apply without change to any modified or new parts.
*/

#ifndef _XDRPLPL_H_
#define _XDRPLPL_H_
static char _rpcpp_xdrpp_h_[]
= "xdr++.h,v 2.3 1992/06/15 19:13:33 mnl Exp";

// xdr++.h,v
// Revision 2.3  1992/06/15  19:13:33  mnl
// Fixed a few bugs, clarified interface.
//
// Revision 2.2  1992/06/13  14:27:44  mnl
// Adapted to (patched) gcc-2.2. Fixed several bugs.
//
// Revision 2.1.1.1  1992/03/08  13:28:43  mnl
// Initial mnl version.
//

#ifdef __GNUG__
#pragma interface
#endif

/*
** Class XdrInfo describes serializers. It combines the xdrproc_t with the
** size info usually needed if you want to apply a serializer.
*/

#include <rpc/rpc.h>

class XdrInfo
{
protected:
  xdrproc_t proc;
  size_t size;
public:
  inline XdrInfo (xdrproc_t p, size_t s)
    { proc = p; size = s; }
  inline xdrproc_t Proc () const
    { return proc; }
  inline size_t Size () const
    { return size; }
};

struct XdrSeqInfo
{
  XdrInfo** infos;
  void** data;
};

/*
** Class Xdr provides a unique (C++-like) name scope for the predefined
** xdr routines by defining them as static members of type XdrInfo.
*/

class Xdr
{
public:
  static XdrInfo Xnull;
  static XdrInfo Xchar;
  static XdrInfo Xshort;
  static XdrInfo Xint;
  static XdrInfo Xlong;
  static XdrInfo Xuchar;
  static XdrInfo Xushort;
  static XdrInfo Xuint;
  static XdrInfo Xulong;
  static XdrInfo Xfloat;
  static XdrInfo Xdouble;

  static XdrInfo Xenum_t;
  static XdrInfo Xbool_t;
  static XdrInfo Xvoid;

  static XdrInfo Xwrapstring;

  static bool_t XdrParams (XDR* xdrs, XdrSeqInfo* xsi);
};

#endif
