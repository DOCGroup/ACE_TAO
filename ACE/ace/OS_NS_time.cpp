#include "ace/OS_NS_time.h"

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_NS_time.inl"
#endif /* ACE_HAS_INLINED_OSCALLS */

#if defined (ACE_LACKS_STRPTIME)
# include "ace/os_include/os_ctype.h"
#endif /* ACE_LACKS_STRPTIME */

#include "ace/OS_NS_Thread.h"
#include "ace/Object_Manager_Base.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

struct tm *
ACE_OS::localtime_r (const time_t *t, struct tm *res)
{
  ACE_OS_TRACE ("ACE_OS::localtime_r");
#if defined (ACE_HAS_TR24731_2005_CRT)
  ACE_SECURECRTCALL (localtime_s (res, t), struct tm *, 0, res);
  return res;
#elif defined (ACE_LACKS_LOCALTIME_R)
  ACE_OS_GUARD

  ACE_UNUSED_ARG (res);
  struct tm * res_ptr = 0;
  ACE_OSCALL (::localtime (t), struct tm *, res_ptr);
  if (res_ptr == 0)
    return 0;
  else
    {
      *res = *res_ptr;
      return res;
    }
#else
  return ace_localtime_r_helper (t, res);
#endif /* ACE_HAS_TR24731_2005_CRT */
}

time_t
ACE_OS::mktime (struct tm *t)
{
  ACE_OS_TRACE ("ACE_OS::mktime");
#if defined (ACE_HAS_THREADS)  &&  !defined (ACE_HAS_MT_SAFE_MKTIME)
  ACE_OS_GUARD
#endif /* ACE_HAS_THREADS  &&  ! ACE_HAS_MT_SAFE_MKTIME */

  return std::mktime (t);
}

