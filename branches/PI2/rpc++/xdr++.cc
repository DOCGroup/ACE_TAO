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

static char _rpcpp_xdrpp_cc_[]
= "xdr++.cc,v 2.3 1992/06/15 19:12:49 mnl Exp";

// xdr++.cc,v
// Revision 2.3  1992/06/15  19:12:49  mnl
// Fixed a few bugs, clarified interface.
//
// Revision 2.2  1992/06/13  14:27:06  mnl
// Adapted to (patched) gcc-2.2. Fixed several bugs.
//
// Revision 2.1.1.1  1992/03/08  13:28:42  mnl
// Initial mnl version.
//

#ifdef __GNUG__
#pragma implementation
#endif

/*
** See Xdr.h.
*/

#include "rpc++/xdr++.h"

XdrInfo Xdr::Xnull ((xdrproc_t)0, 0);
XdrInfo Xdr::Xchar ((xdrproc_t)xdr_char, sizeof (char));
XdrInfo Xdr::Xshort ((xdrproc_t)xdr_short, sizeof (short));
XdrInfo Xdr::Xint ((xdrproc_t)xdr_int, sizeof (int));
XdrInfo Xdr::Xlong ((xdrproc_t)xdr_long, sizeof (long));
XdrInfo Xdr::Xuchar ((xdrproc_t)xdr_u_char, sizeof (u_char));
XdrInfo Xdr::Xushort ((xdrproc_t)xdr_u_short, sizeof (u_short));
XdrInfo Xdr::Xuint ((xdrproc_t)xdr_u_int, sizeof (u_int));
XdrInfo Xdr::Xulong ((xdrproc_t)xdr_u_long, sizeof (u_long));
XdrInfo Xdr::Xfloat ((xdrproc_t)xdr_float, sizeof (float));
XdrInfo Xdr::Xdouble ((xdrproc_t)xdr_double, sizeof (double));

XdrInfo Xdr::Xenum_t ((xdrproc_t)xdr_enum, sizeof (enum_t));
XdrInfo Xdr::Xbool_t ((xdrproc_t)xdr_bool, sizeof (bool_t));
XdrInfo Xdr::Xvoid ((xdrproc_t)xdr_void, 0);
XdrInfo Xdr::Xwrapstring ((xdrproc_t)xdr_wrapstring, sizeof (char*));

bool_t Xdr::XdrParams (XDR* xdrs, XdrSeqInfo* xsi)
{
  XdrInfo** infop = xsi->infos;
  void** datap = xsi->data;

  while (*infop)
    {
      if (! (*infop->Proc ()) (xdrs, *datap))
	return FALSE;
      infop++, datap++;
    }
  return TRUE;
}
