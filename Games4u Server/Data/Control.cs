using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Drawing;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;


namespace Games4u_Server.Data
{
    public class Control
    {
        static string m_szModuleFolderPath = System.Environment.CurrentDirectory + "\\anticheatmodules";
        public static List<string> m_szListOfModules = new List<string>( );
        public delegate ListView.ListViewItemCollection GetItems( ListView lstview );
        public static ListView.ListViewItemCollection GetListViewItems( ListView lstview )
        {
            ListView.ListViewItemCollection temp = new ListView.ListViewItemCollection( new ListView( ) );
            if ( !lstview.InvokeRequired )
            {
                foreach ( ListViewItem item in lstview.Items )
                    temp.Add( ( ListViewItem )item.Clone( ) );
                return temp;
            }
            else
                return ( ListView.ListViewItemCollection )lstview.Invoke( new GetItems( GetListViewItems ), new object[ ] { lstview } );
        }
        public static void RefreshModulesList()
        {
            try
            {
                //Core.Log.NetworkLog("Refreshing anti-cheat modules list.");
                Program.mainForm.moduleList.Invoke(t => t.Items.Clear());
                m_szListOfModules.Clear( );

                foreach (string fi in Core.Log.GetFiles(m_szModuleFolderPath, "*.dll"))
                {
                    m_szListOfModules.Add( fi );
                    string m_szTemp = Path.GetFileName( fi );
                    Program.mainForm.moduleList.Invoke( t => t.Items.Add( m_szTemp ) );
                }

                Core.Array.ShuffleList( m_szListOfModules );
            }

            catch (Exception z)
            {
                Core.Log.NetworkLog("Error - " + z.Message);
            }
        }
        public static bool RefreshScreenshotList(string szFolder)
        {
            try
            {
                //Core.Log.NetworkLog("Refreshing screenshot list.");
                Program.mainForm.screenshotLayoutPanel.Invoke(t => t.Controls.Clear());

                int m_xCoord = 20, m_yCoord = 20, m_iMaxHeight = -1;
                if(!Directory.Exists(szFolder))
                {
                    Program.mainForm.ScreenshotSearchResult.Invoke( t => t.Text = "Folder not found." );
                    return false;
                }

                string[] pics = Core.Log.GetFiles( szFolder, "*.jpeg|*.png|*.jpg|*.bmp", SearchOption.AllDirectories );
                if(pics.Length == 0)
                {
                    Program.mainForm.ScreenshotSearchResult.Invoke( t => t.Text = "No screenshots." );
                    return false;
                }
                foreach ( string fi in pics  )
                {
                    PictureBox m_Pic = new PictureBox();
                    m_Pic.Image = Image.FromFile(fi);
                    m_Pic.Location = new Point(m_xCoord, m_yCoord);
                    m_Pic.SizeMode = PictureBoxSizeMode.StretchImage;
                    m_xCoord += m_Pic.Width + 10;
                    m_iMaxHeight = Math.Max(m_Pic.Height, m_iMaxHeight);
                    if( m_xCoord > Program.mainForm.ClientSize.Width - 100)
                    {
                        m_xCoord = 20;
                        m_yCoord += m_iMaxHeight + 10;
                    }
                    Program.mainForm.screenshotLayoutPanel.Invoke(t => t.Controls.Add(m_Pic));
                }
                Program.mainForm.ScreenshotSearchResult.Invoke( t => t.Text = "Found" );
                return true;
            }

            catch (Exception z)
            {
                Core.Log.NetworkLog("Error - " + z.Message);
                return false;
            }
        }
    }
}
