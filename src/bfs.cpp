#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>
#include <cmath>
#include "parallel_utils.h"
#include "bfs.h"

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

void free_graph(Node *graph, unsigned size) {
  for(unsigned i=0; i < size; ++i) {
    delete[] graph[i].neighbors;
  }
  delete[] graph;
}

BFS::BFS(unsigned graph_size) : graph_size(graph_size) {
  visited = new bool[graph_size];
  distance = new unsigned[graph_size];
  reset();
}
BFS::~BFS() {
  delete[] visited;
  delete[] distance;
}
bool BFS::verify_visited() {
  for(unsigned i = 0; i < graph_size; ++i) {
    if(!visited[i]) {
      return false;
    }
  }
  return true;
}

void BFS::reset() {
  memset(visited, 0, graph_size * sizeof(bool));
  memset(distance, 0, graph_size*sizeof(unsigned));
}

void BFS::log_distances(std::ostream &out) {
  for(unsigned i=0; i<graph_size; ++i) {
    out << distance[i] << std::endl;
  }
}

bool BFS::verify_distance() {
  unsigned side = std::cbrt(graph_size);
  for(unsigned i=0; i < graph_size; ++i) {
    unsigned decomp = i;
    unsigned true_dist = decomp / (side * side);
    decomp %= (side * side);
    true_dist += decomp / side;
    decomp %= side;
    true_dist += decomp;
    if(true_dist != distance[i]) {
      return false;
    }
  }
  return true;
}

BFSSequential::BFSSequential(unsigned graph_size): BFS(graph_size) {

}

void BFSSequential::search(const Node* nodes) {
  q.push(0);
  visited[0] = true;
  distance[0] = 0;
  while(!q.empty()) {
    unsigned cur = q.front();
    q.pop();
    for(unsigned i=0; i < nodes[cur].deg; ++i) {
      unsigned neigh = nodes[cur].neighbors[i];
      if(!visited[neigh]) {
        q.push(nodes[cur].neighbors[i]);
        distance[neigh] = distance[cur] + 1;
        visited[neigh] = true;
      }
    }
  }
}

void BFSParallel::search(const Node* nodes) {
  std::function<unsigned(int)> get_deg = [nodes](int x) -> unsigned { return nodes[x].deg; };
  std::function<unsigned(int)> filter_missed = [nodes](int x) -> unsigned { return x >= 0 ? 1 : 0; };
  front[0] = 0;
  visited[0] = true;
  distance[0] = 0;
  unsigned front_size = 1;
  while (front_size > 0) {
    map(front, degs, 0, front_size, get_deg);
    unsigned size = 0;
    size = scan_simd(degs, degs_out, 0, front_size);
#pragma omp parallel for
    for(unsigned i = 0; i < front_size; ++i) {
//#pragma omp parallel for
      for(unsigned j = 0; j < nodes[front[i]].deg; ++j) {
        unsigned neigh = nodes[front[i]].neighbors[j];
        bool taken;
#pragma omp atomic capture
        {
          taken = visited[neigh];
          visited[neigh] = true;
        }
        if (!taken) {
          front_buf[degs_out[i] + j] = neigh;
          distance[neigh] = distance[front[i]] + 1;
        }
        else {
          front_buf[degs_out[i] + j] = -1;
        }
      }
    }
    front_size = filter_par(front_buf, front, buf1, buf2, 0, size, filter_missed);
  }
}

BFSParallel::BFSParallel(unsigned graph_size): BFS(graph_size) {
  front = new int[graph_size];
  front_buf = new int[graph_size];
  buf1 = new unsigned[graph_size];
  buf2 = new unsigned[graph_size];
  degs = new unsigned[graph_size];
  degs_out = new unsigned[graph_size];
}

BFSParallel::~BFSParallel() {
  delete[] front;
  delete[] front_buf;
  delete[] buf1;
  delete[] buf2;
  delete[] degs;
  delete[] degs_out;

}
