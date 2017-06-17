package com.mycompany.app;

import java.util.Random;

/**
 * Created by arthurn on 12.06.17.
 */
public class Neuron {
    private double[] weights;
    private double[] previousWeights;
    private double[] inputs;

    private Neuron[] neuronsInLayerAbove;
    private Neuron[] neuronsInLayerBelow;

    private double desiredValue;
    private double error;
    private double sigmoid;
    private double sum;
    private double output;
    private final int neuronLayer;
    private final int numberOfInputs;
    private final int numberInLayer;
    private static final double alpha = 0.003;
    private static final double momentum = 0.3;
    private static final boolean isMomentum = false;
    private static final double beta = 1;
    private double bias;
    private Boolean BIAS=true;

    public Neuron(int neuronLayer, int numberOfInputs, int thisNeuronsNumberInLayer) {
        this.neuronLayer = neuronLayer;
        this.numberOfInputs = numberOfInputs;
        this.numberInLayer = thisNeuronsNumberInLayer;
        this.inputs = new double[numberOfInputs];
        this.weights = new double[numberOfInputs];
        this.previousWeights = new double[numberOfInputs];
        for (int i = 0; i < numberOfInputs; i++) {
            weights[i] = new Random(System.nanoTime()).nextDouble() - .5;
        }
        this.bias = new Random(System.nanoTime()).nextDouble() - .5;

    }



    public void setInput(int inputNumber, double inputValue) {
        this.inputs[inputNumber] = inputValue;
    }

    public void setInputs(double[] inputs) {
        if (inputs.length == this.numberOfInputs) {
            this.inputs = inputs;
        }
    }

    public double getDesiredValue() {
        return desiredValue;
    }

    public void setDesiredValue(double desiredValue) {
        this.desiredValue = desiredValue;
    }


    public double getOutput() {
        return output;
    }

    public void setOutput(double output) {
        this.output = output;
    }

    public void setOutNeurons(Neuron[] neuronArray) {
        this.neuronsInLayerAbove = neuronArray;
    }

    public void setInNeurons(Neuron[] neuronArray) {
        this.neuronsInLayerBelow = neuronArray;
    }

    public void calculateOutput() {

        switch (neuronLayer) {
            case 0:
                calculateOutputForNeuronInInputLayer();
                break;
            case 1:
                calculateOutputForNeuronInHiddenLayer();
                break;
            case 2:
                calculateOutputForNeuronInOutputLayer();
                break;
        }
    }

    void calculateError() {

        switch (neuronLayer) {
            case 0:
                break;
            case 1:
                calculateErrorForNeuronInHiddenLayer();
                break;
            case 2:
                calculateErrorForNeuronInOutputLayer();
                break;
        }
    }

    void changeWeights() {
        double temp, temp2;
        for (int i = 0; i < weights.length; i++) {
            if (isMomentum) {
                temp = weights[i];
                weights[i] = temp + alpha * error * inputs[i] + momentum * (temp - previousWeights[i]);
                previousWeights[i] = temp;
            } else {
                weights[i] = weights[i] + alpha * error * inputs[i];
                weights[i] = weights[i] + alpha * error * inputs[i];
            }
        }
        if (BIAS) {
            bias = bias + alpha * error;
        }
    }

    public void calculateOutputForNeuronInInputLayer() {
        output = inputs[0];
    }

    public void calculateOutputForNeuronInHiddenLayer() {
        sum = 0;

        for (int i = 0; i < numberOfInputs; i++) {
            sum += inputs[i] * weights[i];
        }

        sigmoid = sigmoid(sum, beta);
        output = sigmoid;
    }

    public void calculateOutputForNeuronInOutputLayer() {
        sum = 0;

        for (int i = 0; i < numberOfInputs; i++) {
            sum += inputs[i] * weights[i];
        }

        output = sum;
    }

    private void calculateErrorForNeuronInHiddenLayer() {
        double outputLayerSum = 0;
        for (Neuron neuron : neuronsInLayerAbove) {
            outputLayerSum += neuron.getWeights()[numberInLayer] * neuron.getError();
        }

        error = sigmoidDerivative(output) * outputLayerSum;
    }

    private void calculateErrorForNeuronInOutputLayer() {
        error = desiredValue - output;
    }

    public double sigmoid(double x, double beta) {
        return 1 / (1 + Math.exp(-x * beta));
    }

    public double sigmoidDerivative(double sigm) {
        return sigm * (1 - sigm);
    }


    public double[] getWeights() {
        return weights;
    }

    public void setWeights(double[] weights) {
        this.weights = weights;
    }

    public double[] getPreviousWeights() {
        return previousWeights;
    }

    public void setPreviousWeights(double[] previousWeights) {
        this.previousWeights = previousWeights;
    }

    public double[] getInputs() {
        return inputs;
    }

    public Neuron[] getNeuronsInLayerAbove() {
        return neuronsInLayerAbove;
    }

    public void setNeuronsInLayerAbove(Neuron[] neuronsInLayerAbove) {
        this.neuronsInLayerAbove = neuronsInLayerAbove;
    }

    public Neuron[] getNeuronsInLayerBelow() {
        return neuronsInLayerBelow;
    }


    public void setNeuronsInLayerBelow(Neuron[] neuronsInLayerBelow) {
        this.neuronsInLayerBelow = neuronsInLayerBelow;
    }


    public double getError() {
        return error;
    }

    public void setError(double error) {
        this.error = error;
    }

    public double getSigmoid() {
        return sigmoid;
    }

    public void setSigmoid(double sigmoid) {
        this.sigmoid = sigmoid;
    }

    public double getSum() {
        return sum;
    }

    public void setSum(double sum) {
        this.sum = sum;
    }


    public int getNeuronLayer() {
        return neuronLayer;
    }

    public int getNumberOfInputs() {
        return numberOfInputs;
    }

    public int getNumberInLayer() {
        return numberInLayer;
    }

    public static double getAlpha() {
        return alpha;
    }

    public static double getMomentum() {
        return momentum;
    }

    public static boolean isMomentum() {
        return isMomentum;
    }

    public static double getBeta() {
        return beta;
    }

    public double getBias() {
        return bias;
    }

    public void setBias(double bias) {
        this.bias = bias;
    }

    public Boolean getBIAS() {
        return BIAS;
    }

    public void setBIAS(Boolean BIAS) {
        this.BIAS = BIAS;
    }
}
