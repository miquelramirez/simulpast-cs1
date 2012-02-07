
#ifndef __RasterConfiguration_hxx__
#define __RasterConfiguration_hxx__

namespace GUI
{
class ColorSelector;

class RasterConfiguration
{
	ColorSelector * _colorSelector;
	int _minValue;
	int _maxValue;

	bool _transparentEnabled;
	int _transparentValue;
public:
	RasterConfiguration( const int & minValue = 0, const int & maxValue = 10);
	RasterConfiguration( const RasterConfiguration & prototype );
	virtual ~RasterConfiguration();

	ColorSelector & getColorRamp();
	const ColorSelector & getColorRamp() const;
	void resetColorRamp();

	const int & getMinValue() const;
	const int & getMaxValue() const;

	const bool & isTransparentEnabled() const;
	void setTransparentEnabled( const bool & transparentEnabled );
	const int & getTransparentValue() const;
	void setTransparentValue( const int & transparentValue );

}; // class RasterConfiguration

} // namespace GUI

#endif // __RasterConfiguration_hxx__

