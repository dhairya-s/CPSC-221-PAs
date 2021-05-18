#include "stats.h"


#include "stats.h"

stats::stats(PNG & im){

    int height = im.height();
    int width = im.width();

    sumRed = vector<vector<long>>(width, vector<long>(height));
    sumGreen = vector<vector<long>>(width, vector<long>(height));
    sumBlue = vector<vector<long>>(width, vector<long>(height));
    sumsqRed = vector<vector<long>>(width, vector<long>(height));
    sumsqGreen = vector<vector<long>>(width, vector<long>(height));
    sumsqBlue = vector<vector<long>>(width, vector<long>(height));
    

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            RGBAPixel *pixel = im.getPixel(i,j);
            if (i == 0 && j == 0) {
                sumRed[i][j] = pixel->r;
                sumGreen[i][j] = pixel->g;
                sumBlue[i][j] = pixel->b;
                sumsqRed[i][j] = pixel->r * pixel->r;
                sumsqGreen[i][j] = pixel->g * pixel->g;
                sumsqBlue[i][j] = pixel->b * pixel->b;
            } else if (i == 0) {
                sumRed[i][j] = sumRed[i][j-1] + pixel->r;
                sumGreen[i][j] = sumGreen[i][j-1] + pixel->g;
                sumBlue[i][j] = sumBlue[i][j-1] + pixel->b;
                sumsqRed[i][j] = sumsqRed[i][j-1] + (pixel->r * pixel->r);
                sumsqGreen[i][j] = sumsqGreen[i][j-1] + (pixel->g * pixel->g);
                sumsqBlue[i][j] = sumsqBlue[i][j-1] + (pixel->b * pixel->b);
            } else if (j == 0) {
                sumRed[i][j] = sumRed[i - 1][j] + pixel->r;
                sumGreen[i][j] = sumGreen[i - 1][j] + pixel->g;
                sumBlue[i][j] = sumBlue[i - 1][j] + pixel->b;
                sumsqBlue[i][j] = sumsqBlue[i - 1][j] + (pixel->b * pixel->b);
                sumsqGreen[i][j] = sumsqGreen[i - 1][j] + (pixel->g * pixel->g);
                sumsqRed[i][j] = sumsqRed[i - 1][j] + (pixel->r * pixel->r);
            } else {
                sumRed[i][j] = sumRed[i - 1][j] + sumRed[i][j-1] - sumRed[i - 1][j - 1] + pixel->r;
                sumGreen[i][j] = sumGreen[i - 1][j] + sumGreen[i][j-1] - sumGreen[i - 1][j - 1] + pixel->g;
                sumBlue[i][j] = sumBlue[i - 1][j] + sumBlue[i][j-1] - sumBlue[i - 1][j - 1] + pixel->b;
                sumsqRed[i][j] = sumsqRed[i - 1][j] + sumsqRed[i][j-1] - sumsqRed[i - 1][j - 1] + (pixel->r*pixel->r);
                sumsqGreen[i][j] = sumsqGreen[i - 1][j] + sumsqGreen[i][j-1] - sumsqGreen[i - 1][j - 1] + (pixel->g*pixel->g);
                sumsqBlue[i][j] = sumsqBlue[i - 1][j] + sumsqBlue[i][j-1] - sumsqBlue[i - 1][j - 1] + (pixel->b*pixel->b);
            }
        }
    }

}


