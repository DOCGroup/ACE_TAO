/* -*- C++ -*- */
// $Id$

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
  IO_Test (const char *name, ACE_Profile_Timer &tm);

  // Return the name of the test
  const char *name (void);

  // Execute the IO test (note this is a pure virtual function...)
  virtual int run_test (int iterations, FILE *input_fp, FILE *output_fp) = 0;

protected:
  // Name of the test
  const char *name_;

  // Reference to a timer
  ACE_Profile_Timer &tm_;
};

class Slow_Read_Write_Test : public IO_Test
{
public:
  Slow_Read_Write_Test (char *name, ACE_Profile_Timer &tm);
  virtual int run_test (int iterations, FILE *input_fp, FILE *output_fp);
};

class Stdio_Test : public IO_Test
{
public:
  Stdio_Test (char *name, ACE_Profile_Timer &tm);
  virtual int run_test (int iterations, FILE *input_fp, FILE *output_fp);
};

class Block_Read_Write_Test : public IO_Test
{
public:
  Block_Read_Write_Test (char *name, ACE_Profile_Timer &tm);
  virtual int run_test (int iterations, FILE *input_fp, FILE *output_fp);
};

class Block_Fread_Fwrite_Test : public IO_Test
{
public:
  Block_Fread_Fwrite_Test (char *name, ACE_Profile_Timer &tm);
  virtual int run_test (int iterations, FILE *input_fp, FILE *output_fp);
};

class Mmap1_Test : public IO_Test
{
public:
  Mmap1_Test (char *name, ACE_Profile_Timer &tm);
  virtual int run_test (int iterations, FILE *input_fp, FILE *output_fp);
};

class Mmap2_Test : public IO_Test
{
public:
  Mmap2_Test (char *name, ACE_Profile_Timer &tm);
  virtual int run_test (int iterations, FILE *input_fp, FILE *output_fp);
};

