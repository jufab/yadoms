#pragma once
#include "IScriptLoader.h"
#include "IScriptFile.h"

//--------------------------------------------------------------
/// \brief	Python script loader
//--------------------------------------------------------------
class CScriptLoader : public IScriptLoader
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   ///\param[in] scriptPath   Script path (without name)
   /// \throw CRunnerException if unable to load script
   //--------------------------------------------------------------
   CScriptLoader(const std::string& scriptPath);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CScriptLoader();

   // IScriptLoader Implementation
   virtual void load();
   virtual CPythonObject& module();
   virtual CPythonObject& yMain();
   // [END] IScriptLoader Implementation

private:
   //--------------------------------------------------------------
   /// \brief	Script file
   //--------------------------------------------------------------
   const boost::shared_ptr<IScriptFile> m_scriptFile;

   //--------------------------------------------------------------
   /// \brief	Module Python object
   //--------------------------------------------------------------
   CPythonObject m_pyModule;

   //--------------------------------------------------------------
   /// \brief	yMain function Python object
   //--------------------------------------------------------------
   CPythonObject m_pyMainFunction;

   //--------------------------------------------------------------
   /// \brief	Wrapper file
   //--------------------------------------------------------------
   static const boost::filesystem::path WrapperModuleFile;
};


