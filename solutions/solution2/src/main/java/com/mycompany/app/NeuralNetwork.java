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
    private double[] outputValues;
    private int epochs;
    private int[] facePosition;
    private double[] testPoint;
    private Boolean flag;
    private ImageScale imageScale;

    public NeuralNetwork() {
        this.epochs = 1;
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

    public void learn(ImageScale imageScales, int[] facePosition, double[] testPoint) {
        this.imageScale=imageScales;
        flag=true;
        this.testPoint=testPoint;
        this.facePosition=facePosition;
        for (int i = 0; i < epochs; i++) {
            for (int j = 0; j < INPUT_LAYER_LENGTH; j++) {
                forwardPropagation(j);
                backPropagation(i);
            }
        }
        System.out.println(hiddenWeights[0][0]+ "    "+outputWeights[0][0]);
        Serializer.sendWeightsToFile(hiddenWeights, outputWeights);
    }

    public void test(ImageScale imageScales,String path, int[] facePosition) {
        this.imageScale=imageScales;
        flag=false;
        this.facePosition=facePosition;
        Serializer.readWeights(INPUT_LAYER_LENGTH, HIDDEN_LAYER_LENGTH, OUTPUT_LAYER_LENGTH);
        hiddenWeights = Serializer.hiddenWeights;
        outputWeights = Serializer.outputWeights;
        outputValues = new double[OUTPUT_LAYER_LENGTH];
        for (int j = 0; j < INPUT_LAYER_LENGTH; j++) {
            forwardPropagation(j);
        }
        for(int j = 0;j<OUTPUT_LAYER_LENGTH;j++) {
            if(j%2==0){
                outputValues[j] = network[2][j].getOutput()*imageScales.getScale1()+facePosition[0];
            }else{
                outputValues[j] = network[2][j].getOutput()*imageScales.getScale2()+facePosition[1];
            }
        }
        Serializer.sendOutput(path,outputValues);
    }


    public void forwardPropagation(int number) {
        double[] tmp = new double[INPUT_LAYER_LENGTH];
        double[] tmp2 = new double[HIDDEN_LAYER_LENGTH];
        for (int k = 0; k < INPUT_LAYER_LENGTH; k++) {
            for(int y=0;y<imageScale.getHeight2();y++){
                for(int x=0;x<imageScale.getWidth2();x++){
                    network[0][k].setInput(0, ((double)imageScale.getImageScaled2d()[y][x])/255);
                    network[0][k].calculateOutput();
                }
            }


            tmp[k]=network[0][k].getOutput();
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
                    network[2][k].setDesiredValue((testPoint[k] - facePosition[0])/imageScale.getScale1());
                else
                    network[2][k].setDesiredValue((testPoint[k] - facePosition[1])/imageScale.getScale2());
            }
            network[2][k].calculateOutput();
        }
    }

    public void backPropagation(int epoch) {
        for (int k = 0; k < OUTPUT_LAYER_LENGTH; k++) {
            network[2][k].calculateError();
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
    }
}
