using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.IO;
using System.Net;
using System.Net.Sockets;

namespace Games4u_Server
{
    public partial class Main : Form
    {
        public Main()
        {
            InitializeComponent();
        }

        public static bool m_bSearchEveryone = true;
        public static void Loop()
        {
            while (true)
            {

                Data.Control.RefreshModulesList();

                if ( m_bSearchEveryone )
                    Data.Control.RefreshScreenshotList( Core.Log.m_szUserData );
                else
                {
                    Data.Control.RefreshScreenshotList( Core.Log.m_szUserData + "\\" + Program.mainForm.SearchScreenshotUsername.Text);
                }

                Thread.Sleep(1000);
            }
        }
        private void Main_Load(object sender, EventArgs e)
        {
            //Core.Array.ConvertToBitmap(@"C:\Users\lolvu\OneDrive\Documents\Visual Studio 2013\Projects\Games4u\Release\file.jpeg");
            anticheatserverCheckBox.Checked = true;
            SendAntiCheatModules.Checked = true;
            Thread m_Thread = new Thread(new ThreadStart(Loop));
            m_Thread.IsBackground = true;
            m_Thread.Start();
            
        }

        private void anticheatserverCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (Program.mainForm.anticheatserverCheckBox.Checked)
            {
                Network.Server.Start();
                Program.mainForm.anticheatServerStatus.Invoke(t => t.Text = "Anti cheat server is running.");
            }
            else
            {
                Network.Server.Stop();
                Program.mainForm.mainListView.Items.Clear();
                Program.mainForm.anticheatServerStatus.Invoke(t => t.Text = "Anti cheat server is not running.");
            }
        }

        private void refreshModuleList_Click(object sender, EventArgs e)
        {
            Data.Control.RefreshModulesList();
        }

        private void SendAntiCheatModules_CheckedChanged( object sender, EventArgs e )
        {

        }

        private void SearchScreenshotUsername_TextChanged( object sender, EventArgs e )
        {
            if(SearchScreenshotUsername.Text == "")
            {
                m_bSearchEveryone = true;
                ScreenshotSearchResult.Text = "Screenshots from everyone.";
                return;
            }

            if ( SearchScreenshotUsername.TextLength < 3 )
            {
                m_bSearchEveryone = false;
                ScreenshotSearchResult.Text = "Username too short.";
                return;
            }

            m_bSearchEveryone = false;
        }
    }
}
