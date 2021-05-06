//Provides status information about Replay Mod via black magic TODO implement black magic
namespace NjsTweaks { namespace ReplayModInterop {
    //Call in main.cpp load()
    void initialize();
    //Sanity check of black magic
    bool isReplayModInteropWorking();
    //Is a replay available for the currently selected song/difficulty/characteristic
    bool isReplayAvailableForCurrentSong();
    //Was a new permanent replay created at the end of the last song
    bool isNewReplayCreatedAtEndOfSong();
}}