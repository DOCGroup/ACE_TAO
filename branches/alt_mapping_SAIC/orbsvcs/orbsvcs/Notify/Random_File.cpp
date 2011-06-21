// $Id$

#include "orbsvcs/Notify/Random_File.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_fcntl.h"
#include "tao/debug.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Guard_T.h"

//#define DEBUG_LEVEL 9
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{

Random_File::Random_File()
  : block_size_(512)
{
}

Random_File::~Random_File()
{
  this->close();
}

size_t
Random_File::block_size() const
{
  return this->block_size_;
}

ACE_OFF_T
Random_File::size() const
{
  Random_File * const mutable_this = const_cast<Random_File *> (this);
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);
  ACE_OFF_T original_pos = mutable_this->tell ();
  mutable_this->ACE_FILE::seek(0, SEEK_END);
  ACE_OFF_T cursize = mutable_this->tell();
  mutable_this->ACE_FILE::seek (original_pos, SEEK_SET);
  if ((cursize % this->block_size_) != 0)
  {
    cursize += this->block_size_;
  }
  return cursize / this->block_size_;
}

bool
Random_File::open(const ACE_TCHAR* filename, size_t block_size)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, false);
  this->block_size_ = block_size;
  bool result = (this->close() == 0);

  if (result)
  {
    if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Opening file %s\n")
      , filename
      ));
    ACE_HANDLE handle = ACE_OS::open(filename,
      O_CREAT | O_RDWR | O_BINARY,
      ACE_DEFAULT_FILE_PERMS);

    if (handle == ACE_INVALID_HANDLE)
    {
      result = false;
    }
    else
    {
      this->set_handle(handle);
      if (this->get_handle() == 0)
      {
        result = false;
      }
      else
      {
        result = (this->addr_.set(filename) == 0);
      }
    }
  }
  return result;
}

bool
Random_File::write(const size_t block_number, void* buf, bool atomic)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, false);
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P|%t) Write block %B %c\n"),
    block_number,
    (atomic ? '*' : ' ')
    ));
  bool result = this->seek(block_number);
  if (result)
  {
    if (atomic)
    {
      // sync before so that any block pointed to from this block
      // will be there when this block is written.
      result = sync();
    }
    // ACE uses an ssize_t for buffer size, so we do this to make it happy.
    ssize_t block_size = this->block_size_;
    if (result && (block_size !=
      ACE_OS::write(this->get_handle(), buf, block_size)))
    {
      result = false;
    }
    if (result && atomic)
    {
      // sync after to provide the caller with a guarantee that
      // this block is physically written to the storage device.
      result = sync();
    }
  }
  return result;
}

bool
Random_File::read(const size_t block_number, void* buf)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, false);
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P|%t) Read block %B\n"),
    block_number
    ));
  bool result = this->seek(block_number);
  if (result)
  {
    ssize_t block_size = this->block_size_;
    if (block_size !=
      ACE_OS::read(this->get_handle(), buf, block_size))
    {
      result = false;
    }
  }
  return result;
}

bool
Random_File::seek(const size_t block_number)
{
  ssize_t destloc = block_number * this->block_size_;
  bool result = (destloc == this->ACE_FILE::seek(destloc, SEEK_SET));
  return result;
}

bool
Random_File::sync()
{
  bool result = false;
  result = (0 == ACE_OS::fsync(this->get_handle()));
  return result;
}

} /* namespace TAO_Notify */

TAO_END_VERSIONED_NAMESPACE_DECL
