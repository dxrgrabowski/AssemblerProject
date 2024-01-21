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
        
    }
}
