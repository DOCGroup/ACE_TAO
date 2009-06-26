
#ifndef SA_POP_UTILS_H_
#define SA_POP_UTILS_H_

//=============================================================================
/**
 * @file  SA_POP_Utils.h
 *
 * This file contains the definitions of classes that are utilized in SA_POP.
 *
 * @author  Daniel L.C. Mack <daniel.l.mack@vanderbilt.edu>
 */
//=============================================================================

#include <string>
#include <set>
#include <list>
#include <map>
#include <sstream>


namespace SA_POP {

  /// Enumerated type for front or back (appending to an ordered list).
  enum FrontBack {FRONT, BACK};


/**
   * @class ListMultiMap
   *
   * @brief A mulitmap that also contains a list for ordering operations
   *        
   */
  template<typename L, typename T> class ListMultiMap {
  public:

    
    typedef std::multimap<L, T> _multimap;
    typedef std::list<std::pair<L, T> > _list;

    
    typedef typename _multimap::key_type key_type;
    typedef typename _multimap::mapped_type mapped_type;
    typedef typename _multimap::referent_type referent_type;	// retained
    typedef typename _multimap::key_compare key_compare;

    typedef typename _multimap::value_compare value_compare;
	  typedef typename _multimap::allocator_type allocator_type;
	  typedef typename _multimap::size_type size_type;
	  typedef typename _multimap::difference_type difference_type;
	  typedef typename _multimap::pointer pointer;
	  typedef typename _multimap::const_pointer const_pointer;
	  typedef typename _multimap::reference reference;
	  typedef typename _multimap::const_reference const_reference;
	  typedef typename _multimap::iterator iterator;
	  typedef typename _multimap::const_iterator const_iterator;
	  typedef typename _multimap::reverse_iterator reverse_iterator;
	  typedef typename _multimap::const_reverse_iterator
		  const_reverse_iterator;
	  typedef typename _multimap::value_type value_type;
    typedef typename _multimap::_Pairib _Pairib;
	  typedef typename _multimap::_Pairii _Pairii;
	  typedef typename _multimap::_Paircc _Paircc;


    ///TypeDefs for the List
    typedef typename std::pair<L, T> list_Ty;
    typedef typename _list::iterator list_iterator;
	  typedef typename _list::const_iterator const_list_iterator;
	  typedef typename _list::reverse_iterator reverse_list_iterator;
	  typedef typename _list::const_reverse_iterator
		  const_reverse_list_iterator;
    typedef typename _list::reference list_reference;
	  typedef typename _list::const_reference const_list_reference;
    
    //Needs to take an enumerated type for list direction.
    ///Constuctor
    ListMultiMap(SA_POP::FrontBack append_direction = SA_POP::FrontBack::BACK, SA_POP::FrontBack remove_direction = SA_POP::FrontBack::FRONT);
    
    ///Destructor
    virtual ~ListMultiMap() { };
	  
    ///Map Insert
    iterator insert(const value_type val);
    
    ///Map Erase
    size_type erase(const key_type& key);
    void erase(iterator _Where);
    void erase(iterator _First, iterator _Last);

    ///Begin
    iterator begin();

    const_iterator begin() const;

    reverse_iterator rbegin();

    const_reverse_iterator rbegin() const;

    ///End
    iterator end();

    const_iterator end() const;

    reverse_iterator rend();

    const_reverse_iterator rend() const;

    ///Lower Bound
    iterator lower_bound(const key_type& _Keyval);

    const_iterator lower_bound(const key_type& _Keyval) const;

    ///Upper Bound
    iterator upper_bound(const key_type& _Keyval);

    const_iterator upper_bound(const key_type& _Keyval) const;

    /// Size
    size_type size() const;

    ///Empty?
    bool empty() const;

    ///Count
    size_type count(const key_type& _Keyval) const;
    
    ///Clear
    void clear();
    
    ///Equal Range
    _Pairii equal_range(const key_type& _Keyval);
    _Paircc equal_range(const key_type& _Keyval) const;

    //List Functions
    ///Erase
    list_iterator erase(list_iterator _Where);

    ///Front
  	list_reference front();
    const_list_reference front() const;
    ///Back
  	list_reference back();
    const_list_reference back() const;
    
    ///Push Front
    void push_front(const list_Ty& _Val);
    
    ///Pop Front
    void pop_front();

    ///Push Back
    void push_back(const list_Ty& _Val);

    ///Pop Back
    void pop_back();

    




  private:

    SA_POP::FrontBack append_dir_;
    SA_POP::FrontBack remove_dir_;
    std::multimap<L, T> multimap_;
    std::list<std::pair<L, T> > list_;


  };

}

#include "SA_POP_Utils.cpp"

 #endif /* SA_POP_UTILS_H_ */