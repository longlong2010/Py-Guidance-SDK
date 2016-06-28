import guidance;
import struct;

lock = guidance.create_lock();
event = guidance.create_event();

def callback(data_type, data):
    guidance.enter(lock);
    if data_type == 1:
        print struct.unpack('iifffffff', data);
    guidance.leave(lock);
    guidance.set_event(event);
    return 0;

guidance.reset_config();
guidance.init_transfer();
guidance.select_imu();
guidance.set_sdk_event_handler(callback);
guidance.start_transfer();
for x in range(1, 100):
    guidance.wait_event(event);
guidance.stop_transfer();
guidance.release_transfer();
