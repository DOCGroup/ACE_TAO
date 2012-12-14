// -*- C++ -*-

//=============================================================================
/**
 * @file  Storable_FlatFileStream.cpp
 *
 * $Id$
 *
 * @author Marina Spivak <marina@cs.wustl.edu>
 * @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#include "tao/Storable_FlatFileStream.h"

#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/Numeric_Limits.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace
{
  /// Avoids using fscanf to read an integer as any
  /// whitespace following the newline will be
  /// consumed. This could create problems if
  /// the data the follows the newline is binary.
  template<typename T>
  void read_integer(const char * format, T & i,
                    TAO::Storable_Base::Storable_State & state,
                    FILE * fl)
  {
    char buf[BUFSIZ];
    char * result = fgets (buf, BUFSIZ, fl);

    /// Consume any starting newline, as fscanf would
    /// do.
    if (buf[0] == '\n')
      {
        result = fgets (buf, BUFSIZ, fl);
      }

    if (feof (fl))
      {
        state = TAO::Storable_Base::eofbit;
        return;
      }

    if (result == NULL)
      {
        state = TAO::Storable_Base::badbit;
        return;
      }

    switch (sscanf (buf, format, &i))
      {
      case 0:
        state = TAO::Storable_Base::badbit;
        return;
      case EOF:
        state = TAO::Storable_Base::eofbit;
        return;
      }
  }

}

TAO::Storable_FlatFileStream::Storable_FlatFileStream (const ACE_CString & file,
                                                            const char * mode)
  : fl_ (0)
{
  ACE_TRACE("TAO::Storable_FlatFileStream");
  file_ = file;
  mode_ = mode;
}

TAO::Storable_FlatFileStream::~Storable_FlatFileStream ()
{
  ACE_TRACE("~Storable_FlatFileStream");
  if ( fl_ != 0 )
    this->close();
}

void
TAO::Storable_FlatFileStream::remove ()
{
  ACE_TRACE("remove");
  ACE_OS::unlink(ACE_TEXT_CHAR_TO_TCHAR(file_.c_str()));
}

int
TAO::Storable_FlatFileStream::exists ()
{
  ACE_TRACE("exists");
  // We could check the mode for this file, but for now just check exists
  return ! ACE_OS::access(file_.c_str(), F_OK);
}

int
TAO::Storable_FlatFileStream::open()
{
  ACE_TRACE("open");
  // For now, three flags exist "r", "w",  and "c"
  int flags = 0;
  const char *fdmode = 0;
  if( ACE_OS::strchr(mode_.c_str(), 'r') )
    if( ACE_OS::strchr(mode_.c_str(), 'w') )
      flags = O_RDWR, fdmode = "r+";
    else
      flags = O_RDONLY, fdmode = "r";
  else
    flags = O_WRONLY, fdmode = "w";
  if( ACE_OS::strchr(mode_.c_str(), 'c') )
    flags |= O_CREAT;
#ifndef ACE_WIN32
  if( ACE_OS::flock_init (&filelock_, flags, ACE_TEXT_CHAR_TO_TCHAR(file_.c_str()), 0666) != 0 )
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open file %s for mode %s: (%d) %s\n",
                       file_.c_str(), mode_.c_str(),
                       errno, ACE_OS::strerror(errno)),
                      -1);
#else
  if( (filelock_.handle_= ACE_OS::open (ACE_TEXT_CHAR_TO_TCHAR(file_.c_str()), flags, 0)) == ACE_INVALID_HANDLE )
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open file %s for mode %s: (%d) %s\n",
                       file_.c_str(), mode_.c_str(),
                       ACE_ERRNO_GET, ACE_OS::strerror(ACE_ERRNO_GET)),
                      -1);
#endif
  this->fl_ = ACE_OS::fdopen(filelock_.handle_, ACE_TEXT_CHAR_TO_TCHAR(fdmode));
  if (this->fl_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot fdopen file %s for mode %s: (%d) %s\n",
                       file_.c_str(), mode_.c_str(),
                       ACE_ERRNO_GET, ACE_OS::strerror(ACE_ERRNO_GET)),
                      -1);
  return 0;
}

int
TAO::Storable_FlatFileStream::close()
{
  ACE_TRACE("close");
  ACE_OS::fflush(fl_);
#ifndef ACE_WIN32
  ACE_OS::flock_destroy (&filelock_, 0);
#endif
  ACE_OS::fclose (fl_);  // even though flock_destroy closes the handle
                         // we still need to destroy the FILE*

  fl_ = 0;
  return 0;
}

int
TAO::Storable_FlatFileStream::flock (int whence, int start, int len)
{
  ACE_TRACE("flock");
#if defined (ACE_WIN32)
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
#else
  if( ACE_OS::strcmp(mode_.c_str(), "r") == 0 )
    ACE_OS::flock_rdlock(&filelock_, whence, start, len);
  else
    ACE_OS::flock_wrlock(&filelock_, whence, start, len);
#endif
  return 0;
}

int
TAO::Storable_FlatFileStream::funlock (int whence, int start, int len)
{
  ACE_TRACE("funlock");
#if defined (ACE_WIN32)
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
#else
  ACE_OS::flock_unlock(&filelock_, whence, start, len);
#endif
  return 0;
}

time_t
TAO::Storable_FlatFileStream::last_changed(void)
{
  ACE_TRACE("TAO::Storable_FlatFileStream::last_changed");
  ACE_stat st;
  ACE_OS::fstat(filelock_.handle_, &st);
  return st.st_mtime;
}

void
TAO::Storable_FlatFileStream::rewind (void)
{
  return ACE_OS::rewind(this->fl_);
}

bool
TAO::Storable_FlatFileStream::flush (void)
{
  return ACE_OS::fflush(this->fl_);
}

int
TAO::Storable_FlatFileStream::sync (void)
{
  return ACE_OS::fsync (this->filelock_.handle_);
}

TAO::Storable_Base &
TAO::Storable_FlatFileStream::operator << (const ACE_CString& str)
{
  ACE_TRACE("TAO::Storable_FlatFileStream::operator <<");
  ACE_OS::fprintf(this->fl_, ACE_SIZE_T_FORMAT_SPECIFIER ACE_TEXT("\n%s\n"),
                  str.length(), str.c_str());

  return *this;
}

TAO::Storable_Base &
TAO::Storable_FlatFileStream::operator >> (ACE_CString& str)
{
  ACE_TRACE("TAO::Storable_FlatFileStream::operator >>");
  int bufSize = 0;
  ACE_CString::size_type const max_buf_len =
    ACE_Numeric_Limits<ACE_CString::size_type>::max ();
  switch (fscanf(fl_, "%d\n", &bufSize))
    {
    case 0:
      this->setstate (badbit);
      return *this;
    case EOF:
      this->setstate (eofbit);
      return *this;
    }

  if (bufSize < 0
      || static_cast<ACE_CString::size_type> (bufSize) >= max_buf_len)
    {
      this->setstate (badbit);
      return *this;
    }
  {
    ACE_Auto_Basic_Array_Ptr<char> str_array (new char[bufSize + 1]);
    str_array[0] = '\0';
    if (ACE_OS::fgets (ACE_TEXT_CHAR_TO_TCHAR (str_array.get ()),
                       bufSize + 1,
                       this->fl_) == 0
        && bufSize != 0)
      {
        this->setstate (badbit);
        return *this;
      }
    str = ACE_CString (str_array.get (), 0, false);
  }

  return *this;
}

TAO::Storable_Base &
TAO::Storable_FlatFileStream::operator << (int i)
{
  ACE_TRACE("TAO::Storable_FlatFileStream::operator <<");
  ACE_OS::fprintf (this->fl_, "%d\n", i);
  return *this;
}

TAO::Storable_Base &
TAO::Storable_FlatFileStream::operator >> (int &i)
{
  ACE_TRACE("TAO::Storable_FlatFileStream::operator >>");

  Storable_State state = this->rdstate ();
  read_integer ("%d\n", i, state, fl_);
  this->setstate (state);

  return *this;
}

TAO::Storable_Base &
TAO::Storable_FlatFileStream::operator << (unsigned int i)
{
  ACE_TRACE("TAO::Storable_FlatFileStream::operator <<");
  ACE_OS::fprintf (this->fl_, "%u\n", i);
  return *this;
}

TAO::Storable_Base &
TAO::Storable_FlatFileStream::operator >> (unsigned int &i)
{
  ACE_TRACE("TAO::Storable_FlatFileStream::operator >>");

  Storable_State state = this->rdstate ();
  read_integer ("%u\n", i, state, fl_);
  this->setstate (state);

  return *this;
}

TAO::Storable_Base &
TAO::Storable_FlatFileStream::operator << (const TAO_OutputCDR & cdr)
{
  ACE_TRACE("TAO::Storable_FlatFileStream::operator <<");

  int length = cdr.total_length ();
  *this << length;
  for (const ACE_Message_Block *i = cdr.begin (); i != 0; i = i->cont ())
    {
      const char *bytes = i->rd_ptr ();
      size_t len = i->length ();
      this->write (len, bytes);
    }
  return *this;
}

size_t
TAO::Storable_FlatFileStream::write (size_t size, const char * bytes)
{
  return ACE_OS::fwrite (bytes, size, 1, fl_);
}

size_t
TAO::Storable_FlatFileStream::read (size_t size, char *& bytes)
{
  return ACE_OS::fread (bytes, size, 1, fl_);
}

TAO::Storable_FlatFileFactory::Storable_FlatFileFactory(const ACE_CString & directory)
: directory_(directory)
{
}

TAO::Storable_FlatFileFactory::~Storable_FlatFileFactory()
{
}

const ACE_CString &
TAO::Storable_FlatFileFactory::get_directory () const
{
  return directory_;
}

TAO::Storable_Base *
TAO::Storable_FlatFileFactory::create_stream (const ACE_CString & file,
                                              const ACE_TCHAR * mode)
{
  ACE_TRACE("TAO::Storable_FlatFileFactory::create_stream");
  TAO::Storable_Base *stream = 0;
  ACE_CString path = this->directory_ + "/" + file;
  ACE_NEW_RETURN (stream,
                  TAO::Storable_FlatFileStream(path, ACE_TEXT_ALWAYS_CHAR (mode)),
                  0);
  return stream;
}

TAO_END_VERSIONED_NAMESPACE_DECL
