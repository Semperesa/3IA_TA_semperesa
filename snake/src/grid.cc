
#include "grid.h"

Grid::Grid(unsigned int w, unsigned int h) {
  w_ = w;
  h_ = h;
  tiles_ = new Tile[w_ * h_];
  for (int x = 0; x < w_; x++) {
    for (int y = 0; y < h_; y++) {
      tiles_[x + y * w_].state = kEmpty;
    }
  }
}

Grid* Grid::CreatedGrid(unsigned int w, unsigned int h) {
  return new Grid(w, h);
}

Tile Grid::getTile(unsigned int x, unsigned int y) {
  Tile t;
  t.state = kInvalid;
  if (x > w_) {
    return t;
  }
  if (y > h_) {
    return t;
  }
  return tiles_[x + y * w_]);
}

Error Grid::freeTile(unsigned int x, unsigned int y) {
  if (x > w_) {
    return kTileOutOfBounds;
  }
  if (y > h_) {
    return kTileOutOfBounds;
  }
  if (tiles_[x + y * w_].state == kEmpty) {
    return kTileAlreadyFreed;
  }
  tiles_[x + y * w_].state = kEmpty;
  return kOK;
}

Error Grid::setTile(unsigned int x, unsigned int y, Tile tile) {
  if (x > w_) {
    return kTileOutOfBounds;
  }
  if (y > h_) {
    return kTileOutOfBounds;
  }
  if (tiles_[x + y * w_].state == kOcupated) {
    return kTileAlreadyOcupated;
  }
  tiles_[x + y * w_].state = tile.state;
  return kOK;
}

void Grid::getTarget(unsigned int& x, unsigned int& y) {
  x = target_x_;
  y = target_y_;
}