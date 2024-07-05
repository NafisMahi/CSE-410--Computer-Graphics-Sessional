#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "bitmap_image.hpp"
using namespace std;

int z_max = 1;
int screenHeight,screenWidth;
double leftLimit,rightLimit,topLimit,bottomLimit,topY,bottomY,leftX,rightX,dx,dy;
static unsigned long int g_seed = 1;

struct Point{
    double x,y,z;
    Point(double x,double y,double z):x(x),y(y),z(z){}
    Point(){}
};

struct Color{
    int r,g,b;
    Color(int r,int g,int b):r(r),g(g),b(b){}
    Color(){}
};

struct Triangle{
    Point p1,p2,p3;
    Color color;
    Triangle(Point p1,Point p2,Point p3,Color color):p1(p1),p2(p2),p3(p3),color(color){}
    Triangle(){}
};

struct Plane{
    double a,b,c,d;
    Plane(double a,double b,double c,double d):a(a),b(b),c(c),d(d){}
    Plane(){}
};

inline int random()
{
 g_seed = (214013 * g_seed + 2531011);
 return (g_seed >> 16) & 0x7FFF;
}

vector<int> returnColor()
{
    int r = random() % 256;
    int g = random() % 256;
    int b = random() % 256;
    vector<int> color;
    color.push_back(r);
    color.push_back(g);
    color.push_back(b);
    return color;
}

void initialize()
{
    leftLimit = -1.0;
    rightLimit = 1.0;
    topLimit = 1.0;
    bottomLimit = -1.0;
    dx = (rightLimit - leftLimit)/screenWidth;
    dy = (topLimit - bottomLimit)/screenHeight;
    topY = topLimit - dy/2;
    bottomY = bottomLimit + dy/2;
    //cout << "topY " << topY << " bottomY " << bottomY << endl;
    leftX = leftLimit + dx/2;
    rightX = rightLimit - dx/2;
    //cout << "leftX " << leftX << " rightX " << rightX << endl;
}

vector<vector<double>> initializeZbuffer()
{
    std::vector<std::vector<double>> zBuffer(screenHeight, std::vector<double>(screenWidth,z_max));
    cout << "zbuffer size " << zBuffer.size() << "   " << zBuffer[0].size() << endl;

    for(int i = 0;i < screenHeight;i++)
    {
        vector<double> row;
        for(int j = 0;j < screenWidth;j++)
        {
            row.push_back(1.0);
        }
        zBuffer.push_back(row);
    }

    return zBuffer;
}

bitmap_image createBitmapObject()
{
    bitmap_image image(screenWidth,screenHeight);
    image.set_all_channels(0,0,0);
    image.save_image("output.bmp");
    return image;
}

vector<double> readLine(string line,std :: ifstream &input)
{
    std::vector<double> doubles;
    for(int lineNum = 0;lineNum < 3;lineNum++)
    {
        std::getline(input, line);
        std::istringstream iss(line);
        double x, y, z;
        iss >> x >> y >> z;
        doubles.push_back(x);
        doubles.push_back(y);
        doubles.push_back(z);

    }
    return doubles;
}

void parseConfigFile()
{
    ifstream configFile("config.txt");
    string line;
    std::vector<double> doubles;
    while(getline(configFile,line))
    {
        std::istringstream iss(line);
        double value;
        while (iss >> value) {
            doubles.push_back(value);
        }   
    }
    
    screenWidth = doubles[0];
    screenHeight = doubles[1];
}

void parseFile(string fileName,vector<Triangle> &triangles)
{
    ifstream configFile(fileName);
    string line;
    std::vector<double> doubles;
    int count = 0;
   
    while(true)
    {
            doubles = readLine(line,configFile);
            Point p1(doubles[0],doubles[1],doubles[2]);
            Point p2(doubles[3],doubles[4],doubles[5]);
            Point p3(doubles[6],doubles[7],doubles[8]);
            vector<int> color = returnColor();
            Color colour(color[0],color[1],color[2]);
            Triangle triangle(p1,p2,p3,colour);
            triangles.push_back(triangle);
            
            if(!getline(configFile,line))
            {
                break;
            }
    }
}

double findIntersection(double x1,double y1,double x2,double y2,double y)
{
    double x = x1 + (x2 - x1) * (y - y1)/(y2 - y1);
    return x;
}

void clipY(Triangle triangle)
{
    Point p1 = triangle.p1;
    Point p2 = triangle.p2;
    Point p3 = triangle.p3;
    double y1 = p1.y;
    double y2 = p2.y;
    double y3 = p3.y;
    double maxY = max(y1,max(y2,y3));
    double minY = min(y1,min(y2,y3));

    if(maxY > topY)
    {
        topY = topY;
    }
    else
    {
        double cellSize = 2.0/screenHeight;
        double diff = ceil(abs(maxY - topLimit)/cellSize);
        topY = topLimit - diff * cellSize - cellSize/2;
    }
    
    if(minY < bottomY)
    {
        bottomY = bottomY;
    }
    else
    {
        double cellSize = 2.0/screenHeight;
        double diff = floor(abs(minY - topLimit)/cellSize);
        bottomY = topLimit - diff * cellSize - cellSize/2;
    }
}

