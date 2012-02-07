
#ifndef __SumDataPlot_hxx__
#define __SumDataPlot_hxx__

#include "DataPlot.hxx"

#include <string>
class QWidget;

namespace GUI
{

class SumDataPlot : public DataPlot
{ 
public:
	SumDataPlot( QWidget * parent );
	virtual ~SumDataPlot();
	void calculateValues( const std::string & type, const std::string & state );
};

} // namespace GUI

#endif // __SumDataPlot_hxx__

