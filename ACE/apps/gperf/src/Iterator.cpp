/**
 * Copyright (C) 1989 Free Software Foundation, Inc.
 * written by Douglas C. Schmidt (d.schmidt@vanderbilt.edu)
 *
 * This file is part of GNU GPERF.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "Iterator.h"
#include "ace/OS_NS_ctype.h"

/// Constructor for Iterator.
Iterator::Iterator (char *s,
                    int lo,
                    int hi,
                    int word_end,
                    int bad_val,
                    int key_end)
  : str (s),
    end (key_end),
    end_word (word_end),
    error_value (bad_val),
    hi_bound (hi),
    lo_bound (lo)
{
}

/// Provide an Iterator, returning the ``next'' value from the list of
/// valid values given in the constructor.
int
Iterator::operator() ()
{
  if (this->range_active_)
    {
      if (++this->current_value_ >= this->upper_bound_)
        {
          this->range_active_ = false;
        }
      return this->current_value_;
    }
  else
    {
      while (*str)
        switch (*str)
          {
          default: return error_value;
          case ',': ++str; break;
          case '$': ++str; return end_word;
          case '0': case '1': case '2': case '3': case '4':
          case '5': case '6': case '7': case '8': case '9':
            for (this->current_value_ = 0; ACE_OS::ace_isdigit (*str); ++str)
              {
                this->current_value_ = this->current_value_ * 10 + *str - '0';
              }

            if (*str == '-')
              {
                this->range_active_ = true;
                this->upper_bound_ = 0;
                for (; ACE_OS::ace_isdigit (*++str);
                     this->upper_bound_ = this->upper_bound_ * 10 + *str - '0')
                  {
                    // Do nothing.
                  }

                if (this->upper_bound_ <= this->current_value_ || this->upper_bound_ > hi_bound)
                  {
                    return error_value;
                  }
              }

            return this->current_value_ >= lo_bound && this->current_value_ <= hi_bound
                   ? this->current_value_
                   : error_value;
          }

      return end;
    }
}
