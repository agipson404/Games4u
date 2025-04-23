using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Text.RegularExpressions;
using System.Security.Cryptography;
using System.IO;

namespace Games4u_Server.Network
{
    public class Site
    {
        public static string _szGames4uSite = "http://games4u.se/";

        public static string GetUserSteamId(string pszUserID, out string pszSteamID)
        {
            try
            {
                string url = _szGames4uSite + "api/?apiaccount=10003&apipassword=8BD85YkVCWx1jy6V1waN&type=userinfo&userid=" + pszUserID;
                HttpWebRequest request = ( HttpWebRequest )WebRequest.Create( url );
                HttpWebResponse response = ( HttpWebResponse )request.GetResponse( );
                Stream stream = response.GetResponseStream( );
                StreamReader reader = new StreamReader( stream );

                string data = reader.ReadToEnd( );
                pszSteamID = Core.Array.GetTextBetween( data, "\u0022steamid\u0022:\u0022", "\u0022,\u0022rank" );
                reader.Close( );
                stream.Close( );
                return data;
            }
            catch ( Exception e )
            {
                Core.Log.NetworkLog( "GetUserSteamId: " + e.Message );
                pszSteamID = "ERROR";
                return "ERROR";
            }
        }
        public static string LogUserIn(string pszUsername, string pszPassword, out string pszSiteUserId)
        {
            try
            { 
                    pszPassword = Core.Array.Base64Encode(pszPassword);
                    string url = _szGames4uSite + "api/?apiaccount=10003&apipassword=8BD85YkVCWx1jy6V1waN&type=login&username=" + pszUsername + "&password=" + pszPassword + "";
                    HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
                    HttpWebResponse response = (HttpWebResponse)request.GetResponse();
                    Stream stream = response.GetResponseStream();
                    StreamReader reader = new StreamReader(stream);

                    string data = reader.ReadToEnd();
                    pszSiteUserId = Core.Array.GetTextBetween( data, "\u0022user_id\u0022:\u0022", "\u0022}" );
                    reader.Close();
                    stream.Close();
                    return data;
            }
            catch(Exception e)
            {
                Core.Log.NetworkLog("LogUserIn: " + e.Message);
                pszSiteUserId = "ERROR";
                return "ERROR";
            }
        }
        public static bool IsOnline(string szUrl)
        {
            try
            {
                HttpWebRequest request = WebRequest.Create(szUrl) as HttpWebRequest;
                request.Method = "HEAD";
                HttpWebResponse response = request.GetResponse() as HttpWebResponse;
                return (response.StatusCode == HttpStatusCode.OK);
            }
            catch
            {
                //Any exception will returns false.
                return false;
            }
        }
    }
}
