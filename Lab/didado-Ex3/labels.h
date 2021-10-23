#include <QLabel>

class labels:public QLabel
{
	Q_OBJECT
	public:
		labels(QWidget *parent);
		int num, desenes, unitats;

	public slots:
		void incrementar();
		void resetear();
};
