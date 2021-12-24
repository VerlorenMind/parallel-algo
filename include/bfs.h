#ifndef QUICKSORT_INCLUDE_BFS_H_
#define QUICKSORT_INCLUDE_BFS_H_

#include <deque>
#include <queue>
struct Node {
  unsigned deg;
  unsigned *neighbors;
};

Node* load_graph(std::ifstream &input_file, unsigned &graph_size);
void free_graph(Node *graph, unsigned size);

class BFS {
 protected:
  bool* visited;
  unsigned* distance;
  unsigned graph_size;
 public:
  explicit BFS(unsigned graph_size);
  ~BFS();
  virtual void search(const Node *graph) = 0;
  bool verify_visited();
  bool verify_distance();
  void reset();
  void log_distances(std::ostream &out);
};
class BFSSequential : public BFS {
 private:
  std::queue<unsigned> q;
 public:
  explicit BFSSequential(unsigned graph_size);
  void search(const Node *graph) override;
};

class BFSParallel : public BFS {
 private:
  int *front;
  int* front_buf;
  unsigned *buf1;
  unsigned *buf2;
  unsigned *degs;
  unsigned *degs_out;
 public:
  explicit BFSParallel(unsigned graph_size);
  void search(const Node *graph) override;
  ~BFSParallel();
};
#endif //QUICKSORT_INCLUDE_BFS_H_
