#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>

class MainWindow : public QMainWindow {
public:
	MainWindow(QWidget *parent, QImage& image);
	~MainWindow();

	void keyPressEvent(QKeyEvent* event);

private:
};