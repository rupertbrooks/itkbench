//
// an improvement on ITKs TimeProbesCollector
//
#include "itkmodTimeProbeCollector.h"
#include <iomanip>
#include <iostream>
#include <sstream>

namespace itkmod {
TimeProbeCollector::ValueType TimeProbeCollector::Now() {
   return m_TimeProbe.GetInstantValue();
}

void TimeProbeCollector::Start(const std::string & name) {
   const ValueType pval=m_TimeProbe.GetInstantValue();
   m_longestName=std::max(m_longestName,name.size());
   MapType::iterator it=m_Probes.find(name);
   if(it != m_Probes.end() ) {
      std::vector<ValueType>& v1=it->second.first;
      std::vector<ValueType>& v2=it->second.second;
      if(v1.size() !=v2.size()) {
         itkExceptionMacro("start called on " << name << " when " << v1.size() << " starts called  and " << v2.size() << " stops");
      }
      v1.push_back(pval);
   } else {
      std::vector<double> v1,v2;
      v1.push_back(pval);
      m_Probes.insert(std::make_pair(name,std::make_pair(v1,v2)));
   }
}

void TimeProbeCollector::Stop(const std::string & name) {
   const ValueType pval=m_TimeProbe.GetInstantValue();
   MapType::iterator it=m_Probes.find(name);
   if(it != m_Probes.end() ) {
      std::vector<ValueType>& v1=it->second.first;
      std::vector<ValueType>& v2=it->second.second;
      v2.push_back(pval);
      if(v1.size() !=v2.size()) {
         itkExceptionMacro("stop called on " << name << " when " << v1.size() << " starts called  and " << v2.size()-1 << " stops");
      }
   } else {
      itkExceptionMacro("stop called on " << name << " but no starts called");
   }
}

void TimeProbeCollector::Report(std::ostream & os) {
   {
      std::stringstream ss;
      ss << std::setw(m_longestName+1) << "Probe Name:";
      ss << std::setw(outwide) << " Count " 
         << std::setw(outwide) << "  Min  "
         << std::setw(outwide) << " Mean " 
         << std::setw(outwide) << " Stdev " 
         << std::setw(outwide) << " Max"
         << std::setw(outwide) << " Total ";
      os << ss.str() << "\n";
   }
   MapType::iterator it=m_Probes.begin();
   MapType::const_iterator end=m_Probes.end();
   while (it!=end) {
      ValueType mean=0;
      ValueType total=0;
      ValueType variance=0;
      ValueType stdev=0;
      ValueType min=DBL_MAX;
      ValueType max=-1;
      const std::vector<ValueType> & v1=it->second.first;
      const std::vector<ValueType> & v2=it->second.second;
      const std::size_t count1=v1.size();
      const std::size_t count2=v2.size();
      if(!count1 || count2!=count1) {
         std::cerr << "Inconsistent starting and stopping for " << it->first << " Starts: " << count1 << " Stops: " << count2 << std::endl;
         itkExceptionMacro("Inconsistent starting and stopping for " << it->first << " Starts: " << count1 << " Stops: " << count2);
      }
      for(std::size_t i=0;i<count1; ++i) {
         const ValueType pval=v2[i]-v1[i];
         min=std::min(pval,min);
         max=std::max(pval,max);
         total+=pval;
         variance+=pval*pval;
      }
      variance-=mean*mean/static_cast<ValueType>(count1);
      mean=total/static_cast<ValueType>(count1);
      if(count1>1) {
         variance/=static_cast<ValueType>(count1-1);
         stdev=sqrt(variance);
      } else {
         // variance undefined
         variance=-1;
         stdev=-1;
      }
      {
         std::stringstream ss;
         ss << std::setw(m_longestName+1) << it->first;
         ss << std::setw(outwide) << count1 
            << std::setw(outwide) << min 
            << std::setw(outwide) << mean 
            << std::setw(outwide) << stdev 
            << std::setw(outwide) << max 
            << std::setw(outwide) << total ;
         os << ss.str() << "\n";
      }
      ++it;
   }
}


} // end namespace itkmod
