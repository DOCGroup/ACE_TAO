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
/*
Some assumptions about the streams created in this file:

        PACKET STREAM:

                * send(), recv() may not work, we can only use
                  read()/write().

                * write(): either send the whole packet, or send nothing

                * read(): read() will read the minimun of the
                  requested number of bytes and current packet size.
                  read() can read part of a packet, but it
                  will not read cross packet boundaris

            There are two modes: non-discard and dicard. If a packets is
                  not read all its bytes, the remaining bytes will be
                  or not be discarded.

                  This is indicated by if (*max_pkt_size) is positive
                  (non-discard) or negative (discard)

        BYTE_STREAM: reliable

                * write(): might write only part of requested bytes
                  each time. (this is the case in SunOS4)

                * read(): might read less than requested bytes.
                  (this is the case in SunOS4).
*/

/* returns: -1 -- failed,  0 -- successful */
int ComInitClient(int inet_port, char * unix_port, char * atm_port);

void ComCloseClient(void);

/* address format: [ATM:|INET:]real_address | ""

   "" -- UNIX socket, reliable byte stream
   [INET:]real_address -- INET sockets, reliable control byte stream, and
                          *max_pkt_size == 0 : reliable data byte stream, or
                                        != 0 : unreliable packet stream;
   ATM:real_address -- ATM sockets, both data and control are
                       unreliable packet streams;
   DATM:real_address -- INET/UNIX control reliable byte stream
                        and ATM unreliable byte stream;

   returns: -1 -- failed,  0 -- successful
            *ctr_fd  -- control connection file descriptor
            *data_fd  -- data connection file descriptor
            *max_pkt_size -- == 0 : reliable BYTE_STREAM
                             > 0  : unreliable non-discard PACKET STREAM
                                    with given maximum packet size (bytes);
                             < 0  : unreliable discard PACKET STREAM
                                    with given maximum packet size (bytes);

          It is suggested that if a BYTE_STREAM is not known to be discarded,
          than it is safer to indicate it as discard.
   */
int ComOpenConnPair(char * address, int *ctr_fd, int *data_fd, int *max_pkt_size);
int VideoComOpenConnPair(char * address, int *ctr_fd, int *data_fd, int *max_pkt_size);


/* returns: -1 -- failed,  >=0 -- fd */
int ComOpenConn(char * address, int *max_pkt_size);

/* returns: -1 -- failed,  0 -- successful */
int ComCloseFd(int fd);

/* returns: -1 -- failed,  0 -- successful */
int ComCloseConn(int fd);

/* returns: -1 -- failed,  0 -- successful */
int ComInitServer(int inet_port, char * unix_port, char * atm_port);

void ComCloseListen(void);

void ComCloseServer(void);

/* returns: -1 -- failed,  0 -- successful
            *max_pkt_size is used only for returning value
*/
int ComGetConnPair(int *ctr_fd, int *data_fd, int *max_pkt_size);

/* returns: -1 -- failed,  >= 0 -- fd */
int ComGetConn(int *max_pkt_size);

