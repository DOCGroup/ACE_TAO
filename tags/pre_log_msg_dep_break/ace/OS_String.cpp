// $Id$

#include "ace/OS_String.h"
#include "ace/OS.h"

ACE_RCSID(ace, OS_String, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_String.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

const char *
ACE_OS_String::strnstr (const char *s1, const char *s2, size_t len2)
{
  // Substring length
  size_t len1 = ACE_OS_String::strlen (s1);

  // Check if the substring is longer than the string being searched.
  if (len2 > len1)
    return 0;

  // Go upto <len>
  size_t len = len1 - len2;

  for (size_t i = 0; i <= len; i++)
    {
      if (ACE_OS::memcmp (s1 + i, s2, len2) == 0)
        // Found a match!  Return the index.
        return s1 + i;
    }

  return 0;
}

#if defined (ACE_HAS_WCHAR)
const wchar_t *
ACE_OS_String::strnstr (const wchar_t *s1, const wchar_t *s2, size_t len2)
{
  // Substring length
  size_t len1 = ACE_OS_String::strlen (s1);

  // Check if the substring is longer than the string being searched.
  if (len2 > len1)
    return 0;

  // Go upto <len>
  size_t len = len1 - len2;

  for (size_t i = 0; i <= len; i++)
    {
      if (ACE_OS::memcmp (s1 + i, s2, len2 * sizeof (wchar_t)) == 0)
        // Found a match!  Return the index.
        return s1 + i;
    }

  return 0;
}
#endif /* ACE_HAS_WCHAR */

char *
ACE_OS_String::strdup (const char *s)
{
  // @@ WINCE Should we provide this function on WinCE?
#if defined (ACE_HAS_STRDUP_EMULATION)
  char *t = (char *) ACE_OS::malloc (::strlen (s) + 1);
  if (t == 0)
    return 0;
  else
    return ACE_OS_String::strcpy (t, s);
#else
  return ::strdup (s);
#endif /* ACE_HAS_STRDUP_EMULATION */
}

#if defined (ACE_HAS_WCHAR)
wchar_t *
ACE_OS_String::strdup (const wchar_t *s)
{
#   if defined (__BORLANDC__)
  wchar_t *buffer =
    (wchar_t *) ACE_OS::malloc ((ACE_OS_String::strlen (s) + 1)
                                * sizeof (wchar_t));
  return ::wcscpy (buffer, s);
#   else
  return ::wcsdup (s);
#   endif /* __BORLANDC__ */
}
#endif /* ACE_HAS_WCHAR */



char *
ACE_OS_String::strptime (char *buf, const char *format, struct tm *tm)
{
#if !defined (ACE_HAS_WINCE)
#if defined (ACE_LACKS_NATIVE_STRPTIME)
  int bi = 0;
  int fi = 0;
  int percent = 0;

  if (!buf || !format)
    return 0;

  while (format[fi] != '\0')
    {
      if (percent)
        {
          percent = 0;
          switch (format[fi])
            {
            case '%':                        // an escaped %
              if (buf[bi] == '%')
                {
                  fi++; bi++;
                }
              else
                return buf + bi;
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
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_mday, &bi, &fi, 1, 31))
                return buf + bi;

              break;

            case 'D':                        /* %m/%d/%y */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_mon, &bi, &fi, 1, 12))
                return buf + bi;

              fi--;
              tm->tm_mon--;

              if (buf[bi] != '/')
                return buf + bi;

              bi++;

              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_mday, &bi, &fi, 1, 31))
                return buf + bi;

              fi--;
              if (buf[bi] != '/')
                return buf + bi;
              bi++;
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_year, &bi, &fi, 0, 99))
                return buf + bi;
              if (tm->tm_year < 69)
                tm->tm_year += 100;
              break;

            case 'H':                        /* hour (0-23) */
              /* FALL THROUGH */
            case 'k':
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
                return buf + bi;
              break;

              /* not supported yet:
                 case 'I':                        / * hour (0-12) * /
                 / * FALL THROUGH * /
                 case 'l':
                 break;
                 */

            case 'j':                        /* day of year (0-366) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_yday, &bi, &fi, 1, 366))
                return buf + bi;

              tm->tm_yday--;
              break;

            case 'm':                        /* an escaped % */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_mon, &bi, &fi, 1, 12))
                return buf + bi;

              tm->tm_mon--;
              break;

            case 'M':                        /* minute (0-59) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
                return buf + bi;

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
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
                return buf + bi;

              fi--;
              if (buf[bi] != ':')
                return buf + bi;
              bi++;
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
                return buf + bi;

              break;

            case 'S':                        /* seconds (0-61) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_sec, &bi, &fi, 0, 61))
                return buf + bi;
              break;

            case 'T':                        /* %H:%M:%S */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
                return buf + bi;

              fi--;
              if (buf[bi] != ':')
                return buf + bi;
              bi++;
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
                return buf + bi;

              fi--;
              if (buf[bi] != ':')
                return buf + bi;
              bi++;
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_sec, &bi, &fi, 0, 61))
                return buf + bi;

              break;

            case 'w':                        /* day of week (0=Sun-6) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_wday, &bi, &fi, 0, 6))
                return buf + bi;

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
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_year, &bi, &fi, 0, 99))
                return buf + bi;

              if (tm->tm_year < 69)
                tm->tm_year += 100;
              break;

            case 'Y':                        /* the full year (1999) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_year, &bi, &fi, 0, 0))
                return buf + bi;

              tm->tm_year -= 1900;
              break;

            default:                        /* unrecognised */
              return buf + bi;
            } /* switch (format[fi]) */

        }
      else
        { /* if (percent) */
          if (format[fi] == '%')
            {
              percent = 1;
              fi++;
            }
          else
            {
              if (format[fi] == buf[bi])
                {
                  fi++;
                  bi++;
                }
              else
                return buf + bi;
            }
        } /* if (percent) */
    } /* while (format[fi] */

  return buf + bi;
#else  /* ! ACE_LACKS_NATIVE_STRPTIME */
  return ::strptime (buf,
                     format,
                     tm);
#endif /* ! ACE_LACKS_NATIVE_STRPTIME */
#else /* ! ACE_HAS_WINCE */
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (format);
  ACE_UNUSED_ARG (tm);

  ACE_NOTSUP_RETURN (0);
#endif /* ! ACE_HAS_WINCE */
}

#if defined (ACE_HAS_STRPTIME)
# if defined (ACE_LACKS_NATIVE_STRPTIME)
int
ACE_OS_String::strptime_getnum (char *buf,
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
      i++;
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
# endif /* ACE_LACKS_NATIVE_STRPTIME */
#endif /* ACE_HAS_STRPTIME */
