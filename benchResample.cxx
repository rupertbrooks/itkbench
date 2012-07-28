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
#include "benchResample.h"

   benchResample::benchResample(const unsigned int numthreads) {
      itk::MultiThreader::SetGlobalDefaultNumberOfThreads(numthreads);

      typedef itk::RandomImageSource<ImageType> SourceType;
      SourceType::Pointer s1=SourceType::New();
      ImageType::SizeType sz;
      sz[0]=50;
      sz[1]=60;
      sz[2]=70;

      ImageType::SpacingType spacing;
      spacing[0]=0.567;spacing[1]=0.789;spacing[2]=1.234;
      ImageType::PointType origin;
      origin[0]=-12.3;origin[1]=23.4;origin[2]=-34.5;
      vnl_vector_fixed<double,3> axis;
      axis[0]=0.1; axis[1]=0.2; axis[2]=0.3;
      ImageType::DirectionType direction(vnl_rotation_matrix(axis));
      s1->SetSpacing(spacing);
      s1->SetOrigin(origin);
      //      s1->SetDirection(direction);

      s1->Update();
      m_Image=s1->GetOutput();
      m_Image->DisconnectPipeline();

      std::stringstream ss;
      ss << "Resample_"<< numthreads << "_threads";
      m_Name=ss.str();





   }

   const char * benchResample::GetNameOfBenchmark() const {return m_Name.c_str();}

   void benchResample::operator()() const {
      ImageType::SizeType sz=m_Image->GetLargestPossibleRegion().GetSize();
      ResamplerType::Pointer resampler=ResamplerType::New();
      ImageType::SpacingType spacing; spacing.Fill(1.001);
      resampler->SetInterpolator(InterpolatorType::New());
      resampler->SetInput(m_Image);
      resampler->SetOutputSpacing(spacing);
      resampler->SetOutputOrigin(m_Image->GetOrigin());
      resampler->SetSize(sz);
      resampler->Update();
   }
