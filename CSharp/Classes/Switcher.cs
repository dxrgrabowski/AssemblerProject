using System;
using System.Drawing;
using System.Windows.Forms;
using System.Threading.Tasks;
using System.Threading;

namespace AssemblerProject
{
    public class Switcher
    {
        public ProcessingManager dataManager { get; private set; }
        public MainMenu mainMenu { get; private set; }

        public Switcher()
        {
            dataManager = new ProcessingManager();
            mainMenu = new MainMenu(this);
        }
        public Bitmap GetFunctionResult(Bitmap bitmap, DllType dllType, ref string executionTime, ref string previousExecutionTime)
        {
            Bitmap result = dataManager.startProcessingImage(dllType);
            executionTime = dataManager.currentExecutionTime != TimeSpan.Zero ? dataManager.currentExecutionTime.ToString() : "";
            previousExecutionTime = dataManager.previousExecutionTime != TimeSpan.Zero ? dataManager.previousExecutionTime.ToString() : "";
            return result;
        }
        
    }
}
