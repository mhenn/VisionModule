#include "imageprocessing.h"
#include "framebufferrgb24be.h"

class Canny{
   
   private:

   uint8_t* edge_dir;
   float* edge_magnitude;
   uint32_t width;
   uint32_t height;
   FrameBuffer* frame;


   double kernel_sobel_v[9] =
   {
      -1.0, 0.0, +1.0,
      -2.0, 0.0, +2.0,
      -1.0, 0.0, +1.0,
   };

   double kernel_sobel_h[9] =
   {
      -1.0, -2.0, -1.0,
      0.0, 0.0, 0.0,
      +1.0, +2.0, +1.0,
   };

   double kernel_gauss[9] =
   {
      0.0625,   0.125,  0.0625,
      0.125, 0.25,   0.125,
      0.0625,   0.125, 0.0625,
   };



   public:
   
   Canny(FrameBuffer* frame);
   void start(FrameBuffer* outFrame);
   void edgeDetection();
   void nonMaxSupression();
   void hysteresisThreshold();
};
