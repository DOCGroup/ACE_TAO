// -*- C++ -*-
// $Id$

#include "PSDL_Stream.h"



TAO_PSDL_Stream::TAO_PSDL_Stream (void)
{
}

TAO_PSDL_Stream::~TAO_PSDL_Stream (void)
{
}

int
TAO_PSDL_Stream::open (const char *fname)
{
  if (fname != 0)
    {
      // File name exists, open an I/O file handle.
      this->fp_ = ACE_OS::fopen (fname, "w");

      if (this->fp_ != 0)
        {
          return 0;
        }
      else
        {
          return -1;
        }
    }
  else
    {
      return -1;
    }
}

// Return the underlying lowlevel file pointer.
// indentation.
FILE *
TAO_PSDL_Stream::file (void)
{
  return this->fp_;
}

int
TAO_PSDL_Stream::incr_indent (unsigned short flag)
{
  indent_level_++;

  if (flag != 0)
    {
      return this->indent ();
    }
  else
    {
      // Do not indent output.
      return 0;
    }
}

// Indentation
int
TAO_PSDL_Stream::decr_indent (unsigned short flag)
{
  this->indent_level_--;
  // Just in case somebody gets "unindent happy".
  if (this->indent_level_ < 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "negative indentation?\n"));
      this->indent_level_ = 0;
    }

  if (flag != 0)
    {
      return this->indent ();
    }
  else
    {
      // Do not indent output.
      return 0;
    }
}

int
TAO_PSDL_Stream::reset (void)
{
  this->indent_level_ = 0;
  return 0;
}

// Indented print.
int
TAO_PSDL_Stream::indent (void)
{
  // Based on the current indentation level, leave appropriate number of blank
  // spaces in the output.
  if (this->indent_level_ > 0)
    {
      for (int i = 0; i < this->indent_level_; i++)
        {
          ACE_OS::fprintf (this->fp_, "  ");
          ACE_OS::fflush (this->fp_);
        }
    }

  return 0;
}

int
TAO_PSDL_Stream::nl (void)
{
  ACE_OS::fprintf (this->fp_, "\n");
  this->indent ();
  return 0;
}

TAO_PSDL_Stream &
TAO_PSDL_Stream::operator<< (const char *str)
{
  ACE_OS::fprintf (this->fp_, "%s", str);
  ACE_OS::fflush (this->fp_);

  return *this;
}

TAO_PSDL_Stream &
TAO_PSDL_Stream::operator<< (ACE_CString str)
{
  ACE_OS::fprintf (this->fp_, "%s", str.c_str ());
  ACE_OS::fflush (this->fp_);

  return *this;
}

TAO_PSDL_Stream &
TAO_PSDL_Stream::operator<< (const unsigned long num)
{
  ACE_OS::fprintf (this->fp_,
                   "%lu",
                   num);

  ACE_OS::fflush (this->fp_);

  return *this;
}

TAO_PSDL_Stream &
TAO_PSDL_Stream::operator<< (const long num)
{
  ACE_OS::fprintf (this->fp_,
                   "%ld",
                   num);

  ACE_OS::fflush (this->fp_);
  return *this;
}
