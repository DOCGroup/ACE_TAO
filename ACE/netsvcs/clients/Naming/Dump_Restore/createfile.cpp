#include "ace/Malloc_Base.h"
#include "ace/Read_Buffer.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"

#include <memory>

static void
free_read_buffer (char *buffer)
{
  ACE_Allocator::instance ()->free (buffer);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc < 3)
    return -1;

  std::unique_ptr<FILE, int (*) (FILE *)> infile {
    ACE_OS::fopen (argv[1], ACE_TEXT ("r")), ACE_OS::fclose
  };
  if (!infile)
    return -1;

  std::unique_ptr<FILE, int (*) (FILE *)> outfile {
    ACE_OS::fopen (argv[2], ACE_TEXT ("w")), ACE_OS::fclose
  };
  if (!outfile)
    return -1;

  ACE_Read_Buffer read_buffer (infile.get (), false, ACE_Allocator::instance ());
  std::size_t count {};
  for (char *line {};
       (line = read_buffer.read ('\n')) != nullptr;)
    {
      std::unique_ptr<char, decltype (&free_read_buffer)> line_owner {
        line, free_read_buffer
      };
      ACE_OS::fputs (line_owner.get (), outfile.get ());
      if (count % 2 == 0)
        ACE_OS::fputs (" ", outfile.get ());
      else
        ACE_OS::fputs ("\n", outfile.get ());
      ++count;
    }

  return 0;
}
