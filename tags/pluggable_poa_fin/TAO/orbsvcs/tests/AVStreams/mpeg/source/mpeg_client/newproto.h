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
/* ui.c */
void UIprocess(int cmdSocket);

#include "mpeg_shared/routine.h"

/* ctr.c */
int CTRmain(int argc,char **argv);

/* ab.c */
void ABinitBuf(int size);
void ABflushBuf(int nextSample);
int ABcheckSamples(void);  /* returns # of samples in ABbuf */
int ABgetSamples(char * buf, int samples);
    /* read at most given number of samples from AB to buf, returns
       number of sample actually read */
int ABskipSamples(int samples);
/* if samples < 0; then stuff |samples| silient samples to ABgetSamples(),
   otherwise wipe out this number of samples from AB */
void ABdeleteBuf(void);
void ABdeleteSem(void);

void ABprocess(int dataSocket);

/* vb.c */
void VBinitBuf(int size);
char * VBgetBuf(int size);  /* block version */
int VBcheckBuf(int size); /* non-block check, return True/False*/
void VBputMsg(char * msgPtr);
char * VBgetMsg();          /* block version */
int  VBcheckMsg();        /* non-block check, return Number of Msgs in buffer */
int VBbufEmpty(void);
void VBreclaimMsg(char * msgPtr);
void VBdeleteBuf(void);
void VBdeleteSem(void);

void VBprocess(int initSocket, int dataSocket);

/* vd.c */
void VDprocess(int CTRpid);
void VDinitBuf(int size);
void VDresizeBuf(int height, int width);
FrameBlock * VDgetBuf(void);     /* block version, return with interrupt */
int   VDcheckBuf(void);    /* non-block check, return True/False */
void VDputMsg(FrameBlock * msgPtr);
FrameBlock * VDgetMsg(void);     /* block version, return with interrupt */
FrameBlock * VDpeekMsg(void);    /* returns a msg for checking only, no modification allowed */
int VDcheckMsg(void);      /* non-block check, return Number of Msg in buffer */
int VDbufEmpty(void);
void VDreferMsg(FrameBlock * msgPtr);
void VDreclaimMsg(FrameBlock * msgPtr);
void VDdeleteBuf(void);
void VDdeleteSem(void);

/* vp.c */

void VPinitWindow(Widget shell, Window monitorWindow, int cmdSocket);
void VPresizeWindow(int height, int width);

/* -- in gdith.c -- */
void VPcommand(int cmd, char * position);
void VPexpose(void);

/* audio.c */
int InitAudioDevice(void);
void SetAudioParameter(AudioParameter * para);
unsigned int GetAudioTime(void);
unsigned int PlayAudioSamples(unsigned int time, char * buf, int size);
void StartAudioPlaySession(void);
void SetAudioGain(void);
void AudioInterpolate(char *rawBuf, int len, char * convBuf, int convLen);

/* prog.c */

void StartProgram(char * title, char * vh, char * vf, char * ah, char * af);
Widget CreateProgramWindow(Widget parent, int cmdSoket, int * playflag, Widget frametext);

/* file.c */

void StartLocalServer(void);
Widget CreateFileWindow(Widget parent, int cmdSoket, int * playflag, Widget frametext);


/* para.c */

Widget CreateParameterWindow(Widget parent);
