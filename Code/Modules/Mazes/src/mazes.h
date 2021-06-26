// All mazes are stored here in byte array form.
// 0 represents a passable edge
// 1 represents a wall
// 2 represents a position
// 3 represents an unused pixel
// 4 represents a green circle position

const byte mazes[9][11][11] = {
  
  {// MAZE 1 //
    {2, 0, 2, 0, 2, 1, 2, 0, 2, 0, 2},
    {0, 3, 1, 3, 0, 3, 0, 3, 1, 3, 1},
    {4, 1, 2, 0, 2, 1, 2, 0, 2, 0, 2},
    {0, 3, 0, 3, 1, 3, 1, 3, 1, 3, 0},
    {2, 1, 2, 0, 2, 1, 2, 0, 2, 0, 4},
    {0, 3, 1, 3, 0, 3, 0, 3, 1, 3, 0},
    {2, 1, 2, 0, 2, 0, 2, 1, 2, 0, 2},
    {0, 3, 1, 3, 1, 3, 1, 3, 1, 3, 0},
    {2, 0, 2, 0, 2, 1, 2, 0, 2, 1, 2},
    {0, 3, 1, 3, 0, 3, 0, 3, 1, 3, 0},
    {2, 0, 2, 1, 2, 0, 2, 1, 2, 0, 2}
  },

  {// MAZE 2 //
    {2, 0, 2, 0, 2, 1, 2, 0, 2, 0, 2},
    {1, 3, 0, 3, 1, 3, 0, 3, 0, 3, 1},
    {2, 0, 2, 1, 2, 0, 2, 1, 4, 0, 2},
    {0, 3, 1, 3, 0, 3, 1, 3, 1, 3, 0},
    {2, 1, 2, 0, 2, 1, 2, 0, 2, 0, 2},
    {0, 3, 0, 3, 1, 3, 0, 3, 1, 3, 0},
    {2, 0, 4, 1, 2, 0, 2, 1, 2, 1, 2},
    {0, 3, 1, 3, 0, 3, 1, 3, 0, 3, 0},
    {2, 1, 2, 1, 2, 1, 2, 0, 2, 1, 2},
    {0, 3, 0, 3, 0, 3, 0, 3, 1, 3, 0},
    {2, 1, 2, 0, 2, 1, 2, 0, 2, 0, 2}
  },

  {// MAZE 3 //
    {2, 0, 2, 0, 2, 1, 2, 1, 2, 0, 2},
    {0, 3, 1, 3, 0, 3, 0, 3, 0, 3, 0},
    {2, 1, 2, 1, 2, 1, 2, 0, 2, 1, 2},
    {1, 3, 0, 3, 0, 3, 1, 3, 1, 3, 0},
    {2, 0, 2, 1, 2, 1, 2, 0, 2, 1, 2},
    {0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0},
    {2, 1, 2, 1, 2, 1, 4, 1, 2, 1, 4},
    {0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0},
    {2, 1, 2, 0, 2, 1, 2, 1, 2, 1, 2},
    {0, 3, 1, 3, 1, 3, 0, 3, 0, 3, 0},
    {2, 0, 2, 0, 2, 0, 2, 1, 2, 0, 2}
  },

  {// MAZE 4 //
    {4, 0, 2, 1, 2, 0, 2, 0, 2, 0, 2},
    {0, 3, 0, 3, 1, 3, 1, 3, 1, 3, 0},
    {2, 1, 2, 1, 2, 0, 2, 0, 2, 0, 2},
    {0, 3, 0, 3, 0, 3, 1, 3, 1, 3, 0},
    {2, 1, 2, 0, 2, 1, 2, 0, 2, 1, 2},
    {0, 3, 1, 3, 1, 3, 0, 3, 1, 3, 0},
    {4, 1, 2, 0, 2, 0, 2, 0, 2, 0, 2},
    {0, 3, 1, 3, 1, 3, 1, 3, 1, 3, 0},
    {2, 0, 2, 0, 2, 0, 2, 0, 2, 1, 2},
    {0, 3, 1, 3, 1, 3, 1, 3, 0, 3, 0},
    {2, 0, 2, 0, 2, 1, 2, 0, 2, 1, 2}
  },

  {// MAZE 5 //
    {2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2},
    {1, 3, 1, 3, 1, 3, 1, 3, 0, 3, 0},
    {2, 0, 2, 0, 2, 0, 2, 0, 2, 1, 2},
    {0, 3, 1, 3, 1, 3, 0, 3, 1, 3, 1},
    {2, 0, 2, 1, 2, 0, 2, 1, 4, 0, 2},
    {0, 3, 0, 3, 1, 3, 1, 3, 0, 3, 0},
    {2, 1, 2, 0, 2, 0, 2, 1, 2, 1, 2},
    {0, 3, 1, 3, 1, 3, 0, 3, 1, 3, 0},
    {2, 1, 2, 0, 2, 0, 2, 0, 2, 1, 2},
    {0, 3, 0, 3, 1, 3, 1, 3, 1, 3, 0},
    {2, 1, 2, 0, 2, 0, 4, 0, 2, 0, 2}
  },

  {// MAZE 6 //
    {2, 1, 2, 0, 2, 1, 2, 0, 4, 0, 2},
    {0, 3, 0, 3, 0, 3, 1, 3, 0, 3, 0},
    {2, 1, 2, 1, 2, 1, 2, 0, 2, 1, 2},
    {0, 3, 0, 3, 0, 3, 0, 3, 1, 3, 0},
    {2, 0, 2, 1, 2, 1, 2, 1, 2, 0, 2},
    {0, 3, 1, 3, 1, 3, 0, 3, 0, 3, 1},
    {2, 0, 2, 1, 2, 0, 2, 1, 2, 1, 2},
    {1, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0},
    {2, 0, 2, 1, 4, 1, 2, 1, 2, 0, 2},
    {0, 3, 1, 3, 1, 3, 0, 3, 1, 3, 0},
    {2, 0, 2, 0, 2, 0, 2, 1, 2, 0, 2}
  },

  {// MAZE 7 //
    {2, 0, 4, 0, 2, 0, 2, 1, 2, 0, 2},
    {0, 3, 1, 3, 1, 3, 0, 3, 0, 3, 0},
    {2, 1, 2, 0, 2, 1, 2, 0, 2, 1, 2},
    {0, 3, 0, 3, 1, 3, 1, 3, 1, 3, 0},
    {2, 0, 2, 1, 2, 0, 2, 1, 2, 0, 2},
    {1, 3, 1, 3, 0, 3, 1, 3, 0, 3, 1},
    {2, 0, 2, 1, 2, 0, 2, 0, 2, 1, 2},
    {0, 3, 0, 3, 0, 3, 1, 3, 1, 3, 0},
    {2, 1, 2, 1, 2, 0, 2, 0, 2, 1, 2},
    {0, 3, 1, 3, 1, 3, 1, 3, 0, 3, 0},
    {2, 0, 4, 0, 2, 0, 2, 0, 2, 0, 2}
  },

  {// MAZE 8 //
    {2, 1, 2, 0, 2, 0, 4, 1, 2, 0, 2},
    {0, 3, 0, 3, 1, 3, 0, 3, 0, 3, 0},
    {2, 0, 2, 0, 2, 1, 2, 0, 2, 1, 2},
    {0, 3, 1, 3, 1, 3, 1, 3, 1, 3, 0},
    {2, 1, 2, 0, 2, 0, 2, 0, 2, 1, 2},
    {0, 3, 0, 3, 1, 3, 1, 3, 0, 3, 0},
    {2, 1, 2, 0, 4, 1, 2, 0, 2, 0, 2},
    {0, 3, 1, 3, 0, 3, 1, 3, 1, 3, 1},
    {2, 1, 2, 1, 2, 0, 2, 0, 2, 0, 2},
    {0, 3, 0, 3, 1, 3, 1, 3, 1, 3, 1},
    {2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2}
  },

  {// MAZE 9 //
    {2, 1, 2, 0, 2, 0, 2, 0, 2, 0, 2},
    {0, 3, 0, 3, 1, 3, 1, 3, 0, 3, 0},
    {2, 1, 2, 1, 4, 0, 2, 1, 2, 1, 2},
    {0, 3, 0, 3, 0, 3, 1, 3, 0, 3, 0},
    {2, 0, 2, 0, 2, 1, 2, 0, 2, 1, 2},
    {0, 3, 1, 3, 1, 3, 0, 3, 1, 3, 0},
    {2, 1, 2, 1, 2, 0, 2, 1, 2, 0, 2},
    {0, 3, 0, 3, 0, 3, 1, 3, 1, 3, 0},
    {4, 1, 2, 1, 2, 1, 2, 0, 2, 1, 2},
    {0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 1},
    {2, 0, 2, 1, 2, 0, 2, 1, 2, 0, 2}
  }
};