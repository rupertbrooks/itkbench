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
#include "benchFilterChain.h"

   benchFilterChain::benchFilterChain(const unsigned int numthreads) {
      itk::MultiThreader::SetGlobalDefaultNumberOfThreads(numthreads);
      std::stringstream ss;
      ss << "FilterChain_" << numthreads << "_threads";
      m_Name=ss.str();


   }

   const char * benchFilterChain::GetNameOfBenchmark() const {return m_Name.c_str();}

   void benchFilterChain::operator()() const {
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

      typedef itk::RecursiveGaussianImageFilter<ImageType,ImageType> Filter1Type;
      Filter1Type::Pointer f1=Filter1Type::New();
      f1->SetSigma(5.6);
      Filter2Type::Pointer f2=Filter2Type::New();
      Filter2Type::FlipAxesArrayType flips;
      flips.Fill(1);
      f2->SetFlipAxes(flips);
      Filter3Type::Pointer f3=Filter3Type::New();
      Filter4Type::Pointer f4=Filter4Type::New();

      f1->SetInput(s1->GetOutput());
      f2->SetInput(f1->GetOutput());
      f3->SetInput(f2->GetOutput());
      f4->SetInput(f3->GetOutput());

      f4->Update();
   }
