#include <iostream>
#include <fstream>
#include <deque>
#include <cstring>
#include "../include/parallel_utils.h"

struct Node {
  unsigned deg;
  unsigned *neighbors;
};

Node* load_graph(std::ifstream &input_file, unsigned &graph_size) {
  input_file >> graph_size;
  Node *nodes = new Node[graph_size];
  for(unsigned i = 0; i < graph_size; ++i) {
    unsigned deg;
    input_file >> deg;
    nodes[i].deg = deg;
    if(deg > 0) {
      nodes[i].neighbors = new unsigned[deg];
      for(unsigned j=0; j < deg; ++j) {
        input_file >> nodes[i].neighbors[j];
      }
    }
  }
  return nodes;
}

void bfs_seq(Node* nodes, bool* visited) {
  std::deque<unsigned> q;
  q.push_back(0);
  visited[0] = true;
  while(!q.empty()) {
    unsigned cur = q.front();
    q.pop_front();
    for(unsigned i=0; i < nodes[cur].deg; ++i) {
      unsigned neigh = nodes[cur].neighbors[i];
      if(!visited[neigh]) {
        q.push_back(nodes[cur].neighbors[i]);
        visited[neigh] = true;
      }
    }
  }
}

bool verify(unsigned n, bool* a) {
  for(unsigned i = 0; i < n; ++i) {
    if(!a[i]) {
      return false;
    }
  }
  return true;
}

void bfs_par(const Node* nodes, bool* visited, unsigned graph_size) {
  std::function<unsigned(const Node&)> get_deg = [](const Node &x) -> unsigned { return x.deg; };
  Node *front = new Node[graph_size];
  Node *newfront = new Node[graph_size];
  unsigned *degs = new unsigned[graph_size];
  front[0] = nodes[0];
  visited[0] = true;
  unsigned front_size = 1;
  while (front_size > 0) {
#pragma omp parallel
    {
#pragma omp single
      {
        scan(front, degs, 0, front_size, get_deg);
      }
    }
#pragma omp parallel for
  for(unsigned i = 0; i < front_size; ++i) {
      for(unsigned j = 0; j < front[i].deg; ++j) {
        unsigned neigh = front[i].neighbors[j];
        bool val;
#pragma omp atomic capture
        {
          val = visited[neigh];
          visited[neigh] = true;
        }
        if (!val) {
          newfront[degs[i] + j] = nodes[neigh];
        }
      }
    }
  }
}
int main(int argc, const char **argv) {
  std::string filename(argv[1]);
  std::ifstream in(filename);
  unsigned graph_size = 0;
  Node* graph = load_graph(in, graph_size);
  in.close();
  bool *visited = new bool[graph_size];
  memset(visited, 0, graph_size*sizeof(bool));
  bfs_seq(graph, visited);
  std::cout << "Graph covered: " << verify(graph_size, visited);
}
