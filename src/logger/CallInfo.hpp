#pragma once

//------------------------------------------

// INCLUDES

//------------------------------------------
// BEGIN OF CLASS DECLARATION

#ifdef _MSC_VER // TODO
#define LOGGER_CALL_INFO ::logger::CallInfo(__FUNCTION__, __FUNCSIG__, __FILE__, __LINE__)
#else
#define LOGGER_CALL_INFO ::logger::CallInfo(__FUNCTION__, __PRETTY_FUNCTION__, __FILE__, __LINE__)
#endif

namespace logger {

  /** Definition of a function call
  */
  struct CallInfo
  {
    CallInfo(const char* aFunc, const char* aFunctionDecorated, const char* aFile, unsigned int aLine)
      :
      function(aFunc),
      functionDecorated(aFunctionDecorated),
      file(aFile),
      line(aLine)
    {
    }

    CallInfo(const CallInfo& info) = default;

    const char* function;
    const char* functionDecorated;
    const char* file;
    const unsigned int line;
  };

}// end logger
