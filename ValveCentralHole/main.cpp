#include "ValveCentralHole.h"
#include <QtWidgets/QApplication>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ValveCentralHole w;
    w.show();
    return a.exec(); 
}
