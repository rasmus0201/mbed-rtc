# RTC = Real-Time Clock for Mbed OS 6

This is a simple library to setup automatic synchronization of `time()` by an online NTP server.

Be aware that the NTP protocol have no idea what timezones is. So timestamps will always be UTC.

### Usage:

|  Method  |  Output Type | Description |
|---|---|---|
| Bundsgaard::Rtc::Rtc(int refreshInterval) |   | Constructor. Interval in **minutes** |
| Bundsgaard::Rtc::Start() |   | Start the RTC |
| Bundsgaard::Rtc::GetNTPTime() | time_t | Get's the latest time from the NTP server |
| Bundsgaard::Rtc::GetTime() | time_t | Get the current system time |
| Bundsgaard::Rtc::GetTimestamp() | uint32_t | Get the current system timestamp |


### Example:

```cpp
#include "Rtc.h"

// Interval is in minutes
#define NTP_REFRESH_INTERVAL 10

int main() {
    Bundsgaard::Rtc* rtc = new Bundsgaard::Rtc(NTP_REFRESH_INTERVAL);
    rtc->Start();

    while(true) {
        time_t t = rtc->GetTime();
        uint32_t timestamp = rtc->GetTimestamp();
        printf("RTC: Clock is %s, Timestamp: %u\n", ctime(&t), timestamp);

        ThisThread::sleep_for(1000ms);
    }
}

```