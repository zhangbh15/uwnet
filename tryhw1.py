from uwnet import *

def conv_net():
    l = [   make_convolutional_layer(32, 32, 3, 8, 3, 1),
            make_activation_layer(RELU),
            make_maxpool_layer(32, 32, 8, 3, 2),
            make_convolutional_layer(16, 16, 8, 16, 3, 1),
            make_activation_layer(RELU),
            make_maxpool_layer(16, 16, 16, 3, 2),
            make_convolutional_layer(8, 8, 16, 32, 3, 1),
            make_activation_layer(RELU),
            make_maxpool_layer(8, 8, 32, 3, 2),
            make_convolutional_layer(4, 4, 32, 64, 3, 1),
            make_activation_layer(RELU),
            make_maxpool_layer(4, 4, 64, 3, 2),
            make_connected_layer(256, 10),
            make_activation_layer(SOFTMAX)]
    return make_net(l)

def fully_net():
    l = [   make_connected_layer(32*32*3, 16*16*3),
            make_activation_layer(RELU),
            make_connected_layer(16*16*3, 8*8*3),
            make_activation_layer(RELU),
            make_connected_layer(8*8*3, 4*4*3),
            make_activation_layer(RELU),
            make_connected_layer(4*4*3, 256),
            make_activation_layer(RELU),
            make_connected_layer(256, 10),
            make_activation_layer(SOFTMAX)]
    return make_net(l)

print("loading data...")
train = load_image_classification_data("cifar/cifar.train", "cifar/cifar.labels")
test  = load_image_classification_data("cifar/cifar.test",  "cifar/cifar.labels")
print("done")
print

print("making model...")
batch = 128
iters = 5000
rate = .01
momentum = .9
decay = .005

# m = conv_net()
m = fully_net()
print("training...")
train_image_classifier(m, train, batch, iters, rate, momentum, decay)
print("done")
print

print("evaluating model...")
print("training accuracy: %f", accuracy_net(m, train))
print("test accuracy:     %f", accuracy_net(m, test))

# How accurate is the fully connected network vs the convnet when they use similar number of operations?
# Why are you seeing these results? Speculate based on the information you've gathered and what you know about DL and ML.
# Your answer:
#
# Operation number = 32*32*3*3*3*8+16*16*3*3*8*16+8*8*3*3*16*32+4*4*3*3*32*64+256*10 = 1108480
# So the approximate operations number is 1108480
#
# cov_net:
# training accuracy: %f 0.6904799938201904
# test accuracy:     %f 0.6470999717712402
#
# fully_net:
# training accuracy: %f 0.5685799717903137
# test accuracy:     %f 0.5121999979019165
#
## As we can see, the convolution net have higher accuracy than fully connected net.
## I think it maybe because the convolution layer only extract "nearby" meaningful features and avoid calculating
## some "far away" useless features. And it is much efficient because of the reduction of much work.


