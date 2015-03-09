#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <linedetector.h>
#include <cardriver.h>
#include <inputdata.h>

class Controller
{
private:
    CvSize	videoRes;
    CvPoint	cropAreaP1;
    CvPoint	cropAreaP2;
    Mat srcImgOCV;
    int frameNumber;

    int videoW;
    int videoH;

    int videoW1_ROI;
    int videoH1_ROI;
    int videoW2_ROI;
    int videoH2_ROI;
    Controller    ();                         // Private constructor
    ~Controller   () {}
    Controller    (const Controller&);        // Prevent copy-construction
    Controller& operator=(const Controller&); // Prevent assignment
public:
    static Controller& Instance()
    {
      static Controller singleton;
      return singleton;
    }
    int start();    
};

#endif // CONTROLLER_H
