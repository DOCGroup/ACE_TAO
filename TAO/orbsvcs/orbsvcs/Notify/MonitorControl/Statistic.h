// $Id$

#ifndef STATISTIC_H
#define STATISTIC_H

#include /**/ "ace/pre.h"

#include "ace/Monitor_Base.h"
#include "ace/Array_Base.h"

#include "tao/Versioned_Namespace.h"
#include "tao/orbconf.h"

#include "orbsvcs/Notify/MonitorControl/notify_mc_export.h"

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_MC_Export TAO_Statistic : public Monitor_Base
{
public:
  /// A statistic can hold various types of data and acts differently
  /// depending upon the type of information stored.
  enum Information_Type
  {
    TS_COUNTER,
    TS_NUMBER,
    TS_TIME,
    TS_INTERVAL,
    TS_LIST
  };

  /// A simple string list.
  typedef Monitor_Control_Types::NameList List;

  /// This exception is thrown if a method is called that doesn't apply
  /// to the type of information being stored.
  class TAO_Notify_MC_Export Invalid_Operation {};

  /// Construct a statistic object.  The name and type of the statistic
  /// will never change once the object is created.
  TAO_Statistic (const char* name, Information_Type type);

  /// Clean up any memory allocated by this object
  virtual ~TAO_Statistic (void);

  /// Perform calculations if the statistic is dynamic.
  virtual void calculate (void);

  /// Override of the base class method.
  virtual void receive (double data);

  /// Take the data and store it.
  /// It is assumed that the type of data corresponds to
  /// the type specified during construction.
  void receive (const TAO_Statistic::List& data);

  /// Clear all stored data. Overridden from Monitor_Base.
  virtual void clear (void);

  /// Calculate the average of the accumulated samples.
  double average (void) const;

  /// Calculate the sum of the squares of the samples.
  double sum_of_squares (void) const;

  /// Returns the number of samples
  size_t count (void) const;

  /// Returns the minimum sample value
  double minimum_sample (void) const;

  /// Returns the maximum sample value
  double maximum_sample (void) const;

  /// Returns the most recent sample value
  double last_sample (void) const;

  /// Return the type of this statistic
  Information_Type type (void) const;

  /// Return the list of information.  This
  /// method is only valid if the statistic type
  /// is TS_LIST
  List get_list (void) const;

private:
  Information_Type type_;

//  mutable TAO_SYNCH_RW_MUTEX mutex_;
  size_t index_;
  bool minimum_set_;
  double minimum_;
  double maximum_;
  double sum_;
  double sum_of_squares_;
  double last_;
  ACE_Array_Base<const char*> data_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#  include "Statistic.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* STATISTIC_H */
