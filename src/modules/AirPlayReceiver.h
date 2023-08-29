#ifndef AIRPLAY_RECEIVER_H
#define AIRPLAY_RECEIVER_H

#include <shairplay/shairplay.h>

class AirPlayReceiver {
public:
    AirPlayReceiver(const char* name, const char* password, const char* hwaddr) {
        shairplay_cfg cfg = {
            .hwaddr = hwaddr,
            .password = password,
            .name = name,
            .on_volume = onVolumeChange,
            .userdata = this
        };
        airplay = shairplay_init(&cfg);
    }

    ~AirPlayReceiver() {
        shairplay_destroy(airplay);
    }

    void start() {
        shairplay_start(airplay);
    }

    void stop() {
        shairplay_stop(airplay);
    }

private:
    static void onVolumeChange(void* userdata, float volume) {
        AirPlayReceiver* receiver = static_cast<AirPlayReceiver*>(userdata);
        // Handle volume change here
    }

    shairplay_t* airplay;
};

#endif // AIRPLAY_RECEIVER_H
