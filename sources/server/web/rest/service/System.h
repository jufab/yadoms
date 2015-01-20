#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "web/rest/RestDispatcher.h"
#include "RunningInformation.h"

namespace web { namespace rest { namespace service {

   class CSystem : public IRestService
   {
   public:
	  CSystem(boost::shared_ptr<CRunningInformation> runningInformation);
      virtual ~CSystem();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      const std::string & getRestKeyword();

   public:
      shared::CDataContainer getBinding(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);
      shared::CDataContainer getSystemInformation(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);

   private:
      shared::CDataContainer getSerialPorts();
      shared::CDataContainer getNetworkInterfaces(bool includeLoopback);

   private:
      static std::string m_restKeyword;

	  boost::shared_ptr<CRunningInformation> m_runningInformation;
   };


} //namespace service
} //namespace rest
} //namespace web 
