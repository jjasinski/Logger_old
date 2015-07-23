#pragma once

//------------------------------------------

// INCLUDES
#include <string>
#include "logger/CallInfo.hpp"

//------------------------------------------
// BEGIN OF CLASS DECLARATION

namespace logger {
  /**
   */
  enum class MessageLevel
  {
    FATAL,    /// fatal error - generally causes stop/crash of the application
    ERROR,    /// normal error - something is wrong
    WARNING,  /// warning - 
    INFO,
    DEBUG,
    DEBUG_FINE,
    DEBUG_FINER,
    DEBUG_FINEST,
    TRACE,
    TRIVIAL
  };

  /**
   */
  class Message
  {
  public:
    Message(MessageLevel aLevel, const CallInfo& anInfo)
      :
      level(aLevel),
      info(anInfo)
    {
    }

    virtual ~Message() = default;

    const MessageLevel level;
    const CallInfo info;
    std::string content;
  };

}// end logger
