// $Id$

// HTTP_Helpers.cpp -- Helper utilities for both server and client

#include "JAWS/server/HTTP_Helpers.h"

// = Static initialization.
const char * const
HTTP_Helper::months_[12]= { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

#if !defined (ACE_HAS_REENTRANT_LIBC)
#if defined (ACE_HAS_THREADS)  
ACE_Thread_Mutex HTTP_Helper::mutex_;
#endif /* ACE_HAS_THREADS */
#endif /* NOT ACE_HAS_REENTRANT_LIBC */

ACE_SYNCH_MUTEX HTTP_Status_Code::lock_;
int HTTP_Status_Code::instance_ = 0;
const char *HTTP_Status_Code::Reason[HTTP_Status_Code::MAX_STATUS_CODE + 1];

time_t
HTTP_Helper::HTTP_mktime (const char *httpdate)
{
  char *buf = new char[strlen(httpdate)+1];

  // Make spaces in the date be semi-colons so we can parse robustly
  // with sscanf.
  {
    const char *ptr1 = httpdate;
    char *ptr2 = buf;
    do {
      if (*ptr1 == ' ') *ptr2++ = ';';
      else *ptr2++ = *ptr1;
    } while (*ptr1++ != '\0');
  }

  // In HTTP/1.0, there are three versions of an HTTP_date.

  // rfc1123-date   = wkday "," SP dd month yyyy SP hh:mm:ss SP "GMT"
  // rfc850-date    = weekday "," SP dd-month-yy SP hh:mm:ss SP "GMT"
  // asctime-date   = wkday SP month dd SP hh:mm:ss SP yyyy

  const char *rfc1123_date = "%3s,;%2d;%3s;%4d;%2d:%2d:%2d;GMT";
  const char *rfc850_date = "%s,;%2d-%3s-%2d;%2d:%2d:%2d;GMT";
  const char *asctime_date = "%3s;%3s;%2d;%2d:%2d:%2d;%4d";

  // should also support other versions (such as from NNTP and SMTP) for
  // robustness, but it should be clear how to extend this.

  struct tm tms;
  char month[4];
  char weekday[10];

  if (::sscanf(buf, rfc1123_date,
               weekday,
               &tms.tm_mday, month, &tms.tm_year,
               &tms.tm_hour, &tms.tm_min, &tms.tm_sec)
      == 7) {
  }
  else if (::sscanf(buf, rfc850_date,
                    weekday,
                    &tms.tm_mday, month, &tms.tm_year,
                    &tms.tm_hour, &tms.tm_min, &tms.tm_sec)
           == 7) {
    weekday[3] = '\0';
    ;
  }
  else if (::sscanf(buf, asctime_date,
                    weekday,
                    month, &tms.tm_mday,
                    &tms.tm_hour, &tms.tm_min, &tms.tm_sec,
                    &tms.tm_year)
           == 7) {
    ;
  }
  delete buf;

  tms.tm_year = HTTP_Helper::fixyear (tms.tm_year);
  tms.tm_mon = HTTP_Helper::HTTP_month (month);
  if (tms.tm_mon == -1) return (time_t)(-1);

  /* mktime is a Standard C function */
  {

#if !defined (ACE_HAS_REENTRANT_LIBC)
#if defined (ACE_HAS_THREADS)
    ACE_Guard<ACE_Thread_Mutex> g(HTTP_Helper::mutex_);
#endif /* ACE_HAS_THREADS */
#endif /* NOT ACE_HAS_REENTRANT_LIBC */

    return ACE_OS::mktime(&tms);
  }
}

const char *
HTTP_Helper::HTTP_date (char * const date_string, int date_length)
{
  time_t tloc;
  struct tm tms;

  if (date_string != 0) {
    if (ACE_OS::time(&tloc) != -1 && ACE_OS::gmtime_r(&tloc, &tms) != NULL)
      ::strftime (date_string, date_length, "%a, %d %b %Y %T %Z", &tms);
    else return 0;
  }

  return date_string;
}

int
HTTP_Helper::HTTP_month (const char *month)
{
  for (int i = 0; i < 12; i++)
    if (ACE_OS::strcmp(month, HTTP_Helper::months_[i]) == 0)
      return i;

  return -1;
}

const char *
HTTP_Helper::HTTP_month (int month)
{
  if (month < 0 || month >= 12) return 0;

  return HTTP_Helper::months_[month];
}

char *
HTTP_Helper::HTTP_decode_string (char *path)
  // fix the path if it needs fixing/is fixable
{
  // replace the percentcodes with the actual character
  int i,j;
  char percentcode[3];
  
  for (i = j = 0; path[i] != '\0'; i++,j++) {
    if (path[i] == '%') {
      percentcode[0] = path[++i];
      percentcode[1] = path[++i];
      percentcode[2] = '\0';
      path[j] = ACE_OS::strtol (percentcode, (char **)0, 16);
    }
    else path[j] = path[i];
  }
  path[j] = path[i];

  return path;
}

int
HTTP_Helper::fixyear (int year)
{
  // Fix the year 2000 problem

  if (year > 1000) year -= 1900;
  else if (year < 100) {
    struct tm tms;
    time_t tloc;

    if (ACE_OS::time(&tloc) != -1) {
      ACE_OS::gmtime_r(&tloc, &tms);

      if (tms.tm_year%100 == year) year = tms.tm_year;
      if ((year+1)%100 == tms.tm_year%100) year = tms.tm_year-1;
      if (year == (tms.tm_year+1)%100) year = tms.tm_year+1;

      // What to do if none of the above?
    }
  }

  return year;
}

const char **
HTTP_Status_Code::instance (void)
{
  if (HTTP_Status_Code::instance_ == 0) 
    {
      ACE_Guard<ACE_SYNCH_MUTEX> g (lock_);
      
      if (HTTP_Status_Code::instance_ == 0) 
	{
	  for (size_t i = 0; i < HTTP_Status_Code::MAX_STATUS_CODE + 1; i++) 
	    {
	      switch (i) 
		{
		case STATUS_OK:
		  HTTP_Status_Code::Reason[i] = "OK"; break;
		case STATUS_CREATED:
		  HTTP_Status_Code::Reason[i] = "Created"; break;
		case STATUS_ACCEPTED:
		  HTTP_Status_Code::Reason[i] = "Accepted"; break;
		case STATUS_NO_CONTENT:
		  HTTP_Status_Code::Reason[i] = "No Content"; break;
		case STATUS_MOVED_PERMANENTLY:
		  HTTP_Status_Code::Reason[i] = "Moved Permanently"; break;
		case STATUS_MOVED_TEMPORARILY:
		  HTTP_Status_Code::Reason[i] = "Moved Temporarily"; break;
		case STATUS_NOT_MODIFIED:
		  HTTP_Status_Code::Reason[i] = "Not Modified"; break;
		case STATUS_BAD_REQUEST:
		  HTTP_Status_Code::Reason[i] = "Bad Request"; break;
		case STATUS_UNAUTHORIZED:
		  HTTP_Status_Code::Reason[i] = "Unauthorized"; break;
		case STATUS_FORBIDDEN:
		  HTTP_Status_Code::Reason[i] = "Forbidden"; break;
		case STATUS_NOT_FOUND:
		  HTTP_Status_Code::Reason[i] = "Not Found"; break;
		case STATUS_INTERNAL_SERVER_ERROR:
		  HTTP_Status_Code::Reason[i] = "Internal Server Error"; break;
		case STATUS_NOT_IMPLEMENTED:
		  HTTP_Status_Code::Reason[i] = "Not Implemented"; break;
		case STATUS_BAD_GATEWAY:
		  HTTP_Status_Code::Reason[i] = "Bad Gateway"; break;
		case STATUS_SERVICE_UNAVAILABLE:
		  HTTP_Status_Code::Reason[i] = "Service Unavailable"; break;
		default:
		  HTTP_Status_Code::Reason[i] = "Unknown";
		}
	    }

	  HTTP_Status_Code::instance_ = 1;
	}

      // GUARD released
    }

  return HTTP_Status_Code::Reason;
}
