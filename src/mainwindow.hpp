#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>

class MainWindow : public QMainWindow {
public:
	MainWindow(QWidget *parent, QImage& image);
	~MainWindow();

private:
};