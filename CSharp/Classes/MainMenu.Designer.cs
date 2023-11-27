
namespace AssemblerProject
{
    partial class MainMenu
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            groupBox1 = new System.Windows.Forms.GroupBox();
            selectAsm = new System.Windows.Forms.RadioButton();
            selectCpp = new System.Windows.Forms.RadioButton();
            previousExecutionTimeLabel = new System.Windows.Forms.Label();
            currentExecutionTimeLabel = new System.Windows.Forms.Label();
            baseImagePreview = new System.Windows.Forms.PictureBox();
            basePreviewGroup = new System.Windows.Forms.GroupBox();
            resultPreviewGroup = new System.Windows.Forms.GroupBox();
            resultImagePreview = new System.Windows.Forms.PictureBox();
            baseImageLabel = new System.Windows.Forms.Label();
            resultImageLabel = new System.Windows.Forms.Label();
            uploadImageButton = new System.Windows.Forms.Button();
            filterImageButton = new System.Windows.Forms.Button();
            threadsNumber = new System.Windows.Forms.TextBox();
            enterThreadsNumber = new System.Windows.Forms.Label();
            groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)baseImagePreview).BeginInit();
            basePreviewGroup.SuspendLayout();
            resultPreviewGroup.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)resultImagePreview).BeginInit();
            SuspendLayout();
            // 
            // groupBox1
            // 
            groupBox1.Controls.Add(selectAsm);
            groupBox1.Controls.Add(selectCpp);
            groupBox1.Font = new System.Drawing.Font("Segoe UI Semibold", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            groupBox1.Location = new System.Drawing.Point(919, 652);
            groupBox1.Name = "groupBox1";
            groupBox1.Size = new System.Drawing.Size(186, 61);
            groupBox1.TabIndex = 7;
            groupBox1.TabStop = false;
            groupBox1.Text = "Lib: ";
            // 
            // selectAsm
            // 
            selectAsm.AutoSize = true;
            selectAsm.Location = new System.Drawing.Point(6, 27);
            selectAsm.Name = "selectAsm";
            selectAsm.Size = new System.Drawing.Size(71, 29);
            selectAsm.TabIndex = 2;
            selectAsm.Text = "ASM";
            selectAsm.UseVisualStyleBackColor = true;
            // 
            // selectCpp
            // 
            selectCpp.AutoSize = true;
            selectCpp.Checked = true;
            selectCpp.Location = new System.Drawing.Point(115, 27);
            selectCpp.Name = "selectCpp";
            selectCpp.Size = new System.Drawing.Size(64, 29);
            selectCpp.TabIndex = 0;
            selectCpp.TabStop = true;
            selectCpp.Text = "CPP";
            selectCpp.UseVisualStyleBackColor = true;
            // 
            // previousExecutionTimeLabel
            // 
            previousExecutionTimeLabel.AutoSize = true;
            previousExecutionTimeLabel.Font = new System.Drawing.Font("Segoe UI Semibold", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            previousExecutionTimeLabel.Location = new System.Drawing.Point(621, 715);
            previousExecutionTimeLabel.Name = "previousExecutionTimeLabel";
            previousExecutionTimeLabel.Size = new System.Drawing.Size(178, 25);
            previousExecutionTimeLabel.TabIndex = 9;
            previousExecutionTimeLabel.Text = "Previous exec time:";
            // 
            // currentExecutionTimeLabel
            // 
            currentExecutionTimeLabel.AutoSize = true;
            currentExecutionTimeLabel.Font = new System.Drawing.Font("Segoe UI Semibold", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            currentExecutionTimeLabel.Location = new System.Drawing.Point(327, 715);
            currentExecutionTimeLabel.Name = "currentExecutionTimeLabel";
            currentExecutionTimeLabel.Size = new System.Drawing.Size(170, 25);
            currentExecutionTimeLabel.TabIndex = 8;
            currentExecutionTimeLabel.Text = "Current exec time:";
            // 
            // baseImagePreview
            // 
            baseImagePreview.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            baseImagePreview.Location = new System.Drawing.Point(6, 14);
            baseImagePreview.Name = "baseImagePreview";
            baseImagePreview.Size = new System.Drawing.Size(378, 535);
            baseImagePreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            baseImagePreview.TabIndex = 13;
            baseImagePreview.TabStop = false;
            // 
            // basePreviewGroup
            // 
            basePreviewGroup.BackColor = System.Drawing.Color.Chocolate;
            basePreviewGroup.Controls.Add(baseImagePreview);
            basePreviewGroup.Font = new System.Drawing.Font("Segoe UI Semibold", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            basePreviewGroup.Location = new System.Drawing.Point(75, 52);
            basePreviewGroup.Name = "basePreviewGroup";
            basePreviewGroup.Size = new System.Drawing.Size(388, 556);
            basePreviewGroup.TabIndex = 14;
            basePreviewGroup.TabStop = false;
            basePreviewGroup.Enter += basePreviewGroup_Enter;
            // 
            // resultPreviewGroup
            // 
            resultPreviewGroup.BackColor = System.Drawing.Color.Chocolate;
            resultPreviewGroup.Controls.Add(resultImagePreview);
            resultPreviewGroup.Font = new System.Drawing.Font("Segoe UI Semibold", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            resultPreviewGroup.Location = new System.Drawing.Point(682, 52);
            resultPreviewGroup.Name = "resultPreviewGroup";
            resultPreviewGroup.Size = new System.Drawing.Size(388, 556);
            resultPreviewGroup.TabIndex = 15;
            resultPreviewGroup.TabStop = false;
            // 
            // resultImagePreview
            // 
            resultImagePreview.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            resultImagePreview.Location = new System.Drawing.Point(6, 16);
            resultImagePreview.Name = "resultImagePreview";
            resultImagePreview.Size = new System.Drawing.Size(378, 535);
            resultImagePreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            resultImagePreview.TabIndex = 13;
            resultImagePreview.TabStop = false;
            // 
            // baseImageLabel
            // 
            baseImageLabel.AutoSize = true;
            baseImageLabel.Font = new System.Drawing.Font("Segoe UI Semibold", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            baseImageLabel.Location = new System.Drawing.Point(186, 25);
            baseImageLabel.Name = "baseImageLabel";
            baseImageLabel.Size = new System.Drawing.Size(157, 25);
            baseImageLabel.TabIndex = 16;
            baseImageLabel.Text = "Uploaded image:";
            baseImageLabel.Click += baseImageLabel_Click;
            // 
            // resultImageLabel
            // 
            resultImageLabel.AutoSize = true;
            resultImageLabel.Font = new System.Drawing.Font("Segoe UI Semibold", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            resultImageLabel.Location = new System.Drawing.Point(789, 25);
            resultImageLabel.Name = "resultImageLabel";
            resultImageLabel.Size = new System.Drawing.Size(160, 25);
            resultImageLabel.TabIndex = 17;
            resultImageLabel.Text = "Processed image:";
            // 
            // uploadImageButton
            // 
            uploadImageButton.BackColor = System.Drawing.Color.SaddleBrown;
            uploadImageButton.Cursor = System.Windows.Forms.Cursors.Hand;
            uploadImageButton.Font = new System.Drawing.Font("Segoe UI Semibold", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            uploadImageButton.ForeColor = System.Drawing.SystemColors.ButtonFace;
            uploadImageButton.Location = new System.Drawing.Point(291, 652);
            uploadImageButton.Margin = new System.Windows.Forms.Padding(5);
            uploadImageButton.Name = "uploadImageButton";
            uploadImageButton.Size = new System.Drawing.Size(245, 58);
            uploadImageButton.TabIndex = 18;
            uploadImageButton.Text = "Enter Image Path";
            uploadImageButton.UseVisualStyleBackColor = false;
            uploadImageButton.Click += uploadImageButton_Click;
            // 
            // filterImageButton
            // 
            filterImageButton.BackColor = System.Drawing.Color.SaddleBrown;
            filterImageButton.Cursor = System.Windows.Forms.Cursors.Hand;
            filterImageButton.Font = new System.Drawing.Font("Segoe UI Semibold", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            filterImageButton.ForeColor = System.Drawing.SystemColors.ButtonFace;
            filterImageButton.Location = new System.Drawing.Point(580, 652);
            filterImageButton.Margin = new System.Windows.Forms.Padding(5);
            filterImageButton.Name = "filterImageButton";
            filterImageButton.Size = new System.Drawing.Size(239, 58);
            filterImageButton.TabIndex = 19;
            filterImageButton.Text = "Process Upladed Image";
            filterImageButton.UseVisualStyleBackColor = false;
            filterImageButton.Click += filterImageButton_Click;
            // 
            // threadsNumber
            // 
            threadsNumber.BorderStyle = System.Windows.Forms.BorderStyle.None;
            threadsNumber.Font = new System.Drawing.Font("Segoe UI Semibold", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            threadsNumber.Location = new System.Drawing.Point(124, 671);
            threadsNumber.MaxLength = 2;
            threadsNumber.Name = "threadsNumber";
            threadsNumber.PlaceholderText = "1";
            threadsNumber.Size = new System.Drawing.Size(50, 26);
            threadsNumber.TabIndex = 20;
            threadsNumber.Text = "1";
            threadsNumber.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            threadsNumber.TextChanged += threadsNumber_TextChanged;
            // 
            // enterThreadsNumber
            // 
            enterThreadsNumber.AutoSize = true;
            enterThreadsNumber.Font = new System.Drawing.Font("Segoe UI Semibold", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            enterThreadsNumber.Location = new System.Drawing.Point(12, 669);
            enterThreadsNumber.Name = "enterThreadsNumber";
            enterThreadsNumber.Size = new System.Drawing.Size(89, 25);
            enterThreadsNumber.TabIndex = 21;
            enterThreadsNumber.Text = "Threads: ";
            enterThreadsNumber.Click += enterThreadsNumber_Click;
            // 
            // MainMenu
            // 
            AutoScaleDimensions = new System.Drawing.SizeF(11F, 24F);
            AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            BackColor = System.Drawing.Color.BlanchedAlmond;
            ClientSize = new System.Drawing.Size(1184, 761);
            Controls.Add(filterImageButton);
            Controls.Add(uploadImageButton);
            Controls.Add(resultImageLabel);
            Controls.Add(baseImageLabel);
            Controls.Add(resultPreviewGroup);
            Controls.Add(basePreviewGroup);
            Controls.Add(previousExecutionTimeLabel);
            Controls.Add(currentExecutionTimeLabel);
            Controls.Add(groupBox1);
            Controls.Add(enterThreadsNumber);
            Controls.Add(threadsNumber);
            Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            Margin = new System.Windows.Forms.Padding(5);
            Name = "MainMenu";
            Text = "AssemblerProject";
            groupBox1.ResumeLayout(false);
            groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)baseImagePreview).EndInit();
            basePreviewGroup.ResumeLayout(false);
            resultPreviewGroup.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)resultImagePreview).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton selectCpp;
        private System.Windows.Forms.Label previousExecutionTimeLabel;
        private System.Windows.Forms.RadioButton selectAsm;
        private System.Windows.Forms.Label currentExecutionTimeLabel;
        private System.Windows.Forms.PictureBox baseImagePreview;
        private System.Windows.Forms.GroupBox basePreviewGroup;
        private System.Windows.Forms.GroupBox resultPreviewGroup;
        private System.Windows.Forms.PictureBox resultImagePreview;
        private System.Windows.Forms.Label baseImageLabel;
        private System.Windows.Forms.Label resultImageLabel;
        private System.Windows.Forms.Button uploadImageButton;
        private System.Windows.Forms.Button filterImageButton;
        private System.Windows.Forms.TextBox threadsNumber;
        private System.Windows.Forms.Label enterThreadsNumber;
    }
}

