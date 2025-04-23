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
using System.Windows.Forms;

namespace Games4u_Server.Network
{
    public class Client
    {
        public struct Info
        {
            public string szIPAddress;
            public string szUsername;
            public string szPassword;
            public string szHWID;
            public string szRC4Key;
            public string szDataReceived;
            public string szSiteUserId;
            public string szSteamId;

            public byte bPacketType;
        };

        public static List<string> m_lActiveUsers = new List<string>( );
        public static List<string> m_lActiveIPAddresses = new List<string>( );
        public static int m_iMaxNumberOfKeepAliveAttempts = 5;

        public static List<Socket> m_ConnectedClientSockets = new List<Socket>( );

        public static bool IsClientAlreadyConnected(Client.Info pClientData)
        {
           bool m_bUserAlreadyConnected = false;
            foreach ( ListViewItem item in Data.Control.GetListViewItems( Program.mainForm.mainListView ) )
            {
                for ( int i = 0; i < 4; i++ )
                {
                    m_bUserAlreadyConnected = item.SubItems[ i ].Text.Contains( pClientData.szUsername );
                    if ( m_bUserAlreadyConnected )
                        return m_bUserAlreadyConnected;

                    m_bUserAlreadyConnected = item.SubItems[ i ].Text.Contains( pClientData.szIPAddress );
                    if ( m_bUserAlreadyConnected )
                        return m_bUserAlreadyConnected;
                }
            }
            return m_bUserAlreadyConnected;
        }
        public static int GetSocket( Socket pSocket )
        {
            int tracer = 0;

            foreach ( Socket s in m_ConnectedClientSockets )
            {
                if ( s == pSocket )
                {
                    break;
                }
                tracer++;
            }

            return tracer;
        }
        public static void Disconnect(Socket pCurrentSocket, Client.Info pClientData)
        {
            try
            {
                Client.m_lActiveUsers.Remove( pClientData.szUsername );
                Client.m_lActiveIPAddresses.Remove( pClientData.szIPAddress );
                Client.m_ConnectedClientSockets.Remove( pCurrentSocket );
                pCurrentSocket.Shutdown( SocketShutdown.Both );
                pCurrentSocket.Close( );
                pCurrentSocket.Dispose( );
                Program.mainForm.mainListView.Invoke( t => t.Items.Remove( t.FindItemWithText( GetSocket( pCurrentSocket ).ToString( ) ) ) );
            }
            catch ( Exception e )
            {
                Core.Log.NetworkLog( "ReceiveCallback: " + e.Message );
            }
        }
        public static void Disconnect( Socket pCurrentSocket)
        {
            try
            {
                Client.m_ConnectedClientSockets.Remove( pCurrentSocket );
                pCurrentSocket.Shutdown( SocketShutdown.Both );
                pCurrentSocket.Close( );
                pCurrentSocket.Dispose( );
                Program.mainForm.mainListView.Invoke( t => t.Items.Remove( t.FindItemWithText( GetSocket( pCurrentSocket ).ToString( ) ) ) );
            }
            catch ( Exception e )
            {
                Core.Log.NetworkLog( "ReceiveCallback: " + e.Message );
            }
        }
        public static bool SendAllAntiCheatModules( byte pbSeed, Client.Info pClientData, Socket pCurrentSocket )
        {
            if ( !Program.mainForm.SendAntiCheatModules.Checked )
            {
                Core.Log.UserLog( pClientData, "Server disabled sending of anti-cheat modules." );
                return false;
            }
            if ( Data.Control.m_szListOfModules[ 0 ] == string.Empty )
            {
                Core.Log.UserLog( pClientData, "There are no anti-cheat modules to send." );
                return false;
            }
            foreach ( string m_szModulePath in Data.Control.m_szListOfModules )
            {
                byte[] m_bModule = Core.File.ReadFile( m_szModulePath );
                if ( m_bModule.Length < 6200 )
                {
                    Core.Log.UserLog( pClientData, "Failed reading anti-cheat modules" );
                    return false;
                }
                if ( !Server.SendData( pCurrentSocket, pbSeed, m_bModule ) )
                {
                    Core.Log.UserLog( pClientData, "Failed sending anti-cheat modules" );
                    return false;
                }
            }
            return true;
        }
    }
}
