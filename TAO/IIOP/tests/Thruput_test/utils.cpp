#include "ttcp_decl.h"

int SetSocketBufferSize(int size, unsigned short isTrans)
{
   // go thru open file descriptors and check if its is a socket and
   // if so, set its socket size to "size"
   struct rlimit rlm;
   memset(&rlm, 0, sizeof(struct rlimit));
   (void) getrlimit(RLIMIT_NOFILE, &rlm);
   cout << "Total file descriptors = " << rlm.rlim_max << endl;
   for (int i=3; i < rlm.rlim_max; i++){
      if (isTrans)
      	(void) setsockopt(i, SOL_SOCKET, SO_SNDBUF,
		(char *) &size, sizeof(size));
      else
      	(void) setsockopt(i, SOL_SOCKET, SO_RCVBUF,
		(char *) &size, sizeof(size));
   }
   return 1;
}

void
err (char *s)
{
  fprintf (stderr, "ttcp%s: ", trans ? "-t" : "-r");
  perror (s);
  fprintf (stderr, "errno=%d\n", errno);
  exit (1);
}

void
mes (char *s)
{
  fprintf (stderr, "ttcp%s: %s\n", trans ? "-t" : "-r", s);
}

char *
outfmt (double b)
{
  static char obuf[50];
  switch (fmt)
    {
    case 'G':
      sprintf (obuf, "%.2f GB", b / 1024.0 / 1024.0 / 1024.0);
      break;
    default:
    case 'K':
      sprintf (obuf, "%.2f KB", b / 1024.0);
      break;
    case 'M':
      sprintf (obuf, "%.2f MB", b / 1024.0 / 1024.0);
      break;
    case 'g':
      sprintf (obuf, "%.2f Gbit", b * 8.0 / 1024.0 / 1024.0 / 1024.0);
      break;
    case 'k':
      sprintf (obuf, "%.2f Kbit", b * 8.0 / 1024.0);
      break;
    case 'm':
      sprintf (obuf, "%.2f Mbit", b * 8.0 / 1024.0 / 1024.0);
      break;
    }
  return obuf;
}

static struct itimerval itime0;	/* Time at which timing started */
static struct rusage ru0;	/* Resource utilization at the start */

#if defined(SYSV)
/*ARGSUSED */
static void
getrusage (int ignored, register struct rusage *ru)
{
  struct tms buf;

  times (&buf);

  /* Assumption: HZ <= 2147 (LONG_MAX/1000000) */
  ru->ru_stime.tv_sec = buf.tms_stime / HZ;
  ru->ru_stime.tv_usec = ((buf.tms_stime % HZ) * 1000000) / HZ;
  ru->ru_utime.tv_sec = buf.tms_utime / HZ;
  ru->ru_utime.tv_usec = ((buf.tms_utime % HZ) * 1000000) / HZ;
}

/*ARGSUSED */
static void
gettimeofday (struct timeval *tp, struct timezone *zp)
{
  tp->tv_sec = time (0);
  tp->tv_usec = 0;
}
#endif /* SYSV */
/*
 *                    P R E P _ T I M E R
 */
void
prep_timer ()
{
  itime0.it_interval.tv_sec = 0;
  itime0.it_interval.tv_usec = 0;
  itime0.it_value.tv_sec = LONG_MAX / 22;	/* greatest possible value , itimer() count backwards */
  itime0.it_value.tv_usec = 0;


  getrusage (RUSAGE_SELF, &ru0);

  /* Init REAL Timer */
  if (setitimer (ITIMER_REAL, &itime0, NULL))
    {
      perror ("Setting 'itimer' REAL failed");
      return;
    }

}

/*
 *                    R E A D _ T I M E R
 * 
 */
double
read_timer (char *str, int len)
{
  struct itimerval itimedol;
  struct rusage ru1;
  struct timeval td;
  struct timeval tend, tstart;
  char line[132];

  getrusage (RUSAGE_SELF, &ru1);

  if (getitimer (ITIMER_REAL, &itimedol))
    {
      perror ("Getting 'itimer' REAL failed");
      return (0.0);
    }

  prusage (&ru0, &ru1, &itime0.it_value, &itimedol.it_value, line);
  (void) strncpy (str, line, len);

  /* Get real time */
  tvsub (&td, &itime0.it_value, &itimedol.it_value);
  realt = td.tv_sec + ((double) td.tv_usec) / 1000000;

  /* Get CPU time (user+sys) */
  tvadd (&tend, &ru1.ru_utime, &ru1.ru_stime);
  tvadd (&tstart, &ru0.ru_utime, &ru0.ru_stime);
  tvsub (&td, &tend, &tstart);
  cput = td.tv_sec + ((double) td.tv_usec) / 1000000;
  if (cput < 0.00001)
    cput = 0.00001;
  return (cput);
}

