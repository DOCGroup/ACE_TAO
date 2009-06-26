//
// $Id$
//

//=============================================================================
/**
 * @file  SA_POP_Types.cpp
 *
 * This file contains the implementation of class types used throughout SA-POP.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_UTILS_CPP_
#define SA_POP_UTILS_CPP_

#include "SA_POP_Utils.h"

//using namespace SA_POP;

template<typename L, typename T>
SA_POP::ListMultiMap<L,T>::ListMultiMap (SA_POP::FrontBack append_direction, SA_POP::FrontBack remove_direction)
: append_dir_ (append_direction),
remove_dir_(remove_direction)
{
    multimap_.clear();
    list_.clear ();
};


// Map Insert
template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::iterator SA_POP::ListMultiMap<L,T>::insert(const typename SA_POP::ListMultiMap<L,T>::value_type val)
{
  
  if(append_dir_ == SA_POP::BACK)
  {
    this->list_.push_back(val);
  }
  else if(append_dir_ == SA_POP::FRONT)
  {
    this->list_.push_front(val);
  }
  else 
  {
    throw "ListMultiMap<L,T>::iterator ListMultiMap<L,T>::insert(const typename ListMultiMap<L,T>::value_type val): Unknown direction to append.";
  }

  return this->multimap_.insert (val);
  

};

    
// Map Erase
template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::size_type SA_POP::ListMultiMap<L,T>::erase(const typename SA_POP::ListMultiMap<L,T>::key_type& key)
{

  //Removal all list items with matching key in the pair

  for(SA_POP::ListMultiMap<L,T>::list_iterator l_iter = list_.begin(); l_iter != list_.end();)
  {
    SA_POP::ListMultiMap<L,T>::list_iterator prev_iter = l_iter;
    l_iter++;
    if((*prev_iter).first == key)
    {
      this->list_.erase(prev_iter);
    }
  }


  return this->multimap_.erase (key);
};

template<typename L, typename T>
void SA_POP::ListMultiMap<L,T>::erase(typename SA_POP::ListMultiMap<L,T>::iterator _Where)
{
  if(_Where != this->end())
  {
      //Removal first or last list items with matching key in the pair
      if(remove_dir_ == SA_POP::FRONT)
      {
        for(SA_POP::ListMultiMap<L,T>::list_iterator l_iter = list_.begin(); l_iter != list_.end();)
        {
          SA_POP::ListMultiMap<L,T>::list_iterator prev_iter = l_iter;
          l_iter++;
          if((*prev_iter).first == _Where->first && (*prev_iter).second == _Where->second )
          {
            this->list_.erase(prev_iter);
            break;
          }
        }
      }
      else if(remove_dir_ == SA_POP::BACK)
      {
        SA_POP::ListMultiMap<L,T>::list_iterator last_iter = list_.end();
        for(SA_POP::ListMultiMap<L,T>::list_iterator l_iter = list_.begin(); l_iter != list_.end();l_iter++)
        {
          if((*l_iter).first == _Where->first && (*l_iter).second == _Where->second)
          {
            last_iter = l_iter;
          }
        }
        if(last_iter != list_.end())
        {
          this->list_.erase(last_iter);
        }
        else
        {
          throw "ListMultiMap<L,T>::erase(typename ListMultiMap<L,T>::iterator _Where) : Could not find value in the list";
        }
      }
      else 
      {
        throw "ListMultiMap<L,T>::erase(typename ListMultiMap<L,T>::iterator _Where): Unknown direction to remove.";
      }
  }
  this->multimap_.erase (_Where);
};

template<typename L, typename T>
void SA_POP::ListMultiMap<L,T>::erase(typename SA_POP::ListMultiMap<L,T>::iterator _First, typename SA_POP::ListMultiMap<L,T>::iterator _Last)
{
  
  //iterate through the range [_First, _Last) and erasing each iterator in turn
  if(_First != this->end())
  {
    for(SA_POP::ListMultiMap<L,T>::iterator m_iter = _First; m_iter != _Last;)
    {
      SA_POP::ListMultiMap<L,T>::iterator prev_iter = m_iter;
      m_iter++;
      this->erase(prev_iter);
    }
  }

};

// Begin
template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::iterator SA_POP::ListMultiMap<L,T>::begin()
{
  return this->multimap_.begin ();
};

template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::const_iterator SA_POP::ListMultiMap<L,T>::begin() const
{
  return this->multimap_.begin ();
};

template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::reverse_iterator SA_POP::ListMultiMap<L,T>::rbegin()
{
  return this->multimap_.rbegin ();
};

template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::const_reverse_iterator SA_POP::ListMultiMap<L,T>::rbegin() const
{
  return this->multimap_.rbegin ();
};

// End
template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::iterator SA_POP::ListMultiMap<L,T>::end()
{
  return this->multimap_.end ();
};

template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::const_iterator SA_POP::ListMultiMap<L,T>::end() const
{
  return this->multimap_.end ();
};

template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::reverse_iterator SA_POP::ListMultiMap<L,T>::rend()
{
  return this->multimap_.rend ();
};

template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::const_reverse_iterator SA_POP::ListMultiMap<L,T>::rend() const
{
  return this->multimap_.rend ();
};

// Lower Bound
template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::iterator SA_POP::ListMultiMap<L,T>::lower_bound(const typename SA_POP::ListMultiMap<L,T>::key_type& _Keyval)
{
  return this->multimap_.lower_bound (_Keyval);
};

template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::const_iterator SA_POP::ListMultiMap<L,T>::lower_bound(const typename SA_POP::ListMultiMap<L,T>::key_type& _Keyval) const
{
  return this->multimap_.lower_bound (_Keyval);
};

// Upper Bound
template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::iterator SA_POP::ListMultiMap<L,T>::upper_bound(const typename SA_POP::ListMultiMap<L,T>::key_type& _Keyval)
{
  return this->multimap_.upper_bound (_Keyval);
};

template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::const_iterator SA_POP::ListMultiMap<L,T>::upper_bound(const typename SA_POP::ListMultiMap<L,T>::key_type& _Keyval) const
{
  return this->multimap_.upper_bound (_Keyval);

};

// Size
template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::size_type SA_POP::ListMultiMap<L,T>::size() const
{
  return this->multimap_.size ();
};

// Empty?
template<typename L, typename T>
bool SA_POP::ListMultiMap<L,T>::empty() const
{
  return this->multimap_.empty ();
};

///Count
template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::size_type SA_POP::ListMultiMap<L,T>::count(const typename SA_POP::ListMultiMap<L,T>::key_type& _Keyval) const
{
  return this->multimap_.count (_Keyval);
};

///Clear
template<typename L, typename T>
void SA_POP::ListMultiMap<L,T>::clear()
{
  this->multimap_.clear ();
  this->list_.clear ();
};

///Equal Range
template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::_Pairii SA_POP::ListMultiMap<L,T>::equal_range(const typename SA_POP::ListMultiMap<L,T>::key_type& _Keyval)
{
  return this->multimap_.equal_range (_Keyval);
};

template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::_Paircc SA_POP::ListMultiMap<L,T>::equal_range(const typename SA_POP::ListMultiMap<L,T>::key_type& _Keyval) const
{
  return this->multimap_.equal_range (_Keyval);
};

//List Functions

///Erase  
template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::list_iterator SA_POP::ListMultiMap<L,T>::erase(typename SA_POP::ListMultiMap<L,T>::list_iterator _Where)
{
  // Erase from the multimap
  if(_Where != list_.end())
  {

    for (std::multimap<L, T>::iterator m_iter = multimap_.lower_bound ((*_Where).first);m_iter != multimap_.upper_bound ((*_Where).first);)
    {
      std::multimap<L, T>::iterator prev_iter = m_iter;
      m_iter++;
      
      if(prev_iter->second == (*_Where).second)
      {
        this->multimap_.erase(prev_iter);
        break;
      }
    }

  }
  
  return this->list_.erase(_Where);
 
};

///Front
template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::list_reference SA_POP::ListMultiMap<L,T>::front()
{
  return this->list_.front ();
};

template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::const_list_reference SA_POP::ListMultiMap<L,T>::front() const
{
  return this->list_.front ();
};

///Back
template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::list_reference SA_POP::ListMultiMap<L,T>::back()
{
  return this->list_.back ();
};

template<typename L, typename T>
typename SA_POP::ListMultiMap<L,T>::const_list_reference SA_POP::ListMultiMap<L,T>::back() const
{
  return this->list_.back ();
};

///Push Front
template<typename L, typename T>
void SA_POP::ListMultiMap<L,T>::push_front(const typename SA_POP::ListMultiMap<L,T>::list_Ty& _Val)
{
  
  this->multimap_.insert(_Val);
  this->list_.push_front (_Val);
};

///Pop Front
template<typename L, typename T>
void SA_POP::ListMultiMap<L,T>::pop_front()
{

  //This will erase the front item in both list and multimap.
  this->erase(this->list_.begin());

};

///Push Back
template<typename L, typename T>
void SA_POP::ListMultiMap<L,T>::push_back(const typename SA_POP::ListMultiMap<L,T>::list_Ty& _Val)
{
  
  this->multimap_.insert(_Val);

  this->list_.push_back (_Val);
};

///Pop Back
template<typename L, typename T>
void SA_POP::ListMultiMap<L,T>::pop_back()
{
  //This will erase the back item in both list and multimap.
  SA_POP::ListMultiMap<L,T>::list_iterator l_iter= -- this->list_.end();
  this->erase(l_iter);
};


#endif /* SA_POP_UTILS_CPP_ */
