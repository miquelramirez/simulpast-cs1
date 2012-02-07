
#include <QApplication>

#include "MainWindow.hxx"
#include "Exceptions.hxx"
#include <iostream>

int main(int argc, char *argv[])
{
	srand(time(0));
	try
	{
		QApplication app(argc, argv);
		GUI::MainWindow mainWindow;
		mainWindow.show();
		return app.exec();
	}
	catch( Engine::Exception & exceptionThrown )
	{
		std::cout << "app exception thrown: " << exceptionThrown.what() << std::endl;
	}
	catch( std::exception & exceptionThrown )
	{
		std::cout << "std exception thrown: " << exceptionThrown.what() << std::endl;
	}
	return 0;
}

