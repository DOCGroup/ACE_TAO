// $Id$

// ============================================================================
//
// = TAO tests
//    Throughput measurement using the TTCP benchmark adapted to work using TAO
// 
// = FILENAME
//   utils.cpp
//
// = AUTHOR
//    Aniruddha Gokhale
// 
// ============================================================================

// This file has all the helper functions that do the computation of
// throughput, system time used, user time, etc based on data collected.

#include "ttcp.H"
#include "ttcp_decl.h"

ACE_RCSID(COOL, utils, "$Id$")

// the error function. 
// displays the error message and exits
int err (char * s)
{
  ACE_OS::fprintf (stderr, "ttcp%s: ", trans ? "-t" : "-r");
  ACE_OS::perror (s);
  ACE_OS::fprintf (stderr, "errno=%d\n", errno);
  return -1;
}

// prints a message indicating if it is a transmitter or a receiver
void mes (CORBA::String s)
{
  ACE_OS::fprintf (stderr, "ttcp%s: %s\n", trans ? "-t" : "-r", s);
}

// does the formatting for the desired units in which the result is to be
// displayed 
CORBA::String
outfmt (CORBA::Double b)
{
  static char obuf[50];
  switch (fmt)
    {
    case 'G':
      ACE_OS::sprintf (obuf, "%.2f GB", b / 1024.0 / 1024.0 / 1024.0);
      break;
    default:
    case 'K':
      ACE_OS::sprintf (obuf, "%.2f KB", b / 1024.0);
      break;
    case 'M':
      ACE_OS::sprintf (obuf, "%.2f MB", b / 1024.0 / 1024.0);
      break;
    case 'g':
      ACE_OS::sprintf (obuf, "%.2f Gbit", b * 8.0 / 1024.0 / 1024.0 / 1024.0);
      break;
    case 'k':
      ACE_OS::sprintf (obuf, "%.2f Kbit", b * 8.0 / 1024.0);
      break;
    case 'm':
      ACE_OS::sprintf (obuf, "%.2f Mbit", b * 8.0 / 1024.0 / 1024.0);
      break;
    }
  return obuf;
}

static struct itimerval itime0;	/* Time at which timing started */
static struct rusage ru0;	/* Resource utilization at the start */

/*
 *                    P R E P _ T I M E R
 */
// this is in fact the internals of the "start_timer" operation
void
prep_timer (void)
{
  itime0.it_interval.tv_sec = 0;
  itime0.it_interval.tv_usec = 0;
  itime0.it_value.tv_sec = LONG_MAX / 22;	/* greatest possible value , itimer() count backwards */
  itime0.it_value.tv_usec = 0;


  ACE_OS::getrusage (RUSAGE_SELF, &ru0);

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
// This implements the internals of the "stop_timer" method
double
read_timer (char *str, CORBA::Long len)
{
  struct itimerval itimedol;
  struct rusage ru1;
  struct timeval td;
  struct timeval tend, tstart;
  char line[132];

  ACE_OS::getrusage (RUSAGE_SELF, &ru1);

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

// prints the rusage stats
void
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
	    ACE_OS::sprintf (outp, "%d.%01d", tdiff.tv_sec, tdiff.tv_usec / 100000);
	    END (outp);
	    break;

	  case 'S':
	    tvsub (&tdiff, &r1->ru_stime, &r0->ru_stime);
	    ACE_OS::sprintf (outp, "%d.%01d", tdiff.tv_sec, tdiff.tv_usec / 100000);
	    END (outp);
	    break;

	  case 'E':
	    psecs (ms / 100, outp);
	    END (outp);
	    break;

	  case 'P':
	    ACE_OS::sprintf (outp, "%d%%", (int) (t * 100 / ((ms ? ms : 1))));
	    END (outp);
	    break;

#if !defined(SYSV)
	  case 'W':
	    i = r1->ru_nswap - r0->ru_nswap;
	    ACE_OS::sprintf (outp, "%d", i);
	    END (outp);
	    break;

	  case 'X':
	    ACE_OS::sprintf (outp, "%d", t == 0 ? 0 : (r1->ru_ixrss - r0->ru_ixrss) / t);
	    END (outp);
	    break;

	  case 'D':
	    ACE_OS::sprintf (outp, "%d", t == 0 ? 0 :
		     (r1->ru_idrss + r1->ru_isrss - (r0->ru_idrss + r0->ru_isrss)) / t);
	    END (outp);
	    break;

	  case 'K':
	    ACE_OS::sprintf (outp, "%d", t == 0 ? 0 :
		     ((r1->ru_ixrss + r1->ru_isrss + r1->ru_idrss) -
		      (r0->ru_ixrss + r0->ru_idrss + r0->ru_isrss)) / t);
	    END (outp);
	    break;

	  case 'M':
	    ACE_OS::sprintf (outp, "%d", r1->ru_maxrss / 2);
	    END (outp);
	    break;

	  case 'F':
	    ACE_OS::sprintf (outp, "%d", r1->ru_majflt - r0->ru_majflt);
	    END (outp);
	    break;

	  case 'R':
	    ACE_OS::sprintf (outp, "%d", r1->ru_minflt - r0->ru_minflt);
	    END (outp);
	    break;

	  case 'I':
	    ACE_OS::sprintf (outp, "%d", r1->ru_inblock - r0->ru_inblock);
	    END (outp);
	    break;

	  case 'O':
	    ACE_OS::sprintf (outp, "%d", r1->ru_oublock - r0->ru_oublock);
	    END (outp);
	    break;
	  case 'C':
	    ACE_OS::sprintf (outp, "%d+%d", r1->ru_nvcsw - r0->ru_nvcsw,
		     r1->ru_nivcsw - r0->ru_nivcsw);
	    END (outp);
	    break;
#endif /* !SYSV */
	  }
    }
  *outp = '\0';
}

