#include <iostream>
#include <vector>
#include <set>
#include <map>

struct State {
  std::vector<int> counts;
  bool normalized() const;
  void normalize();
  int cmp(const State &to) const;

  inline bool operator<(const State &to) const { return cmp(to) < 0; }
  inline bool operator>(const State &to) const { return cmp(to) > 0; }
  inline bool operator<=(const State &to) const { return cmp(to) <= 0; }
  inline bool operator>=(const State &to) const { return cmp(to) >= 0; }
  inline bool operator==(const State &to) const { return cmp(to) == 0; }
  inline bool operator!=(const State &to) const { return cmp(to) != 0; }
};

std::ostream& operator<<(std::ostream &out, const State &state);

typedef std::set < State > States;

std::ostream& operator<<(std::ostream &out, const States &states);

States moves(const State &state);

typedef std::pair < States, int> Option;

typedef std::map < State, Option > Cache;

struct Game {
  int rows;
  int cols;
  Cache mybest;
  Cache otherbest;
  State start();
  States all();
  Option evaluate(const State &state, bool myturn);
};
