# RTC = Real-Time Clock for Mbed OS 6

This is a simple library to setup automatic synchronization of `time()` by an online NTP server. This library will synchronize the system time with the NTP server every _X_ minutes on another thread.

Be aware that the NTP protocol have no idea what timezones is. So times/timestamps will always be UTC.

### Installation:

**Mbed Studio:**

1. Copy the git URL of this repo (`https://github.com/rasmus0201/mbed-rtc`)
2. Open the `Libraries` tab and press the "+" button
3. Paste the github URL - press next
4. Use master as the version
5. You are done installing!

### Usage:

|  Method  |  Output Type | Description |
|---|---|---|
| Bundsgaard::Rtc::Rtc(int syncInterval, bool networkless = false) |   | Constructor. Interval in **minutes**. Networkless if the RTC should operate without network. Can be useful for debugging |
| Bundsgaard::Rtc::Start() | RtcErrors (int) | Start the RTC |
| Bundsgaard::Rtc::GetNTPTime() | time_t | Get's the latest time from the NTP server |
| Bundsgaard::Rtc::GetTime() | time_t | Get the current system time |
| Bundsgaard::Rtc::GetTimestamp() | uint32_t | Get the current system timestamp |
| Bundsgaard::Rtc::GetTimestampMS() | uint64_t | Get the current system timestamp in milliseconds |


### Example:

```cpp
#include "Rtc.h"

// Interval is in minutes
#define NTP_SYNC_INTERVAL 10

int main() {    
    Bundsgaard::Rtc* rtc = new Bundsgaard::Rtc(NTP_SYNC_INTERVAL);
    rtc->Start();

    while(true) {
        time_t t = rtc->GetTime();
        uint32_t timestamp = rtc->GetTimestamp();
        printf("RTC: Clock is %s, Timestamp: %u\n", ctime(&t), timestamp);

        ThisThread::sleep_for(1000ms);
    }
}

```
