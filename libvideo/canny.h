#include "imageprocessing.h"
#include "framebufferrgb24be.h"

class CannyEdgeDetection{

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

      CannyEdgeDetection (uint32_t width, uint32_t height);
      void start(FrameBuffer* frame, FrameBuffer* outFrame);
      void edgeDetection(FrameBuffer* frame, FrameBuffer *out);
      void nonMaxSupression(FrameBuffer* out);
      void hysteresisThreshold(FrameBuffer* out, uint8_t lowThreshold, uint8_t highThreshold);
      void HystRec(FrameBuffer* out, long x, long y,uint8_t lowThresh);
};
