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
// = BEING MODIFIED BY
//    Shawn Atkins
// ============================================================================

// This file has all the helper functions that do the computation of
// throughput, system time used, user time, etc based on data collected.

#include "ttcpC.h"
#include "ttcp_decl.h"

ACE_RCSID(TAO, utils, "$Id$")

// the error function.
// displays the error message and exits
int err (CORBA::String s)
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
  static CORBA::Char obuf[50];
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

#if defined (VXWORKS) || defined (CHORUS)
// Just so this file will compile.
inline
int
lrand48 ()
{
  return ::rand ();
}

inline
double
drand48 ()
{
  return ::rand ();
}
#endif /* VXWORKS || CHORUS */

// fill up a buffer with a data type that we want to send
void
FillPattern (register CORBA::Char *cp, register CORBA::Long bufLen, CORBA::ULong dt)
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
       register CORBA::Long *SeqPtr = (CORBA::Long *)cp;
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
  case SEND_COMPOSITE:
    /*still being worked on*/
    /*
   { fill in the RtiPacketSequence
     register RtiPacket *SeqPtr = (RtiPacket *)cp;
     register char c = 0;
     num = bufLen / sizeof (RtiPacket);
     can't know sizeof (RtiPacket) can it? if can have variable length sequence inside of it.
     num = bufLen;
     int numUpdates = 2;
     int numAttrs = 5;
     for (i = 0; bufLen > 0; i++){
       SeqPtr [i].packetHeader.packetLength = 1; // this is probably redundant
       SeqPtr [i].packetHeader.federationHandle = 2;
       SeqPtr [i].packetHeader.channelHandle = 3;
       SeqPtr [i].packetHeader.packetColor = 4;
       
       SeqPtr [i].msgs.length (numUpdates);
       
       for (int j = 0; j < numUpdates; ++j) 
	 {
	   SeqPtr [i].msgs[j].oumh (RtiObjectUpdateMessageHeader ());
	   RtiObjectUpdateMessageHeader & oumh = SeqPtr [i].msgs[j].oumh ();
	   oumh.updateLength = 2001; // redundant
	   oumh.updateTag = 2002;
	   oumh.objectHandle = 2003;
	   oumh.timestamp = 3.14159;
	   oumh.eventRetractionHandle = 2004;
	   oumh.classHandle = 2005;
	   oumh.sendingFederateHandle = 2006;
	   oumh.userTag = CORBA::string_dup ("beefcake!");
	   oumh.regionData.length(0);
	   oumh.transportationHandle = 1;
	   oumh.orderingHandle = 1;
	   oumh.messagePayload.length (numAttrs);
  	   
	   for (int k = 0; k < numAttrs; ++k) 
	     {
	       oumh.messagePayload[k] = HandleValuePair ();
	       HandleValuePair &hvp = oumh.messagePayload[k];
	       hvp.handle = k * k;
	       char *d1 = "somedata";
	       hvp.data.length (ACE_OS::strlen (d1)+1);
	       ACE_OS::strcpy ((char *) hvp.data.get_buffer (), d1);
	     }
	   num -= sizeof (SeqPtr [i]);
	   printf ("SIZE OF RTIPACKET IS %d", sizeof (SeqPtr [i]));
	 }

       fill in the RtiPacket
     }
     rtipacketSeq = new ttcp_sequence::RtiPacketSeq (num, num, SeqPtr);

   }*/ 
 
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
      ACE_OS::fprintf (outFile, "%ldk \t", buflen / 1024);
      tmp = ((double) nbytes) / realt;
      ACE_OS::fprintf (outFile, "%.2f\n", tmp * 8.0 / 1024.0 / 1024.0);
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

// generate the specified delay in microseconds
void
delay (int us)
{
  struct timeval tv;

  tv.tv_sec = 0;
  tv.tv_usec = us;
  (void) select (1, (fd_set *) 0, (fd_set *) 0, (fd_set *) 0, &tv);
}

#if defined (ACE_HAS_PRUSAGE_T) || defined (ACE_HAS_GETRUSAGE)
// Mon Apr 06 09:50:16 1998  David L. Levine  <levine@cs.wustl.edu>
// @@ This file should be updated to use ACE_Profile_Timer instead of
//    using rusage directly.

ACE_Profile_Timer ru0; //Timer to calculate stats

/*
 *                    P R E P _ T I M E R
 */
// this is in fact the internals of the "start_timer" operation
void
prep_timer (void)
{
  ru0.start ();
}

/*
 *                    R E A D _ T I M E R
 *
 */
// This implements the internals of the "stop_timer" method
double
read_timer (CORBA::Char *str, CORBA::Long len)
{
  char line[132];
  ACE_Profile_Timer::ACE_Elapsed_Time et;
 
  ru0.stop ();
  ru0.elapsed_time (et);

  prusage (line);
  (void) strncpy (str, line, len);
  
  //Get real time 
  realt = et.real_time;
 
  //Get CPU time (user+sys) 
  cput = et.user_time + et.system_time;
  if (cput < 0.00001)
    cput = 0.00001;
  return (cput);
}

