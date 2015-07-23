#pragma once

//------------------------------------------

// INCLUDES
#include <memory>
#include "logger/Message.hpp"

//------------------------------------------
// BEGIN OF CLASS DECLARATION

namespace logger {

  /**
   */
  class MessageService
  {
  public:
    typedef std::unique_ptr< Message > MessagePtr;
    MessageService() = default;
    virtual ~MessageService() = default;

    virtual MessagePtr createMessage(MessageLevel aLevel, const CallInfo& anInfo) = 0;
    virtual void writeMessage(MessagePtr aMessage) = 0;
  };

  /** Null Object Pattern implementation
   */
  class NullMessageService : public MessageService
  {
    // Inherited via MessageService
    virtual MessagePtr createMessage(MessageLevel aLevel, const CallInfo & anInfo) override
    {
      return nullptr;
    }

    virtual void writeMessage(MessagePtr aMessage) override
    {
      // empty line
    }
  };

}// end logger
