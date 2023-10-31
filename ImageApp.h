//
// Created by Jsl on 2023/10/20.
//

#ifndef WXIMAGETOOL_IMAGEAPP_H
#define WXIMAGETOOL_IMAGEAPP_H
#include "ImageFrame.h"

class ImageApp : public wxApp{
public:
    virtual bool OnInit() override;
};

IMPLEMENT_APP(ImageApp);

#endif //WXIMAGETOOL_IMAGEAPP_H
