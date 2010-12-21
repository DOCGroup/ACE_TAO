// -*- C++ -*-
// $Id$

#ifndef I2C_SHAPES_TRAITS_H_
#define I2C_SHAPES_TRAITS_H_

#include "Base/I2C_Shapes_BaseSupport.h"
#include "Base/I2C_ShapeTypeC.h"

struct ShapeType_DDS_Traits
{
  typedef ::ShapeType value_type;
  typedef ::ShapeTypeRTISeq dds_seq_type;
  typedef ::ShapeTypeTypeSupport type_support;
  typedef ::DDS_SampleInfoSeq sampleinfo_seq_type;
  typedef ::DDS_SampleInfo sampleinfo_type;
  typedef ::ShapeTypeDataWriter datawriter_type;
  typedef ::ShapeTypeDataReader datareader_type;
  typedef ::I2C_Shapes::DataWriter typed_writer_type;
  typedef ::I2C_Shapes::DataReader typed_reader_type;
};

#endif /* I2C_SHAPES_TRAITS_H_ */
