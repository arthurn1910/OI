package com.mycompany.app;

/**
 * Created by arthurn on 17.06.17.
 */
public class ImageScale {
    private double scale1;
    private double scale2;
    private int[][] data2d;
    private int[] image;
    private int[] imageScaled;
    private int[][] imageScaled2d;
    private int[] facePosition;
    private int width1;
    private int width2;
    private int height1;
    private int height2;


    public void setFacePosition(int[] facePosition) {
        this.facePosition = facePosition;
    }

    public double getScale1() {
        return scale1;
    }

    public void setScale1(double scale1) {
        this.scale1 = scale1;
    }

    public double getScale2() {
        return scale2;
    }

    public void setScale2(double scale2) {
        this.scale2 = scale2;
    }


    public void setData2d(int[][] data2d) {
        this.data2d = data2d;
    }

    public int[] getImage() {
        return image;
    }

    public void setImage(int[] image) {
        this.image = image;
    }


    public void setImageScaled(int[] imageScaled) {
        this.imageScaled = imageScaled;
    }

    public int getWidth1() {
        return width1;
    }

    public void setWidth1(int width1) {
        this.width1 = width1;
    }

    public int getWidth2() {
        return width2;
    }

    public void setWidth2(int width2) {
        this.width2 = width2;
    }

    public int getHeight1() {
        return height1;
    }

    public void setHeight1(int height1) {
        this.height1 = height1;
    }

    public int getHeight2() {
        return height2;
    }

    public void setHeight2(int height2) {
        this.height2 = height2;
    }

    public int[][] getImageScaled2d() {
        return imageScaled2d;
    }

    public void setImageScaled2d() {
        int k=0;
        int[][] image=new int[this.getHeight2()][this.getWidth2()];
        for(int y=0;y<this.getHeight2();y++){
            for(int x=0;x<this.getWidth2();x++){
                image[y][x]=this.imageScaled[k];
                k++;
            }
        }
        this.imageScaled2d = image;
    }
}
