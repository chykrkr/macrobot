local M = {}

local function wait(hour, min, sec, int_sec, vk)
	temp = os.date("*t");

	temp.hour = hour;
	temp.min = min;
	temp.sec = sec;

	t2 = os.time(temp);

	while os.time() < t2 do
		print(os.date("%X %x"), " | ", os.date("%X %x", t2));
		key(vk)
		sleep(int_sec * 1000);
	end
end

local function isCurTimeOverETA(hour, min, sec)
	temp = os.date("*t");

	temp.hour = hour;
	temp.min = min;
	temp.sec = sec;

	t2 = os.time(temp);

	if os.time() > t2 then
		return true
	end

	return false
end

local function rand_sleep(from, to)
	local t = math.random(from, to)

	print(os.date("%X %x"), from, to, t)
	sleep(t)
end

M.wait = wait
M.isCurTimeOverETA = isCurTimeOverETA
M.rand_sleep = rand_sleep


return M
