

class S21_Graph:
    def __init__(self):
        self._adjacency_matrix = []

    def load_graph_from_file(self, filename):

        with open(filename, 'r', encoding="utf-8") as file:
            lines = [line.strip() for line in file if line.strip()]

        for line in lines:
            line = list(map(int, line.split()))
            self._adjacency_matrix.append(line)

    def get_count_vertex(self):
        return len(self._adjacency_matrix)

    def get_neighbors(self, vertex: int) -> list:
        size_matrix = self.get_count_vertex()

        if size_matrix == 0:
            return None
        
        cur_vertex = vertex - 1
        neighbors = []

        for k in range(size_matrix):
            if self._adjacency_matrix[cur_vertex][k]:
                neighbor = (k+1, self._adjacency_matrix[cur_vertex][k])
                neighbors.append(neighbor)
        return neighbors

    def get_edge_weigh(self, v1: int, v2: int):
        return self._adjacency_matrix[v1 - 1][v2 - 1]

    def get_adjacency_matrix(self):
        matrix = [[self._adjacency_matrix[i][j] for j in range(len(self._adjacency_matrix))] for i in range(len(self._adjacency_matrix))]
        return matrix


# graph = S21_Graph()
# graph.load_graph_from_file('datasets/test_1.txt')
# matrix = graph.get_adjacency_matrix()
# neighbors = graph.get_neighbors(2)
# print(graph.get_edge_weigh(3, 4))
# print(graph.get_adjacency_matrix())