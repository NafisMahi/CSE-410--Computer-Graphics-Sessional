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
using namespace std;

double eyeX,eyeY,eyeZ;
double lookX,lookY,lookZ;
double upX,upY,upZ;
double fovy,aspect,near,far;

//define a stack of transformation matrices

stack<vector<vector<double>>> stackOfTransformations;

vector<vector<double>> T;

// 3D vector type
struct Vector3 {
    double x, y, z;

    Vector3(double _x = 0.0f, double _y = 0.0f, double _z = 0.0f) : x(_x), y(_y), z(_z) {}

    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    Vector3 operator*(double scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
};

// Dot product of two 3D vectors
float dotProduct(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Cross product of two 3D vectors
Vector3 crossProduct(const Vector3& a, const Vector3& b) {
    return Vector3(a.y * b.z - a.z * b.y,
                   a.z * b.x - a.x * b.z,
                   a.x * b.y - a.y * b.x);
}

void initializeTransformationMatrix()
{
    T.resize(4);
    for(int i=0;i<4;i++)
    {
        T[i].resize(4);
    }
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            T[i][j]=0;
        }
    }
    T[0][0]=1;
    T[1][1]=1;
    T[2][2]=1;
    T[3][3]=1;
}

vector<vector<double>> initializePointMatrix(vector<double> point)
{
    vector<vector<double>> pointMatrix;
    pointMatrix.resize(4);
    for(int i=0;i<4;i++)
    {
        pointMatrix[i].resize(1);
    }
    
    pointMatrix[0][0] = point[0];
    pointMatrix[1][0] = point[1];
    pointMatrix[2][0] = point[2];
    pointMatrix[3][0]=1;

    return pointMatrix;
}

vector<vector<double>> matrixMultiply(vector<vector<double>> m1,vector<vector<double>> m2)
{
    vector<vector<double>> result;
    result.resize(4);
    for(int i=0;i<4;i++)
    {
        result[i].resize(4);
    }
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            result[i][j]=0;
        }
    }
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            for(int k=0;k<4;k++)
            {
                result[i][j]+=m1[i][k]*m2[k][j];
            }
        }
    }
    return result;
}

vector<vector<double>> getTranslationMatrix(double dx,double dy,double dz)
{
    vector<vector<double>> translationMatrix;
    translationMatrix.resize(4);
    for(int i=0;i<4;i++)
    {
        translationMatrix[i].resize(4);
    }
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            translationMatrix[i][j]=0;
        }
    }
    translationMatrix[0][0]=1;
    translationMatrix[1][1]=1;
    translationMatrix[2][2]=1;
    translationMatrix[3][3]=1;
    translationMatrix[0][3]=dx;
    translationMatrix[1][3]=dy;
    translationMatrix[2][3]=dz;
    return translationMatrix;
}

vector<vector<double>> getScalingMatrix(double sx,double sy,double sz)
{
    vector<vector<double>> scalingMatrix;
    scalingMatrix.resize(4);
    for(int i=0;i<4;i++)
    {
        scalingMatrix[i].resize(4);
    }
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            scalingMatrix[i][j]=0;
        }
    }
    scalingMatrix[0][0]=sx;
    scalingMatrix[1][1]=sy;
    scalingMatrix[2][2]=sz;
    scalingMatrix[3][3]=1;
    return scalingMatrix;
}

Vector3 RodriguesFormula(Vector3 x,Vector3 a,double theta)
{
    Vector3 v1=x*cos(theta*M_PI/180);
    Vector3 v2=crossProduct(a,x)*sin(theta*M_PI/180);
    Vector3 v3=a*dotProduct(a,x)*(1-cos(theta*M_PI/180));
    return v1+v2+v3;
}

Vector3 normalize(Vector3 v)
{
    float length=sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
    return Vector3(v.x/length,v.y/length,v.z/length);
}

vector<vector<double>> getRotationMatrix(Vector3 a,double angle)
{
    a = normalize(a);
    
    vector<vector<double>> rotationMatrix;
    rotationMatrix.resize(4);
    for(int i=0;i<4;i++)
    {
        rotationMatrix[i].resize(4);
    }

    Vector3 c1=RodriguesFormula(Vector3(1,0,0),a,angle);
    Vector3 c2=RodriguesFormula(Vector3(0,1,0),a,angle);
    Vector3 c3=RodriguesFormula(Vector3(0,0,1),a,angle);
    
    rotationMatrix[0][0]=c1.x;
    rotationMatrix[0][1]=c2.x;
    rotationMatrix[0][2]=c3.x;
    rotationMatrix[0][3]=0;
    rotationMatrix[1][0]=c1.y;
    rotationMatrix[1][1]=c2.y;
    rotationMatrix[1][2]=c3.y;
    rotationMatrix[1][3]=0;
    rotationMatrix[2][0]=c1.z;
    rotationMatrix[2][1]=c2.z;
    rotationMatrix[2][2]=c3.z;
    rotationMatrix[2][3]=0;
    rotationMatrix[3][0]=0;
    rotationMatrix[3][1]=0;
    rotationMatrix[3][2]=0;
    rotationMatrix[3][3]=1;

    return rotationMatrix;
}

