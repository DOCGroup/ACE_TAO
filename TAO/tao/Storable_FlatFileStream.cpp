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
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace
{
  /// Avoids using fscanf to read an integer as any
  /// whitespace following the newline will be
  /// consumed. This could create problems if
  /// the data that follows the newline is binary.
  template<typename T>
  void read_integer(const char * format, T & i,
                    TAO::Storable_Base::Storable_State & state,
                    FILE * fl)
  {
    char buf[BUFSIZ];
    char * result = fgets (buf, BUFSIZ, fl);

    if (result == 0)
      {
        if (feof (fl))
          {
            state = TAO::Storable_Base::eofbit;
            return;
          }
        state = TAO::Storable_Base::badbit;
        return;
      }

    /// Consume any starting newline, as fscanf would
    /// do.
    if (buf[0] == '\n')
      {
        result = fgets (buf, BUFSIZ, fl);
      }

    if (result == 0)
      {
        if (feof (fl))
          {
            state = TAO::Storable_Base::eofbit;
            return;
          }
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

  int file_copy(FILE *f1, FILE *f2)
  {
    char buffer[BUFSIZ];
    size_t n_read;

    bool all_read = false;
    bool some_read = false;

    while (!all_read)
      {
        n_read =
          ACE_OS::fread(buffer, 1, sizeof(buffer), f1);
        if (n_read > 0)
          {
            if (ACE_OS::fwrite(buffer, 1, n_read, f2) != n_read)
              return -1;
            some_read = true;
          }
        else if (!some_read)
          {
            // Nothing was read
            TAOLIB_ERROR ((LM_ERROR,
                        ACE_TEXT ("TAO: (%P|%t) ERROR: could not read from file\n")));

            if (ferror (f1))
              {
                TAOLIB_ERROR ((LM_ERROR,
                            "%p\n",
                            "fread error"));
              }
            return -1;
          }
        else
          {
            all_read = true;
          }
      }

    return 0;
  }

}

TAO::Storable_FlatFileStream::Storable_FlatFileStream (const ACE_CString & file,
                                                       const char * mode,
                                                       bool use_backup)
  : Storable_Base(use_backup)
  , filelock_ ()
  , fl_ (0)
  , file_(file)
  , mode_(mode)
{
  // filelock_ will be completely initialized in call to init ().
  filelock_.handle_ = ACE_INVALID_HANDLE;
  filelock_.lockname_ = 0;
}

TAO::Storable_FlatFileStream::~Storable_FlatFileStream ()
{
  if ( fl_ != 0 )
    this->close();
}

void
TAO::Storable_FlatFileStream::do_remove ()
{
  ACE_OS::unlink(file_.c_str());
}

int
TAO::Storable_FlatFileStream::exists ()
{
  // We could check the mode for this file, but for now just check exists
  return ! ACE_OS::access(file_.c_str(), F_OK);
}

int
TAO::Storable_FlatFileStream::open()
{
  // For now, three flags exist "r", "w",  and "c"
  int flags = 0;
  const char *fdmode = 0;
  if( ACE_OS::strchr(mode_.c_str(), 'r') )
    if( ACE_OS::strchr(mode_.c_str(), 'w') )
      flags = O_RDWR, fdmode = "w+";
    else
      flags = O_RDONLY, fdmode = "r";
  else
    flags = O_WRONLY, fdmode = "w";
  if( ACE_OS::strchr(mode_.c_str(), 'c') )
    flags |= O_CREAT;
#ifndef ACE_WIN32
  if( ACE_OS::flock_init (&filelock_, flags,
                          ACE_TEXT_CHAR_TO_TCHAR (file_.c_str()), 0666) != 0 )
    TAOLIB_ERROR_RETURN ((LM_ERROR,
                       "Cannot open file %s for mode %s: (%d) %s\n",
                       file_.c_str(), mode_.c_str(),
                       errno, ACE_OS::strerror(errno)),
                      -1);
#else
  if( (filelock_.handle_= ACE_OS::open (file_.c_str(), flags, 0)) == ACE_INVALID_HANDLE )
    TAOLIB_ERROR_RETURN ((LM_ERROR,
                       "Cannot open file %s for mode %s: (%d) %s\n",
                       file_.c_str(), mode_.c_str(),
                       ACE_ERRNO_GET, ACE_OS::strerror(ACE_ERRNO_GET)),
                      -1);
#endif
  this->fl_ = ACE_OS::fdopen(filelock_.handle_, ACE_TEXT_CHAR_TO_TCHAR (fdmode));
  if (this->fl_ == 0)
    TAOLIB_ERROR_RETURN ((LM_ERROR,
                       "Cannot fdopen file %s for mode %s: (%d) %s\n",
                       file_.c_str(), mode_.c_str(),
                       ACE_ERRNO_GET, ACE_OS::strerror(ACE_ERRNO_GET)),
                      -1);
  return 0;
}

int
TAO::Storable_FlatFileStream::close()
{
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
#if defined (ACE_WIN32)
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
#else
  if( ACE_OS::strcmp(mode_.c_str(), "r") == 0 )
    {
       if (ACE_OS::flock_rdlock(&filelock_, whence, start, len) != 0)
         TAOLIB_ERROR_RETURN ((LM_ERROR,
                            ACE_TEXT ("TAO (%P|%t) - ")
                            ACE_TEXT ("Storable_FlatFileStream::flock, ")
                            ACE_TEXT ("Error trying to get a read lock for file %s\n"),
                            file_.c_str ()),
                           -1);
    }

  else
    {
      if (ACE_OS::flock_wrlock(&filelock_, whence, start, len) != 0)
        TAOLIB_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("TAO (%P|%t) - ")
                           ACE_TEXT ("Storable_FlatFileStream::flock, ")
                           ACE_TEXT ("Error trying to get a write lock for file %s\n"),
                           file_.c_str ()),
                          -1);
    }

#endif
  return 0;
}

