import std.stdio;
import dlangui;

mixin APP_ENTRY_POINT;

//Download the contents fo this file via CURL
const string versionURL = "http://www.stickknightsonline.com/version.txt";
const string patchNotesURL = "http://www.stickknightsonline.com/patchNotes.txt";
const string newsFeedURL = "http://twitrss.me/twitter_user_to_rss/?user=StickKnights";

//Text to display on the Update/Launcher window
const dstring txtPortalTitle = "Stick Knights Online Portal"d;
const dstring txtUpdate = "Download and Install Update"d;
const dstring txtLaunch = "Launch Stick Knights Online"d;
const dstring txtUpdateAvailable = "An update is required."d;
const dstring txtDownloadingUpdate = "[1/2] Downloading update..."d;
const dstring txtInstallingUpdate = "[2/2] Installing update..."d;
const dstring txtDownloadProgress = "Download Progress"d;
const dstring txtInstallProgress = "Update Progress"d;
const dstring txtUpToDate = "Stick Knights Online is up to date."d;
const dstring txtViewInBrowser = "Click to view in browser"d;
//colors
enum colorUpdateRequired = 0xFF101F;    //Red
enum colorDownloadProgress = 0xf26000;  //Dark Orange
enum colorUpdateComplete = 0x4080ff;    //Blue
enum colorNewsUpdatesBackground = 0xFAFBFC;  //Light Steel Blue
enum colorWindowBackground = 0xFFFFFF;  //Pure White
enum colorLink = 0x003FFF; //Bright Blue

//Varaiables used for multi-threading communication
__gshared ProgressBarWidget download_progress_bar, install_progress_bar;
__gshared Button btnUpdateLaunch;
__gshared TextWidget twUpdateLaunch;
__gshared Window progress_window;
__gshared bool isUpdated;
__gshared dstring txtPatchNotes;
__gshared string txtNewsFeed;
__gshared NewsFeedItem[] newsFeedItems;

private ubyte[] _patch;

struct NewsFeedItem
{
    dstring title;
    dstring pubDate;
    dstring link;
}


extern (C) int UIAppMain(string[] args)
{
    //Download some meta data first to display in the update screen
    import std.net.curl, std.string, std.conv, std.xml, std.stdio;

    //Download rss feed of news
    try {
        txtNewsFeed = to!string(get(newsFeedURL));
    } catch (Throwable) {
        txtNewsFeed = "<xml><rss><channel><item><title>Could not download latest News.</title><description>Could not download news!</description><pubDate>Fri, 24 Mar 2017 19:45:11 +0000</pubDate><link>http://www.stickknightsonline.com/</link></item></channel></rss></xml>";
    }

    //Download plain text patch notes.
    try {
        txtPatchNotes = to!dstring(get(patchNotesURL)).replace("\r", "");
    } catch (Throwable) {
        txtPatchNotes = "Unable to download or parse patch notes."d;
    }


    //parse newsFeed
    try {
        //Parse XML feed if successful
        auto xml = new DocumentParser(txtNewsFeed);

        xml.onStartTag["item"] = (ElementParser xml)
        {
            NewsFeedItem item;
            
            //parse each tag one at a time and add it to a struct NewsFeedItem
            xml.onEndTag["title"]   = (in Element e) 
            { 
                item.title   = to!dstring(e.text()); 
            };
            xml.onEndTag["pubDate"] = (in Element e) 
            { 
                item.pubDate = to!dstring(e.text()[0 .. 17]); 
            };
            xml.onEndTag["link"]    = (in Element e) 
            { 
                item.link    = to!dstring(e.text()); 
            };

            xml.parse();

            //Add to the list of items in the News Feed
            newsFeedItems ~= item;
        };
        xml.parse();
    } catch (Throwable) {
        txtPatchNotes = "Unable to download or parse patch notes."d;
    }

	return startLauncherWindow();
}

