from stack_queue import Stack, Queue
from s21_graph import S21_Graph

class GraphAlgorithms:
    def depth_first_search(self, graph: S21_Graph, start_vertex: int) -> list[int]:
        dfs_stack = Stack()
        visited = {start_vertex}
        result = []

        dfs_stack.push(start_vertex)

        while not dfs_stack.empty():

            current_vertex = dfs_stack.pop()
            result.append(current_vertex)

            neighbors = graph.get_neighbors(current_vertex)

            for neighbor, _ in neighbors:
                if neighbor not in visited:
                    dfs_stack.push(neighbor)
                    visited.add(neighbor)

        return result


    def breadth_first_search(self, graph: S21_Graph, start_vertex) -> list[int]:
        queue = Queue()
        visited = {start_vertex}
        result = []

        queue.push(start_vertex)

        while not queue.empty():

            current_vertex = queue.pop()
            result.append(current_vertex)

            neighbors = graph.get_neighbors(current_vertex)

            for neighbor, _ in neighbors:
                if neighbor not in visited:
                    queue.push(neighbor)
                    visited.add(neighbor)

        return result


graph = S21_Graph()
graph.load_graph_from_file('datasets/test_1.txt')

res = GraphAlgorithms()
dfs = res.depth_first_search(graph, 1)
bfs = res.breadth_first_search(graph, 1)
print(dfs)
print(bfs)