local M = {}

local function get(from, to)
	local ret = {}

	t1 = os.time(year=1970, month=1, day=1, hour=from.hour, min=from.min, sec=from.sec}
	t2 = os.time(year=1970, month=1, day=1, hour=to.hour, min=to.min, sec=to.sec}
	t3 = math.random(t1, t2)

	print(t1, t2, t3)

	result = os.date("*t", t3)

	ret.sec = result.sec
	ret.min = result.min
	ret.hour = result.hour

	return ret
end

local function between(from_hour, from_min, from_sec, to_hour, to_min, to_sec)
	from = {hour = from_hour, min = from_min, sec = from_sec}
	to = {hour = to_hour, min = to_min, sec = to_sec}

	ret = get(from, to)

	return ret
end

M.between = between

return M
