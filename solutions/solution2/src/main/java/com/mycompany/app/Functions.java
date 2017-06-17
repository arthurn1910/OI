package com.mycompany.app;

/**
 * Created by arthurn on 12.06.17.
 */
public class Functions {
    public static double[] countError(double[] out, double[] result ){
        double[] error = new double[out.length];
        for(int i=0;i<out.length;i++)
            error[i]=result[i]-out[i];
        return error;
    }

    public static double meanSquaredError(double[] tab){
        double sum=0;
        for(int i=0;i<tab.length;i++) {
            sum += Math.pow(tab[i], 2);
        }
        sum=sum/tab.length;
        return sum;
    }
}
