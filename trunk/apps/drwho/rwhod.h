/* -*- C++ -*- */
// $Id$

/* The /usr/include/protocols/rwhod.h header breaks cfront 2.0... */

#ifndef _RWHOD_H
#define _RWHOD_H

struct outmp 
{
  char	out_line[8];		/* tty name */
  char	out_name[8];		/* user id */
  long	out_time;		/* time on */
};

struct whoent 
{
  outmp we_utmp;		/* active tty info */
  int	we_idle;		/* tty idle time */
};

struct whod 
{
  char		wd_vers;		/* protocol version # */
  char		wd_type;		/* packet type, see below */
  char		wd_pad[2];
  int		wd_sendtime;		/* time stamp by sender */
  int		wd_recvtime;		/* time stamp applied by receiver */
  char		wd_hostname[32];	/* hosts's name */
  int		wd_loadav[3];		/* load average as in uptime */
  int		wd_boottime;		/* time system booted */
  whoent	wd_we[1024 / sizeof (struct whoent)];
};

#define	WHODVERSION	1
#define	WHODTYPE_STATUS	1		/* host status */

#endif /* _RWHOD_H */
