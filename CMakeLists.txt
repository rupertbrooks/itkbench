PROJECT(itkbench)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
    if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 OLD)
    endif(COMMAND cmake_policy)

FIND_PACKAGE(ITK REQUIRED)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
 MESSAGE(FATAL_ERROR
        "Cannot build without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)

SET(BENCHSRC 
    benchImage2D.cxx 
	 benchImage3D.cxx
	 benchFilterChain.cxx
	 benchMeanSquares.cxx
	 benchResample.cxx
)

SET(BENCHHDR
    CBenchmark.h
    benchImage2D.h 
	 benchImage3D.h
	 benchInterpolator.h
	 benchFilterChain.h
	 benchMeanSquares.h
	 benchResample.h
)

ADD_EXECUTABLE(itkbench itkbench.cxx itkmodTimeProbeCollector.cxx itkmodTimeProbeCollector.h ${BENCHSRC} ${BENCHHDR})
TARGET_LINK_LIBRARIES ( itkbench  ${ITK_LIBRARIES})
