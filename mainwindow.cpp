#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), startNode(-1), endNode(-1)
{
    graph.loadFromXML("Harta_Luxemburg.xml");
}

MainWindow::~MainWindow()
{
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    needsRedraw = true;
}
void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const auto& nodes = graph.getNodes();
    if (nodes.empty())
        return;

    double minLon = graph.getMinLon();
    double maxLat = graph.getMaxLat();
    double widthGeo = graph.getMaxLon() - minLon;
    double heightGeo = maxLat - graph.getMinLat();

    auto toScreenX = [&](double lon) {
        return (lon - minLon) / widthGeo * width();
    };

    auto toScreenY = [&](double lat) {
        return (maxLat - lat) / heightGeo * height();
    };

    if (needsRedraw || backgroundPixmap.size() != this->size()) {
        backgroundPixmap = QPixmap(this->size());
        backgroundPixmap.fill(Qt::black);

        QPainter bgPainter(&backgroundPixmap);
        bgPainter.setRenderHint(QPainter::Antialiasing);
        bgPainter.setPen(QPen(Qt::lightGray, 1));

        const auto& adjList = graph.getAdjacencyList();
        QVector<QLineF> lines;

        for (auto const& [id, node] : nodes) {
            if (adjList.count(id)) {
                double x1 = toScreenX(node.getLon());
                double y1 = toScreenY(node.getLat());

                for (const auto& edge : adjList.at(id)) {
                    if (nodes.count(edge.getToNodeId())) {
                        Node dest = nodes.at(edge.getToNodeId());
                        double x2 = toScreenX(dest.getLon());
                        double y2 = toScreenY(dest.getLat());
                        lines.append(QLineF(x1, y1, x2, y2));
                    }
                }
            }
        }
        bgPainter.drawLines(lines);
        needsRedraw = false;
    }

    painter.drawPixmap(0, 0, backgroundPixmap);

    double pointRadius = 5.0;

    if (startNode != -1) {
        painter.setBrush(Qt::green);
        painter.setPen(Qt::black);
        Node n = nodes.at(startNode);
        painter.drawEllipse(QPointF(toScreenX(n.getLon()), toScreenY(n.getLat())), pointRadius, pointRadius);
    }

    if (endNode != -1) {
        painter.setBrush(Qt::red);
        painter.setPen(Qt::black);
        Node n = nodes.at(endNode);
        painter.drawEllipse(QPointF(toScreenX(n.getLon()), toScreenY(n.getLat())), pointRadius, pointRadius);
    }

    if (!currentPath.empty()) {
        QPen pathPen(Qt::blue, 3);
        painter.setPen(pathPen);

        QVector<QLineF> pathLines;
        for (size_t i = 0; i < currentPath.size() - 1; ++i) {
            Node n1 = nodes.at(currentPath[i]);
            Node n2 = nodes.at(currentPath[i+1]);
            pathLines.append(QLineF(toScreenX(n1.getLon()), toScreenY(n1.getLat()),
                                    toScreenX(n2.getLon()), toScreenY(n2.getLat())));
        }
        painter.drawLines(pathLines);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    double clickX = event->x();
    double clickY = event->y();

    int nodeId = graph.getClosestNode(clickX, clickY, width(), height());

    if (event->button() == Qt::LeftButton) {
        startNode = nodeId;
        currentPath.clear();
    } else if (event->button() == Qt::RightButton) {
        endNode = nodeId;
        if (startNode != -1 && endNode != -1) {
            currentPath = graph.dijkstra(startNode, endNode);
        }
    }
    update();
}
