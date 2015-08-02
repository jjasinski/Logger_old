#pragma once

//------------------------------------------

// INCLUDES
#include "logger/MessageService.hpp"
#include "logger/Message.hpp"

#include "logger/StringHelpers.hpp"

//------------------------------------------
// BEGIN OF CLASS DECLARATION

namespace logger
{

  class Logger
  {
  public:
    typedef std::shared_ptr< MessageService > MessageServicePtr;

    /**
     */
    Logger(const std::string aName);

    const std::string name;

    template<typename... Args>
    void fatal(const CallInfo& anInfo, const Args&... args)
    {
      writeNewMessage(MessageLevel::FATAL, anInfo, args...);
    }
    
    template<typename... Args>
    void error(const CallInfo& anInfo, const Args&... args)
    {
      writeNewMessage(MessageLevel::ERROR, anInfo, args...);
    }
    
    template<typename... Args>
    void warning(const CallInfo& anInfo, const Args&... args)
    {
      writeNewMessage(MessageLevel::ERROR, anInfo, args...);
    }

    template<typename... Args>
    void info(const CallInfo& anInfo, const Args&... args)
    {
      writeNewMessage(MessageLevel::INFO, anInfo, args...);
    }

    template<typename... Args>
    void debug(const CallInfo& anInfo, const Args&... args)
    {
      writeNewMessage(MessageLevel::DEBUG, anInfo, args...);
    }

    template<typename... Args>
    void debugFine(const CallInfo& anInfo, const Args&... args)
    {
      writeNewMessage(MessageLevel::DEBUG_FINE, anInfo, args...);
    }

    template<typename... Args>
    void debugFiner(const CallInfo& anInfo, const Args&... args)
    {
      writeNewMessage(MessageLevel::DEBUG_FINER, anInfo, args...);
    }

    template<typename... Args>
    void debugFinest(const CallInfo& anInfo, const Args&... args)
    {
      writeNewMessage(MessageLevel::DEBUG_FINEST, anInfo, args...);
    }

    template<typename... Args>
    void trace(const CallInfo& anInfo, const Args&... args)
    {
      writeNewMessage(MessageLevel::TRACE, anInfo, args...);
    }

    template<typename... Args>
    void trivial(const CallInfo& anInfo, const Args&... args)
    {
      writeNewMessage(MessageLevel::TRIVIAL, anInfo, args...);
    }

    MessageLevel getLevel() const { return loggingLevel; }
    void setLevel(MessageLevel aLevel) { loggingLevel = aLevel; }

    MessageServicePtr getMessageService() { return messageService; }
    void setMessageService(MessageServicePtr aService);
  private:
    MessageLevel loggingLevel;
    MessageServicePtr messageService;

    template<typename... Args>
    void writeNewMessage(const MessageLevel aLevel, const CallInfo& anInfo, const Args&... args)
    {
      if (loggingLevel >= aLevel)
      {
        if (auto message = messageService->createMessage(aLevel, anInfo))
        {
          message->content = makeString(args...);
          messageService->writeMessage(std::move(message));
        }
      }
    }
  };

  ///**
  //*/
  //enum DebugLevel
  //{
  //	NO_DEBUG,
  //	LEVEL_DEBUG,
  //	LEVEL_FINE,
  //	LEVEL_FINER,
  //	LEVEL_FINEST
  //};

  ///** Interface for logger instance
  // *  any module can have own instance with a specific configuration
  // *  messageFactory defined strategy of formatting and writing log message
  // *  note: const functions should be thread-safe
  // */
  //class Logger
  //{
  //public:
  //	/// defines friendship with loggers container
  //	friend class Registry;
  //	///
  //	bool operator<(const Logger& aRight) const
  //	{
  //		return name() < aRight.name();
  //	}
  //	template<typename... Args>
  //	void error(const CallInfo& aCallInfo, const Args&... args) const
  //	{
  //		auto message = theMessageFactory->createErrorMessage(aCallInfo);
  //		processMessage(std::move(message), args...);
  //	}
  //	///
  //	template<typename... Args>
  //	void warning(const CallInfo& aCallInfo, const Args&... args) const
  //	{
  //		auto message = theMessageFactory->createWarningMessage(aCallInfo);
  //		processMessage(std::move(message), args...);
  //	}
  //	///
  //	template<typename... Args>
  //	void info(const CallInfo& aCallInfo, const Args&... args) const
  //	{
  //		auto message = theMessageFactory->createInfoMessage(aCallInfo);
  //		processMessage(std::move(message), args...);
  //	}
  //	///
  //	template<typename... Args>
  //	void debug(const CallInfo& aCallInfo, const Args&... args) const
  //	{
  //		debugForLevel(LEVEL_DEBUG, aCallInfo, args...);
  //	}
  //	///
  //	template<typename... Args>
  //	void debug_fine(const CallInfo& aCallInfo, const Args&... args) const
  //	{
  //		debugForLevel(LEVEL_FINE, aCallInfo, args...);
  //	}
  //	///
  //	template<typename... Args>
  //	void debug_finer(const CallInfo& aCallInfo, const Args&... args) const
  //	{
  //		debugForLevel(LEVEL_FINER, aCallInfo, args...);
  //	}
  //	///
  //	template<typename... Args>
  //	void debug_finest(const CallInfo& aCallInfo, const Args&... args) const
  //	{
  //		debugForLevel(LEVEL_FINEST, aCallInfo, args...);
  //	}
  //	///
  //	const std::string& name() const
  //	{
  //		return theName;
  //	}
  //	///
  //	DebugLevel debugLevel() const
  //	{
  //		return theDebugLevel;
  //	}
  //	///
  //	void setDebugLevel(DebugLevel aLevel)
  //	{
  //		theDebugLevel = aLevel;
  //	}
  //	///
  //	bool isDebug() const
  //	{
  //		return theDebugLevel != NO_DEBUG;
  //	}
  //	/////
  //	//void enableDebug()
  //	//{
  //	//	if (theDebugLevel == NO_DEBUG)
  //	//	{
  //	//		theDebugLevel = LEVEL_DEBUG;
  //	//	}
  //	//}
  //	///
  //	void disableDebug()
  //	{
  //		theDebugLevel = NO_DEBUG;
  //	}
  //	/// used for change MessageFactory in current instance
  //	void setMessageFactory(std::shared_ptr<MessageFactory> aMessageFactory);
  //	///
  //	void setMessageOutput(std::shared_ptr<MessageOutput> aMessageOutput);
  //	///
  //	void flush()
  //	{
  //		theMessageOutput->flush();
  //	}
  //private:
  //	/// private constructor calls from Registry
  //	explicit Logger(const std::string& aName);
  //	///
  //	const std::string theName;
  //	std::shared_ptr<MessageFactory> theMessageFactory;
  //	std::shared_ptr<MessageOutput> theMessageOutput;
  //	DebugLevel theDebugLevel;

  //	template<typename... Args>
  //	void processMessage(std::unique_ptr<Message> aMessage, const Args&... args) const
  //	{
  //		if (aMessage)
  //		{
  //			aMessage->rawMessage = makeString(args...);
  //			theMessageOutput->addMessage(std::move(aMessage));
  //		}
  //	}

  //	///
  //	template<typename... Args>
  //	void debugForLevel(DebugLevel aLevel, const CallInfo& aCallInfo, const Args&... args) const
  //	{
  //		if (aLevel <= debugLevel())
  //		{
  //			auto message = theMessageFactory->createDebugMessage(aCallInfo);
  //			processMessage(std::move(message), args...);
  //		}
  //	}
  //};

}// end logger
