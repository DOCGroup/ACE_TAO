// $Id$

#define ACE_BUILD_DLL
#include "ace/Read_Buffer.h"
#include "ace/Service_Config.h"

#if !defined (__ACE_INLINE__)
#include "ace/Read_Buffer.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Read_Buffer, "$Id$")

void
ACE_Read_Buffer::dump (void) const
{
  ACE_TRACE ("ACE_Read_Buffer::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("size_ = %d"), this->size_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\noccurrences_ = %d"), this->occurrences_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nstream_ = %x"), this->stream_));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\nallocator_ = %x"), this->allocator_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Read_Buffer::ACE_Read_Buffer (FILE *fp,
                                  int close_on_delete,
                                  ACE_Allocator *alloc)
  : stream_ (fp),
    close_on_delete_ (close_on_delete),
    allocator_ (alloc)
{
  ACE_TRACE ("ACE_Read_Buffer::ACE_Read_Buffer");
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();
}

ACE_Read_Buffer::ACE_Read_Buffer (ACE_HANDLE handle,
                                  int close_on_delete,
                                  ACE_Allocator *alloc)
  : stream_ (ACE_OS::fdopen (handle, "r")),
    close_on_delete_ (close_on_delete),
    allocator_ (alloc)
{
  ACE_TRACE ("ACE_Read_Buffer::ACE_Read_Buffer");

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();
}

ACE_Read_Buffer::~ACE_Read_Buffer (void)
{
  ACE_TRACE ("ACE_Read_Buffer::~ACE_Read_Buffer");

  if (this->close_on_delete_)
    ACE_OS::fclose (this->stream_);
}

// Input: term        the character to terminate on
//        search      the character to search for
//        replace     the character with which to replace search
// Output: a buffer containing the contents of stream
// Method: call the recursive helper function read_helper

char *
ACE_Read_Buffer::read (int term, int search, int replace)
{
  ACE_TRACE ("ACE_Read_Buffer::read");
  this->occurrences_ = 0;
  this->size_ = 0;
  return this->rec_read (term, search, replace);
}

// Input: term       the termination character
//        search     the character to search for
//        replace    the character with which to replace search
// Purpose: read in a file to a buffer using only a single dynamic
//          allocation.
// Method: read until the local buffer is full and then recurse.
//         Must continue until the termination character is reached.
//         Allocate the final buffer based on the number of local
//         buffers read and as the recursive calls bottom out,
//         copy them in reverse order into the allocated buffer.

char *
ACE_Read_Buffer::rec_read (int term, int search, int replace)
{
  ACE_TRACE ("ACE_Read_Buffer::rec_read");
  // This is our temporary workspace.
  char buf[BUFSIZ];

  int c = EOF;
  size_t index = 0;
  int done = 0;

  // Read in the file char by char
  while (index < BUFSIZ)
    {
      c = getc (this->stream_);

      // Don't insert EOF into the buffer...
      if (c == EOF)
        {
          if (index == 0)
            return 0;
          else
            {
              ungetc (c, this->stream_);
              break;
            }
        }
      else if (c == term)
        done = 1;

      // Check for possible substitutions.
      if (c == search)
        {
          this->occurrences_++;

          if (replace >= 0)
            c = replace;
        }

      buf[index++] = (char) c;

      // Substitutions must be made before checking for termination.
      if (done)
        break;
    }

  // Increment the number of bytes.
  this->size_ += index;

  char *result;

  // Recurse, when the recursion bottoms out, allocate the result
  // buffer.
  if (done || c == EOF)
    {
      // Use the allocator to acquire the memory.  The + 1 allows
      // space for the null terminator.
      result = (char *) this->allocator_->malloc (this->size_ + 1);

      if (result == 0)
        {
          errno = ENOMEM;
          return 0;
        }
      result += this->size_;
    }
  else if ((result = this->rec_read (term, search, replace)) == 0)
    return 0;

  // Null terminate the buffer.
  *result = '\0';

  // Copy buf into the appropriate location starting from end of
  // buffer.  Peter says this is confusing and that we should use
  // memcpy() ;-)
  for (size_t j = index; j > 0; j--)
    *--result = buf[j - 1];

  return result;
}
