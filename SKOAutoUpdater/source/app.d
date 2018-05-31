import std.stdio;
import dlangui;

mixin APP_ENTRY_POINT;

//Download the contents fo this file via CURL
const string versionURL = "http://www.stickknightsonline.com/version.txt";

//Text to display on the Update/Launcher window
const dstring title = "Stick Knights Online Portal"d;
const dstring txtUpdate = "Download and Install Update"d;
const dstring txtLaunch = "Launch Stick Knights Online"d;
const dstring txtUpdateAvailable = "An update is required."d;
const dstring txtDownloadingUpdate = "[1/2] Downloading update..."d;
const dstring txtInstallingUpdate = "[2/2] Installing update..."d;
const dstring txtDownloadProgress = "Download Progress"d;
const dstring txtInstallProgress = "Update Progress"d;
const dstring txtUpToDate = "Stick Knights Online is up to date."d;

__gshared ProgressBarWidget download_progress_bar, install_progress_bar;
__gshared Button btnUpdateLaunch;
__gshared TextWidget twUpdateLaunch;
__gshared Window progress_window;
__gshared bool isUpdated;
private ubyte[] _patch;

extern (C) int UIAppMain(string[] args)
{
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
        version(linux) downloadPatch!("patch_linux.zip");
        else version(Windows) downloadPatch!("patch_windows.zip");
        else version(OSX) downloadPatch!("patch_osx.zip");
        else static assert(false, "unsupported platform");

        // Unzip patch
        twUpdateLaunch.text = txtInstallingUpdate;
        unzipPatch();

        //Update is complete! Set state to launch game
        twUpdateLaunch.text = txtUpToDate;
        btnUpdateLaunch.text = txtLaunch;
        btnUpdateLaunch.enabled = true;
        isUpdated = true;
    }
}

int startLauncherWindow()
{

	import std.string, std.format, std.conv, core.thread, core.sys.windows.windows;
	
    // First set isUpdated by checking the Internet for the latest version
    // Later, after a patch is installed, set isUpdated to true.
    isUpdated = isLatestVersion;

     // create window
    progress_window = Platform.instance.createWindow(title, null, 4, 480, 320);
    auto vlayout = new VerticalLayout();
    vlayout.margins = 0;
    vlayout.padding = 10;
    vlayout.backgroundColor = 0xF7FAFC; // Very light steel blue

    if (!isUpdated)
    {
        // Download Progress Bar
        vlayout.addChild(new TextWidget(null, txtDownloadProgress));
        download_progress_bar = new ProgressBarWidget();
        download_progress_bar.progress = 1; // max 1000, so 100% = 1000, 25.1% = 251, etc.
        download_progress_bar.animationInterval = 33; // 33 milliseconds is about 30FPS for animation / screen refresh.
        vlayout.addChild(download_progress_bar);

        // Install Progress Bar
        vlayout.addChild(new TextWidget(null, txtInstallProgress));
        install_progress_bar = new ProgressBarWidget();
        install_progress_bar.progress = 1; // max 1000, so 100% = 1000, 25.1% = 251, etc.
        install_progress_bar.animationInterval = 33; // 33 milliseconds is about 30FPS for animation / screen refresh.
        vlayout.addChild(install_progress_bar);
    }

    // Update and Launch button
    twUpdateLaunch = new TextWidget(null, isUpdated ? txtUpToDate : txtUpdateAvailable);
    vlayout.addChild(twUpdateLaunch);
    btnUpdateLaunch = new Button("btnUpdateLaunch", isUpdated ? txtLaunch : txtUpdate);
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


