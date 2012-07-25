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
#include "itkOtsuThresholdImageFilter.h"
#include "itkFlipImageFilter.h"
#include "itkBinaryContourImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itksys/SystemInformation.hxx"


class CBenchmark {
public:
   CBenchmark() {}
   virtual const char * GetNameOfBenchmark() const =0;
   virtual void operator()() const=0;
};

class benchImage2D: public CBenchmark {
public:
   typedef itk::Image<unsigned char,2> ImageType;
   benchImage2D() {
   }
   
   const char * GetNameOfBenchmark() const {return "Image2D";}

   void operator()() const {
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
};

class benchImage3D: public CBenchmark {
public:
   typedef itk::Image<unsigned char,3> ImageType;
   benchImage3D() {
   }

   const char * GetNameOfBenchmark() const {return "Image3D";}

   void operator()() const {
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
};

class benchMeanSquares: public CBenchmark {
public:
   typedef itk::Image<unsigned char,3> ImageType;
   typedef itk::MeanSquaresImageToImageMetric<ImageType,ImageType> MetricType;
   typedef itk::TranslationTransform<double,3> TransformType;
   typedef itk::LinearInterpolateImageFunction<ImageType> InterpolatorType;

   benchMeanSquares(const unsigned int numthreads) {
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

   const char * GetNameOfBenchmark() const {return m_Name.c_str();}

   void operator()() const {
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

private:
   std::string m_Name;
   ImageType::Pointer m_Fixed;
   ImageType::Pointer m_Moving;
};

class benchFilterChain: public CBenchmark {
public:
   typedef itk::Image<unsigned char,3> ImageType;
   typedef itk::RandomImageSource<ImageType> SourceType;
   typedef itk::RecursiveGaussianImageFilter<ImageType,ImageType> Filter1Type;
   typedef itk::FlipImageFilter<ImageType> Filter2Type;
   typedef itk::OtsuThresholdImageFilter<ImageType,ImageType> Filter3Type;
   typedef itk::BinaryContourImageFilter<ImageType,ImageType> Filter4Type;

   benchFilterChain(const unsigned int numthreads) {
      itk::MultiThreader::SetGlobalDefaultNumberOfThreads(numthreads);
      std::stringstream ss;
      ss << "FilterChain_" << numthreads << "_threads";
      m_Name=ss.str();


   }

   const char * GetNameOfBenchmark() const {return m_Name.c_str();}

   void operator()() const {
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

private:
   std::string m_Name;
   ImageType::Pointer m_Fixed;
   ImageType::Pointer m_Moving;
};



//void benchGenerateRandomNumbers() {
//}


typedef unsigned char PixelType;
typedef itk::Image< PixelType, 2 >  ImageType;

typedef itk::OtsuThresholdImageFilter<ImageType,ImageType> ThresholdFilter;
typedef itk::BinaryContourImageFilter<ImageType,ImageType> ContourFilter;
typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::ImageFileWriter<ImageType> WriterType;

itkmod::TimeProbeCollector gTimer;

inline void run_benchmark(const CBenchmark & bench, const int numTries) 
{
   const char * name=bench.GetNameOfBenchmark();
   for(unsigned int t=0;t<numTries;++t) {
      gTimer.Start(name);
      try {
         bench();
      } catch(itk::ExceptionObject & e) {
         std::cout << "Benchmark: " << name << " FAILED!" << std::endl;
         std::cout << e << std::endl;
         gTimer.Stop(name);
         break;
      }
      gTimer.Stop(name);
   }
}
int main(int argc, char ** argv) {


   itksys::SystemInformation sysinfo;
   sysinfo.RunCPUCheck();
   sysinfo.RunMemoryCheck();
   sysinfo.RunOSCheck();
   std::cout << "System: "    <<sysinfo.GetHostname() << std::endl;
   std::cout << "Processor: " <<sysinfo.GetExtendedProcessorName() << std::endl;
   std::cout << " Serial #: " <<sysinfo.GetProcessorSerialNumber() << std::endl;

   std::cout << "    Cache: "<<sysinfo.GetProcessorCacheSize() << std::endl;
   std::cout << "    Clock: "<<sysinfo.GetProcessorClockFrequency() << std::endl;
   std::cout << "    Cores: "<<sysinfo.GetNumberOfPhysicalCPU()
      << " cpus x "<<sysinfo.GetNumberOfLogicalCPU()
      << " Cores = "
      << sysinfo.GetNumberOfLogicalCPU()*sysinfo.GetNumberOfPhysicalCPU() << std::endl;

   std::cout << "OSName:     "<<sysinfo.GetOSName() << std::endl;
   std::cout << "  Release:  "<<sysinfo.GetOSRelease() << std::endl;
   std::cout << "  Version:  "<<sysinfo.GetOSVersion() << std::endl;
   std::cout << "  Platform: "<<sysinfo.GetOSPlatform() << std::endl;

   std::cout << "  Operating System is " << (sysinfo.Is64Bits()?"64 bit":"32 bit") << std::endl; 

//#define TOSTRING(x) #x
//#define ITK_VERSION TOSTRING(ITK_VERSION_MAJOR) "." TOSTRING(ITK_VERSION_MINOR) "." TOSTRING(ITK_VERSION_PATCH)

   std::cout << "ITK Version: " << ITK_VERSION_MAJOR << "." << ITK_VERSION_MINOR << "." << ITK_VERSION_PATCH << std::endl;


   // Retrieve memory information in megabyte.
   std::cout << "Virtual Memory: Total: "<<sysinfo.GetTotalVirtualMemory()<<" Available: "<<sysinfo.GetAvailableVirtualMemory() << std::endl;
   std::cout << "Physical Memory: Total:"<<sysinfo.GetTotalPhysicalMemory()<<" Available: "<<sysinfo.GetAvailablePhysicalMemory() << std::endl;


   const int numTries=10;
   run_benchmark(benchImage2D(),numTries);
   run_benchmark(benchImage3D(),numTries);
   run_benchmark(benchMeanSquares(1),numTries);
   run_benchmark(benchMeanSquares(2),numTries);
   run_benchmark(benchMeanSquares(3),numTries);
   run_benchmark(benchMeanSquares(4),numTries);
   run_benchmark(benchFilterChain(1),numTries);
   run_benchmark(benchFilterChain(2),numTries);
   run_benchmark(benchFilterChain(3),numTries);
   run_benchmark(benchFilterChain(4),numTries);

   gTimer.Report();
}
