#include "stdafx.h"
#include "Astronomy.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

CAstronomy::CAstronomy(boost::shared_ptr<yApi::IYPluginApi> context, IWUConfiguration& WUConfiguration, const std::string & PluginName, const std::string & Prefix):
           m_Localisation         ( WUConfiguration.getLocalisation() ),
           m_CountryOrState       ( WUConfiguration.getCountryOrState() ),
           m_PluginName           ( PluginName ),
           m_MoonCharacteristics  ( PluginName, Prefix + "Moon" )
{
	m_CatchError = false;
	m_URL.str("");
	m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/astronomy/q/" << m_CountryOrState << "/" << m_Localisation << ".json";

   try 
   {
	   if (WUConfiguration.IsAstronomyEnabled())
	   {
         m_MoonCharacteristics.Initialize  ( context );

         m_MoonCharacteristics.AddUnit(
                                          shared::plugin::yPluginApi::CStandardCapacities::Load.getName(),
                                          shared::plugin::yPluginApi::CStandardCapacities::Load.getUnit() 
                                      );
	   }
   }
   catch (shared::exception::CException& e)
   {
	   YADOMS_LOG(warning) << "Configuration or initialization error of Astronomy module :" << e.what() << std::endl;

       context->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError" );
	   m_CatchError = true;
   }
}

void CAstronomy::OnUpdate( IWUConfiguration& WUConfiguration )
{
   m_Localisation = WUConfiguration.getLocalisation();

   //read the country or State code
   m_CountryOrState = WUConfiguration.getCountryOrState();
	
	m_URL.str("");

	m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/astronomy/q/" << m_CountryOrState << "/" << m_Localisation << ".json";
}

bool CAstronomy::Request( boost::shared_ptr<yApi::IYPluginApi> context )
{
	try
	{
		m_CatchError = false;
	   m_data = m_webServer.SendGetRequest( m_URL.str() );
	}
    catch (shared::exception::CException& e)
	{
      YADOMS_LOG(warning) << "Astronomy :" << e.what()  << std::endl;
	  context->setPluginState(yApi::historization::EPluginState::kCustom, "NoConnection" );
	  m_CatchError = true;
	}

   return m_CatchError;
}

void CAstronomy::Parse( boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration )
{
	try
	{
		std::string error = m_data.getWithDefault<std::string>( "response.error.description","" );

		if (!error.empty())
		{
			m_CatchError = true;
			YADOMS_LOG(error) << "ERROR : " << error  << std::endl;

			if (error.compare ("No cities match your search query") == 0)
				context->setPluginState(yApi::historization::EPluginState::kCustom, "CityNotFound" );
		}
		else
		{
			std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > KeywordList;

			if (WUConfiguration.IsAstronomyEnabled())
			{
            m_MoonCharacteristics.SetParameters  (m_data,
                                                  "moon_phase.percentIlluminated",
                                                  "moon_phase.ageOfMoon" );

            KeywordList.push_back                (m_MoonCharacteristics.GetHistorizable());
			}

			context->historizeData(m_PluginName, KeywordList);
		}
	}
   catch (shared::exception::CException& e)
	{
      YADOMS_LOG(warning) << e.what() << std::endl;
	}
}

bool CAstronomy::IsModuleInFault ( void )
{
	return m_CatchError;
}

CAstronomy::~CAstronomy()
{}
