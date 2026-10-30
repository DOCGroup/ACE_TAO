/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.
2550 Garcia Avenue
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

 */

// Implementation of a list of utl_string nodes.

// NOTE: This list class only works correctly because we use single public
//   inheritance, as opposed to multiple inheritance or public virtual.
//   It relies on a type-unsafe cast from UTL_List to subclasses, which
//   will cease to operate correctly if you use either multiple or
//   public virtual inheritance.

#include "utl_idlist.h"
#include "utl_identifier.h"
#include "ace/OS_Memory.h"

#include <optional>

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

// Constructor
UTL_IdList::UTL_IdList (Identifier *s,
                        UTL_IdList *cdr)
  : UTL_List (static_cast<UTL_List *> (cdr)),
    pd_car_data (s)
{
}

// Public operations

// Copy an IdList.
UTL_IdList *
UTL_IdList::copy ()
{
  UTL_IdList *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  UTL_IdList (this->head ()->copy (),
                              nullptr),
                  nullptr);

  if (this->tail () != nullptr)
    {
      retval->nconc (static_cast<UTL_IdList *> (this->tail ()->copy ()));
    }

  return retval;
}

// Get list item.
Identifier *
UTL_IdList::head ()
{
  return this->pd_car_data;
}

Identifier const *
UTL_IdList::head () const
{
  return this->pd_car_data;
}

// Get last item of this list.
Identifier *
UTL_IdList::last_component ()
{
  if (this->tail () == nullptr)
    {
      return this->head ();
    }

  return static_cast<UTL_IdList *> (this->tail ())->last_component ();
}

Identifier const *
UTL_IdList::last_component () const
{
  if (this->tail () == nullptr)
    {
      return this->head ();
    }

  return static_cast<UTL_IdList const *> (this->tail ())->last_component ();
}

// Get first item of this list holding a non-empty string.
Identifier *
UTL_IdList::first_component ()
{
  if (!this->pd_car_data->get_string ().empty ())
    {
      return this->pd_car_data;
    }

  return static_cast<UTL_IdList *> (this->tail ())->first_component ();
}

Identifier const *
UTL_IdList::first_component () const
{
  if (!this->pd_car_data->get_string ().empty ())
    {
      return this->pd_car_data;
    }

  return static_cast<UTL_IdList const *> (this->tail ())->first_component ();
}

int
UTL_IdList::compare (UTL_IdList const *other) const
{
  ACE_CDR::Long this_length = 0;
  UTL_IdList const *this_list = this;
  while (this_list != nullptr)
    {
      ++this_length;
      this_list = static_cast<UTL_IdList const *> (this_list->tail ());
    }

  // Strip away any leading empty segments.

  if (this->pd_car_data->get_string ().empty ())
    {
      UTL_List const *this_tail = this->tail ();

      if (this_tail == nullptr)
        {
          return 1;
        }

      return static_cast<UTL_IdList const *> (this_tail)->compare (other);
    }

  if (other->pd_car_data->get_string ().empty ())
    {
      UTL_List const *other_tail = other->tail ();

      if (other_tail == nullptr)
        {
          return 1;
        }

      return this->compare (static_cast<UTL_IdList const *> (other->tail ()));
    }

  ACE_CDR::Long other_length = 0;
  UTL_IdList const *other_list = other;
  while (other_list != nullptr)
    {
      ++other_length;
      other_list = static_cast<UTL_IdList const *> (other_list->tail ());
    }

  if (this_length != other_length)
    {
      return 1;
   }

  this_list = this;
  other_list = other;
  while (this_list != nullptr)
    {
      Identifier const *this_id = this_list->head ();
      Identifier const *other_id = other_list->head ();

      if (this_id->get_string () != other_id->get_string ())
        {
          return 1;
        }

      this_list = static_cast<UTL_IdList const *> (this_list->tail ());
      other_list = static_cast<UTL_IdList const *> (other_list->tail ());
    }

  return 0;
}

// AST Dumping.
void
UTL_IdList::dump (ACE_OSTREAM_TYPE &o)
{
  bool first = true;
  bool second = false;

  for (UTL_IdListActiveIterator i (this);
       !i.is_done ();
       i.next ())
    {
      if (!first)
        {
          o << "::";
        }
      else if (second)
        {
          first = second = false;
        }

      std::string const& item = i.item ()->get_string ();
      i.item ()->dump (o);

      if (first)
        {
          if (item != "::")
            {
              first = false;
            }
          else
            {
              second = true;
            }
        }
    }
}

void
UTL_IdList::destroy ()
{
  if (this->pd_car_data != nullptr)
    {
      this->pd_car_data->destroy ();
      delete this->pd_car_data;
      this->pd_car_data = nullptr;
    }

  this->UTL_List::destroy ();
}

// UTL_IdList active iterator.

// Constructor
UTL_IdListActiveIterator::UTL_IdListActiveIterator (UTL_IdList *s)
  : UTL_ListActiveIterator (s)
{
}

// Public operations.

// Get current item.
Identifier *
UTL_IdListActiveIterator::item () const
{
    if (this->source == nullptr)
      {
        return nullptr;
      }

    return static_cast<UTL_IdList *> (source)->head ();
}

std::string
UTL_IdList::get_string_copy () const
{
  /*
   * Absolute Names have "::" as the first item in the idlist, so delimiters
   * have to start be inserted depending on if the name is absolute or not
   */
  size_t delimiter_start = is_absolute () ? 1 : 0;

  std::string result;
  size_t n = 0;
  UTL_IdList const *item_list = this;
  while (item_list != nullptr)
    {
      if (n > delimiter_start)
        {
          result.append ("::");
        }
      result.append (item_list->head ()->get_string ());
      n++;
      item_list = static_cast<UTL_IdList const *> (item_list->tail ());
    }

  return result;
}

bool
UTL_IdList::is_absolute () const
{
  return first_component ()->get_string () == "::";
}
