import std.stdio;
import dlangui;

version(Windows) 
{
	import core.sys.windows.windows;
}

mixin APP_ENTRY_POINT;

const string title = "Stick Knights Online Auto Updater";
const string versionURL = "http://www.stickknightsonline.com/version.txt";
__gshared ProgressBarWidget progress_bar; 
__gshared Window progress_window;	

void startDownloadMessage()
{
	import std.string, std.format, std.conv, core.sys.windows.windows;

	
     // create window
    progress_window = Platform.instance.createWindow("Dlang Auto-Updater with Progress", null, 4, 256, 64);

	progress_bar = new ProgressBarWidget();
	progress_bar.progress = 1; // max 1000, so 100% = 1000, 25.1% = 251, etc.
	progress_bar.animationInterval = 16; // 16 milliseconds is about 60FPS for animation / screen refresh.
	
			
	// create some widget to show in window
    progress_window.mainWidget = progress_bar;
		
    // show window
    progress_window.show();

    // run message loop
    Platform.instance.enterMessageLoop();

}

extern (C) int UIAppMain(string[] args)
{
	import core.thread;

	if(isUpdated)
	{
		launch();
		return 0;
	}
	if(!prompt!("Stick Knights Online requires an update. Download and install now?")) return 0;

	auto composed = new Thread(&startDownloadMessage);
    composed.start();

	version(linux) downloadPatch!("patch_linux.zip");
	else version(Windows) downloadPatch!("patch_windows.zip");
	else version(OSX) downloadPatch!("patch_osx.zip");
	else static assert(false, "unsupported platform");
	unzipPatch();

	composed.join();

	if(!prompt!("Update complete! Launch Stick Knights Online?")) return 0;
	
	
	return launch();
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

bool isUpdated() @property
{
	import std.net.curl;
	import core.sys.windows.windows;
    import std.net.curl, std.string;

	try{
		return File("version.txt").readln() == get(versionURL);
	} catch (Throwable) {
		return false; 
	}

}

void display(string message)()
{
    version(Windows)
    {
        import std.string, core.sys.windows.windows;

        MessageBoxA(
					null,
					message.ptr,
					title.ptr,
					MB_OK);
    }
    else 
	{
		writeln("[" + title.toStringz() + "]" + message.toStringz());
	}
}

bool prompt(string message)()
{
    import std.string;
    string promptString = format("%s [Y/n] ", message);
	
    version(Windows)
    {
        auto result = MessageBoxA(
								  NULL,
								  promptString.ptr,
								  "Stick Knights Online (Update Available)".ptr,
								  MB_YESNO);
        return result == IDYES ? true : false;
    }
    else
    {
        import std.algorithm;
        write(promptString);
        char[] actualResponse;
        stdin.readln(actualResponse);
        actualResponse.toLowerInPlace();
        static immutable validResponses = ["y\n", "n\n", "yes\n", "no\n", "\n"];
        while(!any!(a => a == actualResponse)(validResponses))
        {
            writeln("Input must be of the form '[Y/n]'");
            write(promptString);
            stdin.readln(actualResponse);
            actualResponse.toLowerInPlace();
        }
        if(actualResponse == "n\n" || actualResponse == "no\n") return false;
        return true;
    }
}

void downloadPatch(string filename)()
{
	import core.sys.windows.windows;
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

    http.onProgress = (size_t dltotal, size_t dlnow,
                       size_t ultotal, size_t ulnow)
    {
        if (dlnow > 0 && dltotal > 0)
        {
            float progress = to!float(dlnow) / to!float(dltotal);
			progress_bar.progress = to!int(progress * 1000);
        }   
        return 0;
    };
    http.perform();
	progress_bar.progress = 1000;
}

void unzipPatch()
{
    import std.algorithm, std.file, std.zip, std.conv, std.string;

	progress_bar.progress = 0;
	
	auto archive = new ZipArchive(_patch);

	//calculate for progress bar
	int finalKey = archive.directory.keys.length;
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
		
		currentKey++;
		progress_bar.progress = to!int(1000 * currentKey / finalKey);
	}
	
	progress_bar.progress = 1000;
	progress_window.close();
}

private ubyte[] _patch;


