/* -*-C++-*- */
#ifndef _COLLECTION
#define _COLLECTION
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    collect.h
//
// = DESCRIPTION
//  Address class definition. Encapsulates various network
//  addresses into easy to use, safe and portable classes.
//
// = AUTHOR
//    Peter E Mellquist 
//
// ============================================================================
/*===================================================================

  Copyright (c) 1996
  Hewlett-Packard Company

  ATTENTION: USE OF THIS SOFTWARE IS SUBJECT TO THE FOLLOWING TERMS.
  Permission to use, copy, modify, distribute and/or sell this software 
  and/or its documentation is hereby granted without fee. User agrees 
  to display the above copyright notice and this license notice in all 
  copies of the software and any documentation of the software. User 
  agrees to assume all liability for the use of the software; Hewlett-Packard 
  makes no representations about the suitability of this software for any 
  purpose. It is provided "AS-IS without warranty of any kind,either express 
  or implied. User hereby grants a royalty-free license to any and all 
  derivatives based upon this software code base. 
=====================================================================*/ 



#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define MAXT 25     // elements per block


#ifdef WIN32
template <class T> class SnmpCollection
{
  class cBlock 
  {
  public:
    T *item[MAXT];
    cBlock *next;
    cBlock *prev;
  };

public:
   
  // create an empty collection
  SnmpCollection( void):count(0)
  { 
    data.next=0;
    data.prev=0;
  };

   
  // create a collection using a single template object
  SnmpCollection( const T &t):count( 1) 
  { 
    data.item[0] = new T( t);
    data.next=0;
    data.prev=0;
  };

  // create a collection with another collection
  // copy constructor
  SnmpCollection( const SnmpCollection<T> &c)
  {
    count = 0;
    data.next=0;
    data.prev=0;

    if ( c.count == 0) {
      count = 0;
      return;
    }

    // load up the new collection
    cBlock *current = &data;
    cBlock *nextBlock;
    int cn = 0;
    count = 0;
    while ( count < c.count) {
      if ( cn >= MAXT) {
	nextBlock = new cBlock;
	nextBlock->prev = current;
	nextBlock->next = 0;
	current->next = nextBlock;
	current = nextBlock;
	cn=0;
      }
      current->item[cn] = new T(c[count]);
      count++;
      cn++;
    }
  };

  // destroy the collection
  ~SnmpCollection()
  { 
    if ( count == 0)
      return;

    // delete the data
    cBlock *current = &data;
    int z=0;
    int cn=0;
    while ( z< count) {
      if (cn >= MAXT) {
	cn =0;
	current = current->next;
      }
      delete current->item[cn];
      cn++;
      z++;
    }


    // delete the blocks
    while ( current->next != 0)
      current = current->next;
    while ( current->prev != 0) {
      current = current->prev;
      delete current->next;
    }

  };
   
  // get the size of the collection
  int size()
  { 
    return count; 
  };

  // append an item to the collection
  SnmpCollection& operator +=( const T &i)
  { 

    cBlock *current = &data;
    cBlock *add;
    int cn = (int) count % MAXT;
    while (current->next != 0)
      current = current->next;
    if ((count > 0) && ((count % MAXT)== 0)) {
      add = new cBlock;
      current->next = add;
      add->prev =  current;
      add->next = 0;
      add->item[0] = new T(i);
    }
    else {
      current->item[cn] = new T(i);
      cn++;
    }
    count++;

    return *this; 
  };


  // assign one collection to another
  SnmpCollection & operator = ( const SnmpCollection<T> &c)
  {

    // delete the data
    cBlock *current = &data;
    int z=0;
    int cn=0;
    while ( z< count) {
      if (cn >= MAXT) {
	cn =0;
	current = current->next;
      }
      delete current->item[cn];
      cn++;
      z++;
    }
    // delete the blocks
    while ( current->next != 0)
      current = current->next;
    while ( current->prev != 0) {
      current = current->prev;
      delete current->next;
    }
    count=0;

    if ( c.count ==0)
      return *this;


    // load up the new collection
    current = &data;
    cBlock *nextBlock;
    cn = 0;
    count = 0;
    while ( count < c.count) {
      if ( cn >= MAXT) {
	nextBlock = new cBlock;
	nextBlock->prev = current;
	nextBlock->next = 0;
	current->next = nextBlock;
	current = nextBlock;
	cn=0;
      }
      current->item[cn] = new T( c[count]);
      count++;
      cn++;
    }

    return *this;
  };

  // access an element in the collection
  T operator[]( int p)	const
  { 
    if (p<count) {
      cBlock const *current = &data;
      int bn = (int) (p / MAXT);
      for (int z=0; z<bn; z++)
	current = current->next;
      int cn = (int) p % MAXT;
      return (T) *(current->item[cn]);
    }
    else {   // return an instance of nothing!!
      T t;
      return (T) (t);
    }

  };

  // set an element in the collection
  int set_element( const T& i, const int p)
  { 

    cBlock *current = &data;
    if ( p > count)
      return -1; // not found!
    int bn = (int) p / MAXT;
    int cn = (int) p % MAXT;
    for (int z=0; z<bn; z++)
      current = current->next;
    delete current->item[cn];
    current->item[cn] = new T(i);
    return 0;

  };

  // get an element in the collection
  int get_element( T& i, const int p) const
  { 

    cBlock const *current = &data;
    if ( p > count)
      return -1; // not found!
    int bn = (int) p / MAXT;
    int cn = (int) p % MAXT;
    for (int z=0; z<bn; z++)
      current = current->next;
    i = *(current->item[cn]);
    return 0;
  };

  // apply an function to the entire collection, iterator
  void apply( void f( T&))
  { 
    T temp;
    for ( int z=0;z<count;z++) {
      this->get_element( temp,z);
      f( temp);
    }
  }

  // looks for an element in the collection
  // returns TRUE if found
  int find( const T& i)
  {
    T temp;
    for ( int z=0;z<count;z++) {
      this->get_element( temp,z);
      if ( temp == i)
	return TRUE;
    }
    return FALSE;
  }


private:
  int count;
  cBlock data;

}; // end template collection class def
#endif  // end if WIN32

#ifdef __unix

template <class T> class SnmpCollection
{

   class cBlock {
   public:
	   T *item[MAXT];
	   cBlock *next;
	   cBlock *prev;
   };

public:
   
   // create an empty collection
   SnmpCollection( void);

   
   // create a collection using a single template object
   SnmpCollection( const T &t); 

   // create a collection with another collection
   // copy constructor
   SnmpCollection( const SnmpCollection<T> &c);

   // destroy the collection
   ~SnmpCollection();
   
   // get the size of the collection
   int size();

   // append an item to the collection
   SnmpCollection& operator +=( const T &i);

   // assign one collection to another
   SnmpCollection & operator = ( const SnmpCollection<T> &c);

   // access an element in the collection
   T operator[]( int p)	const;

   // set an element in the collection
   int set_element( const T& i, const int p);

   // get an element in the collection
   int get_element( T& i, const int p) const;

   // apply an function to the entire collection, iterator
   void apply( void f( T&));

   // looks for an element in the collection
   // returns TRUE if found
   int find( const T& i);

private:
   int count;
   cBlock data;

}; // end template collection class def

#endif  // end if __unix

#endif  // end if _COLLECTION

