#ifndef BUNDSGAARD_RTC_H
#define BUNDSGAARD_RTC_H

#import "mbed.h"

namespace Bundsgaard
{
    enum RtcErrors
    {
        NO_ERROR = 0,
        NO_INTERFACE = 1,
        CONNECTION_FAILED = 2
    };

    class Rtc
    {
        public:
            Rtc(int refreshInterval);
            void Start();
            time_t GetNTPTime();
            time_t GetTime();
            unsigned int GetTimestamp();
        
        private:
            void Run();
            RtcErrors error = Bundsgaard::RtcErrors::NO_ERROR;
            Thread _thread;
            NetworkInterface *net;
            time_t ntpTime = 0;
            int refreshInterval;
    };
}

#endif
