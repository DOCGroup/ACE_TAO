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

#include "Globals.h"

/* las.c */
int InitLiveAudio(int argc, char ** argv); /* -1 -- failed, 0 - succ */
void LeaveLiveAudio(void);
             /* each child forked not doing live audio should call this */
void ExitLiveAudio(void);
int OpenLiveAudio(AudioParameter * ap);  /* -1 -- failed, 0 - succ */
void StartPlayLiveAudio(void);
void StopPlayLiveAudio(void);
int ReadLiveAudioSamples(char * buf, int samples); /* ret # samples */

/* lvs.c */

int InitLiveVideo(int argc, char ** argv);  /* -1 -- failed, 0 - succ */
void LeaveLiveVideo(void);
             /* each child forked not doing live video should call this */
void ExitLiveVideo(void);
int OpenLiveVideo(int * format,
                  int * width, int * height,
                  int * averageFrameSize,
                  double * fps,
                  int * pelAspactRatio  /* MPEG1 encoding used here */
                  );   /* -1 -- failed, 0 - succ */
void StartPlayLiveVideo(void);
void StopPlayLiveVideo(void);
int ReadLiveVideoPicture(int * frame, char * buf, int size); /* ret # bytes */

/*from camera driver definitions */

int LiveVideoProcess(int argc, char ** argv);

/* as.c */
int AudioServer(int ctr_fd, int data_fd, int rttag, int max_pkt_size);

/* vs.c */
int VideoServer(int ctr_fd, int data_fd, int rttag, int max_pkt_size);
