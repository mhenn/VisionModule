#include "canny.h"


CannyEdgeDetection::CannyEdgeDetection (uint32_t width, uint32_t height){
   this->width = width;
   this->height = height;
   edge_dir = new uint8_t[height * width];
   edge_magnitude = new float[height* width];
}

void CannyEdgeDetection::start(FrameBuffer* frame, FrameBuffer* outFrame){

   FrameBuffer* out = new FrameBufferRGB24BE();
   out->initialize(width, height);

   ImageProcessing::toGreyScale(frame);
   ImageProcessing::convolution(frame, out, kernel_gauss);
   edgeDetection(frame,out);
   nonMaxSupression(out);
   hysteresisThreshold(out, 5,20);
   ImageProcessing::convertBuffer(out,outFrame);
}

void CannyEdgeDetection::edgeDetection(FrameBuffer* frame, FrameBuffer* out){

   float max = 0.0;
   float angle = 0.0;

   for(uint32_t x = 0; x < width;x++)
      for(uint32_t y = 0; y< height;y++){
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
         }
         uint32_t pos = y * width +x;
         uint32_t v = sumV.red , h = sumH.red;

         edge_magnitude[pos] = sqrt(h * h + v * v) / 4;

         max = edge_magnitude[pos] > max ? edge_magnitude[pos] : max;

         if ((h + v) != 0)
            angle = atan2(h,v) * 180 / 3.14;
         else 
            angle = 0;

         if (((angle > -22.5) && (angle <= 22.5)) ||
               ((angle > 157.5) && (angle <= -157.5))) {
            edge_dir[pos] = 0;
         } else if (((angle > 22.5) && (angle <= 67.5)) ||
               ((angle > -157.5) && (angle <= -112.5))) {
            edge_dir[pos] = 45;
         } else if (((angle > 67.5) && (angle <= 112.5)) ||
               ((angle > -112.5) && (angle <= -67.5))) {
            edge_dir[pos] = 90;
         } else if (((angle > 112.5) && (angle <= 157.5)) ||
               ((angle > -67.5) && (angle <= -22.5))) {
            edge_dir[pos] = 135;
         }
         
      }

   for (uint32_t x =0; x < width; x++ )
      for(uint32_t y = 0; y < height; y++){
         uint32_t pos = y * width + x;
         edge_magnitude[pos] =255.0f * edge_magnitude[pos] / max;
         int c = edge_magnitude[pos]; 
         RawPixel p = RawPixel(c,c,c);
         out->setPixel(y, x, p);
      }
}

void CannyEdgeDetection::nonMaxSupression(FrameBuffer* out){

   float pixel_1 = 0;
   float pixel_2 = 0;
   float pixel;

   for (int x = 1; x < width - 1; x++) 
      for (int y = 1; y < height - 1; y++) {
         uint32_t pos =y* width + x; 
         if (edge_dir[pos] == 0) {
            pixel_1 = edge_magnitude[(y + 1) * width + x];
            pixel_2 = edge_magnitude[(y - 1) * width + x];
         } else if (edge_dir[pos] == 45) {
            pixel_1 = edge_magnitude[(y + 1) * width + x - 1];
            pixel_2 = edge_magnitude[(y - 1) * width + x + 1];
         } else if (edge_dir[pos] == 90) {
            pixel_1 = edge_magnitude[y * width + x - 1];
            pixel_2 = edge_magnitude[y * width + x + 1];
         } else if (edge_dir[pos] == 135) {
            pixel_1 = edge_magnitude[(y + 1) * width + x + 1];
            pixel_2 = edge_magnitude[(y - 1) * width + x - 1];
         }
         pixel = edge_magnitude[pos];
         if ((pixel >= pixel_1) && (pixel >= pixel_2))
            out->setPixel(y, x, RawPixel(pixel,pixel,pixel));
         else
            out->setPixel(y, x, RawPixel(0,0,0));
         
      }

   auto setValue = [](uint32_t x, uint32_t y, FrameBuffer* frame){
       RawPixel p;
       frame->getPixel(y,x,&p);
       if (p.red == 128) {
               RawPixel white = RawPixel(255,255,255);
               frame->setPixel(y , x, white);
               return true;
            }
       return false;
   };

   auto setValues = [setValue](uint32_t x, uint32_t y, FrameBuffer* out){
      bool change =  setValue(x + 1, y    , out);
      change = setValue(x - 1, y    , out);
      change = setValue(x    , y + 1, out);
      change = setValue(x    , y - 1, out);
      change = setValue(x + 1, y + 1, out);
      change = setValue(x - 1, y - 1, out);
      change = setValue(x - 1, y + 1, out);
      return change && setValue(x + 1, y - 1, out);
   };


   bool change = true;
   RawPixel p;
   while (change) {
      change = false;
      for (uint32_t x = 1; x < width - 1; x++) 
         for (uint32_t y = 1; y < height - 1; y++) {
            out->getPixel(y,x,&p);
            if (p.red != 255)
               continue;
            change = setValues(x,y,out);
         }

      if (change) {
         for (uint32_t x = width - 2; x > 0; x--) 
            for (uint32_t y = height - 2; y > 0; y--) {
               out->getPixel(y,x,&p);
               if (p.red != 255)
                  continue;
               change = setValues(x,y,out);
            }
      }
   }

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
         out->getPixel(y,x,&p);
			if (p.red == 128) {
				out->setPixel(y, x, RawPixel(0,0,0));
			}
		}
	}
}

void CannyEdgeDetection::hysteresisThreshold(FrameBuffer* out, uint8_t lowThreshold, uint8_t highThreshold ){
 
   RawPixel p;  
   for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
         out->getPixel(y,x,&p);
			if (p.red >= highThreshold) {
            out->setPixel(y,x,RawPixel(255,255,255));
				HystRec(out,x, y, lowThreshold);
			}
		}
	}

	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
         out->getPixel(x,y,&p);
			if (p.red != 255) {
				out->setPixel(x, y, RawPixel(0,0,0));
			}
		}
	}
}

void CannyEdgeDetection::HystRec(FrameBuffer* out, long x, long y,uint8_t lowThreshold){

   uint8_t value = 0;
   RawPixel p;
   for (long x1 = x - 1; x1 <= x + 1; x1++) {
      for (long y1 = y - 1; y1 <= y + 1; y1++) {
         if ((x1 < height) & (y1 < width) & (x1 >= 0) & (y1 >= 0)
               & (x1 != x) & (y1 != y)) {
            out->getPixel(y1,x1,&p);
            value = p.red;
            if (value != 255) {
               if (value >= lowThreshold) {
                  out->setPixel(y1, x1, RawPixel(255,255,255));
                  HystRec(out,x1, y1, lowThreshold);
               }
               else {
                  out->setPixel(y1, x1, RawPixel(0,0,0));
               }
            }
         }
      }
   }
}

