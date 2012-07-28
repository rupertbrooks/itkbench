#include <iostream>
#include "CBenchmark.h"
#include "benchImage2D.h"
#include "benchImage3D.h"
#include "benchInterpolator.h"
#include "benchFilterChain.h"
#include "benchMeanSquares.h"
#include "benchResample.h"

#include <itksys/SystemInformation.hxx>



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

   typedef itk::LinearInterpolateImageFunction<benchImage3D::ImageType> Linear;
   typedef itk::NearestNeighborInterpolateImageFunction<benchImage3D::ImageType> Nearest;
   typedef itk::BSplineInterpolateImageFunction<benchImage3D::ImageType> BSpline;
   const int numTries=10;
   run_benchmark(benchImage2D(),numTries);
   run_benchmark(benchImage3D(),numTries);
   //run_benchmark(benchMeanSquares(1),numTries);
   //run_benchmark(benchMeanSquares(2),numTries);
   //run_benchmark(benchMeanSquares(3),numTries);
   //run_benchmark(benchMeanSquares(4),numTries);
   //run_benchmark(benchFilterChain(1),numTries);
   //run_benchmark(benchFilterChain(2),numTries);
   //run_benchmark(benchFilterChain(3),numTries);
   //run_benchmark(benchFilterChain(4),numTries);
   run_benchmark(benchInterpolator<Linear>(1),numTries);
   //run_benchmark(benchInterpolator<Nearest>(1),numTries);
   //run_benchmark(benchInterpolator<BSpline>(1),numTries);
   run_benchmark(benchResample(1),numTries);
   run_benchmark(benchResample(2),numTries);
   run_benchmark(benchResample(3),numTries);
   run_benchmark(benchResample(4),numTries);

   gTimer.Report();
}
