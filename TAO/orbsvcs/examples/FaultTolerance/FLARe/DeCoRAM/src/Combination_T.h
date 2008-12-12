// -*- C++ -*-

//=============================================================================
/**
 *  @file    Combination_T.h
 *
 *  $Id$
 *
 *  2003 - 2006 Wong Shao Voon
 *  No warranty, implied or expressed, is included.
 *  Author is not liable for any type of loss through 
 *  the use of this source code. Use it at your own risk!
 */
//=============================================================================

#ifndef COMBINATION_T_H_
#define COMBINATION_T_H_

template <class ITERATOR>
inline bool
next_combination (ITERATOR base_begin,
                  ITERATOR base_end,
                  ITERATOR comb_begin,
                  ITERATOR comb_end)
{
  bool marked = false;
  ITERATOR comb_mark;

  ITERATOR base_it = base_end;
  --base_it;

  ITERATOR temp_comb_end = comb_end;
  --temp_comb_end;

  for (ITERATOR it = temp_comb_end;
       it != comb_begin && base_it != base_begin;
       --it, --base_it)
    {
      if (*it == *base_it)
        {
          if (it != comb_begin)
            {
              marked = true;
              comb_mark = --it;
              ++it;
              continue;
            }
          else
            {
              return false;
            }
        }
      else
        {
          if (marked)
            {
              ITERATOR base_mark = 
                std::find (base_begin, base_end, *comb_mark);

              ITERATOR kt = ++base_mark;
              
              for (ITERATOR lt = comb_mark; lt != comb_end; ++lt, ++kt)
                {
                  *lt = *kt;
                }

              return true;
            }

          for (ITERATOR jt = base_begin; jt != base_end; ++jt)
            {
              if (*it == *jt)
                {
                  *it = *(++jt);

                  return true;
                }
            }
        }
    }

  return false;
}

#endif /* COMBINATION_T_H_ */
