// $Id$

#include "common/FileCharStream.h"
#include "ace/ACE.h"

ACEXML_FileCharStream::ACEXML_FileCharStream (void)
  : filename_ (0),
    istream_ ()
{
}

ACEXML_FileCharStream::ACEXML_FileCharStream (const ACEXML_Char *name)
  : filename_ (),
    istream_ ()
{
  this->open (name);
}


ACEXML_FileCharStream::~ACEXML_FileCharStream (void)
{
  delete this->filename_;
}

int
ACEXML_FileCharStream::open (const ACEXML_Char *name)
{
  delete this->filename_;
  this->filename_ = 0;

  this->istream_.open (name, ios::in);
  if (this->istream_.bad ())
    return -1;

  this->filename_ = ACE::strnew (name);
  return 0;
}

int
ACEXML_FileCharStream::available (void)
{
  // Should return # of char available.

  if (this->istream_.is_open ())
    return 0;

  return -1;
}

int
ACEXML_FileCharStream::close (void)
{
  delete this->filename_;
  this->filename_ = 0;
  this->istream_.close ();
  return 0;
}

int
ACEXML_FileCharStream::get (ACEXML_Char& ch)
{
  // @@ Error checking?
  ch = this->istream_.get ();
  return 0;
}

int
ACEXML_FileCharStream::read (ACEXML_Char *str,
                             size_t len)
{
  // @@ Error Checking?
  this->istream_.read (str, len);
  return 0;
}

int
ACEXML_FileCharStream::peek (void)
{
  return this->istream_.peek ();
}
