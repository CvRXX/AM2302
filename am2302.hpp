#include <bitset>

namespace oscarIO {
    namespace am2302 {

        namespace detail {
            struct packet {
                unsigned int temp;
                bool tempSign;
                u_int8_t rh;
            };
            template<std::size_t N>
            void reverse(std::bitset<N> &b) {
                for (std::size_t i = 0; i < N / 2; ++i) {
                    bool t = b[i];
                    b[i] = b[N - i - 1];
                    b[N - i - 1] = t;
                }
            }
        }
        auto measure(hwlib::pin_in_out& sensor) {
            auto waitTillState = [&](bool state) {
                for (int i; i < 1000000; i++) {
                    if (sensor.read() == state) {
                        return true;
                    }
                }
                return false;
            };
            auto getBit = [&]() {
                waitTillState(true);
                auto duration = hwlib::now_us(); // get current time
                waitTillState(false);
                duration = hwlib::now_us() - duration; // Calculate time
                return duration > 40;
            };

            std::bitset<40> receivebufferb = 0;
            sensor.direction_set_output();
            sensor.write(false);
            hwlib::wait_ms(1);
            sensor.write(true); // Host pull up
            hwlib::wait_us(30);
            sensor.direction_set_input();
            waitTillState(true);
            waitTillState(false);
            for (int i = 0; i < 40; i++) {
                receivebufferb[i] = getBit();
            }
            std::bitset<16> rh = 0;
            for (int i = 0; i < 16; i++) {
                rh[i] = receivebufferb[i];
            }
            detail::reverse(rh);

            std::bitset<15> temp = 0;
            for (int i = 17; i < 32; i++) {
                temp[i - 17] = receivebufferb[i];
            }
            detail::reverse(temp);

            return detail::packet{static_cast<unsigned int>(temp.to_ulong()),receivebufferb[16], static_cast<unsigned int>(rh.to_ulong())};


        }


    }
}
