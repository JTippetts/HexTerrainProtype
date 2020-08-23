#pragma once

// Hex Grid implementation
#include <Urho3D/Scene/Node.h>
using namespace Urho3D;

enum ETileDirections {
  NorthEast = 0,
  East,
  SouthEast,
  SouthWest,
  West,
  NorthWest,
  DirNone,
  NumberOfDirections
};

class HexGrid {
public:
  HexGrid(float hexside);

  void SetHexSide(float hexside);
  Vector2 CalcTileOrigin(int tx, int ty);
  Vector2 CalcTileCenter(int tx, int ty);
  Vector2 CalcTileBottom(int tx, int ty);
  IntVector2 CalcPointTile(float px, float py);
  Vector2 CalcMapArea(int sx, int sy);
  int RotateDirection(int dir, int amount);
  int OppositeDirection(int dir);
  IntVector2 CalcNeighbor(int tx, int ty, int dir);
  int CalcDistance(int sx, int sy, int dx, int dy);

  float GetRowHeight() { return rowheight_; }
  float GetHexWidth() { return hexwidth_; }
  float GetHexSide() { return hexside_; }

private:
  float hexside_, hexheight_, rowheight_, halfwidth_, hexwidth_;
};