void UpdateOrLaunch()
{
    if (isUpdated) {
        launch();
        progress_window.close();
    }  else  {
        //Disable Download button
        btnUpdateLaunch.enabled = false;

        // Download patch
        twUpdateLaunch.text = txtDownloadingUpdate;
        twUpdateLaunch.textColor(colorDownloadProgress);
        version(linux) downloadPatch!("patch_linux.zip");
        else version(Windows) downloadPatch!("patch_windows.zip");
        else version(OSX) downloadPatch!("patch_osx.zip");
        else static assert(false, "unsupported platform");

        // Unzip patch
        twUpdateLaunch.text = txtInstallingUpdate;
        unzipPatch();

        //Update is complete! Set state to launch game
        twUpdateLaunch.text = txtUpToDate;
        twUpdateLaunch.textColor(colorUpdateComplete);
        btnUpdateLaunch.text = txtLaunch;
        btnUpdateLaunch.enabled = true;
        isUpdated = true;
    }
}

int startLauncherWindow()
{
	import std.string, std.format, std.conv, core.thread, std.process;

    // First set isUpdated by checking the Internet for the latest version
    // Later, after a patch is installed, set isUpdated to true.
    isUpdated = isLatestVersion;

    // create window
    progress_window = Platform.instance.createWindow(txtPortalTitle, null, WindowFlag.Modal | WindowFlag.ExpandSize, 100, 100);
    auto vlayout = new VerticalLayout();
         vlayout.padding = 10;
         vlayout.backgroundColor = colorWindowBackground;

    // news  / changelog side by side in scroll bar boxes
    auto hlayout = new HorizontalLayout();
    auto labelNews = new TextWidget(null, "Stick Knights Online News"d);
         labelNews.fontSize(24).layoutWidth(464).alignment(Align.Center);
    auto labelUpdates = new TextWidget(null, "Updates and Patch Notes"d);
         labelUpdates.fontSize(24).layoutWidth(464).alignment(Align.Center);
    auto titles = new HorizontalLayout();
         titles.addChild(labelNews);
         titles.addChild(labelUpdates);

    //News Section
    auto scrollNews = new ScrollWidget("scrollNews", ScrollBarMode.Invisible, ScrollBarMode.Visible);
         scrollNews.layoutWidth(464).layoutHeight(420);
         scrollNews.backgroundColor = colorNewsUpdatesBackground;
    auto newsContent = new VerticalLayout();
         newsContent.layoutWidth(432).layoutHeight(4200).margins(10);

         int i = 0;
    foreach (NewsFeedItem item; newsFeedItems)
    {
        import std.conv;

        auto date = new MultilineTextWidget(null, item.pubDate);
             date.fontWeight(FontWeight.Bold);
        auto title = new MultilineTextWidget(null, item.title);
        auto link = new Button("btnLink" ~ to!string(i), txtViewInBrowser);
             link.fontItalic(true);
             link.styleId = STYLE_TEXT;
             link.textColor(colorLink);
        link.click = delegate(Widget src) {
            browse(to!string(item.link));
            return true;
        };

        //Add each part of this news element in order
        auto dateAndLink= new HorizontalLayout();
        dateAndLink.addChild(date);
        dateAndLink.addChild(link);

        newsContent.addChild(dateAndLink);
        newsContent.addChild(title);
        newsContent.addChild(new TextWidget(null, ""d));
        newsContent.addChild(new TextWidget(null, ""d));
    }

    scrollNews.contentWidget(newsContent);

    //Changelog / Updates section
    auto scrollUpdates = new ScrollWidget("scrollUpdates", ScrollBarMode.Invisible, ScrollBarMode.Visible);
         scrollUpdates.layoutWidth(464).layoutHeight(420);
         scrollUpdates.backgroundColor = colorNewsUpdatesBackground;
    auto updatesContent = new VerticalLayout();
         updatesContent.layoutWidth(432).layoutHeight(4200).margins(10);
         updatesContent.addChild(new MultilineTextWidget(null, txtPatchNotes));
    scrollUpdates.contentWidget(updatesContent);

    //Add horizontal layout content panes to vertical layout
    hlayout.addChild(scrollNews);
    hlayout.addChild(scrollUpdates);
    vlayout.addChild(titles);
    vlayout.addChild(hlayout);

    //Status message above progress bars
    twUpdateLaunch = new TextWidget(null, isUpdated? txtUpToDate : txtUpdateAvailable);

    if (!isUpdated)
    {
        twUpdateLaunch.fontSize(18);
        twUpdateLaunch.textColor(colorUpdateRequired);
        vlayout.addChild(twUpdateLaunch);

        // Download Progress Bar
        vlayout.addChild(new TextWidget(null, txtDownloadProgress));
        download_progress_bar = new ProgressBarWidget();
        download_progress_bar.minHeight(32);
        download_progress_bar.progress = 1; // max 1000, so 100% = 1000, 25.1% = 251, etc.
        download_progress_bar.animationInterval = 33; // 33 milliseconds is about 30FPS for animation / screen refresh.
        vlayout.addChild(download_progress_bar);

        // Install Progress Bar
        vlayout.addChild(new TextWidget(null, txtInstallProgress));
        install_progress_bar = new ProgressBarWidget();
        install_progress_bar.minHeight(32);
        install_progress_bar.progress = 1; // max 1000, so 100% = 1000, 25.1% = 251, etc.
        install_progress_bar.animationInterval = 33; // 33 milliseconds is about 30FPS for animation / screen refresh.
        vlayout.addChild(install_progress_bar);
    }

    // Update and Launch button
    btnUpdateLaunch = new Button("btnUpdateLaunch", isUpdated ? txtLaunch : txtUpdate);
    btnUpdateLaunch.minHeight(64);
    btnUpdateLaunch.click = delegate(Widget src) {
        auto composed = new Thread(&UpdateOrLaunch);
        composed.start();

        return true;
    };
    vlayout.addChild(btnUpdateLaunch);

    // show window
    progress_window.mainWidget = vlayout;
    progress_window.show();
    Platform.instance.enterMessageLoop();

    // exit program when finished!
    return 0;
}

