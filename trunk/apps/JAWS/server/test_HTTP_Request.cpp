#include <stdio.h>
#include "HTTP_Request.h"
#include "ace/Mem_Map.h"

static char* request_strings [] =
{ 
  "GET \\~irfan\\hi\\mom.html\r\n\r\n",
  "GET \\~irfan\\hi\\mom.html HTTP/1.0\r\n\r\n",
  "PUT filename.html HTTP/1.0\nContent-type: text/plain\nContent-length: 15\r\n\r\nTHIS IS DATA!!!",
  "GET \\~harrison\\mybutt.html in my program\r\n\r\n",
  "PUT \\~harrison\bilename.html HTTP/1.0\ni suck... asldkasldk Content-type: text/plain\nContent-length: 1024 alskdkls",
  "PUT bomb in my program\r\n\r\n"
};

static int total = 2;

int
main ()
{
  int index = 0;
  HTTP_Request request;
  while (index < total)
    {
      const char *r = request_strings[index++];
      int len = ::strlen (r);
      request.init (r, len);
      request.dump ();

    if (request.type () == HTTP_Request::PUT)
      {
	ACE_Mem_Map outfile;
	ACE_HANDLE handle = ACE_OS::open (request.filename (), 
					  FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_OVERLAPPED | O_RDWR | O_CREAT);
	if (outfile.map (handle,
			 request.content_length (),
			 PROT_RDWR,
			 MAP_SHARED) == -1)
	  ACE_ERROR ((LM_ERROR, "%p: opening %s.\n",
		      "main",
		      request.filename ()));
	else
	  {
	    // Write the file.
	    ACE_OS::memcpy (outfile.addr (), 
			    request.data (),
			    request.content_length ());
	    outfile.sync ();
	  }
      }
    }
 
  return 0;
}
