// $Id$

#include "ACEXML/common/FileCharStream.h"
#include "ace/ACE.h"

ACEXML_FileCharStream::ACEXML_FileCharStream (void)
  : filename_ (0),
    infile_ (NULL)
{
}

ACEXML_FileCharStream::ACEXML_FileCharStream (const ACEXML_Char *name)
  : filename_ (),
    infile_ (NULL)
{
  this->open (name);
}


ACEXML_FileCharStream::~ACEXML_FileCharStream (void)
{
  if (this->infile_ != NULL)
    ACE_OS::fclose (this->infile_);
  delete this->filename_;
}

int
ACEXML_FileCharStream::open (const ACEXML_Char *name)
{
  delete this->filename_;
  this->filename_ = 0;

  this->infile_ = ACE_OS::fopen (name, ACE_LIB_TEXT ("r"));
  if (this->infile_ == NULL)
    return -1;

  ACE_stat statbuf;
  if (ACE_OS::stat (name, &statbuf) < 0)
    return -1;

  this->size_ = statbuf.st_size;
  this->filename_ = ACE::strnew (name);
  return 0;
}

int
ACEXML_FileCharStream::available (void)
{
  long curr;
  if ((curr = ACE_OS::ftell (this->infile_)) < 0)
    return -1;
  return (this->size_ - curr);
}

int
ACEXML_FileCharStream::close (void)
{
  delete this->filename_;
  this->filename_ = 0;
  ACE_OS::fclose (this->infile_);
  this->infile_ = NULL;
  this->size_ = 0;
  return 0;
}

int
ACEXML_FileCharStream::get (ACEXML_Char& ch)
{
  // @@ Error checking?
  ch = (unsigned char) fgetc (this->infile_);

  return (((char) ch) == EOF ? -1 :0);
}

int
ACEXML_FileCharStream::read (ACEXML_Char *str,
                             size_t len)
{
  return ACE_OS::fread (str, len, 1, this->infile_);
}

int
ACEXML_FileCharStream::peek (void)
{
  ACEXML_Char ch = ::fgetc (this->infile_);
  ::ungetc (ch, this->infile_);
  return ch;
}
