#include "stdafx.h"
#include "MusicBeeInterface_1_0.h" 
#include "Logitech.h"
#include "State.h"

// DLL needs to be named MB_xxxxxxxx.dll and placed in the Plugins folder where MusicBee is installed
// the plugin will automatically be added to the MusicBee Plugin preferences panel and needs to be explicitly enabled in that panel
namespace MusicBeePlugin 
{ 
	MusicBeeApiInterface mbApiInterface;
	PluginInfo about;
	Logitech * logitech = 0;

	PluginInfo* Initialise(MusicBeeApiInterface* apiInterface)
	{
		mbApiInterface = *apiInterface;
		about.PluginInfoVersion = PluginInfoVersion;
		about.Name = L"Logitech G13, G15, G510, G19 LCD";
		about.Description = L"Add support for Logitech G13, G15, G510, G19 LCD to MusicBee";
		about.Author = L"JimmyD";
		about.TargetApplication = L"MusicBee";  // current only applies to artwork, lyrics or instant messenger name that appears in the provider drop down selector or target Instant Messenger
		about.Type = General;
		about.VersionMajor = 1;  // your plugin version
		about.VersionMinor = 0;
		about.Revision = 0;
		about.MinInterfaceVersion = MinInterfaceVersion;
		about.MinApiRevision = MinApiRevision;
		about.ReceiveNotifications = (ReceiveNotificationFlags)(PlayerEvents | TagEvents );
		about.ConfigurationPanelHeight = 0;  // height in pixels that musicbee should reserve in a panel for config settings. When set, a handle to an empty panel will be passed to the Configure function
		return &about;
	}

	// configuration dialog
	// if this function is removed or false is returned, MusicBee will display an About dialog when the user clicks Configure
	int Configure(void *panelHandle)
	{
		// save any persistent settings in a sub-folder of this path
		//LPCWSTR dataPath = mbApiInterface.Setting_GetPersistentStoragePath();
		// panelHandle will only be set if you set about.ConfigurationPanelHeight to a non-zero value
		// keep in mind the panel width is scaled according to the font the user has selected
		// Panel configPanel = (Panel)Panel.FromHandle(panelHandle);
		// if about.ConfigurationPanelHeight is set to 0, you can display your own popup window
		//...
		//mbApiInterface.MB_ReleaseString(dataPath);
		return (int)false;
	}


	// called by MusicBee when the user clicks Apply or Save in the MusicBee Preferences screen.
	// its up to you to figure out whether anything has changed and needs updating
	void SaveSettings()
	{
		// save any persistent settings in a sub-folder of this path
		//LPCWSTR dataPath = mbApiInterface.Setting_GetPersistentStoragePath();
	}


	// MusicBee is closing the plugin (plugin is being disabled by user or MusicBee is shutting down)
	void Close(PluginCloseReason reason)
	{
		if(logitech != 0)
		{
			delete logitech;
			logitech = 0;
		}
	}

	// uninstall this plugin - clean up any persisted files
	void Uninstall()
	{
	}

	// recieve event notifications from MusicBee
	// you need to set about.ReceiveNotificationFlags = PlayerEvents to receive all notifications, and not just the startup event
	void ReceiveNotification(LPCWSTR sourceFileUrl, NotificationType type)
	{
		// perform some action depending on the notification type
		switch (type)
		{
		case PluginStartup:
			// perform startup initialisation

			if(logitech == 0)
			{
				logitech = new Logitech();

				if(!logitech->OnInitDialog())
				{
					Close(UserDisabled);
				}
			}
			break;

		case PlayStateChanged:
			if(logitech !=0)
			{
				switch (mbApiInterface.Player_GetPlayState())
				{
				case Playing:
					logitech->changeState(StatePlay::Playing);
					if(!logitech->getFirstTime())
					{
						logitech->setPosition(mbApiInterface.Player_GetPosition());
						logitech->setDuration(mbApiInterface.NowPlaying_GetDuration());
					}
					break;
				case Paused:
					if(!logitech->getFirstTime())
					{
						logitech->changeState(StatePlay::Paused);
					}
					break;
				case Stopped:
					if(!logitech->getFirstTime())
					{
						logitech->changeState(StatePlay::Stopped);
					}
					break;
				case Loading:
					if(!logitech->getFirstTime())
					{
						logitech->changeState(StatePlay::Loading);
					}
					break;
				case Undefined:
					if(!logitech->getFirstTime())
					{
						logitech->changeState(StatePlay::Undefined);
					}
					break;
				}
			}
			break;
		case TrackChanged:
			if(logitech !=0)
			{
				LPCWSTR artist = mbApiInterface.NowPlaying_GetFileTag(Artist);
				LPCWSTR album = mbApiInterface.NowPlaying_GetFileTag(Album);
				LPCWSTR title = mbApiInterface.NowPlaying_GetFileTag(TrackTitle);

				logitech->changeArtistTitle((wstring)artist, (wstring)album, (wstring)title, mbApiInterface.NowPlaying_GetDuration(), mbApiInterface.Player_GetPosition());
				mbApiInterface.MB_ReleaseString(artist);
				mbApiInterface.MB_ReleaseString(album);
				mbApiInterface.MB_ReleaseString(title);
			}
			break;
		}
	}

