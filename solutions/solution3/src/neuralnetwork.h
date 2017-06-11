#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <QtCore>

#include "image.h"
#include "tiny_dnn/tiny_dnn.h"
using namespace tiny_dnn;
using namespace tiny_dnn::layers;
using namespace tiny_dnn::activation;

#define DIMENSION 32
#define NETWORK_SIZE DIMENSION * DIMENSION
#define NETWORK_BATCHSIZE 1
#define NETWORK_EPOCHS 50
#define NETWORK_ALPHA_START 0.0001
#define NETWORK_ALPHA_MAX 0.000001
#define NETWORK_HIDDEN_LAYER 200

static int PrewittX[3][3] = {
    { -1, 0, 1 },
    { -1, 0, 1 },
    { -1, 0, 1 }
};

static int PrewittY[3][3] = {
    { 1, 1, 1 },
    { 0, 0, 0 },
    { -1, -1, -1 }
};

static int SchaarX[3][3] = {
    { -3, 0, 3 },
    { -10, 0, 10 },
    { -3, 0, 3}
};

static int SchaarY[3][3] = {
    { 3, 10, 3 },
    { 0, 0, 0 },
    { -3, -10, -3 }
};

static int HighFilter[3][3] = {
    { -1, -1, -1 },
    { -1, 14, -1 },
    { -1, -1, -1 }
};

class NeuralNetwork
{
    public:
        void initNetwork();
        bool convertImage(const QString path, std::vector<vec_t>& data);
        bool convertImages(const QString directory, std::vector<vec_t> &data);
        bool loadAnnotation(const QString path, std::vector<vec_t>& data);
        bool loadAnnotations(const QString directory, std::vector<vec_t>& data);
        void testNetwork(int paramType, QString database, QString annotations);

    private:
        momentum optimizer;
        network<sequential> neuralNetwork;
};

#endif // NEURALNETWORK_H
