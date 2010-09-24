// -*- C++ -*-

//=============================================================================
/**
 *  @file     ORB_Table.h
 *
 *  $Id$
 *
 *  @author Ossama Othman
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_ORB_TABLE_H
#define TAO_ORB_TABLE_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/CORBA_String.h"

#include "ace/Array_Map.h"
#include "ace/Thread_Mutex.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations.
class TAO_ORB_Core;

namespace TAO
{
  class ORB_Core_Ref_Counter;

  /**
   * @class ORB_Table
   *
   * @brief Keep a table with all the ORBs in the system.
   *
   * CORBA::ORB_init() is supposed to return the same ORB if the
   * user specifies the same ORBid, either in the ORB_init()
   * parameter or in the -ORBid option.
   * This class is used to implement that feature.
   * It is also useful when trying to determine if an object
   * reference is collocated or not.
   *
   * @note This class should be instantiated via its instance()
   *       method.  Normally this would be enforced by making the
   *       constructor protected but that forces a friend declaration
   *       containing a template type (TAO_Singleton) with a static
   *       member to be introduced.  In turn, this potentially
   *       introduces problems in MS Windows DLL environments due to
   *       the occurrence of multiple singleton instances.  There
   *       should only be one!
   */
  class TAO_Export ORB_Table
  {
  public:

    /// Constructor
    /**
     * @note See the note in the class description for an explanation
     *       of why this constructor is not protected.
     */
    ORB_Table (void);

    typedef ACE_Array_Map<CORBA::String_var,
                          ORB_Core_Ref_Counter,
                          TAO::String_Var_Equal_To> Table;
    typedef Table::key_type   key_type;
    typedef Table::data_type  data_type;
    typedef Table::value_type value_type;
    typedef Table::size_type  size_type;
    typedef Table::iterator   iterator;

    /**
     * @name The canonical ACE_Map methods.
     */
    //@{
    iterator begin (void);
    iterator end (void);
    int bind (const char *orb_id, ::TAO_ORB_Core *orb_core);

    /// Return @c TAO_ORB_Core corresponding to ORB with given @a
    /// orb_id.
    /**
     * @note The caller must decrease the reference count on the
     *       returned ORB_Core, i.e. the callers "owns" it.
     */
    ::TAO_ORB_Core* find (const char *orb_id);

    int unbind (const char *orb_id);
    //@}

    /// Obtain the first ORB for the @c ORB_Core_instance()
    /// implementation.
    ::TAO_ORB_Core * first_orb (void);

    /// Return a unique instance
    static ORB_Table * instance (void);

    /// Set the ORB related to the orb_id as the default ORB and not the
    /// ORB that is first binded.
    void set_default (char const * orb_id);

    /// Method the ORB invokes to specify that it doesnt want to be the
    /// default ORB if there are more than one ORB registered.
    void not_default (char const * orb_id);

    /// Accessor to the underlying table_
    Table * table (void);

    /// Return reference to underlying lock.
    TAO_SYNCH_MUTEX & lock (void);

  private:

    // Prevent copying
    ORB_Table (const ORB_Table &);
    void operator= (const ORB_Table &);

    /// Return @c TAO_ORB_Core corresponding to ORB with given @a
    /// orb_id.  (underlying unlocked implementation).
    ::TAO_ORB_Core * find_i (char const * orb_id);

  private:

    /// Lock used to synchronize access to the internal state.
    ::TAO_SYNCH_MUTEX lock_;

    /// Variable to check if the first ORB decides not to be the
    /// default.
    bool first_orb_not_default_;

    /// The underlying table.
    Table table_;

    /// The first ORB created by the user
    ::TAO_ORB_Core * first_orb_;
  };

  // -----------------------------------------------

  /**
   * @class ORB_Table_Ref_Counter
   *
   * @brief Class contained by ORB_Table's Unbounded_Set.
   *
   * Class contained by ORB_Table's Unbounded_Set.
   */
  class ORB_Core_Ref_Counter
  {
  public:

    /// Constructor.
    ORB_Core_Ref_Counter (void);

    /// Constructor.
    ORB_Core_Ref_Counter (::TAO_ORB_Core * core);

    /// Destructor.
    ~ORB_Core_Ref_Counter (void);

    /// Copy constructor.
    ORB_Core_Ref_Counter (ORB_Core_Ref_Counter const & rhs);

    /// Assignment operator.
    void operator= (ORB_Core_Ref_Counter const & rhs);

    /// ORB_Core pointer accessor.
    ::TAO_ORB_Core * core (void) const { return this->core_; }

  private:
    ::TAO_ORB_Core * core_;
  };

}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/ORB_Table.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_ORB_TABLE_H */