void clipX(double x1,double x2,double x3)
{
    double maxX,minX;

    if(std::isnan(x1) || std::isinf(x1))
    {
            //cout << x2 <<" this is x2 " << x3 << " this is x3 " << endl;
            minX = min(x2,x3);
            maxX = max(x2,x3);
    }

    if(std::isnan(x2) || std::isinf(x2))
    {
            minX = min(x1,x3);
            maxX = max(x1,x3);
    }

    if(std::isnan(x3) || std::isinf(x3))
    {
            minX = min(x1,x2);
            maxX = max(x1,x2);
    }        

   // cout<< "minX " << minX << " maxX " << maxX << endl;
    if(maxX > rightX)
    {
        rightX = screenWidth - 1;
    }
    else
    {
        rightX = round((maxX - leftX)/dx);
        // double cellSize = 2.0/screenWidth;
        // double diff = floor(abs(maxX - leftLimit)/cellSize);
        // rightX = leftLimit + diff * cellSize + cellSize;
    }
    
    if(minX < leftX)
    {
        leftX = 0;
    }
    else
    {
        leftX = round((minX - leftX)/dx);
        // double cellSize = 2.0/screenWidth;
        // double diff = floor(abs(minX - leftLimit)/cellSize);
        // leftX = leftLimit + diff * cellSize + cellSize/2.0;

    }
}

//find equation of a plane given three points 
Plane findPlaneEquation(Triangle triangle)
{
    Point p1 = triangle.p1;
    Point p2 = triangle.p2;
    Point p3 = triangle.p3;
    Point v1 = {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z};
    Point v2 = {p3.x - p1.x, p3.y - p1.y, p3.z - p1.z};

    // Cross product of the two vectors gives the normal to the plane
    Point normal;
    normal.x = v1.y * v2.z - v1.z * v2.y;
    normal.y = v1.z * v2.x - v1.x * v2.z;
    normal.z = v1.x * v2.y - v1.y * v2.x;

    // The equation of the plane is given by the normal and a point on the plane
    double d = -(normal.x * p1.x + normal.y * p1.y + normal.z * p1.z);
    return {normal.x, normal.y, normal.z, d};
}

double findZ(double x,double y,Triangle triangle)
{
    Plane plane = findPlaneEquation(triangle);
    double z = (-plane.d - plane.a * x - plane.b * y)/plane.c;
    return z;
}

vector<vector<double>> scanTriangle(Triangle triangle,vector<vector<double>> &zBuffer,bitmap_image &image)
{
    Point p1 = triangle.p1;
    Point p2 = triangle.p2;
    Point p3 = triangle.p3;
    double minX;
    double maxX;
    double cellI = (topLimit - (topY + dy/2))/dy;
   // cout << "this is cellI " << cellI << endl;
    double cellJ;
    //cout << topY << " this is topy and bottomy  " << bottomY << endl;

    for(double i = topY ; i >= bottomY;i -= dy)
    {
        double x1 = findIntersection(p1.x,p1.y,p2.x,p2.y,i);
        double x2 = findIntersection(p2.x,p2.y,p3.x,p3.y,i);
        double x3 = findIntersection(p3.x,p3.y,p1.x,p1.y,i);
        clipX(x1,x2,x3);
        cellJ = ((leftX - dx/2) - leftLimit)/dx;
        for(double j = leftX;j <= rightX;j ++)
        {
            double thisJ = leftLimit + dx/2 + j*dx;
            double z = findZ(thisJ,i,triangle);
            if(z > -1.0 && z < zBuffer[cellI][j])
            {
                zBuffer[cellI][j] = z;
                //cout << "hello " << " ";
                image.set_pixel(j,cellI,triangle.color.r,triangle.color.g,triangle.color.b);
            }
           
            cellJ++;
        }
        leftX = leftLimit + dx/2;
        rightX = rightLimit - dx/2;
        cellI++;
    }
    return zBuffer;
}

void writeZbufferToFile(vector<vector<double>> &zBuffer)
{
    ofstream myfile;
    myfile.open("zbuffer.txt");
    for(int i = 0;i < screenHeight;i++)
    {
        for(int j = 0;j < screenWidth;j++)
        {
            if(zBuffer[i][j] < 1.0)
            {
                myfile << std :: fixed << std :: setprecision(6) << zBuffer[i][j] << "\t";
            }

            else
            {
             myfile << std :: fixed << std :: setprecision(6) << zBuffer[i][j] << "\t";        
            }

        }
        myfile << endl;
    }
    myfile.close();
}

void printZbuffer(vector<vector<double>> &zBuffer,int count)

{
    cout << "iteration num " << count << endl;
    for(int i = 0;i < zBuffer.size();i++)
    {
        for(int j = 0;j < zBuffer[i].size();j++)
        {
           if(zBuffer[i][j] < 1.0)
           {
                cout << std :: fixed << std :: setprecision(6) << zBuffer[i][j] << "\t";
           }
        }
        cout << endl;
    }
}

void Zbuffer()
{
    vector<Triangle> triangles;
    parseConfigFile();
    parseFile("stage3.txt",triangles);
    initialize();
    vector<vector<double>> zBuffer = initializeZbuffer();

     bitmap_image image = createBitmapObject();
    for(int i = 0;i < triangles.size() - 1;i++)
    {
        //cout << "hello here" << endl;
        Triangle triangle = triangles[i];
        Color color = triangle.color;
        clipY(triangle);
        zBuffer = scanTriangle(triangle,zBuffer,image);
        //printZbuffer(zBuffer,i+1);
        topY = topLimit - dy/2;
        bottomY = bottomLimit + dy/2;
        leftX = leftLimit + dx/2;
        rightX = rightLimit - dx/2;
    }

    printZbuffer(zBuffer,100);

    for(int i = 0;i<screenHeight;i++)
    {
        for(int j=0;j<screenWidth;j++)
        {
           //cout << zBuffer[i][j] <<" ";
        }
        cout << endl;
    }
    image.save_image("out.bmp");
    writeZbufferToFile(zBuffer);
    
}

int main()
{
    Zbuffer();
    return 0;

}