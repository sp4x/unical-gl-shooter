#!/usr/bin/python


import sys
import matrix
from scene import Scene
from PyQt4 import QtGui, QtCore

class Area(QtGui.QWidget):
    def __init__(self, data, parent=None):
        QtGui.QWidget.__init__(self, parent)

        hlayout = QtGui.QHBoxLayout()
        vlayout = QtGui.QVBoxLayout()
        self.scene = Scene(data)
        self.label = QtGui.QLabel(self.scene.dim(), self)

        self.buttons = matrix.values.copy()
        for c in self.buttons.keys():
            radio = QtGui.QRadioButton(c, self)
            self.connect(radio, QtCore.SIGNAL('toggled(bool)'), self.pen )
            hlayout.addWidget(radio)
            self.buttons[c] = radio
        self.buttons['wall'].setChecked(True)

        scrollArea = QtGui.QScrollArea()
        scrollArea.setWidget(self.scene)

        vlayout.addWidget( self.label )
        vlayout.addWidget(scrollArea)
        vlayout.addLayout(hlayout)

        self.setLayout(vlayout)

    def pen(self, checked):
        if checked:
            chosen = False
            for radio in self.buttons.keys():
                if self.buttons[radio].isChecked():
                    self.scene.value = matrix.values[radio]
                    chosen = True


    def zoomIn(self):
        self.scene.zoom('+')

    def zoomOut(self):
        self.scene.zoom('-')

    def setDimLabel(self, text):
        self.label.setText(text)
        print text



class Window(QtGui.QMainWindow):
    def __init__(self, data):
        QtGui.QMainWindow.__init__(self)
        self.setWindowTitle('Editor')
        self.area = Area(data)

        width = self.area.scene.width
        height = self.area.scene.height
        self.setGeometry(200,200,width,height)

        self.setCentralWidget(self.area)
        self.buildToolbar()


    def buildToolbar(self):
        toolbar = self.addToolBar('toolbar')

        zoom_in_action = QtGui.QAction('Zoom in' ,self)
        zoom_out_action = QtGui.QAction('Zoom out' ,self)
        save_action = QtGui.QAction('Save' ,self)
        open_action = QtGui.QAction('Open' ,self)

        toolbar.addAction(save_action)
        toolbar.addAction(open_action)
        toolbar.addAction(zoom_in_action)
        toolbar.addAction(zoom_out_action)

        self.connect(zoom_in_action, QtCore.SIGNAL('triggered()'), self.area.zoomIn)
        self.connect(zoom_out_action, QtCore.SIGNAL('triggered()'), self.area.zoomOut)
        self.connect(save_action, QtCore.SIGNAL('triggered()'), self.save)
        self.connect(open_action, QtCore.SIGNAL('triggered()'), self.open)


    def save(self):
        filename = QtGui.QFileDialog.getSaveFileName(self)
        self.area.scene.save(filename)

    def open(self):
        filename = QtGui.QFileDialog.getOpenFileName(self)
        self.area.scene.open(filename)
        dim  = self.area.scene.dim()
        self.area.setDimLabel(dim)



if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    if len(sys.argv) > 2:
        lines = int(sys.argv[1])
        cols = int(sys.argv[2])
        data = matrix.Matrix(lines, cols)
    else:
        data = matrix.Matrix()
    win = Window(data)
    win.show()
    sys.exit(app.exec_())
