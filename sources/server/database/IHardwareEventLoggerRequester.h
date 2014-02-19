#pragma once

#include "entities/HardwareEventLogger.h"
#include <shared/StringExtension.h>

class IHardwareEventLoggerRequester
{
public:
   //--------------------------------------------------------------
   /// \brief           Add new hardware event log entry
   /// \param [in]      pluginName   the plugin name
   /// \param [in]      pluginVersion   the plugin version
   /// \param [in]      pluginReleaseType   the plugin release type
   /// \param [in]      eventType   the event type
   /// \param [in]      message   the message associated to event
   /// \return          harwareId generated by the database
   /// \throw           CEmptyResultException if fails
   //--------------------------------------------------------------
   virtual int addEvent(const std::string & pluginName, const std::string & pluginVersion, const shared::plugin::IInformation::EReleaseType pluginReleaseType, const CHardwareEventLogger::EEventType eventType, const std::string & message = CStringExtension::EmptyString) = 0;

   //--------------------------------------------------------------
   /// \brief           Add new hardware event log entry
   /// \param [in]      hardwareLogEntry   new hardware event log entry
   /// \return          eventId generated by the database
   /// \throw           CEmptyResultException if fails
   //--------------------------------------------------------------
   virtual int addEvent(const CHardwareEventLogger & hardwareLogEntry) = 0;
   
   //--------------------------------------------------------------
   /// \brief           Get the events history for a plugin
   /// \param [in]      pluginName   the plugin name
   /// \param [in]      pluginVersion   the plugin version
   /// \param [in]      pluginReleaseType   the plugin release type
   /// \return          list of events
   /// \throw           CEmptyResultException if fails
   //--------------------------------------------------------------
   virtual std::vector<boost::shared_ptr<CHardwareEventLogger> > getHardwareEvents(const std::string & pluginName, const std::string & pluginVersion, const shared::plugin::IInformation::EReleaseType pluginReleaseType) = 0;

   //--------------------------------------------------------------
   /// \brief           Get the events history for a plugin
   /// \param [in]      pluginName   the plugin name
   /// \param [in]      pluginVersion   the plugin version
   /// \param [in]      pluginReleaseType   the plugin release type
   /// \param [in]      fromDate   the date from which to take data
   /// \return          list of events
   /// \throw           CEmptyResultException if fails
   //--------------------------------------------------------------
   virtual std::vector<boost::shared_ptr<CHardwareEventLogger> > getHardwareEvents(const std::string & pluginName, const std::string & pluginVersion, const shared::plugin::IInformation::EReleaseType pluginReleaseType, const boost::posix_time::ptime & fromDate) = 0;

   //--------------------------------------------------------------
   /// \brief       Destructor
   //--------------------------------------------------------------
   virtual ~IHardwareEventLoggerRequester()
   {
   }
};
