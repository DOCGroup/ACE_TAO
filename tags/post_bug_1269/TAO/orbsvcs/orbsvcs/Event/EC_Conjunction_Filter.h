/* -*- C++ -*- */
/**
 *  @file   EC_Conjunction_Filter.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_CONJUNCTION_FILTER_H
#define TAO_EC_CONJUNCTION_FILTER_H
#include "ace/pre.h"

#include "EC_Filter.h"
#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_EC_Conjunction_Filter
 *
 * @brief The conjunction filter.
 *
 * This filter waits until each one of its children has accepted
 * at least one event.  Only in that case it accepts and publishes
 * the sequence formed by all the children events.
 *
 * <H2>Memory Management</H2>
 * It assumes ownership of the children.
 */
class TAO_RTEvent_Export TAO_EC_Conjunction_Filter : public TAO_EC_Filter
{
public:
  /// Constructor. It assumes ownership of both the array and the
  /// children.
  TAO_EC_Conjunction_Filter (TAO_EC_Filter* children[],
                             size_t n);

  /// Destructor
  virtual ~TAO_EC_Conjunction_Filter (void);

  // = The TAO_EC_Filter methods, please check the documentation in
  // TAO_EC_Filter.
  virtual ChildrenIterator begin (void) const;
  virtual ChildrenIterator end (void) const;
  virtual int size (void) const;
  virtual int filter (const RtecEventComm::EventSet& event,
                      TAO_EC_QOS_Info& qos_info
                      ACE_ENV_ARG_DECL);
  virtual int filter_nocopy (RtecEventComm::EventSet& event,
                             TAO_EC_QOS_Info& qos_info
                             ACE_ENV_ARG_DECL);
  virtual void push (const RtecEventComm::EventSet& event,
                     TAO_EC_QOS_Info& qos_info
                     ACE_ENV_ARG_DECL);
  virtual void push_nocopy (RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info
                            ACE_ENV_ARG_DECL);
  virtual void clear (void);
  virtual CORBA::ULong max_event_size (void) const;
  virtual int can_match (const RtecEventComm::EventHeader& header) const;
  virtual int add_dependencies (const RtecEventComm::EventHeader& header,
                                const TAO_EC_QOS_Info &qos_info
                                ACE_ENV_ARG_DECL);

  typedef unsigned int Word;

private:
  /// Determine if all the children have received their events.
  int all_received (void) const;

  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Conjunction_Filter
                              (const TAO_EC_Conjunction_Filter&))
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Conjunction_Filter& operator=
                              (const TAO_EC_Conjunction_Filter&))

private:
  /// The children
  TAO_EC_Filter** children_;

  /// The number of children.
  size_t n_;

  /// The event we send up (once all the children have pushed theirs).
  RtecEventComm::EventSet event_;

  /**
   * The number of words in the bit vector
   * The bit vector to keep track of the children that have received
   * their events.
   */
  size_t nwords_;
  Word* bitvec_;

  /// The current child in the iteration, used in the push() method...
  ChildrenIterator current_child_;
};

#if defined (__ACE_INLINE__)
#include "EC_Conjunction_Filter.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_CONJUNCTION_FILTER_H */
