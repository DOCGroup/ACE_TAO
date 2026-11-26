#include "ace/Log_Msg.h"

#include "Parse_Headers.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_ctype.h"
#include <cstring>
#include <memory>

// Implementation of class Headers

Headers::Headers () = default;

void
Headers::recognize (char const * const header)
{
  (void)this->map_[header];
}

void
Headers::parse_header_line (char * const header_line)
{
  char *buf = header_line;
  int offset = 1;

  char *ptr = std::strchr (header_line, '\n');

  if (ptr > header_line && ptr[-1] == '\r')
    {
      --ptr;
      ++offset;
    }

  if (ptr == header_line)
    {
      this->done_ = true;
      return;
    }

  *ptr = '\0';
  ptr += offset;

  char *value {};
  char *header = ACE_OS::strtok_r (buf, ":", &value);

  ACE_DEBUG((LM_DEBUG, " (%t) Headers::parse_header_line [%s]\n",
             header ? header : "<empty>"));

  if (header != nullptr && value != nullptr && this->map_.mapped (header))
    {
      while (ACE_OS::ace_isspace (*value))
        {
          ++value;
        }

      this->map_[header] = value;

      ACE_DEBUG((LM_DEBUG, " (%t) Headers::parse_header_line <%s>\n",
                 value));
    }

  // Write back the unused portion of the input.
  std::memmove (header_line, ptr, std::strlen (ptr) + 1);
}

int
Headers::complete_header_line (char *const header_line)
{
  // Algorithm --
  // Scan for end of line marker.
  // If the next character is linear white space, then unfold the header.
  // Else, if the next character is printable, we have a complete header line.
  // Else, presumably the next character is '\0', so the header is incomplete.

  // return -1 if end of line but not complete header line
  // return 0 if no end of line marker
  // return 1 if complete header line

  char *ptr = header_line;
  int offset;

  if (!this->end_of_line (ptr, offset))
  {
    return 0;
  }

  if (ptr == header_line)
    {
      std::memmove (ptr, ptr + offset, std::strlen (ptr + offset) + 1);
      this->done_ = true;
      ACE_DEBUG ((LM_DEBUG, "  (%t) no more headers\n"));
      return 0;
    }

  do
    {
      switch (ptr[offset])
        {
        case ' ':
        case '\t':
          std::memmove (ptr, ptr + offset, std::strlen (ptr + offset) + 1);
          break;

        case '\n':
        case '\r':
          return 1;

        default:
          if (ACE_OS::ace_isalpha (ptr[offset]))
            {
              return 1;
            }
          else
            {
              return -1;
            }
        }
    }
  while (this->end_of_line (ptr, offset) != 0);

  return 0;
}

int
Headers::end_of_headers () const
{
  return this->done_;
}

Headers_Map_Item &
Headers::operator[] (char const * const header)
{
  return this->map_[header];
}

Headers_Map_Item const &
Headers::operator[] (char const * const header) const
{
  return this->map_[header];
}

int
Headers::end_of_line (char *&line, int &offset) const
{
  char *old_line = line;
  char *ptr = std::strchr (old_line, '\n');

  if (ptr == nullptr)
  {
    return 0;
  }

  line = ptr;
  offset = 1;

  if (line > old_line
      && line[-1] == '\r')
    {
      --line;
      offset = 2;
    }

  return 1;
}

// Implementation of class Headers_Map

Headers_Map::Headers_Map () = default;

Headers_Map_Item::Headers_Map_Item () = default;

Headers_Map_Item::~Headers_Map_Item ()
{
  ACE_OS::free (const_cast<char *> (this->header_));
  ACE_OS::free (const_cast<char *> (this->value_));
  this->header_ = this->value_ = nullptr;
}

// Headers_Map_Item::operator const char * () const
// {
//   return this->value_ == 0 ? this->no_value_ : this->value_;
// }

Headers_Map_Item &
Headers_Map_Item::operator= (char * value)
{
  ACE_OS::free (const_cast<char *> (this->value_));
  this->value_ = ACE_OS::strdup (value);
  return *this;
}

Headers_Map_Item &
Headers_Map_Item::operator= (char const * value)
{
  ACE_OS::free (const_cast<char *> (this->value_));
  this->value_ = ACE_OS::strdup (value);
  return *this;
}

