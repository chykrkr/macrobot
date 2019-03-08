-- click and type enter repeatedly
-- Initial cursor position is remembered and restored to that position before every click

local config = require("module/config_load");

-- make your own setting.lua using setting_sample.lua
local cnf = config.load("setting.lua");
cnf = cnf.setting

--default
twait = 3000
tdelay = 13000

if cnf ~= nil then
	twait = cnf.twait;
	tdelay = cnf.tdelay;
end

print("twait = ", twait, ", tdelay = ", tdelay)

x, y = readcursor();

print("x = ", x, ", y = ", y);

print("Sleeping ...")

sleep(twait)

print("Go")

setcursor(x, y);

print("Left click");
left_click();

hwnd = 0;

while hwnd == 0 do
	sleep(500);
	hwnd = get_foreground_wnd();
	print(string.format("%X", hwnd));
end

sleep(tdelay);

while true do
--	caues error commented out.
--	set_foreground_wnd(hwnd);
	setcursor(x, y);

	if hwnd == get_foreground_wnd() then
		print("Left click");
		left_click();
	else
		print("Enter");
		key("VK_RETURN");
	end

	print("sleep for ", tdelay);
	sleep(tdelay);
end



