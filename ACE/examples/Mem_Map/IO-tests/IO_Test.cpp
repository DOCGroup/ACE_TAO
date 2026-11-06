#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Mem_Map.h"
#include "ace/Log_Msg.h"
#include "IO_Test.h"

#if !defined(ACE_WIN32)


IO_Test::IO_Test (char const *name,
                  ACE_Profile_Timer &tm)
  : name_ (name), tm_ (tm)
{
}

IO_Test::~IO_Test () = default;

char const *
IO_Test::name () const
{
  return this->name_;
}

Slow_Read_Write_Test::Slow_Read_Write_Test (char const *name,
                                            ACE_Profile_Timer &tm)
  : IO_Test (name, tm)
{
}

int
Slow_Read_Write_Test::run_test (int iterations,
                                FILE *input_fp,
                                FILE *output_fp)
{
  ACE_HANDLE const ifd = ACE_OS::fileno (input_fp);
  ACE_HANDLE const ofd = ACE_OS::fileno (output_fp);

  this->tm_.start ();

  for (int iteration = 0; iteration < iterations; ++iteration)
    {
      char c {};

      while (ACE_OS::read (ifd, &c, sizeof c) > 0)
        ACE_OS::write (ofd, &c, sizeof c);

      ACE_OS::lseek (ifd, 0, SEEK_SET);
      ACE_OS::lseek (ofd, 0, SEEK_SET);
    }

  this->tm_.stop ();
  return 0;
}

Stdio_Test::Stdio_Test (char const *name,
                        ACE_Profile_Timer &tm)
  : IO_Test (name, tm)
{
}

int
Stdio_Test::run_test (int iterations,
                      FILE *input_fp,
                      FILE *output_fp)
{
  this->tm_.start ();

  for (int iteration = 0; iteration < iterations; ++iteration)
    {
      int c;

      while ((c = ACE_OS::getc (input_fp)) != EOF)
        ACE_OS::putc (c, output_fp);

      ACE_OS::rewind (input_fp);
      ACE_OS::rewind (output_fp);
    }
  this->tm_.stop ();
  return 0;
}

Block_Read_Write_Test::Block_Read_Write_Test (char const *name,
                                              ACE_Profile_Timer &tm)
  : IO_Test (name, tm)
{
}

int
Block_Read_Write_Test::run_test (int iterations,
                                 FILE *input_fp,
                                 FILE *output_fp)
{
  ACE_HANDLE const ifd = ACE_OS::fileno (input_fp);
  ACE_HANDLE const ofd = ACE_OS::fileno (output_fp);

  this->tm_.start ();

  for (int iteration = 0; iteration < iterations; ++iteration)
    {
      char buf[BUFSIZ];
      ssize_t n;

      while ((n = ACE_OS::read (ifd,
                                buf,
                                sizeof buf)) > 0)
        ACE_OS::write (ofd, buf, n);

      ACE_OS::lseek (ifd, 0, SEEK_SET);
      ACE_OS::lseek (ofd, 0, SEEK_SET);
    }

  this->tm_.stop ();
  return 0;
}

Block_Fread_Fwrite_Test::Block_Fread_Fwrite_Test (char const *name,
                                                  ACE_Profile_Timer &tm)
  : IO_Test (name, tm)
{
}

int
Block_Fread_Fwrite_Test::run_test (int iterations,
                                   FILE *input_fp,
                                   FILE *output_fp)
{
  this->tm_.start ();

  for (int iteration = 0; iteration < iterations; ++iteration)
    {
      char buf[BUFSIZ];
      ssize_t n;

      while ((n = ACE_OS::fread (buf,
                                 1,
                                 sizeof buf,
                                 input_fp)) != 0)
        ACE_OS::fwrite (buf, n, 1, output_fp);

      ACE_OS::rewind (input_fp);
      ACE_OS::rewind (output_fp);
    }

  this->tm_.stop ();
  return 0;
}

Mmap1_Test::Mmap1_Test (char const *name,
                        ACE_Profile_Timer &tm)
  : IO_Test (name, tm)
{
}

int
Mmap1_Test::run_test (int iterations,
                      FILE *input_fp,
                      FILE *output_fp)
{
  ACE_Mem_Map map_input (ACE_OS::fileno (input_fp));
  void *const src = map_input.addr ();

  if (src == MAP_FAILED)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%C"), this->name ()), -1);
  else
    {
      this->tm_.start ();

      for (int iteration = 0; iteration < iterations; ++iteration)
        {
          if (ACE_OS::write (ACE_OS::fileno (output_fp),
                             src,
                             map_input.size ()) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%C"),
                               this->name ()),
                              -1);
          ACE_OS::lseek (ACE_OS::fileno (output_fp),
                         0,
                         SEEK_SET);
        }

      this->tm_.stop ();
    }

  if (map_input.unmap () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%C"),
                       this->name ()),
                      -1);
  else
    return 0;
}

Mmap2_Test::Mmap2_Test (char const *name,
                        ACE_Profile_Timer &tm)
  : IO_Test (name, tm)
{
}

int
Mmap2_Test::run_test (int iterations,
                      FILE *input_fp,
                      FILE *output_fp)
{
  ACE_Mem_Map map_input (ACE_OS::fileno (input_fp));
  int const size = map_input.size ();
  ACE_Mem_Map map_output (ACE_OS::fileno (output_fp),
                          size,
                          PROT_WRITE,
                          MAP_SHARED);
  void *const src = map_input.addr ();
  void *const dst = map_output.addr ();

  if (src == MAP_FAILED || dst == MAP_FAILED)
    return -1;
  else
    {
      this->tm_.start ();

      for (int iteration = 0; iteration < iterations; ++iteration)
        ACE_OS::memcpy (dst, src, size);

      this->tm_.stop ();
    }

  if (map_input.unmap () == -1
      || map_output.unmap () == -1)
    return -1;
  else
    return 0;
}
#endif