Headers_Map_Item &
Headers_Map_Item::operator= (Headers_Map_Item const & mi)
{
  ACE_OS::free (const_cast<char *> (this->value_));
  ACE_OS::free (const_cast<char *> (this->header_));
  this->header_ = ACE_OS::strdup (mi.header_);
  this->value_ = (mi.value_ ? ACE_OS::strdup (mi.value_) : nullptr);
  return *this;
}

char const *
Headers_Map_Item::header () const
{
  return this->header_;
}

char const *
Headers_Map_Item::value () const
{
  return this->value_;
}

Headers_Map_Item &
Headers_Map::operator[] (char const * const header)
{
  Headers_Map_Item *item_ptr;

  item_ptr = this->find (header);

  if (item_ptr == nullptr)
  {
    item_ptr = this->place (header);
  }

  return *item_ptr;
}

Headers_Map_Item const &
Headers_Map::operator[] (char const * const header) const
{
  Headers_Map_Item *item_ptr;
  Headers_Map *mutable_this = const_cast<Headers_Map *> (this);

  item_ptr = this->find (header);

  if (item_ptr == nullptr)
  {
    item_ptr = mutable_this->place (header);
  }

  return *item_ptr;
}

int
Headers_Map::mapped (char const * const header) const
{
  int result = this->find (header) != nullptr;

  return result;
}

Headers_Map_Item *
Headers_Map::find (char const * const header) const
{
  Headers_Map *const mutable_this = const_cast<Headers_Map *> (this);

  mutable_this->garbage_.header_ = header;
#if 0
  Headers_Map_Item *mi_ptr = static_cast<Headers_Map_Item *> (
    ACE_OS::bsearch (std::addressof (this->garbage_),
                     this->map_,
                     this->num_headers_,
                     sizeof (Headers_Map_Item),
                     Headers_Map::compare));
#else
  int i = 0;
  int j = this->num_headers_;

  while (i < j-1)
    {
      int k = (i+j)/2;
      if (Headers_Map::compare (std::addressof (this->garbage_), this->map_+k) < 0)
        {
          j = k;
        }
      else
        {
          i = k;
        }
    }

  Headers_Map_Item *mi_ptr = mutable_this->map_ + i;
  if (Headers_Map::compare (std::addressof (this->garbage_), mi_ptr) != 0)
  {
    mi_ptr = nullptr;
  }
#endif

  mutable_this->garbage_.header_ = nullptr;

  return mi_ptr;
}

Headers_Map_Item *
Headers_Map::place (char const *const header)
{
  this->garbage_.header_ = ACE_OS::strdup (header);

  int i = this->num_headers_++;
  ACE_OS::free (const_cast<char *> (this->map_[i].header_));
  ACE_OS::free (const_cast<char *> (this->map_[i].value_));
  this->map_[i].header_ = nullptr;
  this->map_[i].value_ = nullptr;
  Headers_Map_Item temp_item;

  while (i > 0)
    {
      if (Headers_Map::compare (std::addressof (this->garbage_),
                                std::addressof (this->map_[i - 1])) > 0)
      {
        break;
      }

      this->map_[i].header_ = this->map_[i - 1].header_;
      this->map_[i].value_ = this->map_[i - 1].value_;
      this->map_[i - 1].header_ = nullptr;
      this->map_[i - 1].value_ = nullptr;

      --i;
    }

  this->map_[i].header_ = this->garbage_.header_;
  this->map_[i].value_ = this->garbage_.value_;

  this->garbage_.header_ = nullptr;

  return std::addressof (this->map_[i]);
}

int
Headers_Map::compare (void const *item1,
                      void const *item2)
{
  Headers_Map_Item const *a, *b;
  int result;

  a = static_cast<Headers_Map_Item const *> (item1);
  b = static_cast<Headers_Map_Item const *> (item2);

  if (a->header_ == nullptr || b->header_ == nullptr)
    {
      if (a->header_ == nullptr && b->header_ == nullptr)
        {
          result = 0;
        }
      else if (a->header_ == nullptr)
        {
          result = 1;
        }
      else
        {
          result = -1;
        }
    }
  else
  {
    result = ACE_OS::strcasecmp (a->header_, b->header_);
  }

  return (result < 0) ? -1 : (result > 0);
}
