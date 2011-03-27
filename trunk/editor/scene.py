import matrix
from PyQt4 import QtGui, QtCore


class Pencil():
    def free_space(self, painter, x, y):
        painter.setBrush( QtGui.QColor(255, 255, 255) )
        painter.drawRect(x, y, self.cellsize, self.cellsize)

    def wall(self, painter, x, y):
        painter.setBrush( QtGui.QColor(0, 0, 0) )
        painter.drawRect(x, y, self.cellsize, self.cellsize)

    def turret(self, painter, x, y):
        painter.setBrush( QtGui.QColor(255, 0, 0) )
        painter.drawRect(x, y, self.cellsize, self.cellsize)

    def cube(self, painter, x, y):
        painter.setBrush( QtGui.QColor(0, 255, 0) )
        painter.drawRect(x, y, self.cellsize, self.cellsize)

    def light(self, painter, x, y):
        painter.setBrush( QtGui.QColor(255, 255, 0) )
        painter.drawRect(x, y, self.cellsize, self.cellsize)

    def win(self, painter, x, y):
        painter.setBrush( QtGui.QColor(0, 0, 255) )
        painter.drawRect(x, y, self.cellsize, self.cellsize)




class Scene(QtGui.QWidget, Pencil):
    def __init__(self, data, parent=None):
        QtGui.QWidget.__init__(self, parent)
        self.data = data
        self.lines = data.lines
        self.cols = data.cols
        self.cellsize = 20
        self.width = 0
        self.height = 0
        self.value = matrix.default_value
        self.redim()
        self.functions = {}
        for f in matrix.values.keys():
            key = matrix.values[f]
            self.functions[key] = getattr(self, f)

    def save(self, filename):
        self.data.store(filename)

    def open(self, filename):
        self.data = matrix.Matrix(filename=filename)
        self.lines = self.data.lines
        self.cols = self.data.cols
        self.redim()
        self.repaint()


    def redim(self):
        self.width = self.cols*self.cellsize
        self.height = self.lines*self.cellsize
        self.resize(self.width, self.height)

    def dim(self):
        return str(self.lines)+'x'+str(self.cols)

    def zoom(self, dir):
        if dir == '+':
            self.cellsize += 1
        else:
            self.cellsize -= 1
        self.redim()


    def paintEvent(self, event):
        qp = QtGui.QPainter()
        qp.begin(self)
        for line in range(self.lines):
            for col in range(self.cols):
                cell = self.data.get(line, col)
                x = col*self.cellsize
                y = line*self.cellsize
                self.functions[cell](qp, x, y)
        qp.end()



    def mouseMoveEvent(self, event):
        if event.buttons() == QtCore.Qt.RightButton:
            setval = matrix.default_value
        else:
            setval = self.value
        line = event.y()/self.cellsize
        col = event.x()/self.cellsize
        self.data.set(line, col, setval)
        self.repaint()



