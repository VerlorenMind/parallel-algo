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

void usage() {
  std::cout << "Usage: " << std::endl;
  std::cout << "bfs graph_file launches [dist_file]" << std::endl;
  std::cout << "graph_file - file containing a graph" << std::endl;
  std::cout << "launches - number of launches of the algorithms" << std::endl;
  std::cout << "dist_file - file to output distances to (optional)" << std::endl;
}

int main(int argc, const char **argv) {
  if(argc != 3 && argc != 4) { usage(); return 1; }
  std::string filename(argv[1]);
  unsigned launches = atoi(argv[2]);
  std::ofstream dist_out;
  if(argc == 4) {
    std::string dist_file(argv[3]);
    dist_out.open(dist_file);
  }
  std::ifstream in(filename);
  unsigned graph_size = 0;
  std::cout << "Loading graph..." << std::endl;
  auto start = std::chrono::steady_clock::now();
  Node* graph = load_graph(in, graph_size);
  auto stop = std::chrono::steady_clock::now();
  std::cout << "Graph loaded in "
            << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
            << "ms" << std::endl;
  in.close();
  unsigned seq_avg = 0, par_avg = 0;
  BFSSequential seq(graph_size);
  BFSParallel par(graph_size);
  double avg_speed = 0;
  for(unsigned i = 0; i < launches; ++i) {
    std::cout << "Launch #" << i << std::endl;
    unsigned seq_dur = run_bfs(seq, graph);
    seq_avg += seq_dur;
    std::cout << "\tGraph covered by seq: " << seq.verify_visited() << std::endl;
    std::cout << "\tDistances are good: " << seq.verify_distance() << std::endl;
    std::cout << "\tSeq time: " << seq_dur << "ms" << std::endl;
    unsigned par_dur = run_bfs(par, graph);
    par_avg += par_dur;
    std::cout << "\tGraph covered by par: " << par.verify_visited() << std::endl;
    std::cout << "\tDistances are good: " << par.verify_distance() << std::endl;
    std::cout << "\tPar time: " << par_dur << "ms" << std::endl;
    double speed = double(seq_dur) / par_dur;
    avg_speed += speed;
    std::cout << "\tSpeed: " << speed << "x" << std::endl;
  }
  if(dist_out) {
    seq.log_distances(dist_out);
    dist_out << std::endl;
    par.log_distances(dist_out);
  }
  seq_avg /= launches;
  par_avg /= launches;
  avg_speed /= launches;
  std::cout << "Avg. Seq time: " << seq_avg << "ms" << std::endl;
  std::cout << "Avg. Par time: " << par_avg << "ms" << std::endl;
  std::cout << "Speed of avg. time: " << double(seq_avg) / par_avg << "x" << std::endl;
  std::cout << "Avg. speed: " << avg_speed << "x" << std::endl;
  free_graph(graph, graph_size);
}

