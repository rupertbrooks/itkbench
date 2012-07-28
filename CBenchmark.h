#pragma once


class CBenchmark {
public:
   CBenchmark() {}
   virtual const char * GetNameOfBenchmark() const =0;
   virtual void operator()() const=0;
};

