
#ifndef __LoadingProgressBar_hxx__
#define __LoadingProgressBar_hxx__

#include "ui_LoadingProgressBar.h"

namespace GUI
{

class LoadingProgressBar : public QWidget
{
	Q_OBJECT

	Ui::LoadingProgressBar _loadingProgressBar;
public:
	LoadingProgressBar();
	virtual ~LoadingProgressBar();
	void initLoading();

public slots:
	void updateProgress();
};

} // namespace GUI

#endif // __LoadingProgressBar_hxx__

