package com.mycompany.app;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;
/**
 * Created by arthurn on 12.06.17.
 */
public class Serializer {
    public static double[][] hiddenWeights;
    public static double[][] outputWeights;

    public static void sendWeightsToFile(double hiddenWeights[][], double outputWeights[][]) {
        PrintWriter writer;
        try {
            writer = new PrintWriter("data/weights.txt");
            writer.println("Hidden weights");
            for (int i = 0; i < hiddenWeights[0].length; i++) {
                for (int j = 0; j < hiddenWeights.length; j++) {
                    writer.print(hiddenWeights[j][i] + " ");
                    writer.println();
                }

            }
            writer.println("Output weights");
            for (int i = 0; i < outputWeights[0].length; i++) {
                for (int j = 0; j < outputWeights.length; j++) {
                    writer.print(outputWeights[j][i] + " ");
                }
            }
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void readWeights(int INPUT_LAYER_LENGTH, int HIDDEN_LAYER_LENGTH, int OUTPUT_LAYER_LENGTH) {
        hiddenWeights = new double[HIDDEN_LAYER_LENGTH][INPUT_LAYER_LENGTH];
        outputWeights = new double[OUTPUT_LAYER_LENGTH][HIDDEN_LAYER_LENGTH];
        String valuesLine;
        String[] stringValues;

        File file = new File("data/weights.txt");
        try (Scanner inFile = new Scanner(file)) {
            while (inFile.hasNext()) {
                inFile.nextLine();
                //System.out.println(hiddenWeights.length);
                for (int i = 0; i < hiddenWeights.length; i++) {
                    for (int j = 0; j < hiddenWeights[0].length; j++) {
                        hiddenWeights[i][j] = inFile.nextDouble();
                        //System.out.println(hiddenWeights[i][j]);
                    }
                }
                inFile.nextLine();
                inFile.nextLine();
                valuesLine = inFile.nextLine();
                stringValues = valuesLine.split(" ");
                for (int i = 0; i < outputWeights.length; i++) {
                    for (int j = 0; j < outputWeights[0].length; j++) {
                        outputWeights[i][j] = Double.parseDouble(stringValues[j]);
                        // System.out.println(outputWeights[i][j]);
                    }
                }
            }
            inFile.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
    public static void sendOutput(String path,double output[]){
        PrintWriter writer;
        try {
            writer = new PrintWriter(path);
            writer.write("version: 1\n");
            writer.write("n_points: 20\n");
            writer.write("{\n");
            for (int i = 0; i < output.length; i+=+2) {
                writer.write(output[i]+" "+output[i+1]+"\n");
            }
            writer.write("}");
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}