using System.Runtime.InteropServices;
using System.Diagnostics;
using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Threading.Tasks;
using System.Threading;
using System.Collections.Generic;

namespace AssemblerProject
{  
    public enum DllType {CPP, ASM}
    public class ProcessingManager
    {
        [DllImport(@"D:\Studia\Projects\AssemblerProject\x64\Debug\JAAsm.dll")]
        static extern void burkesDitheringAsm(IntPtr inputImage, IntPtr outputImage, int width, int height, int startRow, int endRow);

        [DllImport(@"D:\Studia\Projects\AssemblerProject\x64\Debug\JACpp.dll")]
        static extern void burkesDitheringCpp(IntPtr inputImage, IntPtr outputImage, int width, int height, int startRow, int endRow);

        private TimeSpan currentExecutionTime;
        private TimeSpan previousExecutionTime;
        public double currentExecutionMs { get; private set; }
        public double previousExecutionMs { get; private set; }

        Stopwatch stopwatch;
        public Bitmap loadedBitmap { get; private set; }

        int bitmapSize;
        int numberOfThreads;

        public ProcessingManager()
        {
            currentExecutionMs = 0;
            previousExecutionMs = 0;
            stopwatch = new Stopwatch();
            loadedBitmap = null;
        }
        public void LoadBitmap(Bitmap bitmap, int numberOfThreads)
        {
            bitmapSize = bitmap.Width * bitmap.Height * 3;
            this.numberOfThreads = numberOfThreads;
            loadedBitmap = bitmap;// ConvertToGrayscaleFast(bitmap);
        }

        public Bitmap startProcessingImage(DllType dllType)
        {
            if (loadedBitmap == null) return null;
            Bitmap result = new Bitmap(loadedBitmap.Width, loadedBitmap.Height);
            stopwatch.Reset();

            switch (dllType)
            {
                case DllType.CPP:
                    stopwatch.Start();
                    ProcessUsingCpp(loadedBitmap, result);
                    stopwatch.Stop();
                    break;
                case DllType.ASM:
                    stopwatch.Start();
                    ProcessUsingAsm(loadedBitmap, result);
                    stopwatch.Stop();
                    break;
            }
           
            if (currentExecutionMs != 0)
                previousExecutionMs = currentExecutionMs;

            currentExecutionMs = stopwatch.Elapsed.TotalMilliseconds;

            return result;
        }

        private void ProcessUsingCpp(Bitmap inputBitmap, Bitmap outputBitmap)
        {
            BitmapData inputData = inputBitmap.LockBits(new Rectangle(0, 0, inputBitmap.Width, inputBitmap.Height), ImageLockMode.ReadWrite, PixelFormat.Format8bppIndexed);
            
            BitmapData outputData = outputBitmap.LockBits(new Rectangle(0, 0, outputBitmap.Width, outputBitmap.Height), ImageLockMode.ReadWrite, PixelFormat.Format8bppIndexed);

            int stride = inputData.Stride;
            int height = inputBitmap.Height;
            int width = inputBitmap.Width;

            // Calculate the number of rows each thread will process
            int rowsPerThread = height / numberOfThreads;

            // Create an array to hold threads
            List<Task> tasks = new List<Task>();

            IntPtr inputPtr, outputPtr;

            unsafe
            {
                inputPtr = inputData.Scan0;
                outputPtr = outputData.Scan0;
            }

            for (int i = 0; i < numberOfThreads; i++)
            {
                int startRow = i * rowsPerThread;
                int endRow = (i == numberOfThreads - 1) ? height : (i + 1) * rowsPerThread;

                tasks.Add(Task.Run(() =>
                {
                    burkesDithering(inputPtr, outputPtr, stride, width, startRow, endRow);
                }));
            }

            Task.WaitAll(tasks.ToArray());

            inputBitmap.UnlockBits(inputData);
            outputBitmap.UnlockBits(outputData);
        }
        
        private void ProcessUsingAsm(Bitmap inputBitmap, Bitmap outputBitmap)
        {
            BitmapData inputData = inputBitmap.LockBits(new Rectangle(0, 0, inputBitmap.Width, inputBitmap.Height), ImageLockMode.ReadWrite, PixelFormat.Format8bppIndexed);
            
            BitmapData outputData = outputBitmap.LockBits(new Rectangle(0, 0, outputBitmap.Width, outputBitmap.Height), ImageLockMode.ReadWrite, PixelFormat.Format8bppIndexed);

            int stride = inputData.Stride;
            int height = inputBitmap.Height;
            int width = inputBitmap.Width;

            int rowsPerThread = height / numberOfThreads;

            List<Task> tasks = new();

            IntPtr inputPtr, outputPtr;

            unsafe
            {
                inputPtr = inputData.Scan0;
                outputPtr = outputData.Scan0;
            }

            for (int i = 0; i < numberOfThreads; i++)
            {
                int startRow = i * rowsPerThread;
                int endRow = (i == numberOfThreads - 1) ? height : (i + 1) * rowsPerThread;

                tasks.Add(Task.Run(() =>
                {
                    //MyProc1(inputPtr, outputPtr, stride, width, startRow, endRow);
                }));
            }

            Task.WaitAll(tasks.ToArray());

            inputBitmap.UnlockBits(inputData);
            outputBitmap.UnlockBits(outputData);
        }


        public static Bitmap ConvertToGrayscaleFast(Bitmap original)
        {
            Bitmap grayscale = new Bitmap(original.Width, original.Height);

            Rectangle rect = new Rectangle(0, 0, original.Width, original.Height);
            BitmapData originalData = original.LockBits(rect, ImageLockMode.ReadOnly, PixelFormat.Format24bppRgb);
            BitmapData grayscaleData = grayscale.LockBits(rect, ImageLockMode.WriteOnly, PixelFormat.Format24bppRgb);

            unsafe
            {
                byte* originalPtr = (byte*)originalData.Scan0.ToPointer();
                byte* grayscalePtr = (byte*)grayscaleData.Scan0.ToPointer();

                int bytesPerPixel = 3; // Dla Format24bppRgb
                int width = original.Width;
                int height = original.Height;

                for (int y = 0; y < height; y++)
                {
                    byte* originalRow = originalPtr + (y * originalData.Stride);
                    byte* grayscaleRow = grayscalePtr + (y * grayscaleData.Stride);

                    for (int x = 0; x < width; x++)
                    {
                        byte grayValue = (byte)((originalRow[x * bytesPerPixel] + originalRow[x * bytesPerPixel + 1] + originalRow[x * bytesPerPixel + 2]) / 3);

                        grayscaleRow[x * bytesPerPixel] = grayValue;
                        grayscaleRow[x * bytesPerPixel + 1] = grayValue;
                        grayscaleRow[x * bytesPerPixel + 2] = grayValue;
                    }
                }
            }

            original.UnlockBits(originalData);
            grayscale.UnlockBits(grayscaleData);

            return grayscale;
        }
    }
}
