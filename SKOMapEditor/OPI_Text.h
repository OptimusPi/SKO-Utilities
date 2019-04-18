#ifndef TEXT_H__
#define TEXT_H__

#include <string.h>
#include <cstdlib>

class Text
{
      
      public:
         Text();
         void SetText(const char* index);
         short int letter_x[100];
         short int letter_y[100];
         short int length;
         float R, G, B;
         float pos_x, pos_y;
         bool used;
             
};

#endif
