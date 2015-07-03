#ifndef Context_H
#define Context_H

#include "StdIncl.hpp"
#include "types/Matrix.hpp"

namespace uipf{


// context 	is a container providing access to the current environment, allowing to open windows, write to logger etc...
class Context {
	public:
		// constructor
		Context(void) : bStopRequested_(false) {};
		// destructor
		~Context(void){};

		// display an image in a window
		void displayImage(const std::string strTitle, const Matrix& oMat, bool bBlocking) const;

		//if set, modules should abort their work (the stop button has been pressed in the GUI).
		//it is their responsibility to check this flag periodically.
		//otherwise they get killed automatically after a grace period
		volatile bool bStopRequested_;
};

} // namespace

#endif
