#include "popper.h"

int main(int argc, char *argv[])
{

  Game game;
  game.rows=5;
  game.cols=5;

  States analyzed;
  States extras;
  bool myside = true;
  extras.insert(game.start());
  while (!extras.empty()) {
    States newExtras;
    for (auto extra : extras) {
      Option play = game.evaluate(extra, myside);
      analyzed.insert(extra);
      for (auto newExtra : play.first) {
	if (analyzed.find(newExtra) == analyzed.end()) {
	  newExtras.insert(newExtra);
	}
      }
    }
    extras=newExtras;
  }

  for (auto state : analyzed) {
    Option play = game.evaluate(state, myside);
    if (play.second < 0) {
      std::cout << "(" << state << ") -> " << play.first << std::endl;
    } else {
      std::cout << state << "->" << play.first << std::endl;
    }
  }
  
  return 0;
}
