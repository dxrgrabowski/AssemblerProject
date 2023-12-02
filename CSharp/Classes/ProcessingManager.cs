using System.Runtime.InteropServices;
using System.Diagnostics;
using System;
using System.Drawing;
using System.IO;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Drawing.Imaging;

namespace AssemblerProject
{  
    public enum DllType {CPP, ASM}
    public class ProcessingManager
    {
        [DllImport(@"D:\Studia\Projects\AssemblerProject\x64\Debug\JAAsm.dll")]
        static extern void MyProc1(byte oneChannel);
        [DllImport(@"D:\Studia\Projects\AssemblerProject\x64\Debug\JACpp.dll")]
        static extern IntPtr KuwaharaFilter(IntPtr inputImage, int width, int height, int numThreads);

        public TimeSpan currentExecutionTime { get; private set; }
        public TimeSpan previousExecutionTime { get; private set; }
        Stopwatch stopwatch;
        public Bitmap loadedBitmap { get; private set; }

        int bitmapSize;
        int numberOfThreads;

        public ProcessingManager()
        {
            currentExecutionTime = TimeSpan.Zero;
            previousExecutionTime = TimeSpan.Zero;
            stopwatch = new Stopwatch();
            loadedBitmap = null;
        }
        public void LoadBitmap(Bitmap bitmap, int numberOfThreads)
        {
            bitmapSize = bitmap.Width * bitmap.Height * 3;
            this.numberOfThreads = numberOfThreads;
            loadedBitmap = bitmap;
        }

        public Bitmap startProcessingImage(DllType dllType)
        {
            if (loadedBitmap == null /*|| loadedBitmapArray == null*/) return null;
            Bitmap result = new Bitmap(loadedBitmap.Width, loadedBitmap.Height);
            stopwatch.Reset();

            switch (dllType)
            {
                case DllType.CPP:
                    stopwatch.Start();
                    ProcessUsingCpp(loadedBitmap);
                    stopwatch.Stop();
                    break;
                case DllType.ASM:
                    stopwatch.Start();
                    //resultPtr = Marshal.AllocHGlobal(bitmapSize);

                    //for (int y = 0; y < loadedBitmap.Height; y++)
                    //{
                    //    for (int x = 0; x < loadedBitmap.Width; x++)
                    //    {
                    //        int pixelOffset = (y * loadedBitmap.Width + x) * 3;  

                    //        byte blueChannel = Marshal.ReadByte(IntPtr.Add(unmanagedPointer, pixelOffset + 2));

                    //        MyProc1(blueChannel);

                    //        Marshal.WriteByte(IntPtr.Add(resultPtr, pixelOffset + 2), blueChannel);
                    //    }
                    //}

                    //stopwatch.Stop();

                    //Marshal.Copy(resultPtr, resultArray, 0, resultArray.Length);
                    //Marshal.FreeHGlobal(unmanagedPointer);
                    //Marshal.FreeHGlobal(resultPtr);
                    //result = CreateBitmapFromArray(resultArray, loadedBitmap.Width, loadedBitmap.Height);

                    break;
            }
            if (currentExecutionTime != TimeSpan.Zero)
                previousExecutionTime = currentExecutionTime;
            currentExecutionTime = stopwatch.Elapsed;

            return loadedBitmap;
        }

        private void ProcessUsingCpp(Bitmap inputBitmap)
        {
            BitmapData inputData = inputBitmap.LockBits(new Rectangle(0, 0, inputBitmap.Width, inputBitmap.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);

            unsafe
            {
                IntPtr inputPtr = inputData.Scan0;

                IntPtr resultPtr = KuwaharaFilter(inputPtr, inputBitmap.Width, inputBitmap.Height, numberOfThreads);

                Buffer.MemoryCopy((void*)resultPtr, (void*)inputPtr, inputData.Stride * inputBitmap.Height, inputData.Stride * inputBitmap.Height);
            }

            inputBitmap.UnlockBits(inputData);
        }
    }
}