	// return an array of lyric or artwork provider names this plugin supports
	// the providers will be iterated through one by one and passed to the RetrieveLyrics/ RetrieveArtwork function in order set by the user in the MusicBee Tags(2) preferences screen until a match is found
	// the last item on the array needs to be NULL
	LPCWSTR* GetProviders()
	{
		return NULL;
	}

	// return lyrics for the requested artist/title from the requested provider
	// only required if PluginType = LyricsRetrieval
	// dispose the returned string the next time this API is called
	// return NULL if no lyrics are found
	LPCWSTR RetrieveLyrics(LPCWSTR sourceFileUrl, LPCWSTR artist, LPCWSTR trackTitle, LPCWSTR album, bool synchronisedPreferred, LPCWSTR provider)
	{
		return NULL;
	}

	// return Base64 string representation of the artwork binary data from the requested provider
	// only required if PluginType = ArtworkRetrieval
	// dispose the returned string the next time this API is called
	// return NULL if no artwork is found
	LPCWSTR RetrieveArtwork(LPCWSTR sourceFileUrl, LPCWSTR albumArtist, LPCWSTR album, LPCWSTR provider)
	{
		// see http://www.adp-gmbh.ch/cpp/common/base64.html has an example of a base64 encoder
		return NULL;
	}

	// ******** the following apply to Storage plugins, and can be removed for other plugin types ************
	//
	// user initiated refresh (eg. pressing F5) - reconnect/ clear cache as appropriate
	void Refresh()
	{
	}

	// is the server ready
	// you can initially return false and then use MB_SendNotification when the storage is ready (or failed)
	bool IsReady()
	{
		return false;
	}

	// return a 16x16 bitmap for the storage icon
	void* GetIcon()
	{
		return NULL;
	}

	bool FolderExists(LPCWSTR path)
	{
		return true;
	}

	// return the full path of folders in a folder
	LPCWSTR * GetFolders(LPCWSTR path)
	{
		return NULL;
	}

	// this function returns an array of files in the specified folder
	// each file is represented as a array of tags - each tag being a KeyValuePair(Of Byte, String), where Byte is a FilePropertyType or MetaDataType enum value and String is the value
	// a tag for FilePropertyType.Url must be included
	// you can initially return NULL and then use MB_SendNotification when the file data is ready (on receiving the notification MB will call GetFiles(path) again)
	void * GetFiles(LPCWSTR path)
	{
		return NULL;
	}

	bool FileExists(LPCWSTR url)
	{
		return true;
	}

	//  each file is represented as a array of tags - each tag being a KeyValuePair(Of Byte, String), where Byte is a FilePropertyType or MetaDataType enum value and String is the value
	// a tag for FilePropertyType.Url must be included
	void * GetFile(LPCWSTR url)
	{
		return NULL;
	}

	// return an array of bytes for the raw picture data
	void * GetFileArtwork(LPCWSTR url)
	{
		return NULL;
	}

	// return an array of playlist identifiers
	// where each playlist identifier is a KeyValuePair(id, name)
	void * GetPlaylists()
	{
		return NULL;
	}

	// return an array of files in a playlist - a playlist being identified by the id parameter returned by GetPlaylists()
	// each file is represented as a array of tags - each tag being a KeyValuePair(Of Byte, String), where Byte is a FilePropertyType or MetaDataType enum value and String is the value
	// a tag for FilePropertyType.Url must be included
	void * GetPlaylistFiles(LPCWSTR id)
	{
		return NULL;
	}

	// return an IO.Stream that can read through the raw (undecoded) bytes of a music file
	void * GetStream(LPCWSTR url)
	{
		return NULL;
	}

	// return the last error that occurred (Exception object)
	void * GetError()
	{
		return NULL;
	}
}