int
TAO::Storable_FlatFileStream::funlock (int whence, int start, int len)
{
#if defined (ACE_WIN32)
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
#else
  if (ACE_OS::flock_unlock(&filelock_, whence, start, len) != 0)
    TAOLIB_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("TAO (%P|%t) - ")
                       ACE_TEXT ("Storable_FlatFileStream::funlock, ")
                       ACE_TEXT ("Error trying to unlock file %s\n"),
                       file_.c_str ()),
                      -1);
#endif
  return 0;
}

time_t
TAO::Storable_FlatFileStream::last_changed(void)
{
  ACE_stat st;
  int result = filelock_.handle_ != ACE_INVALID_HANDLE ?
    ACE_OS::fstat(filelock_.handle_, &st) :
    ACE_OS::stat (file_.c_str (), &st);
  if (result != 0)
    {
      TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - ")
                  ACE_TEXT ("Storable_FlatFileStream::last_changed, ")
                  ACE_TEXT ("Error getting file information\n")));
      throw Storable_Exception (this->file_);
    }

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
  int n =
    ACE_OS::fprintf(this->fl_, ACE_SSIZE_T_FORMAT_SPECIFIER_ASCII "\n%s\n",
                    str.length(), str.c_str());
  if (n < 0)
    this->throw_on_write_error (badbit);

  return *this;
}

TAO::Storable_Base &
TAO::Storable_FlatFileStream::operator >> (ACE_CString& str)
{
  int bufSize = 0;
  ACE_CString::size_type const max_buf_len =
    ACE_Numeric_Limits<ACE_CString::size_type>::max ();
  int const max_int = ACE_Numeric_Limits<int>::max ();
  switch (fscanf(fl_, "%d\n", &bufSize))
    {
    case 0:
      this->throw_on_read_error (badbit);
      break;
    case EOF:
      this->throw_on_read_error (eofbit);
    }

  if (bufSize < 0
      || static_cast<ACE_CString::size_type> (bufSize) >= max_buf_len
      || bufSize == max_int)
    {
      this->throw_on_read_error (badbit);
    }
  {
    int strSize = bufSize + 1; // Account for newline
    ACE_Auto_Basic_Array_Ptr<char> str_array (new char[strSize]);
    str_array[0] = '\0';
    if (ACE_OS::fgets (str_array.get (),
                       strSize,
                       this->fl_) == 0
        && bufSize != 0)
      {
        this->throw_on_read_error (badbit);
      }
    str = ACE_CString (str_array.get (), 0, false);
  }

  return *this;
}

