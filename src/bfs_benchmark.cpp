#include <iostream>
#include <fstream>
#include <chrono>
#include "bfs.h"

unsigned run_bfs(BFS &algo, Node *graph) {
  algo.reset();
  auto start = std::chrono::steady_clock::now();
  algo.search(graph);
  auto stop = std::chrono::steady_clock::now();
  auto seq_dur = stop - start;
  return std::chrono::duration_cast<std::chrono::milliseconds>(seq_dur).count();
}

int main(int argc, const char **argv) {
  std::string filename(argv[1]);
  unsigned launches = atoi(argv[2]);
  std::ifstream in(filename);
  unsigned graph_size = 0;
  Node* graph = load_graph(in, graph_size);
  in.close();
  unsigned seq_avg = 0, par_avg = 0;
  BFSSequential seq(graph_size);
  BFSParallel par(graph_size);
  for(unsigned i = 0; i < launches; ++i) {
    std::cout << "Launch #" << i << std::endl;
    unsigned seq_dur = run_bfs(seq, graph);
    seq_avg += seq_dur;
    std::cout << "\tGraph covered by seq: " << seq.verify() << std::endl;
    std::cout << "\tSeq time: " << seq_dur << "ms" << std::endl;
    unsigned par_dur = run_bfs(par, graph);
    par_avg += par_dur;
    std::cout << "\tGraph covered by par: " << par.verify() << std::endl;
    std::cout << "\tPar time: " << par_dur << "ms" << std::endl;
    std::cout << "\tSpeed: " << double(seq_dur) / par_dur << "x" << std::endl;
  }
  seq_avg /= launches;
  par_avg /= launches;
  std::cout << "Avg Seq time: " << seq_avg << "ms" << std::endl;
  std::cout << "Avg Par time: " << par_avg << "ms" << std::endl;
  std::cout << "Speed: " << double(seq_avg) / par_avg << "x" << std::endl;
  free_graph(graph, graph_size);
}

