/* -*- C++ -*- */
/* Class hierarchy for the File I/O tests. */

#include "ace/Profile_Timer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/* Base class for all the File I/O tests. */

class IO_Test
{
public:
  // Initialize the test name
  IO_Test (char const *name,
           ACE_Profile_Timer &tm);

  // Destructor.
  virtual ~IO_Test ();

  IO_Test (IO_Test const &) = delete;
  IO_Test (IO_Test &&) = delete;
  IO_Test &operator= (IO_Test const &) = delete;
  IO_Test &operator= (IO_Test &&) = delete;

  // Return the name of the test
  char const *name () const;

  // Execute the IO test (note this is a pure virtual function...)
  virtual int run_test (int iterations,
                        FILE *input_fp,
                        FILE *output_fp) = 0;

protected:
  // Name of the test
  char const *const name_;

  // Reference to a timer
  ACE_Profile_Timer &tm_;
};

class Slow_Read_Write_Test : public IO_Test
{
public:
  Slow_Read_Write_Test (char const *name,
                        ACE_Profile_Timer &tm);
  Slow_Read_Write_Test (Slow_Read_Write_Test const &) = delete;
  Slow_Read_Write_Test (Slow_Read_Write_Test &&) = delete;
  Slow_Read_Write_Test &operator= (Slow_Read_Write_Test const &) = delete;
  Slow_Read_Write_Test &operator= (Slow_Read_Write_Test &&) = delete;
  int run_test (int iterations,
                FILE *input_fp,
                FILE *output_fp) override;
};

class Stdio_Test : public IO_Test
{
public:
  Stdio_Test (char const *name,
              ACE_Profile_Timer &tm);
  Stdio_Test (Stdio_Test const &) = delete;
  Stdio_Test (Stdio_Test &&) = delete;
  Stdio_Test &operator= (Stdio_Test const &) = delete;
  Stdio_Test &operator= (Stdio_Test &&) = delete;
  int run_test (int iterations,
                FILE *input_fp,
                FILE *output_fp) override;
};

class Block_Read_Write_Test : public IO_Test
{
public:
  Block_Read_Write_Test (char const *name,
                         ACE_Profile_Timer &tm);
  Block_Read_Write_Test (Block_Read_Write_Test const &) = delete;
  Block_Read_Write_Test (Block_Read_Write_Test &&) = delete;
  Block_Read_Write_Test &operator= (Block_Read_Write_Test const &) = delete;
  Block_Read_Write_Test &operator= (Block_Read_Write_Test &&) = delete;
  int run_test (int iterations,
                FILE *input_fp,
                FILE *output_fp) override;
};

class Block_Fread_Fwrite_Test : public IO_Test
{
public:
  Block_Fread_Fwrite_Test (char const *name,
                           ACE_Profile_Timer &tm);
  Block_Fread_Fwrite_Test (Block_Fread_Fwrite_Test const &) = delete;
  Block_Fread_Fwrite_Test (Block_Fread_Fwrite_Test &&) = delete;
  Block_Fread_Fwrite_Test &operator= (Block_Fread_Fwrite_Test const &) = delete;
  Block_Fread_Fwrite_Test &operator= (Block_Fread_Fwrite_Test &&) = delete;
  int run_test (int iterations,
                FILE *input_fp,
                FILE *output_fp) override;
};

class Mmap1_Test : public IO_Test
{
public:
  Mmap1_Test (char const *name,
              ACE_Profile_Timer &tm);
  Mmap1_Test (Mmap1_Test const &) = delete;
  Mmap1_Test (Mmap1_Test &&) = delete;
  Mmap1_Test &operator= (Mmap1_Test const &) = delete;
  Mmap1_Test &operator= (Mmap1_Test &&) = delete;
  int run_test (int iterations,
                FILE *input_fp,
                FILE *output_fp) override;
};

class Mmap2_Test : public IO_Test
{
public:
  Mmap2_Test (char const *name,
              ACE_Profile_Timer &tm);
  Mmap2_Test (Mmap2_Test const &) = delete;
  Mmap2_Test (Mmap2_Test &&) = delete;
  Mmap2_Test &operator= (Mmap2_Test const &) = delete;
  Mmap2_Test &operator= (Mmap2_Test &&) = delete;
  int run_test (int iterations,
                FILE *input_fp,
                FILE *output_fp) override;
};

