#pragma once
#include "itkImage.h"
#include "itkImageFileReader.h"
#include <iostream>
//#include "itkOutputWindow.h"
//#include "itkTextOutput.h"
#include "itkImageFileWriter.h"
#include "itkOtsuThresholdImageFilter.h"
#include "itkBinaryContourImageFilter.h"
#include "itkmodTimeProbeCollector.h"
#include "vnl/vnl_rotation_matrix.h"
#include "itkMultiThreader.h"
#include "itkRandomImageSource.h"
#include "itkTranslationTransform.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkBSplineInterpolateImageFunction.h"
#include "itkOtsuThresholdImageFilter.h"
#include "itkFlipImageFilter.h"
#include "itkBinaryContourImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itksys/SystemInformation.hxx"
#include "itkResampleImageFilter.h"
#include "CBenchmark.h"



class benchMeanSquares: public CBenchmark {
public:
   typedef itk::Image<unsigned char,3> ImageType;
   typedef itk::MeanSquaresImageToImageMetric<ImageType,ImageType> MetricType;
   typedef itk::TranslationTransform<double,3> TransformType;
   typedef itk::LinearInterpolateImageFunction<ImageType> InterpolatorType;

   benchMeanSquares(const unsigned int numthreads);

   const char * GetNameOfBenchmark() const ;
   
   void operator()() const ;

private:
   std::string m_Name;
   ImageType::Pointer m_Fixed;
   ImageType::Pointer m_Moving;
};
