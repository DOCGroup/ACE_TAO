/* $Id$ */

/* Copyright (c) 1995 Oregon Graduate Institute of Science and Technology
 * P.O.Box 91000-1000, Portland, OR 97291, USA;
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its 
 * documentation for any purpose is hereby granted without fee, provided that 
 * the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of O.G.I. not be used in advertising or 
 * publicity pertaining to distribution of the software without specific, 
 * written prior permission.  O.G.I. makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty.
 * 
 * O.G.I. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL 
 * O.G.I. BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN 
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF 
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Shanwei Cen
 *         Department of Computer Science and Engineering
 *         email: scen@cse.ogi.edu
 */

#include "ace/ACE.h"

ACE_RCSID(mpeg_client, info, "$Id$")

char infomessage[] = "\
\n\
\n\
\n\
TAO Audio/Video Streaming service\n\
---------------------------------\n\
\n\
This is a distributed audio/video streaming application that uses\n\
interfaces and interactions defined in the OMG \"Control and Management\n\
of A/V streams\" spec. An implementation of the spec can be found in\n\
the following directory.\n\
\n\
TAO/orbsvcs/orbsvcs/AV/\n\
\n\
The TAO application is a modified version of a streaming application\n\
developed at the Oregon Graduate Institute.\n\
\n\
\n\
\n\
The player features common VCR functionalities like synchronized\n\
video(MPEG) and audio(.au) play back, play speed set on the fly,\n\
fast-forward, rewind, step forward, random Positioning, and more.\n\
\n\
The player is of server/client architecture. The server manages program\n\
retrieval and distribution. The client decodes video frames, plays video\n\
and audio streams, and provides a Motif style user interface.\n\
\n\
\n\
The basic client/server used by the application originated from:\n\
	  Distributed Systems Research Group\n\
      Department of Computer Science and Engineering\n\
    Oregon Graduate Institute of Science and Technology\n\
                       October, 1995\n\
The MPEG decoder used by the client is originated from U.C.Berkeley\n\
MPEG decoder 2.0, by Lawrence A. Rowe, Ketan Patel, and Brian Smith,\n\
of Computer Science Division-EECS, Univ. of Calif. at Berkeley.\n\
\n\
The Motif user interface is based on an implementation by Daeron Meyer\n\
of The Geometry Center, University of Minnesota.\n\
\n\
This software is covered by copyrights. It contains code contributed\n\
by the authors and several other parties. Please see the beginning of\n\
source files and copyright file(s) in the root directory of the source\n\
kit.\n\
\n\
Sumedh Mungee <sumedh@cs.wustl.edu>\n\
Nagarajan Surendran <naga@cs.wustl.edu>\n\
Seth Widoff <sbw1@cs.wustl.edu>\n\
Alexander Arulanthu <alex@cs.wustl.edu>\n\
Distributed Object Computing Group\n\
Washington University\n\
\n\
\n\
";