// adds two "timeval" structures
void
tvadd (struct timeval *tsum, struct timeval *t0, struct timeval *t1)
{

  tsum->tv_sec = t0->tv_sec + t1->tv_sec;
  tsum->tv_usec = t0->tv_usec + t1->tv_usec;
  if (tsum->tv_usec > 1000000)
    tsum->tv_sec++, tsum->tv_usec -= 1000000;
}

// finds difference between two timevals
void
tvsub (struct timeval *tdiff, struct timeval *t1, struct timeval *t0)
{

  tdiff->tv_sec = t1->tv_sec - t0->tv_sec;
  tdiff->tv_usec = t1->tv_usec - t0->tv_usec;
  if (tdiff->tv_usec < 0)
    tdiff->tv_sec--, tdiff->tv_usec += 1000000;
}

// print in seconds
void
psecs (CORBA::Long l, register char *cp)
{
  register int i;

  i = l / 3600;
  if (i)
    {
      ACE_OS::sprintf (cp, "%d:", i);
      END (cp);
      i = l % 3600;
      ACE_OS::sprintf (cp, "%d%d", (i / 60) / 10, (i / 60) % 10);
      END (cp);
    }
  else
    {
      i = l;
      ACE_OS::sprintf (cp, "%d", i / 60);
      END (cp);
    }
  i %= 60;
  *cp++ = ':';
  ACE_OS::sprintf (cp, "%d%d", i / 10, i % 10);
}

// generate the specified delay in microseconds
void
delay (int us)
{
  struct timeval tv;

  tv.tv_sec = 0;
  tv.tv_usec = us;
  (void) select (1, (fd_set *) 0, (fd_set *) 0, (fd_set *) 0, &tv);
}

// fill up a buffer with a data type that we want to send
void
FillPattern (register char *cp, register CORBA::Long bufLen, CORBA::ULong dt)
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
       register CORBA::Char *SeqPtr = (CORBA::Char *)cp;
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
       register BinStruct *SeqPtr = (BinStruct *)cp;
       register char c = 0;
       num = bufLen/sizeof(BinStruct);
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
       register CORBA::Octet *SeqPtr = (CORBA::Octet *)cp;
       register char c = 0;
       num = bufLen/sizeof(CORBA::Octet);
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

// print all the statistics
void PrintStats (void)
{
  if (cput <= 0.0)
    cput = 0.001;
  if (realt <= 0.0)
    realt = 0.001;

  if (title != 0)
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
      ACE_OS::fprintf (outFile, "\n%ldk \t", buflen / 1024);
      tmp = ((double) nbytes) / realt;
      ACE_OS::fprintf (outFile, "%.2f ", tmp * 8.0 / 1024.0 / 1024.0);
      fclose (outFile);
    }

  ACE_OS::fprintf (stdout,
           "ttcp%s: %ld bytes in %.2f real seconds = %s/sec +++\n",
           trans ? "-t" : "-r",
           nbytes, realt, outfmt (((double) nbytes) / realt));
  if (verbose)
    {
      ACE_OS::fprintf (stdout,
               "ttcp%s: %ld bytes in %.2f CPU seconds = %s/cpu sec\n",
               trans ? "-t" : "-r",
               nbytes, cput, outfmt (((double) nbytes) / cput));
    }
  ACE_OS::fprintf (stdout,
           "ttcp%s: %d Server Method calls, msec/call = %.2f, calls/sec = %.2f\n",
           trans ? "-t" : "-r",
           numCalls,
           1024.0 * realt / ((double) numCalls),
           ((double) numCalls) / realt);
  ACE_OS::fprintf (stdout, "ttcp%s: %s\n", trans ? "-t" : "-r", stats);
  if (verbose)
    {
      ACE_OS::fprintf (stdout,
               "ttcp%s: buffer address %#x\n",
               trans ? "-t" : "-r",
               buf);
    }
}
