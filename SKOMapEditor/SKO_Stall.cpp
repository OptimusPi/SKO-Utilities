#include "SKO_Stall.h"

//default constructor
SKO_Stall::SKO_Stall()
{
}

//alternate constructor
SKO_Stall::SKO_Stall(int shopid_in, int x_in, int y_in, int w_in, int h_in)
{
    x = x_in;
    y = y_in;
    w = w_in;
    h = h_in;
    type = shopid_in;
}

