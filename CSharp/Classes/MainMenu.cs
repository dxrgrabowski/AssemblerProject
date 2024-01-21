using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;


namespace AssemblerProject
{
    public partial class MainMenu : Form
    {
        Switcher controller;
        public MainMenu(Switcher controller)
        {
            InitializeComponent();
            this.controller = controller;

            currentExecutionTimeLabel.Text = "";
            previousExecutionTimeLabel.Text = "";
        }


        private void uploadImageButton_Click(object sender, EventArgs e)
        {
            try
            {
                OpenFileDialog dialog = new OpenFileDialog();
                // dialog.Filter = "JPG files(*.jpg)|*.jpg;*png|PNG files(*.png)|*.png";
                dialog.Filter = "Image files (*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp";
                if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    Bitmap bitmap = new Bitmap(dialog.FileName);
                    controller.dataManager.LoadBitmap(bitmap);
                    baseImagePreview.Image = bitmap;
                }
            }
            catch (Exception)
            {
                MessageBox.Show("Wrong file selected", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

        }

        private async void filterImageButton_Click(object sender, EventArgs e)
        {
            int numberOfThreads;
            try
            {
                numberOfThreads = Int32.Parse(threadsNumber.Text);
            }
            catch (FormatException)
            {
                MessageBox.Show("Enter a number of threads between 1 and 64", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            if (numberOfThreads < 1 || numberOfThreads > 64)
            {
                MessageBox.Show("Enter a number of threads between 1 and 64", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if (controller.dataManager.loadedBitmap != null)
            {
                DllType dllType = selectAsm.Checked ? DllType.ASM : selectCpp.Checked ? DllType.CPP : DllType.CPP;
                string executionTime = "";
                string previousExecutionTime = "";

                resultImagePreview.Image = controller.dataManager.startProcessingImage(dllType, numberOfThreads);

                executionTime = controller.dataManager.currentExecutionMs != 0 ? Math.Round(controller.dataManager.currentExecutionMs).ToString() : "";
                previousExecutionTime = controller.dataManager.previousExecutionMs != 0 ? Math.Round(controller.dataManager.previousExecutionMs).ToString() : "";

                currentExecutionTimeLabel.Text = $"Execution time: {executionTime}ms";
                if (previousExecutionTime != "") previousExecutionTimeLabel.Text = $"Previous execution time: {previousExecutionTime}ms";

            }
            else
            {
                MessageBox.Show("You must upload an image first", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void enterThreadsNumber_Click(object sender, EventArgs e)
        {

        }

        private void threadsNumber_TextChanged(object sender, EventArgs e)
        {

        }

        private void basePreviewGroup_Enter(object sender, EventArgs e)
        {

        }

        private void baseImageLabel_Click(object sender, EventArgs e)
        {

        }
    }
}
