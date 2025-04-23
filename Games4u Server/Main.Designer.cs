namespace Games4u_Server
{
    partial class Main
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main));
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.serverTextBox = new System.Windows.Forms.RichTextBox();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.SendAntiCheatModules = new System.Windows.Forms.CheckBox();
            this.anticheatserverCheckBox = new System.Windows.Forms.CheckBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.moduleList = new System.Windows.Forms.ListBox();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.banOptionP2CMember = new System.Windows.Forms.CheckBox();
            this.label6 = new System.Windows.Forms.Label();
            this.BanUsers = new System.Windows.Forms.Button();
            this.banOptionReversing = new System.Windows.Forms.CheckBox();
            this.banOptionCheating = new System.Windows.Forms.CheckBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.anticheatServerStatus = new System.Windows.Forms.Label();
            this.mainListView = new System.Windows.Forms.ListView();
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.IPCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.UsernameCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.HWIDCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.activateServer = new System.Windows.Forms.TabControl();
            this.tabPage5 = new System.Windows.Forms.TabPage();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.ScreenshotSearchResult = new System.Windows.Forms.Label();
            this.SearchScreenshotUsername = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.screenshotLayoutPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.screenShotList = new System.Windows.Forms.ImageList(this.components);
            this.tabPage4.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.activateServer.SuspendLayout();
            this.tabPage5.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.serverTextBox);
            this.tabPage4.Location = new System.Drawing.Point(4, 22);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Size = new System.Drawing.Size(986, 433);
            this.tabPage4.TabIndex = 3;
            this.tabPage4.Text = "Log";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // serverTextBox
            // 
            this.serverTextBox.Location = new System.Drawing.Point(4, 4);
            this.serverTextBox.Name = "serverTextBox";
            this.serverTextBox.ReadOnly = true;
            this.serverTextBox.Size = new System.Drawing.Size(1055, 411);
            this.serverTextBox.TabIndex = 0;
            this.serverTextBox.Text = "";
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.SendAntiCheatModules);
            this.tabPage3.Controls.Add(this.anticheatserverCheckBox);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(986, 433);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Settings";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // SendAntiCheatModules
            // 
            this.SendAntiCheatModules.AutoSize = true;
            this.SendAntiCheatModules.Location = new System.Drawing.Point(4, 28);
            this.SendAntiCheatModules.Name = "SendAntiCheatModules";
            this.SendAntiCheatModules.Size = new System.Drawing.Size(143, 17);
            this.SendAntiCheatModules.TabIndex = 1;
            this.SendAntiCheatModules.Text = "Send anti-cheat modules";
            this.SendAntiCheatModules.UseVisualStyleBackColor = true;
            this.SendAntiCheatModules.CheckedChanged += new System.EventHandler(this.SendAntiCheatModules_CheckedChanged);
            // 
            // anticheatserverCheckBox
            // 
            this.anticheatserverCheckBox.AutoSize = true;
            this.anticheatserverCheckBox.Location = new System.Drawing.Point(4, 4);
            this.anticheatserverCheckBox.Name = "anticheatserverCheckBox";
            this.anticheatserverCheckBox.Size = new System.Drawing.Size(128, 17);
            this.anticheatserverCheckBox.TabIndex = 0;
            this.anticheatserverCheckBox.Text = "Run anti-cheat server";
            this.anticheatserverCheckBox.UseVisualStyleBackColor = true;
            this.anticheatserverCheckBox.CheckedChanged += new System.EventHandler(this.anticheatserverCheckBox_CheckedChanged);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.label3);
            this.tabPage2.Controls.Add(this.label2);
            this.tabPage2.Controls.Add(this.label1);
            this.tabPage2.Controls.Add(this.moduleList);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(986, 433);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Modules";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(266, 52);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(40, 13);
            this.label3.TabIndex = 3;
            this.label3.Text = "Status:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(265, 28);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(72, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Last updated:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(265, 7);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(38, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Name:";
            // 
            // moduleList
            // 
            this.moduleList.FormattingEnabled = true;
            this.moduleList.Location = new System.Drawing.Point(7, 7);
            this.moduleList.Name = "moduleList";
            this.moduleList.Size = new System.Drawing.Size(251, 394);
            this.moduleList.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.groupBox1);
            this.tabPage1.Controls.Add(this.label4);
            this.tabPage1.Controls.Add(this.anticheatServerStatus);
            this.tabPage1.Controls.Add(this.mainListView);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(986, 433);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Connected";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.banOptionP2CMember);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.BanUsers);
            this.groupBox1.Controls.Add(this.banOptionReversing);
            this.groupBox1.Controls.Add(this.banOptionCheating);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Location = new System.Drawing.Point(843, 33);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(137, 138);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Ban Options";
            // 
            // banOptionP2CMember
            // 
            this.banOptionP2CMember.AutoSize = true;
            this.banOptionP2CMember.Location = new System.Drawing.Point(10, 83);
            this.banOptionP2CMember.Name = "banOptionP2CMember";
            this.banOptionP2CMember.Size = new System.Drawing.Size(87, 17);
            this.banOptionP2CMember.TabIndex = 5;
            this.banOptionP2CMember.Text = "P2C Member";
            this.banOptionP2CMember.UseVisualStyleBackColor = true;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 20);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(35, 13);
            this.label6.TabIndex = 4;
            this.label6.Text = "Flags:";
            // 
            // BanUsers
            // 
            this.BanUsers.Location = new System.Drawing.Point(10, 109);
            this.BanUsers.Name = "BanUsers";
            this.BanUsers.Size = new System.Drawing.Size(75, 23);
            this.BanUsers.TabIndex = 3;
            this.BanUsers.Text = "Ban";
            this.BanUsers.UseVisualStyleBackColor = true;
            // 
            // banOptionReversing
            // 
            this.banOptionReversing.AutoSize = true;
            this.banOptionReversing.Location = new System.Drawing.Point(10, 59);
            this.banOptionReversing.Name = "banOptionReversing";
            this.banOptionReversing.Size = new System.Drawing.Size(119, 17);
            this.banOptionReversing.TabIndex = 2;
            this.banOptionReversing.Text = "Loader Modification";
            this.banOptionReversing.UseVisualStyleBackColor = true;
            // 
            // banOptionCheating
            // 
            this.banOptionCheating.AutoSize = true;
            this.banOptionCheating.Location = new System.Drawing.Point(10, 36);
            this.banOptionCheating.Name = "banOptionCheating";
            this.banOptionCheating.Size = new System.Drawing.Size(68, 17);
            this.banOptionCheating.TabIndex = 1;
            this.banOptionCheating.Text = "Cheating";
            this.banOptionCheating.UseVisualStyleBackColor = true;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(7, 20);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(0, 13);
            this.label5.TabIndex = 0;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(677, 33);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(0, 13);
            this.label4.TabIndex = 3;
            // 
            // anticheatServerStatus
            // 
            this.anticheatServerStatus.AutoSize = true;
            this.anticheatServerStatus.Location = new System.Drawing.Point(7, 7);
            this.anticheatServerStatus.Name = "anticheatServerStatus";
            this.anticheatServerStatus.Size = new System.Drawing.Size(72, 13);
            this.anticheatServerStatus.TabIndex = 1;
            this.anticheatServerStatus.Text = "Server status:";
            // 
            // mainListView
            // 
            this.mainListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader2,
            this.IPCol,
            this.UsernameCol,
            this.HWIDCol,
            this.columnHeader1});
            this.mainListView.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.mainListView.FullRowSelect = true;
            this.mainListView.Location = new System.Drawing.Point(7, 33);
            this.mainListView.Name = "mainListView";
            this.mainListView.Size = new System.Drawing.Size(830, 394);
            this.mainListView.TabIndex = 0;
            this.mainListView.UseCompatibleStateImageBehavior = false;
            this.mainListView.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "ID";
            this.columnHeader2.Width = 55;
            // 
            // IPCol
            // 
            this.IPCol.Text = "IP Address";
            this.IPCol.Width = 190;
            // 
            // UsernameCol
            // 
            this.UsernameCol.Text = "Username";
            this.UsernameCol.Width = 224;
            // 
            // HWIDCol
            // 
            this.HWIDCol.Text = "HWID";
            this.HWIDCol.Width = 201;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Flagged";
            this.columnHeader1.Width = 213;
            // 
            // activateServer
            // 
            this.activateServer.Controls.Add(this.tabPage1);
            this.activateServer.Controls.Add(this.tabPage2);
            this.activateServer.Controls.Add(this.tabPage5);
            this.activateServer.Controls.Add(this.tabPage3);
            this.activateServer.Controls.Add(this.tabPage4);
            this.activateServer.Location = new System.Drawing.Point(13, 13);
            this.activateServer.Name = "activateServer";
            this.activateServer.SelectedIndex = 0;
            this.activateServer.Size = new System.Drawing.Size(994, 459);
            this.activateServer.TabIndex = 0;
            // 
            // tabPage5
            // 
            this.tabPage5.Controls.Add(this.groupBox2);
            this.tabPage5.Controls.Add(this.screenshotLayoutPanel);
            this.tabPage5.Location = new System.Drawing.Point(4, 22);
            this.tabPage5.Name = "tabPage5";
            this.tabPage5.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage5.Size = new System.Drawing.Size(986, 433);
            this.tabPage5.TabIndex = 4;
            this.tabPage5.Text = "Screenshots";
            this.tabPage5.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.ScreenshotSearchResult);
            this.groupBox2.Controls.Add(this.SearchScreenshotUsername);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Location = new System.Drawing.Point(788, 7);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(195, 85);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Options";
            // 
            // ScreenshotSearchResult
            // 
            this.ScreenshotSearchResult.AutoSize = true;
            this.ScreenshotSearchResult.Location = new System.Drawing.Point(7, 60);
            this.ScreenshotSearchResult.Name = "ScreenshotSearchResult";
            this.ScreenshotSearchResult.Size = new System.Drawing.Size(40, 13);
            this.ScreenshotSearchResult.TabIndex = 3;
            this.ScreenshotSearchResult.Text = "Result:";
            // 
            // SearchScreenshotUsername
            // 
            this.SearchScreenshotUsername.Location = new System.Drawing.Point(10, 37);
            this.SearchScreenshotUsername.MaxLength = 18;
            this.SearchScreenshotUsername.Name = "SearchScreenshotUsername";
            this.SearchScreenshotUsername.Size = new System.Drawing.Size(179, 20);
            this.SearchScreenshotUsername.TabIndex = 1;
            this.SearchScreenshotUsername.TextChanged += new System.EventHandler(this.SearchScreenshotUsername_TextChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(7, 20);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(112, 13);
            this.label7.TabIndex = 0;
            this.label7.Text = "Search Screenshot(s):";
            // 
            // screenshotLayoutPanel
            // 
            this.screenshotLayoutPanel.Location = new System.Drawing.Point(6, 7);
            this.screenshotLayoutPanel.Name = "screenshotLayoutPanel";
            this.screenshotLayoutPanel.Size = new System.Drawing.Size(775, 389);
            this.screenshotLayoutPanel.TabIndex = 0;
            // 
            // screenShotList
            // 
            this.screenShotList.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.screenShotList.ImageSize = new System.Drawing.Size(16, 16);
            this.screenShotList.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1019, 479);
            this.Controls.Add(this.activateServer);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "Main";
            this.Text = "Games4u: Anti-Cheat Server";
            this.Load += new System.EventHandler(this.Main_Load);
            this.tabPage4.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.activateServer.ResumeLayout(false);
            this.tabPage5.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabPage tabPage4;
        public System.Windows.Forms.RichTextBox serverTextBox;
        private System.Windows.Forms.TabPage tabPage3;
        public System.Windows.Forms.CheckBox anticheatserverCheckBox;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        public System.Windows.Forms.ListBox moduleList;
        private System.Windows.Forms.TabPage tabPage1;
        public System.Windows.Forms.Label anticheatServerStatus;
        public System.Windows.Forms.ListView mainListView;
        private System.Windows.Forms.ColumnHeader IPCol;
        private System.Windows.Forms.ColumnHeader UsernameCol;
        private System.Windows.Forms.ColumnHeader HWIDCol;
        private System.Windows.Forms.TabControl activateServer;
        private System.Windows.Forms.TabPage tabPage5;
        public System.Windows.Forms.FlowLayoutPanel screenshotLayoutPanel;
        public System.Windows.Forms.ImageList screenShotList;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button BanUsers;
        private System.Windows.Forms.CheckBox banOptionReversing;
        private System.Windows.Forms.CheckBox banOptionCheating;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        public System.Windows.Forms.CheckBox SendAntiCheatModules;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.CheckBox banOptionP2CMember;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.GroupBox groupBox2;
        public System.Windows.Forms.TextBox SearchScreenshotUsername;
        private System.Windows.Forms.Label label7;
        public System.Windows.Forms.Label ScreenshotSearchResult;


    }
}

