#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include "uwnet.h"


// Run a maxpool layer on input
// layer l: pointer to layer to run
// matrix in: input to layer
// returns: the result of running the layer
matrix forward_maxpool_layer(layer l, matrix in)
{
    // Saving our input
    // Probably don't change this
    free_matrix(*l.x);
    *l.x = copy_matrix(in);

    int outw = (l.width-1)/l.stride + 1;
    int outh = (l.height-1)/l.stride + 1;
    matrix out = make_matrix(in.rows, outw*outh*l.channels);


    // TODO: 6.1 - iterate over the input and fill in the output with max values

    int c = l.channels;
    int size = l.size;
    int stride = l.stride;
    for(int r=0; r<in.rows;r++) {
        for(int k=0; k<c;k++) {
            for(int i=0; i<l.height; i+=stride) {
                for(int j=0; j<l.width; j+=stride) {
                    float max = -100.0;
                    if(size%2==0) {
                        for(int y=i; y<=i+size-1; y++) {
                            for(int x=j; x<=j+size-1; x++) {
                                int index = r*in.cols+k*l.width*l.height+y*l.width+x;
                                if(x<0||y<0||x>l.width||y>l.height) {
                                }else {
                                    max = max>in.data[index]? max:in.data[index];

                                }
                            }
                        }
                    } else {
                        for(int y=i-(size-1)/2; y<=i+(size-1)/2; y++) {
                            for(int x=j-(size-1)/2; x<=j+(size-1)/2; x++) {
                                int index = r*in.cols+k*l.width*l.height+y*l.width+x;
                                if(x<0||y<0||x>l.width||y>l.height) {
                                }else {
                                    max = max>in.data[index]? max:in.data[index];
                                }
                            }
                        }
                    }
                    out.data[r*out.cols+k*outw*outh+i*outw/stride+j/stride]=max;
                }
            }
        }
    }


    return out;
}

// Run a maxpool layer backward
// layer l: layer to run
// matrix dy: error term for the previous layer
matrix backward_maxpool_layer(layer l, matrix dy)
{
    matrix in = *l.x;
    matrix dx = make_matrix(dy.rows, l.width*l.height*l.channels);

    int outw = (l.width-1)/l.stride + 1;
    int outh = (l.height-1)/l.stride + 1;
    // TODO: 6.2 - find the max values in the input again and fill in the
    // corresponding delta with the delta from the output. This should be
    // similar to the forward method in structure.
    int c = l.channels;
    int size = l.size;
    int stride = l.stride;
    for(int r=0; r<in.rows;r++) {
        for(int k=0; k<c;k++) {
            for(int i=0; i<l.height; i+=stride) {
                for(int j=0; j<l.width; j+=stride) {
                    float max = -100.0;
                    int maxX = j;
                    int maxY = i;
                    if(size%2==0) {
                        for(int y=i; y<=i+size-1; y++) {
                            for(int x=j; x<=j+size-1; x++) {
                                int index = r*in.cols+k*l.width*l.height+y*l.width+x;
                                if(x<0||y<0||x>l.width||y>l.height) {
                                }else {
                                    if(max<in.data[index]) {
                                        max = in.data[index];
                                        maxX = x;
                                        maxY = y;
                                    }
                                }
                            }
                        }
                    } else {
                        for(int y=i-(size-1)/2; y<=i+(size-1)/2; y++) {
                            for(int x=j-(size-1)/2; x<=j+(size-1)/2; x++) {
                                int index = r*in.cols+k*l.width*l.height+y*l.width+x;
                                if(x<0||y<0||x>l.width||y>l.height) {
                                }else {
                                    if(max<in.data[index]) {
                                        max = in.data[index];
                                        maxX = x;
                                        maxY = y;
                                    }
                                }
                            }
                        }
                    }
                    dx.data[r*dx.cols+k*l.width*l.height+maxY*l.width+maxX] +=
                    dy.data[r*dy.cols+k*outw*outh+i*outw/stride+j/stride];
                }
            }
        }
    }
    return dx;
}

// Update maxpool layer
// Leave this blank since maxpool layers have no update
void update_maxpool_layer(layer l, float rate, float momentum, float decay){}

// Make a new maxpool layer
// int w: width of input image
// int h: height of input image
// int c: number of channels
// int size: size of maxpool filter to apply
// int stride: stride of operation
layer make_maxpool_layer(int w, int h, int c, int size, int stride)
{
    layer l = {0};
    l.width = w;
    l.height = h;
    l.channels = c;
    l.size = size;
    l.stride = stride;
    l.x = calloc(1, sizeof(matrix));
    l.forward  = forward_maxpool_layer;
    l.backward = backward_maxpool_layer;
    l.update   = update_maxpool_layer;
    return l;
}

//float max(float a, float b) {
//    return a>b ? a : b;
//}


