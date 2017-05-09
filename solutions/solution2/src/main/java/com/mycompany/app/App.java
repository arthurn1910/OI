package com.mycompany.app;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        String openPath=args[1];
        String savePath=args[2];

	System.out.println(openPath);
	System.out.println(savePath);

        Solution.SolutionIO.Run(openPath,savePath);
    }
}
