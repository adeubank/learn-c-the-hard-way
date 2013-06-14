

class Stack:
  """ A simple stack class """
  def __init__(self):
    self.arr = []

  def push(self, elem):
    self.arr.insert(-1, elem)

  def pop(self):
    return self.arr.pop()

  def top(self):
    return self.arr[-1]


