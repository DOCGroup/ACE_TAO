// test_JAWS_File.cpp

#include "JAWS/server/JAWS_File.h"

const char *message = "this is the contents of a new testfile\n";

void
dump (JAWS_File_Handle &h)
{
  ACE_OS::printf ("%s", h.address ());
}

void
delay (void)
{
  JAWS_File_Handle fh4("test_somefile");
  dump (fh4);
}

int
main (void)
{
  // Grab a file three times
  {
    JAWS_File_Handle fh1("test_somefile");
    JAWS_File_Handle fh2("test_somefile");
    JAWS_File_Handle fh3("test_somefile");

    dump (fh1);
    dump (fh2);

    ::printf("change the test file now\n");
    ::getchar ();

    delay ();

    {
      JAWS_File_Handle fh5("test_somefile", ACE_OS::strlen (message));
      ACE_OS::memcpy (fh5.address (), message, ACE_OS::strlen (message));
    }

    JAWS_File_Handle fh6("test_somefile");

    dump (fh3);
    dump (fh6);
  }

  JAWS_File_Handle fh7("test_somefile");
  dump (fh7);

  return 0;
}
