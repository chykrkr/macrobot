-- wait module.
-- this module can be used to wait until specified time.
local M = {}

function wait(hour, min, sec, int_sec, vk)
	temp = os.date("*t");

	if min == nil then
		min = 0;
	end

	if sec == nil then
		sec = 0;
	end

	if int_sec == nil then
		int_sec = 2;
	end

	temp.hour = hour;
	temp.min = min;
	temp.sec = sec;

	t2 = os.time(temp);

	print("INPUT : ", os.date("%X %x", t2));

	while os.time() < t2 do
		print(os.date("%X %x"), " | ", os.date("%X %x", t2));

		if vk ~= nil then
			print("key : ", vk)
			key(vk)
		end
		sleep(int_sec * 1000);
	end
end

M.wait = wait

return M
