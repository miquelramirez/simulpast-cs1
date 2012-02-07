
#ifndef __MeanDataPlot_hxx__
#define __MeanDataPlot_hxx__

#include "DataPlot.hxx"

#include <string>
class QWidget;

namespace GUI
{

class MeanDataPlot : public DataPlot
{ 
public:
	MeanDataPlot( QWidget * parent );
	virtual ~MeanDataPlot();
	void calculateValues( const std::string & type, const std::string & state );
};

} // namespace GUI

#endif // __MeanDataPlot_hxx__

