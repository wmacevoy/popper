#include <vector>
#include <algorithm>

#include "popper.h"

bool State::normalized() const
{
  if (counts.size() > 0 && counts[counts.size()-1] == 0) {
    return false;
  }
  for (size_t i=1; i<counts.size(); ++i) {
    if (counts[i-1] < counts[i]) {
	return false;
    }
  }
  return true;
}

void State::normalize()
{
  std::sort(counts.begin(),counts.end(), std::greater<int>());
  while (counts.size() > 0 && counts[counts.size()-1] == 0) {
    counts.pop_back();
  }
}

int State::cmp(const State &to) const
{
  if (counts.size() != to.counts.size()) {
    return counts.size() < to.counts.size() ? -1 : 1;
  }
  for (size_t i=0; i<counts.size(); ++i) {
    if (counts[i] != to.counts[i]) {
	return counts[i] < to.counts[i] ? -1 : 1;
    }
  }
  return 0;
}

States moves(const State &state) {
  States ans;
  if (!state.normalized()) {
    throw std::string("unormalized state");
  }
  for (size_t i=0; i<state.counts.size(); ++i) {
    for (int j=0; j<state.counts[i]; ++j) {
      State changed(state);
      changed.counts[i]=j;
      changed.normalize();
      ans.insert(changed);
    }
  }
  //  std::cout << "moves(" << state << ")=" << ans << std::endl;
  return ans;
}

std::ostream& operator<<(std::ostream &out, const State &state)
{
  bool first = true;
  out << "[";
  for (auto x : state.counts) {
    if (first) {
      first = false;
    } else {
      out << ",";
    }
    out << x;
  }
  out << "]";  
  return out;
}

std::ostream& operator<<(std::ostream &out, const States &states)
{
  bool first = true;
  out << "{";
  for (auto x : states) {
    if (first) {
      first = false;
    } else {
	out << ",";
    }
    out << x;
  }
  out << "}";
  return out;
}


typedef std::pair < States, int> Option;
typedef std::map < State, Option > Cache;


Option Game::evaluate(const State &state, bool myturn)
{
  if (myturn) {
    Cache::iterator i = mybest.find(state);
    if (i != mybest.end()) {
      return i->second;
    }
  } else {
    Cache::iterator i = otherbest.find(state);
    if (i != otherbest.end()) {
      return i->second;
    }
  }

  States options=moves(state);
  Option best;
  bool first = true;
    
  if (state.counts.size() == 0) {
    best.first.clear();
    best.second = myturn ? 1000 : -1000;
  } else {
    if (myturn) {
      for (auto move : options) {
	Option next=evaluate(move, !myturn);
	if (first || best.second <= next.second-1) {
	  if (!first && best.second < next.second-1) {
	    best.first.clear();
	  }
	  best.first.insert(move);
	  best.second=next.second-1;
	}
	first = false;
      }
    } else {
      for (auto move : options) {
	Option next=evaluate(move, !myturn);
	if (first || best.second >= next.second+1) {
	  if (!first && best.second > next.second+1) {
	    best.first.clear();
	  }
	  best.first.insert(move);
	  best.second=next.second+1;
	}
	first = false;	
      }
    }
  }
  
  if (myturn) {
    mybest[state]=best;
  } else {
    otherbest[state]=best;
  }

  return best;
}

States allrc(int rows, int cols) {
  States ans;
  if (rows == 1) {
    for (int i=1; i<=cols; ++i) {
      State state;
      state.counts.push_back(i);
      ans.insert(state);
    }
    return ans;
  } else {
    States prevs(allrc(rows-1,cols));
    ans.insert(prevs.begin(),prevs.end());
    for (auto prev : prevs) {
      State next(prev);
      for (int i=1; i<=cols; ++i) {
	State normalized(next);
	normalized.counts.push_back(i);
	normalized.normalize();
	ans.insert(normalized);
      }
    }
    return ans;
  }
}

States Game::all() { return allrc(rows,cols); }

State Game::start() {
  State state;
  for (int i=0; i<rows; ++i) {
    state.counts.push_back(cols);
  }
  return state;
}




