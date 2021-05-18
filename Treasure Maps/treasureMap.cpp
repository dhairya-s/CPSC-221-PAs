#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{

    base = baseim;
    maze = mazeim;
    start = s;

}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){

    RGBAPixel *pixel = im.getPixel(loc.first,loc.second);

    pixel->r=2*((pixel->r)/4);
    pixel->g=2*((pixel->g)/4);
    pixel->b=2*((pixel->b)/4);

}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){

    RGBAPixel *pixel = im.getPixel(loc.first,loc.second);
    d = d%64;

    int r = d >> 4;
    r = r & 0b00000011;
    pixel->r = pixel->r & 0b11111100;
    pixel->r = pixel->r + r;

    int g = d >> 2;
    g = g & 0b00000011;
    pixel->g = pixel->g & 0b11111100;
    pixel->g = pixel->g + g;

    int b = d;
    b = b & 0b00000011;
    pixel->b = pixel->b & 0b11111100;
    pixel->b = pixel->b + b;

}

PNG treasureMap::renderMap(){

    PNG pngBase = base;
    vector<vector<bool>> v (pngBase.height() ,vector<bool> (pngBase.width(), 0)); 
    vector<vector<int>> d (pngBase.height() ,vector<int> (pngBase.width(), 0));

    v[start.second][start.first] = true;
    d[start.second][start.first] = 0;
    
    Queue<pair<int,int>> q;

    setLOB(pngBase,start,0); 

    q.enqueue(start);
     
    while(!q.isEmpty()) {
        pair<int,int> curr;
        curr = q.dequeue();
        vector<pair<int,int>> vecTemp = neighbors(curr);

        for (int i = 0; i < 4; i++) {
            if (good(v,curr,vecTemp[i])) {

                v[vecTemp[i].second][vecTemp[i].first] = true;
                d[vecTemp[i].second][vecTemp[i].first] = d[curr.second][curr.first] + 1;
                setLOB(pngBase,vecTemp[i],d[vecTemp[i].second][vecTemp[i].first]);
                q.enqueue(vecTemp[i]);
            }
        }
    }
    return pngBase;

}


PNG treasureMap::renderMaze(){

PNG pngBase = base;
RGBAPixel *pixel1 = maze.getPixel(start.first,start.second);
vector<vector<bool>> v(pngBase.height() ,vector<bool> (pngBase.width(), 0));
v[start.second][start.first] = true;

Queue<pair<int,int>> q;
q.enqueue(start); 

while(!q.isEmpty()) {
    pair<int,int> curr;
    curr = q.dequeue();
    vector<pair<int,int>> vecTemp = neighbors(curr);
    for(int i = 0; i < 4; i++) {
        if (good(v,curr,vecTemp[i])) {
            v[vecTemp[i].second][vecTemp[i].first] = true;
            RGBAPixel *pixel2 = maze.getPixel(vecTemp[i].first,vecTemp[i].second);
            
            if (*pixel2 == *pixel1) {
                setGrey(pngBase,vecTemp[i]);
            }
            q.enqueue(vecTemp[i]);
        }
    }
}
    for (int i = start.second-3; i <= start.second + 3 ; i++) {
        for (int j = start.first-3; j<= start.first + 3; j++) {
            if (j < pngBase.width() && i < pngBase.height() &&  j >= 0 &&  i >= 0) {
            RGBAPixel *pixel = pngBase.getPixel(j,i);
            pixel->r=255;
            pixel->g=0;
            pixel->b=0;
            }
        }
    }
 return pngBase;
}


bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){

    if (next.first < base.width() && next.second < base.height() &&  next.first >= 0 &&  next.second >= 0) {
        if (v[next.second][next.first] == false) {
            RGBAPixel *pixel1 = maze.getPixel(curr.first,curr.second);
            RGBAPixel *pixel2 = maze.getPixel(next.first,next.second);
            
            if(*pixel1==*pixel2) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }

}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {

    vector<pair<int,int>> vecTemp;
    vecTemp.push_back(make_pair(curr.first-1,curr.second));
    vecTemp.push_back(make_pair(curr.first,curr.second-1));
    vecTemp.push_back(make_pair(curr.first+1,curr.second));
    vecTemp.push_back(make_pair(curr.first,curr.second+1));
    return vecTemp;

}

