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
/* routine.c */
#ifndef _MPEG_ROUTINE_H
#define _MPEG_ROUTINE_H

#include "include/common.h"
#ifdef LINUX
#else
void usleep(unsigned int usec);
#endif
void beep(void);
int get_hostname(char *name, int len);
long get_usec(void);
long get_msec(void);
long get_sec(void);
long get_duration(long valstart, long valstop);
void remove_shmem_id(int id);
char * creat_shared_mem_id(int size, int * id);
char * creat_shared_mem(int size);
void remove_shared_mem(char *ptr);
void enter_cs(int semaphore_id);
void leave_cs(int semaphore_id);
int creat_semaphore(void);
void remove_semaphore(int sid);
void remove_all_semaphores (void);
int get_semval(int sid);
void get_full_path(char *filename, char * buffer, int bufsize);
void setsignal(int sig, void (func)(int));
int SetRTpriority(char *msg, int pri);

#endif /* _MPEG_ROUTINE_H */
