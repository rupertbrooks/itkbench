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
#include "benchImage2D.h"


   
   const char * benchImage2D::GetNameOfBenchmark() const {return "Image2D";}

   void benchImage2D::operator()() const {
      ImageType::Pointer mImage=ImageType::New();
      ImageType::SizeType sz;
      sz[0]=1000;
      sz[1]=1234;
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
