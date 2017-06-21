#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
using namespace std;

class imageProcessing{
    friend class HoughTransform;
private:
public:
    int numRows, numCols, minVal, maxVal;
    int** imgAry;
    
    imageProcessing(int numRows, int numCols, int minVal, int maxVal){
        this->numRows = numRows;
        this->numCols = numCols;
        this->minVal = minVal;
        this->maxVal = maxVal;
        
        //dynamically allocate imgAry
        imgAry = new int*[numRows];
        for(int i=0; i<numRows; i++){
            imgAry[i] = new int[numCols];
        }//for i
        
        //initialize imgAry to 0
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                imgAry[i][j] = 0;
            }//for j
        }//for i
    }//constructor
    
    void loadImage(ifstream& inFile){
        int data;
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                while(!inFile.eof()){
                    inFile >> data;
                    imgAry[i][j] = data;
                    break;
                }//while
            }//for j
        }//for i
        inFile.close();
    }//loadImage
    
    void print(ofstream& out){
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                out << imgAry[i][j] << " ";
            }//for j
            out << "\n";
        }//for i
    }//print
    
    void prettyPrint(ofstream& out){
        out << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                if(imgAry[i][j] > 0){
                    out << "1" << " ";
                    //cout << "i: " << i << "j: " << j << "\n";
                }//if
                else{
                    out << " ";
                }//else
            }//for j
            out << "\n";
        }//for i
    }//prettyPrint
    
};//imageProcessing

class HoughTransform{
    class xyCoord{
    private:
    public:
        int x, y;
    };//xyCoord
private:
public:
    xyCoord point; //point of type xyCoord
    int angleInDegree;
    double angleInRadians;
    int numRows, numCols, minVal, maxVal;
    int** HoughAry;
    double pi = 3.14159265359;
    
    HoughTransform(imageProcessing img){
        numRows = 180;
        numCols = sqrt((img.numRows*img.numRows) + (img.numCols*img.numCols));
        minVal = 0;
        maxVal = 0;
        
        //dynamically allocate HoughAry
        HoughAry = new int*[numRows];
        for(int i=0; i<numRows; i++){
            HoughAry[i] = new int[numCols];
        }//for i
        
        //initialize HoughAry to 0
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                HoughAry[i][j] = 0;
            }//for j
        }//for i
    }//constructor
    
    void computeHough(imageProcessing img,ofstream& out){
        int dist = 0;
        for(int i=0; i<img.numRows; i++){
            for(int j=0; j<img.numCols; j++){
                if(img.imgAry[i][j] > 0){
                    point.x = i;
                    point.y = j;
                    angleInDegree = 0;
                    while(angleInDegree < 180){
                        angleInRadians = angleInDegree / 180.00 * pi;
                        dist = computeDistance(point, angleInRadians);
                        HoughAry[angleInDegree][dist]++;
                        angleInDegree++;
                    }//while
                }//if
            }//for j
        }//for i
        detemineHeader(out);
    }//computeHough
    
    int computeDistance(xyCoord pt, double angle){
        int distance = 0, distance1 = 0;
        double t = 0.0;
        t = angle-atan(point.y/point.x) - (pi/2);
        distance1 = (int)sqrt((point.x * point.x) + (point.y * point.y)) * cos(t);
        distance = abs(distance1);
        return distance;
    }//compute Distance
    
    void detemineHeader(ofstream& out){
        out << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                maxVal = HoughAry[i][j];
                out << HoughAry[i][j];
            }//for j
            out << "\n";
        }//for i
    }//determineHeader
    
    void print(ofstream& out){
        out << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                out << HoughAry[i][j];
            }//for j
            out << "\n";
        }//for i
    }//prettyPrint
    
    void prettyPrint(ofstream& out){
        out << numRows << " " << numCols << " " << minVal << " " << maxVal << "\n";
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                if(HoughAry[i][j] > 0){
                    out << HoughAry[i][j] << " ";
                }//if
                else{
                    out << " ";
                }//else
            }//for j
            out << "\n";
        }//for i
    }//prettyPrint
    
};//HoughTransform

int main(int argc, const char * argv[]) {
    ifstream inFile;
    ofstream outFile1;
    ofstream outFile2;
    
    inFile.open(argv[1]);
    outFile1.open(argv[2]);
    outFile2.open(argv[3]);
    int numRows, numCols, minVal, maxVal;
    inFile >> numRows;
    inFile >> numCols;
    inFile >> minVal;
    inFile >> maxVal;
    
    imageProcessing img(numRows, numCols, minVal, maxVal);
    img.loadImage(inFile);
    HoughTransform HT(img);
    HT.computeHough(img,outFile1);
    HT.prettyPrint(outFile2);
    
    outFile1.close();
    outFile2.close();
    return 0;
}//main
