#ifndef MODULEINTERFACE_H
#define MODULEINTERFACE_H

#include <QObject>
#include <QtPlugin>
#include <string>
#include <map>
#include "types/Data.hpp"
#include "Context.hpp"
#include "MetaData.hpp"
#include "InvalidConfigException.hpp"
#include "ErrorException.hpp"
#include "DataManager.hpp"

namespace uipf{


// describes the m
class ModuleInterface {
	public:
		// constructor (can't be virtual!)
		ModuleInterface(void){};
		// destructor
		virtual ~ModuleInterface(void){};

		// method has to be implemented in the Module
		// DataManager provides access to all resources described in the module meta description (params,input,output)
		virtual void run( DataManager& data ) const = 0;

		// the name of the module, which can be referenced in the yaml configuration
		virtual std::string name() const = 0;

		// meta data that contains description of modules inputs, outputs and parameters
		virtual uipf::MetaData getMetaData() const = 0;

		// context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
		virtual void setContext(Context*) = 0;
};

} //namespace

QT_BEGIN_NAMESPACE
#define ModuleInterface_iid "org.tu-berlin.uipf.ModuleInterface"
Q_DECLARE_INTERFACE(uipf::ModuleInterface,ModuleInterface_iid)
QT_END_NAMESPACE


#endif // MODULEINTERFACE_H

