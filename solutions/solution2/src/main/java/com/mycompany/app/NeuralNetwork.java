package com.mycompany.app;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;
/**
 * Created by arthurn on 12.06.17.
 */
public class NeuralNetwork {

    private int INPUT_LAYER_LENGTH=900;
    private final static int HIDDEN_LAYER_LENGTH = 200;
    private final static int OUTPUT_LAYER_LENGTH = 40;
    private Neuron network[][];
    private double[][] hiddenWeights;
    private double[][] outputWeights;
    private double[] errors;
    private double[] inputPoints;
    private double[] outputPoints;
    private double[] outputValues;
    private int epochs;
    private int[][] data2d;
    private int[] facePosition;
    private double[] testPoint;
    private Boolean flag;
    private int countX;
    private int countY;

    public NeuralNetwork() {
        this.epochs = 1;
        errors = new double[1];
        network = new Neuron[3][];
        network[0] = new Neuron[INPUT_LAYER_LENGTH];
        network[1] = new Neuron[HIDDEN_LAYER_LENGTH];
        network[2] = new Neuron[OUTPUT_LAYER_LENGTH];
        hiddenWeights = new double[HIDDEN_LAYER_LENGTH][INPUT_LAYER_LENGTH];
        outputWeights = new double[OUTPUT_LAYER_LENGTH][HIDDEN_LAYER_LENGTH];
        for (int i = 0; i < INPUT_LAYER_LENGTH; i++) {
            network[0][i] = new Neuron(0, INPUT_LAYER_LENGTH, i);
        }
        for (int i = 0; i < HIDDEN_LAYER_LENGTH; i++) {
            network[1][i] = new Neuron(1, INPUT_LAYER_LENGTH, i);
            network[1][i].setInNeurons(network[0]);
        }
        for (int i = 0; i < OUTPUT_LAYER_LENGTH; i++) {
            network[2][i] = new Neuron(2, HIDDEN_LAYER_LENGTH, i);
            network[2][i].setInNeurons(network[1]);
        }

        for (int i = 0; i < HIDDEN_LAYER_LENGTH; i++) {
            network[1][i].setOutNeurons(network[2]);
        }
    }

    public void learn(int[][] data2d, int[] facePosition, double[] testPoint) {
        flag=true;
        this.testPoint=testPoint;
        this.data2d=data2d;
        this.facePosition=facePosition;
        inputPoints = new double[INPUT_LAYER_LENGTH];
        outputPoints = new double[HIDDEN_LAYER_LENGTH ];
        outputValues = new double[OUTPUT_LAYER_LENGTH];
//        double[] errorTmp2 = new double[INPUT_LAYER_LENGTH];
        for (int i = 0; i < epochs; i++) {
//            double sumError=0;
            for (int j = 0; j < INPUT_LAYER_LENGTH; j++) {
                forwardPropagation(j);
                //errorTmp2[j] =
                        Functions.meanSquaredError(backPropagation(i));
//                if (i == epochs - 1) {
//                    inputPoints[j] = values.get(j).getX();
//                    outputPoints[j] = values.get(j).getY();
//                }
            }
//            for(int z=0;z<OUTPUT_LAYER_LENGTH;z++){
//                outputValues[z] = network[2][z].getOutput();
//            }
//            for (double value : errorTmp2) {
//                sumError += value;
//            }
//            errors[i] = sumError / OUTPUT_LAYER_LENGTH;
        }
//        Serializer.sendOutput(outputValues);
        Serializer.sendWeightsToFile(hiddenWeights, outputWeights);
    }

    public void test(String path,int[][] data2d, int[] facePosition) {
        flag=false;
        this.data2d=data2d;
        this.facePosition=facePosition;
        Serializer.readWeights(INPUT_LAYER_LENGTH, HIDDEN_LAYER_LENGTH, OUTPUT_LAYER_LENGTH);
        hiddenWeights = Serializer.hiddenWeights;
        outputWeights = Serializer.outputWeights;
        inputPoints = new double[INPUT_LAYER_LENGTH];
        outputPoints = new double[HIDDEN_LAYER_LENGTH];
        outputValues = new double[OUTPUT_LAYER_LENGTH];
        for (int j = 0; j < INPUT_LAYER_LENGTH; j++) {
            forwardPropagation(j);
        }
        int tmp;
        for(int j = 0;j<OUTPUT_LAYER_LENGTH;j++) {
            if(j%2==0){
                outputValues[j] = network[2][j].getOutput()*3+facePosition[0];
            }else{
                outputValues[j] = network[2][j].getOutput()*3+facePosition[1];
            }
        }
        Serializer.sendOutput(path,outputValues);
    }


    public void forwardPropagation(int number) {
        double[] tmp = new double[INPUT_LAYER_LENGTH];
        double[] tmp2 = new double[HIDDEN_LAYER_LENGTH];
        int count0=facePosition[0], count2=facePosition[1]-4;
        countX= (int) Math.floor(facePosition[2]/Math.sqrt(INPUT_LAYER_LENGTH));
        countY=(int) Math.floor((facePosition[3]+8)/Math.sqrt(INPUT_LAYER_LENGTH));
        if(count2<0)
            count2=0;
        for (int k = 0; k < INPUT_LAYER_LENGTH; k++) {
            if(count0>=(facePosition[0]+facePosition[2])){
                count2+=countY;
                count0=facePosition[0];
            }
            network[0][k].setInput(0, ((double)data2d[count2][count0])/255);
            network[0][k].calculateOutput();

            tmp[k]=network[0][k].getOutput();
            count0+=countX;
        }

        for (int k = 0; k < HIDDEN_LAYER_LENGTH; k++) {
            network[1][k].setInputs(tmp);
            network[1][k].calculateOutput();
            tmp2[k] = network[1][k].getOutput();
        }

        for (int k = 0; k < OUTPUT_LAYER_LENGTH; k++) {
            network[2][k].setInputs(tmp2);
            if(flag) {
                if (k % 2 == 0)
                    network[2][k].setDesiredValue((testPoint[k] - facePosition[0])/countX);
                else
                    network[2][k].setDesiredValue((testPoint[k] - facePosition[1])/countY);
            }
            network[2][k].calculateOutput();
        }
    }

    public double[] backPropagation(int epoch) {
        double[] errorTmp = new double[OUTPUT_LAYER_LENGTH];

        for (int k = 0; k < OUTPUT_LAYER_LENGTH; k++) {
            network[2][k].calculateError();
            errorTmp[k] = network[2][k].getError();
        }

        for (int k = 0; k < HIDDEN_LAYER_LENGTH; k++) {
            network[1][k].calculateError();
        }

        for (int k = 0; k < HIDDEN_LAYER_LENGTH; k++) {
            network[1][k].changeWeights();
            if (epoch == epochs - 1) {
                hiddenWeights[k] = network[1][k].getWeights();
            }
        }

        for (int k = 0; k < OUTPUT_LAYER_LENGTH; k++) {
            network[2][k].changeWeights();
            if (epoch == epochs - 1) {
                outputWeights[k] = network[2][k].getWeights();
            }
        }
        return errorTmp;
    }

    public double[] getErrors() {
        return errors;
    }
//
//    public Point[] getInputPoints() {
//        return inputPoints;
//    }
//
//    public Point[] getOutputPoints() {
//        return outputPoints;
//    }
//
//    public Point[] getOutputValues() {
//        return outputValues;
//    }
}