void stackPush()
{
    stackOfTransformations.push(T);
}

void stackPop()
{
    T=stackOfTransformations.top();
    stackOfTransformations.pop();
}

void stackTop()
{
    T=stackOfTransformations.top();
}

vector<double> readLine(string line)
{
    std::vector<double> doubles;
    std::istringstream iss(line);
    double value;

    while (iss >> value) {
        doubles.push_back(value);
    }

    return doubles;
}

void writeToFile(vector<vector<double>> p,std :: ofstream& file)
{
    file << std :: fixed << std :: setprecision(7) << p[0][0] << " " << p[1][0] << " " << p[2][0] << endl;
}

void writeToFile2(vector<vector<double>> p,std :: ofstream& file)
{
    file << std :: fixed << std :: setprecision(7) << p[0][0]/p[3][0] << " " << p[1][0]/p[3][0] << " " << p[2][0]/p[3][0] << endl;
}

void parseSceneFile()
{
    ofstream outputFile;
    outputFile.open("stage1.txt");

    ifstream file;
    file.open("scene.txt");
    string line;
    int lineCount = 0;
    
    while(getline(file,line))
    {
        lineCount++;
        if(lineCount == 1)
        {
            vector<double> eye = readLine(line);
            eyeX = eye[0];
            eyeY = eye[1];
            eyeZ = eye[2];
        }
        else if(lineCount == 2)
        {
            vector<double> look = readLine(line);
            lookX = look[0];
            lookY = look[1];
            lookZ = look[2];
        }
        else if(lineCount == 3)
        {
            vector<double> up = readLine(line);
            upX = up[0];
            upY = up[1];
            upZ = up[2];
        }
        else if(lineCount == 4)
        {
            vector<double> fovyAspectNearFar = readLine(line);
            fovy = fovyAspectNearFar[0];
            aspect = fovyAspectNearFar[1];
            near = fovyAspectNearFar[2];
            far = fovyAspectNearFar[3];
            //std :: cout << fovy << " " << aspect << " " << near << " " << far << endl;
        }
        else
        {
            if(line == "triangle")
            {
                getline(file,line);  
                vector<double> v1 = readLine(line);
                vector<vector<double>> v1Matrix = initializePointMatrix(v1);
                vector<vector<double>> result1 = matrixMultiply(T,v1Matrix);
                writeToFile(result1,outputFile);
                //outputFile << std :: fixed << std :: setprecision(6) << result1[0][0] << " " << result1[1][0] << " " << result1[2][0] << endl;

                getline(file,line);
                vector<double> v2 = readLine(line);
                vector<vector<double>> v2Matrix = initializePointMatrix(v2);
                vector<vector<double>> result2 = matrixMultiply(T,v2Matrix);
                writeToFile(result2,outputFile);
                //outputFile << std :: fixed << std :: setprecision(6) << result2[0][0] << " " << result2[1][0] << " " << result2[2][0] << endl;

                getline(file,line);
                vector<double> v3 = readLine(line);
                vector<vector<double>> v3Matrix = initializePointMatrix(v3);
                vector<vector<double>> result3 = matrixMultiply(T,v3Matrix);
                writeToFile(result3,outputFile);
                //outputFile << std :: fixed << std :: setprecision(6) << result3[0][0] << " " << result3[1][0] << " " << result3[2][0] << endl;
                
                outputFile << endl;
                
            }

            else if(line == "scale")
            {
                getline(file,line);
                vector<double> scale = readLine(line);
                vector<vector<double>> scalingMatrix = getScalingMatrix(scale[0],scale[1],scale[2]);
                T = matrixMultiply(T,scalingMatrix);
            }

            else if(line == "translate")
            {
                getline(file,line);
                vector<double> translate = readLine(line);
                vector<vector<double>> translationMatrix = getTranslationMatrix(translate[0],translate[1],translate[2]);
                T = matrixMultiply(T,translationMatrix);
            }

            else if(line == "rotate")
            {
                getline(file,line);
                vector<double> rotate = readLine(line);
                vector<vector<double>> rotationMatrix = getRotationMatrix(Vector3(rotate[1],rotate[2],rotate[3]),rotate[0]);
                T = matrixMultiply(T,rotationMatrix);
            }

            else if(line == "push")
            {
                stackPush();
            }

            else if(line == "pop")
            {
                stackPop();
            }

            else if(line == "end")
            {
                break;
            }
            
        }
    }

    outputFile.close();
    file.close();
}

