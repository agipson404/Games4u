#pragma semicolon 1

/* SM Includes */
#include <sourcemod>
#include <sdktools>
#include <socket>
//#include <smac>
#undef REQUIRE_PLUGIN
#tryinclude <updater>

/* GLOBAL VARS */
#define STEAMID_LEN 32

new Handle:h_GetUnauthenticatedClients = INVALID_HANDLE;
new Handle:g_AuthenticatedSteamIDs = INVALID_HANDLE;
new Handle:g_ConnectedClients = INVALID_HANDLE;
new bool:g_ServerLoaded = false;

/* Plugin Info */
public Plugin:myinfo =
{
	name = "Games4U Server Anti-Cheat",
	author = "Vuno",
	description = "Verifies that each player is running Games4u anti-cheat, and attempts to detect cheats from the server.",
	version = "1.0.0",
	url = "http://games4u.se/"
};

public OnPluginStart()
{
	
	h_GetUnauthenticatedClients = CreateTimer( 5.0, GetUnauthenticatedClients, _, TIMER_REPEAT );
	
	// enable socket debugging (only for testing purposes!)
	SocketSetOption(INVALID_HANDLE, DebugMode, 1);
	
	// create a new tcp socket
	new Handle:socket = SocketCreate(SOCKET_TCP, OnSocketError);
	// bind the socket to all interfaces, port 50000
	SocketBind(socket, "0.0.0.0", 50000);
	// let the socket listen for incoming connections
	SocketListen(socket, OnSocketIncoming);
}

public OnSocketIncoming(Handle:socket, Handle:newSocket, String:remoteIP[], remotePort, any:arg) 
{
	PrintToServer("Packet from: %s:%d", remoteIP, remotePort);

	// setup callbacks required to 'enable' newSocket
	// newSocket won't process data until these callbacks are set
	SocketSetReceiveCallback(newSocket, OnChildSocketReceive);
	SocketSetDisconnectCallback(newSocket, OnChildSocketDisconnected);
	SocketSetErrorCallback(newSocket, OnChildSocketError);

	SocketSend(newSocket, "games4u csgo server authenticator\n");
}

public OnSocketError(Handle:socket, const errorType, const errorNum, any:ary) {
	// a socket error occured

	LogError("socket error %d (errno %d)", errorType, errorNum);
	CloseHandle(socket);
}

public OnChildSocketReceive(Handle:socket, String:receiveData[], const dataSize, any:hFile) 
{
	// close the connection/socket/handle if it matches quit
	if (strncmp(receiveData, "quit", 4) == 0) 
	{
		CloseHandle(socket);
	}

	// If the server is not yet loaded, tell the AC loader to try again
	if(!g_ServerLoaded)
	{
		SocketSend( socket, "retry" );
		CloseHandle(socket);
	}

	// Verify the packet contains the client's steamID sent from the AC Loader (Verifier)
	if(!StrContains(receiveData, ":", false))
	{
		PrintToServer("Client didn't send a valid AC packet.");
		SocketSend( socket, "retry" );
		CloseHandle(socket);
	}

	// Check to see if the user's SteamID is already authenticated
	if( !FindStringInArray( g_AuthenticatedSteamIDs, receiveData ) )
	{
		// If not found, add it

		if( !PushArrayString( g_AuthenticatedSteamIDs, receiveData ) )
		{
			SocketSend( socket, "retry" );
			CloseHandle(socket);
		}
	}

	// Send the AC loader back that the user is safe to play on the CSGO server
	SocketSend( socket, "safe" );
	CloseHandle(socket);
}

public OnChildSocketDisconnected(Handle:socket, any:hFile) 
{
	// remote side disconnected

	CloseHandle(socket);
}

public OnChildSocketError(Handle:socket, const errorType, const errorNum, any:ary) {
	// a socket error occured

	LogError("child socket error %d (errno %d)", errorType, errorNum);
	CloseHandle(socket);
}
public Action KickClientDelay(Handle timer, any clientid)
{
	KickClient( clientid, "You were kicked for not running Games4u Anti-Cheat." );
}
public Action GetUnauthenticatedClients(Handle timer)
{
	int numberOfClients = 0;
	int clientSteamID = 0;
	new String:szClientSteamID[STEAMID_LEN];
	new String:szClientName[64];
	
	// Make sure the Server is started
	if( g_ServerLoaded )
	{
		// Get how many clients are currently connected;
		numberOfClients = GetArraySize( g_ConnectedClients );
		if( numberOfClients != 0 )
		{
			// Get each of the connected clients SteamID
			for (new i = 1; i <= numberOfClients; i++ )
			{
				if( IsClientConnected( i ) )
				{
					clientSteamID = GetSteamAccountID( i, true );
					GetClientName( i, szClientName, sizeof(szClientName ) );

					// Search for the client's steamID on the authenticated ID list
					IntToString( clientSteamID, szClientSteamID, sizeof( szClientSteamID ) );
					if( FindStringInArray( g_AuthenticatedSteamIDs, szClientSteamID ) == -1 )
					{
						// If not found, kick him!
						PrintHintText( i, "In order to play on this server, you need to be running Games4u Anti-Cheat which you currently aren't. Please start the anti-cheat and try again." );
						PrintToServer( "Games4u AC: Kicking %s for not running the anti-cheat.", szClientName );
						
						// Create a timer to kick the user in 15 seconds after the alert was displayed
						CreateTimer( 15.0, KickClientDelay, _, i );
						
						// Remove user from array
						RemoveFromArray(g_ConnectedClients, i);
					}
					else
					{
						PrintToServer( "Games4u AC: %s is running the anti-cheat.", szClientName );
					}
				}
				else
				{
					RemoveFromArray(g_ConnectedClients, i);
				}
			}
		}
		else
		{
			PrintToServer( "Games4u AC: no users are currently authenticated" );
		}
	}
	return Plugin_Continue;
}

public void OnMapStart()
{
	PrintToServer("Server loaded.");
	g_ServerLoaded = true;

	g_AuthenticatedSteamIDs = CreateArray(STEAMID_LEN);
	g_ConnectedClients = CreateArray(STEAMID_LEN);
}
public void OnMapEnd()
{
	PrintToServer("Server ended.");
	g_ServerLoaded = false;
	
	// Each time the server/map is ended, clear the array of authenticated steamIDs
	if( GetArraySize( g_AuthenticatedSteamIDs ) != 0 || GetArraySize( g_ConnectedClients ) != 0 )
	{
		ClearArray( g_AuthenticatedSteamIDs );
		ClearArray( g_ConnectedClients );
	}
}

public void OnClientPutInServer(int client)
{
	int usersteamid = 0;

		if( !IsFakeClient(client) )
		{
			usersteamid = GetSteamAccountID(client, true);
			if(usersteamid == 0)
			{
				KickClient( client, "Failed grabbing client's SteamId." );
				return;
			}

			new String:szusersteamid[STEAMID_LEN];
			if(!IntToString( usersteamid, szusersteamid, STEAMID_LEN ))
			{
				KickClient( client, "Failed converting SteamId to int." );
				return;
			}

			new String:userip[16];
			if(!GetClientIP(client, userip, sizeof(userip), true))
			{
				KickClient( client, "Failed grabbing client's IP." );
				return;
			}
			PushArrayString( g_ConnectedClients, szusersteamid );
			PrintToChat(client, "Games4u Server Side Anti-Cheat is loaded.");
		}

	return;
	
}