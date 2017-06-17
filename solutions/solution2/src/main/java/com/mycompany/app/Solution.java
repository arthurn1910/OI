package com.mycompany.app;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.awt.image.WritableRaster;
import java.io.*;
import java.util.Scanner;

/**
 * Created by arthurn on 07.05.17.
 */
public class Solution {
    public static class SolutionIO{
        private static BufferedImage image;
        public static void Run(String pathOpen, String pathSave, String opencvPath, String testPts){
            String pathFile;
            String pathFileSave;
            String pathFacePosition;
            String pathTestPoint=null;
            NeuralNetwork nn = new NeuralNetwork();

            for(int i=0; i<=1520; i++) {
                if(i<10){
                    pathFile=pathOpen+"BioID_000"+i+".pgm";
                    pathFileSave=pathSave+"/bioid_000"+i+".pts";
                    pathFacePosition=opencvPath+"/train/bioid_000"+i+".opencv";
                    pathTestPoint=testPts+"/bioid_000"+i+".pts";
                } else if(i<100){
                    pathFile=pathOpen+"BioID_00"+i+".pgm";
                    pathFileSave=pathSave+"/bioid_00"+i+".pts";
                    pathFacePosition=opencvPath+"/train/bioid_00"+i+".opencv";
                    pathTestPoint=testPts+"/bioid_00"+i+".pts";
                } else if(i<913){
                    pathFile=pathOpen+"BioID_0"+i+".pgm";
                    pathFileSave=pathSave+"/bioid_0"+i+".pts";
                    pathFacePosition=opencvPath+"/train/bioid_0"+i+".opencv";
                    pathTestPoint=testPts+"/bioid_0"+i+".pts";
                }else if(i<1000){
                    pathFile=pathOpen+"BioID_0"+i+".pgm";
                    pathFileSave=pathSave+"/bioid_0"+i+".pts";
                    pathFacePosition=opencvPath+"/test/bioid_0"+i+".opencv";
                } else {
                    pathFile = pathOpen + "BioID_" + i + ".pgm";
                    pathFileSave=pathSave+"bioid_"+i+".pts";
                    pathFacePosition=opencvPath+"/train/bioid_"+i+".opencv";
                }
                int[] facePosition=readFacePosition(pathFacePosition);
                int[][] data2d = openPicture(pathFile);
                //data2d[108][159]=255;
                if(i<913){
                    //if(i==10)
                        System.out.println(i);
                        double[] testPoint = readTestPoint(pathTestPoint);
                        nn.learn(data2d, facePosition, testPoint);

                }else{
                    System.out.println("Test "+i);
                    nn.test(pathFileSave,data2d,facePosition);
                    //double [] positionnn=nn.test(data2d,facePosition);
                }
                //savePicture(data2d);


//                int[] nostril=detectNostril(data2d);
//                int[] nose = detectTipOfNose(nostril);
//                int[] eyes = detectEyes(data2d,nose,nostril);
//                int[] temples=detectTemples(eyes);
//                int[] brows = detectBrows(eyes,temples);
//                int[] mouth = detectMouth(eyes,nose, nostril);
//                int[] chin=detectChin(nose,mouth);

                    //savePicture(data2d,nostril, nose, eyes, temples, brows, mouth, chin);
//                saveResult(pathFileSave,data2d,nostril, nose, eyes, temples, brows, mouth, chin);

            }



        }

