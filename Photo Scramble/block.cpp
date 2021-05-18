#include "block.h"
#include <cmath>
#include <iostream>

int Block::height() const {

  int height = data[0].size();  

  return height;
}

int Block::width() const {

    int width = data.size();

    return width;
}

 /**
   * Renders the given block onto img with its upper
   * left corner at (x, 0). Assumes block fits on the 
   * image.
   */
void Block::render(PNG &im, int x) const {

    for( int xCoord= x ; xCoord < x + width() ; xCoord++ ){

    for( int yCoord=0 ; yCoord < height() ; yCoord++ ){

      *im.getPixel(xCoord,yCoord) = data[xCoord-x][yCoord];     

    }

  }

}

 /** 
   * Creates a block that is width X img.height pixels in size
   * by copying the pixels from (x, 0) to (x+width-1, img.height-1)
   * in img. Assumes img is large enough to supply these pixels.
   */
void Block::build(PNG &im, int x, int width) {
  
  for(int xCoord= x ; xCoord < x + width ; xCoord++ ){

    vector<HSLAPixel> column;

    for(unsigned int yCoord=0 ; yCoord < im.height() ; yCoord++ ){

      column.push_back(*im.getPixel(xCoord,yCoord));      

    }

    data.push_back(column);

  }

}
