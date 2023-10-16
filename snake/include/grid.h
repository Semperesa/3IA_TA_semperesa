
#ifndef __GRID_H__
#define __GRID_H__ 1

enum TileState {
  kInvalid,
  kEmpty,
  kTarget,
  kOcupated,
};

struct Tile {
  TileState state;
};

enum Error {
  kOK = 0,
  kTileAlreadyFreed,
  kTileAlreadyOcupated,
  kTileOutOfBounds,
  // ...
};

class Grid {
  
public:

  ~Grid();

  static Grid* CreateGrid(unsigned int w, unsigned int h);

  Tile getTile(unsigned int x, unsigned int y);
  Error freeTile(unsigned int x, unsigned int y);
  Error setTile(unsigned int x, unsigned int y, Tile tile);

  void getTarget(unsigned int& x, unsigned int& y);

private:
  Grid(unsigned int w, unsigned int h);

  unsigned int w_;
  unsigned int h_;
  unsigned int target_x_;
  unsigned int target_y_;
  Tile* tiles_;

};

#endif // __GRID_H__