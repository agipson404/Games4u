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
using System.Drawing;

//tv pw: 77riy1
namespace Games4u_Server.Network
{
    public class Server
    {
        public static bool m_ServerIsRunning = false;
        public static TcpListener m_listener;
        public static Thread m_listenThread;
        public const int m_iPort = 8453;
        public static Socket m_ServerSocket;
        public const int _BUFFER_SIZE = 20971520;
        public static readonly byte[] _buffer = new byte[ _BUFFER_SIZE ];

        public static bool SendData(Socket pCurrentSocket, byte pbSeed, string pszData)
        {
            byte[] m_Temp = Encoding.UTF8.GetBytes(pszData);
            Core.Cryption.XOR( pbSeed, m_Temp );
            return Convert.ToBoolean( pCurrentSocket.Send( m_Temp ) );

        }
        public static bool SendData( Socket pCurrentSocket, byte pbSeed, byte[] pszData )
        {
            Core.Cryption.XOR( pbSeed, pszData );
            return Convert.ToBoolean(pCurrentSocket.Send( pszData ));

        }
        public static void ReceiveCallback( IAsyncResult AR )
        {
            Socket m_CurrentSocket = ( Socket )AR.AsyncState;
            ListViewItem m_ItemUsername;

            byte m_bSeed = 0;
            byte m_SizeOfHWID = 0;
            byte []m_dwKernelDebugging = new byte[ 2 ];
            byte []m_dwDbgBreakPoint = new byte [2];
            byte []m_dwDbgUserBreakPoint = new byte[ 2 ];
            int m_SizeOfData = 0;

            Client.Info m_ClientData = new Client.Info( );

            try
            {
                if ( !Program.mainForm.anticheatserverCheckBox.Checked )
                    goto end;

                m_ClientData.szIPAddress = m_CurrentSocket.RemoteEndPoint.ToString( );

                int m_iReceivedData = m_CurrentSocket.EndReceive( AR );
                byte[] m_ReceivedBuffer = new byte[ m_iReceivedData ];
                Array.Copy( _buffer, m_ReceivedBuffer, m_iReceivedData );
                

                if ( m_ReceivedBuffer.Length == 0 )
                {
                    Core.Log.NetworkLog( m_ClientData.szIPAddress + " sent no packets." );
                    goto end;
                }
                if ( m_ReceivedBuffer.Length > 0x1500000 )
                {
                    Core.Log.NetworkLog( m_ClientData.szIPAddress + " sent alot of packets." );
                    goto end;
                }

                Core.Log.NetworkLog( "Packet Size: " + m_ReceivedBuffer.Length.ToString("X") );

                // Fill loader packet strut
                m_bSeed = m_ReceivedBuffer[ 0 ];
                m_SizeOfHWID = m_ReceivedBuffer[ 1 ];
                m_SizeOfHWID ^= m_bSeed;

                byte[] m_TempData = new byte[ m_SizeOfHWID ];
                Buffer.BlockCopy( m_ReceivedBuffer, 2, m_TempData, 0, m_TempData.Length );
                Core.Cryption.XOR( m_bSeed, m_TempData );
                m_ClientData.szRC4Key = Encoding.UTF8.GetString( m_TempData );
                m_ClientData.szHWID = Core.Array.RemoveWhitespace( m_ClientData.szRC4Key );

                m_TempData = new byte[ m_ReceivedBuffer.Length - 2 - m_SizeOfHWID ];
                Buffer.BlockCopy( m_ReceivedBuffer, 2 + m_SizeOfHWID, m_TempData, 0, m_TempData.Length );
                //Core.Cryption.RC4Crypt( m_ClientData.szRC4Key, m_TempData );
                Core.Cryption.XOR( m_bSeed, m_TempData );

                // Get the rest of the packet
                m_ClientData.bPacketType = m_TempData[ 0 ];
                m_SizeOfData = m_TempData[ 1 ];
                m_dwKernelDebugging[ 0 ] = m_TempData[ 2 ];
                m_dwKernelDebugging[ 1 ] = m_TempData[ 3 ];
                m_dwDbgBreakPoint[0] = m_TempData[4];
                m_dwDbgBreakPoint[1] = m_TempData[5];
                m_dwDbgUserBreakPoint[0] = m_TempData[6];
                m_dwDbgUserBreakPoint[1] = m_TempData[7];
                string m_szDataReceived = Encoding.UTF8.GetString( m_TempData );
                m_ClientData.szUsername = Core.Array.GetTextBetween( m_szDataReceived, "UE?", "EE?" );
                m_ClientData.szPassword = Core.Array.GetTextBetween( m_szDataReceived, "EE?", "PEP?" );
                m_ClientData.szDataReceived = Core.Array.GetTextAfter( m_szDataReceived, "PEP?" );

                /*
                 * Anti-debugging checks.
                 * If any of the anti-debugging attempts is altered, flag the user for ban
                */
                
                if ( m_dwKernelDebugging[ 0 ] == 0x01 && m_dwKernelDebugging[ 1 ] == 0x00 )
                {
                    Core.Log.SetUserFlagMode( m_ClientData, "ModifiedLoader" );
                    Core.Log.UserLog( m_ClientData, "The user is kernel debugging." );
                    SendData( m_CurrentSocket, m_bSeed, "Error Code: X1" );
                    Client.Disconnect( m_CurrentSocket, m_ClientData );
                    return;
                }

                if ( m_dwDbgBreakPoint[ 0 ] != 0x0F || m_dwDbgBreakPoint[ 1 ] != 0x0B 
                    || m_dwDbgUserBreakPoint[ 0 ] != 0x0F || m_dwDbgUserBreakPoint[ 1 ] != 0x0B )
                {
                    Core.Log.SetUserFlagMode( m_ClientData, "ModifiedLoader" );
                    Core.Log.UserLog( m_ClientData, "DbgBreakPoint or DbgUserBreakPoint was modified on the user's client." );
                    SendData( m_CurrentSocket, m_bSeed, "Error Code: X2" );
                    Client.Disconnect( m_CurrentSocket, m_ClientData );
                    return;
                }
                
                if ( m_ClientData.szUsername.Length < 3 || m_ClientData.szPassword.Length < 3 )
                {
                    Core.Log.SetUserFlagMode( m_ClientData, "ModifiedLoader" );
                    Core.Log.UserLog( m_ClientData, "Username or password is below the minimum requirement." );
                    SendData( m_CurrentSocket, m_bSeed, "Either username or password doesn't meet the minimum character requirement." );
                    Client.Disconnect( m_CurrentSocket, m_ClientData );
                    return;
                }
                else if ( m_ClientData.szUsername.Length > 18 || m_ClientData.szPassword.Length > 18 )
                {
                    Core.Log.SetUserFlagMode( m_ClientData, "ModifiedLoader" );
                    Core.Log.UserLog( m_ClientData, "Username or password is above the maximum requirement." );
                    SendData( m_CurrentSocket, m_bSeed, "Either username or password exceeds the maximum character requirement." );
                    Client.Disconnect( m_CurrentSocket, m_ClientData );
                    return;
                  
                }
                else if (m_ClientData.szHWID.Length < 6 )
                {
                    Core.Log.SetUserFlagMode( m_ClientData, "ModifiedLoader" );
                    Core.Log.UserLog( m_ClientData, "User's HWID is invalid." );
                    SendData( m_CurrentSocket, m_bSeed, "Error Code: X3" );
                    Client.Disconnect( m_CurrentSocket, m_ClientData );
                    return;
                }

                // Verify the user crenditals on site
                
                string m_szReturnString = Network.Site.LogUserIn( m_ClientData.szUsername, m_ClientData.szPassword, out m_ClientData.szSiteUserId );
                if ( m_szReturnString.Contains( "suspended" ) )
                {
                    // If the user is banned on the site, ban them on the server
                    Core.Log.UserLog( m_ClientData, "Transferring site ban to anti-cheat server ban..." );
                    Core.Log.BanUser( m_ClientData, "SiteBan" );
                    SendData( m_CurrentSocket, m_bSeed, m_ClientData.szUsername + " is not allowed to play on the servers." );
                    Client.Disconnect( m_CurrentSocket, m_ClientData );
                    return;
                }
                else if ( m_szReturnString.Contains( "ncorrec" ) )
                {
                    // If the user crenditals were invalid
                    Core.Log.NetworkLog( m_ClientData.szUsername + " login failed due to invalid credentials." );
                    SendData( m_CurrentSocket, m_bSeed, "Failed to login due to invalid crenditals." );
                    Client.Disconnect( m_CurrentSocket, m_ClientData );
                    return;
                }
                else if ( m_szReturnString.Contains( "success" ) )
                {
                    // Get the user's steamID
                    Site.GetUserSteamId( m_ClientData.szSiteUserId, out m_ClientData.szSteamId );
                    if ( m_ClientData.szSteamId.Contains( "error" ))
                    {
                        Core.Log.NetworkLog( m_ClientData.szUsername + " no steamID was found for the user." );
                        SendData( m_CurrentSocket, m_bSeed, "Please set your steamID on the site." );
                        Client.Disconnect( m_CurrentSocket, m_ClientData );
                        return;
                    }
                    else if ( m_ClientData.szSteamId.Contains( "" ) )
                    {
                        Core.Log.NetworkLog( m_ClientData.szUsername + " site ID is invalid." );
                        SendData( m_CurrentSocket, m_bSeed, "Retrieved site ID is invalid. Report to administrators." );
                        Client.Disconnect( m_CurrentSocket, m_ClientData );
                        return;
                    }

                    if ( m_ClientData.bPacketType == 1 )
                    {
                        // Disconnect packet
                        Core.Log.UserLog( m_ClientData, "User is disconnecting..." );
                        Client.Disconnect( m_CurrentSocket, m_ClientData );
                        return;
                    }
                    else if ( m_ClientData.bPacketType == 2 )
                    {
                        if ( !Client.IsClientAlreadyConnected( m_ClientData ) )
                        {
                            // Create the user a folder if they don't already have one
                            if ( !Directory.Exists( Core.Log.m_szUserData + "\\" + m_ClientData.szUsername ) )
                            {
                                Directory.CreateDirectory( Core.Log.m_szUserData + "\\" + m_ClientData.szUsername );
                                Core.Log.UserLog( m_ClientData, "First time running anti-cheat. Created the user a folder." );
                            }
                            Client.m_ConnectedClientSockets.Add( m_CurrentSocket );

                            int id = Client.m_ConnectedClientSockets.Count - 1;
                            m_ItemUsername = new ListViewItem( id.ToString( ) );
                            Program.mainForm.mainListView.Invoke( t => t.Items.Add( m_ItemUsername ) );
                            Program.mainForm.mainListView.Invoke( t => m_ItemUsername.SubItems.Add( m_ClientData.szIPAddress ) );
                            Program.mainForm.mainListView.Invoke( t => m_ItemUsername.SubItems.Add( m_ClientData.szUsername ) );
                            Program.mainForm.mainListView.Invoke( t => m_ItemUsername.SubItems.Add( m_ClientData.szHWID ) );

                            string m_szFlagMode = Core.Log.GetUserFlagMode( m_ClientData );
                            if ( m_szFlagMode != string.Empty )
                            {
                                Program.mainForm.mainListView.Invoke( t => m_ItemUsername.SubItems.Add( m_szFlagMode ) );
                            }

                            byte[] m_ServerStatus = { Convert.ToByte( Program.mainForm.anticheatserverCheckBox.Checked ), Convert.ToByte( Program.mainForm.SendAntiCheatModules.Checked ) };
                            SendData( m_CurrentSocket, m_bSeed, m_ServerStatus );
                            Core.Log.UserLog( m_ClientData, "User was added to the server." );
                            goto restart;
                        }
                        else
                        {
                            byte[] m_ServerStatus = { Convert.ToByte( Program.mainForm.anticheatserverCheckBox.Checked ), Convert.ToByte( Program.mainForm.SendAntiCheatModules.Checked ) };
                            SendData( m_CurrentSocket, m_bSeed, m_ServerStatus );
                            Core.Log.UserLog( m_ClientData, "User was added to the server." );
                            goto restart;
                        }
                    }
                    else if ( m_ClientData.bPacketType == 3 )
                    {
                        Client.SendAllAntiCheatModules( m_bSeed, m_ClientData, m_CurrentSocket );
                        Core.Log.UserLog( m_ClientData, "Sent user anti-cheat modules." );
                        goto restart;
                    }
                    else if ( m_ClientData.bPacketType == 4 )
                    {
                        if ( m_TempData.Length < 700 )
                        {
                            SendData( m_CurrentSocket, m_bSeed, "Retry" );
                            Core.Log.UserLog( m_ClientData, "Invalid screenshot size!" );
                            goto restart;
                        }

                        byte[] m_JPEGSig = { 0xFF, 0xD8, 0xFF, 0xE0, 0x00 };
                        int m_iOffset = Core.Array.FindBytes( m_TempData, m_JPEGSig );
                        if ( m_iOffset == 0 )
                        {
                            SendData( m_CurrentSocket, m_bSeed, "Retry" );
                            Core.Log.UserLog( m_ClientData, "Invalid screenshot data!" );
                            goto restart;
                        }

                        byte[] m_TempScreen = new byte[ m_TempData.Length - m_iOffset ];
                        Buffer.BlockCopy( m_TempData, m_iOffset, m_TempScreen, 0, m_TempScreen.Length - m_iOffset );

                        Core.Log.UserScreenshot( m_ClientData, m_TempScreen );
                        SendData( m_CurrentSocket, m_bSeed, "Thanks" );
                        goto restart;
                    }
                }
            restart:
                m_CurrentSocket.BeginReceive( Server._buffer, 0, Server._BUFFER_SIZE, SocketFlags.None, ReceiveCallback, m_CurrentSocket );
            }
            catch ( Exception e )
            {
                Core.Log.NetworkLog( "ReceiveCallback: " + e.Message );
                Client.Disconnect( m_CurrentSocket, m_ClientData );
            }
        end:
            return;
        }
        private static void AcceptCallback( IAsyncResult AR )
        {
            Socket socket;

            try
            {
                socket = m_ServerSocket.EndAccept( AR );
                socket.ReceiveTimeout = 11000; // If no packet has been sent in 11 seconds, time out
                socket.BeginReceive( _buffer, 0, _BUFFER_SIZE, SocketFlags.None, ReceiveCallback, socket );
                m_ServerSocket.BeginAccept( AcceptCallback, null );

            }
            catch ( Exception e )
            {
                Core.Log.NetworkLog( "AcceptCallback: " + e.Message );
            }
            return;
        }
        public static void Stop( )
        {
            try
            {
                foreach ( Socket socketz in Client.m_ConnectedClientSockets )
                {
                    Core.Log.NetworkLog( "Stopping anti-cheat server..." );
                    Client.Disconnect( socketz);
                }
            }
            catch ( SocketException e )
            {
                Core.Log.NetworkLog( "Error - " + e.Message );
            }
            Core.Log.NetworkLog( "Anti-cheat server has been stopped." );
        }

        public static void Start( )
        {
            try
            {
                Core.Log.NetworkLog( "Running anti-cheat server..." );

                m_ServerSocket = new Socket( AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp );
                m_ServerSocket.Bind( new IPEndPoint( IPAddress.Any, m_iPort ) );
                m_ServerSocket.Listen( 500 );
                m_ServerSocket.BeginAccept( AcceptCallback, null );
            }
            catch ( SocketException e )
            {
                Core.Log.NetworkLog( "Error - " + e.Message );
            }
            Core.Log.NetworkLog( "Server is running." );
        }
    }
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                