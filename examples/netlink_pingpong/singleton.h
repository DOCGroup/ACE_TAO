// $Id$

/*! \file NFSingleton.h
 * \brief singleton template
 *
 */
#ifndef _NFSINGLETON_H_
#define _NFSINGLETON_H_

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
