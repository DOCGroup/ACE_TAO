// $Id$

#include "Parse_Headers.h"


// Implementation of class Headers

Headers::Headers (void) : done_(0)
{
}

Headers::~Headers (void)
{
}

void
Headers::recognize (const char * const header)
{
  (void)this->map_[header];
}

void
Headers::parse_header_line (char * const header_line)
{
  char *ptr = header_line;
  char *buf = header_line;
  int offset = 1;

  ptr = ACE_OS::strchr (header_line, '\n');

  if (ptr > header_line && ptr[-1] == '\r')
    {
      ptr--;
      offset++;
    }

  if (ptr == header_line) 
    {
      this->done_ = 1;
      return;
    }

  *ptr = '\0';
  ptr += offset;

  char *value;
  char *header = ACE_OS::strtok_r (buf, ":", &value);

  if (header != NULL && this->map_.mapped (header)) 
    {
      while (isspace (*value))
	value++;

      this->map_[header] = value;
    }

  // Write back the unused portion of the input.
  ACE_OS::memmove (header_line, ptr, strlen(ptr) + 1);
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
    return 0;

  if (ptr == header_line) 
    {
      ACE_OS::memmove (ptr, ptr+offset, ACE_OS::strlen (ptr + offset) + 1);
      this->done_ = 1;
      ACE_DEBUG ((LM_DEBUG, "  (%t) no more headers\n"));
      return 0;
    }

  do 
    {
      switch (ptr[offset]) 
	{
	case ' ':
	case '\t':
	  ACE_OS::memmove (ptr, ptr+offset, ACE_OS::strlen (ptr + offset) + 1);
	  break;

	case '\n':
	case '\r':
	  return 1;

	default:
	  if (isalpha (ptr[offset])) 
	    return 1;
	  else 
	    return -1;
	}
    } 
  while (this->end_of_line (ptr, offset) != 0);

  return 0;
}

int
Headers::end_of_headers (void) const
{
  return this->done_;
}

Headers_Map::Map_Item &
Headers::operator[] (const char * const header)
{
  return this->map_[header];
}

const Headers_Map::Map_Item &
Headers::operator[] (const char * const header) const
{
  return this->map_[header];
}

int
Headers::end_of_line (char * &line, int &offset) const
{
  const char *old_line = line;
  char *ptr = ACE_OS::strchr (old_line, '\n');

  if (ptr == NULL)
    return 0;

  line = ptr;
  offset = 1;

  if (line > old_line
      && line[-1] == '\r')
    {
      line--;
      offset = 2;
    }

  return 1;
}


// Implementation of class Headers_Map

Headers_Map::Headers_Map (void) 
  : num_headers_(0)
{
}

Headers_Map::~Headers_Map (void)
{
}

Headers_Map::Map_Item::Map_Item (void) 
  : header_(0),
    value_(0),
    no_value_("")
{
}

Headers_Map::Map_Item::~Map_Item (void)
{
  ACE_OS::free ((void *) this->header_);
  ACE_OS::free ((void *) this->value_);
}

Headers_Map::Map_Item::operator const char * (void) const
{
  return this->value_ == NULL ? this->no_value_ : this->value_;
}

Headers_Map::Map_Item &
Headers_Map::Map_Item::operator= (char * value)
{
  ACE_OS::free ((void *) this->value_);
  this->value_ = ACE_OS::strdup (value);
  return *this;
}

Headers_Map::Map_Item &
Headers_Map::Map_Item::operator= (const char * value)
{
  ACE_OS::free ((void *) this->value_);
  this->value_ = ACE_OS::strdup (value);
  return *this;
}

Headers_Map::Map_Item &
Headers_Map::Map_Item::operator= (const Headers_Map::Map_Item & mi)
{
  ACE_OS::free ((void *) this->value_);
  this->value_ = ACE_OS::strdup (mi.value_);
  return *this;
}

Headers_Map::Map_Item &
Headers_Map::operator[] (const char * const header)
{
  Headers_Map::Map_Item *item_ptr;

  item_ptr = this->find (header);

  if (item_ptr == NULL)
    item_ptr = this->place (header);

  return *item_ptr;
}

const Headers_Map::Map_Item &
Headers_Map::operator[] (const char * const header) const
{
  Headers_Map::Map_Item *item_ptr;
  Headers_Map *mutable_this = (Headers_Map *)this;

  item_ptr = this->find (header);

  if (item_ptr == NULL)
    item_ptr = mutable_this->place (header);

  return *item_ptr;
}

int
Headers_Map::mapped (const char * const header) const
{
  return this->find (header) != NULL;
}

Headers_Map::Map_Item *
Headers_Map::find (const char * const header) const
{
  Headers_Map *const mutable_this = (Headers_Map *) this;

  mutable_this->garbage.header_ = header;
  Headers_Map::Map_Item *mi_ptr = (Headers_Map::Map_Item *)
    ::bsearch (&this->garbage,
	       this->map_,
	       this->num_headers_,
               sizeof (Headers_Map::Map_Item),
	       Headers_Map::compare);

  mutable_this->garbage.header_ = 0;
  return mi_ptr;
}

Headers_Map::Map_Item *
Headers_Map::place (const char *const header)
{
  ACE_OS::free ((void *) this->map_[this->num_headers_++].header_);
  this->map_[this->num_headers_++].header_ = ACE_OS::strdup(header);

  int i = this->num_headers_ - 1;
  Headers_Map::Map_Item temp_item;

  while (i > 0)
    {
      if (Headers_Map::compare (&this->map_[i], &this->map_[i - 1]) > 0)
        break;
      ACE_OS::memcpy (&temp_item,
                      &(this->map_[i-1]),
                      sizeof (Headers_Map::Map_Item));
      ACE_OS::memcpy (&(this->map_[i-1]),
                      &(this->map_[i]),
                      sizeof (Headers_Map::Map_Item));
      ACE_OS::memcpy (&(this->map_[i]),
                      &temp_item,
                      sizeof (Headers_Map::Map_Item));
      i--;
    }

  return &this->map_[i];
}

int
Headers_Map::compare (const void *item1,
		      const void *item2)
{
  Headers_Map::Map_Item *a, *b;

  a = (Headers_Map::Map_Item *) item1;
  b = (Headers_Map::Map_Item *) item2;

  return ACE_OS::strcasecmp (a->header_ ? a->header_ : "",
			     b->header_ ? a->header_ : "");
}