int launch()
{
	version(Posix)
	{
		import core.sys.posix.unistd : execv;
		enum exe = "Stick-Knights";
		execv(exe.ptr, [exe.ptr, null].ptr);
		import std.string;
		assert(false, format("Failed to launch %s", exe));
	}
	else version(Windows)
	{
		import core.sys.windows.windows;
		STARTUPINFOA startinfo;
		startinfo.cb = startinfo.sizeof;
		PROCESS_INFORMATION pi;
		CreateProcessA (
						"Stick-Knights.exe".ptr,
						null,
						null,
						null,
						false,
						0,
						null,
						null,
						&startinfo,
						&pi);
	}
	else static assert(false, "unsupported platform");
	
	return 0;
}

bool isLatestVersion() @property
{
    import std.net.curl, std.string;

	try{
		return File("version.txt").readln() == get(versionURL);
	} catch (Throwable) {
		return false; 
	}
}

void downloadPatch(string filename)()
{
    import std.net.curl, std.string, std.conv;


	enum url = format("http://www.stickknightsonline.com/%s", filename);
    //_patch = get!(HTTP, ubyte)(url);


    auto http = HTTP(url);
    http.method = HTTP.Method.get;
    http.onReceive = (ubyte[] data) 
    { 
        _patch ~= data;
        return data.length; 
    };

    //download_progress_bar.progress = 500;
    http.onProgress = (size_t dltotal, size_t dlnow,
                       size_t ultotal, size_t ulnow)
    {
        if (dlnow > 0 && dltotal > 0)
			download_progress_bar.progress = to!int((to!float(dlnow) / to!float(dltotal)) * 1000); 
        return 0;
    };
    http.perform();
}

void unzipPatch()
{
    import std.algorithm, std.file, std.zip, std.conv, std.string;

	install_progress_bar.progress = 0;
	auto archive = new ZipArchive(_patch);
	int currentKey = 0;
	
    foreach(e; archive.directory.keys.sort())
	{
		string name = archive.directory[e].name;
		auto dirs = name.split("/");
		string createDir = "";

		for (int i = 0; i < dirs.length - 1; i++)
		{
			createDir = format("%s%s%s", createDir, createDir == "" ? "" : "/", dirs[i]);
			if (!createDir.exists)
				mkdir(createDir);
		}

		write(e, archive.expand(archive.directory[e]));
		install_progress_bar.progress = to!int(1000 * currentKey++ / archive.directory.keys.length);
	}
    install_progress_bar.progress = 1000;
}


