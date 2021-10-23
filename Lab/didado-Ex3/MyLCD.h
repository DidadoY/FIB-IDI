#include <QLCDNumber>

class MyLCD:public QLCDNumber
{
	Q_OBJECT
	public:
		MyLCD(QWidget *parent);
		int num;

	public slots:
		void incrementar();
		void resetear();
};
