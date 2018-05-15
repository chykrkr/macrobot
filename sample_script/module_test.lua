local mwait = require("wait");

temp = os.time();
temp = temp + 5;

temp = os.date("*t", temp);

-- wait until 5 seconds after, send VK 0x07 every 1 seconds
mwait.wait(temp.hour, temp.min, temp.sec, 1, "07");