// $Id$

#if !defined(_WIN32) 

#include "ace/OS.h"
#include "ace/Mem_Map.h"
#include "ace/Log_Msg.h"
#include "IO_Test.h"

ACE_RCSID(IO_tests, IO_Test, "$Id$")



IO_Test::IO_Test (const char *name,
                  ACE_Profile_Timer &tm)
  : name_ (name), tm_ (tm)
{
}

const char *
IO_Test::name (void)
{
  return this->name_;
}

Slow_Read_Write_Test::Slow_Read_Write_Test (const char *name,
                                            ACE_Profile_Timer &tm)
  : IO_Test (name, tm)
{
}

int 
Slow_Read_Write_Test::run_test (int iterations,
                                FILE *input_fp,
                                FILE *output_fp)
{
  ACE_HANDLE ifd = fileno (input_fp);
  ACE_HANDLE ofd = fileno (output_fp);

  this->tm_.start ();

  while (--iterations >= 0)
    {
      char c;

      while (ACE_OS::read (ifd, &c, sizeof c) > 0)
        ::write (ofd, &c, sizeof c);

      ACE_OS::lseek (ifd, 0, SEEK_SET);
      ACE_OS::lseek (ofd, 0, SEEK_SET);
    }

  this->tm_.stop ();
  return 0;
}  

Stdio_Test::Stdio_Test (const char *name,
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

  while (--iterations >= 0)
    {
      int c;

      while ((c = getc (input_fp)) != EOF)
        putc (c, output_fp);

      ACE_OS::rewind (input_fp);
      ACE_OS::rewind (output_fp);
    }
  this->tm_.stop ();
  return 0;
}  

Block_Read_Write_Test::Block_Read_Write_Test (const char *name,
                                              ACE_Profile_Timer &tm)
  : IO_Test (name, tm)
{
}

int 
Block_Read_Write_Test::run_test (int iterations,
                                 FILE *input_fp,
                                 FILE *output_fp)
{
  int ifd = fileno (input_fp);
  int ofd = fileno (output_fp);

  this->tm_.start ();

  while (--iterations >= 0)
    {
      char buf[BUFSIZ];
      ssize_t n;

      while ((n = ACE_OS::read (ifd,
                                buf,
                                sizeof buf)) > 0)
                                ::write (ofd, buf, n);

      ACE_OS::lseek (ifd, 0, SEEK_SET);
      ACE_OS::lseek (ofd, 0, SEEK_SET);
    }

  this->tm_.stop ();
  return 0;
}  

Block_Fread_Fwrite_Test::Block_Fread_Fwrite_Test (const char *name,
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

  while (--iterations >= 0)
    {
      char buf[BUFSIZ];
      ssize_t n;

      while ((n = ACE_OS::fread (buf,
                                 1,
                                 sizeof buf,
                                 input_fp)) != 0)
                                 ::fwrite (buf, n, 1, output_fp);

      ACE_OS::rewind (input_fp);
      ACE_OS::rewind (output_fp);
    }

  this->tm_.stop ();
  return 0;
}  

Mmap1_Test::Mmap1_Test (const char *name,
                        ACE_Profile_Timer &tm)
  : IO_Test (name, tm)
{
}

int 
Mmap1_Test::run_test (int iterations,
                      FILE *input_fp,
                      FILE *output_fp)
{
  ACE_Mem_Map map_input (fileno (input_fp));
  void *src = map_input.addr ();

  if (src == MAP_FAILED)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%s",
                       this->name ()),
                      -1);
  else
    {
      this->tm_.start ();

      while (--iterations >= 0)
      {
        if (ACE_OS::write (fileno (output_fp),
                             src,
                             map_input.size ()) == -1)
                             ACE_ERROR_RETURN ((LM_ERROR,
                               "%s",
                               this->name ()),
                              -1);
        ACE_OS::lseek (fileno (output_fp),
                         0,
                         SEEK_SET);
      }

      this->tm_.stop ();
    }
  
  if (map_input.unmap () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%s",
                       this->name ()),
                      -1);
  else
    return 0;
}  

Mmap2_Test::Mmap2_Test (const char *name,
                        ACE_Profile_Timer &tm)
  : IO_Test (name, tm)
{
}

int 
Mmap2_Test::run_test (int iterations,
                      FILE *input_fp,
                      FILE *output_fp)
{
  ACE_Mem_Map map_input (fileno (input_fp));
  int size = map_input.size ();
  ACE_Mem_Map map_output (fileno (output_fp),
                          size,
                          PROT_WRITE,
                          MAP_SHARED);
  void *src = map_input.addr ();
  void *dst = map_output.addr ();

  if (src == MAP_FAILED || dst == MAP_FAILED)
    return -1;
  else
    {
      this->tm_.start ();

      while (--iterations >= 0)
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
