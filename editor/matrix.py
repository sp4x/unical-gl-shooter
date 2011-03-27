

values = {'free_space':'0', 'wall':'1', 'turret':'t', 'win':'w', 'cube':'c', 'light':'l' }
default_value = '0'



class Matrix():
    def __init__(self, lines=20, cols=20, filename=None):
        self.data = []
        self.lines = lines
        self.cols = cols
        if filename != None:
            self.load(filename)
        else:
            for c in range(lines*cols):
                self.data.append(default_value)

    def load(self, filename):
        self.data = []
        file = open(filename)
        size = file.readline().split(' ')
        self.lines = int(size[0])
        self.cols = int(size[1])
        for line in file.readlines():
            for c in line[:-1]:
                self.data.append(c)
        file.close()

    def get(self, i, j):
        return self.data[i*self.cols+j]

    def set(self, i, j, value):
        self.data[i*self.cols+j] = value

    def store(self, filename):
        file = open(filename, 'w')
        file.write( str(self.lines) + ' ' + str(self.cols) + '\n' )
        for line in range(self.lines):
            start = line*self.cols
            end = start+self.cols
            file.write(''.join(self.data[start:end])+'\n')
        file.close()



