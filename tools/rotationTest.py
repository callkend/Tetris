


class point():

    def __init__(self, x, y):

        self.x = x
        self.y = y
 
    def __str__(self):
        return "{}, {}".format(self.x, self.y)

    def transpose(self):
        t = self.x
        self.x = self.y
        self.y = t

    def rotateClockwise(self):

        self.transpose()
        self.y = self.y * -1

    def rotateAntiClockwise(self):

        self.y = self.y * -1
        self.transpose()


p = point(1, 2)
print(p)

for x in range (0, 7):
    p.rotateAntiClockwise()
    print(p)

