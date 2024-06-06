/*
* C++ collection that handles most of every call to foobar itself
* make sure that when you call these you're doing them on the main thread
* foobar really hates it when you don't, in the sense that it'll off itself lol
*/


#include "player_functions.h"
#include "stdafx.h"
#include <foobar2000/SDK/foobar2000.h>


void PlayFunction() {
    static_api_ptr_t<playback_control> pbc;
    pbc->play_or_pause();
}

void NextFunction() {
    static_api_ptr_t<playback_control> pbc;
    pbc->next();
}

void PreviousFunction() {
    static_api_ptr_t<playback_control> pbc;
    pbc->previous();
}

void VolumeUpFunction() {
    static_api_ptr_t<playback_control> pbc;
    pbc->volume_up();
}

void VolumeDownFunction() {
    static_api_ptr_t<playback_control> pbc;
    pbc->volume_down();
}

void GetSongInfo() {
    metadb_handle_ptr now_playing;
    static_api_ptr_t<playback_control> pbc;
    pbc->get_now_playing(now_playing);
    
    if (pbc->is_playing()) {
        metadb_handle_ptr track_handle;
        if (pbc->get_now_playing(track_handle)) {
            metadb_info_container::ptr track_info;
            track_handle->get_info_ref(track_info);
            
            const file_info& info = track_info->info();

            const char* title = info.meta_get("title", 0);
            const char* artist = info.meta_get("artist", 0);
            const char* album = info.meta_get("album", 0);

            console::print("Currently Playing Track Info:");
            console::printf("Title: %s", title ? title : "Unknown");
            console::printf("Artist: %s", artist ? artist : "Unknown");
            console::printf("Album: %s", album ? album : "Unknown");
        }
    }
    
}