TAO::Storable_Base &
TAO::Storable_FlatFileStream::operator << (int i)
{
  int const n = ACE_OS::fprintf (this->fl_, "%d\n", i);
  if (n < 0)
    this->throw_on_write_error (badbit);
  return *this;
}

TAO::Storable_Base &
TAO::Storable_FlatFileStream::operator >> (int &i)
{
  Storable_State state = this->rdstate ();
  read_integer ("%d\n", i, state, fl_);
  this->throw_on_read_error (state);

  return *this;
}

TAO::Storable_Base &
TAO::Storable_FlatFileStream::operator << (unsigned int i)
{
  int const n = ACE_OS::fprintf (this->fl_, "%u\n", i);
  if (n < 0)
    this->throw_on_write_error (badbit);
  return *this;
}

TAO::Storable_Base &
TAO::Storable_FlatFileStream::operator >> (unsigned int &i)
{
  Storable_State state = this->rdstate ();
  read_integer ("%u\n", i, state, fl_);
  this->throw_on_read_error (state);

  return *this;
}

TAO::Storable_Base &
TAO::Storable_FlatFileStream::operator << (const TAO_OutputCDR & cdr)
{
  unsigned int const length = cdr.total_length ();
  *this << length;
  for (const ACE_Message_Block *i = cdr.begin (); i != 0; i = i->cont ())
    {
      const char *bytes = i->rd_ptr ();
      size_t const len = i->length ();
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
TAO::Storable_FlatFileStream::read (size_t size, char * bytes)
{
  return ACE_OS::fread (bytes, size, 1, fl_);
}

ACE_CString
TAO::Storable_FlatFileStream::backup_file_name ()
{
  return file_ + ".bak";
}

int
TAO::Storable_FlatFileStream::create_backup ()
{
  FILE * backup = ACE_OS::fopen (this->backup_file_name ().c_str (), "w");
  this->rewind();
  int result = file_copy(this->fl_, backup);
  if (result != 0)
    {
      TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO: (%P|%t) ERROR: Unable to create backup ")
                  ACE_TEXT ("of file\n%s\n"), file_.c_str ()));
    }
  ACE_OS::fclose (backup);
  return result;
}

void
TAO::Storable_FlatFileStream::remove_backup ()
{
  ACE_CString backup_name = this->backup_file_name ();

  if (ACE_OS::access (backup_name.c_str (), F_OK) == 0)
    {
      ACE_OS::unlink (backup_name.c_str ());
    }
}

int
TAO::Storable_FlatFileStream::restore_backup ()
{
  ACE_CString backup_name = this->backup_file_name ().c_str ();

  if (ACE_OS::access (backup_name.c_str (), F_OK))
    return -1;

  FILE * backup = ACE_OS::fopen (backup_name.c_str (),
                                 "r");
  this->rewind();
  int result = file_copy(backup, this->fl_);
  ACE_OS::fclose (backup);
  this->flush ();
  this->clear ();
  return result;
}

void
TAO::Storable_FlatFileStream::throw_on_read_error (Storable_State state)
{
  this->setstate (state);

  if (!this->good ())
    {
      throw Storable_Read_Exception (this->rdstate (), this->file_);
    }
}

void
TAO::Storable_FlatFileStream::throw_on_write_error (Storable_State state)
{
  this->setstate (state);

  if (!this->good ())
    {
      throw Storable_Write_Exception (this->rdstate (), this->file_);
    }
}

TAO::Storable_FlatFileFactory::Storable_FlatFileFactory(const ACE_CString & directory)
  : Storable_Factory ()
  , directory_(directory)
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
                                              const char * mode,
                                              bool use_backup)
{
  TAO::Storable_Base *stream = 0;
  ACE_CString path = this->directory_ + "/" + file;
  ACE_NEW_RETURN (stream,
                  TAO::Storable_FlatFileStream(path,
                                               mode,
                                               use_backup),
                  0);
  return stream;
}

TAO_END_VERSIONED_NAMESPACE_DECL
