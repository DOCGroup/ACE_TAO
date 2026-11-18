#include "ace/Read_Buffer.h"

#include "ace/config-all.h"

#if !defined (__ACE_INLINE__)
#include "ace/Read_Buffer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Log_Category.h"
#include "ace/Malloc_Base.h"
#include "ace/Service_Config.h"
#include "ace/OS_NS_stdio.h"

#include <cstring>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

void
ACE_Read_Buffer_Unique_Ptr_Deleter::operator() (char *buffer) const
{
  if (buffer != nullptr)
    {
      ACE_Allocator * const allocator =
        this->allocator != nullptr ? this->allocator : ACE_Allocator::instance ();
      allocator->free (buffer);
    }
}

void
ACE_Read_Buffer::dump () const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Read_Buffer::dump");
  ACELIB_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACELIB_DEBUG ((LM_DEBUG,  ACE_TEXT ("size_ = %B"), this->size_));
  ACELIB_DEBUG ((LM_DEBUG,  ACE_TEXT ("\noccurrences_ = %d"), this->occurrences_));
  ACELIB_DEBUG ((LM_DEBUG,  ACE_TEXT ("\nstream_ = %@"), this->stream_));
  ACELIB_DEBUG ((LM_DEBUG,  ACE_TEXT ("\nallocator_ = %@"), this->allocator_));
  ACELIB_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

ACE_Read_Buffer::ACE_Read_Buffer (FILE *fp,
                                  bool close_on_delete,
                                  ACE_Allocator *alloc)
  : size_ (0),
    occurrences_ (0),
    stream_ (fp),
    close_on_delete_ (close_on_delete),
    allocator_ (alloc)
{
  ACE_TRACE ("ACE_Read_Buffer::ACE_Read_Buffer");
  if (this->allocator_ == nullptr)
    {
      this->allocator_ = ACE_Allocator::instance ();
    }
}

ACE_Read_Buffer::ACE_Read_Buffer (ACE_HANDLE handle,
                                  bool close_on_delete,
                                  ACE_Allocator *alloc)
  : size_ (0),
    occurrences_ (0),
    stream_ (ACE_OS::fdopen (handle, ACE_TEXT ("r"))),
    close_on_delete_ (close_on_delete),
    allocator_ (alloc)
{
  ACE_TRACE ("ACE_Read_Buffer::ACE_Read_Buffer");

  if (this->allocator_ == nullptr)
    {
      this->allocator_ = ACE_Allocator::instance ();
    }
}

ACE_Read_Buffer::~ACE_Read_Buffer ()
{
  ACE_TRACE ("ACE_Read_Buffer::~ACE_Read_Buffer");

  if (this->close_on_delete_ && this->stream_ != nullptr)
    {
      ACE_OS::fclose (this->stream_);
    }
}

/// Reads from the stream up to the termination character.
///
/// If @a search is nonnegative, its occurrences are replaced with @a replace.
/// The result is returned in a dynamically allocated buffer.
char *
ACE_Read_Buffer::read (int term, int search, int replace)
{
  ACE_TRACE ("ACE_Read_Buffer::read");
  this->occurrences_ = 0;
  this->size_ = 0;
  return this->rec_read (term, search, replace);
}

/// Reads through the stream using a single dynamic allocation.
///
/// Reads until the local buffer is full and then recurses, continuing until
/// the termination character is reached. It allocates the final buffer based
/// on the number of local buffers read and copies them in reverse order as the
/// recursive calls return.
char *
ACE_Read_Buffer::rec_read (int term, int search, int replace)
{
  ACE_TRACE ("ACE_Read_Buffer::rec_read");
  // This is our temporary workspace.
  char buf[BUFSIZ];

  int c {EOF};
  size_t slot {};
  bool done {};

  // Read in the file char by char
  while (slot < BUFSIZ)
    {
      c = ACE_OS::getc (this->stream_);

      // Don't insert EOF into the buffer...
      if (c == EOF)
        {
          ACE_OS::ungetc (c, this->stream_);
          break;
        }
      else if (c == term)
        {
          done = true;
        }

      // Check for possible substitutions.
      if (c == search)
        {
          ++this->occurrences_;

          if (replace >= 0)
            {
              c = replace;
            }
        }

      buf[slot++] = static_cast<char> (c);

      // Substitutions must be made before checking for termination.
      if (done)
        {
          break;
        }
    }

  // Increment the number of bytes.
  this->size_ += slot;

  // Don't bother going any farther if the total size is 0.
  if (this->size_ == 0)
    {
      return nullptr;
    }

  char *result {};

  // Recurse, when the recursion bottoms out, allocate the result
  // buffer.
  if (done || c == EOF)
    {
      // Use the allocator to acquire the memory.  The + 1 allows
      // space for the null terminator.
      result = static_cast<char *> (this->allocator_->malloc (this->size_ + 1));

      if (result == nullptr)
        {
          errno = ENOMEM;
          return nullptr;
        }
      result += this->size_;

      // Null terminate the buffer.
      *result = '\0';
    }
  else if ((result = this->rec_read (term, search, replace)) == nullptr)
    {
      return nullptr;
    }

  if (slot != 0)
    {
      result -= slot;
      std::memcpy (result, buf, slot);
    }

  return result;
}

ACE_END_VERSIONED_NAMESPACE_DECL
