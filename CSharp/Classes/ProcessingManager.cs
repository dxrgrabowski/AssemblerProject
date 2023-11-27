using System.Runtime.InteropServices;
using System.Diagnostics;
using System;
using System.Drawing;
using System.IO;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace AssemblerProject
{  
    public enum DllType { CPP, ASM}
    public class ProcessingManager
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct Arrays
        {
            public IntPtr top_array;
            public IntPtr bot_array;
            public IntPtr left_array;
            public IntPtr right_array;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct Histo
        {
            public byte tresh_r, tresh_g, tresh_b;
            public byte r_start, r_end;
            public byte g_start, g_end;
            public byte b_start, b_end;

            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
            public uint[] hist_r;

            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
            public uint[] hist_g;

            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
            public uint[] hist_b;
        }

        [DllImport(@"C:\Users\windo\Downloads\JA\x64\Release\JAAsm.dll")]
        static extern void MyProc1(byte oneChannel);

        [DllImport(@"C:\Users\windo\Downloads\JA\x64\Release\JACpp.dll")]
        static extern IntPtr fill_edge_and_punch_hole(IntPtr bitmap, int width, int height, ref Arrays arrays, ref Histo hist);
        [DllImport(@"C:\Users\windo\Downloads\JA\x64\Release\JACpp.dll")]
        static extern IntPtr KuwaharaFilter(IntPtr inputImage, int width, int height, int numThreads);

  

        //static extern void border_random_fill(unsigned char* raw_buffer, unsigned short width, unsigned short height,  const Arrays* arrays, const Histo* hist);

        public TimeSpan currentExecutionTime { get; private set; }
        public TimeSpan previousExecutionTime { get; private set; }
        Stopwatch stopwatch;
        public Bitmap loadedBitmap { get; private set; }
        byte[] loadedBitmapArray;
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
            loadedBitmapArray = ConvertBitmapToByteArray(bitmap);
        }

        public Bitmap startProcessingImage(DllType dllType)
        {
            Arrays arrays = new Arrays();
            Histo hist = new Histo();

            if (loadedBitmap == null || loadedBitmapArray == null) return null;
            Bitmap result = new Bitmap(loadedBitmap.Width, loadedBitmap.Height);
            stopwatch.Reset();

            IntPtr unmanagedPointer = Marshal.AllocHGlobal(bitmapSize);
            Marshal.Copy(loadedBitmapArray, 0, unmanagedPointer, bitmapSize);
            IntPtr resultPtr = Marshal.AllocHGlobal(bitmapSize);
            byte[] resultArray = new byte[loadedBitmapArray.Length];

           fill_edge_and_punch_hole(unmanagedPointer, result.Width, result.Height, ref arrays, ref hist);

            switch (dllType)
            {
                case DllType.CPP:
                    stopwatch.Start();
                    //resultPtr = border_random_fill(unmanagedPointer, result.Width, result.Height, ref arrays, ref hist);
                    //ApplyKuwaharaFilter(unmanagedPointer, resultPtr, result.Width, result.Height);
                    resultPtr = KuwaharaFilter(unmanagedPointer, result.Width, result.Height, numberOfThreads);

                    stopwatch.Stop();

                    Marshal.Copy(resultPtr, resultArray, 0, resultArray.Length);
                    Marshal.FreeHGlobal(unmanagedPointer);
                    result = CreateBitmapFromArray(resultArray, loadedBitmap.Width, loadedBitmap.Height);
                    break;
                case DllType.ASM:
                    stopwatch.Start();
                    resultPtr = Marshal.AllocHGlobal(bitmapSize);

                    //Parallel.For(0, loadedBitmap.Height, y =>
                    for (int y = 0; y < loadedBitmap.Height; y++)
                    {
                        for (int x = 0; x < loadedBitmap.Width; x++)
                        {
                            int pixelOffset = (y * loadedBitmap.Width + x) * 3;  // 3 bajty na piksel (RGB)

                            byte blueChannel = Marshal.ReadByte(IntPtr.Add(unmanagedPointer, pixelOffset + 2));

                            MyProc1(blueChannel);

                            Marshal.WriteByte(IntPtr.Add(resultPtr, pixelOffset + 2), blueChannel);
                        }
                    }

                    stopwatch.Stop();

                    Marshal.Copy(resultPtr, resultArray, 0, resultArray.Length);
                    Marshal.FreeHGlobal(unmanagedPointer);
                    Marshal.FreeHGlobal(resultPtr);
                    result = CreateBitmapFromArray(resultArray, loadedBitmap.Width, loadedBitmap.Height);

                    break;
            }
            if (currentExecutionTime != TimeSpan.Zero)
                previousExecutionTime = currentExecutionTime;
            currentExecutionTime = stopwatch.Elapsed;

            return result;
        }




        //public Bitmap startProcessingImage(DllType dllType)
        //{
        //    Arrays arrays = new Arrays();
        //    Histo hist = new Histo();

        //    if (loadedBitmap == null || loadedBitmapArray == null) return null;
        //    Bitmap result = new Bitmap(loadedBitmap.Width, loadedBitmap.Height);
        //    stopwatch.Reset();

        //    IntPtr unmanagedPointer = Marshal.AllocHGlobal(bitmapSize);
        //    Marshal.Copy(loadedBitmapArray, 0, unmanagedPointer, bitmapSize);
        //    IntPtr resultPtr = IntPtr.Zero;
        //    byte[] resultArray = new byte[loadedBitmapArray.Length];

        //    fill_edge_and_punch_hole(unmanagedPointer, result.Width, result.Height, ref arrays, ref hist);

        //    switch (dllType)
        //    {
        //        case DllType.CPP:
        //            stopwatch.Start();
        //            resultPtr = border_random_fill(unmanagedPointer, result.Width, result.Height, ref arrays, ref hist);
        //            stopwatch.Stop();

        //            Marshal.Copy(resultPtr, resultArray, 0, resultArray.Length);
        //            Marshal.FreeHGlobal(unmanagedPointer);
        //            result = CreateBitmapFromArray(resultArray, loadedBitmap.Width, loadedBitmap.Height);

        //            break;
        //        case DllType.ASM:
        //            stopwatch.Start();
        //            resultPtr = Marshal.AllocHGlobal(bitmapSize);

        //            Parallel.For(0, Environment.ProcessorCount, threadNum =>
        //            {
        //                int rowStart = threadNum * result.Height / Environment.ProcessorCount;
        //                int rowEnd = (threadNum + 1) * result.Height / Environment.ProcessorCount;

        //                for (int row = rowStart; row < rowEnd; row++)
        //                {
        //                    IntPtr srcRowPtr = IntPtr.Add(unmanagedPointer, row * result.Width * 4);
        //                    IntPtr destRowPtr = IntPtr.Add(resultPtr, row * result.Width * 4);

        //                    MyProc1(srcRowPtr, destRowPtr, result.Width, 1);
        //                }
        //            });
        //            stopwatch.Stop();

        //            Marshal.Copy(resultPtr, resultArray, 0, resultArray.Length);
        //            Marshal.FreeHGlobal(unmanagedPointer);
        //            result = CreateBitmapFromArray(resultArray, loadedBitmap.Width, loadedBitmap.Height);

        //            break;
        //    }
        //    if (currentExecutionTime != TimeSpan.Zero)
        //        previousExecutionTime = currentExecutionTime;
        //    currentExecutionTime = stopwatch.Elapsed;

        //    return result;
        //}

        public byte[] ConvertPointerToByteArray(IntPtr ptr, int size)
{
            byte[] byteArray = new byte[size];
            Marshal.Copy(ptr, byteArray, 0, size);
            return byteArray;
        }
        public byte[] ConvertBitmapToByteArray(Bitmap bitmap)
        {
            int byteArrayLength = bitmap.Width * bitmap.Height * 3;
            byte[] byteArray = new byte[byteArrayLength];
            int byteIndex = 0;

            for (int y = 0; y < bitmap.Height; y++)
            {
                for (int x = 0; x < bitmap.Width; x++)
                {
                    Color pixelColor = bitmap.GetPixel(x, y);
                    byteArray[byteIndex] = pixelColor.R;
                    byteArray[byteIndex + 1] = pixelColor.G;
                    byteArray[byteIndex + 2] = pixelColor.B;
                    byteIndex += 3;
                }
            }
            return byteArray;
        }
        public Bitmap CreateBitmapFromArray(byte[] byteArray, int width, int height)
        {
            Bitmap bitmap = new Bitmap(width, height);
            int byteIndex = 0;

            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    Color pixelColor = Color.FromArgb(byteArray[byteIndex], byteArray[byteIndex + 1], byteArray[byteIndex + 2]);
                    bitmap.SetPixel(x, y, pixelColor);
                    byteIndex += 3;
                }
            }
            return bitmap;
        }
    }
}
