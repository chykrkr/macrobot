local M = {}

local default = {}

local function load_config(path)
	local config = {}
	local f, err = loadfile(path, "t", config)

	if f == nil then
		print(err)
		config = default
	else
		f()
	end

	return config
end

M.load = load_config

return M
