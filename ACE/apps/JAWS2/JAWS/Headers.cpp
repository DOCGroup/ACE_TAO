#include "JAWS/Headers.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"

// Header Data

JAWS_Header_Data::JAWS_Header_Data (const char *name, const char *value,
                                    int type)
  : header_name_ (name ? ACE_OS::strdup (name) : nullptr),
    header_value_ (value ? ACE_OS::strdup (value) : nullptr),
    header_type_ (type)
{
}

JAWS_Header_Data::JAWS_Header_Data (const char *name, int type,
                                    const char *value)
  : header_name_ (name ? ACE_OS::strdup (name) : nullptr),
    header_value_ (value ? ACE_OS::strdup (value) : nullptr),
    header_type_ (type)
{
}

JAWS_Header_Data::~JAWS_Header_Data ()
{
  ACE_OS::free (this->header_name_);
  ACE_OS::free (this->header_value_);
}

const char *
JAWS_Header_Data::header_name () const
{
  return this->header_name_;
}

const char *
JAWS_Header_Data::header_value () const
{
  return this->header_value_;
}

int
JAWS_Header_Data::header_type () const
{
  return this->header_type_;
}

void
JAWS_Header_Data::header_name (const char *name)
{
  ACE_OS::free (this->header_name_);
  this->header_name_ = name ? ACE_OS::strdup (name) : nullptr;
}

void
JAWS_Header_Data::header_value (const char *value)
{
  ACE_OS::free (this->header_value_);
  this->header_value_ = value ? ACE_OS::strdup (value) : nullptr;
}

void
JAWS_Header_Data::header_type (int type)
{
  this->header_type_ = type;
}

// Header Table

JAWS_Headers::JAWS_Headers ()
  : iter_ (*this)
{
}

JAWS_Headers::~JAWS_Headers ()
{
}

JAWS_Header_Table_Iterator &
JAWS_Headers::iter ()
{
  return this->iter_;
}

int
JAWS_Headers::insert (JAWS_Header_Data *new_data)
{
  // Since there may be duplicate header entries, we don't worry about
  // doing this find anymore.  Make the application developer figure
  // out how to interpret duplicate entries.

  return (JAWS_Header_Table::insert_tail (new_data) ? 0 : -1);
}

JAWS_Header_Data *
JAWS_Headers::find (const char *const &header_name)
{
  this->iter_.first ();
  return this->find_next (header_name);
}

JAWS_Header_Data *
JAWS_Headers::find_next (const char *const &header_name)
{
  JAWS_Header_Data *data {};
  JAWS_Header_Table_Iterator &i = this->iter_;

  while (! i.done ())
    {
      data = i.next ();
      if (data != nullptr)
        {
          if (ACE_OS::strcasecmp (data->header_name (), header_name) != 0)
            data = nullptr;
        }
      i.advance ();
      if (data != nullptr)
        break;
    }

  return data;
}

void
JAWS_Headers::remove_all (const char *const &header_name)
{
  JAWS_Header_Data *data;
  int done;

  do
    {
      JAWS_Header_Table_Iterator i (*this);
      i.first ();
      done = 1;
      while (! i.done ())
        {
          data = i.next ();
          if (data != nullptr
              && ACE_OS::strcasecmp (data->header_name (), header_name) == 0)
            {
              i.remove ();
              delete data;
              done = 0;
              break;
            }
          else
            i.advance ();
        }
    }
  while (! done);
}

