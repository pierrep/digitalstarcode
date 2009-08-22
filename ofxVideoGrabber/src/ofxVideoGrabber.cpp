
#include "ofxVideoGrabber.h"
#include "ofUtils.h"

//--------------------------------------------------------------------
ofxVideoGrabber::ofxVideoGrabber()
{
    bGrabberInited = false;
	bUseTexture	= true;
	bIsFrameNew = false;
	pixels = NULL;
}

//--------------------------------------------------------------------
ofxVideoGrabber::~ofxVideoGrabber(){
	close();

}

//--------------------------------------------------------------------
bool ofxVideoGrabber::initGrabber( int _width, int _height, int _format, int _targetFormat, int _frameRate, bool _useTexture, ofxVideoGrabberSDK *sdk, ofxVideoGrabberSettings* _settings)
{
    videoGrabber = sdk;
    settings = _settings;
    bool initResult = videoGrabber->init( _width, _height, _format, _targetFormat, _frameRate );

    width = videoGrabber->width;
    height = videoGrabber->height;
    bpp = videoGrabber->bpp;
    bUseTexture = _useTexture;

    if (bUseTexture){
        targetFormat = _targetFormat;

        /* create the texture, set the pixels to black and upload them to the texture (so at least we see nothing black the callback) */
        if(targetFormat == VID_FORMAT_GREYSCALE)
        {
            tex.allocate(videoGrabber->width,videoGrabber->height,GL_LUMINANCE);
            pixels = new unsigned char[width * height * bpp];
            memset(pixels, 0, width*height*bpp);
            tex.loadData(pixels, width, height, GL_LUMINANCE);
        }
        else if(targetFormat == VID_FORMAT_RGB)
        {
            tex.allocate(videoGrabber->width,videoGrabber->height,GL_RGB);
            pixels = new unsigned char[width * height * bpp];
            memset(pixels, 0, width*height*bpp);
            tex.loadData(pixels, width, height, GL_RGB);
        }
        else
        {
            cout << "** Texture allocation failed. Target format must be either VID_FORMAT_GREYSCALE or VID_FORMAT_RGB **" << endl;
        }
    }

    settings->setupVideoSettings(videoGrabber);

    bGrabberInited = true;
    return initResult;
}


//--------------------------------------------------------------------
void ofxVideoGrabber::setUseTexture(bool bUse)
{
	bUseTexture = bUse;
}

//--------------------------------------------------------------------
bool ofxVideoGrabber::isFrameNew()
{
    return bIsFrameNew;
}

//--------------------------------------------------------------------
unsigned char* ofxVideoGrabber::getPixels()
{
    return pixels;
    //return videoGrabber->getPixels();
}

//--------------------------------------------------------------------
void ofxVideoGrabber::update()
{
	grabFrame();
	settings->update();
}

//--------------------------------------------------------------------
void ofxVideoGrabber::grabFrame()
{

    if (bGrabberInited){
        bIsFrameNew = videoGrabber->grabFrame(&pixels);
        if(bIsFrameNew) {
            if (bUseTexture){
                if(targetFormat == VID_FORMAT_GREYSCALE)
                {
                    tex.loadData(pixels, width, height, GL_LUMINANCE);
                }
                else
                {
                    tex.loadData(pixels, width, height, GL_RGB);
                }
			}
        }
    }
}

//--------------------------------------------------------------------
void ofxVideoGrabber::draw(float _x, float _y, float _w, float _h)
{
	if (bUseTexture){
		tex.draw(_x, _y, _w, _h);
	}
	settings->draw();
}

//--------------------------------------------------------------------
void ofxVideoGrabber::draw(float _x, float _y)
{
	draw(_x, _y, (float)width, (float)height);
}

//--------------------------------------------------------------------
void ofxVideoGrabber::close()
{
    if(bGrabberInited){
        bGrabberInited 		= false;
        bIsFrameNew 		= false;
        videoGrabber->close();
    }
}

//--------------------------------------------------------------------
float ofxVideoGrabber::getHeight(){
	return (float)height;
}

//--------------------------------------------------------------------
float ofxVideoGrabber::getWidth(){
	return (float)width;
}

//--------------------------------------------------------------------
ofTexture & ofxVideoGrabber::getTextureReference()
{
	if(!tex.bAllocated() ){
		ofLog(OF_LOG_WARNING, "ofxVideoGrabber - getTextureReference - texture is not allocated");
	}
	return tex;
}