// prints the rusage stats
void
prusage (char *outp)
{
  register ACE_timer_t t, ms;
  register char *cp;
  register int i;

  ACE_Profile_Timer::ACE_Elapsed_Time et;
  ACE_Profile_Timer::Rusage rusage;
  ru0.elapsed_time (et);
  ru0.elapsed_rusage (rusage);
  
  t = et.user_time + et.system_time;
  ms = et.real_time; 
  
#define END(x)  {while(*x) x++;}
#if defined(SYSV)
  cp = "%Uuser %Ssys %Ereal %P";
#else
#if defined(sgi)                /* IRIX 3.3 will show 0 for %M,%F,%R,%C */
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
            ACE_OS::sprintf (outp, "%f ", et.user_time);
            END (outp);
            break;
	    
          case 'S':
	    ACE_OS::sprintf (outp, "%f ", et.system_time);
            END (outp);
            break;

          case 'E':
            psecs (ms, outp);
            END (outp);
            break;

          case 'P':
	    ACE_OS::sprintf (outp, "%d%%", (int) (t * 100 / ((ms ? ms : 1))));
	    END (outp);
            break;

	    /*possible thing to add in is the equivalent for case X, D, K, M, F, and R for prusage_t*/
#if !defined(SYSV) 
#   if defined (ACE_HAS_PRUSAGE_T)
          case 'W':
            i = rusage.pr_nswap;;
            ACE_OS::sprintf (outp, "%d", i);
            END (outp);
            break;

          case 'X':
	    // ACE_OS::sprintf (outp, "%d", t == 0 ? 0 : (rusage.ru_ixrss) / t);
            //END (outp);
            break;

          case 'D':
            //ACE_OS::sprintf (outp, "%d", t == 0 ? 0 :
            //         (rusage.ru_idrss + rusage.ru_isrss) / t);
            //END (outp);
            break;

          case 'K':
            //ACE_OS::sprintf (outp, "%d", t == 0 ? 0 :
	    //        (rusage.ru_ixrss + rusage.ru_isrss + rusage.ru_idrss) / t);
            //END (outp);
            break;

          case 'M':
	    /* ACE_OS::sprintf (outp, "%d", rusage.ru_maxrss / 2);
            END (outp);*/
            break;

          case 'F':
	    /*
            ACE_OS::sprintf (outp, "%d", rusage.ru_majflt);
	    END (outp);*/
            break;

          case 'R':
	    /*ACE_OS::sprintf (outp, "%d", rusage.ru_minflt);
            END (outp);*/
            break;

          case 'I':
            ACE_OS::sprintf (outp, "%d", rusage.pr_inblk);
            END (outp);
            break;

          case 'O':
            ACE_OS::sprintf (outp, "%d", rusage.pr_oublk);
            END (outp);
            break;
          case 'C':
	    ACE_OS::sprintf (outp, "%d+%d", rusage.pr_vctx, rusage.pr_ictx);
            END (outp);
            break;
#   elif defined (ACE_HAS_GETRUSAGE)
	  case 'W':
            i = rusage.ru_nswap;;
            ACE_OS::sprintf (outp, "%d", i);
            END (outp);
            break;

          case 'X':
	    ACE_OS::sprintf (outp, "%d", t == 0 ? 0 : (int) ((rusage.ru_ixrss) / t));
            END (outp);
            break;

          case 'D':
            ACE_OS::sprintf (outp, "%d", t == 0 ? 0 :
                     (int) ((rusage.ru_idrss + rusage.ru_isrss) / t));
            END (outp);
            break;

          case 'K':
            ACE_OS::sprintf (outp, "%d", t == 0 ? 0 :
	            (int) ((rusage.ru_ixrss + rusage.ru_isrss + rusage.ru_idrss) / t));
            END (outp);
            break;

          case 'M':
	    ACE_OS::sprintf (outp, "%d", rusage.ru_maxrss / 2);
            END (outp);
            break;

          case 'F':
            ACE_OS::sprintf (outp, "%d", rusage.ru_majflt);
	    END (outp);
            break;

          case 'R':
	    ACE_OS::sprintf (outp, "%d", rusage.ru_minflt);
            END (outp);
            break;

          case 'I':
            ACE_OS::sprintf (outp, "%d", rusage.ru_inblock);
            END (outp);
            break;

          case 'O':
            ACE_OS::sprintf (outp, "%d", rusage.ru_oublock);
            END (outp);
            break;
          
	  case 'C':
	    ACE_OS::sprintf (outp, "%d+%d", rusage.ru_nvcsw, rusage.ru_nivcsw);
            END (outp);
            break;
#   endif /*ACE HAS PRUSAGE_T |   ACE_HAS_GETRUSAGE*/
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
psecs (CORBA::Double d, register CORBA::Char *cp)
{
  register int i;

  i = (int) d / 3600;
  if (i)
    {
      ACE_OS::sprintf (cp, "%d:", i);
      END (cp);
      i = (int) d % 3600;
      ACE_OS::sprintf (cp, "%d%d ", (i / 60) / 10, (i / 60) % 10);
      END (cp);
    }
  else
    {
      i = (int) d;
      ACE_OS::sprintf (cp, "%d", i / 60);
      END (cp);
    }
  i %= 60;
  *cp++ = ':';
  ACE_OS::sprintf (cp, "%d%d ", i / 10, i % 10);
}


#else /* ! ACE_HAS_PRUSAGE_T || ! ACE_HAS_GETRUSAGE */

void
prep_timer (void)
{
  ACE_ERROR ((LM_ERROR, "%s:%d; not supported\n", __FILE__, __LINE__));
  errno = ENOTSUP;
}

double
read_timer (CORBA::Char *str, CORBA::Long len)
{
  ACE_NOTSUP_RETURN (0.0);
}

#endif /* ! ACE_HAS_PRUSAGE_T || ! ACE_HAS_GETRUSAGE */









