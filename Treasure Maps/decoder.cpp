#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {

   mapImg = tm;
   start = s;
   vector<vector<bool>> v(mapImg.width(), vector<bool>(mapImg.height(), false));
   shortPath = vector<vector<int>>(mapImg.width(), vector<int>(mapImg.height(), -1));
   firstPath = vector<vector<pair<int,int>>>(mapImg.width(), vector<pair<int,int>>(mapImg.height(), make_pair(-1,-1)));
   
   v[start.first][start.second] = true;
   shortPath[start.first][start.second] = 0;

   Queue<pair<int,int>> q;
   q.enqueue(start);

   while (!q.isEmpty()) {
      pair<int,int> curr = q.dequeue();
      vector<pair<int,int>> vecTemp = neighbors(curr);

      for (int i = 0; i < vecTemp.size(); i++) {
         pair<int,int> p = vecTemp[i];
         if (good(v, shortPath, curr, p)) {
               v[p.first][p.second] = true;
               shortPath[p.first][p.second] = shortPath[curr.first][curr.second] + 1;
               firstPath[p.first][p.second] = curr;
               q.enqueue(p);
         }
      }
   }
   int longPath = -1;
   pair<int,int> treasureLocation;

   treasureLocation.first = -1;
   treasureLocation.second = -1;
   for (int i = 0; i < mapImg.width(); i++) {
      for (int j = 0; j < mapImg.height(); j++) {
         if (shortPath[i][j] >= longPath) {
               longPath = shortPath[i][j];
               treasureLocation.first = i;
               treasureLocation.second = j;
         }
      }
   }    

   pathPts = vector<pair<int,int>>(longPath + 1, make_pair(0,0));
   pathPts[longPath] = treasureLocation;

   for (int i = longPath - 1; i >= 0; i--) {
      pair<int,int> curr = pathPts[i+1];
      pathPts[i] = firstPath[curr.first][curr.second];
   }    
}

PNG decoder::renderSolution(){

   PNG pngMap = mapImg;

   for (int i = 0; i < pathLength(); i++) {
      int x = pathPts[i].first;
      int y = pathPts[i].second;

      RGBAPixel *pixel = pngMap.getPixel(x, y);

      pixel->r = 255;
      pixel->g = 0;
      pixel->b = 0;
   }

   return pngMap;
}



PNG decoder::renderMaze() {

   PNG pngMap = mapImg;

   for (int i = 0; i < pngMap.width(); i++) {
      for (int j = 0; j < pngMap.height(); j++) {
         if (firstPath[i][j].first >= 0) {
               setGrey(pngMap, make_pair(i, j));
         }
      }
   }  
   for (int i = start.first - 3; i <= start.first + 3; i++) {
      for (int j = start.second - 3; j <= start.second + 3; j++) {
         if (i >= 0 && i < pngMap.width() && j >= 0 && j <= pngMap.height()) {
               RGBAPixel *pixel = pngMap.getPixel(i, j);
                  pixel->r = 255;
                  pixel->g = 0;
                  pixel->b = 0;
         }
      }
   }
   return pngMap;

}


void decoder::setGrey(PNG & im, pair<int,int> loc){

   RGBAPixel *pixel = im.getPixel(loc.first, loc.second);

   pixel->r = 2 * (pixel->r / 4);
   pixel->g = 2 * (pixel->g / 4);
   pixel->b = 2 * (pixel->b / 4);
}

pair<int,int> decoder::findSpot(){

   return pathPts[pathPts.size() - 1];

}

int decoder::pathLength(){

   return pathPts.size();  
}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){

   if ((next.first >= 0 && next.first < mapImg.width() && next.second < mapImg.height() && next.second >= 0) ==  false) {
      return false;
   } else if (v[next.first][next.second] == true) {
      return false;
   } else if (compare(*(mapImg.getPixel(next.first, next.second)), d[curr.first][curr.second]) == false) {
      return false;
   }
   return true;

}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {

vector<pair<int,int>> vecTemp; 

   pair<int,int> l;
   l.first = curr.first - 1;
   l.second = curr.second;
   vecTemp.push_back(l);
   
   pair<int,int> b;
   b.first = curr.first;
   b.second = curr.second + 1;
   vecTemp.push_back(b);

   pair<int,int> r;
   r.first = curr.first + 1;
   r.second = curr.second;
   vecTemp.push_back(r);

   pair<int,int> a;
   a.first = curr.first;
   a.second = curr.second - 1;
   vecTemp.push_back(a);
   
   return vecTemp;

}


bool decoder::compare(RGBAPixel p, int d){

   return ((p.r % 4) * 16 + (p.g % 4) * 4 + (p.b % 4) == (d+1) % 64);
}
