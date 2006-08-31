#ifndef _CUTS_UTIL_FUNCTOR_T_H_
#define _CUTS_UTIL_FUNCTOR_T_H_

template <typename ELEMENT>
struct Delete_Element_T
{
  void operator () (ELEMENT element)
  {
    delete element;
  }
};

template <typename ELEMENT>
struct Delete_Element_Array
{
  void operator () (ELEMENT element)
  {
    delete [] element;
  }
};

#endif  // !defined _CUTS_UTIL_FUNCTOR_T_H_
