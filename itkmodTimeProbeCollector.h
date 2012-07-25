/*=========================================================================
* Author of Module: Rupert Brooks
*
* Module name: itkmodInstantaneousMemoryProbeCollector
* Description: 
*  This is an improvement of ITKs time probe collector.  This one
*  supports means, standard deviation, etc
*
=========================================================================*/
#ifndef __itkmodTimeProbeCollector_h
#define __itkmodTimeProbeCollector_h

#include <map>
#include <vector>
#include <itkTimeProbe.h>
#include <cfloat>

namespace itkmod {

class TimeProbeCollector  {
public:
   typedef itk::TimeProbe::TimeStampType ValueType;
   typedef std::map<std::string, std::pair<std::vector< ValueType >, std::vector< ValueType > > > MapType;
   TimeProbeCollector(): m_longestName(11) {}; // 11 is length of the title
   const char * GetNameOfClass() { return "TimeProbeCollector"; } // needed for itkExceptionMacro
   void Start(const std::string & name);
   void Stop(const std::string & name);
   void Report(std::ostream & os = std::cout);
   ValueType Now();
private:
   static const unsigned int outwide=14;
   MapType m_Probes;
   itk::TimeProbe m_TimeProbe;
   std::size_t m_longestName;
};

} // end namespace itkmod
#endif
