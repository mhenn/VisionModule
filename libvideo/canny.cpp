#include "canny.h"


Canny::Canny(FrameBuffer* frame){
   this->width = frame->width;
   this->height = frame->height;
   this->frame = frame;
   edge_dir = new uint8_t[height * width];
   edge_magnitude = new float[height* width];

}

void Canny::start(FrameBuffer* outFrame){

//   ImageProcessing:convolution(frame, outFrame, kernel_gauss);

                edgeDetection();
                // nonMaxSupression();
                // hysteresisThreshold();
                ImageProcessing::convertBuffer(frame,outFrame);


}

void Canny::edgeDetection(){

   FrameBuffer* outFrame = new FrameBufferRGB24BE();
   outFrame->initialize(width,height);

   for(int x = 0; x < width;x++)
      for(int y = 0; y< height;x++){
         RawPixel sumH = RawPixel(0,0,0);
         RawPixel sumV = RawPixel(0,0,0);
         for (int n = -1; n < +2; n++){
            for (int m = -1; m < +2; m++){
               int currx = n+x;
               int curry = m+y;
               if (currx >= 0 && curry >= 0 && currx < frame->width && curry < frame->height ){
                  RawPixel h,v;
                  frame->getPixel(curry, currx ,&v);
                  frame->getPixel(curry, currx ,&h);
                  sumH += h * kernel_sobel_h[(n+1) * 3+ (m+1)];
                  sumV += v * kernel_sobel_v[(n+1) * 3+ (m+1)];
               }
            }
            int f = sqrt(sumH.red *sumH.red + sumV.red * sumV.red);
            outFrame->setPixel(y,x,RawPixel(f,f,f));
         }
      }
   ImageProcessing::convertBuffer(outFrame,frame);
}

void Canny::nonMaxSupression(){}

void Canny::hysteresisThreshold(){}