        private static double[] readTestPoint(String path){
            double[] points=new double[40];
            FileInputStream fileInputStream = null;
            double x;
            try {
                fileInputStream = new FileInputStream(path);
                Scanner scan = new Scanner(fileInputStream);
                scan.next();
                scan.next();
                scan.next();
                scan.next();
                scan.next();
                for(int i=0;i<40;i++) {
                    x=scan.nextDouble();
                    points[i] = x;
                }
                return  points;
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
            return null;
        }

        private static int[] readFacePosition(String pathFacePosition) {
            int[] facePosition=new int[4];
            FileInputStream fileInputStream = null;
            try {
                fileInputStream = new FileInputStream(pathFacePosition);
                Scanner scan = new Scanner(fileInputStream);
                for(int i=0;i<4;i++)
                    facePosition[i]=scan.nextInt();
                return  facePosition;
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
           return null;

        }

        public static int[] detectEyes(int[][] data, int[] nose, int[] nostril){
            double hight=nostril[2] - nostril[0];
            int[] eyes=new int[12];
            double x=(7/4)*hight;
            double y=(10/6)*hight;
            int x_i=(int) x;
            int y_i=(int) y;
            eyes[0]=nose[0]-x_i;
            eyes[1]=nose[1]-y_i;
            eyes[2]=nose[0]+x_i;
            eyes[3]=nose[1]-y_i;

            for(int r=eyes[1]-5; r<eyes[1]+5;r++)
                if(r>=data.length || r<0){
                    continue;
                } else {
                    for (int z = eyes[0] - 10; z < eyes[0]; z++) {
                        if (z >= data.length || z < 0) {
                            continue;
                        } else if (data[r][z] < 100) {
                            eyes[0] = z;
                            eyes[1] = r;
                        }
                    }
                }

            for(int t=eyes[3]-7; t<eyes[3]+7;t++)
                if(t>data.length || t<0){
                    continue;
                } else {
                    for (int z = eyes[2]; z < eyes[2] + 10; z++) {
                        if (z > data[0].length || z < 0) {
                            continue;
                        } else if (data[t][z] < 100) {
                            eyes[8] = z;
                            eyes[9] = t;
                        }
                    }
                }

                for(int z=eyes[0]+10; z<eyes[0]+15;z++) {
                    if (z > data[0].length || z < 0 || eyes[1]<0 || eyes[1]>data[0].length) {
                        continue;
                    } else if (data[eyes[1]][z] < 100) {
                        eyes[2] = z;
                        eyes[3] = eyes[1];
                    }
                }
            eyes[4]=eyes[2]+(eyes[2]-eyes[0]);
            eyes[5]=eyes[3];

                eyes[6] = eyes[8]-(eyes[2]-eyes[0]);
                eyes[7] = eyes[9];
            eyes[10] = eyes[8]+(eyes[2]-eyes[0]);
            eyes[11] = eyes[9];



            return eyes;
        }

        public static int[] detectTemples(int [] eyes){

            int[] temples=new int[4];
            temples[0]=eyes[0]-(eyes[2]-eyes[0]);
            temples[1]=eyes[1];
            temples[2]=eyes[10]+(eyes[8]-eyes[6]);
            temples[3]=eyes[11];
            return temples;

        }

        public static int[] detectMouth( int[] eyes, int[] nose, int[] nostril){
            int[] mouth=new int[8];

            mouth[2]=nose[0];
            mouth[3]=nose[1] -(eyes[1]-nose[1]) - (nose[1]-nostril[1]);
            mouth[4]=nose[0];
            mouth[5]=mouth[3] -(nostril[0]-nose[0]) - (nose[1]-nostril[1]);
            mouth[0]=mouth[2]-(eyes[4] - eyes[0]);
            mouth[1]=(mouth[3] + mouth[5])/2;
            mouth[6]=mouth[2]+(eyes[4] - eyes[0]);
            mouth[7]=(mouth[3] + mouth[5])/2;
            return mouth;

        }

        public static int[] detectChin(int[] nose, int[] mouth){
            int[] chin=new int[2];
            chin[0]=nose[0];
            chin[1]=mouth[5] + (mouth[3]-nose[1]);
            return chin;

        }

        public static int[] detectBrows(int [] eyes, int[] temples){

            int[] brows=new int[8];
            brows[0]=temples[0];
            brows[1]=eyes[1]-(eyes[2]-eyes[0]);
            brows[2]=eyes[4];
            brows[3]=eyes[1]-(eyes[2]-eyes[0]);
            brows[4]=eyes[6];
            brows[5]=eyes[7]-(eyes[2]-eyes[0]);
            brows[6]=temples[2];
            brows[7]=eyes[7]-(eyes[2]-eyes[0]);
            return brows;

        }

        public static int[] detectNostril(int[][] data){
            int center_y=data.length/2;
            int center_x=data[0].length/2;


            int[] nostril=new int[4];
            int number=0;




            for(int y=center_y;y<center_y+15;y++){
                for(int x=center_x-15;x<center_x+25;x++){

                    if(data[x][y]<160 && data[x][y+1]<170){
                        if((number > 0 & nostril[0]<x+15) || number==0){
                            nostril[number] = x;
                            nostril[number + 1] = y;
                            number += 2;
                            x += 15;
                        }

                    }
                    if(number>=3)
                        break;
                }
                if(number>=3)
                    break;
            }

            return nostril;

        }

        public static int[] detectTipOfNose(int[] nostril){
            int[] nose=new int[2];

            nose[0]=(nostril[0]+nostril[2])/2;
            nose[1]=(nostril[1]+nostril[3])/2-4;

            return nose;

        }
        
        public static int[][] openPicture(String path){
            try{
            FileInputStream fileInputStream = new FileInputStream(path);
            Scanner scan = new Scanner(fileInputStream);
            scan.nextLine();
            int picWidth = scan.nextInt();
            int picHeight = scan.nextInt();
                scan.nextLine();

            fileInputStream.close();

            fileInputStream = new FileInputStream(path);
            DataInputStream dis = new DataInputStream(fileInputStream);

            int numnewlines = 4;
            while (numnewlines > 0) {
                char c;
                do {
                    c = (char)(dis.readUnsignedByte());
                } while (c != '\n');
                numnewlines--;
            }

            // read the image data
            int[][] data2D = new int[picHeight][picWidth];
            for (int row = 0; row < picHeight; row++) {
                for (int col = 0; col < picWidth; col++) {
                    data2D[row][col] = dis.readUnsignedByte();
                }
            }
                return data2D;

            } catch(Exception ex){
                System.out.println("ERROR read picture "+path);
            }

            return null;
        }

        public static void saveResult(String path, int[][] data, int[] nostril, int[] nose, int[] eyesCorner, int[] temples, int[] brows, int[] mouth,
                                       int[] chin){

            Writer writer = null;
            try {
                writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(path), "utf-8"));
                writer.write("version: 1\n");
                writer.write("n_points: 20\n");
                writer.write("{\n");
                writer.write(eyesCorner[2]+" "+eyesCorner[3]+"\n");
                writer.write(eyesCorner[8]+" "+eyesCorner[9]+"\n");
                writer.write(mouth[0]+" "+mouth[1]+"\n");
                writer.write(mouth[6]+" "+mouth[7]+"\n");
                writer.write(brows[0]+" "+brows[1]+"\n");
                writer.write(brows[2]+" "+brows[3]+"\n");
                writer.write(brows[4]+" "+brows[5]+"\n");
                writer.write(brows[6]+" "+brows[7]+"\n");
                writer.write(temples[0]+" "+temples[1]+"\n");
                writer.write(eyesCorner[0]+" "+eyesCorner[1]+"\n");
                writer.write(eyesCorner[4]+" "+eyesCorner[5]+"\n");
                writer.write(eyesCorner[6]+" "+eyesCorner[7]+"\n");
                writer.write(eyesCorner[10]+" "+eyesCorner[11]+"\n");
                writer.write(temples[2]+" "+temples[3]+"\n");
                writer.write(nose[0]+" "+nose[1]+"\n");
                writer.write(nostril[0]+" "+nostril[1]+"\n");
                writer.write(nostril[2]+" "+nostril[3]+"\n");
                writer.write(mouth[2]+" "+mouth[3]+"\n");
                writer.write(mouth[4]+" "+mouth[5]+"\n");
                writer.write(chin[0]+" "+chin[1]+"\n");
                writer.write("}");

            } catch (Exception e) {
                e.printStackTrace();
            }finally {
                try {
                    writer.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

        }

        public static void savePicture(int[][] data) {
            int number=0;
            int[] data4=new int[data[0].length*data.length];
            for (int y = 0; y < data.length; ++y) {
                for (int x = 0; x < data[0].length; ++x) {
                    data4[number]=data[y][x];
                    number++;
                }
            }


            BufferedImage outputImage = new BufferedImage(data[0].length, data.length, BufferedImage.TYPE_BYTE_GRAY);
            WritableRaster raster = outputImage.getRaster();
            raster.setSamples(0, 0, data[0].length, data.length, 0, data4);
            try {
                ImageIO.write(outputImage, "png", new File("p6.png"));
            } catch (IOException e) {
                e.printStackTrace();
            }


        }

        public static ImageScale scaleImage(int [][] image, int[] facePosition){
            ImageScale imageScale=new ImageScale();
            int add=0;
            if(image.length>= (facePosition[1]+facePosition[3]+8)){
                add=8;
            }else if( image.length>= (facePosition[1]+facePosition[3]+4)){
                add=4;
            }else if( image.length>= (facePosition[1]+facePosition[3]+2)){
                add=2;
            }

            int[] image2=new int[(facePosition[3]+add)*facePosition[2]];
            int k=0;
            for(int y=facePosition[1];y<(facePosition[3]+add);y++){
                for(int x=facePosition[0];x<(facePosition[2]);x++){
                    image2[k]=image[y][x];
                    k++;
                }
            }
            imageScale.setFacePosition(facePosition);
            imageScale.setData2d(image);
            imageScale.setHeight1(facePosition[3]+add);
            imageScale.setHeight2(30);
            imageScale.setWidth1(facePosition[2]);
            imageScale.setWidth2(30);
            imageScale.setImage(image2);

            resizePixels(imageScale);
            imageScale.setImageScaled2d();

            return imageScale;
        }

        public static ImageScale resizePixels(ImageScale imageScale){//int[] pixels,int w1,int h1,int w2,int h2) {
            int[] temp = new int[imageScale.getHeight2()*imageScale.getWidth2()] ;
            double x_ratio = imageScale.getWidth1()/(double)imageScale.getScale2() ;
            double y_ratio = imageScale.getHeight1()/(double)imageScale.getHeight2() ;
            double px, py ;
            for (int i=0;i<imageScale.getHeight2();i++) {
                for (int j=0;j<imageScale.getWidth2();j++) {
                    px = Math.floor(j*x_ratio) ;
                    py = Math.floor(i*y_ratio) ;
                    temp[(i*imageScale.getWidth2())+j] = imageScale.getImage()[(int)((py*imageScale.getWidth1())+px)] ;
                }
            }
            imageScale.setScale1(x_ratio);
            imageScale.setScale2(y_ratio);
            imageScale.setImageScaled(temp);
            return imageScale ;
        }

    }
}
