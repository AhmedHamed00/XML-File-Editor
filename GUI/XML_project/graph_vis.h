#ifndef GRAPH_VIS_H
#define GRAPH_VIS_H

#include <QDialog>

namespace Ui {
class Graph_vis;
}

class Graph_vis : public QDialog
{
    Q_OBJECT

public:
    explicit Graph_vis(QWidget *parent = nullptr);
    ~Graph_vis();

private:
    Ui::Graph_vis *ui;
};

#endif // GRAPH_VIS_H
