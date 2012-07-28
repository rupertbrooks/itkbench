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


template<class InterpolatorType>
class benchInterpolator: public CBenchmark {
public:
   typedef typename InterpolatorType::InputImageType ImageType;
   typedef itk::RandomImageSource<ImageType> SourceType;

   benchInterpolator(const unsigned int numthreads) {
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

      m_Interpolator=InterpolatorType::New();
      m_Interpolator->SetInputImage(m_Image);

      std::string interpolatorName=m_Interpolator->GetNameOfClass();
      interpolatorName.resize(interpolatorName.find("Interpolate"));


      std::stringstream ss;
      ss << "Interp_"<<interpolatorName << "_" << numthreads << "_threads";
      m_Name=ss.str();





   }

   const char * GetNameOfBenchmark() const {return m_Name.c_str();}

   void operator()() const {
      ImageType::SizeType sz=m_Image->GetLargestPossibleRegion().GetSize();
      ImageType::IndexType ix; ix.Fill(0);
      ImageType::PointType pt1,pt2,pt;
      m_Image->TransformIndexToPhysicalPoint(ix,pt1);
      for(unsigned int d=0;d<ImageType::GetImageDimension();++d) {
         ix[d]=sz[d];
      }
      m_Image->TransformIndexToPhysicalPoint(ix,pt2);
      for(pt[0]=std::min(pt1[0],pt2[0]);pt[0]<std::max(pt1[0],pt2[0]);pt[0]+=1.12334) {
         for(pt[1]=std::min(pt1[1],pt2[1]);pt[1]<std::max(pt1[1],pt2[1]);pt[1]+=1.2334) {
            for(pt[2]=std::min(pt1[2],pt2[2]);pt[2]<std::max(pt1[2],pt2[2]);pt[2]+=1.2334) {
               if(m_Interpolator->IsInsideBuffer(pt)) {
                  ImageType::PixelType p=m_Interpolator->Evaluate(pt);
               }
            }
         }
      }
   }

private:
   std::string m_Name;
   typename InterpolatorType::Pointer m_Interpolator;
   typename ImageType::Pointer m_Image;
   typename ImageType::Pointer m_Moving;
};
