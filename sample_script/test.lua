twait = 3000
tdelay = 13000

print("twait = ", twait, ", tdelay = ", tdelay)

print("Sleeping ...")

sleep(twait)

print("Go")

while true do
	print("Left click");
	left_click();

	print("sleep for ", tdelay);
	sleep(tdelay);

	print("Enter");
	key("VK_RETURN");

	print("sleep for ", tdelay);
	sleep(tdelay);
end



