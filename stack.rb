
class Stack

  def initialize(size=0, type=Object)
    @arr = Array.new(size, type)
  end

  def push(elem)
    @arr.push(elem)
  end

  def pop
    @arr.pop
  end

  def top
    @arr.last
  end
  
end
