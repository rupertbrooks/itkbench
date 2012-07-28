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
#include "benchMeanSquares.h"


   benchMeanSquares::benchMeanSquares(const unsigned int numthreads) {
      itk::MultiThreader::SetGlobalDefaultNumberOfThreads(numthreads);
      std::stringstream ss;
      ss << "MeanSquares_" << numthreads << "_threads";
      m_Name=ss.str();

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
      m_Fixed=s1->GetOutput();
      m_Fixed->DisconnectPipeline();

      s1->Update();
      m_Moving=s1->GetOutput();
      m_Moving->DisconnectPipeline();

   }

   const char * benchMeanSquares::GetNameOfBenchmark() const {return m_Name.c_str();}

   void benchMeanSquares::operator()() const {
      MetricType::Pointer m=MetricType::New();
      TransformType::Pointer t=TransformType::New();
      InterpolatorType::Pointer i=InterpolatorType::New();

      m->SetTransform(t);
      m->SetInterpolator(i);
      m->SetFixedImage(m_Fixed);
      m->SetMovingImage(m_Moving);
      m->SetFixedImageRegion(m_Fixed->GetLargestPossibleRegion());
      m->Initialize();

      TransformType::ParametersType p(3);


      MetricType::DerivativeType derivative;
      double value;
      for(p[0]=-5;p[0]<5;p[0]+=5.587) {
         for(p[1]=-5;p[1]<5;p[1]+=5.587) {
            for(p[2]=-5;p[2]<5;p[2]+=5.587) {
               m->GetValueAndDerivative(p,value,derivative);
            }
         }
      }
   }
