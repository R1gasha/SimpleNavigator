from s21py import S21_Stack, S21_Queue


class Stack:
    def __init__(self):
        self._impl = S21_Stack()

    def push(self, value):
        self._impl.push(value)

    def pop(self):
        if self._impl.empty():
            raise IndexError("pop from empty stack")
        value = self._impl.top()
        self._impl.pop()
        return value

    def top(self):
        if self._impl.empty():
            raise IndexError("top from empty stack")
        return self._impl.top()

    def empty(self):
        return self._impl.empty()

    def size(self):
        return self._impl.size()


class Queue:
    def __init__(self):
        self._impl = S21_Queue()

    def push(self, value):
        self._impl.push(value)

    def pop(self):
        if self._impl.empty():
            raise IndexError("pop from empty queue")
        value = self._impl.front()
        self._impl.pop()
        return value

    def front(self):
        if self._impl.empty():
            raise IndexError("front from empty queue")
        return self._impl.front()

    def back(self):
        if self._impl.empty():
            raise IndexError("back from empty queue")
        return self._impl.back()

    def empty(self):
        return self._impl.empty()

    def size(self):
        return self._impl.size()