static void
prusage (register struct rusage *r0, struct rusage *r1, 
	 struct timeval *e, struct timeval *b, char *outp)
{
  struct timeval tdiff;
  register time_t t;
  register char *cp;
  register int i;
  int ms;

  t = (r1->ru_utime.tv_sec - r0->ru_utime.tv_sec) * 100 +
    (r1->ru_utime.tv_usec - r0->ru_utime.tv_usec) / 10000 +
    (r1->ru_stime.tv_sec - r0->ru_stime.tv_sec) * 100 +
    (r1->ru_stime.tv_usec - r0->ru_stime.tv_usec) / 10000;
  ms = (e->tv_sec - b->tv_sec) * 100 + (e->tv_usec - b->tv_usec) / 10000;

#define END(x)  {while(*x) x++;}
#if defined(SYSV)
  cp = "%Uuser %Ssys %Ereal %P";
#else
#if defined(sgi)		/* IRIX 3.3 will show 0 for %M,%F,%R,%C */
  cp = "%Uuser %Ssys %Ereal %P %Mmaxrss %F+%Rpf %Ccsw";
#else
  cp = "%Uuser %Ssys %Ereal %P %Xi+%Dd %Mmaxrss %F+%Rpf %Ccsw";
#endif
#endif
  for (; *cp; cp++)
    {
      if (*cp != '%')
	*outp++ = *cp;
      else if (cp[1])
	switch (*++cp)
	  {

	  case 'U':
	    tvsub (&tdiff, &r1->ru_utime, &r0->ru_utime);
	    sprintf (outp, "%d.%01d", tdiff.tv_sec, tdiff.tv_usec / 100000);
	    END (outp);
	    break;

	  case 'S':
	    tvsub (&tdiff, &r1->ru_stime, &r0->ru_stime);
	    sprintf (outp, "%d.%01d", tdiff.tv_sec, tdiff.tv_usec / 100000);
	    END (outp);
	    break;

	  case 'E':
	    psecs (ms / 100, outp);
	    END (outp);
	    break;

	  case 'P':
	    sprintf (outp, "%d%%", (int) (t * 100 / ((ms ? ms : 1))));
	    END (outp);
	    break;

#if !defined(SYSV)
	  case 'W':
	    i = r1->ru_nswap - r0->ru_nswap;
	    sprintf (outp, "%d", i);
	    END (outp);
	    break;

	  case 'X':
	    sprintf (outp, "%d", t == 0 ? 0 : (r1->ru_ixrss - r0->ru_ixrss) / t);
	    END (outp);
	    break;

	  case 'D':
	    sprintf (outp, "%d", t == 0 ? 0 :
		     (r1->ru_idrss + r1->ru_isrss - (r0->ru_idrss + r0->ru_isrss)) / t);
	    END (outp);
	    break;

	  case 'K':
	    sprintf (outp, "%d", t == 0 ? 0 :
		     ((r1->ru_ixrss + r1->ru_isrss + r1->ru_idrss) -
		      (r0->ru_ixrss + r0->ru_idrss + r0->ru_isrss)) / t);
	    END (outp);
	    break;

	  case 'M':
	    sprintf (outp, "%d", r1->ru_maxrss / 2);
	    END (outp);
	    break;

	  case 'F':
	    sprintf (outp, "%d", r1->ru_majflt - r0->ru_majflt);
	    END (outp);
	    break;

	  case 'R':
	    sprintf (outp, "%d", r1->ru_minflt - r0->ru_minflt);
	    END (outp);
	    break;

	  case 'I':
	    sprintf (outp, "%d", r1->ru_inblock - r0->ru_inblock);
	    END (outp);
	    break;

	  case 'O':
	    sprintf (outp, "%d", r1->ru_oublock - r0->ru_oublock);
	    END (outp);
	    break;
	  case 'C':
	    sprintf (outp, "%d+%d", r1->ru_nvcsw - r0->ru_nvcsw,
		     r1->ru_nivcsw - r0->ru_nivcsw);
	    END (outp);
	    break;
#endif /* !SYSV */
	  }
    }
  *outp = '\0';
}

