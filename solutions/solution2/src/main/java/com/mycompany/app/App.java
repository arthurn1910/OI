package com.mycompany.app;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        //String openPath=args[1];
        //String savePath=args[2];
        String openPath="/media/sf_sf/a/";//args[1];
        String savePath="/home/arthurn/wyniki";//args[2];
        String opencvPath="/media/sf_sf/solution2/data/OpenCV_addnotations";//args[3]
        String testPts="/media/sf_sf/solution2/data/points_20";

        Solution.SolutionIO.Run(openPath,savePath,opencvPath,testPts);
    }
}
