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
 public:
  virtual void search(const Node *graph) = 0;
  virtual bool verify() = 0;
  virtual void reset() = 0;
};
class BFSSequential : public BFS {
 private:
  std::queue<unsigned> q;
  std::vector<bool> visited;
 public:
  explicit BFSSequential(unsigned graph_size);
  void search(const Node *graph) override;
  bool verify() override;
  void reset() override;
};

class BFSParallel : public BFS {
 private:
  int *front;
  int* front_buf;
  unsigned *buf1;
  unsigned *buf2;
  unsigned *degs;
  unsigned *degs_out;
  bool *visited;
  unsigned graph_size;
 public:
  explicit BFSParallel(unsigned graph_size);
  void search(const Node *graph) override;
  bool verify() override;
  ~BFSParallel();
  void reset() override;
};
#endif //QUICKSORT_INCLUDE_BFS_H_