static void
tvadd (struct timeval *tsum, struct timeval *t0, struct timeval *t1)
{

  tsum->tv_sec = t0->tv_sec + t1->tv_sec;
  tsum->tv_usec = t0->tv_usec + t1->tv_usec;
  if (tsum->tv_usec > 1000000)
    tsum->tv_sec++, tsum->tv_usec -= 1000000;
}

static void
tvsub (struct timeval *tdiff, struct timeval *t1, struct timeval *t0)
{

  tdiff->tv_sec = t1->tv_sec - t0->tv_sec;
  tdiff->tv_usec = t1->tv_usec - t0->tv_usec;
  if (tdiff->tv_usec < 0)
    tdiff->tv_sec--, tdiff->tv_usec += 1000000;
}

static void
psecs (long l, register char *cp)
{
  register int i;

  i = l / 3600;
  if (i)
    {
      sprintf (cp, "%d:", i);
      END (cp);
      i = l % 3600;
      sprintf (cp, "%d%d", (i / 60) / 10, (i / 60) % 10);
      END (cp);
    }
  else
    {
      i = l;
      sprintf (cp, "%d", i / 60);
      END (cp);
    }
  i %= 60;
  *cp++ = ':';
  sprintf (cp, "%d%d", i / 10, i % 10);
}

/*
 *                    N R E A D
 */
int
Nread (int fd, void *buf, int count)
{
  struct sockaddr_in from;
  int len = sizeof (from);
  register int cnt;
  if (udp)
    {
      cnt = recvfrom (fd, (char *) buf, count, 0, (struct sockaddr *) &from, &len);
      numCalls++;
    }
  else
    {
      if (b_flag)
	cnt = mread (fd, (char *) buf, count);	/* fill buf */
      else
	{
	  cnt = read (fd, buf, count);
	  numCalls++;
	}
      if (touchdata && cnt > 0)
	{
	  register int c = cnt, sum;
	  register char *b = (char *) buf;
	  while (c--)
	    sum += *b++;
	}
    }
  return (cnt);
}


/*
 *                    N W R I T E
 */
int
Nwrite (int fd, void *buf, int count)
{
  return 0;
}

void
delay (int us)
{
  struct timeval tv;

  tv.tv_sec = 0;
  tv.tv_usec = us;
  (void) select (1, (fd_set *) 0, (fd_set *) 0, (fd_set *) 0, &tv);
}

/*
 *                    M R E A D
 *
 * This function performs the function of a read(II) but will
 * call read(II) multiple times in order to get the requested
 * number of characters.  This can be necessary because
 * network connections don't deliver data with the same
 * grouping as it is written with.  Written by Robert S. Miles, BRL.
 */
int
mread (int fd, register char *bufp, unsigned n)
{
  register unsigned count = 0;
  register int nread;

  do
    {
      nread = read (fd, bufp, n - count);
      numCalls++;
      if (nread < 0)
	{
	  perror ("ttcp_mread");
	  return (-1);
	}
      if (nread == 0)
	return ((int) count);
      count += (unsigned) nread;
      bufp += nread;
    }
  while (count < n);

  return ((int) count);
}

