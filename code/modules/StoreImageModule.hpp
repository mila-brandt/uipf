#ifndef _STOREIMAGEMODULE_
#define _STOREIMAGEMODULE_

#include "../framework/ModuleInterface.hpp"
#include "../framework/ModuleBase.hpp"
#include <QObject>
#include <QtPlugin>

namespace uipf{


class StoreImageModule :  public QObject, ModuleBase, ModuleInterface
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.tu-berlin.uipf.ModuleInterface" )
Q_INTERFACES(uipf::ModuleInterface)

	public:
		// constructor
		StoreImageModule(void){};
		// destructor
		~StoreImageModule(void){};

		// runs the module chain with the params
		void run( std::map<std::string, uipf::Data::ptr& >& input,
			  std::map<std::string, std::string>& params, 
		   	  std::map<std::string, uipf::Data::ptr >& output ) const Q_DECL_OVERRIDE;

		std::string name() const Q_DECL_OVERRIDE;
};

}

#endif //STOREIMAGEMODULE
