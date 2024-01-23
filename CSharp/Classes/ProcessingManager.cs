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
        [DllImport(@"D:\Studia\Projects\AssemblerProject\x64\Debug\JAAsm.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void burkesDitheringAsm(IntPtr inputImage, IntPtr outputImage, int width, int height, int startRow, int endRow);

        [DllImport(@"D:\Studia\Projects\AssemblerProject\x64\Debug\JACpp.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void burkesDitheringCpp(IntPtr inputImage, IntPtr outputImage, int width, int height, int startRow, int endRow);
        public double currentExecutionMs { get; private set; }
        public double previousExecutionMs { get; private set; }

        Stopwatch stopwatch;
        public Bitmap loadedBitmap { get; private set; }

        int bitmapSize;
        int numberOfSegments;

        public ProcessingManager()
        {
            currentExecutionMs = 0;
            previousExecutionMs = 0;
            stopwatch = new Stopwatch();
            loadedBitmap = null;
        }
        public void LoadBitmap(Bitmap bitmap)
        {
            bitmapSize = bitmap.Width * bitmap.Height * 3;
            loadedBitmap = new Bitmap(bitmap);
        }

        public Bitmap startProcessingImage(DllType dllType, int numberOfThreads)
        {
            if (loadedBitmap == null) return null;
            Bitmap result = new Bitmap(loadedBitmap.Width, loadedBitmap.Height);
            stopwatch.Reset();

            numberOfSegments = numberOfThreads;
            
            ProcessImage(loadedBitmap, result, dllType);

            if (currentExecutionMs != 0)
                previousExecutionMs = currentExecutionMs;

            currentExecutionMs = stopwatch.Elapsed.TotalMilliseconds;

            return result;
        }

        private void ProcessImage(Bitmap inputBitmap, Bitmap outputBitmap, DllType dllType)
        {
            Bitmap inputCopy = new Bitmap(inputBitmap);
            BitmapData inputData = inputCopy.LockBits(new Rectangle(
                0, 0, inputCopy.Width, inputCopy.Height), 
                ImageLockMode.ReadWrite, PixelFormat.Format8bppIndexed);
            BitmapData outputData = outputBitmap.LockBits(new Rectangle(
                0, 0, outputBitmap.Width, outputBitmap.Height), 
                ImageLockMode.ReadWrite, PixelFormat.Format8bppIndexed);

            int height = inputCopy.Height;
            int width = inputCopy.Width;
            int rowsPerThread = height / numberOfSegments;
            List<Task> tasks = new();

            IntPtr inputPtr, outputPtr;
            stopwatch.Start();
            unsafe { inputPtr = inputData.Scan0; outputPtr = outputData.Scan0; }

            for (int i = 0; i < numberOfSegments; i++)
            {
                int startRow = i * rowsPerThread;
                int endRow = (i == numberOfSegments - 1) ? 
                    height : (i + 1) * rowsPerThread;
                tasks.Add(Task.Run(() =>
                {
                    switch (dllType)
                    {
                        case DllType.CPP:
                            burkesDitheringCpp(inputPtr, outputPtr, 
                                width, height, startRow, endRow);
                            break;
                        case DllType.ASM:
                            burkesDitheringAsm(inputPtr, outputPtr, 
                                width, height, startRow, endRow);
                            break;
                    }
                }));
            }
            Task.WaitAll(tasks.ToArray());
            stopwatch.Stop();
            inputCopy.UnlockBits(inputData);
            outputBitmap.UnlockBits(outputData);
        }
        
        public static Bitmap ConvertToGrayscaleFast(Bitmap original)
        {
            Bitmap grayscale = new(original.Width, original.Height);

            Rectangle rect = new(0, 0, original.Width, original.Height);
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