void
FillPattern (register char *cp, register int bufLen, unsigned long dt)
{
  unsigned long
        num, i;

  switch(dt){
  case SEND_SHORT:
    {
       register short *SeqPtr = (short *)cp;
       num = bufLen/sizeof(short);
       for (i=0; i < num; i++)
		SeqPtr[i] = (short)lrand48(); 
       sseq = new ttcp_sequence::ShortSeq(num,num, SeqPtr);
    } 
    break;
  case SEND_LONG:
    {
       register long *SeqPtr = (long *)cp;
       num = bufLen/sizeof(long);
       for (i=0; i < num; i++)
		SeqPtr[i] = lrand48(); 
       lseq = new ttcp_sequence::LongSeq(num, num, SeqPtr);
    } 
    break;
  case SEND_DOUBLE:
    {
       register double *SeqPtr = (double *)cp;
       num = bufLen/sizeof(double);
       for (i=0; i < num; i++)
		SeqPtr[i] = drand48(); 
       dseq = new ttcp_sequence::DoubleSeq(num, num, SeqPtr);
    } 
    break;
  case SEND_CHAR:
    {
       register CORBA_Char *SeqPtr = (CORBA_Char *)cp;
       register char c = 0;
       num = bufLen/sizeof(char);
       for(i=0; i < num; i++){
         while (!isprint(c & 0x7f))
                c++;
         SeqPtr[i] = (c++ & 0x7f);
       }
       cseq = new ttcp_sequence::CharSeq(num, num, SeqPtr);
    } 
    break;
  case SEND_STRUCT:
    {
       register PerfStruct *SeqPtr = (PerfStruct *)cp;
       register char c = 0;
       num = bufLen/sizeof(PerfStruct);
       for (i=0; i < num; i++){
           SeqPtr[i].s = (short)lrand48();
           SeqPtr[i].l = lrand48();
           SeqPtr[i].d = drand48();
           while (!isprint(c & 0x7f))
                c++;
           SeqPtr[i].c = (c++ & 0x7f);
           while (!isprint(c & 0x7f))
                c++;
           SeqPtr[i].o = (unsigned char)(c++ & 0x7f);
       }
       Sseq = new ttcp_sequence::StructSeq(num, num, SeqPtr);
 
    } 
    break;
  case SEND_OCTET:
  default:
    {
       register CORBA_Octet *SeqPtr = (CORBA_Octet *)cp;
       register char c = 0;
       num = bufLen/sizeof(CORBA_Octet);
       for(i=0; i < num; i++){
         while (!isprint(c & 0x7f))
                c++;
         SeqPtr[i] = (c++ & 0x7f);
       }
       oseq = new ttcp_sequence::OctetSeq(num, num, SeqPtr);
    }
    break;
  }
}
void PrintStats()
{
  if (cput <= 0.0)
    cput = 0.001;
  if (realt <= 0.0)
    realt = 0.001;

//#if defined (LM_RESULTS)
  if (trans && (title != 0))
    {
      double tmp;
      FILE *outFile;
      char filename[BUFSIZ];
 
      strcpy(filename, title);
      switch(dt){
      case SEND_SHORT:
        strcat(filename, ".shortSeq.results");
        break;
      case SEND_LONG:
        strcat(filename, ".longSeq.results");
        break;
      case SEND_DOUBLE:
        strcat(filename, ".doubleSeq.results");
        break;
      case SEND_CHAR:
        strcat(filename, ".charSeq.results");
        break;
      case SEND_STRUCT:
        strcat(filename, ".structSeq.results");
        break;
      case SEND_COMPOSITE:
        strcat(filename, ".compositeSeq.results");
        break;
      case SEND_OCTET:
      default:
        strcat(filename, ".octetSeq.results");
        break;
      }
      outFile = fopen (filename, "a+");
      if (new_line)
        fprintf (outFile, "\n  -l %ldk \t", buflen / 1024);
      tmp = ((double) nbytes) / realt;
      fprintf (outFile, "%.2f ", tmp * 8.0 / 1024.0 / 1024.0);
      fclose (outFile);
    }
//#endif

  fprintf (stdout,
           "ttcp%s: %ld bytes in %.2f real seconds = %s/sec +++\n",
           trans ? "-t" : "-r",
           nbytes, realt, outfmt (((double) nbytes) / realt));
  if (verbose)
    {
      fprintf (stdout,
               "ttcp%s: %ld bytes in %.2f CPU seconds = %s/cpu sec\n",
               trans ? "-t" : "-r",
               nbytes, cput, outfmt (((double) nbytes) / cput));
    }
  fprintf (stdout,
           "ttcp%s: %d Server Method calls, msec/call = %.2f, calls/sec = %.2f\n",
           trans ? "-t" : "-r",
           numCalls,
           1024.0 * realt / ((double) numCalls),
           ((double) numCalls) / realt);
  fprintf (stdout, "ttcp%s: %s\n", trans ? "-t" : "-r", stats);
  if (verbose)
    {
      fprintf (stdout,
               "ttcp%s: buffer address %#x\n",
               trans ? "-t" : "-r",
               buf);
    }
}
