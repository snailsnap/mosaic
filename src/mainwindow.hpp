#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMoveEvent>

class MainWindow : public QMainWindow {
public:
	MainWindow(QWidget *parent, QImage& image);
	~MainWindow();

	void keyPressEvent(QKeyEvent* event);
	void showSidebar();

private:
};