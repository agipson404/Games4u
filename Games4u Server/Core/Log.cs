using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Drawing;

namespace Games4u_Server.Core
{
    public class Log
    {
        static string _networkfilepath = System.Environment.CurrentDirectory + "\\serverlog.txt";
        public static string _flaggedusersfilepath = System.Environment.CurrentDirectory + "\\flaggedusers.ini";
        public static string _bannedusersfilepath = System.Environment.CurrentDirectory + "\\bannedusers.ini";
        public static string _hwidusersfilepath = System.Environment.CurrentDirectory + "\\usershwid.ini";
        public static string m_szUserData = System.Environment.CurrentDirectory + "\\anticheatuserdata";

        public static void BanUser( Network.Client.Info pClientData,  string pszReason )
        {
            string test = System.IO.File.ReadAllText( _bannedusersfilepath );
            if ( test.Contains( pClientData.szUsername ) || test.Contains( pClientData.szHWID ) || test.Contains( pClientData.szIPAddress ) )
                return;

            Core.Log.UserLog( pClientData, "was banned for: " + pszReason );

            File.WriteConfig( string.Empty, pClientData.szUsername, pszReason, _bannedusersfilepath );
            File.WriteConfig( string.Empty, pClientData.szHWID, pszReason, _bannedusersfilepath );
            File.WriteConfig( string.Empty, pClientData.szIPAddress, pszReason, _bannedusersfilepath );
        }
        public static void SetUserHWID( Network.Client.Info pClientData, string pszHWID )
        {
            Core.Log.UserLog( pClientData, "HWID was set." );
            File.WriteConfig(string.Empty, pClientData.szUsername, pszHWID, _hwidusersfilepath);
        }
        public static string CheckUserHWID( Network.Client.Info pClientData )
        {
            return File.ReadConfig( string.Empty, pClientData.szUsername, _hwidusersfilepath );
        }
        public static string GetUserFlagMode( Network.Client.Info pClientData )
        {
            string m_szFlagMode = string.Empty;

            m_szFlagMode = File.ReadConfig( string.Empty, pClientData.szUsername, _flaggedusersfilepath );
            if ( m_szFlagMode != string.Empty )
                return m_szFlagMode;

            m_szFlagMode = File.ReadConfig( string.Empty, pClientData.szHWID, _flaggedusersfilepath );
            if ( m_szFlagMode != string.Empty )
                return m_szFlagMode;

            m_szFlagMode = File.ReadConfig( string.Empty, pClientData.szIPAddress, _flaggedusersfilepath );

            return m_szFlagMode;
        }
        public static void SetUserFlagMode( Network.Client.Info pClientData, string pszReason )
        {
            string test = Encoding.UTF8.GetString(File.ReadFile( _flaggedusersfilepath ));
            if ( test.Contains( pClientData.szUsername ) || test.Contains( pClientData.szHWID ) || test.Contains( pClientData.szIPAddress ) )
                return;

            Core.Log.UserLog( pClientData, "was flagged for: " + pszReason );
            File.WriteConfig( string.Empty, pClientData.szUsername, pszReason, _flaggedusersfilepath );
            File.WriteConfig( string.Empty, pClientData.szHWID, pszReason, _flaggedusersfilepath );
            File.WriteConfig( string.Empty, pClientData.szIPAddress, pszReason, _flaggedusersfilepath );
        }
        public static string[] GetFiles(string path, string searchPattern, SearchOption pSearchOption = SearchOption.TopDirectoryOnly)
        {
            string[] searchPatterns = searchPattern.Split('|');
            List<string> files = new List<string>();
            foreach (string sp in searchPatterns)
                files.AddRange(System.IO.Directory.GetFiles(path, sp, pSearchOption));
                //files.AddRange(System.IO.Directory.GetFiles(path, sp, pSearchOption).Select(f => Path.GetFileName(f)));
            files.Sort();
            return files.ToArray();
        }
        public static void UserLog( Network.Client.Info pClientData, string pszText )
        {
            try
            {
            
                System.IO.File.AppendAllText( m_szUserData + "\\" + pClientData.szUsername + "\\log.txt", "[ " + DateTime.Now.ToString( ) + " ] " + "(" + pClientData.szHWID + ")" + " and (" + pClientData.szIPAddress + ") " + Environment.NewLine  + pszText + Environment.NewLine );
            }
            catch( Exception e)
            {
                return;
            }
        }
        public static void UserScreenshot( Network.Client.Info pClientData, byte[] pImage)
        {
            try
            {
                string m_szScreenshotName = m_szUserData + "\\" + pClientData.szUsername + "\\" + Core.Array.GetUniqueKey( 12 ) + ".jpeg";
                System.IO.File.WriteAllBytes( m_szScreenshotName, pImage );

                Core.Log.UserLog( pClientData, "Screenshot saved to: " + m_szScreenshotName );
            }
            catch ( Exception e )
            {
                Core.Log.UserLog( pClientData, e.Message );
                return;
            }
        }
        public static void NetworkLog(string szText)
        {
            Program.mainForm.serverTextBox.Invoke(t => t.Text += "[ " + DateTime.Now.ToString() + " ] " + szText + Environment.NewLine);
            try
            {
                System.IO.File.AppendAllText(_networkfilepath, "[ " + DateTime.Now.ToString() + " ] " + szText + Environment.NewLine);
            }
            catch (Exception e)
            {
                return;
            }

        }
    }
}
