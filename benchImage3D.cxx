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

#include "benchImage3D.h"

   const char * benchImage3D::GetNameOfBenchmark() const {return "Image3D";}

   void benchImage3D::operator()() const {
      ImageType::Pointer mImage=ImageType::New();
      ImageType::SizeType sz;
      sz[0]=100;
      sz[1]=124;
      sz[2]=234;

      ImageType::SpacingType spacing;
      spacing[0]=0.567;spacing[1]=0.789;spacing[2]=1.234;
      ImageType::PointType origin;
      origin[0]=-12.3;origin[1]=23.4;origin[2]=-34.5;
      vnl_vector_fixed<double,3> axis;
      axis[0]=0.1; axis[1]=0.2; axis[2]=0.3;
      ImageType::DirectionType direction(vnl_rotation_matrix(axis));
      mImage->SetSpacing(spacing);
      mImage->SetOrigin(origin);
      mImage->SetDirection(direction);


      mImage->SetRegions(ImageType::RegionType(sz));
      mImage->Allocate();
      ImageType::IndexType ix;
      ImageType::PointType p;
      for(ix[0]=-1000;ix[0]<2000;++ix[0]) {
         for(ix[1]=-1000;ix[1]<2000;++ix[1]) {
            mImage->TransformIndexToPhysicalPoint(ix,p);
         }
      }
      for(p[0]=-1001.123;p[0]<2000;p[0]+=1.123) {
         for(p[1]=-897.3;p[1]<2000;p[1]+=1.01) {
            mImage->TransformPhysicalPointToIndex(p,ix);
         }
      }
   }
