#include "graph_vis.h"
#include "ui_graph_vis.h"

Graph_vis::Graph_vis(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Graph_vis)
{
    ui->setupUi(this);
   QPixmap p("graph.dot.png");

    ui->label->setPixmap(p);
}

Graph_vis::~Graph_vis()
{
    delete ui;
}
