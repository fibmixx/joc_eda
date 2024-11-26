#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Sohyun


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
  const vector<Dir> dirs_mag = {Up,Down,Left,Right};
  const vector<Dir> dirs_fant = {Up,Down,Left,Right,RU,UL,DR,LD};

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
        vector<int> ws = wizards(me());
        int g = ghost(me());
        if (round()<150) { //early game
            for (int id : ws) {
                Dir d = Right;
                Pos new_pos = unit(id).pos + d;
                if (pos_ok(new_pos) and cell(new_pos.i,new_pos.j).type != Wall) move(id,d);
                else move(id,Up);
            }
            
        }
        else { // late game
            return;
        }
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