long stats::getSum(char channel, pair<int,int> ul, int w, int h){
    int x = ul.first;
    int y = ul.second;
    long sum = 0;

    if (x == 0 && y == 0) {
        if (channel == 'r') {
            sum = sumRed[x+w-1][y+h-1];
        } else if (channel == 'g') {
            sum = sumGreen[x+w-1][y+h-1];
        } else if (channel == 'b') {
            sum = sumBlue[x+w-1][y+h-1];
        }
    } else if (x == 0) {
        if (channel == 'r') {
            sum = sumRed[x+w-1][y+h-1] - sumRed[x+w-1][y-1];
        } else if (channel == 'g') {
            sum = sumGreen[x+w-1][y+h-1] - sumGreen[x+w-1][y-1];
        } else if (channel == 'b') {
            sum = sumBlue[x+w-1][y+h-1] - sumBlue[x+w-1][y-1];
        }
    } else if (y == 0) {
        if (channel == 'r') {
            sum = sumRed[x+w-1][y+h-1] - sumRed[x-1][y+h-1];
        } else if (channel == 'g') {
            sum = sumGreen[x+w-1][y+h-1] - sumGreen[x-1][y+h-1];
        } else if (channel == 'b') {
            sum = sumBlue[x+w-1][y+h-1] - sumBlue[x-1][y+h-1];
        }
    } else {
        if (channel == 'r') {
            sum = sumRed[x+w-1][y+h-1] - sumRed[x+w-1][y-1] - sumRed[x-1][y+h-1] + sumRed[x-1][y-1];
        } else if (channel == 'g') {
            sum = sumGreen[x+w-1][y+h-1] - sumGreen[x+w-1][y-1] - sumGreen[x-1][y+h-1] + sumGreen[x-1][y-1];
        } else if (channel == 'b') {
            sum = sumBlue[x+w-1][y+h-1] - sumBlue[x+w-1][y-1] - sumBlue[x-1][y+h-1] + sumBlue[x-1][y-1];
        }
    }

    return sum;

}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
    int x = ul.first;
    int y = ul.second;
    long sum = 0;

    if (x == 0 && y == 0) {
        if (channel == 'r') {
            sum = sumsqRed[x+w-1][y+h-1];
        } else if (channel == 'g') {
            sum = sumsqGreen[x+w-1][y+h-1];
        } else if (channel == 'b') {
            sum = sumsqBlue[x+w-1][y+h-1];
        }
    } else if (x == 0) {
        if (channel == 'r') {
            sum = sumsqRed[x+w-1][y+h-1] - sumsqRed[x+w-1][y-1];
        } else if (channel == 'g') {
            sum = sumsqGreen[x+w-1][y+h-1] - sumsqGreen[x+w-1][y-1];
        } else if (channel == 'b') {
            sum = sumsqBlue[x+w-1][y+h-1] - sumsqBlue[x+w-1][y-1];
        }
    } else if (y == 0) {
        if (channel == 'r') {
            sum = sumsqRed[x+w-1][y+h-1] - sumsqRed[x-1][y+h-1];
        } else if (channel == 'g') {
            sum = sumsqGreen[x+w-1][y+h-1] - sumsqGreen[x-1][y+h-1];
        } else if (channel == 'b') {
            sum = sumsqBlue[x+w-1][y+h-1] - sumsqBlue[x-1][y+h-1];
        }
    } else {
        if (channel == 'r') {
            sum = sumsqRed[x+w-1][y+h-1] - sumsqRed[x+w-1][y-1] - sumsqRed[x-1][y+h-1] + sumsqRed[x-1][y-1];
        } else if (channel == 'g') {
            sum = sumsqGreen[x+w-1][y+h-1] - sumsqGreen[x+w-1][y-1] - sumsqGreen[x-1][y+h-1] + sumsqGreen[x-1][y-1];
        } else if (channel == 'b') {
            sum = sumsqBlue[x+w-1][y+h-1] - sumsqBlue[x+w-1][y-1] - sumsqBlue[x-1][y+h-1] + sumsqBlue[x-1][y-1];
        }
    }

    return sum;

}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
    double redsq = getSumSq('r', ul, w, h);
    double greensq = getSumSq('g', ul, w, h);
    double bluesq = getSumSq('b', ul, w, h);
    
    double red = getSum('r', ul, w, h);
    double green = getSum('g', ul, w, h);
    double blue = getSum('b', ul, w, h);

    double redv = redsq - (red) * (red) / (w * h);
    double greenv = greensq - (green) * (green) / (w * h);
    double bluev = bluesq - (blue) * (blue) / (w * h);

    return redv+greenv+bluev;

}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
    RGBAPixel pixel;

    long r = getSum('r', ul, w, h)/(w*h);
    long g = getSum('g', ul, w, h)/(w*h);
    long b = getSum('b', ul, w, h)/(w*h);

    pixel = *(new RGBAPixel(r,g,b));

    return pixel;

}
