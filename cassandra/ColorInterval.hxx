
#ifndef __ColorInterval_hxx__
#define __ColorInterval_hxx__

#include <QWidget>

#include "ui_ColorInterval.h"

namespace GUI
{

class ColorInterval : public QWidget
{
	Q_OBJECT

	Ui::ColorInterval _colorInterval;
	QColor _color;

private slots:
	void selectColor();
	void split();
	void remove();

public slots:
	void colorSelected( const QColor & color );
	void checkMinimum( int value);
	void checkMaximum( int value);

	void adjustMaxValue(int value);
	void adjustMinValue(int value);
	
	// update maxValue QSpinBox when minValue of next QSpinBox is modified (max Value = next min Value - 1
	void updateMaxValue(int);
	// update minimum of QSpinbox minValue to value of previous min value + 1
	void updateMinValue(int);

public:
	ColorInterval(QWidget * parent = 0);
	virtual ~ColorInterval();
	void setMinValue( const int & value );
	void setMaxValue( const int & value );
	void setColor( const QColor & color );

	int getMinValue();
	int getMaxValue();
	const QColor & getColor();

	void connectToNext( ColorInterval * next );
	void disconnectMaxValue();
	void disconnectMinValue();

	void deactivateMinValue();

signals:
	void splitColorInterval();
	void removeColorInterval();
	// emit a signal when maximum is modified, to adjust maximum of the interval before this to value-1
	void adjustMax(int);
	// emit a signal when minimum is modified, to adjust maximum of the interval before this
	void adjustMin(int);
};

} // namespace GUI

#endif // __ColorInterval_hxx__