#if defined (ACE_LACKS_STRPTIME)
char *
ACE_OS::strptime_emulation (const char *buf, const char *format, struct tm *tm)
{
  int bi = 0;
  int fi = 0;
  bool percent = false;

  if (!buf || !format)
    return 0;

  while (format[fi] != '\0')
    {
      if (percent)
        {
          percent = false;
          switch (format[fi])
            {
            case '%':                        // an escaped %
              if (buf[bi] == '%')
                {
                  ++fi;
                  ++bi;
                }
              else
                return const_cast<char*> (buf + bi);
              break;

              /* not supported yet: weekday via locale long/short names
                 case 'a':                        / * weekday via locale * /
                 / * FALL THROUGH * /
                 case 'A':                        / * long/short names * /
                 break;
                 */

              /* not supported yet:
                 case 'b':                        / * month via locale * /
                 / * FALL THROUGH * /
                 case 'B':                        / * long/short names * /
                 / * FALL THROUGH * /
                 case 'h':
                 break;
                 */

              /* not supported yet:
                 case 'c':                        / * %x %X * /
                 break;
                 */

              /* not supported yet:
                 case 'C':                        / * date & time -  * /
                 / * locale long format * /
                 break;
                 */

            case 'd':                        /* day of month (1-31) */
              /* FALL THROUGH */
            case 'e':
              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_mday, &bi, &fi, 1, 31))
                return const_cast<char*> (buf + bi);

              break;

            case 'D':                        /* %m/%d/%y */
              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_mon, &bi, &fi, 1, 12))
                return const_cast<char*> (buf + bi);

              --fi;
              tm->tm_mon--;

              if (buf[bi] != '/')
                return const_cast<char*> (buf + bi);

              ++bi;

              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_mday, &bi, &fi, 1, 31))
                return const_cast<char*> (buf + bi);

              --fi;
              if (buf[bi] != '/')
                return const_cast<char*> (buf + bi);
              ++bi;
              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_year, &bi, &fi, 0, 99))
                return const_cast<char*> (buf + bi);
              if (tm->tm_year < 69)
                tm->tm_year += 100;
              break;

            case 'H':                        /* hour (0-23) */
              /* FALL THROUGH */
            case 'k':
              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
                return const_cast<char*> (buf + bi);
              break;

              /* not supported yet:
                 case 'I':                        / * hour (0-12) * /
                 / * FALL THROUGH * /
                 case 'l':
                 break;
                 */

            case 'j':                        /* day of year (0-366) */
              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_yday, &bi, &fi, 1, 366))
                return const_cast<char*> (buf + bi);

              tm->tm_yday--;
              break;

            case 'm':                        /* an escaped % */
              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_mon, &bi, &fi, 1, 12))
                return const_cast<char*> (buf + bi);

              tm->tm_mon--;
              break;

            case 'M':                        /* minute (0-59) */
              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
                return const_cast<char*> (buf + bi);

              break;

              /* not supported yet:
                 case 'p':                        / * am or pm for locale * /
                 break;
                 */

              /* not supported yet:
                 case 'r':                        / * %I:%M:%S %p * /
                 break;
                 */

            case 'R':                        /* %H:%M */
              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
                return const_cast<char*> (buf + bi);

              --fi;
              if (buf[bi] != ':')
                return const_cast<char*> (buf + bi);
              ++bi;
              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
                return const_cast<char*> (buf + bi);

              break;

            case 'S':                        /* seconds (0-61) */
              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_sec, &bi, &fi, 0, 61))
                return const_cast<char*> (buf + bi);
              break;

            case 'T':                        /* %H:%M:%S */
              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
                return const_cast<char*> (buf + bi);

              --fi;
              if (buf[bi] != ':')
                return const_cast<char*> (buf + bi);
              ++bi;
              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
                return const_cast<char*> (buf + bi);

              --fi;
              if (buf[bi] != ':')
                return const_cast<char*> (buf + bi);
              ++bi;
              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_sec, &bi, &fi, 0, 61))
                return const_cast<char*> (buf + bi);

              break;

            case 'w':                        /* day of week (0=Sun-6) */
              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_wday, &bi, &fi, 0, 6))
                return const_cast<char*> (buf + bi);

              break;

              /* not supported yet: date, based on locale
                 case 'x':                        / * date, based on locale * /
                 break;
                 */

              /* not supported yet:
                 case 'X':                        / * time, based on locale * /
                 break;
                 */

            case 'y':                        /* the year - 1900 (0-99) */
              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_year, &bi, &fi, 0, 99))
                return const_cast<char*> (buf + bi);

              if (tm->tm_year < 69)
                tm->tm_year += 100;
              break;

            case 'Y':                        /* the full year (1999) */
              if (!ACE_OS::strptime_getnum
                     (buf + bi, &tm->tm_year, &bi, &fi, 0, 0))
                return const_cast<char*> (buf + bi);

              tm->tm_year -= 1900;
              break;

            default:                        /* unrecognised */
              return const_cast<char*> (buf + bi);
            } /* switch (format[fi]) */

        }
      else
        { /* if (percent) */
          if (format[fi] == '%')
            {
              percent = true;
              ++fi;
            }
          else
            {
              if (format[fi] == buf[bi])
                {
                  ++fi;
                  ++bi;
                }
              else
                return const_cast<char*> (buf + bi);
            }
        } /* if (percent) */
    } /* while (format[fi] */

  return const_cast<char*> (buf + bi);
}

int
ACE_OS::strptime_getnum (const char *buf,
                         int *num,
                         int *bi,
                         int *fi,
                         int min,
                         int max)
{
  int i = 0, tmp = 0;

  while (isdigit (buf[i]))
    {
      tmp = (tmp * 10) + (buf[i] - '0');
      if (max && (tmp > max))
        return 0;
      ++i;
    }

  if (tmp < min)
    return 0;
  else if (i)
    {
      *num = tmp;
      (*fi)++;
      *bi += i;
      return 1;
    }
  else
    return 0;
}
#endif /* ACE_LACKS_STRPTIME */

ACE_END_VERSIONED_NAMESPACE_DECL
