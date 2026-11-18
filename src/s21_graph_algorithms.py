from stack_queue import Stack, Queue
from s21_graph import S21_Graph
import math

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
    
    def get_shortest_path_between_vertices(self, graph: S21_Graph, vertex1: int, vertex2: int) -> int:
        count_nodes = graph.get_count_vertex()
        all_path = {i: math.inf for i in range(1, count_nodes + 1)}
        visited = set()

        current_vertex = vertex1

        while current_vertex != vertex2:
            neighbors = graph.get_neighbors(current_vertex)
            for neighbor, weigth in neighbors:
                all_path[neighbor] = weigth
                visited.add(neighbor)
            
            min_wight_vertex = min(key for key, value in all_path.items() if key != vertex1)

            current_vertex = min_wight_vertex

        # min_wight = min(key for key, value in all_path.items() if key != vertex1)
        return


graph = S21_Graph()
graph.load_graph_from_file('datasets/test_1.txt')

res = GraphAlgorithms()
dfs = res.depth_first_search(graph, 1)
bfs = res.breadth_first_search(graph, 1)
print(dfs)
print(bfs)
res.get_shortest_path_between_vertices(graph, 1 , 2)