Vector3 getl()
{
    Vector3 look(lookX,lookY,lookZ);
    Vector3 eye(eyeX,eyeY,eyeZ);
    return normalize(look-eye);
}

Vector3 getr()
{
    Vector3 up(upX,upY,upZ);
    return normalize(crossProduct(getl(),up));
}

Vector3 getu()
{
    Vector3 up(upX,upY,upZ);
    return normalize(crossProduct(getr(),normalize(getl())));
}

double getFovX()
{
    return fovy*aspect;
}

double getT()
{
    return near*tan((fovy/2)*(acos(-1)/180));
}

double getR()
{
    return near*tan((getFovX()/2)*(acos(-1)/180));
}   

vector<vector<double>> getProjectionMatrix()
{
    vector<vector<double>> projectionMatrix;
    projectionMatrix.resize(4);
    for(int i=0;i<4;i++)
    {
        projectionMatrix[i].resize(4);
    }
    projectionMatrix[0][0]=near/getR();
    projectionMatrix[0][1]=0;
    projectionMatrix[0][2]=0;
    projectionMatrix[0][3]=0;

    projectionMatrix[1][0]=0;
    projectionMatrix[1][1]=near/getT();
    projectionMatrix[1][2]=0;
    projectionMatrix[1][3]=0;

    projectionMatrix[2][0]=0;
    projectionMatrix[2][1]=0;
    projectionMatrix[2][2]=-(far+near)/(far-near);
    projectionMatrix[2][3]=-(2*far*near)/(far-near);

    projectionMatrix[3][0]=0;
    projectionMatrix[3][1]=0;
    projectionMatrix[3][2]=-1;
    projectionMatrix[3][3]=0;

    return projectionMatrix;
}

vector<vector<double>> getViewRotation()
{
    vector<vector<double>> viewRotationMatrix;
    viewRotationMatrix.resize(4);
    for(int i=0;i<4;i++)
    {
        viewRotationMatrix[i].resize(4);
    }
    viewRotationMatrix[0][0]=getr().x;
    viewRotationMatrix[0][1]=getr().y;
    viewRotationMatrix[0][2]=getr().z;
    viewRotationMatrix[0][3]=0;

    viewRotationMatrix[1][0]=getu().x;
    viewRotationMatrix[1][1]=getu().y;
    viewRotationMatrix[1][2]=getu().z;
    viewRotationMatrix[1][3]=0;

    viewRotationMatrix[2][0]=-getl().x;
    viewRotationMatrix[2][1]=-getl().y;
    viewRotationMatrix[2][2]=-getl().z;
    viewRotationMatrix[2][3]=0;

    return viewRotationMatrix;
}

void parseStage1File()
{
    ofstream outputFile;
    outputFile.open("stage2.txt");

    ifstream file;
    file.open("stage1.txt");
    string line;
    int lineCount = 0;
    while(getline(file,line))
    {
        lineCount++;
        vector<double> v1 = readLine(line);
        
        if(v1.size() == 0)
        {
            outputFile << endl;
            continue;
        }

        vector<vector<double>> v1Matrix = initializePointMatrix(v1);
        vector<vector<double>> RT = matrixMultiply(getViewRotation(),getTranslationMatrix(-eyeX,-eyeY,-eyeZ));
        vector<vector<double>> result1 = matrixMultiply(RT,v1Matrix);
        //std :: cout << result1[0][0] << " " << result1[1][0] << " " << result1[2][0] << " " << result1[3][0] << endl;
        writeToFile(result1,outputFile);
    }
}

void parseStage2File()
{
   // std :: cout << getFovX() << " " << getT() << " " << getR() << endl;
    ofstream outputFile;
    outputFile.open("stage3.txt");

    ifstream file;
    file.open("stage2.txt");
    string line;
    int lineCount = 0;
    while(getline(file,line))
    {
        lineCount++;
        vector<double> v1 = readLine(line);
        //std :: cout << v1[0] << " " << v1[1] << " " << v1[2] <<endl;
        
        if(v1.size() == 0)
        {
            outputFile << endl;
            continue;
        }

        vector<vector<double>> v1Matrix = initializePointMatrix(v1);
        vector<vector<double>> result1 = matrixMultiply(getProjectionMatrix(),v1Matrix);
        //std :: cout << result1[0][0] << " " << result1[1][0] << " " << result1[2][0] << " " << result1[3][0] << endl;
        writeToFile2(result1,outputFile);
    }
}

int main()
{
    initializeTransformationMatrix();
    parseSceneFile();
    parseStage1File();
    parseStage2File();
    return 0;
}