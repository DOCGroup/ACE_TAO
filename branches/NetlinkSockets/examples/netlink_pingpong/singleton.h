// $Id$

/*! \file singleton.h
 * \brief singleton template
 *
 */
#ifndef _SINGLETON_H_
#define _SINGLETON_H_

/*! \class CNFSingleon
    \brief singleton template
*/
template <class T> class CSingleton
{
protected:
    static T* m_instance;
    CSingleton(){};

public:

    static T* instance()
    {
      if (!m_instance)
          m_instance = new T();
      return m_instance;
    }
};

#endif
